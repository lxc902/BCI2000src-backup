////////////////////////////////////////////////////////////////////////////////
// $Id: TelnetServer.cpp 6562 2022-02-12 16:32:17Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A simple telnet server that runs inside a BCI2000
//   operator module.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2022: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "TelnetServer.h"

#include "BCIException.h"
#include "ScriptInterpreter.h"
#include "Streambuf.h"
#include "VersionInfo.h"

#include <iostream>
#include <string>

#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32

class TelnetServer::Session : private ScriptInterpreter, private Thread
{
  public:
    Session(TelnetServer *parent);
    ~Session();

    // Thread interface
    int OnExecute();
    // ScriptInterpreter interface
    void OnScriptError(const std::string &);
    // CommandInterpreter interface
    static bool OnWriteLine(void *, const std::string &);
    static bool OnReadLine(void *, std::string &);

    // Input
    Session &ReadCharacter();
    // Output
    Session &WriteHello();
    Session &Write(const std::string &);
    Session &WriteNewline();
    Session &WritePrompt();

    void AbortAndWait();

  private:
    std::atomic<TelnetServer *> mpParent = nullptr;
    std::string mLineBuffer;
    TCPSocket mSocket;
    BufferedIO mBuffer;
    std::iostream mStream;
};

// TelnetServer
TelnetServer::TelnetServer(class StateMachine &inStateMachine, const std::string &inAddress)
    : mrStateMachine(inStateMachine)
{
    mListeningSocket.SetTCPNodelay(true);
    mListeningSocket.Open(inAddress);
    if (!mListeningSocket.IsOpen())
        throw bciexception << "TelnetServer: Cannot listen at " << inAddress;
    Thread::Start();
}

TelnetServer::~TelnetServer()
{
    mListeningSocket.Close();
    Thread::TerminateAndWait();
    for (auto pSession : mSessions)
        pSession->AbortAndWait();
}

int TelnetServer::OnExecute()
{
    Waitables waitables;
    waitables.Add(mListeningSocket);
    waitables.Add(mDeletionQueue);
    const Waitable* pWaitable = nullptr;
    while ((pWaitable = waitables.Wait())) {
        if (pWaitable == &mListeningSocket) {
            mSessions.push_back(new Session(this));
        }
        else if (pWaitable == &mDeletionQueue) {
            SynchronizedQueue<Session*>::Consumable consumable = mDeletionQueue.Consume();
            Session* pSession = *consumable;
            auto i = std::find(mSessions.begin(), mSessions.end(), pSession);
            if (i != mSessions.end()) {
                mSessions.erase(i);
                pSession->AbortAndWait();
                delete pSession;
            }
        }
    }
    return 0;
}

// TelnetServer::Session
TelnetServer::Session::Session(TelnetServer *pParent)
    : Thread(false), ScriptInterpreter(pParent->mrStateMachine), mpParent(pParent), mStream(&mBuffer)
{
    mSocket.SetBlockingMode(false);
    mBuffer.SetIO(&mSocket);
    ScriptInterpreter::WriteLineHandler(&OnWriteLine, this);
    ScriptInterpreter::ReadLineHandler(&OnReadLine, this);
    mpParent.load()->mListeningSocket.WaitForAccept(mSocket, 0);
    Thread::Start();
}

TelnetServer::Session::~Session()
{
}

void TelnetServer::Session::AbortAndWait()
{
    ScriptInterpreter::Abort();
    Thread::TerminateAndWait();
}

int TelnetServer::Session::OnExecute()
{
    LocalVariables()[RemoteHostName()] = mSocket.Address();
    WriteHello().WriteNewline().WritePrompt();
    while (mStream)
        ReadCharacter();
    mpParent.load()->mDeletionQueue.Produce(this);
    return 0;
}

void TelnetServer::Session::OnScriptError(const std::string &inMessage)
{
    Write(inMessage).WriteNewline();
    ScriptInterpreter::OnScriptError(inMessage);
}

bool TelnetServer::Session::OnWriteLine(void *inInstance, const std::string &inLine)
{
    Session *this_ = reinterpret_cast<Session *>(inInstance);
    this_->Write(inLine).WriteNewline();
    return !!(this_->mStream << std::flush);
}

bool TelnetServer::Session::OnReadLine(void *inInstance, std::string &outLine)
{
    Session *this_ = reinterpret_cast<Session *>(inInstance);
    this_->Write("\\AwaitingInput:").WriteNewline();
    this_->mStream << std::flush;
    bool result = !!std::getline(this_->mStream, outLine);
    if (result)
        this_->Write("\\AcknowledgedInput").WriteNewline();
    if (!outLine.empty() && *outLine.rbegin() == '\r')
        outLine = outLine.substr(0, outLine.length() - 1);
    return result;
}

TelnetServer::Session &TelnetServer::Session::ReadCharacter()
{
    int c = mStream.get();
    if (mStream)
        switch (c)
        {
        case '\n':
            if (ScriptInterpreter::Execute(mLineBuffer))
            {
                const std::string &r = ScriptInterpreter::Result();
                Write(r);
                if (!r.empty() && *r.rbegin() != '\n')
                    WriteNewline();
                if (r == ScriptInterpreter::TerminationTag())
                    mSocket.Close();
            }
            WritePrompt();
            mLineBuffer.clear();
            break;
        case '\r':
        case '\x07': // Bell
            break;
        case '\x08': // Backspace
            if (!mLineBuffer.empty())
                mLineBuffer = mLineBuffer.substr(0, mLineBuffer.length() - 1);
            break;
        case 255: // Interpret As Command
            switch (mStream.get())
            {
            case 247: // Erase Character
                if (!mLineBuffer.empty())
                    mLineBuffer = mLineBuffer.substr(0, mLineBuffer.length() - 1);
                break;
            case 248: // Erase Line
                mLineBuffer.clear();
                break;
            default:
                while (mStream && mStream.rdbuf()->in_avail() && mStream.peek() > 127)
                    mStream.get();
            }
            break;
        default:
            mLineBuffer += c;
        }
    return *this;
}

TelnetServer::Session &TelnetServer::Session::WriteHello()
{
    mStream << "BCI2000 Version " << VersionInfo::Current[VersionInfo::VersionID];
    std::string hostname = Socket::Hostname();
    if (!hostname.empty())
        mStream << " on " << hostname;
    WriteNewline();
    mStream << "Type 'help' for a list of commands.";
    return *this;
}

TelnetServer::Session &TelnetServer::Session::Write(const std::string &inString)
{
    const char *p, *q;
    for (p = inString.c_str(), q = p; *p; ++p)
    {
        if (*p == '\n')
        {
            mStream.write(q, p - q);
            WriteNewline();
            q = p + 1;
        }
    }
    mStream.write(q, p - q);
    return *this;
}

TelnetServer::Session &TelnetServer::Session::WriteNewline()
{
    mStream.write("\r\n", 2);
    return *this;
}

TelnetServer::Session &TelnetServer::Session::WritePrompt()
{
    mStream.put('>').flush();
    return *this;
}
