/////////////////////////////////////////////////////////////////////////////
// $Id: WebcamLogger.cpp 5817 2018-11-08 16:56:17Z mellinger $
// Authors: adam.wilson@uc.edu & Alex Belsten belsten@neurotechcenter.org,
//   mellinger@neurotechcenter.org
//
// Description: The WebcamController controls a WebcamRecorder instance
//   and records the current frame number to a bcievent.
//
// Event Variables:
//   WebcamFrame<n> - The current frame number for camera index n 
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
#include "WebcamController.h"

#include "BCIEvent.h"
#include "BCIStream.h"
#include "ChildProcess.h"
#include "Thread.h"
#include "Runnable.h"
#include "Streambuf.h"
#include "SysError.h"

struct WebcamController::Private
{
  std::string mError;

  int mCameraIndex = -1;
  ChildProcess* mpProcess = nullptr;
  std::iostream mStdIO{ nullptr };

  MemberCall<void(WebcamController::Private*)> mLoggerThreadCall{ 
    &WebcamController::Private::loggerThreadFunc, this
  };
  Thread mLoggerThread{
    &mLoggerThreadCall, "WebcamLogger event thread"
  };

  void loggerThreadFunc();
  void sendCommand(const std::string&);
};

void WebcamController::Private::sendCommand(const std::string& inCommand)
{
  if (!inCommand.empty())
    mStdIO << inCommand << std::endl;
  if (mpProcess->standardIO().Input()->Wait(Time::Seconds(1)))
  {
    std::string error;
    std::getline(mStdIO, error);
    if (!error.empty() && error.back() == '\r')
      error.pop_back();
    if (!error.empty() && mError.empty())
      mError = error;
  }
}

void WebcamController::Private::loggerThreadFunc()
{
  Waitables waitables;
  waitables.Add(mpProcess->standardError().Input());
  waitables.Add(mpProcess);
  const Waitable* pWaitable;
  while (pWaitable = waitables.Wait())
  {
    if (pWaitable == mpProcess->standardError().Input())
    {
      std::istream is(&mpProcess->standardError());
      std::string line;
      std::getline(is, line);
      if (!line.empty() && line.back() == '\r')
        line.pop_back();
      if (line == "end")
        return;
      bcievent << "WebcamFrame" << mCameraIndex << " " << line;
    }
    else if (pWaitable == mpProcess)
      return;
  }
}

const std::string& WebcamController::ExecutableName()
{
  static std::string executableName = "WebcamRecorder";
  return executableName;
}

WebcamController::WebcamController()
  : p(new Private)
{
}

WebcamController::~WebcamController()
{
  p->mLoggerThread.Terminate();
  p->sendCommand("quit");
  p->mpProcess->Wait(Time::Seconds(5));
  delete p->mpProcess;
  delete p;
}

const std::string& WebcamController::Error() const
{
  return p->mError;
}

void WebcamController::Initialize(int camIndex, int width, int height, int fps)
{
  p->mCameraIndex = camIndex;
  std::ostringstream args;
  args << "--camera " << camIndex << " --width " << width
       << " --height " << height << " --fps " << fps;
  p->mpProcess = new ChildProcess(ExecutableName(), args.str());
  p->mStdIO.set_rdbuf(&p->mpProcess->standardIO());
  p->mStdIO.clear();
  if (!p->mpProcess->run())
    p->mError = "Could not start " + ExecutableName() + " process: "
    + std::string(SysError(p->mpProcess->result()).Message());
  p->sendCommand("");
}

void WebcamController::ShowWindow()
{
  p->sendCommand("show");
}

void WebcamController::HideWindow()
{
  p->sendCommand("hide");
}

void WebcamController::SetDecimation(int decimation)
{
  p->sendCommand("decimation " + std::to_string(decimation));
}

void WebcamController::SetTimestampOption(int option)
{
  p->sendCommand("timestamp " + std::to_string(option));
}

void WebcamController::StartRecording(const std::string& baseName)
{
  std::string outputFileName = baseName + "_" + std::to_string(p->mCameraIndex) + "_vid.mp4";
  p->sendCommand("record " + outputFileName);
  if (p->mError.empty()) {
    p->mLoggerThread.Start();
    bciout << "Started Recording Camera " << p->mCameraIndex;
  }
}

void WebcamController::StopRecording()
{
  p->sendCommand("stop");
  p->mLoggerThread.Terminate();
  bciout << "Stopped Recording Camera " << p->mCameraIndex;
}

