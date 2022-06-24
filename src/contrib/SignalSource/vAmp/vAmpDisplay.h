////////////////////////////////////////////////////////////////////////////////
// $Id: vAmpDisplay.h 6484 2022-01-03 16:59:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that provides access to the vAmp's built-in display.
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
#ifndef V_AMP_DISPLAY_H
#define V_AMP_DISPLAY_H

#if __BORLANDC__
# include <vcl.h>
#else
# include <QImage>
#endif // __BORLANDC__

#include <vector>
#include <string>
#include "Color.h"

class vAmpDisplay
{
 public:
  static void Clear( int inAmpID );
  static void DisplayLogo( int inAmpID );
  static unsigned int DisplayImpedances( int inAmpID, const std::vector<float>& inImpedances );

 private:
  static void ValueToText( float inValue, std::string& outText, RGBColor& outColor );
#if __BORLANDC__
  static Graphics::TBitmap* NewBitmap();
#else //__BORLANDC__
  static QImage* NewImage();
  static HBITMAP QImageToHBITMAP( const QImage& );
#endif // __BORLANDC__
};

#endif // V_AMP_DISPLAY_H
