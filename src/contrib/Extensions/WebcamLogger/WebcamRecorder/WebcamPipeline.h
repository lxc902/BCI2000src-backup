////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A Windows Media Foundation pipeline which reads video input
//   from a camera, and displays it in a window. Optionally, an intermediate
//   transform may be specified.
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
#ifndef WEBCAM_PIPELINE_H
#define WEBCAM_PIPELINE_H

#include <Windows.h>

#include <mfidl.h>
#include <mfapi.h>
#include <evr.h>

#include <string>
#include <mutex>
#include "ComPtr.h"

#include "VideoMediaType.h"
#include "MFInit.h"

class WebcamPipeline : private IMFAsyncCallback
{
public:
  WebcamPipeline();
  ~WebcamPipeline();

  std::string Error() const;

  void SetDesiredVideoMediaType(const VideoMediaType&);
  const VideoMediaType& ActualMediaType() const;

  void SetCameraIndex(int);
  int CameraIndex() const;
  const std::string& CameraName() const;

  void SetTransform(com::Ptr<IMFTransform>);
  com::Ptr<IMFTransform> Transform() const;

  void SetWindowHandle(HWND);
  HWND WindowHandle() const;

  void CreateTopology();
  com::Ptr<IMFVideoDisplayControl> ResolveTopology();

  void Start();
  void Pause();
  void Stop();

protected:
  // IMFAsyncCallback
  HRESULT __stdcall GetParameters(DWORD* pdwFlags, DWORD* pdwQueue) override;
  HRESULT __stdcall Invoke(IMFAsyncResult* pAsyncResult) override;

  void OnEvent(IMFMediaEvent*, MediaEventType);
  virtual void OnMediaEvent(IMFMediaEvent*, MediaEventType);
  virtual void OnMediaError(HRESULT);

  virtual void OnTopologyStatusChanged(MF_TOPOSTATUS);

private:
  // IUnknown
  HRESULT __stdcall QueryInterface(const IID& inIID, void** outInterface) override;
  ULONG __stdcall AddRef() override;
  ULONG __stdcall Release() override;

  void GetVideoInputDescriptors(com::Ptr<IMFStreamDescriptor>&, com::Ptr<IMFPresentationDescriptor>&);
  void ChooseVideoType(IMFMediaTypeHandler*);
  void WaitForDone();
  void SetError(const std::string&);

private:
  ULONG mRefCount;

  VideoMediaType mDesiredVideoMediaType, mActualVideoMediaType;

  com::Ptr<IMFMediaSession> mpMediaSession;

  int mCameraIndex;
  std::string mCameraName;

  com::Ptr<IMFMediaSource> mpSource;
  com::Ptr<IMFTransform> mpTransform;
  com::Ptr<IMFActivate> mpRendererActivate;
  com::Ptr<IMFTopology> mpTopology;
  com::Ptr<IMFVideoDisplayControl> mpVideoDisplayControl;

  HWND mWindowHandle;
  HANDLE mDoneEvent;
  mutable std::mutex mMutex;
  std::string mError;
  MFInit mInit;
};

#endif // WEBCAM_PIPELINE_H
