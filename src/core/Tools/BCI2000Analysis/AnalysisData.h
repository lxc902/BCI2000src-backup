////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A struct that holds data analysis parameters, and training
//   data.
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
#ifndef ANALYSIS_DATA_H
#define ANALYSIS_DATA_H

#include "AnalysisParamWidget.h"
#include "FilterGraph.h"
#include "GenericSignal.h"
#include "ParamList.h"
#include <string>
#include <vector>

struct AnalysisData
{
    std::vector<std::pair<std::string, std::string>> trainingFiles, testFiles;
    FilterGraph::Graph filterGraph;
    ParamList filterParams;
    AnalysisParamWidget::AnalysisParams analysisParams;
    std::vector<GenericSignal> trainingData1, trainingData2, testData1, testData2;

    const SignalProperties *signalProperties() const;
    void serialize(std::ostream &) const;
    static AnalysisData unserialize(std::istream &);
};

#endif // ANALYSIS_DATA_H
