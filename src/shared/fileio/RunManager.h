////////////////////////////////////////////////////////////////////////////////
// $Id: RunManager.h 6484 2022-01-03 16:59:03Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that manages BCI2000 data file names.
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
#include "Environment.h"
#include "Files.h"
#include <string>

class RunManager : private EnvironmentExtension
{
  public:
    RunManager();
    ~RunManager();
    std::string CurrentSession();    // path to data file without run and extension
    const std::string &CurrentRun(); // path to data file, including extension
    File &CurrentRunFile();

    static std::string CurrentSession(ParamList &);

  protected:
    void Publish();
    void Preflight() const;
    void Initialize();
    void StopRun();

  private:
    std::string ConstructRun();
    static std::string DataDirectory(ParamList &);
    static std::string SubstituteParameters(const std::string &, const ParamList &);

    bool mPublished, mInWritingModule;
    mutable File mCurrentRunFile;
    mutable std::string mCurrentRun;
};
