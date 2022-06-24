////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A thin C++ wrapper around a Win32 window.
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
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

class Window
{
public:
  Window();
  ~Window();
  HWND Handle();
  void Show(int nCmdShow = SW_SHOWNORMAL);
  Window& SetStyle(int style);
  int Style() const;
  Window& SetLeft(int left);
  int Left() const;
  Window& SetTop(int top);
  int Top() const;
  Window& SetWidth(int width);
  int Width() const;
  Window& SetHeight(int height);
  int Height() const;
  Window& SetTitle(const std::string& title);
  const std::string& Title() const;

protected:
  virtual void OnCreate() {}
  virtual bool OnPreTranslate(MSG*) { return false; }
  virtual void OnMove(int, int) {}
  virtual void OnResize(int, int) {}
  virtual bool OnClose() { return true; }

private:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  void Create();
  void SyncGeometry();

private:
  friend class Application;
  const int mMagic = 'WNDW';
  int mStyle = WS_OVERLAPPEDWINDOW;
  int mLeft = 100, mTop = 100, mWidth = 640, mHeight = 480;
  HWND mHandle = NULL;
  std::string mTitle;
  static const char* sClassName;
};

#endif // WINDOW_H

