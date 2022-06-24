/////////////////////////////////////////////////////////////////////////////
// $Id: AudioExtension.h 6725 2022-05-06 14:08:30Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The audio extension enables input and mixed output from/to 
//   sound cards on the machine. The extension also handles filtering of
//   audio input/output envelope to states
//
// Documentation located at: 
//   http://www.bci2000.org/wiki/index.php/Contributions:AudioExtension
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
#ifndef AUDIO_EXTENSION_H
#define AUDIO_EXTENSION_H

#include <string>
#include <iostream>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <portaudio.h>
#include <sndfile.h>
#include "Environment.h"
#include "Thread.h"
#include "RandomGenerator.h"
#include "Expression/Expression.h"
#include "IIRFilter.h"
#include "WaitableEvent.h"


class AudioExtension : public EnvironmentExtension, public Thread
{
 public:
  //Constructors and virtual interface
  AudioExtension();
  virtual ~AudioExtension();
  virtual void Publish();
  virtual void Preflight() const;
  virtual void Initialize();
  virtual void Process();
  virtual void StartRun();
  virtual void StopRun();
  
  // Inherited from Thread
  virtual int OnExecute() override;

 private:
  typedef std::vector< Expression > ExpressionVector;
  typedef std::vector< ExpressionVector > ExpressionMatrix;
  typedef std::vector< std::pair< std::string, int > > ChannelDef;
  typedef std::vector< IIRFilter< FilterDesign::Real > > FilterBank;
  ExpressionMatrix mMixer;
  std::vector< std::vector< float > > mGainMatrix;
  
  // Internal configuration methods
  bool GetHostApiConfig( PaHostApiTypeId hostApi, const PaHostApiInfo* &apiInfo, PaHostApiIndex &hostIdx ) const;
  void GetDeviceConfig( const PaHostApiInfo* apiInfo, PaHostApiIndex hostIdx, 
    int &inputIdx, int &outputIdx, int &inputChannels, int &outputChannels ) const;
  void LoadMixer( const ParamRef &matrix, ExpressionMatrix &mixer,
    ChannelDef &channelDef, int audioIns, int audioOuts, int fileIns ) const;
  void EvaluateMixer( ExpressionMatrix &mixer, std::vector< std::vector< float > > &gainMatrix ) const;
  void DesignFilterbank( const ParamRef &matrix, IIRFilter< FilterDesign::Real > &filter, size_t numCh ) const;


  // Static audio callback
  static int AudioCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void *userData );
  
  // Member variables
  std::set< int >                    mSupportedAPIs;
  int                                mHostAPI;
  
  WaitableEvent                      mBufferProcessed;
  GenericSignal                      mAudioInputBlock,
                                     mAudioOutputBlock,
                                     mAudioInputEnvelope,
                                     mAudioOutputEnvelope;
  
  unsigned int                       mFrameCount,
                                     mFramesPerBuffer,
                                     mMaxInputEnvelopes,
                                     mMaxOutputEnvelopes;
  
  int                                mInputChannels,
                                     mOutputChannels,
                                     mFileChannels;
  
  ChannelDef                         mChannelDef;
  RandomGenerator                    mRand;
  
  PaStream                          *mpAudioStream;
	PaError														pa_error;
  
  SNDFILE                           *mpAudioInputFile,
                                    *mpAudioRecInputFile,
                                    *mpAudioRecOutputFile;
  
  FilterBank                         mInputEnvelopeFilter,
                                     mOutputEnvelopeFilter;
  IIRFilter< FilterDesign::Real >    mInputFilter,
                                     mOutputFilter;
};

#endif // AUDIO_EXTENSION_H
