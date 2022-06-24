////////////////////////////////////////////////////////////////////////////////
// $Id: Watches.cpp 6562 2022-02-12 16:32:17Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A watch object, and a container for watches. A watch consists
//   of a number of expressions which send their values to a UDP port whenever
//   any of them changes.
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
#include "Watches.h"

#include "BCIException.h"
#include "StateMachine.h"
#include "SystemStates.h"
#include "WildcardMatch.h"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>

// Watch
Watch::Watch(CommandInterpreter &inInterpreter, const std::string &inAddress, long inID)
    : mInterpreter(inInterpreter.StateMachine()), mID(inID), mrList(inInterpreter.StateMachine().Watches()),
      mSendMessages(&Watch::SendMessages, this), mThread(&mSendMessages, "Watch Send Messages"),
      mDecimationCarry(0), mDecimation(1)
{
    mThread.Start();
    ScopedLock(mrList);
    if (mID == BCI_None)
    {
        if (inAddress.empty())
        {
            std::string addr, var = inInterpreter.RemoteHostName();
            if (inInterpreter.LocalVariables().Exists(var))
                addr = inInterpreter.LocalVariables()[var];
            addr = inInterpreter.StateMachine().SuggestUDPAddress(addr);
            mSocket.Open(addr);
            if (mSocket.IsOpen())
                mAddress = addr;
        }
        else
        {
            mSocket.Open(inAddress);
            if (mSocket.IsOpen())
            {
                std::string s = mSocket.Address();
                if (inInterpreter.StateMachine().SuggestUDPAddress(s) == s)
                    mAddress = s;
                else
                    mSocket.Close();
            }
        }
    }
    else
        while (mrList.FindByID(mID))
            ++mID;
    mrList.Add(this);
}

void Watch::AboutToDelete()
{
    mrList.Remove(this);
    mThread.Terminate();
}

bool Watch::CheckAndTrigger()
{
    bool triggered = false;
    int i = mDecimationCarry, max = OnLoopMax();
    for (; i <= max; i += mDecimation)
    {
        if (OnCheck(i))
        {
            triggered = true;
            OnTrigger();
        }
    }
    mDecimationCarry = i % (max + 1);
    return triggered;
}

void Watch::QueueMessage(PrecisionTime inTime, const std::string &inMessage)
{
    mBuf = inMessage;
    std::string msg = std::to_string(inTime) + '\t' + inMessage + "\r\n";
    mQueue.Produce(msg);
}

void Watch::SendMessages()
{
  while (mQueue.Wait())
  {
    std::vector<std::string> values;
    SynchronizedQueue<std::string>::Consumable c;
    while ((c = mQueue.Consume()))
    {
        if (mSocket.IsOpen())
            mSocket.Write(c->c_str(), c->length() + 1);
        if (mID != BCI_None)
            values.push_back(*c);
    }
    std::vector<const char*> pointers;
    for (const auto& s : values)
        pointers.push_back(s.c_str());
    if (!pointers.empty())
        Interpreter().StateMachine().ExecuteCallback<int, const char* const*>(mID, pointers.size(), pointers.data());
  }
}

// SystemStateWatch
SystemStateWatch::SystemStateWatch(CommandInterpreter &inInterpreter, const std::string &inAddress, long inID)
    : Watch(inInterpreter, inAddress, inID), mState(BCI_None)
{
}

bool SystemStateWatch::OnCheck(int)
{
    int state = BCI_GetStateOfOperation();
    bool result = (state != mState);
    mState = state;
    return result;
}

void SystemStateWatch::OnTrigger()
{
    std::string name = SystemStates::Name(mState);
    if (name.empty())
        name = "<unknown>";
    int64_t timestamp = List().SampleTime(Interpreter().StateMachine().StateVector().Samples());
    QueueMessage(timestamp, name);
}

// ExpressionWatch
ExpressionWatch::ExpressionWatch(CommandInterpreter &inInterpreter, const std::string &inAddress, long inID)
    : Watch(inInterpreter, inAddress, inID), mSample(-1)
{
}

void ExpressionWatch::SetExpressions(const std::vector<std::string> &inExpressions)
{
    Disable();
    mExpressions.clear();
    for (size_t i = 0; i < inExpressions.size(); ++i)
    {
        mExpressions.push_back(InterpreterExpression(Interpreter(), inExpressions[i]));
        mExpressions.back().ForbidAssignment().Evaluate();
    }
    mValues.clear();
    mValues.resize(mExpressions.size(), std::numeric_limits<double>::quiet_NaN());
    Enable();
}

int ExpressionWatch::OnLoopMax()
{
    int loopMax = 0;
    for (const auto &expr : mExpressions)
        if (expr.RefersStates())
            loopMax = Interpreter().StateMachine().StateVector().Samples() - 2;
    return std::max(loopMax, 0);
}

bool ExpressionWatch::OnCheck(int inSample)
{
    bool changed = false;
    size_t idx = 0;
    for (ExpressionList::iterator i = mExpressions.begin(); i != mExpressions.end(); ++i)
    {
        double result = i->Evaluate(inSample);
        if (result != mValues[idx])
        {
            changed = true;
            mValues[idx] = result;
        }
        ++idx;
    }
    if (changed)
      mSample = inSample;
    return changed;
}

void ExpressionWatch::OnTrigger()
{
    std::ostringstream oss;
    oss << std::setprecision(16);
    if (!mValues.empty())
        oss << Pretty(mValues[0]);
    for (size_t idx = 1; idx < mValues.size(); ++idx)
        oss << '\t' << Pretty(mValues[idx]);
    int64_t timestamp = List().SampleTime(mSample);
    QueueMessage(timestamp, oss.str());
}

// Watch::Set
Watch::Set::Set(const Watch::Set &other) : vector<Watch *>(other), mpList(other.mpList)
{
    mpList->Lock();
}

Watch::Set::Set(const class Watch::List *inList) : mpList(inList)
{
    if (mpList != this)
        mpList->Lock();
}

Watch::Set::~Set()
{
    if (mpList != this)
        mpList->Unlock();
}

Watch *Watch::Set::First() const
{
    Lock_ lock(this);
    return empty() ? 0 : *begin();
}

Watch *Watch::Set::Next(Watch *inEl) const
{
    Lock_ lock(this);
    const_iterator i = find(begin(), end(), inEl);
    if (i != end())
        ++i;
    return i == end() ? 0 : *i;
}

Watch *Watch::Set::FindByID(int inID) const
{
    if (inID == BCI_None)
        return 0;

    Lock_ lock(this);
    for (const_iterator i = begin(); i != end(); ++i)
        if ((*i)->ID() == inID)
            return *i;
    return 0;
}

Watch::Set Watch::Set::SelectByAddress(const std::string &inAddress, options inOpt) const
{
    Set result(mpList);
    struct Comp
    {
        static bool Eq(const std::string &a, const std::string &b)
        {
            return a == b;
        }
        static bool Wild(const std::string &a, const std::string &b)
        {
            return WildcardMatch(a, b);
        }
    };
    bool (*match)(const std::string &, const std::string &) = (inOpt & WildcardYes) ? &Comp::Wild : &Comp::Eq;

    Lock_ lock(this);
    for (const_iterator i = begin(); i != end(); ++i)
        if (!(*i)->Address().empty() && match(inAddress, (*i)->Address()))
            result.push_back(*i);
    return result;
}

// Watch::Set::Lock_
Watch::Set::Lock_::Lock_(const Set *p) : mp(p)
{
    mp->mpList->Lock();
}

Watch::Set::Lock_::~Lock_()
{
    mp->mpList->Unlock();
}

// Watch::List
Watch::List::List()
  : Set(this), 
    mSourceTime(0), mBlockDuration(-1), mSamples(-1)
{
}

Watch::List::~List()
{
    Lock_ lock(this);
    while (!empty())
        delete back();
}

PrecisionTime Watch::List::SampleTime(int inSample) const
{
  if (mBlockDuration <= 0)
    return -1;
  return mSourceTime - mBlockDuration + mBlockDuration * inSample / mSamples;
}

void Watch::List::Add(Watch *inpWatch)
{
    Lock_ lock(this);
    if (find(begin(), end(), inpWatch) == end())
        push_back(inpWatch);
}

void Watch::List::Remove(Watch *inpWatch)
{
    Lock_ lock(this);
    iterator i = find(begin(), end(), inpWatch);
    if (i != end())
        erase(i);
}

void Watch::List::Check(int samples, double blockDuration, PrecisionTime sourceTime)
{
    Lock_ lock(this);
    mSamples = samples;
    mBlockDuration = 1000 * blockDuration + 0.5;
    mSourceTime = sourceTime;

    for (iterator i = begin(); i != end(); ++i)
        (*i)->Check();

    mBlockDuration = 0;
}
