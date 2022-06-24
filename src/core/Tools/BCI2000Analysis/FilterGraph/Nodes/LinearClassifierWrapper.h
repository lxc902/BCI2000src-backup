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
#ifndef LINEAR_CLASSIFIER_WRAPPER_H
#define LINEAR_CLASSIFIER_WRAPPER_H

#include "LinearClassifier.h"
#include <vector>

class LinearClassifierWrapper : public LinearClassifier
{
  public:
    static LinearClassifierWrapper *instance();
    LinearClassifierWrapper();
    ~LinearClassifierWrapper();
    void setObserve(bool);
    void setCondition1(const std::string &);
    void setCondition2(const std::string &);
    void resetStatistics();
    const std::vector<GenericSignal> &data1() const;
    const std::vector<GenericSignal> &data2() const;

  protected:
    void Preflight(const SignalProperties &, SignalProperties &) const override;
    void Initialize(const SignalProperties &, const SignalProperties &) override;
    void Process(const GenericSignal &, GenericSignal &) override;

  private:
    struct Private;
    Private *p;
};

#endif // LINEAR_CLASSIFIER_WRAPPER_H
