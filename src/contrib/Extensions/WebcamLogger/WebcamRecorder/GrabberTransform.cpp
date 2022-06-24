////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.com
// Description: A base class for Windows Media Foundation transforms that allow 
//   grabbing content as it flows through the pipeline.
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
#include "GrabberTransform.h"
#include "VideoMediaType.h"

#include <mfapi.h>
#include <Mferror.h>

#include "Errors.h"

namespace {
  IMFMediaType* CloneMediaType(IMFMediaType* pType)
  {
    IMFMediaType* pCopy;
    HRSucceedOrThrow hr = ::MFCreateMediaType(&pCopy);
    hr = pType->CopyAllItems(pCopy);
    return pCopy;
  }
}

GrabberTransform::GrabberTransform()
  : mRefCount(1)
{
}

GrabberTransform::~GrabberTransform()
{
}

void GrabberTransform::AddInputAvailableType(IMFMediaType* pType)
{
  std::lock_guard<std::mutex> lock(mMutex);
  mInputAvailableTypes.push_back(pType);
}

// IUnknown
HRESULT GrabberTransform::QueryInterface(const IID& inIID, void** outInterface)
{
  *outInterface = nullptr;
  if (inIID == IID_IMFTransform)
    *outInterface = static_cast<IMFTransform*>(this);
  else if (inIID == IID_IUnknown)
    *outInterface = static_cast<IUnknown*>(this);
  if (*outInterface)
    this->AddRef();
  return *outInterface ? S_OK : E_NOINTERFACE;
}

ULONG GrabberTransform::AddRef()
{
  return ::InterlockedIncrement(&mRefCount);
}

ULONG GrabberTransform::Release()
{
  ULONG refCount = ::InterlockedDecrement(&mRefCount);
  if (refCount == 0)
    delete this;
  return refCount;
}

// IMFTransform

HRESULT GrabberTransform::GetStreamLimits(
  DWORD* pdwInputMinimum,
  DWORD* pdwInputMaximum,
  DWORD* pdwOutputMinimum,
  DWORD* pdwOutputMaximum)
{
  *pdwInputMinimum = *pdwInputMaximum = 1;
  *pdwOutputMinimum = *pdwOutputMaximum = 1;
  return S_OK;
}

HRESULT GrabberTransform::GetStreamCount(
  DWORD* pcInputStreams,
  DWORD* pcOutputStreams)
{
  *pcInputStreams = *pcOutputStreams = 1;
  return S_OK;
}

HRESULT GrabberTransform::GetStreamIDs(
  DWORD dwInputIDArraySize,
  DWORD* pdwInputIDs,
  DWORD dwOutputIDArraySize,
  DWORD* pdwOutputIDs)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::GetInputStreamInfo(
  DWORD dwInputStreamID,
  MFT_INPUT_STREAM_INFO* pStreamInfo)
{
  if (dwInputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  pStreamInfo->cbSize = 0;
  pStreamInfo->dwFlags = MFT_INPUT_STREAM_PROCESSES_IN_PLACE;
  return S_OK;
}

HRESULT GrabberTransform::GetOutputStreamInfo(
  DWORD dwOutputStreamID,
  MFT_OUTPUT_STREAM_INFO* pStreamInfo)
{
  if (dwOutputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  pStreamInfo->cbSize = 0;
  pStreamInfo->dwFlags = MFT_OUTPUT_STREAM_PROVIDES_SAMPLES;
  return S_OK;
}

HRESULT GrabberTransform::GetAttributes(
  IMFAttributes** pAttributes)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::GetInputStreamAttributes(
  DWORD dwInputStreamID,
  IMFAttributes** pAttributes)
{
  if (dwInputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  return E_NOTIMPL;
}

HRESULT GrabberTransform::GetOutputStreamAttributes(
  DWORD dwOutputStreamID,
  IMFAttributes** pAttributes)
{
  if (dwOutputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  return E_NOTIMPL;
}

HRESULT GrabberTransform::DeleteInputStream(
  DWORD dwStreamID)
{
  if (dwStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  return E_NOTIMPL;
}

HRESULT GrabberTransform::AddInputStreams(
  DWORD cStreams,
  DWORD* adwStreamIDs)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::GetInputAvailableType(
  DWORD dwInputStreamID,
  DWORD dwTypeIndex,
  IMFMediaType** ppType)
{
  if (dwInputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  std::lock_guard<std::mutex> lock(mMutex);
  if (dwTypeIndex < mInputAvailableTypes.size()) {
    *ppType = CloneMediaType(mInputAvailableTypes[dwTypeIndex]);
    return S_OK;
  }
  return MF_E_NO_MORE_TYPES;
}

HRESULT GrabberTransform::GetOutputAvailableType(
  DWORD dwOutputStreamID,
  DWORD dwTypeIndex,
  IMFMediaType** ppType)
{
  if (dwOutputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  if (dwTypeIndex == 0 && mpInputType) {
    *ppType = CloneMediaType(mpInputType);
    return S_OK;
  }
  return MF_E_NO_MORE_TYPES;
}

HRESULT GrabberTransform::SetInputType(
  DWORD dwInputStreamID,
  IMFMediaType* pType,
  DWORD dwFlags)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (mpSample)
    return MF_E_TRANSFORM_CANNOT_CHANGE_MEDIATYPE_WHILE_PROCESSING;
  if (dwInputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;

  bool isCompatible = false;
  for (auto& pAvailableType : mInputAvailableTypes) {
    isCompatible = isCompatible || VideoMediaType(pType).IsCompatible(pAvailableType);
  }
  if (!isCompatible)
    return MF_E_INVALIDMEDIATYPE;

  if (!(dwFlags & MFT_SET_TYPE_TEST_ONLY))
    mpInputType = CloneMediaType(pType);
  return S_OK;
}

HRESULT GrabberTransform::SetOutputType(
  DWORD dwOutputStreamID,
  IMFMediaType* pType,
  DWORD dwFlags)
{
  std::lock_guard<std::mutex> lock(mMutex);
  if (mpSample)
    return MF_E_TRANSFORM_CANNOT_CHANGE_MEDIATYPE_WHILE_PROCESSING;
  if (dwOutputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  if (!(dwFlags & MFT_SET_TYPE_TEST_ONLY))
    mpOutputType = CloneMediaType(pType);
  return S_OK;
}

HRESULT GrabberTransform::GetInputCurrentType(
  DWORD dwInputStreamID,
  IMFMediaType** ppType)
{
  if (dwInputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  std::lock_guard<std::mutex> lock(mMutex);
  if (!mpInputType)
    return MF_E_TRANSFORM_TYPE_NOT_SET;
  *ppType = CloneMediaType(mpInputType);
  return S_OK;
}

HRESULT GrabberTransform::GetOutputCurrentType(
  DWORD dwOutputStreamID,
  IMFMediaType** ppType)
{
  if (dwOutputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  std::lock_guard<std::mutex> lock(mMutex);
  if (!mpOutputType)
    return MF_E_TRANSFORM_TYPE_NOT_SET;
  *ppType = CloneMediaType(mpOutputType);
  return S_OK;
}

HRESULT GrabberTransform::GetInputStatus(
  DWORD dwInputStreamID,
  DWORD* pdwFlags)
{
  if (dwInputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  std::lock_guard<std::mutex> lock(mMutex);
  DWORD flags = 0;
  if (!mpSample)
    flags |= MFT_INPUT_STATUS_ACCEPT_DATA;
  *pdwFlags = flags;
  return S_OK;
}

HRESULT GrabberTransform::GetOutputStatus(
  DWORD* pdwFlags)
{
  std::lock_guard<std::mutex> lock(mMutex);
  DWORD flags = 0;
  if (mpSample)
    flags |= MFT_OUTPUT_STATUS_SAMPLE_READY;
  *pdwFlags = flags;
  return S_OK;
}

HRESULT GrabberTransform::SetOutputBounds(
  LONGLONG hnsLowerBound,
  LONGLONG hnsUpperBound)
{
  return E_NOTIMPL;
}

HRESULT GrabberTransform::ProcessEvent(
  DWORD dwInputStreamID,
  IMFMediaEvent* pEvent)
{
  if (dwInputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  return E_NOTIMPL;
}

HRESULT GrabberTransform::ProcessMessage(
  MFT_MESSAGE_TYPE eMessage,
  ULONG_PTR ulParam)
{
  std::lock_guard<std::mutex> lock(mMutex);
  HRESULT hr = S_OK;
  switch (eMessage) {
  case MFT_MESSAGE_NOTIFY_START_OF_STREAM:
  case MFT_MESSAGE_COMMAND_FLUSH:
    mpSample = nullptr;
    /* fall through */
  case MFT_MESSAGE_COMMAND_DRAIN:
  case MFT_MESSAGE_NOTIFY_END_OF_STREAM:
  case MFT_MESSAGE_NOTIFY_BEGIN_STREAMING:
  case MFT_MESSAGE_NOTIFY_END_STREAMING:
    hr = S_OK;
    break;

  case MFT_MESSAGE_COMMAND_MARKER:
  case MFT_MESSAGE_SET_D3D_MANAGER:
  default:
    hr = E_NOTIMPL;
    break;
  }
  return hr;
}

HRESULT GrabberTransform::ProcessInput(
  DWORD dwInputStreamID,
  IMFSample* pSample,
  DWORD dwFlags)
{
  if (dwInputStreamID != 0)
    return MF_E_INVALIDSTREAMNUMBER;
  if (!pSample)
    return E_POINTER;
  if (dwFlags != 0)
    return E_INVALIDARG;
  std::lock_guard<std::mutex> lock(mMutex);
  if (!mpInputType || !mpOutputType)
    return MF_E_TRANSFORM_TYPE_NOT_SET;
  if (mpSample)
    return MF_E_NOTACCEPTING;
  mpSample = pSample;
  return S_OK;
}

HRESULT GrabberTransform::ProcessOutput(
  DWORD dwFlags,
  DWORD cOutputBufferCount,
  MFT_OUTPUT_DATA_BUFFER* pOutputSamples,
  DWORD* pdwStatus)
{
  if (dwFlags != 0)
    return E_INVALIDARG;
  if (cOutputBufferCount != 1)
    return E_INVALIDARG;
  if (!pOutputSamples)
    return E_POINTER;
  std::lock_guard<std::mutex> lock(mMutex);
  if (!mpInputType || !mpOutputType)
    return MF_E_TRANSFORM_TYPE_NOT_SET;
  if (!mpSample)
    return MF_E_TRANSFORM_NEED_MORE_INPUT;
  pOutputSamples->dwStatus = 0;
  pOutputSamples->pSample = mpSample;
  mpSample->AddRef();
  HRESULT hr = OnProcess(mpSample);
  mpSample = nullptr;
  return hr;
}

