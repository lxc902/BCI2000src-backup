////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A simple INI file implementation.
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
#ifndef INIFILE_H
#define INIFILE_H

#include <map>
#include <string>
#include <iostream>

class IniFile
{
public:
  IniFile(const std::string& filename);
  ~IniFile();

  class Value
  {
  public:
    Value();
    Value(const std::string&);
    Value(double);
    const std::string& ToString() const;
    operator const std::string&() const;
    double ToDouble() const;
    operator double() const;
    bool Empty() const;
  private:
    std::string mStorage;
  };

  class Group
  {
  public:
    bool Empty() const;
    const Value& Value(const std::string&) const;
    class Value& Value(const std::string&);
    const class Value& operator[](const std::string&) const;
    class Value& operator[](const std::string&);
    std::ostream& Serialize(std::ostream&) const;
    std::istream& Unserialize(std::istream&);
  private:
    std::map<std::string, class Value> mStorage;
  };

  const Group& Group(const std::string&) const;
  class Group& Group(const std::string&);

  const class Group& operator[](const std::string&) const;
  class Group& operator[](const std::string&);

  std::ostream& Serialize(std::ostream&) const;
  std::istream& Unserialize(std::istream&);

private:
  std::string mFilename;
  std::map<std::string, class Group> mStorage;
};

#endif // INIFILE_H
