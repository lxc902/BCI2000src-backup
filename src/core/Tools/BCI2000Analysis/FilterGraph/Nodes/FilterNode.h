////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A template to create BCI2000 filter graph nodes from filters.
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
#ifndef FILTER_NODE_H
#define FILTER_NODE_H

#include "ClassName.h"
#include "FilterGraphNode.h"

namespace FilterGraph
{

template <class T> class FilterNode : public Node
{
  public:
    FilterNode(const std::string &name = "") : Node(name.empty() ? ClassName(typeid(T)) : name)
    {
    }

    Node *clone() const override
    {
        FilterNode *pNode = new FilterNode;
        pNode->copyFrom(this);
        return pNode;
    }

    static Node *create()
    {
        return new FilterNode;
    }

    const std::string &type() const override
    {
        return sType;
    }

    GenericFilter::Registrar *getRegistrar(Directory::Node *pNode) const override
    {
        return new Registrar(this, this->mpFilter, pNode);
    }

  private:
    struct Registrar : GenericFilter::FilterRegistrar<T>
    {
        const Node *mpNode;
        GenericFilter *&mpFilter;
        Registrar(const Node *pNode, GenericFilter *&pFilter, Directory::Node *pDirNode)
            : GenericFilter::FilterRegistrar<T>(pDirNode), mpNode(pNode), mpFilter(pFilter)
        {
        }
        GenericFilter *NewInstance() const override
        {
            GenericFilter *pFilter = GenericFilter::FilterRegistrar<T>::NewInstance();
            pFilter->SetName(mpNode->name());
            mpFilter = pFilter;
            return pFilter;
        }
    };
    friend struct Registrar;

    static std::string sType;
};
template <class T>
std::string FilterNode<T>::sType = Node::registerType(ClassName(typeid(T)), &FilterNode::create);

} // namespace FilterGraph

#endif // FILTER_NODE_H
