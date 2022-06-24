////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A wrapper for QFileDialog methods that saves current
//   directories based on file type.
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
#include "FileDialog.h"

#include "Settings.h"
#include <QApplication>
#include <QFileDialog>

namespace
{
const char *sSettingsKey = "Directories";

QString sDefaultDirectory = QApplication::applicationDirPath();

struct DirectoryMap : std::map<QString, QString>
{
    DirectoryMap();
    ~DirectoryMap();
} sDirectoryMap;

DirectoryMap::DirectoryMap()
{
    Settings settings;
    int size = settings.beginReadArray(sSettingsKey);
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        QString mask = settings.value("type").toString();
        QString dir = settings.value("directory").toString();
        sDirectoryMap[mask] = dir;
    }
    settings.endArray();
}

DirectoryMap::~DirectoryMap()
{
    Settings settings;
    settings.beginWriteArray(sSettingsKey);
    int i = 0;
    for (const auto &pair : sDirectoryMap)
    {
        settings.setArrayIndex(i++);
        settings.setValue("type", pair.first);
        settings.setValue("directory", pair.second);
    }
    settings.endArray();
}
} // namespace

void FileDialog::setDefaultDirectory(const QString &directory)
{
    sDefaultDirectory = directory;
}

QList<QString> FileDialog::getFileNames(Kind kind, QWidget *pParent, const QString &title, const QString &mask,
                                        const QString &category, const QString &name)
{
    QList<QString> files;
    QString key = category.isEmpty() ? mask : category;
    QString file = sDirectoryMap[key];
    if (file.isEmpty())
        file = sDefaultDirectory;
    if (!name.isEmpty())
        file += "/" + name;
    switch (kind)
    {
    case open:
        file = QFileDialog::getOpenFileName(pParent, title, file, mask);
        files << file;
        break;
    case openMultiple:
        files = QFileDialog::getOpenFileNames(pParent, title, file, mask);
        break;
    case save:
        file = QFileDialog::getSaveFileName(pParent, title, file, mask);
        files << file;
        break;
    }
    if (!files.isEmpty() && !files.first().isEmpty())
        sDirectoryMap[key] = QFileInfo(files.first()).absolutePath();
    return files;
}

QString FileDialog::getOpenFileName(QWidget *pParent, const QString &title, const QString &mask,
                                    const QString &category, const QString &name)
{
    auto files = getFileNames(open, pParent, title, mask, category, name);
    return files.isEmpty() ? "" : files.first();
}

QList<QString> FileDialog::getOpenFileNames(QWidget *pParent, const QString &title, const QString &mask,
                                            const QString &category, const QString &name)
{
    return getFileNames(openMultiple, pParent, title, mask, category, name);
}

QString FileDialog::getSaveFileName(QWidget *pParent, const QString &title, const QString &mask,
                                    const QString &category, const QString &name)
{
    auto files = getFileNames(save, pParent, title, mask, category, name);
    return files.isEmpty() ? "" : files.first();
}
