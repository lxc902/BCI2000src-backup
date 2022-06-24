////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A node to represent the LinearClassifier filter in a graph.
//   Special treatment is necessary because the chain's output signal must be
//   intercepted at the LinearClassifier's location.
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
#include "LinearClassifierNode.h"
#include "FilterGraph.h"

namespace FilterGraph
{

std::string LinearClassifierNode::sType = Node::registerType("LinearClassifierNode", &LinearClassifierNode::create);

LinearClassifierNode::LinearClassifierNode() : FilterNode<LinearClassifierWrapper>("LinearClassifier")
{
}

Node *LinearClassifierNode::clone() const
{
    LinearClassifierNode *pNode = new LinearClassifierNode();
    pNode->copyFrom(this);
    return pNode;
}

Node *LinearClassifierNode::create()
{
    return new LinearClassifierNode;
}

const std::string &LinearClassifierNode::type() const
{
    return sType;
}

} // namespace FilterGraph
