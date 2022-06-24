////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A Win32 application that records video from a camera,
//   optionally saving to file and/or displaying in a window.
//   Dynamic application settings are controlled through stdio.
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
#include <Windows.h>

#include "Application.h"
#include "Window.h"
#include "WebcamPipeline.h"
#include "GrabberTransform.h"
#include "SaveToFileTransform.h"

#include "Exception.h"
#include "ProcessUtils.h"
#include "VersionInfo.h"
#include "IniFile.h"
#include "FileUtils.h"
#include "SysError.h"
#include "ExceptionCatcher.h"

#include <iostream>
#include <thread>

struct ExceptionCatcher_: ExceptionCatcher
{
  void OnReportException(const std::string& msg) override
  {
    ::MessageBoxA(NULL, msg.c_str(), "Exception Caught", MB_OK);
  }
};

static int WebcamRecorderMain(int argc, char** argv);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int nShowCmd)
{
  int argc = __argc;
  char** argv = __argv;

  ProcessUtils::WindowsEnableStdio(argc, argv);
  ProcessUtils::InitializeDesktopIntegration("org.bci2000.OnlineModules");

  FunctionCall<int(int, char**)> mainCall(&WebcamRecorderMain, argc, argv);
  if (ExceptionCatcher_().Run(mainCall))
    return mainCall.Result();
  return -1;
}

void IOThreadFunc(HWND window, SaveToFileTransform* pSaveToFile)
{
  try {
    std::string line;
    while (std::getline(std::cin, line)) {
      if (line.find("record ") == 0) {
        pSaveToFile->StartRecording(line.substr(7));
      }
      else if (line == "stop") {
        pSaveToFile->StopRecording();
      }
      else if (line.find("timestamp ") == 0) {
        pSaveToFile->SetTimestampMode(std::stoi(line.substr(10)));
      }
      else if (line.find("decimation ") == 0) {
        pSaveToFile->SetDecimation(std::stoi(line.substr(11)));
      }
      else if (line == "show") {
        ::ShowWindow(window, SW_SHOWNORMAL);
      }
      else if (line == "hide") {
        ::ShowWindow(window, SW_HIDE);
      }
      else if (line == "quit") {
        ::SendMessageA(window, WM_DESTROY, 0, 0);
        std::cout << std::endl;
        return;
      }
      else {
        std::cout << "Unknown command: \"" << line << "\"";
      }
      std::cout << std::endl;
    }
  } catch(const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

int WebcamRecorderMain(int argc, char** argv)
{
  int result = 0;

  int camera = 0, width = 640, height = 480, fps = 15;
  bool version = false;
  for (int i = 1; i < argc; ++i) {
    if (!::strcmp(argv[i], "--camera") && ++i < argc)
      camera = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--width") && ++i < argc)
      width = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--height") && ++i < argc)
      height = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--fps") && ++i < argc)
      fps = ::atoi(argv[i]);
    else if (!::strcmp(argv[i], "--version"))
      version = true;
    else
      return -1;
  }

  if (version) {
    VersionInfo::Current.InsertInto(std::cout, true) << std::endl;
    return 0;
  }

  struct : Window
  {
    com::Ptr<IMFVideoDisplayControl> mpVideoDisplayControl;
    bool OnPreTranslate(MSG* pMsg) override
    {
      if (pMsg->message == WM_SYSKEYDOWN) {
        if (pMsg->wParam == VK_F4)
          return true;
      }
      return Window::OnPreTranslate(pMsg);
    }
    bool OnClose() override
    {
      return false;
    }
    void OnCreate() override
    {
      HMENU hSysMenu = ::GetSystemMenu(Handle(), FALSE);
      if (hSysMenu)
        ::EnableMenuItem(hSysMenu, SC_CLOSE, (MF_DISABLED | MF_GRAYED | MF_BYCOMMAND));
    }
    void OnResize(int width, int height) override
    {
      if (mpVideoDisplayControl) {
        MFVideoNormalizedRect sourceRect = { 0, 0, 1, 1 };
        RECT destRect = { 0, 0, width, height };
        mpVideoDisplayControl->SetVideoPosition(&sourceRect, &destRect);
      }
    }
  } window;

  std::thread* pIOThread = nullptr;

  try {
    WebcamPipeline pipeline;

    VideoMediaType type;
    type.codec = FCC('raw ');
    type.width = width;
    type.height = height;
    type.framerateN = fps;
    type.framerateD = 1;
    pipeline.SetDesiredVideoMediaType(type);

    pipeline.SetCameraIndex(camera);
    std::string title = "Camera " + std::to_string(camera) + ": " + pipeline.CameraName();
    window.SetTitle(title);
    pipeline.SetWindowHandle(window.Handle());

    SaveToFileTransform saveToFile;
    pipeline.SetTransform(&saveToFile);

    pipeline.CreateTopology();

    ::OutputDebugStringA((pipeline.ActualMediaType().Describe() + "\n").c_str());
    auto pType = pipeline.ActualMediaType().Strip();
    pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
    saveToFile.AddInputAvailableType(pType);

    auto videoControl = pipeline.ResolveTopology();
    videoControl->SetAspectRatioMode(MFVideoARMode_PreservePicture);
    SIZE size;
    videoControl->GetNativeVideoSize(&size, nullptr);
    window.mpVideoDisplayControl = videoControl;

    pipeline.Start();
    std::string error = pipeline.Error();
    if (!error.empty())
      throw std_runtime_error << error;

    std::string iniFilePath = FileUtils::StripExtension(FileUtils::ExecutablePath());
    iniFilePath += "_" + std::to_string(camera) + ".ini";
    IniFile iniFile(iniFilePath);
    auto& geom = iniFile["Geometry"];
    if (geom.Empty()) {
      window.SetLeft(100 + 50 * camera).SetTop(100 + 50 * camera);
      window.SetWidth(size.cx / 2).SetHeight(size.cy / 2);
    }
    else {
      window.SetLeft(geom["Left"]).SetTop(geom["Top"]);
      window.SetWidth(geom["Width"]).SetHeight(geom["Height"]);
    }

    std::cout << std::endl; // signal that initialization is finished successfully
    pIOThread = new std::thread(IOThreadFunc, window.Handle(), &saveToFile);
    result = Application::Run();
    pIOThread->join();

    geom["Left"] = window.Left();
    geom["Top"] = window.Top();
    geom["Width"] = window.Width();
    geom["Height"] = window.Height();
  }
  catch(const std::exception& e)
  {
    std::string what = e.what();
    std::cout << what << std::endl;
    result = -1;
  }
  if (pIOThread && pIOThread->joinable())
    pIOThread->join();
  return result;
}

