//////////////////////////////////////////////////////////////////////
// $Id: ConfigWindow.cpp 6707 2022-05-02 16:18:15Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's parameter configuration dialog.
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
///////////////////////////////////////////////////////////////////////
#include "ConfigWindow.h"
#include "ui_ConfigWindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QTemporaryFile>
#include <QWhatsThis>
#include <algorithm>
#include <string>
#include <vector>

#include "BCI2000FileReader.h"
#include "ExecutableHelp.h"
#include "FileUtils.h"
#include "OperatorUtils.h"
#include "ParamList.h"
#include "Preferences.h"
#include "ShowParameters.h"

ConfigWindow *gpConfig = NULL;

ConfigWindow::ConfigWindow(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::ConfigWindow), mUserSwitchedTabs(false), mpScrollArea(NULL), mScrollPos(-1)
{
    m_ui->setupUi(this);
    connect(this, SIGNAL(finished(int)), this, SLOT(OnClose()));
    connect(m_ui->bConfigureLoad, SIGNAL(clicked()), this, SLOT(OnConfigureLoadFilterClick()));
    connect(m_ui->bConfigureSave, SIGNAL(clicked()), this, SLOT(OnConfigureSaveFilterClick()));
    connect(m_ui->bHelp, SIGNAL(clicked()), this, SLOT(OnHelpClick()));
    connect(m_ui->bLoadParameters, SIGNAL(clicked()), this, SLOT(OnLoadParametersClick()));
    connect(m_ui->bSaveParameters, SIGNAL(clicked()), this, SLOT(OnSaveParametersClick()));
    connect(m_ui->bClose, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_ui->cfgTabControl, SIGNAL(currentChanged(int)), this, SLOT(OnCfgTabControlChange()));
    OperatorUtils::RestoreWidget(this);
    if (ExecutableHelp().ParamHelp().Empty())
        m_ui->bHelp->setVisible(false);
    mOriginalTitle = this->windowTitle();
}

ConfigWindow::~ConfigWindow()
{
    OperatorUtils::SaveWidget(this);
    DisposeWidgets();
    delete m_ui;
}

void ConfigWindow::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

static bool CompareTabNames(const std::string &s1, const std::string &s2)
{ // This comparison function makes sure that standard tabs have standard positions.
    static const char *leftTabs_[] = {
        "Visualize",
        "System",
        "Source",
        "Storage",
    };
    static std::vector<std::string> leftTabs(leftTabs_, leftTabs_ + sizeof(leftTabs_) / sizeof(*leftTabs_));
    std::vector<std::string>::const_iterator i1 = leftTabs.begin();
    for (; i1 != leftTabs.end(); ++i1)
        if (s1.find(*i1) == 0)
            break;
    std::vector<std::string>::const_iterator i2 = leftTabs.begin();
    for (; i2 != leftTabs.end(); ++i2)
        if (s2.find(*i2) == 0)
            break;
    if (i1 == i2)
        return s1 < s2;
    return i1 < i2;
}

int ConfigWindow::Initialize(ParamList *inParameters, Preferences *inPreferences)
{
    mpPreferences = inPreferences;
    mpParameters = inParameters;

    mpParameters->Sort();
    DeleteAllTabs();

    DisplayBase::InitMeasures(this->logicalDpiX());

    std::vector<std::string> tabNames;
    for (int i = 0; i < mpParameters->Size(); ++i)
        if (OperatorUtils::GetUserLevel(mpParameters->ByIndex(i).Path().c_str()) <= mpPreferences->mUserLevel)
        {
            std::string tabName = mpParameters->ByIndex(i).Section();
            if (std::find(tabNames.begin(), tabNames.end(), tabName) == tabNames.end())
                tabNames.push_back(tabName);
        }
    stable_sort(tabNames.begin(), tabNames.end(), CompareTabNames);

    mUserSwitchedTabs = false;
    for (std::vector<std::string>::const_iterator i = tabNames.begin(); i != tabNames.end(); ++i)
    {
        QScrollArea *pScrollArea = new QScrollArea;
        pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_ui->cfgTabControl->addTab(pScrollArea, i->c_str());
        pScrollArea->setWidget(new QLabel);
    }
    mUserSwitchedTabs = true;

    if (m_ui->cfgTabControl->count() == 0)
    {
        if (mpParameters->Empty())
        {
            m_ui->cfgTabControl->insertTab(0, new QWidget, tr("No parameters available"));
        }
        else
        {
            m_ui->cfgTabControl->insertTab(0, new QWidget, tr("No parameter visible"));
            QMessageBox::information(this, tr("Message"), tr("No parameter visible! Increase user level"),
                                     QMessageBox::Yes | QMessageBox::Default);
        }
    }
    if (std::find(tabNames.begin(), tabNames.end(), mCurTab) == tabNames.end())
        mCurTab = "Storage";
    if (std::find(tabNames.begin(), tabNames.end(), mCurTab) == tabNames.end())
        mCurTab = "Filtering";
    if (std::find(tabNames.begin(), tabNames.end(), mCurTab) == tabNames.end())
        if (!tabNames.empty())
            mCurTab = tabNames.front();
    mUserSwitchedTabs = false;
    int curTabIdx = std::find(tabNames.begin(), tabNames.end(), mCurTab) - tabNames.begin();
    if (curTabIdx != tabNames.size())
        m_ui->cfgTabControl->setCurrentIndex(curTabIdx);
    mUserSwitchedTabs = true;
    mCurTab = m_ui->cfgTabControl->tabText(curTabIdx).toLocal8Bit().constData();
    RenderParameters(mCurTab);
    if (mScrollPos >= 0 && mpScrollArea)
        mpScrollArea->verticalScrollBar()->setValue(mScrollPos);
    return 0;
}

void ConfigWindow::DeleteAllTabs()
{
    mUserSwitchedTabs = false;
    // delete old Tabs, if present
    while (m_ui->cfgTabControl->count() > 0)
        m_ui->cfgTabControl->removeTab(0);
    mUserSwitchedTabs = true;
}

// render all parameters in a particular section on the screen
void ConfigWindow::RenderParameters(const std::string &inSection)
{
    if (inSection.empty())
        return;

    DisposeWidgets();
    mParamDisplays.clear();

    int currentTop = DisplayBase::Measures[DisplayBase::buttonHeight_] / 2;
    std::string lastSubsection = "";
    QFrame *pLastBevel = NULL;
    QWidget *pTabWidget = m_ui->cfgTabControl->currentWidget();
    mpScrollArea = dynamic_cast<QScrollArea *>(pTabWidget);
    if (mpScrollArea == NULL)
        return;
    QWidget *pScrollingPane = new QWidget(mpScrollArea);
    delete mpScrollArea->widget();
    pScrollingPane->resize(
        DisplayBase::Measures[DisplayBase::totalWidth_] + 2 * DisplayBase::Measures[DisplayBase::leftMargin_], 1);

    std::map<std::string, int> subsectionIndex;
    std::vector<std::string> subsectionTable;
    std::vector<std::vector<int>> subsectionGroups;

    for (int i = 0; i < mpParameters->Size(); ++i)
    {
        const Param &p = mpParameters->ByIndex(i);
        if (inSection == p.Section() && OperatorUtils::GetUserLevel(p.Path().c_str()) <= mpPreferences->mUserLevel)
        {
            std::string subsection;
            if (p.Sections().size() > 1)
                subsection = p.Sections()[1];
            if (subsectionIndex.find(subsection) == subsectionIndex.end())
            {
                subsectionIndex[subsection] = static_cast<int>(subsectionTable.size());
                subsectionTable.push_back(subsection);
                subsectionGroups.resize(subsectionGroups.size() + 1);
            }
            subsectionGroups[subsectionIndex[subsection]].push_back(i);
        }
    }

    for (size_t i = 0; i < subsectionTable.size(); ++i)
    {
        const std::string &subsection = subsectionTable[i];
        if (mpPreferences->mUserLevel > Preferences::Beginner)
        { // A bevel for each subsection.
            if (pLastBevel != NULL)
            {
                pLastBevel->resize(pLastBevel->size().width(), DisplayBase::Measures[DisplayBase::buttonHeight_] / 2 +
                                                                   currentTop - pLastBevel->y());
                pLastBevel->show();
                pLastBevel = NULL;
                currentTop += DisplayBase::Measures[DisplayBase::buttonHeight_];
            }
            if (subsection != "")
            {
                QFrame *pBevel = new QFrame(pScrollingPane);
                mWidgets.insert(pBevel);
                pBevel->setFrameShadow(QFrame::Sunken);
                pBevel->setFrameShape(QFrame::StyledPanel);
                pBevel->move(DisplayBase::Measures[DisplayBase::leftMargin_] / 2, currentTop);
                pBevel->resize(pScrollingPane->width() - DisplayBase::Measures[DisplayBase::leftMargin_],
                               pBevel->height());
                pLastBevel = pBevel;

                QLabel *pLabel = new QLabel(pScrollingPane);
                mWidgets.insert(pLabel);
                pLabel->setText((subsection + " ").c_str());
                QFont font = pLabel->font();
                font.setItalic(true);
                pLabel->setFont(font);
                pLabel->move(DisplayBase::Measures[DisplayBase::leftMargin_] / 2 + 4, currentTop + 2);
                pLabel->show();

                currentTop += pLabel->height();
            }
        }
        int groupIndex = subsectionIndex[subsection];
        for (size_t j = 0; j < subsectionGroups[groupIndex].size(); ++j)
        {
            int paramIndex = subsectionGroups[groupIndex][j];
            const Param &p = mpParameters->ByIndex(paramIndex);
            ParamDisplay paramDisplay(p, pScrollingPane);
            paramDisplay.SetLeft(DisplayBase::Measures[DisplayBase::leftMargin_]);
            paramDisplay.SetTop(currentTop);
            paramDisplay.ReadValuesFrom(p);
            mParamDisplays[p.Path()] = paramDisplay;
            currentTop = paramDisplay.GetBottom();
        }
    }
    if (pLastBevel != NULL)
    {
        pLastBevel->resize(pLastBevel->width(),
                           DisplayBase::Measures[DisplayBase::buttonHeight_] / 2 + currentTop - pLastBevel->y());
        pLastBevel->show();
    }

    for (DisplayContainer::iterator i = mParamDisplays.begin(); i != mParamDisplays.end(); ++i)
        i->second.Show();

    pScrollingPane->resize(pScrollingPane->width(), currentTop + DisplayBase::Measures[DisplayBase::buttonHeight_]);
    mpScrollArea->setWidget(pScrollingPane);
}

// update one particular parameter on the screen
// useful, for example, if parameters change while stuff on screen
void ConfigWindow::RenderParameter(Param *inParam)
{
    if (mParamDisplays.find(inParam->Path()) != mParamDisplays.end())
        mParamDisplays[inParam->Path()].ReadValuesFrom(*inParam);
}

// go through the parameters on the screen and update the parameters using the data on the screen
void ConfigWindow::UpdateParameters()
{
    for (DisplayContainer::const_iterator i = mParamDisplays.begin(); i != mParamDisplays.end(); ++i)
        if (i->second.Modified())
            i->second.WriteValuesTo(mpParameters->ByPath(i->first));
}

void ConfigWindow::OnCfgTabControlChange()
{
    if (!mUserSwitchedTabs)
        return;

    UpdateParameters();
    int curTabIdx = m_ui->cfgTabControl->currentIndex();
    mCurTab = m_ui->cfgTabControl->tabText(curTabIdx).toLocal8Bit().constData();
    RenderParameters(mCurTab);
}

void ConfigWindow::OnClose()
{
    if (mpScrollArea)
        mScrollPos = mpScrollArea->verticalScrollBar()->value();
    UpdateParameters();
    mParamDisplays.clear();
    DisposeWidgets();
    DeleteAllTabs();
}

void ConfigWindow::OnSaveParametersClick()
{
    // Avoid Qt warning about empty path when calling canonicalPath().
    QString path = mpPreferences->mCurrentDir.path();
    if (!path.isEmpty())
        path = mpPreferences->mCurrentDir.canonicalPath();
    QString fileName =
        QFileDialog::getSaveFileName(this, tr("Save Parameter File"), path,
                                     tr("BCI2000 parameter files (*.prm)"));
    if (!fileName.isEmpty())
    {
        mpPreferences->mCurrentDir.setPath(QFileInfo(fileName).canonicalPath());
        UpdateParameters();

        ParamList paramsToSave;
        for (int i = 0; i < mpParameters->Size(); ++i)
            if (0 == OperatorUtils::GetFilterStatus(mpParameters->ByIndex(i).Name().c_str(), OperatorUtils::saveFilter))
                paramsToSave.Add(mpParameters->ByIndex(i));

        bool result = paramsToSave.Save(fileName.toLocal8Bit().data());
        if (result)
            this->setWindowTitle(mOriginalTitle + tr(" - wrote %1").arg(fileName));
        else
            QMessageBox::critical(this, tr("Error"), tr("Error writing parameter file"));
    }
}

bool ConfigWindow::LoadParameters(const QString &inName)
{
    ParamList paramsFromFile = *mpParameters;
    paramsFromFile.Unchanged();
    bool result = false;
    QString fileExtension = QFileInfo(inName).suffix();
    if (fileExtension == "dat")
    {
        BCI2000FileReader file(inName.toLocal8Bit().constData());
        if (file.IsOpen())
        {
            FileUtils::TemporaryFile temp;
            file.Parameters()->Save(temp.Name());
            result = paramsFromFile.Load(temp.Name(), false);
        }
    }
    else
        result = paramsFromFile.Load(inName.toLocal8Bit().constData(), false);

    for (int i = 0; i < paramsFromFile.Size(); ++i)
    {
        const Param &p = paramsFromFile.ByIndex(i);
        Param &q = mpParameters->ByPath(p.Path());
        if (p.Changed() && 0 == OperatorUtils::GetFilterStatus(q.Path().c_str(), OperatorUtils::loadFilter))
            q.AssignValues(p, true);
    }
    return result;
}

void ConfigWindow::DisposeWidgets()
{
    for (WidgetContainer::iterator i = mWidgets.begin(); i != mWidgets.end(); ++i)
        delete *i;
    mWidgets.clear();
}

void ConfigWindow::OnLoadParametersClick()
{
    // Avoid Qt warning about empty path when calling canonicalPath().
    QString path = mpPreferences->mCurrentDir.path();
    if (!path.isEmpty())
        path = mpPreferences->mCurrentDir.canonicalPath();
    QString fileName =
        QFileDialog::getOpenFileName(NULL, tr("Open Parameter File"), path,
                                     tr("BCI2000 parameter file (*.prm);;Any file (*.*)"));
    if (!fileName.isEmpty())
    {
        mpPreferences->mCurrentDir.setPath(QFileInfo(fileName).canonicalPath());
        UpdateParameters();
        bool result = LoadParameters(fileName);
        if (!result)
        {
            QMessageBox::critical(this, tr("Error"), tr("Error reading parameter file"));
        }
        else
        {
            this->setWindowTitle(mOriginalTitle + tr(" - read %1").arg(fileName));
            RenderParameters(mCurTab);
        }
    }
}

void ConfigWindow::OnConfigureSaveFilterClick()
{
    ShowParameters dialog(this, *mpParameters, OperatorUtils::saveFilter);
    dialog.exec();
}

void ConfigWindow::OnConfigureLoadFilterClick()
{
    ShowParameters dialog(this, *mpParameters, OperatorUtils::loadFilter);
    dialog.exec();
}

void ConfigWindow::OnHelpClick()
{
    QWhatsThis::enterWhatsThisMode();
}
