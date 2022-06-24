////////////////////////////////////////////////////////////////////////////////
// $Id: Watches.h 6536 2022-02-06 16:48:20Z mellinger $
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
#ifndef WATCHES_H
#define WATCHES_H

#include "BCI_OperatorLib.h"
#include "CallbackBase.h"
#include "CommandInterpreter.h"
#include "InterpreterExpression.h"
#include "Thread.h"
#include "Sockets.h"
#include "SynchronizedQueue.h"
#include "PrecisionTime.h"
#include "StateRef.h"

#include <list>
#include <string>
#include <vector>
#include <mutex>

class Watch
{
  public:
    virtual ~Watch()
    {
    }

    long ID() const
    {
        return mID;
    }

    const std::string &Address() const
    {
        return mAddress;
    }

    void SetDecimation(int d)
    {
        mDecimation = d;
    }
    int Decimation() const
    {
        return mDecimation;
    }

    void SetTag(const std::string &s)
    {
        mTag = s;
    }
    const std::string &Tag() const
    {
        return mTag;
    }

    const std::string &Check()
    {
        CheckAndTrigger();
        return mBuf;
    }
    const std::string &Trigger()
    {
        if (!CheckAndTrigger())
            OnTrigger();
        return mBuf;
    }

    void Disable()
    {
      mrList.Remove(this);
    }
    void Enable()
    {
      mrList.Add(this);
    }

  public:
    class List;
    class Set : protected std::vector<Watch *>
    {
        Set();
        Set &operator=(const Set &);

      public:
        Set(const Set &);
        ~Set();

        int Size() const
        {
            return static_cast<int>(size());
        }
        bool Empty() const
        {
            return empty();
        }

        Watch *First() const;
        Watch *Next(Watch *) const;
        Watch *FindByID(int) const;

        enum options
        {
            WildcardNo = 0,
            WildcardYes = 1
        };
        Set SelectByAddress(const std::string &, options = WildcardNo) const;

      protected:
        explicit Set(const List *);

        struct Lock_
        {
            Lock_(const Set *);
            ~Lock_();
            const Set *mp;
        };
        friend struct Lock_;

      private:
        const List *mpList;
    };

    class List : public Set, public Lockable<>
    {
      public:
        List();
        ~List();

        void Check(int samples, double blockDuration, PrecisionTime sourceTime);
        PrecisionTime SampleTime(int) const;

      private:
        void Add(Watch *);
        void Remove(Watch *);
        friend class Watch;

      private:
        PrecisionTime mSourceTime;
        double mBlockDuration;
        int mSamples;
    };

  protected:
    Watch(CommandInterpreter &, const std::string &address, long ID);

    CommandInterpreter &Interpreter()
    {
        return mInterpreter;
    }
    void QueueMessage(PrecisionTime, const std::string &);
    void AboutToDelete();
    virtual int OnLoopMax()
    {
        return 0;
    }
    virtual bool OnCheck(int)
    {
        return false;
    }
    virtual void OnTrigger()
    {
    }

    const class List& List() const
    {
      return mrList;
    }

  private:
    bool CheckAndTrigger();
    void SendMessages();

    long mID;
    CommandInterpreter mInterpreter;
    class List &mrList;

    SynchronizedQueue<std::string> mQueue;

    SendingUDPSocket mSocket;
    std::string mAddress, mTag, mBuf;
    int mDecimation, mDecimationCarry;
    Thread mThread;
    MemberCall<void(Watch *)> mSendMessages;
};

class SystemStateWatch final : public Watch
{
  public:
    SystemStateWatch(CommandInterpreter &, const std::string &address = "", long ID = BCI_None);
    ~SystemStateWatch()
    {
        AboutToDelete();
    }

  protected:
    bool OnCheck(int) override;
    void OnTrigger() override;

  private:
    int mState;
};

class ExpressionWatch final : public Watch
{
  public:
    typedef std::list<InterpreterExpression> ExpressionList;

    ExpressionWatch(CommandInterpreter &, const std::string &address = "", long ID = BCI_None);
    ~ExpressionWatch()
    {
        AboutToDelete();
    }
    void SetExpressions(const std::vector<std::string> &expressions);
    const ExpressionList &Expressions() const
    {
        return mExpressions;
    }

  protected:
    int OnLoopMax() override;
    bool OnCheck(int) override;
    void OnTrigger() override;

  private:
    ExpressionList mExpressions;
    std::vector<double> mValues;
    int mSample;
};

#endif // WATCHES_H
