/////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: This Logger simulates gaze data from an eyetracker and
//   changes states accordingly.
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --LogEyetrackerSimulator=1
//   As a command line option.
//   Information may be selectively logged by setting these parameters to true
//     LogGazeData - record gaze data to states
//     LogEyePosition - record eye position to states
//     LogPupilSize - record pupil size to states
//     LogEyeDist - record eye distances to states
//
// State Variables:
//   EyetrackerLeftEyeGazeX   - Left Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyeGazeY   - Left Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyeGazeX  - Right Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyeGazeY  - Right Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftEyePosX    - Left Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyePosY    - Left Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyePosX   - Right Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyePosY   - Right Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftPupilSize  - Left Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerRightPupilSize - Right Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerLeftEyeDist    - Left Eye Distance from Screen (mm), Approx
//   EyetrackerRightEyeDist   - Right Eye Distance from Screen (mm), Approx
//   EyetrackerTimeStamp      - The timestamp for the gaze data relative to BCI timer
//
// Expressions:
//   For each state variable, there is a parameter with the same name, holding an expression
//   from which to compute that state. Within the expression, the parameter t is a timestamp
//   increased by 1 for each time step.
//
// Parameter TimeStep:
//   Parameter determining the duration of a time step.
//
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
/////////////////////////////////////////////////////////////////////////////
#include "EyetrackerLoggerSimulator.h"
#include "BCIEvent.h"
#include "Thread.h"
#include "Expression.h"
#include "Rate.h"
#include "Waitable.h"

#include <vector>
#include <string>
#include <algorithm>

RegisterExtension(EyetrackerLoggerSimulator);

struct EyetrackerLoggerSimulator::Private : Thread
{
  int OnExecute() override;

  bool mEnabled = false;
  Expression mInit;
  std::vector<std::pair<std::string, Expression>> mExpressions;
  TimeUtils::TimeInterval mPeriod = 0;
};


EyetrackerLoggerSimulator::EyetrackerLoggerSimulator()
: p(new Private)
{
}

EyetrackerLoggerSimulator::~EyetrackerLoggerSimulator()
{
  Halt();
  delete p;
}

void EyetrackerLoggerSimulator::Publish()
{
  bool enabled = (OptionalParameter("LogEyetrackerSimulator") != 0);
  if (!enabled)
    return;

  BEGIN_PARAMETER_DEFINITIONS
    "Source:Eyetracker int /LogEyetrackerSimulator= 1 0 0 1"
    " // enable eye tracker logging (boolean)",
    "Source:Eyetracker float EyetrackerUpdateRate= 1Hz // update rate for simulated eyetracker data",
    "Source:Eyetracker string EyetrackerInitialization= omega:=2*pi/5 // initialization expression",
    "Source:Eyetracker matrix EyetrackerExpressions= {Left Right} "
       "{EyeGazeX EyeGazeY EyePosX EyePosY EyeDist PupilSize} "
       " sin(omega*t) cos(omega*t) % % % % cos(omega*t) sin(omega*t) % % % % % % % % % % % % % % % % // "
       "expressions for eyetracker data in normalized coordinate system [-1, 1]",
  END_PARAMETER_DEFINITIONS

  BEGIN_EVENT_DEFINITIONS
    "EyetrackerTime 16 0 0 0",
    "EyetrackerLeftPupilSize 16 0 0 0",
    "EyetrackerRightPupilSize 16 0 0 0",
    "EyetrackerLeftEyeGazeX 16 0 0 0",
    "EyetrackerRightEyeGazeX 16 0 0 0",
    "EyetrackerLeftEyeGazeY 16 0 0 0",
    "EyetrackerRightEyeGazeY 16 0 0 0",
    "EyetrackerLeftEyePosX 16 0 0 0",
    "EyetrackerLeftEyePosY 16 0 0 0",
    "EyetrackerRightEyePosX 16 0 0 0",
    "EyetrackerRightEyePosY 16 0 0 0",
    "EyetrackerLeftEyeDist 16 0 0 0",
    "EyetrackerRightEyeDist 16 0 0 0",
  END_EVENT_DEFINITIONS
}

void EyetrackerLoggerSimulator::Preflight() const
{
  bool enabled = (OptionalParameter("LogEyetrackerSimulator") != 0);
  if (enabled) {
    PreflightCondition(Parameter("EyetrackerUpdateRate").InHertz() > 0);
    ArithmeticExpression::VariableContainer variables;
    variables["t"] = 0.0;
    Expression init = Parameter("EyetrackerInitialization").c_str();
    init.Compile(variables);
    ParamRef expressions = Parameter("EyetrackerExpressions");
    for (int row = 0; row < expressions->NumRows(); ++row) {
      for (int col = 0; col < expressions->NumColumns(); ++col) {
        std::string stateName = "Eyetracker";
        stateName += expressions->RowLabels()[row];
        stateName += expressions->ColumnLabels()[col];
        if (!States->Exists(stateName))
          bcierr << "Invalid labels: there is no state named " << stateName;
        Expression expr(expressions(row, col));
        expr.Compile(variables);
        if (expr.RefersSignal())
          bcierr << "Eyetracker expression may not refer to signal: " << expr.AsString();
        if (expr.RefersStates())
          bcierr << "Eyetracker expression may not refer to states: " << expr.AsString();
      }
    }
  }
}


void EyetrackerLoggerSimulator::Initialize()
{
  p->Terminate();
  p->mExpressions.clear();
  p->mEnabled = (OptionalParameter("LogEyetrackerSimulator") != 0);
  if(p->mEnabled)
  {
    p->mPeriod = Rate::FromHertz(Parameter("EyetrackerUpdateRate").InHertz()).Period();
    p->mInit = Expression(Parameter("EyetrackerInitialization").c_str());
    ParamRef expressions = Parameter("EyetrackerExpressions");
    for (int row = 0; row < expressions->NumRows(); ++row) {
      for (int col = 0; col < expressions->NumColumns(); ++col) {
        std::string state = "Eyetracker";
        state += expressions->RowLabels()[row];
        state += expressions->ColumnLabels()[col];
        Expression expr = expressions(row, col);
        p->mExpressions.push_back(std::make_pair(state, expr));
      }
    }
  }
}

void EyetrackerLoggerSimulator::StartRun()
{
  if(p->mEnabled) {
    p->Start();
  }
}

void EyetrackerLoggerSimulator::StopRun()
{
  p->Terminate();
}

void EyetrackerLoggerSimulator::Halt()
{
  StopRun();
}

int EyetrackerLoggerSimulator::Private::OnExecute()
{
  ArithmeticExpression::VariableContainer variables;
  variables["t"] = 0;
  mInit.Compile(variables);
  mInit.Evaluate();

  for (auto& entry : mExpressions)
    entry.second.Compile(variables);

  Waitable::Timeout timeout;
  while (timeout.Wait(mPeriod)) {
    for (auto& entry : mExpressions) {
      double value = entry.second.Evaluate();
      value *= 32767;
      value += 32767;
      value = ::floor(value + 0.5);
      value = std::min(value, 65535.0);
      value = std::max(value, 0.0);
      bcievent << entry.first << " " << value;
    }
    variables["t"] += 1;
    bcievent << "EyetrackerTime " << PrecisionTime::Now();
  }
  return 0;
}