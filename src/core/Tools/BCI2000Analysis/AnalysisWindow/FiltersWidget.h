////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A Widget displaying a filter graph.
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
#ifndef FILTERS_WIDGET_H
#define FILTERS_WIDGET_H
#include "FilterGraph.h"
#include <QGraphicsView>

class FiltersWidget : public QGraphicsView
{
    Q_OBJECT

  public:
    FiltersWidget(QWidget *);
    ~FiltersWidget();
    FiltersWidget(FiltersWidget &) = delete;
    FiltersWidget &operator=(FiltersWidget &) = delete;
    void clearGraph();
    void setGraph(const FilterGraph::Graph &);
    const FilterGraph::Graph &graph() const;
    void setEditable(bool);
    bool editable() const;

  signals:
    void edited();
    void graphReplaced();

  private:
    struct Private;
    Private *p;
};

#endif // FILTERS_WIDGET_H
