////////////////////////////////////////////////////////////////////////////////
// $Id: RunManager.cpp 6584 2022-02-23 15:07:43Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class that manages BCI2000 data file names.
//   Some of the functionality here consists of workarounds that will no longer
//   be necessary in 4.x (which will introduce publication of output file name).
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
#include "RunManager.h"

#include "BCIStream.h"
#include "FilePath.h"
#include "FileUtils.h"
#include "GenericFilter.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

static const std::string sbTag = "${", seTag = "}";

static std::string IntToString(int inNumber, int inWidth)
{
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(inWidth) << inNumber;
    return oss.str();
}

static int StringToInt(const std::string &inString)
{
    return ::atoi(inString.c_str());
}

RunManager::RunManager() : mPublished(false), mInWritingModule(false)
{
}

RunManager::~RunManager()
{
    if (mCurrentRunFile.Length() == 0)
    {
        mCurrentRunFile.Close();
        FileUtils::RemoveFile(mCurrentRun);
    }
}

std::string RunManager::CurrentSession()
{
    return CurrentSession(*Parameters);
}

const std::string &RunManager::CurrentRun()
{
    if (!mInWritingModule)
        bcierr << "Cannot obtain current run outside writing module";
    else if (!mPublished)
        bcierr << "Trying to access current run file, which is undefined";
    else if (!mCurrentRunFile.IsOpen())
        mCurrentRun = ConstructRun();
    return mCurrentRun;
}

Tiny::File &RunManager::CurrentRunFile()
{
    CurrentRun();
    return mCurrentRunFile;
}

void RunManager::Publish()
{
#ifdef TODO
#error Remove this test from V4
#endif
    GenericFilter::ChainInfo info = GenericFilter::GetChainInfo();
    for (GenericFilter::ChainInfo::const_iterator i = info.begin(); i != info.end(); ++i)
        mInWritingModule |= !::stricmp(i->name.c_str(), "DataIOFilter");

    if (mInWritingModule)
    {
        bool existed = Parameters->Exists("DataFile");
        BEGIN_PARAMETER_DEFINITIONS
          "Storage:Data%20Location string DataFile= % % % % "
             "// name of data file relative to DataDirectory",
        END_PARAMETER_DEFINITIONS
        if (!existed)
            Parameter("DataFile") = "${SubjectName}${SubjectSession}/${SubjectName}S${SubjectSession}R${SubjectRun}.${FileFormat}";
    }
    mPublished = true;
}

void RunManager::Preflight() const
{
    if (mCurrentRunFile.Length() == 0)
    {
        mCurrentRunFile.Close();
        FileUtils::RemoveFile(mCurrentRun);
    }
    else if (mCurrentRunFile.Length() > 0)
    {
        mCurrentRunFile.Close();
    }
}

void RunManager::Initialize()
{
}

void RunManager::StopRun()
{
    mCurrentRunFile.Close();
}

std::string RunManager::DataDirectory(ParamList &inParams)
{
    Param &session = inParams.ByPath("SubjectSession");
    session.Value() = IntToString(StringToInt(session.Value()), 3);
    Param &run = inParams.ByPath("SubjectRun");
    run.Value() = IntToString(StringToInt(run.Value()), 2);

    Tiny::FilePath path = inParams.ByPath("DataDirectory").Value().AsString();
    path.ToAbsolute(FileUtils::InstallationDirectory()).Simplify();
    return path.ToString() + path.NativeSeparator();
}

std::string RunManager::CurrentSession(ParamList &inParams)
{
    std::string dataDir = DataDirectory(inParams), file = inParams.ByPath("DataFile").Value(),
                dir = FileUtils::ExtractDirectory(file), runRef = sbTag + "SubjectRun" + seTag;
    file = FileUtils::ExtractBase(file);

    size_t pos;
    while (std::string::npos != (pos = file.find(runRef)))
    {
        size_t i = pos;
        while (i > 0 && ::isalpha(file[i - 1]))
            --i;
        file = file.substr(0, i) + file.substr(pos + runRef.length());
    }
    if (file.empty())
        file = "Session${SubjectSession}";

    while (std::string::npos != (pos = dir.find(runRef)))
        dir = dir.substr(0, pos) + "_MultipleRuns_" + dir.substr(pos + runRef.length());

    return dataDir + SubstituteParameters(dir + file, inParams);
}

std::string RunManager::ConstructRun()
{
    ParamList &parameters = *Parameters;
    std::string path = DataDirectory(parameters), first, file, result;
    int run = ::atoi(parameters.ByPath("SubjectRun").Value().c_str());
    if (--run < 0)
        run = 0;
    bool done = false, ok = true;
    while (ok && !done)
    {
        parameters.ByPath("SubjectRun").Value() = IntToString(++run, 2);
        file = SubstituteParameters(parameters.ByPath("DataFile").Value(), *Parameters);
        if (first.empty())
            first = file;
        else if (file == first)
        {
            std::string ext = FileUtils::ExtractExtension(file);
            file = FileUtils::StripExtension(file) + IntToString(run, 0) + ext;
        }
        result = path + file;

        std::string dir = FileUtils::ExtractDirectory(result);
        if (!FileUtils::MakeDirectory(dir, true))
        {
            ok = false;
            bcierr << "Could not create directory: " << dir;
        }
        else
        {
            done = mCurrentRunFile.Open(result, File::out | File::exclusive | File::nodelete);
            ok = FileUtils::IsFile(result); // ok but not done if file existed
            if (!ok)
                bcierr << "Could not create output file: " << result;
        }
    }
    if (ok)
        result = FileUtils::NormalizedPath(result);
    return result;
}

std::string RunManager::SubstituteParameters(const std::string &inExpr, const ParamList &inParams)
{
    std::set<std::string> occurred;

    std::string result = inExpr;
    size_t beg;
    while (std::string::npos != (beg = result.find(sbTag)))
    {
        size_t end = result.find(seTag, beg + sbTag.length());
        if (end != std::string::npos)
        {
            std::string name = result.substr(beg, end - beg);
            name = name.substr(sbTag.length());
            std::string value;
            if (occurred.find(name) != occurred.end())
                bcierr << "Circular reference to parameter " << name;
            else if (inParams.Exists(name))
                value = inParams.ByPath(name).Value();
            result = result.substr(0, beg) + value + result.substr(end + 1);
        }
    }
    return result;
}
