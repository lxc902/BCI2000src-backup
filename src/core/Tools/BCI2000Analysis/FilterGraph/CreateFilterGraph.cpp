////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: Functions to create filter graphs.
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
#include "CreateFilterGraph.h"
#include "FilterGraphNode.h"

#include "AlignmentFilter.h"
#include "IIRBandpass.h"
#include "LPFilter.h"
#include "Normalizer.h"
#include "P3TemporalFilter.h"
#include "SpatialFilter.h"
#include "SpectralEstimator.h"
#include "TransmissionFilter.h"
#include "WindowingFilter.h"

#include "LinearClassifierNode.h"
#include "SplitterNode.h"
#include "StaticNodes.h"

#define FILTER_NAMES                                                                                                   \
    _(AlignmentFilter)                                                                                                 \
    _(TransmissionFilter)                                                                                              \
    _(SpatialFilter)                                                                                                   \
    _(IIRBandpass)                                                                                                     \
    _(WindowingFilter)                                                                                                 \
    _(SpectralEstimator)                                                                                               \
    _(P3TemporalFilter)                                                                                                \
    _(LinearClassifier)                                                                                                \
    _(LPFilter)                                                                                                        \
    _(Normalizer)

namespace FilterGraph
{

std::vector<std::string> filterNames()
{
    const char *names[] = {
#define _(x) #x,
        FILTER_NAMES
#undef _
    };
    std::vector<std::string> result;
    for (const char *p : names)
        result.push_back(p);
    return result;
}

Node *newFilterNode(const std::string &name)
{
    if (name == "LinearClassifier")
        return new LinearClassifierNode;
#define _(x)                                                                                                           \
    if (name == #x)                                                                                                    \
        return new FilterNode<x>;
    FILTER_NAMES
#undef _
    return nullptr;
}

Graph createMinimalGraph()
{
    Graph graph;
    graph.push_back(new SourceNode());
    graph.push_back(new ApplicationNode());
    return graph;
}

Graph createSMRAnalysisGraph()
{
    Graph graph;
    graph.push_back(new SourceNode());
    graph.push_back(new FilterNode<AlignmentFilter>());
    graph.push_back(new FilterNode<TransmissionFilter>());
    graph.push_back(new FilterNode<SpatialFilter>());
    graph.push_back(new FilterNode<SpectralEstimator>());
    graph.push_back(new LinearClassifierNode());
    graph.push_back(new FilterNode<Normalizer>());
    graph.push_back(new ApplicationNode());
    return graph;
}

Graph createERPAnalysisGraph()
{
    Graph graph;
    graph.push_back(new SourceNode());
    graph.push_back(new FilterNode<AlignmentFilter>());
    graph.push_back(new FilterNode<TransmissionFilter>());
    graph.push_back(new FilterNode<SpatialFilter>());
    graph.push_back(new FilterNode<P3TemporalFilter>());
    graph.push_back(new LinearClassifierNode());
    graph.push_back(new ApplicationNode());
    return graph;
}

Graph createComplexExampleGraph()
{
    Graph graph;
    graph.push_back(new SourceNode());
    graph.push_back(new FilterNode<AlignmentFilter>());
    graph.push_back(new FilterNode<TransmissionFilter>());
    Graph graph1("MainProcessing");
    graph1.push_back(new FilterNode<SpatialFilter>());
    Graph graph2("P3Processing");
    graph2.push_back(new FilterNode<P3TemporalFilter>());
    Graph graph3("ERDProcessing");
    graph3.push_back(new FilterNode<SpectralEstimator>());
    graph1.push_back(new SplitterNode("Splitter2", graph2, graph3));

    Graph graph4("LFProcessing");
    graph4.push_back(new FilterNode<SpatialFilter>());
    graph4.push_back(new FilterNode<LPFilter>());
    graph.push_back(new SplitterNode("Splitter1", graph4, graph1));
    graph.push_back(new LinearClassifierNode());
    graph.push_back(new FilterNode<Normalizer>());
    graph.push_back(new ApplicationNode());
    return graph;
}

} // namespace FilterGraph
