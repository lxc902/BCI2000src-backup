/////////////////////////////////////////////////////////////////////////////
//
// File: GUIBackgroundView.cpp
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIBackgroundView class implements the GUI specific details
//              of the TBackgroundView class.
//
// Changes:
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

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include "OSIncludes.h"

#ifndef VCL
# error This is the VCL implementation of TGUIBackgroundView.
#endif

#include "GUIBackgroundView.h"

TGUIBackgroundView::TGUIBackgroundView()
: TGUIView( backgroundViewZ )
{
}

void
TGUIBackgroundView::Paint()
{
    TCanvas *canvas = GetCanvas();
    canvas->Pen->Color = TGUIView::GetElementColor( fbBackground ).cl;
    canvas->Brush->Color = canvas->Pen->Color;
    canvas->FillRect( viewTRect );
}

