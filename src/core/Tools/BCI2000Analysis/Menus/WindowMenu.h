////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A menu holding window titles, and activating a window when
//   clicked.
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
#ifndef WINDOW_MENU_H
#define WINDOW_MENU_H

#include <QString>

class QMenu;
class QMainWindow;

class WindowMenu
{
  public:
    WindowMenu() = delete;

    static void addWindow(QMainWindow *, const QString &title = "");
    static void removeWindow(QMainWindow *);
};

#endif // WINDOW_MENU_H
