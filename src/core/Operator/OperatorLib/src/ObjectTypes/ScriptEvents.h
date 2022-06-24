////////////////////////////////////////////////////////////////////////////////
// $Id: ScriptEvents.h 6484 2022-01-03 16:59:03Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: Operator script event names, and a container for scripts.
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
#ifndef SCRIPT_EVENTS
#define SCRIPT_EVENTS

#include "Lockable.h"
#include <map>
#include <string>

class ScriptEvents : private std::map<int, std::string>, Lockable<std::mutex>
{
  public:
    ScriptEvents &Set(int inEventCallbackID, const std::string &);
    std::string Get(int inEventCallbackID) const;

    static const char *Name(int inEventCallbackID);
    static int ID(const std::string &inName);
};

#endif // SCRIPT_EVENTS
