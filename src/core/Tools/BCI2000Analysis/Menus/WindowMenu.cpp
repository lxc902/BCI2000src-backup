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
#include "WindowMenu.h"
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <map>

namespace
{
std::map<QMainWindow *, QMenu *> sMenus;
struct MenuHandler : QObject
{
    void onMenuTriggered();
} sMenuHandler;
void MenuHandler::onMenuTriggered()
{
    auto pAction = dynamic_cast<QAction *>(sender());
    if (pAction)
    {
        auto pWindow = pAction->data().value<QMainWindow *>();
        if (pWindow)
        {
            pWindow->show();
            pWindow->activateWindow();
        }
    }
}
} // namespace

void WindowMenu::addWindow(QMainWindow *pWindow, const QString &title)
{
    QString menuTitle = title;
    if (menuTitle.isEmpty())
        menuTitle = pWindow->windowTitle();
    QMenu *pMenu = pWindow->menuBar()->addMenu("&Window");
    if (!sMenus.empty())
    {
        auto actions = sMenus.begin()->second->actions();
        for (auto pAction : actions)
        {
            auto pAction2 = pMenu->addAction(pAction->text());
            pAction2->setData(pAction->data());
            pAction2->connect(pAction2, &QAction::triggered, &sMenuHandler, &MenuHandler::onMenuTriggered);
        }
    }
    sMenus[pWindow] = pMenu;
    for (auto &s : sMenus)
    {
        auto pAction = s.second->addAction(menuTitle);
        pAction->setData(QVariant::fromValue(pWindow));
        pWindow->connect(pAction, &QAction::triggered, &sMenuHandler, &MenuHandler::onMenuTriggered);
    }
}

void WindowMenu::removeWindow(QMainWindow *pWindow)
{
    sMenus.erase(pWindow);
    for (auto &s : sMenus)
    {
        for (auto pAction : s.second->actions())
        {
            if (pAction->data().isValid() && pAction->data().value<QWidget *>() == pWindow)
            {
                s.second->removeAction(pAction);
                break;
            }
        }
    }
}
