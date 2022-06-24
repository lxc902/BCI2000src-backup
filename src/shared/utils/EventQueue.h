//////////////////////////////////////////////////////////////////////
// $Id: EventQueue.h 6503 2022-01-21 19:16:23Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A thread-safe event queue.
//   An event is defined by a string description, and a time stamp.
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
///////////////////////////////////////////////////////////////////////
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "PrecisionTime.h"
#include <queue>
#include <mutex>

#if 0

class EventQueue
{
    typedef SynchronizedQueue<std::pair<std::string, PrecisionTime>> Queue;

  public:
    EventQueue(int maxCount) : mEventsAllowed(false), mCount(0), mMaxCount(maxCount)
    {
    }
    void SetMaxCount(int maxCount)
    {
        mMaxCount = maxCount;
    }
    void AllowEvents()
    {
        mEventsAllowed = true;
    }
    void DenyEvents()
    {
        mEventsAllowed = false;
    }
    bool IsEmpty();
    void PushBack(const std::string &inDescriptor, PrecisionTime);
    void PopFront();
    const std::string &FrontDescriptor() const;
    PrecisionTime FrontTimeStamp() const;

  private:
    Queue mQueue;
    mutable Queue::Consumable mFront;
    std::atomic<bool> mEventsAllowed = false;
    std::atomic<int> mCount = 0, mMaxCount = 0;
};

#else

class EventQueue
{
  typedef std::queue<std::pair<std::string, PrecisionTime>> Queue;

public:
  EventQueue(int maxCount) : mEventsAllowed(false), mMaxCount(maxCount)
  {
  }
  void SetMaxCount(int maxCount)
  {
    mMaxCount = maxCount;
  }
  void AllowEvents()
  {
    mEventsAllowed = true;
  }
  void DenyEvents()
  {
    mEventsAllowed = false;
  }
  bool IsEmpty();
  void PushBack(const std::string& inDescriptor, PrecisionTime);
  void PopFront();
  const std::string& FrontDescriptor() const;
  PrecisionTime FrontTimeStamp() const;

private:
  Queue mQueue;
  mutable std::mutex mMutex;
  std::atomic<bool> mEventsAllowed = false;
  std::atomic<int> mMaxCount = 0;
};

#endif

#endif // EVENT_QUEUE_H
