////////////////////////////////////////////////////////////////////
// $Id: bci_dat2stream.cpp 6618 2022-03-11 22:48:07Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: See the ToolInfo definition below.
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
////////////////////////////////////////////////////////////////////
#include "bci_tool.h"

#include "BCI2000FileReader.h"
#include "GenericSignal.h"
#include "MeasurementUnits.h"
#include "MessageChannel.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"

#include <cassert>
#include <fstream>

std::string ToolInfo[] = {"bci_dat2stream",
                          "Convert a BCI2000 data file into a BCI2000 stream.",
                          "Reads a BCI2000 data file (*.dat) compliant stream from "
                          "standard input and writes it to the standard output as a BCI2000 "
                          "compliant binary stream.",
                          "binary",
                          "-t,       --transmit={spd}      Select States, Parameters, and/or",
                          "                                Data for transmission",
                          "-r,       --raw                 Transmit uncalibrated data",
                          "-p<file>, --parameters=<file>   Incorporate parameters from named file",
                          "-s<time>, --start=<time>        Start at a given offset within the file",
                          "-d<time>, --duration=<time>     Transmit only a limited amount of signal",
                          " ",
                          "Amounts of <time> are expressed in SampleBlocks or (if the unit is",
                          "explicitly appended) as a number of seconds or milliseconds that",
                          "corresponds to a whole number of SampleBlocks.",
                          ""};

ToolResult ToolInit()
{
    return noError;
}

ToolResult ToolMain(Streambuf &inbuf, Streambuf &outbuf, OptionSet &options)
{
    std::chrono::high_resolution_clock::time_point beginTime, endTime;
    beginTime = std::chrono::high_resolution_clock::now();

    ToolResult result = noError;
    std::string transmissionList = options.getopt("-t|-T|--transmit", "spd");
    bool transmitStates = (transmissionList.find_first_of("sS") != std::string::npos),
         transmitParameters = (transmissionList.find_first_of("pP") != std::string::npos),
         transmitData = (transmissionList.find_first_of("dD") != std::string::npos),
         calibrateData = !options.findopt("-r|-R|--raw");
    std::string paramFileName = options.getopt("-p|-P|--parameters", "");
    std::string offsetString = options.getopt("-s|-S|--start", "");
    std::string durationString = options.getopt("-d|-D|--duration", "");

    // Read the BCI2000 header.
    std::string token;
    int headerLength, sourceCh, stateVectorLength;
    SignalType dataFormat;
    StateList states;
    enum
    {
        v10,
        v11
    } fileFormatVersion = v10;

    std::string fName = options.getopt("-i|-I|--input", "");

    ////////////////////////
    // If a file name is given, use the BCI2000FileReader to read the file. It is faster
    // If file is not specified, use the stream
    ///////////////////////
    if (fName.empty())
    {
        std::istream in(&inbuf);

        bool legalInput = !!(in >> token);
        if (legalInput && token == "BCI2000V=")
        {
            legalInput = legalInput && in >> token;
            if (token == "1.1")
                fileFormatVersion = v11;
            else
                legalInput = false;
            legalInput = legalInput && in >> token;
        }
        legalInput &= token == "HeaderLen=" && in >> headerLength && in >> token && token == "SourceCh=" &&
                      in >> sourceCh && in >> token && token == "StatevectorLen=" && in >> stateVectorLength;
        switch (fileFormatVersion)
        {
        case v10:
            dataFormat = SignalType::int16;
            break;
        case v11:
            legalInput &= in >> token && token == "DataFormat=" && in >> dataFormat;
            break;
        default:
            assert(false);
        }
        legalInput &= std::getline(in >> std::ws, token, ']') >> std::ws && token == "[ State Vector Definition ";
        while (legalInput && in.peek() != '[' && getline(in, token))
        {
            std::istringstream is(token);
            State state;
            if (is >> state)
            {
                states.Delete(state.Name());
                states.Add(state);
            }
            legalInput = legalInput && is;
        }

        MessageChannel output(outbuf);
        if (transmitStates)
        { // Transmit states ordered by name, i.e. independently of their order in the file.
            std::vector<std::string> stateNames;
            for (const auto &state : states)
                stateNames.push_back(state.Name());
            sort(stateNames.begin(), stateNames.end(), State::NameCmp());
            for (const auto &name : stateNames)
                output.Send(states.ByName(name));
        }

        legalInput &= std::getline(in >> std::ws, token, ']') >> std::ws && token == "[ Parameter Definition ";

        ParamList parameters;
        while (legalInput && getline(in, token) && token.length() > 1)
        {
            std::istringstream is(token);
            Param param;
            if (is >> param)
                parameters.ByPath(param.Name()) = param;
            legalInput = legalInput && is;
            if (transmitParameters)
                output.Send(param);
        }
        if (paramFileName.size())
        {
            std::ifstream pIn(paramFileName.c_str(), std::ifstream::in);
            while (pIn.good() && std::getline(pIn, token) && token.length() > 1)
            {
                std::istringstream is(token);
                Param param;
                if (is >> param)
                    parameters.ByPath(param.Name()) = param;
                if (transmitParameters)
                    output.Send(param);
            }
        }
        MeasurementUnits::Initialize(parameters);
        double offset = (offsetString.size() ? MeasurementUnits::TimeInSampleBlocks(offsetString) : 0.0);
        double duration = (durationString.size() ? MeasurementUnits::TimeInSampleBlocks(durationString) : -1.0);

        int sampleBlockSize = 0;
        if (legalInput)
            sampleBlockSize = static_cast<int>(PhysicalUnit().SetGain(1.0).SetOffset(0.0).SetSymbol("").PhysicalToRaw(
                parameters.ByPath("SampleBlockSize").Value()));
        legalInput &= (sampleBlockSize > 0);
        if (!legalInput)
        {
            std::cerr << "Illegal header format or content" << std::endl;
            return illegalInput;
        }

        SignalProperties inputProperties(sourceCh, sampleBlockSize, dataFormat);
        if (transmitData || transmitStates)
        {
            StateVector statevector(states, sampleBlockSize + 1);
            if (statevector.Length() != stateVectorLength)
            {
                std::cerr << "Statevector's length (" << statevector.Length() << ")"
                          << " differs from StateVectorLen field (" << stateVectorLength << ")" << std::endl;
                return illegalInput;
            }
            if (calibrateData && parameters.Exists("SamplingRate"))
            {
                double samplingRate = PhysicalUnit().SetOffset(0.0).SetGain(1.0).SetSymbol("Hz").PhysicalToRaw(
                    parameters.ByPath("SamplingRate").Value());
                inputProperties.ElementUnit().SetOffset(0).SetGain(1.0 / samplingRate).SetSymbol("s");
            }
            std::vector<double> offsets(sourceCh, 0), gains(sourceCh, 1);
            if (calibrateData && parameters.Exists("SourceChOffset"))
            {
                const Param &sourceChOffset = parameters.ByPath("SourceChOffset");
                for (int ch = 0; ch < std::min(sourceCh, sourceChOffset.NumValues()); ++ch)
                    offsets[ch] = ::atof(sourceChOffset.Value(ch).c_str());
            }
            if (calibrateData && parameters.Exists("SourceChGain"))
            {
                const Param &sourceChGain = parameters.ByPath("SourceChGain");
                for (int ch = 0; ch < std::min(sourceCh, sourceChGain.NumValues()); ++ch)
                    gains[ch] = ::atof(sourceChGain.Value(ch).c_str());
                inputProperties.ValueUnit().SetOffset(0).SetGain(1e-6).SetSymbol("V");
            }
            if (parameters.Exists("ChannelNames") && parameters.ByPath("ChannelNames").NumValues() > 0)
            {
                LabelIndex &outputLabels = inputProperties.ChannelLabels();
                for (int i = 0; i < std::min(inputProperties.Channels(), parameters.ByPath("ChannelNames").NumValues());
                     ++i)
                    outputLabels[i] = parameters.ByPath("ChannelNames").Value(i).c_str();
            }
            if (transmitData)
            {
                if (calibrateData)
                {
                    SignalProperties outputProperties(inputProperties);
                    outputProperties.SetType(SignalType::float32);
                    output.Send(outputProperties);
                }
                else
                    output.Send(inputProperties);
            }

            int curSample = 0;
            int nBlocksRead = 0, nBlocksTransmitted = 0;
            GenericSignal inputSignal(inputProperties);

            while (in && in.peek() != EOF && (duration < 0.0 || nBlocksTransmitted < duration))
            {
                for (int i = 0; i < sourceCh; ++i)
                    inputSignal.ReadValueBinary(in, i, curSample);

                in.read(statevector.Data(curSample), statevector.Length());

                if (++curSample == sampleBlockSize)
                {
                    curSample = 0;
                    if (++nBlocksRead > offset)
                    {
                        if (transmitStates)
                        {
                            output.Send(statevector);
                        }
                        if (transmitData)
                        {
                            if (calibrateData)
                            {
                                SignalProperties outputProperties(inputProperties);
                                outputProperties.SetType(SignalType::float32);
                                GenericSignal outputSignal(outputProperties);
                                for (int i = 0; i < sourceCh; ++i)
                                    for (int j = 0; j < sampleBlockSize; ++j)
                                        outputSignal(i, j) = (inputSignal(i, j) - offsets[i]) * gains[i];
                                // Send the data.
                                output.Send(outputSignal);
                            }
                            else
                                output.Send(inputSignal);
                        }
                        nBlocksTransmitted++;
                    }
                }
            }

            if (curSample != 0)
            {
                std::cerr << "Non-integer number of data blocks in input" << std::endl;
                result = illegalInput;
            }
        }
    }
    else
    {
        BCI2000FileReader file;
        file.Open(fName.c_str());

        //fileFormatVersion = strcmp(file.FileFormatVersion().c_str(), "1.1") ? v11 : v10;
        //headerLength = file.HeaderLength();
        sourceCh = file.SourceCh();
        stateVectorLength = file.StateVectorLength();
        dataFormat = file.DataFormat();

        states = *(file.States());
        MessageChannel output(outbuf);
        if (transmitStates)
        { // Transmit states ordered by name, i.e. independently of their order in the file.
            std::vector<std::string> stateNames;
            for (const auto &state : states)
                stateNames.push_back(state.Name());
            std::sort(stateNames.begin(), stateNames.end(), State::NameCmp());
            for (const auto &name : stateNames)
                output.Send(states.ByName(name));
        }

        ParamList parameters;
        parameters = *(file.Parameters());

        if (transmitParameters)
        {
            for (const Param &param : parameters)
                output.Send(param);
        }

        if (paramFileName.size())
        {
            std::ifstream pIn(paramFileName.c_str(), std::ifstream::in);
            while (pIn.good() && getline(pIn, token) && token.length() > 1)
            {
                std::istringstream is(token);
                Param param;
                if (is >> param)
                    parameters.ByPath(param.Name()) = param;
                if (transmitParameters)
                    output.Send(param);
            }
        }
        MeasurementUnits::Initialize(parameters);
        double offset = (offsetString.size() ? MeasurementUnits::TimeInSampleBlocks(offsetString) : 0.0);
        double duration = (durationString.size() ? MeasurementUnits::TimeInSampleBlocks(durationString) : -1.0);

        int sampleBlockSize = 0;

        if (parameters.Exists("SampleBlockSize"))
            sampleBlockSize = static_cast<int>(PhysicalUnit().SetGain(1.0).SetOffset(0.0).SetSymbol("").PhysicalToRaw(
                parameters.ByPath("SampleBlockSize").Value().c_str()));

        if (sampleBlockSize <= 0)
        {
            std::cerr << "Illegal header format or content" << std::endl;
            return illegalInput;
        }

        SignalProperties inputProperties(sourceCh, sampleBlockSize, dataFormat);
        if (transmitData || transmitStates)
        {
            StateVector statevector(states, sampleBlockSize + 1);
            if (statevector.Length() != stateVectorLength)
            {
                std::cerr << "Statevector's length (" << statevector.Length() << ")"
                          << " differs from StateVectorLen field (" << stateVectorLength << ")" << std::endl;
                return illegalInput;
            }
            if (calibrateData && parameters.Exists("SamplingRate"))
            {
                double samplingRate = PhysicalUnit().SetOffset(0.0).SetGain(1.0).SetSymbol("Hz").PhysicalToRaw(
                    parameters.ByPath("SamplingRate").Value());
                inputProperties.ElementUnit().SetOffset(0).SetGain(1.0 / samplingRate).SetSymbol("s");
            }
            std::vector<double> offsets(sourceCh, 0), gains(sourceCh, 1);
            if (calibrateData && parameters.Exists("SourceChOffset"))
            {
                const Param &sourceChOffset = parameters.ByPath("SourceChOffset");
                for (int ch = 0; ch < std::min(sourceCh, sourceChOffset.NumValues()); ++ch)
                    offsets[ch] = ::atof(sourceChOffset.Value(ch).c_str());
            }
            if (calibrateData && parameters.Exists("SourceChGain"))
            {
                const Param &sourceChGain = parameters.ByPath("SourceChGain");
                for (int ch = 0; ch < std::min(sourceCh, sourceChGain.NumValues()); ++ch)
                    gains[ch] = ::atof(sourceChGain.Value(ch).c_str());
                inputProperties.ValueUnit().SetOffset(0).SetGain(1e-6).SetSymbol("V");
            }
            if (parameters.Exists("ChannelNames") && parameters.ByPath("ChannelNames").NumValues() > 0)
            {
                LabelIndex &outputLabels = inputProperties.ChannelLabels();
                for (int i = 0; i < std::min(inputProperties.Channels(), parameters.ByPath("ChannelNames").NumValues());
                     ++i)
                    outputLabels[i] = parameters.ByPath("ChannelNames").Value(i).c_str();
            }
            if (transmitData)
            {
                if (calibrateData)
                {
                    SignalProperties outputProperties(inputProperties);
                    outputProperties.SetType(SignalType::float32);
                    output.Send(outputProperties);
                }
                else
                    output.Send(inputProperties);
            }

            int curSample = 0;
            int nBlocksRead = 0, nBlocksTransmitted = 0;
            GenericSignal inputSignal(inputProperties);

            int sampleCount = 0;
            ///////////////////////
            // Derive a code on the basis of whether we want to transmit states, data or calibrated data
            // Choose a suitable case in the switch statement to transfer the data accordingly
            // Saves time as compared to checking for these states in while loop everytime
            // Bit(Possible values) - Transmit States(0/1), Transmit Data(0/1), CalibrateData(0/1)
            // There are six possible cases -
            // 1. Transmit States(0), Transmit Data(0), CalibrateData(0) - 0
            // 2. Transmit States(0), Transmit Data(1), CalibrateData(0) - 2
            // 3. Transmit States(0), Transmit Data(1), CalibrateData(1) - 3
            // 4. Transmit States(1), Transmit Data(0), CalibrateData(0) - 4
            // 5. Transmit States(1), Transmit Data(1), CalibrateData(0) - 6
            // 6. Transmit States(1), Transmit Data(1), CalibrateData(1) - 7
            // It's no use to calibrateData if we are not transmitting the data. Hence, omitting the two cases where
            // Transmit Data(0), CalibrateData(1)
            ///////////////////////
            int switchCase = 0;
            if (transmitStates)
            {
                if (transmitData)
                {
                    switchCase = calibrateData ? 7 : 6; // Transmit States(1), Transmit Data(1), CalibrateData(1) - 7;
                                                        // Transmit States(1), Transmit Data(1), CalibrateData(0) - 6;
                }
                else
                {
                    switchCase = 4;
                }
            }
            else
            {
                if (transmitData)
                {
                    switchCase = calibrateData ? 3 : 2;
                }
                else
                {
                    switchCase = 0;
                }
            }

            switch (switchCase)
            {
            case 0: // Transmit States(0), Transmit Data(0), CalibrateData(0) - 0
                while (sampleCount < file.NumSamples() && (duration < 0.0 || nBlocksTransmitted < duration))
                {
                    for (int i = 0; i < sourceCh; ++i)
                        inputSignal.SetValue(i, curSample, file.RawValue(i, sampleCount));

                    file.ReadStateVector(sampleCount);
                    ::memcpy(statevector.Data(curSample), file.StateVector()->Data(), stateVectorLength);

                    if (++curSample == sampleBlockSize)
                    {
                        curSample = 0;
                        if (++nBlocksRead > offset)
                        {
                            nBlocksTransmitted++;
                        }
                    }
                    ++sampleCount;
                }
                break;
            case 2: // Transmit States(0), Transmit Data(1), CalibrateData(0) - 2
                while (sampleCount < file.NumSamples() && (duration < 0.0 || nBlocksTransmitted < duration))
                {
                    for (int i = 0; i < sourceCh; ++i)
                        inputSignal.SetValue(i, curSample, file.RawValue(i, sampleCount));

                    file.ReadStateVector(sampleCount);
                    ::memcpy(statevector.Data(curSample), file.StateVector()->Data(), stateVectorLength);

                    if (++curSample == sampleBlockSize)
                    {
                        curSample = 0;
                        if (++nBlocksRead > offset)
                        {
                            output.Send(inputSignal);
                            nBlocksTransmitted++;
                        }
                    }
                    ++sampleCount;
                }
                break;
            case 3: // Transmit States(0), Transmit Data(1), CalibrateData(1) - 3
                while (sampleCount < file.NumSamples() && (duration < 0.0 || nBlocksTransmitted < duration))
                {
                    for (int i = 0; i < sourceCh; ++i)
                        inputSignal.SetValue(i, curSample, file.RawValue(i, sampleCount));

                    file.ReadStateVector(sampleCount);
                    ::memcpy(statevector.Data(curSample), file.StateVector()->Data(), stateVectorLength);

                    if (++curSample == sampleBlockSize)
                    {
                        curSample = 0;
                        if (++nBlocksRead > offset)
                        {
                            SignalProperties outputProperties(inputProperties);
                            outputProperties.SetType(SignalType::float32);
                            GenericSignal outputSignal(outputProperties);
                            for (int i = 0; i < sourceCh; ++i)
                                for (int j = 0; j < sampleBlockSize; ++j)
                                    outputSignal(i, j) = (inputSignal(i, j) - offsets[i]) * gains[i];
                            // Send the data.
                            output.Send(outputSignal);
                            nBlocksTransmitted++;
                        }
                    }
                    ++sampleCount;
                }
                break;
            case 4: // Transmit States(1), Transmit Data(0), CalibrateData(0) - 4
                while (sampleCount < file.NumSamples() && (duration < 0.0 || nBlocksTransmitted < duration))
                {
                    for (int i = 0; i < sourceCh; ++i)
                        inputSignal.SetValue(i, curSample, file.RawValue(i, sampleCount));

                    file.ReadStateVector(sampleCount);
                    ::memcpy(statevector.Data(curSample), file.StateVector()->Data(), stateVectorLength);

                    if (++curSample == sampleBlockSize)
                    {
                        curSample = 0;
                        if (++nBlocksRead > offset)
                        {
                            output.Send(statevector);
                            nBlocksTransmitted++;
                        }
                    }
                    ++sampleCount;
                }
                break;
            case 6: // Transmit States(1), Transmit Data(1), CalibrateData(0) - 6
                while (sampleCount < file.NumSamples() && (duration < 0.0 || nBlocksTransmitted < duration))
                {
                    for (int i = 0; i < sourceCh; ++i)
                        inputSignal.SetValue(i, curSample, file.RawValue(i, sampleCount));

                    file.ReadStateVector(sampleCount);
                    ::memcpy(statevector.Data(curSample), file.StateVector()->Data(), stateVectorLength);

                    if (++curSample == sampleBlockSize)
                    {
                        curSample = 0;
                        if (++nBlocksRead > offset)
                        {
                            output.Send(statevector);
                            output.Send(inputSignal);
                            nBlocksTransmitted++;
                        }
                    }
                    ++sampleCount;
                }
                break;
            case 7: // Transmit States(1), Transmit Data(1), CalibrateData(1) - 7
                while (sampleCount < file.NumSamples() && (duration < 0.0 || nBlocksTransmitted < duration))
                {
                    for (int i = 0; i < sourceCh; ++i)
                        inputSignal.SetValue(i, curSample, file.RawValue(i, sampleCount));

                    file.ReadStateVector(sampleCount);
                    ::memcpy(statevector.Data(curSample), file.StateVector()->Data(), stateVectorLength);

                    if (++curSample == sampleBlockSize)
                    {
                        curSample = 0;
                        if (++nBlocksRead > offset)
                        {
                            output.Send(statevector);
                            SignalProperties outputProperties(inputProperties);
                            outputProperties.SetType(SignalType::float32);
                            GenericSignal outputSignal(outputProperties);
                            for (int i = 0; i < sourceCh; ++i)
                                for (int j = 0; j < sampleBlockSize; ++j)
                                    outputSignal(i, j) = (inputSignal(i, j) - offsets[i]) * gains[i];
                            // Send the data.
                            output.Send(outputSignal);
                            nBlocksTransmitted++;
                        }
                    }
                    ++sampleCount;
                }
                break;
            default:
                std::cerr << "Incorrect states for transmitStates, transmitData and calibrateData" << std::endl;
                return illegalOption;
                break;
            }
            /*
            while (sampleCount < file.NumSamples() && (duration < 0.0 || nBlocksTransmitted < duration))
            {
                beginForLoopTime = chrono::high_resolution_clock::now();
                for (int i = 0; i < sourceCh; ++i)
                    inputSignal.SetValue(i, curSample, file.RawValue(i, sampleCount));
                endForLoopTime = chrono::high_resolution_clock::now();

                file.ReadStateVector(sampleCount);
                ::memcpy(statevector.Data(curSample), file.StateVector()->Data(), stateVectorLength);
                endInReadTime = chrono::high_resolution_clock::now();


                if (++curSample == sampleBlockSize)
                {
                    curSample = 0;
                    if (++nBlocksRead > offset)
                    {
                        if (transmitStates)
                        {
                            output.Send(statevector);
                        }
                        if (transmitData)
                        {
                            if (calibrateData)
                            {
                                SignalProperties outputProperties(inputProperties);
                                outputProperties.SetType(SignalType::float32);
                                GenericSignal outputSignal(outputProperties);
                                for (int i = 0; i < sourceCh; ++i)
                                    for (int j = 0; j < sampleBlockSize; ++j)
                                        outputSignal(i, j) = (inputSignal(i, j) - offsets[i]) * gains[i];
                                // Send the data.
                                output.Send(outputSignal);
                            }
                            else
                                output.Send(inputSignal);
                        }
                        nBlocksTransmitted++;
                    }
                }
                ++sampleCount;
                ++niter;
                endOuterIfTime = chrono::high_resolution_clock::now();
                totalForLoopTime += chrono::duration_cast<chrono::nanoseconds>(endForLoopTime -
            beginForLoopTime).count(); totalInReadTime += chrono::duration_cast<chrono::nanoseconds>(endInReadTime -
            endForLoopTime).count(); totalOuterIfTime += chrono::duration_cast<chrono::nanoseconds>(endOuterIfTime -
            endInReadTime).count();
            }*/

            if (curSample != 0)
            {
                std::cerr << "Non-integer number of data blocks in input" << std::endl;
                result = illegalInput;
            }
        }
    }
    return result;
}
