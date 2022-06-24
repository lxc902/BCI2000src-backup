////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A struct that holds data analysis parameters, and training
//   data.
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
#include "AnalysisData.h"
#include "Exception.h"

namespace
{
void writeFileList(std::ostream &os, const std::vector<std::pair<std::string, std::string>> &list)
{
    for (const auto &file : list)
        os << file.first << '\t' << file.second << '\n';
    os << '\n';
}

void readFileList(std::istream &is, std::vector<std::pair<std::string, std::string>> &list)
{
    list.clear();
    std::string s;
    std::getline(is, s);
    while (!s.empty())
    {
        auto pos = s.find('\t');
        if (pos != s.npos)
            list.push_back(std::make_pair(s.substr(0, pos), s.substr(pos + 1)));
        else
            list.push_back(std::make_pair(s, s));
        std::getline(is, s);
    }
}

void writeData(std::ostream &os, const std::vector<GenericSignal> &container)
{
    os << container.size() << '\n';
    for (const auto &s : container)
        s.Serialize(os);
    os << '\n';
}

void readData(std::istream &is, const SignalProperties &properties, std::vector<GenericSignal> &container)
{
    size_t length = 0;
    is >> length;
    if (!is || is.peek() != '\n')
        throw std_runtime_error << "Could not read length information in data section";
    is.ignore();
    container.clear();
    container.reserve(length);
    GenericSignal signal(properties);
    for (size_t i = 0; i < length; ++i)
    {
        signal.Unserialize(is);
        container.push_back(signal);
    }
    if (is.peek() != '\n')
        throw std_runtime_error << "Unexpected character at end of data section: " << char(is.peek());
    is.ignore();
}
} // namespace

const SignalProperties *AnalysisData::signalProperties() const
{
    if (!trainingData1.empty())
        return &trainingData1.front().Properties();
    if (!trainingData2.empty())
        return &trainingData2.front().Properties();
    if (!testData1.empty())
        return &testData1.front().Properties();
    if (!testData2.empty())
        return &testData2.front().Properties();
    return nullptr;
}

void AnalysisData::serialize(std::ostream &os) const
{
    os << "[Training Files]\n";
    writeFileList(os, trainingFiles);
    os << "[Test Files]\n";
    writeFileList(os, testFiles);
    os << "[Filter Graph]\n";
    filterGraph.serialize(os);
    os << '\n';
    os << "[Filter Parameters]\n";
    filterParams.Serialize(os);
    os << '\n';
    os << "[Analysis Parameters]\n";
    analysisParams.serialize(os);
    const GenericSignal *pSignal = nullptr;
    if (!trainingData1.empty())
        pSignal = &trainingData1.front();
    else if (!trainingData2.empty())
        pSignal = &trainingData2.front();
    else if (!testData1.empty())
        pSignal = &testData1.front();
    else if (!testData2.empty())
        pSignal = &testData2.front();
    if (!pSignal)
        return;
    os << "[Signal Properties]\n";
    pSignal->Properties().InsertInto(os);
    os << '\n';
    if (!trainingData1.empty())
    {
        os << "[Training Data 1]\n";
        writeData(os, trainingData1);
    }
    if (!trainingData2.empty())
    {
        os << "[Training Data 2]\n";
        writeData(os, trainingData2);
    }
    if (!testData1.empty())
    {
        os << "[Test Data 1]\n";
        writeData(os, testData1);
    }
    if (!testData2.empty())
    {
        os << "[Test Data 2]\n";
        writeData(os, testData2);
    }
}

AnalysisData AnalysisData::unserialize(std::istream &is)
{
    AnalysisData data;
    std::string section;
    SignalProperties signalProperties;
    while (std::getline(is >> std::ws, section))
    {
        if (section == "[Training Files]")
            readFileList(is, data.trainingFiles);
        else if (section == "[Test Files]")
            readFileList(is, data.testFiles);
        else if (section == "[Filter Graph]")
            data.filterGraph = FilterGraph::Graph::unserialize(is);
        else if (section == "[Filter Parameters]")
        {
            std::string params, s;
            std::getline(is, s);
            while (!s.empty())
            {
                params += s + '\n';
                std::getline(is, s);
            }
            std::istringstream iss(params);
            data.filterParams.Unserialize(iss);
        }
        else if (section == "[Analysis Parameters]")
            data.analysisParams = AnalysisParamWidget::AnalysisParams::unserialize(is);
        else if (section == "[Signal Properties]")
            signalProperties.ExtractFrom(is);
        else if (section == "[Training Data 1]")
            readData(is, signalProperties, data.trainingData1);
        else if (section == "[Training Data 2]")
            readData(is, signalProperties, data.trainingData2);
        else if (section == "[Test Data 1]")
            readData(is, signalProperties, data.testData1);
        else if (section == "[Test Data 2]")
            readData(is, signalProperties, data.testData2);
        else
            throw std_runtime_error << "Unexpected section header: " << section;
    }
    return data;
}
