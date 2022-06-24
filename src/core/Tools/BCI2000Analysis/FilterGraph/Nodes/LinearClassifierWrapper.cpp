////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A wrapper filter for LinearClassifier that observes
//   LinearClassifier's input signal, and does statistics on it.
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
#include "LinearClassifierWrapper.h"
#include "Expression.h"
#include <vector>

struct LinearClassifierWrapper::Private
{
    static LinearClassifierWrapper *spInstance;
    bool mObserve;
    Expression mCondition1, mCondition2;
    std::vector<GenericSignal> mData1, mData2;

    Private();
    void reset();
    void initialize(const SignalProperties &);
    void observe(const GenericSignal &);
};

LinearClassifierWrapper *LinearClassifierWrapper::Private::spInstance = nullptr;

LinearClassifierWrapper::Private::Private() : mObserve(false)
{
}

void LinearClassifierWrapper::Private::reset()
{
    mData1.clear();
    mData2.clear();
}

void LinearClassifierWrapper::Private::initialize(const SignalProperties &Input)
{
    reset();
}

void LinearClassifierWrapper::Private::observe(const GenericSignal &Input)
{
    if (mCondition1.Evaluate(&Input))
        mData1.push_back(Input);
    if (mCondition2.Evaluate(&Input))
        mData2.push_back(Input);
}

LinearClassifierWrapper *LinearClassifierWrapper::instance()
{
    return Private::spInstance;
}

LinearClassifierWrapper::LinearClassifierWrapper() : p(new Private)
{
    Private::spInstance = this;
}

LinearClassifierWrapper::~LinearClassifierWrapper()
{
    delete p;
    Private::spInstance = nullptr;
}

void LinearClassifierWrapper::setObserve(bool b)
{
    p->mObserve = b;
}

void LinearClassifierWrapper::setCondition1(const std::string &s)
{
    p->mCondition1 = s;
}

void LinearClassifierWrapper::setCondition2(const std::string &s)
{
    p->mCondition2 = s;
}

void LinearClassifierWrapper::resetStatistics()
{
    p->reset();
}

const std::vector<GenericSignal> &LinearClassifierWrapper::data1() const
{
    return p->mData1;
}

const std::vector<GenericSignal> &LinearClassifierWrapper::data2() const
{
    return p->mData2;
}

void LinearClassifierWrapper::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    GenericSignal preflightSignal(Input);
    p->mCondition1.Evaluate(&preflightSignal);
    p->mCondition2.Evaluate(&preflightSignal);
    LinearClassifier::Preflight(Input, Output);
}

void LinearClassifierWrapper::Initialize(const SignalProperties &Input, const SignalProperties &Output)
{
    p->initialize(Input);
    LinearClassifier::Initialize(Input, Output);
}

void LinearClassifierWrapper::Process(const GenericSignal &Input, GenericSignal &Output)
{
    if (p->mObserve)
        p->observe(Input);
    else
        LinearClassifier::Process(Input, Output);
}
