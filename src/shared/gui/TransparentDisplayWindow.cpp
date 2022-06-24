////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphDisplay descendant which is a frameless GUI window for
//   an application's user display.
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
#include "DisplayWindow.h"

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

namespace GUI {

	struct DisplayWindow::Data : QWidget
	{
		Data(GraphDisplay& display)
			: mrDisplay(display)
		{
			setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
			setAttribute(Qt::WA_TranslucentBackground);
			setAttribute(Qt::WA_NoSystemBackground);
			setAttribute(Qt::WA_PaintOnScreen);
		}

		void paintEvent(QPaintEvent*) override
		{
			GUI::DrawContext dc = {0};
			dc.device = this;
			dc.rect.left = 0;
			dc.rect.right = width();
			dc.rect.top = 0;
			dc.rect.bottom = height();
			mrDisplay.SetContext(dc);
			mrDisplay.Paint();
		}

		GraphDisplay& mrDisplay;
		int mBitmapBufferWidth = 0, mBitmapBufferHeight = 0;
	};


	DisplayWindow::DisplayWindow()
		: d(new Data(*this))
	{
	}

	DisplayWindow::~DisplayWindow()
	{
		delete d;
	}

	DisplayWindow& DisplayWindow::Update(void*)
	{
		d->update();
		return *this;
	}

	DisplayWindow& DisplayWindow::SetAsyncRendering(bool)
	{
		return *this;
	}

	DisplayWindow& DisplayWindow::SetUpdateRateDecimation(int)
	{
		return *this;
	}

	StringUtils::NameValueList DisplayWindow::Info() const
	{
		return StringUtils::NameValueList();
	}

	bci::ClockTick::Statistics DisplayWindow::FrameClockStatistics() const
	{
		bci::ClockTick::Statistics statistics = {0};
		return statistics;
	}

	struct DisplayWindow::RenderStatistics DisplayWindow::RenderStatistics() const
	{
	  struct DisplayWindow::RenderStatistics statistics = {0};
		return statistics;
	}

	struct DisplayWindow::PresentationStatistics DisplayWindow::PresentationStatistics() const
	{
		struct DisplayWindow::PresentationStatistics statistics = { 0 };
		return statistics;
	}

	DisplayWindow& DisplayWindow::SetTitle(const std::string& s)
	{
		d->setWindowTitle(QString::fromUtf8(s.c_str()));
		return *this;
	}

	DisplayWindow& DisplayWindow::SetLeft(int left)
	{
		d->move(left, d->pos().y());
		return *this;
	}

	DisplayWindow& DisplayWindow::SetTop(int top)
	{
		d->move(d->pos().x(), top);
		return *this;
	}

	DisplayWindow& DisplayWindow::SetWidth(int width)
	{
		d->resize(width, d->size().height());
		return *this;
	}

	int DisplayWindow::Width() const
	{
		return d->width();
	}

	DisplayWindow& DisplayWindow::SetHeight(int height)
	{
		d->resize(d->size().width(), height);
		return *this;
	}

	int DisplayWindow::Height() const
	{
		return d->height();
	}

	DisplayWindow& DisplayWindow::SetVisible(bool v)
	{
		d->setVisible(v);
		return *this;
	}

	DisplayWindow& DisplayWindow::ConfigureBitmapBuffer(int w, int h)
	{
		d->mBitmapBufferWidth = w;
		d->mBitmapBufferHeight = h;
		return *this;
	}

	void DisplayWindow::OnBitmapData(GUI::Bitmap& b, const GUI::Rect* r) const
	{
		b = GUI::Bitmap(d->mBitmapBufferHeight, d->mBitmapBufferWidth);
	}
}