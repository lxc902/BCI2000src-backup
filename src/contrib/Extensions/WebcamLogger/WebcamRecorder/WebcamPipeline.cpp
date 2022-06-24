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
#include "WebcamPipeline.h"
#include "Errors.h"
#include "ComStrings.h"
#include "Exception.h"

#include <vector>
#include <algorithm>

namespace {
  class DeviceList
  {
  public:
    DeviceList(IMFAttributes* attr)
    {
      HRSucceedOrThrow hr = ::MFEnumDeviceSources(attr, &mpList, &mCount);
    }
    ~DeviceList()
    {
      for (int i = 0; i < mCount; ++i)
        mpList[i]->Release();
      ::CoTaskMemFree(mpList);
    }
    uint32_t size()
    {
      return mCount;
    }
    com::Ptr<IMFActivate> operator[](int i)
    {
      return mpList[i];
    }
  private:
    IMFActivate** mpList = nullptr;
    UINT32 mCount = 0;
  };
}

WebcamPipeline::WebcamPipeline()
  : mRefCount(1), // create with a refcount of 1 so it is never deleted
    mCameraIndex(-1),
    mWindowHandle(NULL),
    mDoneEvent(INVALID_HANDLE_VALUE)
{
  mDoneEvent = ::CreateEventA(nullptr, TRUE, FALSE, nullptr);
  HRSucceedOrThrow hr = ::MFCreateMediaSession(nullptr, mpMediaSession.Assignee());
  hr = mpMediaSession->BeginGetEvent(this, nullptr);
}

WebcamPipeline::~WebcamPipeline()
{
  mpMediaSession->Close();
  WaitForDone();
  mpMediaSession->Shutdown();
  ::CloseHandle(mDoneEvent);
}

std::string WebcamPipeline::Error() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mError;
}

void WebcamPipeline::SetError(const std::string& s)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mError = s;
}

// IUnknown
HRESULT WebcamPipeline::QueryInterface(const IID& inIID, void** outInterface)
{
  *outInterface = nullptr;
  if (inIID == IID_IMFMediaSession)
    *outInterface = mpMediaSession;
  else if (inIID == IID_IMFAsyncCallback)
    *outInterface = static_cast<IMFAsyncCallback*>(this);
  else if (inIID == IID_IUnknown)
    *outInterface = static_cast<IUnknown*>(this);
  if (*outInterface)
    static_cast<IUnknown*>(*outInterface)->AddRef();
  return *outInterface ? S_OK : E_NOINTERFACE;
}

ULONG WebcamPipeline::AddRef()
{
  return ::InterlockedIncrement(&mRefCount);
}

ULONG WebcamPipeline::Release()
{
  ULONG refCount = ::InterlockedDecrement(&mRefCount);
  if (refCount == 0)
    delete this;
  return refCount;
}

// IMFAsyncCallback
HRESULT WebcamPipeline::GetParameters(DWORD* pdwFlags, DWORD* pdwQueue)
{
  *pdwFlags |= MFASYNC_FAST_IO_PROCESSING_CALLBACK;
  *pdwQueue = MFASYNC_CALLBACK_QUEUE_STANDARD;
  return S_OK;
}

HRESULT WebcamPipeline::Invoke(IMFAsyncResult* pAsyncResult)
{
  com::Ptr<IMFMediaEvent> pEvent;
  if (!mpMediaSession)
    return S_OK;
  HRESULT hr = mpMediaSession->EndGetEvent(pAsyncResult, pEvent.Assignee());
  MediaEventType eventType;
  hr = pEvent->GetType(&eventType);
  try {
    OnEvent(pEvent, eventType);
  }
  catch(const std::exception& e) {
    SetError(e.what());
    std::string s = e.what();
    s += "\n";
    ::OutputDebugStringA(s.c_str());
    ::SetEvent(mDoneEvent);
  }
  if (eventType != MESessionClosed && eventType != MEEndOfPresentation)
    hr = mpMediaSession->BeginGetEvent(this, nullptr);
  return hr;
}


void WebcamPipeline::OnEvent(IMFMediaEvent* pEvent, MediaEventType eventType)
{
  HRESULT eventStatus = S_OK;
  HRSucceedOrThrow hr = pEvent->GetStatus(&eventStatus);
  if (FAILED(eventStatus))
    OnMediaError(eventStatus);
  else
    OnMediaEvent(pEvent, eventType);
}

void WebcamPipeline::OnMediaEvent(IMFMediaEvent* pEvent, MediaEventType type)
{
  HRSucceedOrThrow hr;
  UINT32 status = 0;
  switch (type)
  {
  case MESessionTopologyStatus:
    // Raised by the Media Session when the status of a topology changes. 
    // Get the topology changed status code. This is an enum in the event
    hr = pEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, &status);
    // the one we are most interested in is i == MFTopoStatus.Ready
    // which we get then the Topology is built and ready to run
    OnTopologyStatusChanged(MF_TOPOSTATUS(status));
    ::SetEvent(mDoneEvent);
    break;

  case MESessionStarted:
    // Raised when the IMFMediaSession::Start method completes asynchronously. 
    ::SetEvent(mDoneEvent);
    break;

  case MESessionPaused:
    // Raised when the IMFMediaSession::Pause method completes asynchronously. 
    ::SetEvent(mDoneEvent);
    break;

  case MESessionStopped:
    // Raised when the IMFMediaSession::Stop method completes asynchronously.
    ::SetEvent(mDoneEvent);
    break;

  case MESessionClosed:
    // Raised when the IMFMediaSession::Close method completes asynchronously. 
    ::SetEvent(mDoneEvent);
    break;

  case MESessionCapabilitiesChanged:
    // Raised by the Media Session when the session capabilities change.
    // You can use IMFMediaEvent::GetValue to figure out what they are
    break;

  case MESessionTopologySet:
    // Raised after the IMFMediaSession::SetTopology method completes asynchronously. 
    // The Media Session raises this event after it resolves the topology into a full topology and queues the topology for playback. 
    break;

  case MESessionNotifyPresentationTime:
    // Raised by the Media Session when a new presentation starts. 
    // This event indicates when the presentation will start and the offset between the presentation time and the source time.      
    break;

  case MEEndOfPresentation:
    // Raised by a media source when a presentation ends. This event signals that all streams 
    // in the presentation are complete. The Media Session forwards this event to the application.
    break;

  case MESessionRateChanged:
    // Raised by the Media Session when the playback rate changes. This event is sent after the 
    // IMFRateControl::SetRate method completes asynchronously. 
    break;

  default:
    break;
  }
}

void WebcamPipeline::OnMediaError(HRESULT hr)
{
  HRSucceedOrThrow h = hr;
}

void WebcamPipeline::OnTopologyStatusChanged(MF_TOPOSTATUS status)
{
  HRSucceedOrThrow hr;
  switch (status) {
  case MF_TOPOSTATUS_READY:
    hr = ::MFGetService(mpMediaSession, MR_VIDEO_RENDER_SERVICE,
      IID_IMFVideoDisplayControl, mpVideoDisplayControl.Assignee());
  }
}

void WebcamPipeline::SetDesiredVideoMediaType(const VideoMediaType& type)
{
  mDesiredVideoMediaType = type;
}

const VideoMediaType& WebcamPipeline::ActualMediaType() const
{
  return mActualVideoMediaType;
}

void WebcamPipeline::SetCameraIndex(int inCameraIndex)
{
  com::Ptr<IMFAttributes> pConfig;
  // Create an attribute store to hold the search criteria.
  HRSucceedOrThrow hr = ::MFCreateAttributes(pConfig.Assignee(), 1);
  // Request video capture devices.
  hr = pConfig->SetGUID(
    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
  );

  // Enumerate devices
  DeviceList devices(pConfig);

  // Create a media source for the desired device in the list.
  if (inCameraIndex >= devices.size())
    throw std::runtime_error("No camera available with index " + std::to_string(inCameraIndex));

  wchar_t buf[1024];
  uint32_t length = 0;
  hr = devices[inCameraIndex]->GetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, buf, sizeof(buf) - 1, &length);
  buf[length] = 0;
  mCameraName = com::DualString(buf);

  mCameraIndex = inCameraIndex;
  hr = devices[inCameraIndex]->ActivateObject(IID_IMFMediaSource, mpSource.Assignee());
}

int WebcamPipeline::CameraIndex() const
{
  return mCameraIndex;
}

const std::string& WebcamPipeline::CameraName() const
{
  return mCameraName;
}

void WebcamPipeline::SetTransform(com::Ptr<IMFTransform> pTransform)
{
  mpTransform = pTransform;
}

com::Ptr<IMFTransform> WebcamPipeline::Transform() const
{
  return mpTransform;
}

void WebcamPipeline::SetWindowHandle(HWND h)
{
  HRSucceedOrThrow hr;
  if (mWindowHandle != h) {
    mWindowHandle = h;
    if (h == NULL)
      mpRendererActivate = nullptr;
    else
      hr = ::MFCreateVideoRendererActivate(h, mpRendererActivate.Assignee());
  }
}

HWND WebcamPipeline::WindowHandle() const
{
  return mWindowHandle;
}

void WebcamPipeline::CreateTopology()
{
  if (!mpSource)
    throw std_runtime_error << "No source given";
  if (!mpRendererActivate)
    throw std_runtime_error << "No renderer defined";

  // Set up video source
  com::Ptr<IMFPresentationDescriptor> pPresentationDescriptor;
  com::Ptr<IMFStreamDescriptor> pVideoStreamDescriptor;
  GetVideoInputDescriptors(pVideoStreamDescriptor, pPresentationDescriptor);
  if (!pVideoStreamDescriptor)
    throw std_runtime_error << "Could not find selected video stream on camera " << mCameraIndex;

  HRSucceedOrThrow hr;
  com::Ptr<IMFMediaTypeHandler> pMediaTypeHandler;
  hr = pVideoStreamDescriptor->GetMediaTypeHandler(pMediaTypeHandler.Assignee());
  ChooseVideoType(pMediaTypeHandler);

  std::vector<com::Ptr<IMFTopologyNode>> nodes;
  com::Ptr<IMFTopologyNode> pNode;
  hr = ::MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, pNode.Assignee());
  hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, mpSource);
  hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPresentationDescriptor);
  hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pVideoStreamDescriptor);
  nodes.push_back(pNode);

  if (mpTransform) {
    hr = ::MFCreateTopologyNode(MF_TOPOLOGY_TRANSFORM_NODE, pNode.Assignee());
    hr = pNode->SetObject(mpTransform);
    nodes.push_back(pNode);
  }

  hr = ::MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, pNode.Assignee());
  hr = pNode->SetObject(mpRendererActivate);
  nodes.push_back(pNode);

  hr = ::MFCreateTopology(mpTopology.Assignee());
  for (auto& node : nodes)
    mpTopology->AddNode(node);

  for (int i = 0; i < nodes.size() - 1; ++i)
    nodes[i]->ConnectOutput(0, nodes[i + 1], 0);
}

com::Ptr<IMFVideoDisplayControl> WebcamPipeline::ResolveTopology()
{
  if (!mpTopology)
    throw std_runtime_error << "No topology given";
  // Setting the topology will result in a MESessionTopologySet event.
  // There, we get a pointer to the EVR control object.
  HRSucceedOrThrow hr = mpMediaSession->SetTopology(0, mpTopology);
  WaitForDone();
  mpTopology = nullptr;
  return mpVideoDisplayControl;
}

void WebcamPipeline::Start()
{
  PROPVARIANT pos = { 0 };
  HRSucceedOrThrow hr = mpMediaSession->Start(nullptr, &pos);
  WaitForDone();
}

void WebcamPipeline::Pause()
{
  HRSucceedOrThrow hr = mpMediaSession->Pause();
  WaitForDone();
}

void WebcamPipeline::Stop()
{
  HRSucceedOrThrow hr = mpMediaSession->Stop();
  WaitForDone();
}

void WebcamPipeline::WaitForDone()
{
  if (WAIT_OBJECT_0 != ::WaitForSingleObject(mDoneEvent, INFINITE))
    throw std_runtime_error << "WaitForSingleObject: " << SysError().Message();
  if (!::ResetEvent(mDoneEvent))
    throw std_runtime_error << "ResetEvent: " << SysError().Message();
}

void WebcamPipeline::GetVideoInputDescriptors(com::Ptr<IMFStreamDescriptor>& outVideoStreamDescriptor, com::Ptr<IMFPresentationDescriptor>& outPresentationDescriptor)
{
  // Get the first enabled video stream from the input device.
  HRSucceedOrThrow hr;
  hr = mpSource->CreatePresentationDescriptor(outPresentationDescriptor.Assignee());
  DWORD streamDescriptorCount = 0;
  hr = outPresentationDescriptor->GetStreamDescriptorCount(&streamDescriptorCount);
  for (int i = 0; i < streamDescriptorCount; ++i) {
    com::Ptr<IMFStreamDescriptor> pStreamDescriptor;
    BOOL selected = FALSE;
    hr = outPresentationDescriptor->GetStreamDescriptorByIndex(i, &selected, pStreamDescriptor.Assignee());
    com::Ptr<IMFMediaTypeHandler> pMediaTypeHandler;
    hr = pStreamDescriptor->GetMediaTypeHandler(pMediaTypeHandler.Assignee());
    GUID majorType;
    pMediaTypeHandler->GetMajorType(&majorType);
    if (majorType == MFMediaType_Video && selected) {
      outVideoStreamDescriptor = pStreamDescriptor;
      break;
    }
  }
}

void WebcamPipeline::ChooseVideoType(IMFMediaTypeHandler* pHandler)
{
  HRSucceedOrThrow hr;
  DWORD count = 0;
  hr = pHandler->GetMediaTypeCount(&count);
  std::vector<VideoMediaType> formats;
  for (int i = 0; i < count; ++i) {
    com::Ptr<IMFMediaType> pMediaType;
    hr = pHandler->GetMediaTypeByIndex(i, pMediaType.Assignee());
    formats.push_back(pMediaType);
  }
  auto j = std::min_element(formats.begin(), formats.end(),
    [this](const VideoMediaType& a, const VideoMediaType& b)
    {
      return a.sqDist(mDesiredVideoMediaType) < b.sqDist(mDesiredVideoMediaType);
    }
  );
  if (j == formats.end())
    throw std_runtime_error << "No suitable MediaType found";
  hr = pHandler->SetCurrentMediaType(j->mpType);
  mActualVideoMediaType = *j;
}
