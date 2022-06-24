////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
// Description: A more complex demo that shows how to render multiple images
//   into offscreen canvases, and display them in operator windows.
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
#ifndef COMPLEX_VISUALIZATION_DEMO_FILTER_H
#define COMPLEX_VISUALIZATION_DEMO_FILTER_H

#include "GenericFilter.h"

class ComplexVisualizationDemoFilter : public GenericFilter
{
  public:
    ComplexVisualizationDemoFilter();
    ~ComplexVisualizationDemoFilter();

  protected:
    void Publish() override;
    void Preflight(const SignalProperties &Input, SignalProperties &Output) const override;
    void Initialize(const SignalProperties &Input, const SignalProperties &Output) override;
    void Process(const GenericSignal &Input, GenericSignal &Output) override;
    void StartRun() override;
    void StopRun() override;

  private:
    struct Private;
    Private *p;
};

#endif // COMPLEX_VISUALIZATION_DEMO_FILTER_H
