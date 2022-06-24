////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: Classifier options window for BCI2000Analysis.
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
#include "ClassifierWindow.h"

#include "AnalysisData.h"
#include "AnalysisWindow.h"
#include "Classifier.h"
#include "ClassifierPropsWidget.h"
#include "ClassifierChannelPlotWindow.h"
#include "TopoplotWindow.h"
#include "EditMenu.h"
#include "FileDialog.h"
#include "HelpMenu.h"
#include "SignalWidget.h"
#include "WindowMenu.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDockWidget>
#include <QGroupBox>
#include <QMenuBar>

struct ClassifierWindow::Private : QObject
{
    QMainWindow *mpParent;
    AnalysisData mAnalysisData;
    GenericSignal mWeights;
    double mOffset, mVariance;

    Private(QMainWindow *);

    void setClassifierParameters(const Classifier &);

    void onApplyClassifierParameters();
    void onSaveClassifierParameters();
    void onSaveAllFilterParameters();

    void onShowCurvesAtChannel();
    void onShowDistributionAtElement();
    void onShowDistributionAt(int element);
    void onShowCurvesAt(int channel);
};

ClassifierWindow::Private::Private(QMainWindow *p) : QObject(p), mpParent(p)
{
}

void ClassifierWindow::Private::setClassifierParameters(const Classifier &trainedClassifier)
{
    ParamList &params = mAnalysisData.filterParams;
    if (!params.Exists("Classifier"))
        params.Add("Filtering matrix Classifier= 0 0");
    MutableParamRef Classifier(&params.ByPath("Classifier"));
    if (!params.Exists("NormalizerOffsets"))
        params.Add("Filtering list NormalizerOffsets= 0");
    MutableParamRef NormalizerOffsets(&params.ByPath("NormalizerOffsets"));
    if (!params.Exists("NormalizerGains"))
        params.Add("Filtering list NormalizerGains= 0");
    MutableParamRef NormalizerGains(&params.ByPath("NormalizerGains"));

    // Determine which channel we should apply our weights to.
    int maxClassifierChannel = 1;
    if (Classifier->NumColumns() != 4)
    {
        Classifier->SetNumColumns(4);
        Classifier->SetNumRows(0);
    }
    for (int row = 0; row < Classifier->NumRows(); ++row)
    {
        int channel = Classifier(row, 2);
        maxClassifierChannel = std::max(channel, maxClassifierChannel);
    }

    if (NormalizerOffsets->NumValues() < maxClassifierChannel)
        NormalizerOffsets->SetNumValues(maxClassifierChannel);
    if (NormalizerGains->NumValues() < maxClassifierChannel)
        NormalizerGains->SetNumValues(maxClassifierChannel);

    // Create a classifier matrix.
    const GenericSignal &weights = trainedClassifier.weightsAsSignal();
    Classifier->SetNumRows(weights.Channels() * weights.Elements());
    int row = 0;
    for (int ch = 0; ch < weights.Channels(); ++ch)
    {
        for (int el = 0; el < weights.Elements(); ++el)
        {
            Classifier(row, 0) = ch + 1;
            Classifier(row, 1) = el + 1;
            Classifier(row, 2) = maxClassifierChannel;
            Classifier(row, 3) = weights(ch, el);
            ++row;
        }
    }
    NormalizerOffsets(maxClassifierChannel - 1) = trainedClassifier.offset();
    NormalizerGains(maxClassifierChannel - 1) = 1.0 / ::sqrt(trainedClassifier.variance());
}

void ClassifierWindow::Private::onApplyClassifierParameters()
{
    AnalysisWindow *pWindow = nullptr;
    auto widgets = QApplication::topLevelWidgets();
    for (auto p : widgets)
    {
        pWindow = dynamic_cast<AnalysisWindow *>(p);
        if (pWindow)
            break;
    }
    Assert(pWindow);
    pWindow->setFilterParameters(mAnalysisData.filterParams);
}

void ClassifierWindow::Private::onSaveClassifierParameters()
{
    Assert(mAnalysisData.filterParams.Exists("Classifier"));
    Assert(mAnalysisData.filterParams.Exists("NormalizerOffsets"));
    Assert(mAnalysisData.filterParams.Exists("NormalizerGains"));
    ParamList classifierParams;
    classifierParams.Add(mAnalysisData.filterParams.ByPath("Classifier"));
    classifierParams.Add(mAnalysisData.filterParams.ByPath("NormalizerOffsets"));
    classifierParams.Add(mAnalysisData.filterParams.ByPath("NormalizerGains"));
    auto filename =
        FileDialog::getSaveFileName(mpParent, "Save Classifier Parameters", "BCI2000 Parameter File (*.prm)");
    if (!filename.isEmpty())
    {
        classifierParams.Save(filename.toLocal8Bit().constData());
    }
}

void ClassifierWindow::Private::onSaveAllFilterParameters()
{
    auto filename =
        FileDialog::getSaveFileName(mpParent, "Save All Filter Parameters", "BCI2000 Parameter File (*.prm)");
    if (!filename.isEmpty())
    {
        mAnalysisData.filterParams.Save(filename.toLocal8Bit().constData());
    }
}

void ClassifierWindow::Private::onShowCurvesAtChannel()
{
    int ch = ClassifierChannelPlotWindow::askForChannel(&mWeights.Properties());
    if (ch >= 0)
        onShowCurvesAt(ch);
}

void ClassifierWindow::Private::onShowDistributionAtElement()
{
    int el = TopoplotWindow::askForElement(&mWeights.Properties());
    if (el >= 0)
        onShowDistributionAt(el);
}

void ClassifierWindow::Private::onShowDistributionAt(int element)
{
    auto pWindow = new TopoplotWindow(mWeights, element, mpParent);
    pWindow->resize(400, 400);
    pWindow->show();
}

void ClassifierWindow::Private::onShowCurvesAt(int channel)
{
    auto pWindow = new ClassifierChannelPlotWindow(mWeights, channel, mpParent);
    pWindow->resize(400, 100);
    pWindow->show();
}


ClassifierWindow::ClassifierWindow(const AnalysisData &data, const Classifier &classifier)
    : QMainWindow(nullptr), p(new Private(this))
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    p->mAnalysisData = data;
    p->mWeights = classifier.weightsAsSignal();
    p->mOffset = classifier.offset();
    p->mVariance = classifier.variance();
    p->setClassifierParameters(classifier);

    static int count = 0;
    setWindowTitle(QString("%1 Classifier %2").arg(classifier.method()).arg(++count));

    //  QMainWindow::move(10, 10);
    QMenu *pMenu = QMainWindow::menuBar()->addMenu("&File");
    QAction *pAction = pMenu->addAction("&Apply Classifier Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onApplyClassifierParameters);
    pAction->setShortcut(Qt::CTRL + Qt::Key_A);
    pAction = pMenu->addAction("&Save Classifier Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveClassifierParameters);
    pAction->setShortcut(Qt::CTRL + Qt::Key_S);
    pAction = pMenu->addAction("Save All Filter &Parameters...");
    connect(pAction, &QAction::triggered, p, &Private::onSaveAllFilterParameters);
    pMenu->addSeparator();
    pAction = pMenu->addAction("&Close Window");
    pAction->setShortcut(Qt::CTRL + Qt::Key_W);
    connect(pAction, &QAction::triggered, this, &QMainWindow::close);

    EditMenu::addToMenuBar(menuBar());

    pMenu = QMainWindow::menuBar()->addMenu("&View");
    pMenu->addAction("Show &Weights for Channel...", p, &Private::onShowCurvesAtChannel);
    pMenu->addAction("Show &Distribution at Time/Frequency...", p, &Private::onShowDistributionAtElement);

    WindowMenu::addWindow(this);
    HelpMenu::addToMenuBar(menuBar());

    QDockWidget *pDockWidget = new QDockWidget("Classifier Information", this);
    pDockWidget->setFeatures(pDockWidget->features() & ~QDockWidget::DockWidgetClosable);
    auto pProps = new ClassifierPropsWidget(data, classifier, this);
    pDockWidget->setWidget(pProps);
    addDockWidget(Qt::LeftDockWidgetArea, pDockWidget);

    GenericSignal s = classifier.weightsAsSignal();
    for (int ch = 0; ch < s.Channels(); ++ch)
        for (int el = 0; el < s.Elements(); ++el)
            s(ch, el) = ::fabs(s(ch, el));
    auto pWidget = new EditMenu::CopyWidget(this);
    auto pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pWidget->setLayout(pLayout);
    QGroupBox *pGroupBox = new QGroupBox("Classifier weights (absolute values)");
    pWidget->layout()->addWidget(pGroupBox);
    pGroupBox->setLayout(new QHBoxLayout);
    auto pSignalWidget = new SignalWidget(s);
    pSignalWidget->setContextMenu(true);
    connect(pSignalWidget, &SignalWidget::showCurvesAt, p, &Private::onShowCurvesAt);
    connect(pSignalWidget, &SignalWidget::showDistributionAt, p, &Private::onShowDistributionAt);
    pGroupBox->layout()->addWidget(pSignalWidget);
    setCentralWidget(pWidget);

    setMinimumHeight(240);
    setMinimumWidth(320);
}

ClassifierWindow::~ClassifierWindow()
{
    WindowMenu::removeWindow(this);
}
