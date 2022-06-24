/////////////////////////////////////////////////////////////////////////////
// $Id: EventLink.cpp 6484 2022-01-03 16:59:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A component that establishes a UDP connection with the
//   operator module, and asynchronously receives events over this
//   connection.
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
/////////////////////////////////////////////////////////////////////////////
#include "EventLink.h"

#include "BCIEvent.h"
#include "BCIStream.h"
#include "CoreModule.h"
#include "PrecisionTime.h"

static const int cTimeoutMs = 500;

Extension(EventLink);

EventLink::EventLink() : mInput(&mStreambuf)
{
    mStreambuf.SetInput(&mInputSocket);
}

EventLink::~EventLink()
{
    Halt();
}

void EventLink::Publish()
{
  BEGIN_PARAMETER_DEFINITIONS
    "System:Protocol int EventLink= 1 1 0 1 // Send events from Operator to Source module (boolean)",
  END_PARAMETER_DEFINITIONS

    std::string operatorAddress = Parameter("OperatorAddress");
    std::string thisPort = operatorAddress.substr(operatorAddress.find(":") + 1);

    bool enabled = (Parameter("EventLink") != 0);
    if (enabled)
    {
        std::istringstream iss(thisPort.c_str());
        unsigned short port;
        iss >> port;
        mInputSocket.Open("localhost", port + 1);
        if (!mInputSocket.IsOpen())
            bcierr << "Could not open UDP port " << port + 1 << " for listening";
        else
        {
            std::string operatorIP = OptionalParameter("OperatorIP", "localhost");
            SendingUDPSocket clientSocket;
            clientSocket.Open(operatorIP, port);
            clientSocket.Write("\n", 1);
            clientSocket.Close();

            if (!mInputSocket.Wait(Time::FromIntTimeout(cTimeoutMs)))
            {
                bciwarn << "Operator does not seem to support EventLink over UDP Port " << port + 1 << ": "
                        << "no additional event definitions were received within " << 1e-3 * cTimeoutMs
                        << "s of query. "
                        << "Using core-module event definitions only.";
            }
            else
            {
                std::string line;
                while (std::getline(mInput, line) && !line.empty())
                {
                    BEGIN_EVENT_DEFINITIONS
                    line.c_str(), END_EVENT_DEFINITIONS
                }
            }
        }
    }
}

void EventLink::Preflight() const
{
}

void EventLink::Initialize()
{
}

void EventLink::StartRun()
{
    Thread::Start();
}

void EventLink::StopRun()
{
    Thread::Terminate();
}

void EventLink::Halt()
{
    StopRun();
}

int EventLink::OnExecute()
{
    const Time::Interval cReactionTime = Time::Seconds(0.1);
    while (!Thread::Terminating())
    {
        if (mInputSocket.Wait(cReactionTime))
        {
            std::string message;
            while (mInput && mInput.rdbuf()->in_avail())
            {
                if (mInput.peek() == '\n')
                {
                    mInput.ignore();
                    if (!message.empty())
                        bcievent << message;
                    message = "";
                }
                else
                {
                    message += mInput.get();
                }
            }
        }
    }
    return 0;
}
