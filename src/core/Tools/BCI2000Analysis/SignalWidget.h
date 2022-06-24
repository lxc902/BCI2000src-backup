////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A widget that holds a BCI2000 signal display.
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
#ifndef SIGNAL_WIDGET_H
#define SIGNAL_WIDGET_H

#include <QWidget>

class GenericSignal;
class SignalDisplay;

class SignalWidget : public QWidget
{
    Q_OBJECT

  public:
    SignalWidget(const GenericSignal &, QWidget * = nullptr);
    ~SignalWidget();
    SignalWidget(SignalWidget &) = delete;
    SignalWidget &operator=(SignalWidget &) = delete;

    SignalDisplay &display();
    const SignalDisplay &display() const;
    bool contextMenu() const;
    void setContextMenu(bool);

  signals:
    void showCurvesAt(int channel);
    void showDistributionAt(int element);

  protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void resizeEvent(QResizeEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void contextMenuEvent(QContextMenuEvent *) override;

  private:
    struct Private;
    Private *p;
};

#endif // SIGNAL_WIDGET_H
