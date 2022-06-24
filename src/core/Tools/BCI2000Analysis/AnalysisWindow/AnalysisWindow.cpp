////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: Main window class for AnalysisWindow.
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
#include "AnalysisWindow.h"

#include "AnalysisData.h"
#include "AnalysisParamWidget.h"
#include "BCI2000FileReader.h"
#include "ConfigWindow.h"
#include "CppTranslator.h"
#include "CreateFilterGraph.h"
#include "DataWindow.h"
#include "EditMenu.h"
#include "Environment.h"
#include "FileDialog.h"
#include "FileUtils.h"
#include "FilesWidget.h"
#include "FiltersWidget.h"
#include "GenericVisualization.h"
#include "HelpMenu.h"
#include "LinearClassifierWrapper.h"
#include "LogWindow.h"
#include "NullMessageChannel.h"
#include "Preferences.h"
#include "ProfilerWindow.h"
#include "Settings.h"
#include "WindowMenu.h"

#include <QApplication>
#include <QDockWidget>
#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressDialog>
#include <QToolBar>
#include <QActionGroup>

static Preferences sPreferences;
Preferences *gpPreferences = &sPreferences;

struct AnalysisWindow::Private : QObject
{
    NullMessageChannel mNullChannel;
    FilesWidget *mpTrainingFilesWidget = nullptr, *mpTestFilesWidget = nullptr;
    FiltersWidget *mpFiltersWidget = nullptr;
    AnalysisParamWidget *mpAnalysisParamWidget = nullptr;
    QMainWindow *mpParent;
    QAction *mpFilterGraphAutoItem, *mpFilterGraphSMRItem, *mpFilterGraphERPItem, *mpFilterGraphUserItem,
        *mpAnalysisParamsAutoItem, *mpAnalysisParamsUserItem, *mpProfiling;
    QAction *mpRunAnalysis1, *mpRunAnalysis2;
    FilesWidget::FileClassification mCurrentClassification;
    struct
    {
        ParamList user, graph, verbosity;
        bool graphValid;
    } mParameters;

    Private(QMainWindow *);
    ~Private();

    void onOpenAnalysisData();
    void onSaveAnalysisData();

    void onOpenFilterGraph();
    void onSaveFilterGraph();
    void onSavePipeDefinition();

    void onLoadFilterParameters();
    void onSaveFilterParameters();
    void onClearFilterParameters();

    void onTrainingFilesWidgetDataChanged();

    void onFilterGraphAutoItemToggled(bool);
    void onFilterGraphSMRItemToggled(bool);
    void onFilterGraphERPItemToggled(bool);
    void onFilterGraphUserItemToggled(bool);
    void onFilterGraphComplexExampleItemTriggered();
    void onFilterGraphEdited();
    void onFilterGraphReplaced();

    void onAnalysisParamsAutoItemToggled(bool);
    void onAnalysisParamsUserItemToggled(bool);
    void onAnalysisParamsEdited();

    void onEditFilterParametersTriggered();
    void updateGraphParameters();
    GenericFilter *instantiateGraph(ParamList &, StateList &);

    void analyze();
};

AnalysisWindow::Private::Private(QMainWindow *p)
    : QObject(p), mpParent(p), mCurrentClassification(FilesWidget::Undefined)
{
    GenericVisualization::SetOutputChannel(&mNullChannel);
}

AnalysisWindow::Private::~Private()
{
}

void AnalysisWindow::Private::onOpenAnalysisData()
{
    QString file = FileDialog::getOpenFileName(mpParent, "Open Analysis Data",
                                               "BCI2000 Analysis Data File (*.bcianalysis)", "output");
    if (!file.isEmpty())
    {
        std::ifstream stream(file.toLocal8Bit().constData(), std::ios::binary);
        AnalysisData data;
        try
        {
            data = AnalysisData::unserialize(stream);
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(
                mpParent, "Error reading file",
                QString("An error occurred when trying to read file\n\"%1\":\n%2").arg(file).arg(e.what()));
            return;
        }
        mpTrainingFilesWidget->setFiles(data.trainingFiles);
        mpTestFilesWidget->setFiles(data.testFiles);
        mpFilterGraphUserItem->setChecked(true);
        mpFiltersWidget->setGraph(data.filterGraph);
        mpAnalysisParamsUserItem->setChecked(true);
        mpAnalysisParamWidget->setParams(data.analysisParams);
        mParameters.user = data.filterParams;
        mParameters.graphValid = false;
        if (!data.trainingData1.empty() || !data.trainingData2.empty())
        {
            DataWindow *pWindow = new DataWindow(data);
            pWindow->show();
        }
    }
}

void AnalysisWindow::Private::onSaveAnalysisData()
{
    QString file = FileDialog::getSaveFileName(mpParent, "Save Analysis Data",
                                               "BCI2000 Analysis Data File (*.bcianalysis)", "output");
    if (!file.isEmpty())
    {
        std::ofstream stream(file.toLocal8Bit().constData(), std::ios::binary);
        if (!stream)
        {
            QMessageBox::critical(mpParent, "Error writing file",
                                  QString("Error opening file\n%1\nfor writing").arg(file));
            return;
        }
        AnalysisData data;
        data.trainingFiles = mpTrainingFilesWidget->files();
        data.testFiles = mpTestFilesWidget->files();
        data.filterGraph = mpFiltersWidget->graph();
        data.analysisParams = mpAnalysisParamWidget->params();
        updateGraphParameters();
        data.filterParams = mParameters.graph;
        try
        {
            data.serialize(stream);
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(
                mpParent, "Error writing file",
                QString("An error occurred when trying to write file\n\"%1\":\n%2").arg(file).arg(e.what()));
        }
    }
}

void AnalysisWindow::Private::onOpenFilterGraph()
{
    QString file =
        FileDialog::getOpenFileName(mpParent, "Open Graph File", "BCI2000 Graph File (*.bcigraph)", "output");
    if (!file.isEmpty())
    {
        std::ifstream stream(file.toLocal8Bit().constData(), std::ios::binary);
        FilterGraph::Graph graph;
        try
        {
            graph = FilterGraph::Graph::unserialize(stream);
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(
                mpParent, "Error reading file",
                QString("An error occurred when trying to read file\n\"%1\":\n%2").arg(file).arg(e.what()));
            return;
        }
        mpFilterGraphUserItem->setChecked(true);
        mpFiltersWidget->setGraph(graph);
    }
}

void AnalysisWindow::Private::onSaveFilterGraph()
{
    QString file = FileDialog::getSaveFileName(mpParent, "Save Graph", "BCI2000 Graph File (*.bcigraph)", "output");
    if (!file.isEmpty())
    {
        std::ofstream stream(file.toLocal8Bit().constData(), std::ios::binary);
        if (!stream)
        {
            QMessageBox::critical(mpParent, "Error writing file",
                                  QString("Error opening file\n%1\nfor writing").arg(file));
            return;
        }
        try
        {
            mpFiltersWidget->graph().serialize(stream);
        }
        catch (std::exception &e)
        {
            QMessageBox::critical(
                mpParent, "Error writing file",
                QString("An error occurred when writing to file\n\"%1\":\n%2").arg(file).arg(e.what()));
        }
    }
}

void AnalysisWindow::Private::onSavePipeDefinition()
{
    QString file = FileDialog::getSaveFileName(mpParent, "Save PipeDefinition File", "C++ source file (*.cpp)", "",
                                               "PipeDefinition.cpp");
    if (!file.isEmpty())
    {
        std::ofstream ofs(file.toLocal8Bit().constData());
        FilterGraph::CppTranslator(ofs).translate(mpFiltersWidget->graph());
    }
}

void AnalysisWindow::Private::onLoadFilterParameters()
{
    QString file = FileDialog::getOpenFileName(mpParent, "Open Parameter File", "BCI2000 Parameter File (*.prm)");
    if (!file.isEmpty())
    {
        if (mParameters.user.Load(file.toLocal8Bit().constData()))
            mParameters.graphValid = false;
        else
            QMessageBox::critical(mpParent, "Error reading parameter file",
                                  QString("An error occurred when trying to read file\n\"%1\"").arg(file));
    }
}

void AnalysisWindow::Private::onSaveFilterParameters()
{
    QString file = FileDialog::getSaveFileName(mpParent, "Save Filter Parameters", "BCI2000 Parameter File (*.prm)");
    if (!file.isEmpty())
    {
        updateGraphParameters();
        if (!mParameters.graph.Save(file.toLocal8Bit().constData()))
            QMessageBox::critical(mpParent, "Error writing parameter file",
                                  QString("An error occurred when writing to file\n\"%1\"").arg(file));
    }
}

void AnalysisWindow::Private::onClearFilterParameters()
{
    mParameters.user.Clear();
    mParameters.graphValid = false;
}

void AnalysisWindow::Private::onTrainingFilesWidgetDataChanged()
{
    mParameters.graphValid = false;
    bool haveFiles = !mpTrainingFilesWidget->data().empty();
    mpRunAnalysis1->setEnabled(haveFiles);
    mpRunAnalysis2->setEnabled(haveFiles);
    mpTestFilesWidget->setReferenceFile(haveFiles ? mpTrainingFilesWidget->data().front() : nullptr);

    if (mpFilterGraphAutoItem->isChecked())
    {
        auto files = mpTrainingFilesWidget->data();
        FilesWidget::FileClassification fileClass = FilesWidget::Undefined;
        if (!files.empty())
            fileClass = FilesWidget::classifyFile(files.front());
        for (auto pFile : files)
            if (FilesWidget::classifyFile(pFile) != fileClass)
                fileClass = FilesWidget::Unknown;
        if (fileClass != mCurrentClassification)
        {
            mCurrentClassification = fileClass;
            switch (fileClass)
            {
            case FilesWidget::Undefined:
                mpFiltersWidget->clearGraph();
                break;
            case FilesWidget::Unknown:
                mpFiltersWidget->setGraph(FilterGraph::createMinimalGraph());
                break;
            case FilesWidget::SMR:
                mpFiltersWidget->setGraph(FilterGraph::createSMRAnalysisGraph());
                break;
            case FilesWidget::ERP:
                mpFiltersWidget->setGraph(FilterGraph::createERPAnalysisGraph());
                break;
            }
        }
    }
    if (mpAnalysisParamsAutoItem->isChecked())
    {
        auto files = mpTrainingFilesWidget->data();
        if (files.empty())
            mpAnalysisParamWidget->setParams(AnalysisParamWidget::AnalysisParams());
        else
        {
            auto params = AnalysisParamWidget::AnalysisParams::createFromFile(files.front());
            mpAnalysisParamWidget->setParams(params);
        }
    }
}

void AnalysisWindow::Private::onFilterGraphAutoItemToggled(bool checked)
{
    if (checked && mpTrainingFilesWidget)
        onTrainingFilesWidgetDataChanged();
}

void AnalysisWindow::Private::onFilterGraphSMRItemToggled(bool checked)
{
    if (checked && mCurrentClassification != FilesWidget::SMR)
    {
        mCurrentClassification = FilesWidget::SMR;
        mpFiltersWidget->setGraph(FilterGraph::createSMRAnalysisGraph());
    }
}

void AnalysisWindow::Private::onFilterGraphERPItemToggled(bool checked)
{
    if (checked && mCurrentClassification != FilesWidget::ERP)
    {
        mCurrentClassification = FilesWidget::ERP;
        mpFiltersWidget->setGraph(FilterGraph::createERPAnalysisGraph());
    }
}

void AnalysisWindow::Private::onFilterGraphUserItemToggled(bool checked)
{
    if (checked && mCurrentClassification != FilesWidget::Unknown)
    {
        mCurrentClassification = FilesWidget::Unknown;
        mpFiltersWidget->setGraph(FilterGraph::createMinimalGraph());
    }
}

void AnalysisWindow::Private::onFilterGraphComplexExampleItemTriggered()
{
    mpFilterGraphUserItem->setChecked(true);
    mpFiltersWidget->setGraph(FilterGraph::createComplexExampleGraph());
}

void AnalysisWindow::Private::onAnalysisParamsAutoItemToggled(bool checked)
{
    if (checked && mpTrainingFilesWidget)
        onTrainingFilesWidgetDataChanged();
}

void AnalysisWindow::Private::onAnalysisParamsUserItemToggled(bool checked)
{
    if (checked && mpTrainingFilesWidget)
        onTrainingFilesWidgetDataChanged();
}

void AnalysisWindow::Private::onFilterGraphEdited()
{
    mCurrentClassification = FilesWidget::Unknown;
    mpFilterGraphUserItem->setChecked(true);
    mParameters.graphValid = false;
}

void AnalysisWindow::Private::onFilterGraphReplaced()
{
    mParameters.graphValid = false;
}

void AnalysisWindow::Private::onAnalysisParamsEdited()
{
    mpAnalysisParamsUserItem->setChecked(true);
}

void AnalysisWindow::Private::onEditFilterParametersTriggered()
{
    updateGraphParameters();
    ParamList parameters;
    float hint = 0;
    for (const auto &p : mParameters.graph)
        if (p.Section().find("Visualize") != 0)
            parameters.Add(p, hint++);
    parameters.Unchanged();

    ConfigWindow configWindow(mpParent);
    configWindow.Initialize(&parameters, gpPreferences);
    configWindow.exec();

    for (const auto &p : parameters)
    {
        if (p.Changed())
        {
            mParameters.user.Add(p);
            mParameters.graph.Add(p);
        }
    }
    mParameters.user.Unchanged();
    mParameters.graph.Unchanged();
}

void AnalysisWindow::Private::updateGraphParameters()
{
    if (mParameters.graphValid)
        return;

    StateList states;
    auto pFilter = instantiateGraph(mParameters.graph, states);
    delete pFilter;
    mParameters.graphValid = true;
    mParameters.graph.Unchanged();
}

GenericFilter *AnalysisWindow::Private::instantiateGraph(ParamList &parameters, StateList &states)
{
    LogWindow::instance()->clear();
    bcierr__.Clear();
    GenericFilter::RootChain().ClearRegistrars();
    GenericFilter::Registrar::Clear();
    parameters.Add(Param("Source:Source int /SampleBlockSize= 1"), INT_MIN);
    auto ctx = Environment::Context::GlobalInstance();
    ctx->EnterPhase(Environment::construction, &parameters, &states);
    GenericFilter *pFilter = mpFiltersWidget->graph().instantiateAsFilter();
    ctx->EnterPhase(Environment::nonaccess);
    ctx->EnterPhase(Environment::publication, &parameters, &states);
    pFilter->CallPublish();
    ctx->EnterPhase(Environment::nonaccess);
    parameters.Sort();
    parameters.Prune();
    auto files = mpTrainingFilesWidget->data();
    if (!files.empty())
        for (const auto &p : *files.front()->Parameters())
            if (parameters.Exists(p.Path()))
                parameters.ByPath(p.Path()).AssignValues(p);
    for (const auto &p : mParameters.user)
        if (parameters.Exists(p.Path()))
            parameters.ByPath(p.Path()).AssignValues(p);
    return pFilter;
}

void AnalysisWindow::Private::analyze()
{
    auto files1 = mpTrainingFilesWidget->data(), files2 = mpTestFilesWidget->data();
    if (files1.empty())
    {
        QMessageBox::critical(mpParent, "Error",
                              "You must specify at least one training file in order to analyze data");
        return;
    }
    size_t numTrainingFiles = files1.size();
    auto files = files1;
    files.insert(files.end(), files2.begin(), files2.end());

    auto pFile = files[0];

    AnalysisData analysisData;
    analysisData.trainingFiles = mpTrainingFilesWidget->files();
    analysisData.testFiles = mpTestFilesWidget->files();
    analysisData.analysisParams = mpAnalysisParamWidget->params();
    analysisData.filterGraph = mpFiltersWidget->graph();

    auto ctx = Environment::Context::GlobalInstance();
    GenericFilter *pFilter = nullptr;
    try
    {
        ParamList parameters;
        parameters.Add("Source float /SamplingRate= 1Hz");
        for (const auto& p : mParameters.verbosity)
            parameters.Add(p);
        StateList states = *pFile->States();
        pFilter = instantiateGraph(parameters, states);

        bool doProfile = mpProfiling->isChecked();
        pFilter->EnableProfiling(doProfile);
        struct F 
        {
          const bool doProfile;
          bool operator()(Directory::Node* pNode) {
            auto pFilterNode = dynamic_cast<GenericFilter*>(pNode);
            if (pFilterNode)
              pFilterNode->EnableProfiling(doProfile);
            return true;
          }
        } f = { doProfile };
        Directory::Traverse(pFilter, f);

        auto pStatistics = LinearClassifierWrapper::instance();
        if (pStatistics)
        {
            auto params = mpAnalysisParamWidget->params();
            pStatistics->setCondition1(params.condition1);
            pStatistics->setCondition2(params.condition2);
            pStatistics->setObserve(!doProfile);
        }

        bcierr__.MessageContext("Basic configuration");
        ParamRef SampleBlockSize(&parameters.ByPath("SampleBlockSize"));
        if (SampleBlockSize < 1)
            bcierr__ << "Parameter SampleBlockSize must be greater 0";
        else if (SampleBlockSize > pFile->NumSamples())
            bcierr__ << "Parameter SampleBlockSize exceeds number of samples in input file";
        bcierr__.MessageContext("");
        if (!bcierr__.Empty())
            throw std::runtime_error("Configuration errors occurred.\nSee the filter log window for details.");

        MeasurementUnits::Initialize(parameters);
        SignalProperties InputProperties = pFile->SignalProperties();
        InputProperties.SetElements(SampleBlockSize);
        InputProperties.ValueUnit().SetRawMin(-100).SetRawMax(100).SetGainWithSymbol("1muV");
        states.AssignPositions();
        StateVector statevector(states, InputProperties.Elements());
        ctx->EnterPhase(Environment::preflight, &parameters, &states, &statevector);
        SignalProperties OutputProperties;
        pFilter->CallPreflight(InputProperties, OutputProperties);
        ctx->EnterPhase(Environment::nonaccess);

        if (!bcierr__.Empty())
            throw std::runtime_error(
                "Errors occurred when configuring filters.\nSee the filter log window for details.");

        ctx->EnterPhase(Environment::initialization, &parameters, &states, &statevector);
        pFilter->CallInitialize(InputProperties, OutputProperties);
        ctx->EnterPhase(Environment::nonaccess);

        GenericSignal Input(InputProperties), Output(OutputProperties);
        int sampleBlockSize = InputProperties.Elements();
        int channels = InputProperties.Channels();

        QProgressDialog dialog("Running analysis...", "Cancel", 0, 1000 * files.size(), mpParent);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
        dialog.setAutoClose(false);
        dialog.setValue(0);
        dialog.show();
        int fileIndex = 0;
        for (auto pData : files)
        {
            if (dialog.wasCanceled())
                break;

            ctx->EnterPhase(Environment::startRun, &parameters, &states, &statevector);
            pFilter->CallStartRun();
            ctx->EnterPhase(Environment::nonaccess);

            int blocks = pData->NumSamples() / sampleBlockSize;
            for (uint64_t sample = 0; sample < pData->NumSamples(); sample += sampleBlockSize)
            {
                if (dialog.wasCanceled())
                    break;
                for (int s = 0; s < sampleBlockSize; ++s)
                {
                    for (int ch = 0; ch < channels; ++ch)
                        Input(ch, s) = pData->CalibratedValue(ch, sample + s);
                    pData->ReadStateVector(sample + s);
                    for (auto &state : *pData->States())
                    {
                        auto value = pData->StateVector()->StateValue(state.Name(), 0);
                        statevector.SetStateValue(state.Name(), s, value);
                    }
                }
                ctx->EnterPhase(Environment::processing, &parameters, &states, &statevector);
                pFilter->CallProcess(Input, Output);
                ctx->EnterPhase(Environment::nonaccess);
                if (!bcierr__.Empty())
                    throw std::runtime_error(
                        "Errors occurred during processing.\nSee the filter log window for details.");
                dialog.setValue(1000 * fileIndex + (sample * 1000) / pData->NumSamples());
            }
            ctx->EnterPhase(Environment::stopRun, &parameters, &states, &statevector);
            pFilter->CallStopRun();
            ctx->EnterPhase(Environment::nonaccess);
            if (!bcierr__.Empty())
                throw std::runtime_error("Errors occurred after processing.\nSee the filter log window for details.");

            ++fileIndex;
            if (fileIndex == numTrainingFiles && pStatistics)
            {
                analysisData.trainingData1 = pStatistics->data1();
                analysisData.trainingData2 = pStatistics->data2();
                pStatistics->resetStatistics();
            }
        }
        analysisData.filterParams = mParameters.graph;

        QWidget *pWindow = nullptr;
        if (doProfile)
        {
            auto graph = mpFiltersWidget->graph();
            graph.gatherPerformanceData();
            graph.clearFilterInstances();
            pWindow = new ProfilerWindow(graph);
        }
        else if (!dialog.wasCanceled() && pStatistics)
        {
            analysisData.testData1 = pStatistics->data1();
            analysisData.testData2 = pStatistics->data2();
            pWindow = new DataWindow(analysisData);
        }
        if (pWindow)
        {
            pWindow->setAttribute(Qt::WA_DeleteOnClose);
            pWindow->show();
        }
        dialog.close();
    }
    catch (const std::runtime_error &e)
    {
        if (e.what() && *e.what())
            QMessageBox::critical(mpParent, "Error", e.what());
    }

    ctx->EnterPhase(Environment::destruction);
    pFilter->CallHalt();
    delete pFilter;
    ctx->EnterPhase(Environment::nonaccess);
}

AnalysisWindow::AnalysisWindow() : QMainWindow(nullptr), p(new Private(this))
{
    // set default directory to BCI2000 root
    FileDialog::setDefaultDirectory(QApplication::applicationDirPath() + "/../..");

    QMainWindow::move(10, 10);
    QMenu *pMenu = QMainWindow::menuBar()->addMenu("&File");
    QAction *pAddTrainingFileAction = pMenu->addAction("Add &Analysis Files...");
    pAddTrainingFileAction->setShortcut(Qt::CTRL + Qt::Key_D);
    QAction *pAddTestFileAction = pMenu->addAction("Add Classifier T&est Files...");
    pAddTestFileAction->setShortcut(Qt::CTRL + Qt::Key_T);
    pMenu->addSeparator();
    QAction *pAction = pMenu->addAction("Open Filter &Graph...");
    connect(pAction, &QAction::triggered, p, &Private::onOpenFilterGraph);
    pAction = pMenu->addAction("Sa&ve Filter Graph...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveFilterGraph);
    pMenu->addSeparator();
    pAction = pMenu->addAction("Save &PipeDefinition.cpp...");
    connect(pAction, &QAction::triggered, p, &Private::onSavePipeDefinition);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Load Filter Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onLoadFilterParameters);
    pAction->setShortcut(Qt::CTRL + Qt::Key_L);
    pAction = pMenu->addAction("Save &Filter Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveFilterParameters);
    pAction->setShortcut(Qt::CTRL + Qt::Key_F);
    pAction = pMenu->addAction("&Clear Filter Parameters");
    connect(pAction, &QAction::triggered, p, &Private::onClearFilterParameters);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Open Analysis Data...");
    pAction->setShortcut(Qt::CTRL + Qt::Key_O);
    connect(pAction, &QAction::triggered, p, &Private::onOpenAnalysisData);
    pAction = pMenu->addAction("&Save Analysis Data...");
    pAction->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(pAction, &QAction::triggered, p, &Private::onSaveAnalysisData);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Quit");
    pAction->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    pMenu = EditMenu::addToMenuBar(menuBar());
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Filter Parameters...");
    pAction->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(pAction, &QAction::triggered, p, &Private::onEditFilterParametersTriggered);

    pMenu = QMainWindow::menuBar()->addMenu("&Analysis");
    QMenu *pMenu2 = pMenu->addMenu("&Filter Graph");
    p->mpFilterGraphAutoItem = pMenu2->addAction("&Auto");
    connect(p->mpFilterGraphAutoItem, &QAction::toggled, p, &Private::onFilterGraphAutoItemToggled);
    p->mpFilterGraphSMRItem = pMenu2->addAction("&SMR/ERD");
    connect(p->mpFilterGraphSMRItem, &QAction::toggled, p, &Private::onFilterGraphSMRItemToggled);
    p->mpFilterGraphERPItem = pMenu2->addAction("&P300/ERP");
    connect(p->mpFilterGraphERPItem, &QAction::toggled, p, &Private::onFilterGraphERPItemToggled);
    p->mpFilterGraphUserItem = pMenu2->addAction("&User Defined");
    connect(p->mpFilterGraphUserItem, &QAction::toggled, p, &Private::onFilterGraphUserItemToggled);
    pMenu2->addSeparator();
    pMenu2->addAction("Complex E&xample", p, &Private::onFilterGraphComplexExampleItemTriggered);
    auto pGroup = new QActionGroup(this);
    pGroup->addAction(p->mpFilterGraphAutoItem);
    p->mpFilterGraphAutoItem->setCheckable(true);
    p->mpFilterGraphAutoItem->setChecked(true);
    pGroup->addAction(p->mpFilterGraphSMRItem);
    p->mpFilterGraphSMRItem->setCheckable(true);
    pGroup->addAction(p->mpFilterGraphERPItem);
    p->mpFilterGraphERPItem->setCheckable(true);
    pGroup->addAction(p->mpFilterGraphUserItem);
    p->mpFilterGraphUserItem->setCheckable(true);

    pMenu2 = pMenu->addMenu("Analysis &Conditions");
    p->mpAnalysisParamsAutoItem = pMenu2->addAction("&Auto");
    connect(p->mpAnalysisParamsAutoItem, &QAction::toggled, p, &Private::onAnalysisParamsAutoItemToggled);
    p->mpAnalysisParamsUserItem = pMenu2->addAction("&User Defined");
    connect(p->mpAnalysisParamsUserItem, &QAction::toggled, p, &Private::onAnalysisParamsUserItemToggled);
    pGroup = new QActionGroup(this);
    pGroup->addAction(p->mpAnalysisParamsAutoItem);
    p->mpAnalysisParamsAutoItem->setCheckable(true);
    p->mpAnalysisParamsAutoItem->setChecked(true);
    pGroup->addAction(p->mpAnalysisParamsUserItem);
    p->mpAnalysisParamsUserItem->setCheckable(true);

    pAction = pMenu->addAction("&Profiler");
    pAction->setCheckable(true);
    pAction->setChecked(false);
    p->mpProfiling = pAction;

    pMenu->addSeparator();
    p->mpRunAnalysis1 = pMenu->addAction("&Run Analysis...", p, &Private::analyze, QKeySequence(Qt::CTRL + Qt::Key_R));
    p->mpRunAnalysis1->setEnabled(false);

    WindowMenu::addWindow(this, QApplication::applicationName());
    HelpMenu::addToMenuBar(menuBar());

    auto pToolBar = QMainWindow::addToolBar("Actions");
    QAction *pAddTrainingFileAction2 = pToolBar->addAction("Add analysis files...");
    QAction *pAddTestFileAction2 = pToolBar->addAction("Add test files...");
    pToolBar->addAction(p->mpProfiling);
    p->mpRunAnalysis2 = pToolBar->addAction("Run analysis", p, &Private::analyze);
    p->mpRunAnalysis2->setEnabled(false);

    QDockWidget *pDockWidget = new QDockWidget("Analysis Files", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    p->mpTrainingFilesWidget = new FilesWidget(this);
    pDockWidget->setWidget(p->mpTrainingFilesWidget);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget);
    connect(p->mpTrainingFilesWidget, &FilesWidget::dataChanged, p, &Private::onTrainingFilesWidgetDataChanged);
    connect(pAddTrainingFileAction, &QAction::triggered, p->mpTrainingFilesWidget, &FilesWidget::addFile);
    connect(pAddTrainingFileAction2, &QAction::triggered, p->mpTrainingFilesWidget, &FilesWidget::addFile);

    pDockWidget = new QDockWidget("Classifier Test Files", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    p->mpTestFilesWidget = new FilesWidget(this);
    pDockWidget->setWidget(p->mpTestFilesWidget);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget);
    connect(pAddTestFileAction, &QAction::triggered, p->mpTestFilesWidget, &FilesWidget::addFile);
    connect(pAddTestFileAction2, &QAction::triggered, p->mpTestFilesWidget, &FilesWidget::addFile);

    pDockWidget = new QDockWidget("Analysis Parameters", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    p->mpAnalysisParamWidget = new AnalysisParamWidget(this);
    pDockWidget->setWidget(p->mpAnalysisParamWidget);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget);
    connect(p->mpAnalysisParamWidget, &AnalysisParamWidget::conditionsEdited, p, &Private::onAnalysisParamsEdited);
    connect(p->mpAnalysisParamWidget, &AnalysisParamWidget::editFilterParamsClicked, p,
            &Private::onEditFilterParametersTriggered);

    pDockWidget = new QDockWidget("Filter Log", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    pDockWidget->setWidget(new LogWindow(&p->mParameters.verbosity));
    addDockWidget(Qt::RightDockWidgetArea, pDockWidget);

    FiltersWidget *pFiltersWidget = new FiltersWidget(this);
    pFiltersWidget->setEditable(true);
    connect(pFiltersWidget, &FiltersWidget::edited, p, &Private::onFilterGraphEdited);
    connect(pFiltersWidget, &FiltersWidget::graphReplaced, p, &Private::onFilterGraphReplaced);
    p->mpFiltersWidget = pFiltersWidget;
    setCentralWidget(pFiltersWidget);

    setMinimumHeight(550);
    setMinimumWidth(940);
}

AnalysisWindow::~AnalysisWindow()
{
    WindowMenu::removeWindow(this);
}

void AnalysisWindow::setFilterParameters(const ParamList &params)
{
    p->mParameters.user = params;
}

const ParamList &AnalysisWindow::filterParameters() const
{
    p->updateGraphParameters();
    return p->mParameters.graph;
}

void AnalysisWindow::closeEvent(QCloseEvent *ev)
{
    QMainWindow::closeEvent(ev);
    QApplication::quit();
}
