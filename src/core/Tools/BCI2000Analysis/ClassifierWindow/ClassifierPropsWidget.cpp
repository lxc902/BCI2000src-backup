////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A widget for classifier properties.
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
#include "ClassifierPropsWidget.h"
#include "AnalysisData.h"
#include "Classifier.h"
#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>

namespace
{
std::pair<double, double> computeAccuracy(const Classifier &classifier, const std::vector<GenericSignal> &data1,
                                          const std::vector<GenericSignal> &data2)
{
    int correct1 = 0, correct2 = 0;
    for (const auto &s : data1)
        if (classifier.classify(s) < 0)
            ++correct1;
    for (const auto &s : data2)
        if (classifier.classify(s) > 0)
            ++correct2;
    double raw = 1.0 * (correct1 + correct2) / (data1.size() + data2.size());
    double normalized = 0.5 * correct1 / data1.size() + 0.5 * correct2 / data2.size();
    return std::make_pair(normalized, raw);
}
} // namespace

struct ClassifierPropsWidget::Private : QObject
{
    Private(QObject *parent) : QObject(parent)
    {
    }
};

ClassifierPropsWidget::ClassifierPropsWidget(const AnalysisData &data, const Classifier &classifier, QWidget *parent)
    : QWidget(parent), p(new Private(this))
{
    auto trainingCorrect = computeAccuracy(classifier, data.trainingData1, data.trainingData2);
    auto testCorrect = computeAccuracy(classifier, data.testData1, data.testData2);

    QString text = "Training method: %1\n"
                   "Downsampling factor: %2\n"
                   "Offset: %3\n"
                   "Variance: %4\n";
    text = text.arg(classifier.method())
               .arg(classifier.downsamplingFactor())
               .arg(classifier.offset())
               .arg(classifier.variance());
    auto pLayout = new QVBoxLayout;
    setLayout(pLayout);
    auto pLabel = new QLabel(text);
    pLayout->addWidget(pLabel);
    auto pGroup = new QGroupBox("Accuracy on Training Data");
    pLayout->addWidget(pGroup);
    auto pLayout2 = new QVBoxLayout;
    pGroup->setLayout(pLayout2);
    text = "normalized: %1%\n"
           "raw: %2%\n"
           "N1 = %3, N2 = %4";
    pLabel = new QLabel(text.arg(trainingCorrect.first * 100)
                            .arg(trainingCorrect.second * 100)
                            .arg(data.trainingData1.size())
                            .arg(data.trainingData2.size()));
    pLayout2->addWidget(pLabel);
    pGroup = new QGroupBox("Accuracy on Test Data");
    pLayout->addWidget(pGroup);
    pLayout2 = new QVBoxLayout;
    pGroup->setLayout(pLayout2);
    pLabel = new QLabel(text.arg(testCorrect.first * 100)
                            .arg(testCorrect.second * 100)
                            .arg(data.testData1.size())
                            .arg(data.testData2.size()));
    pLayout2->addWidget(pLabel);
}

ClassifierPropsWidget::~ClassifierPropsWidget()
{
}
