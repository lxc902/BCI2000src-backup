//////////////////////////////////////////////////////////////////////////////
// $Id: SignalWidget.cpp 6484 2022-01-03 16:59:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A widget to display signals from a BCI2000 file.
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
//////////////////////////////////////////////////////////////////////////////
#include "SignalWidget.h"
#include <QtWidgets>

SignalWidget::SignalWidget(QWidget *inParent)
    : QWidget(inParent), mpBackgroundLabel(new QLabel(this)), mPreviousPaintDuration(0)
{
    UpdateDrawContext();
    mpBackgroundLabel->setAlignment(Qt::AlignCenter);
}

SignalDisplay &SignalWidget::Display()
{
    return mDisplay;
}

const SignalDisplay &SignalWidget::Display() const
{
    return mDisplay;
}

bool SignalWidget::IsEmpty() const
{
    return mDisplay.NumSamples() == 0;
}

void SignalWidget::SetBackgroundText(const QString &inText)
{
    mpBackgroundLabel->setText(inText);
}

QString SignalWidget::BackgroundText() const
{
    return mpBackgroundLabel->text();
}

void SignalWidget::AddSeparatorAction()
{
    QAction *pSeparator = new QAction(NULL);
    pSeparator->setSeparator(true);
    this->addAction(pSeparator);
}

void SignalWidget::paintEvent(QPaintEvent *inpEvent)
{
    if (IsEmpty())
    {
        mpBackgroundLabel->setVisible(true);
        QWidget::paintEvent(inpEvent);
    }
    else
    {
        mpBackgroundLabel->setVisible(false);
        bool setCursor = mPreviousPaintDuration > 100;
        if (setCursor)
            QApplication::setOverrideCursor(Qt::WaitCursor);
        int64_t startTime = QDateTime::currentMSecsSinceEpoch();
        mDisplay.Paint(const_cast<void *>(reinterpret_cast<const void *>(&(inpEvent->region()))));
        mPreviousPaintDuration = QDateTime::currentMSecsSinceEpoch() - startTime;
        if (setCursor)
            QApplication::restoreOverrideCursor();
    }
}

void SignalWidget::resizeEvent(QResizeEvent *inpEvent)
{
    UpdateDrawContext();
    QWidget::resizeEvent(inpEvent);
}

void SignalWidget::UpdateDrawContext()
{
    GUI::DrawContext dc = {{0, 0, this->width(), this->height()}, 0};
    dc.device = this;
    mDisplay.SetContext(dc);
    mpBackgroundLabel->setGeometry(0, 0, this->width(), this->height());
}
