////////////////////////////////////////////////////////////////////////////////
// $Id: OperatorUtils.cpp 6657 2022-04-01 19:48:41Z mellinger $
// Description: A file intended to hold global utility functions common to
//              various operator source files.
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
#include "OperatorUtils.h"
#include "ClassName.h"
#include "Param.h"
#include "ParamList.h"
#include "Preferences.h"
#include "Settings.h"

#include <QApplication>
#include <QDir>
#include <QWidget>
#include <QtGlobal>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <typeinfo>

extern Preferences *gpPreferences;

static const QString KeyConfig = "Config";
static const QString KeyParameters = "Parameters";

void OperatorUtils::PutPersistentData(const char *inName, const QByteArray &inData)
{
    Settings settings;
    settings.beginGroup(KeyConfig);
    settings.beginGroup(inName);
    settings.setValue("Data", inData);
}

void OperatorUtils::GetPersistentData(const char *inName, QByteArray &outData)
{
    Settings settings;
    settings.beginGroup(KeyConfig);
    settings.beginGroup(inName);
    outData = settings.value("Data").toByteArray();
}

void OperatorUtils::SaveWidget(const QWidget *inWidget, const QByteArray *inState)
{
    Settings settings;
    settings.beginGroup(KeyConfig);
    settings.beginGroup(ClassName(typeid(*inWidget)).c_str());
    settings.setValue("Geometry", inWidget->saveGeometry());
    if (inState)
        settings.setValue("State", *inState);
}

bool OperatorUtils::RestoreWidget(QWidget *inWidget, QByteArray *outState)
{
    Settings settings;
    settings.beginGroup(KeyConfig);
    settings.beginGroup(ClassName(typeid(*inWidget)).c_str());
    QByteArray geometry = settings.value("Geometry", QByteArray()).toByteArray();
    bool ok = !geometry.isEmpty();
    if (ok)
        inWidget->restoreGeometry(geometry);
    if (outState)
        *outState = settings.value("State").toByteArray();
    return ok;
}

QFileInfo OperatorUtils::AppRelativeToAbsolutePath(const QString &inPath)
{
    QString path = inPath;
    if (!QFileInfo(inPath).isAbsolute())
        path.prepend(QDir::separator()).prepend(QCoreApplication::applicationDirPath());
    QFileInfo result(path);
    Q_ASSERT(result.isAbsolute());
    return result;
}

QDir &OperatorUtils::CurrentDir()
{
    if (gpPreferences != NULL)
        return gpPreferences->mCurrentDir;
    static QDir dir;
    return dir;
}

int OperatorUtils::UserLevel()
{
    if (gpPreferences != NULL)
        return gpPreferences->mUserLevel;
    return Preferences::Beginner;
}

// retrieves the user level of one particular parameter
int OperatorUtils::GetUserLevel(const char *inName)
{
    Settings settings;
    settings.beginGroup(KeyParameters + "/" + inName);
    return settings.value("UserLevel", Preferences::Advanced).toInt();
}

// sets the user level of one particular parameter
void OperatorUtils::SetUserLevel(const char *inName, int inUserlevel)
{
    Settings settings;
    settings.beginGroup(KeyParameters + "/" + inName);
    settings.setValue("UserLevel", inUserlevel);
}

// retrieves the filter status of one particular parameter
// filtertype == 1 ... load filter
// filtertype == 2 ... save filter
// returns 0 (not set) or 1 (set)
int OperatorUtils::GetFilterStatus(const char *inName, int inFiltertype)
{
    Settings settings;
    int result = 0;
    settings.beginGroup(KeyParameters + "/" + inName);
    switch (inFiltertype)
    {
    case saveFilter:
        result = settings.value("SaveFilter", 0).toInt();
        break;
    case loadFilter:
        result = settings.value("LoadFilter", 0).toInt();
        break;
    }
    return result;
}

// sets the filter status of one particular parameter
// filtertype ... see GetFilterStatus
// filterstatus ... 0 ... not set; 1 ... set
void OperatorUtils::SetFilterStatus(const char *inName, int inFiltertype, int inFilterstatus)
{
    Settings settings;
    settings.beginGroup(KeyParameters + "/" + inName);
    switch (inFiltertype)
    {
    case saveFilter:
        settings.setValue("SaveFilter", inFilterstatus);
        break;
    case loadFilter:
        settings.setValue("LoadFilter", inFilterstatus);
        break;
    }
}

int OperatorUtils::LoadMatrix(const QString &inFileName, Param &outParam)
{
    if (inFileName.isEmpty())
        return MatNotFound;

    if (QFileInfo(inFileName).suffix() == MATRIX_EXTENSION)
    {
        ParamList paramsFromFile;
        paramsFromFile.Load(inFileName.toLocal8Bit().constData(), true);
        if (paramsFromFile.Size() == 0)
            return MatNotFound;
        if (paramsFromFile.Size() > 1)
            return MatMultipleParams;
        Param p = paramsFromFile.ByIndex(0);
        outParam.SetDimensions(p.NumRows(), p.NumColumns());
        for (int row = 0; row < p.NumRows(); ++row)
            for (int col = 0; col < p.NumColumns(); ++col)
                outParam.Value(row, col) = p.Value(row, col);
        outParam.RowLabels() = p.RowLabels();
        outParam.ColumnLabels() = p.ColumnLabels();
    }
    else
    {
        std::ifstream input(inFileName.toLocal8Bit());
        input.clear();
        std::vector<std::vector<std::string>> matrix;
        std::string line;
        while (std::getline(input, line))
        {
            std::istringstream is(line);
            std::vector<std::string> row;
            std::string value;
            while (std::getline(is, value, '\t'))
                row.push_back(value);
            if (!row.empty())
                matrix.push_back(row);
        }
        if (matrix.empty())
            return MatNotFound;

        size_t numRows = matrix.size(), numCols = matrix[0].size();
        for (size_t row = 1; row < numRows; ++row)
            if (matrix[row].size() != numCols)
                return MatLoadColsDiff;

        outParam.SetDimensions(numRows, numCols);
        for (size_t row = 0; row < numRows; ++row)
            for (size_t col = 0; col < numCols; ++col)
            {
                std::istringstream iss(matrix[row][col]);
                iss >> outParam.Value(row, col);
            }
    }
    return NoError;
}

int OperatorUtils::SaveMatrix(const QString &inFileName, const Param &inParam)
{
    if (inFileName.isEmpty())
        return CouldNotWrite;

    bool saveAsMatrix = (QFileInfo(inFileName).suffix() == MATRIX_EXTENSION);
    if (!saveAsMatrix)
    {
        bool isNested = false;
        for (int row = 0; row < inParam.NumRows(); ++row)
            for (int col = 0; col < inParam.NumColumns(); ++col)
                isNested = isNested | (inParam.Value(row, col).Kind() != Param::ParamValue::Single);

        if (isNested)
            return CannotWriteNestedMatrixAsText;
    }

    std::ofstream output(inFileName.toLocal8Bit());
    if (saveAsMatrix)
    {
        output << inParam;
    }
    else
    {
        for (int row = 0; row < inParam.NumRows(); ++row)
        {
            int col = 0;
            while (col < inParam.NumColumns() - 1)
                output << inParam.Value(row, col++) << '\t';
            output << inParam.Value(row, col) << std::endl;
        }
    }
    return output ? NoError : CouldNotWrite;
}
