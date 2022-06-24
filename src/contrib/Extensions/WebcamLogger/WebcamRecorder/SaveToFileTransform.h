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
#ifndef SAVE_TO_FILE_TRANSFORM_H
#define SAVE_TO_FILE_TRANSFORM_H

#include <mfidl.h>
#include <mfreadwrite.h>
#include <string>
#include <mutex>

#include "GrabberTransform.h"
#include "VideoMediaType.h"
#include "ComPtr.h"

class SaveToFileTransform : public GrabberTransform
{
public:
  SaveToFileTransform();
  ~SaveToFileTransform();

  enum {
    TimestampNone,
    TimestampTopRight,
    TimestampTopLeft,
    TimestampBottomRight,
    TimestampBottomLeft,
  };
  void SetTimestampMode(int);
  int TimestampMode() const;

  void SetDecimation(int);
  int Decimation() const;

  bool StartRecording(const std::string&);
  bool StopRecording();

protected:
  HRESULT __stdcall SetInputType(
    DWORD dwInputStreamID,
    IMFMediaType* pType,
    DWORD dwFlags) override;

  HRESULT OnProcess(IMFSample* pSample) override;
  void AddTimestamp(IMFSample*);
  void OnStartRecording(IMFSample*);
  void OnRecord(IMFSample*);
  void OnStopRecording(IMFSample*);

private:
  void CreateOffscreenBuffer();
  void DeleteOffscreenBuffer();
  void CreateOverlayFont();
  void DeleteOverlayFont();

private:
  uint64_t mSampleCount;

  enum {
    Idle = 0,
    StartRecordingPending,
    Recording,
    StopRecordingPending,
  };
  std::atomic<int> mState;
  std::atomic<int> mTimestampMode;
  std::atomic<int> mDecimation;

  std::mutex mWriterMutex;
  com::Ptr<IMFSinkWriter> mpSinkWriter;
  DWORD mVideoStreamID;
  LONGLONG mFirstSampleTime;
  VideoMediaType mInputType;
  HFONT mOverlayFont;
  HGDIOBJ mOffscreenBuffer;
  BYTE* mpOffscreenBufferMemory;
};

#endif // SAVE_TO_FILE_TRANSFORM_H
