////////////////////////////////////////////////////////////////////////////////
// $Id: bci2000logo_mini.h 6484 2022-01-03 16:59:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 73x72 pixel version of the BCI2000 logo.
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
namespace Resources
{
static struct Element_BCI2000logo_mini
{ // run-length encoded bitmap data
    unsigned char color;
    unsigned short count;
} BCI2000logo_mini[] = {
    {0x0, 0x49}, // width
    {0x0, 0x48}, // height
    {0xff, 0x1b}, {0xf8, 0x1},  {0xc0, 0x1},  {0xa1, 0x1},  {0x80, 0x1},  {0x55, 0x1},  {0x40, 0x1},
    {0x2a, 0x1},  {0x0, 0x8},   {0x2a, 0x1},  {0x55, 0x1},  {0x80, 0x1},  {0xc0, 0x1},  {0xf8, 0x1},
    {0xff, 0x31}, {0xc9, 0x1},  {0xa1, 0x1},  {0x6a, 0x1},  {0x2a, 0x1},  {0x0, 0x4},   {0x2a, 0x1},
    {0x40, 0x1},  {0x6a, 0x1},  {0x80, 0x2},  {0xa1, 0x3},  {0x80, 0x2},  {0x6a, 0x1},  {0x40, 0x1},
    {0x2a, 0x1},  {0x0, 0x3},   {0x40, 0x1},  {0x80, 0x1},  {0xc9, 0x1},  {0xff, 0x2b}, {0xc0, 0x1},
    {0x6a, 0x1},  {0x2a, 0x1},  {0x0, 0x2},   {0x2a, 0x1},  {0x55, 0x1},  {0x80, 0x1},  {0xc0, 0x1},
    {0xc9, 0x1},  {0xf8, 0x1},  {0xff, 0xd},  {0xf8, 0x1},  {0xc0, 0x1},  {0xa1, 0x1},  {0x40, 0x1},
    {0x0, 0x2},   {0x2a, 0x1},  {0xa1, 0x1},  {0xf8, 0x1},  {0xff, 0x25}, {0xc9, 0x1},  {0x80, 0x1},
    {0x2a, 0x1},  {0x0, 0x2},   {0x55, 0x1},  {0xa1, 0x1},  {0xc9, 0x1},  {0xff, 0x17}, {0xf8, 0x1},
    {0xa1, 0x1},  {0x40, 0x1},  {0x0, 0x2},   {0x80, 0x1},  {0xf8, 0x1},  {0xff, 0x21}, {0xc0, 0x1},
    {0x40, 0x1},  {0x0, 0x2},   {0x6a, 0x1},  {0xc0, 0x1},  {0xf8, 0x1},  {0xff, 0x1d}, {0xc9, 0x1},
    {0x6a, 0x1},  {0x0, 0x2},   {0xa1, 0x1},  {0xff, 0x1e}, {0xc9, 0x1},  {0x40, 0x1},  {0x0, 0x1},
    {0x2a, 0x1},  {0xa1, 0x1},  {0xf8, 0x1},  {0xff, 0x22}, {0xc9, 0x1},  {0x55, 0x1},  {0x0, 0x1},
    {0x40, 0x1},  {0xc9, 0x1},  {0xff, 0x1b}, {0x80, 0x1},  {0x0, 0x1},   {0x2a, 0x1},  {0xa1, 0x1},
    {0xff, 0x27}, {0xc0, 0x1},  {0x2a, 0x1},  {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x18}, {0xc9, 0x1},
    {0x40, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xf8, 0x1},  {0xff, 0x29}, {0xf8, 0x1},  {0x55, 0x1},
    {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x16}, {0xc0, 0x1},  {0x0, 0x1},   {0x2a, 0x1},  {0xc0, 0x1},
    {0xff, 0x2d}, {0x80, 0x1},  {0x0, 0x1},   {0x55, 0x1},  {0xff, 0x14}, {0xa1, 0x1},  {0x0, 0x1},
    {0x40, 0x1},  {0xf8, 0x1},  {0xff, 0x2f}, {0xa1, 0x1},  {0x0, 0x1},   {0x40, 0x1},  {0xff, 0x12},
    {0xa1, 0x1},  {0x0, 0x1},   {0x55, 0x1},  {0xff, 0x32}, {0xc0, 0x1},  {0x0, 0x1},   {0x40, 0x1},
    {0xff, 0x10}, {0xc0, 0x1},  {0x0, 0x1},   {0x55, 0x1},  {0xff, 0x34}, {0xa1, 0x1},  {0x0, 0x1},
    {0x55, 0x1},  {0xff, 0xe},  {0xf8, 0x1},  {0x0, 0x1},   {0x40, 0x1},  {0xff, 0xd},  {0xf8, 0x1},
    {0xc9, 0x1},  {0xc0, 0x6},  {0xf8, 0x1},  {0xff, 0xc},  {0xf8, 0x1},  {0xc9, 0x1},  {0xa1, 0x2},
    {0x80, 0x1},  {0xa1, 0x2},  {0xc0, 0x1},  {0xf8, 0x1},  {0xff, 0x3},  {0xc9, 0x1},  {0xc0, 0x3},
    {0xc9, 0x1},  {0xff, 0x3},  {0xa1, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xff, 0xd},  {0x40, 0x1},
    {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0xc},  {0xa1, 0x1},  {0x0, 0x9},   {0x40, 0x1},  {0x80, 0x1},
    {0xf8, 0x1},  {0xff, 0x7},  {0xc9, 0x1},  {0x80, 0x1},  {0x2a, 0x1},  {0x0, 0x7},   {0x2a, 0x1},
    {0xf8, 0x1},  {0xff, 0x2},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x4},  {0x80, 0x1},
    {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0xb},  {0xa1, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0xd},
    {0xa1, 0x1},  {0x0, 0xb},   {0x40, 0x1},  {0xf8, 0x1},  {0xff, 0x5},  {0x80, 0x1},  {0x0, 0xa},
    {0x40, 0x1},  {0xff, 0x3},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x5},  {0x55, 0x1},
    {0x0, 0x1},   {0xc9, 0x1},  {0xff, 0x9},  {0xf8, 0x1},  {0x0, 0x1},   {0x55, 0x1},  {0xff, 0xe},
    {0xa1, 0x1},  {0x0, 0x4},   {0x80, 0x3},  {0x40, 0x1},  {0x0, 0x4},   {0x80, 0x1},  {0xff, 0x4},
    {0x80, 0x1},  {0x0, 0x5},   {0x2a, 0x1},  {0x40, 0x1},  {0x55, 0x2},  {0x40, 0x1},  {0x0, 0x1},
    {0xa1, 0x1},  {0xff, 0x3},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x5},  {0xf8, 0x1},
    {0x2a, 0x1},  {0x40, 0x1},  {0xff, 0x9},  {0x6a, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0xe},
    {0xa1, 0x1},  {0x0, 0x4},   {0xff, 0x4},  {0x40, 0x1},  {0x0, 0x3},   {0x40, 0x1},  {0xff, 0x3},
    {0xa1, 0x1},  {0x0, 0x4},   {0x2a, 0x1},  {0xa1, 0x1},  {0xf8, 0x1},  {0xff, 0x4},  {0xf8, 0x2},
    {0xff, 0x3},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x6},  {0xc0, 0x1},  {0x0, 0x1},
    {0x80, 0x1},  {0xff, 0x7},  {0xc9, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xff, 0xf},  {0xa1, 0x1},
    {0x0, 0x4},   {0xf8, 0x1},  {0xff, 0x3},  {0x55, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x2},
    {0xf8, 0x1},  {0x2a, 0x1},  {0x0, 0x3},   {0x2a, 0x1},  {0xc9, 0x1},  {0xff, 0xb},  {0x40, 0x1},
    {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x7},  {0x6a, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x6},
    {0x6a, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0xf},  {0xa1, 0x1},  {0x0, 0x4},   {0xc0, 0x1},
    {0xc9, 0x1},  {0xc0, 0x1},  {0x6a, 0x1},  {0x0, 0x4},   {0xc0, 0x1},  {0xff, 0x2},  {0xc0, 0x1},
    {0x0, 0x4},   {0xa1, 0x1},  {0xff, 0xc},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x7},
    {0xf8, 0x1},  {0x0, 0x1},   {0x55, 0x1},  {0xff, 0x5},  {0xf8, 0x1},  {0x0, 0x1},   {0x80, 0x1},
    {0xff, 0x10}, {0xa1, 0x1},  {0x0, 0xa},   {0x40, 0x1},  {0xc9, 0x1},  {0xff, 0x3},  {0x80, 0x1},
    {0x0, 0x4},   {0xc9, 0x1},  {0xff, 0xc},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x8},
    {0xa1, 0x1},  {0x0, 0x1},   {0xc9, 0x1},  {0xff, 0x4},  {0xa1, 0x1},  {0x0, 0x1},   {0xc9, 0x1},
    {0xff, 0x10}, {0xa1, 0x1},  {0x0, 0xa},   {0x2a, 0x1},  {0xa1, 0x1},  {0xff, 0x3},  {0x6a, 0x1},
    {0x0, 0x4},   {0xf8, 0x1},  {0xff, 0xc},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x8},
    {0xf8, 0x1},  {0x0, 0x1},   {0x55, 0x1},  {0xff, 0x4},  {0x2a, 0x1},  {0x40, 0x1},  {0xff, 0x11},
    {0xa1, 0x1},  {0x0, 0x4},   {0x55, 0x1},  {0x6a, 0x1},  {0x40, 0x1},  {0x2a, 0x1},  {0x0, 0x4},
    {0x6a, 0x1},  {0xff, 0x2},  {0x6a, 0x1},  {0x0, 0x4},   {0xf8, 0x1},  {0xff, 0xc},  {0x40, 0x1},
    {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x9},  {0x80, 0x1},  {0x0, 0x1},   {0xc9, 0x1},  {0xff, 0x2},
    {0xc9, 0x1},  {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x11}, {0xa1, 0x1},  {0x0, 0x4},   {0xff, 0x3},
    {0xf8, 0x1},  {0x6a, 0x1},  {0x0, 0x4},   {0xc0, 0x1},  {0xff, 0x1},  {0xa1, 0x1},  {0x0, 0x4},
    {0xa1, 0x1},  {0xff, 0xc},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0x9},  {0xf8, 0x1},
    {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x2},  {0x80, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x11},
    {0xa1, 0x1},  {0x0, 0x4},   {0xf8, 0x1},  {0xff, 0x3},  {0xc9, 0x1},  {0x0, 0x4},   {0x80, 0x1},
    {0xff, 0x1},  {0xc9, 0x1},  {0x0, 0x4},   {0x2a, 0x1},  {0xf8, 0x1},  {0xff, 0xb},  {0x40, 0x1},
    {0x0, 0x3},   {0x55, 0x1},  {0xff, 0xa},  {0x55, 0x1},  {0x2a, 0x1},  {0xff, 0x2},  {0x40, 0x2},
    {0xff, 0x12}, {0xa1, 0x1},  {0x0, 0x4},   {0xff, 0x4},  {0x80, 0x1},  {0x0, 0x4},   {0xa1, 0x1},
    {0xff, 0x2},  {0x55, 0x1},  {0x0, 0x4},   {0x2a, 0x1},  {0xc0, 0x1},  {0xff, 0x5},  {0xf8, 0x1},
    {0xff, 0x4},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0xa},  {0xc0, 0x1},  {0x0, 0x1},
    {0xc0, 0x1},  {0xff, 0x1},  {0x0, 0x1},   {0x6a, 0x1},  {0xff, 0x12}, {0xa1, 0x1},  {0x0, 0x4},
    {0xa1, 0x3},  {0x55, 0x1},  {0x0, 0x5},   {0xf8, 0x1},  {0xff, 0x2},  {0xf8, 0x1},  {0x2a, 0x1},
    {0x0, 0x5},   {0x2a, 0x1},  {0x55, 0x1},  {0x6a, 0x2},  {0x40, 0x1},  {0x2a, 0x1},  {0xa1, 0x1},
    {0xff, 0x3},  {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0xa},  {0xf8, 0x1},  {0x0, 0x1},
    {0x80, 0x1},  {0xff, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x12}, {0xa1, 0x1},  {0x0, 0xc},
    {0xc0, 0x1},  {0xff, 0x4},  {0xc9, 0x1},  {0x40, 0x1},  {0x0, 0xa},   {0x6a, 0x1},  {0xff, 0x3},
    {0x40, 0x1},  {0x0, 0x3},   {0x55, 0x1},  {0xff, 0xb},  {0x40, 0x1},  {0x2a, 0x1},  {0xff, 0x1},
    {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x12}, {0xa1, 0x1},  {0x0, 0x9},   {0x2a, 0x1},  {0x6a, 0x1},
    {0xc9, 0x1},  {0xff, 0x7},  {0xa1, 0x1},  {0x40, 0x1},  {0x0, 0x8},   {0x40, 0x1},  {0xff, 0x3},
    {0x40, 0x1},  {0x0, 0x3},   {0x40, 0x1},  {0xff, 0xb},  {0xa1, 0x1},  {0x0, 0x1},   {0xf8, 0x1},
    {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x12}, {0xf8, 0x1},  {0xa1, 0x3},  {0x80, 0x3},  {0xa1, 0x1},
    {0xc0, 0x1},  {0xc9, 0x1},  {0xf8, 0x1},  {0xff, 0xb},  {0xc9, 0x1},  {0xa1, 0x1},  {0x80, 0x4},
    {0xa1, 0x1},  {0xc0, 0x1},  {0xf8, 0x1},  {0xff, 0x3},  {0xc0, 0x1},  {0xa1, 0x3},  {0xc0, 0x1},
    {0xff, 0xb},  {0xc9, 0x1},  {0x0, 0x1},   {0xa1, 0x1},  {0x0, 0x1},   {0x55, 0x1},  {0xff, 0x44},
    {0xf8, 0x1},  {0x0, 0x1},   {0x6a, 0x1},  {0x55, 0x1},  {0x2a, 0x1},  {0xff, 0x45}, {0x40, 0x2},
    {0xc0, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0x12}, {0xf8, 0x1},  {0x80, 0x1},  {0x40, 0x1},
    {0x2a, 0x2},  {0x55, 0x1},  {0xc0, 0x1},  {0xff, 0x4},  {0xc9, 0x1},  {0x55, 0x1},  {0x2a, 0x2},
    {0x55, 0x1},  {0xc0, 0x1},  {0xff, 0x4},  {0xc0, 0x1},  {0x55, 0x1},  {0x2a, 0x2},  {0x6a, 0x1},
    {0xc9, 0x1},  {0xff, 0x4},  {0xc0, 0x1},  {0x40, 0x1},  {0x2a, 0x2},  {0x6a, 0x1},  {0xf8, 0x1},
    {0xff, 0xd},  {0x6a, 0x1},  {0x0, 0x1},   {0xff, 0x1},  {0x2a, 0x1},  {0x40, 0x1},  {0xff, 0x12},
    {0x2a, 0x1},  {0x0, 0x6},   {0xa1, 0x1},  {0xff, 0x2},  {0xc9, 0x1},  {0x0, 0x6},   {0xc0, 0x1},
    {0xff, 0x2},  {0xc0, 0x1},  {0x0, 0x5},   {0x2a, 0x1},  {0xc9, 0x1},  {0xff, 0x2},  {0xa1, 0x1},
    {0x0, 0x5},   {0x2a, 0x1},  {0xf8, 0x1},  {0xff, 0xc},  {0x80, 0x1},  {0x0, 0x1},   {0xff, 0x1},
    {0xa1, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0x11}, {0x6a, 0x1},  {0x0, 0x1},   {0x6a, 0x1},
    {0x80, 0x1},  {0x40, 0x1},  {0x0, 0x2},   {0x2a, 0x1},  {0xff, 0x2},  {0x40, 0x1},  {0x0, 0x2},
    {0x80, 0x2},  {0x0, 0x2},   {0x40, 0x1},  {0xff, 0x1},  {0xf8, 0x1},  {0x2a, 0x1},  {0x0, 0x2},
    {0x80, 0x1},  {0x55, 0x1},  {0x0, 0x2},   {0x55, 0x1},  {0xff, 0x1},  {0xf8, 0x1},  {0x0, 0x3},
    {0x80, 0x1},  {0x40, 0x1},  {0x0, 0x2},   {0x80, 0x1},  {0xff, 0xc},  {0xa1, 0x1},  {0x0, 0x1},
    {0xff, 0x2},  {0x40, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x10}, {0xc9, 0x2},  {0xff, 0x2},
    {0xf8, 0x1},  {0x0, 0x3},   {0xf8, 0x1},  {0xc9, 0x1},  {0x0, 0x2},   {0x40, 0x1},  {0xff, 0x2},
    {0x2a, 0x1},  {0x0, 0x2},   {0xc9, 0x1},  {0xc0, 0x1},  {0x0, 0x2},   {0x55, 0x1},  {0xff, 0x1},
    {0xf8, 0x1},  {0x0, 0x3},   {0xf8, 0x1},  {0xa1, 0x1},  {0x0, 0x2},   {0x80, 0x1},  {0xff, 0x1},
    {0xc9, 0x1},  {0x0, 0x2},   {0x2a, 0x1},  {0xff, 0xc},  {0xa1, 0x1},  {0x0, 0x1},   {0xff, 0x2},
    {0xf8, 0x1},  {0x2a, 0x2},  {0xf8, 0x1},  {0xff, 0x13}, {0xf8, 0x1},  {0x0, 0x3},   {0xf8, 0x1},
    {0xa1, 0x1},  {0x0, 0x2},   {0x6a, 0x1},  {0xff, 0x2},  {0x6a, 0x1},  {0x0, 0x2},   {0xa1, 0x1},
    {0x80, 0x1},  {0x0, 0x2},   {0xa1, 0x1},  {0xff, 0x2},  {0x40, 0x1},  {0x0, 0x2},   {0xc0, 0x1},
    {0x6a, 0x1},  {0x0, 0x2},   {0xc0, 0x1},  {0xff, 0x2},  {0x2a, 0x1},  {0x0, 0x2},   {0xc9, 0x1},
    {0xff, 0xb},  {0xa1, 0x1},  {0x0, 0x1},   {0xff, 0x3},  {0xc9, 0x1},  {0x0, 0x1},   {0x2a, 0x1},
    {0xc0, 0x1},  {0xff, 0x12}, {0x80, 0x1},  {0x0, 0x2},   {0x40, 0x1},  {0xff, 0x1},  {0x80, 0x1},
    {0x0, 0x2},   {0x80, 0x1},  {0xff, 0x2},  {0x80, 0x1},  {0x0, 0x2},   {0xa1, 0x1},  {0x80, 0x1},
    {0x0, 0x2},   {0xa1, 0x1},  {0xff, 0x2},  {0x55, 0x1},  {0x0, 0x2},   {0xa1, 0x1},  {0x40, 0x1},
    {0x0, 0x2},   {0xc0, 0x1},  {0xff, 0x2},  {0x40, 0x1},  {0x0, 0x2},   {0xc0, 0x1},  {0xff, 0xb},
    {0x80, 0x1},  {0x0, 0x1},   {0xff, 0x4},  {0xc9, 0x1},  {0x40, 0x1},  {0x0, 0x1},   {0x6a, 0x1},
    {0xf8, 0x1},  {0xff, 0xf},  {0xc0, 0x1},  {0x0, 0x3},   {0xc9, 0x1},  {0xff, 0x1},  {0x80, 0x1},
    {0x0, 0x2},   {0x80, 0x1},  {0xff, 0x2},  {0x80, 0x1},  {0x0, 0x2},   {0xa1, 0x1},  {0x6a, 0x1},
    {0x0, 0x2},   {0xa1, 0x1},  {0xff, 0x2},  {0x55, 0x1},  {0x0, 0x2},   {0xa1, 0x1},  {0x40, 0x1},
    {0x0, 0x2},   {0xc0, 0x1},  {0xff, 0x2},  {0x40, 0x1},  {0x0, 0x2},   {0xc0, 0x1},  {0xff, 0xb},
    {0x55, 0x1},  {0x2a, 0x1},  {0xff, 0x6},  {0x80, 0x1},  {0x0, 0x2},   {0x6a, 0x1},  {0xc0, 0x1},
    {0xff, 0xc},  {0xc0, 0x1},  {0x0, 0x3},   {0xa1, 0x1},  {0xff, 0x2},  {0xa1, 0x1},  {0x0, 0x2},
    {0x80, 0x1},  {0xff, 0x2},  {0x6a, 0x1},  {0x0, 0x2},   {0xa1, 0x1},  {0x80, 0x1},  {0x0, 0x2},
    {0xa1, 0x1},  {0xff, 0x2},  {0x40, 0x1},  {0x0, 0x2},   {0xc0, 0x1},  {0x55, 0x1},  {0x0, 0x2},
    {0xc0, 0x1},  {0xff, 0x2},  {0x2a, 0x1},  {0x0, 0x2},   {0xc9, 0x1},  {0xff, 0xb},  {0x2a, 0x1},
    {0x40, 0x1},  {0xff, 0x7},  {0xf8, 0x1},  {0x80, 0x1},  {0x0, 0x2},   {0x2a, 0x1},  {0x6a, 0x1},
    {0xa1, 0x1},  {0xc9, 0x1},  {0xff, 0x7},  {0xa1, 0x1},  {0x0, 0x3},   {0xc0, 0x1},  {0xff, 0x3},
    {0xc0, 0x1},  {0x0, 0x2},   {0x40, 0x1},  {0xff, 0x2},  {0x40, 0x1},  {0x0, 0x2},   {0xc9, 0x1},
    {0xa1, 0x1},  {0x0, 0x2},   {0x6a, 0x1},  {0xff, 0x2},  {0x2a, 0x1},  {0x0, 0x2},   {0xf8, 0x1},
    {0x80, 0x1},  {0x0, 0x2},   {0x80, 0x1},  {0xff, 0x1},  {0xf8, 0x1},  {0x0, 0x2},   {0x2a, 0x1},
    {0xf8, 0x1},  {0xff, 0xa},  {0xc9, 0x1},  {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x9},  {0xf8, 0x1},
    {0xa1, 0x1},  {0x55, 0x1},  {0x2a, 0x1},  {0x0, 0x2},   {0x2a, 0x1},  {0x80, 0x1},  {0xff, 0x4},
    {0x80, 0x1},  {0x0, 0x3},   {0x40, 0x1},  {0x80, 0x3},  {0xc9, 0x1},  {0xff, 0x1},  {0x2a, 0x1},
    {0x0, 0x2},   {0xa1, 0x2},  {0x0, 0x2},   {0x2a, 0x1},  {0xff, 0x1},  {0xf8, 0x1},  {0x0, 0x3},
    {0xc0, 0x1},  {0x80, 0x1},  {0x0, 0x2},   {0x40, 0x1},  {0xff, 0x1},  {0xc9, 0x1},  {0x0, 0x2},
    {0x2a, 0x1},  {0xc0, 0x1},  {0x6a, 0x1},  {0x0, 0x2},   {0x6a, 0x1},  {0xff, 0xb},  {0x80, 0x1},
    {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0xd},  {0xc9, 0x1},  {0xa1, 0x1},  {0x6a, 0x1},  {0x0, 0x1},
    {0x6a, 0x1},  {0xff, 0x2},  {0xc9, 0x1},  {0x0, 0x8},   {0xa1, 0x1},  {0xff, 0x1},  {0xa1, 0x1},
    {0x0, 0x6},   {0xa1, 0x1},  {0xff, 0x2},  {0x80, 0x1},  {0x0, 0x6},   {0xc0, 0x1},  {0xff, 0x2},
    {0x6a, 0x1},  {0x0, 0x6},   {0xc9, 0x1},  {0xff, 0xa},  {0xf8, 0x1},  {0x0, 0x1},   {0x55, 0x1},
    {0xff, 0x11}, {0x80, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x1},  {0xc9, 0x1},  {0x2a, 0x8},
    {0xc0, 0x1},  {0xff, 0x2},  {0xa1, 0x1},  {0x2a, 0x1},  {0x0, 0x2},   {0x2a, 0x1},  {0xa1, 0x1},
    {0xff, 0x4},  {0x80, 0x1},  {0x2a, 0x1},  {0x0, 0x2},   {0x40, 0x1},  {0xc0, 0x1},  {0xff, 0x4},
    {0x80, 0x1},  {0x2a, 0x1},  {0x0, 0x2},   {0x40, 0x1},  {0xc0, 0x1},  {0xff, 0xb},  {0x80, 0x1},
    {0x0, 0x1},   {0xc9, 0x1},  {0xff, 0x11}, {0xc9, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0x2},
    {0xf8, 0x8},  {0xff, 0x5},  {0xc9, 0x2},  {0xff, 0x7},  {0xf8, 0x1},  {0xc9, 0x2},  {0xff, 0x7},
    {0xf8, 0x1},  {0xc9, 0x2},  {0xff, 0xc},  {0xc0, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x12},
    {0xf8, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x30}, {0xc0, 0x1},  {0x0, 0x1},   {0x40, 0x1},
    {0xff, 0x14}, {0x6a, 0x1},  {0x0, 0x1},   {0xc9, 0x1},  {0xff, 0x2e}, {0xc0, 0x1},  {0x0, 0x1},
    {0x40, 0x1},  {0xf8, 0x1},  {0xff, 0x14}, {0xf8, 0x1},  {0x2a, 0x2},  {0xf8, 0x1},  {0xff, 0x2c},
    {0xf8, 0x1},  {0x2a, 0x1},  {0x40, 0x1},  {0xff, 0x17}, {0xc9, 0x1},  {0x0, 0x1},   {0x2a, 0x1},
    {0xf8, 0x1},  {0xff, 0x2b}, {0xc0, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0x18}, {0xc9, 0x1},
    {0x2a, 0x2},  {0xc0, 0x1},  {0xff, 0x2a}, {0xa1, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x19},
    {0xc9, 0x1},  {0x40, 0x1},  {0x0, 0x1},   {0x40, 0x1},  {0xa1, 0x1},  {0xc9, 0x1},  {0xf8, 0x1},
    {0xff, 0x26}, {0xa1, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x1b}, {0xa1, 0x1},  {0x40, 0x1},
    {0x0, 0x3},   {0x2a, 0x1},  {0x55, 0x1},  {0x80, 0x1},  {0xc0, 0x1},  {0xff, 0x22}, {0x80, 0x1},
    {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x1d}, {0xf8, 0x1},  {0xc0, 0x1},  {0x80, 0x1},  {0x55, 0x1},
    {0x2a, 0x1},  {0x0, 0x2},   {0x2a, 0x1},  {0xa1, 0x1},  {0xff, 0x20}, {0x40, 0x1},  {0x2a, 0x1},
    {0xff, 0x23}, {0xf8, 0x1},  {0xc0, 0x1},  {0x55, 0x1},  {0x0, 0x1},   {0x40, 0x1},  {0xf8, 0x1},
    {0xff, 0x1d}, {0xc9, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x26}, {0xc0, 0x1},  {0x0, 0x1},
    {0x40, 0x1},  {0xff, 0x1d}, {0x55, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x27}, {0xc0, 0x1},
    {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x1b}, {0xa1, 0x1},  {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x29},
    {0x80, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0x19}, {0xc0, 0x1},  {0x0, 0x1},   {0x55, 0x1},
    {0xff, 0x2b}, {0x2a, 0x2},  {0xff, 0x18}, {0xc9, 0x1},  {0x0, 0x1},   {0x2a, 0x1},  {0xf8, 0x1},
    {0xff, 0x2b}, {0xc0, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xff, 0x16}, {0xc9, 0x1},  {0x2a, 0x2},
    {0xf8, 0x1},  {0xff, 0x2d}, {0x80, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0x13}, {0xf8, 0x1},
    {0xa1, 0x1},  {0x0, 0x1},   {0x2a, 0x1},  {0xc9, 0x1},  {0xff, 0x2e}, {0xf8, 0x1},  {0x2a, 0x1},
    {0x40, 0x1},  {0xff, 0x11}, {0xc9, 0x1},  {0x80, 0x1},  {0x2a, 0x1},  {0x0, 0x1},   {0x6a, 0x1},
    {0xf8, 0x1},  {0xff, 0x30}, {0xc0, 0x1},  {0x0, 0x1},   {0x80, 0x1},  {0xff, 0xa},  {0xf8, 0x1},
    {0xc9, 0x1},  {0xa1, 0x1},  {0x80, 0x1},  {0x40, 0x1},  {0x2a, 0x1},  {0x0, 0x2},   {0x55, 0x1},
    {0xc0, 0x1},  {0xff, 0x33}, {0x6a, 0x1},  {0x0, 0x1},   {0xc9, 0x1},  {0xff, 0x8},  {0x80, 0x1},
    {0x2a, 0x1},  {0x0, 0x3},   {0x40, 0x1},  {0x6a, 0x1},  {0xa1, 0x1},  {0xf8, 0x1},  {0xff, 0x35},
    {0xf8, 0x1},  {0x2a, 0x1},  {0x40, 0x1},  {0xff, 0x8},  {0x6a, 0x1},  {0x0, 0x1},   {0xa1, 0x1},
    {0xc9, 0x1},  {0xf8, 0x1},  {0xff, 0x3a}, {0xc0, 0x1},  {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x7},
    {0xc9, 0x1},  {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x3d}, {0x55, 0x1},  {0x0, 0x1},   {0xf8, 0x1},
    {0xff, 0x7},  {0x55, 0x1},  {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x3c}, {0xf8, 0x1},  {0x0, 0x1},
    {0x40, 0x1},  {0xff, 0x7},  {0xc9, 0x1},  {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x3d}, {0xa1, 0x1},
    {0x0, 0x1},   {0xa1, 0x1},  {0xff, 0x7},  {0x40, 0x1},  {0x2a, 0x1},  {0xff, 0x3e}, {0x40, 0x1},
    {0x0, 0x1},   {0xf8, 0x1},  {0xff, 0x6},  {0xc0, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0x3d},
    {0xc9, 0x1},  {0x0, 0x1},   {0x6a, 0x1},  {0xff, 0x6},  {0xc9, 0x1},  {0x0, 0x1},   {0x40, 0x1},
    {0xff, 0x3e}, {0xa1, 0x1},  {0x0, 0x1},   {0xc0, 0x1},  {0xff, 0x1},  {0xf8, 0x1},  {0xc0, 0x1},
    {0x6a, 0x1},  {0x2a, 0x1},  {0x0, 0x2},   {0x40, 0x1},  {0xf8, 0x1},  {0xff, 0x3e}, {0x40, 0x1},
    {0x2a, 0x1},  {0x40, 0x1},  {0x0, 0x2},   {0x2a, 0x1},  {0x55, 0x1},  {0xa1, 0x1},  {0xc9, 0x1},
    {0xff, 0x40}, {0xc0, 0x1},  {0x0, 0x1},   {0x40, 0x1},  {0x80, 0x1},  {0xc0, 0x1},  {0xff, 0x18},
    {0x0, 0x0} // terminating entry
};
} // namespace Resources
