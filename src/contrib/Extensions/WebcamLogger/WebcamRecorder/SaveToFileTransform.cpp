////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A GrabberTransform descendant that saves grabbed content to a 
//   H264 encoded video file, with optional time stamp.
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
#include "SaveToFileTransform.h"
#include "VideoMediaType.h"
#include "ComStrings.h"
#include "Debugging.h"
#include "Errors.h"
#include "FileUtils.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <codecapi.h>

namespace {
  const int cFontSize = 32;

  time_t Now()
  {
    return ::time(0);
  }

  std::string TimeToString(time_t inTime)
  {
    struct ::tm* p = ::localtime(&inTime);
    if (!p)
      return "<n/a>";
    char buf[32];
    ::strftime(buf, sizeof(buf), "%d %b %y %T", p);
    return buf;
  }
}

SaveToFileTransform::SaveToFileTransform()
  : mSampleCount(0), mDecimation(1),
  mTimestampMode(TimestampNone),
  mOverlayFont(NULL), mOffscreenBuffer(NULL)
{
  CreateOverlayFont();
}

SaveToFileTransform::~SaveToFileTransform()
{
  DeleteOffscreenBuffer();
  DeleteOverlayFont();
}

void SaveToFileTransform::SetTimestampMode(int mode)
{
  mTimestampMode = mode;
}

int SaveToFileTransform::TimestampMode() const
{
  return mTimestampMode;
}

void SaveToFileTransform::SetDecimation(int decimation)
{
  if (decimation < 1)
    decimation = 1;
  mDecimation = decimation;
}

int SaveToFileTransform::Decimation() const
{
  return mDecimation;
}

HRESULT SaveToFileTransform::SetInputType(
  DWORD dwInputStreamID,
  IMFMediaType* pType,
  DWORD dwFlags)
{
  HRESULT hr = GrabberTransform::SetInputType(dwInputStreamID, pType, dwFlags);
  if (hr == S_OK) {
    mInputType = VideoMediaType(pType);
    CreateOffscreenBuffer();
  }
  return hr;
}

bool SaveToFileTransform::StartRecording(const std::string& filename)
{
  std::string fullfile = FileUtils::StripExtension(filename) + ".mp4";
  if (FileUtils::Exists(fullfile)) {
    std::cout << "Output file \"" << fullfile << "\" exists, will not be touched." << std::endl;
    return false;
  }
  if (!FileUtils::IsWritableLocation(fullfile)) {
    std::cout << "Output file \"" << fullfile << "\" cannot be created at the given location." << std::endl;
    return false;
  }

  com::Ptr<IMFAttributes> pAttr;
  HRSucceedOrThrow hr = ::MFCreateAttributes(pAttr.Assignee(), 4);
  hr = pAttr->SetUINT32(MF_SINK_WRITER_DISABLE_THROTTLING, 1);
  hr = pAttr->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, 0);
  hr = pAttr->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);
  hr = pAttr->SetGUID(MF_TRANSCODE_CONTAINERTYPE, MFTranscodeContainerType_MPEG4);
  // Note that MFTranscodeContainerType_AVI is broken:
  // https://docs.microsoft.com/en-us/answers/questions/114418/mftranscodecontainertype-avi-not-creating-index.html

  hr = ::MFCreateSinkWriterFromURL(com::DualString(fullfile), nullptr, nullptr, mpSinkWriter.Assignee());

  com::Ptr<IMFMediaType> pH264Type = mInputType.Strip();
  hr = pH264Type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
  hr = pH264Type->SetUINT32(MF_MT_MPEG2_PROFILE, eAVEncH264VProfile_Main);
  UINT32 bitrate = 0.14 * mInputType.width * mInputType.height * mInputType.framerateN / mInputType.framerateD;
  hr = pH264Type->SetUINT32(MF_MT_AVG_BITRATE, bitrate);
  hr = mpSinkWriter->AddStream(pH264Type, &mVideoStreamID);
  hr = mpSinkWriter->SetInputMediaType(mVideoStreamID, mInputType.Strip(), nullptr);

  switch (mState) {
  case Idle:
    mState = StartRecordingPending;
    return true;
  default:
    ;
  }
  return false;
}

bool SaveToFileTransform::StopRecording()
{
  switch (mState) {
  case Recording:
    mState = StopRecordingPending;
    return true;
  default:
    ;
  }
  return false;
}

HRESULT SaveToFileTransform::OnProcess(IMFSample* pSample)
{
  try {
    AddTimestamp(pSample);

    switch (mState.load()) {
    case Idle:
      break;
    case StartRecordingPending:
      mState = Recording;
      OnStartRecording(pSample);
      OnRecord(pSample);
      break;
    case Recording:
      OnRecord(pSample);
      break;
    case StopRecordingPending:
      OnRecord(pSample);
      mState = Idle;
      OnStopRecording(pSample);
      break;
    default:
      Assert(false);
    }
  } catch(const std::exception& e) {
    std::string what = e.what();
    std::cout << what << std::endl;
    ::OutputDebugStringA((what + "\n").c_str());
  }
  return S_OK;
}

void SaveToFileTransform::OnStartRecording(IMFSample* pSample)
{
  mSampleCount = 0;
  std::lock_guard<std::mutex> lock(mWriterMutex);
  HRSucceedOrThrow hr = mpSinkWriter->BeginWriting();
}

void SaveToFileTransform::OnRecord(IMFSample* pSample)
{
  if (mSampleCount % mDecimation) {
    ++mSampleCount;
    return;
  }

  std::clog << mSampleCount++ / mDecimation << std::endl;

  std::lock_guard<std::mutex> lock(mWriterMutex);

  com::Ptr<IMFSample> pDuplicatedSample;
  HRSucceedOrThrow hr = ::MFCreateSample(pDuplicatedSample.Assignee());

  com::Ptr<IMFAttributes> pSourceAttr, pDestAttr;
  hr = pSample->QueryInterface(IID_IMFAttributes, pSourceAttr.Assignee());
  hr = pDuplicatedSample->QueryInterface(IID_IMFAttributes, pDestAttr.Assignee());
  hr = pSourceAttr->CopyAllItems(pDestAttr);

  LONGLONG duration = 0;
  hr = pSample->GetSampleDuration(&duration);
  hr = pDuplicatedSample->SetSampleDuration(duration);
  LONGLONG time = 0;
  hr = pSample->GetSampleTime(&time);
  if (mSampleCount == 1) {
    pDuplicatedSample->SetUINT32(MFSampleExtension_Discontinuity, TRUE);
    mFirstSampleTime = time;
  }
  hr = pDuplicatedSample->SetSampleTime(time - mFirstSampleTime);
  DWORD flags = 0;
  hr = pSample->GetSampleFlags(&flags);
  hr = pDuplicatedSample->SetSampleFlags(flags);
  DWORD count = 0;
  hr = pSample->GetBufferCount(&count);
  for (int i = 0; i < count; ++i) {
    com::Ptr<IMFMediaBuffer> pBuffer;
    hr = pSample->GetBufferByIndex(i, pBuffer.Assignee());

    INT32 frameStride = 0;
    hr = mInputType.mpType->GetUINT32(MF_MT_DEFAULT_STRIDE, reinterpret_cast<UINT32*>(&frameStride));
    if (frameStride < 0) // No flip necessary.
    {
        hr = pDuplicatedSample->AddBuffer(pBuffer);
    }
    else // Flip vertically to match expected input of H.264 codec.
    {
        BYTE* pData = nullptr;
        DWORD currentLength = 0;
        pBuffer->Lock(&pData, nullptr, &currentLength);
        com::Ptr<IMFMediaBuffer> pDuplicatedBuffer;
        hr = ::MFCreateMemoryBuffer(currentLength, pDuplicatedBuffer.Assignee());
        hr = pDuplicatedBuffer->SetCurrentLength(currentLength);
        BYTE* pDuplicatedData = nullptr;
        hr = pDuplicatedBuffer->Lock(&pDuplicatedData, nullptr, &currentLength);
        UINT32 width = 0, height = 0;
        hr = ::MFGetAttributeSize(mInputType.mpType, MF_MT_FRAME_SIZE, &width, &height);
        hr = ::MFCopyImage(pDuplicatedData + frameStride * (height - 1), -frameStride,
          pData, frameStride, width * sizeof(RGBQUAD), height);
        hr = pDuplicatedBuffer->Unlock();
        hr = pBuffer->Unlock();
        hr = pDuplicatedSample->AddBuffer(pDuplicatedBuffer);
    }
  }
  hr = mpSinkWriter->WriteSample(0, pDuplicatedSample);
}

void SaveToFileTransform::OnStopRecording(IMFSample*)
{
  std::clog << "end" << std::endl;
  std::lock_guard<std::mutex> lock(mWriterMutex);
  HRSucceedOrThrow hr;// = mpSinkWriter->Flush(0); // will block indefinitely
  hr = mpSinkWriter->Finalize();
  mpSinkWriter = nullptr;
}

void SaveToFileTransform::AddTimestamp(IMFSample* pSample)
{
  if (mTimestampMode == TimestampNone)
    return;

  DWORD count = 0;
  HRSucceedOrThrow hr = pSample->GetBufferCount(&count);
  if (count > 0) {
    com::Ptr<IMFMediaBuffer> pBuffer;
    hr = pSample->GetBufferByIndex(0, pBuffer.Assignee());
    BYTE* pData = nullptr;
    DWORD currentLength = 0;
    hr = pBuffer->Lock(&pData, nullptr, &currentLength);

    UINT32 frameStride = 0;
    hr = mInputType.mpType->GetUINT32(MF_MT_DEFAULT_STRIDE, &frameStride);
    int width = mInputType.width, height = mInputType.height;
    int bitmapStride = width * sizeof(RGBQUAD);

    // Copy data from buffer into bitmap. Negative stride accounts for vertical flip.
    // Possible optimization: Only copy area used by time stamp.
    hr = ::MFCopyImage(mpOffscreenBufferMemory + bitmapStride * (height - 1), -bitmapStride, 
      pData, frameStride, width * sizeof(RGBQUAD), height);

    RECT rect;
    rect.top = 0;
    rect.bottom = height;
    rect.left = 0;
    rect.right = width;

    int flags = DT_NOCLIP | DT_SINGLELINE;
    switch (mTimestampMode) {
    case TimestampTopLeft:
      flags |= DT_LEFT | DT_TOP;
      break;
    case TimestampTopRight:
      flags |= DT_RIGHT | DT_TOP;
      break;
    case TimestampBottomLeft:
      flags |= DT_LEFT | DT_BOTTOM;
      break;
    case TimestampBottomRight:
      flags |= DT_RIGHT | DT_BOTTOM;
      break;
    }

    std::string time = TimeToString(Now());

    HDC dc = ::GetDC(NULL);
    HDC memDC = ::CreateCompatibleDC(dc);
    ::ReleaseDC(NULL, dc);
    HGDIOBJ oldBitmap = ::SelectObject(memDC, mOffscreenBuffer);

    ::SelectObject(memDC, mOverlayFont);
    ::SetTextColor(memDC, RGB(0, 255, 0));
    ::SetBkMode(memDC, TRANSPARENT);
    ::DrawTextA(memDC, time.c_str(), -1, &rect, flags);

    ::SelectObject(memDC, oldBitmap);
    ::DeleteDC(memDC);

    // Copy data from bitmap back into buffer.
    hr = ::MFCopyImage(pData, frameStride,
      mpOffscreenBufferMemory + bitmapStride * (height - 1), -bitmapStride, bitmapStride, height);

    hr = pBuffer->Unlock();
  }
}

void SaveToFileTransform::CreateOffscreenBuffer()
{
  DeleteOffscreenBuffer();

  BITMAPV5HEADER bi = { 0 };
  bi.bV5Size = sizeof(BITMAPV5HEADER);
  bi.bV5Width = mInputType.width;
  bi.bV5Height = mInputType.height;
  bi.bV5Planes = 1;
  bi.bV5BitCount = 32;
  bi.bV5Compression = BI_RGB;
  bi.bV5RedMask = 0x00FF0000;
  bi.bV5GreenMask = 0x0000FF00;
  bi.bV5BlueMask = 0x000000FF;
  bi.bV5AlphaMask = 0xFF000000;

  HDC dc = ::GetDC(NULL);
  mOffscreenBuffer = ::CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS,
    reinterpret_cast<void**>(&mpOffscreenBufferMemory), NULL, 0);
  ::ReleaseDC(NULL, dc);
}

void SaveToFileTransform::DeleteOffscreenBuffer()
{
  if (mOffscreenBuffer) {
    ::DeleteObject(mOffscreenBuffer);
    mOffscreenBuffer = NULL;
    mpOffscreenBufferMemory = nullptr;
  }
}

void SaveToFileTransform::CreateOverlayFont()
{
  DeleteOverlayFont();
  mOverlayFont = ::CreateFontA(cFontSize, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE,
    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    FIXED_PITCH | FF_MODERN, nullptr);
}

void SaveToFileTransform::DeleteOverlayFont()
{
  if (mOverlayFont) {
    ::DeleteObject(mOverlayFont);
    mOverlayFont = NULL;
  }
}
