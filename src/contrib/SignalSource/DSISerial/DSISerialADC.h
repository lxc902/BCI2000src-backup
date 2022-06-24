////////////////////////////////////////////////////////////////////////////////
// $Id: DSISerialADC.h 6484 2022-01-03 16:59:03Z mellinger $
// Authors:     Jeremy Hill <jezhill@gmail.com>
// Description: DSISerialADC class header file.
//              Acquires data from Dry Sensor Interface (DSI) headsets by
//              Wearable Sensing LLC / Quasar USA.  Tested with Wearable Sensing
//              DSI7 and DSI24.  May also work with the ANI-SI headset by
//              Advanced Neurometrics, Inc. The dynamic library from DSI_API is
//              used to make a direct serial-port connection to the headset (as
//              distinct from the TCP/IP streaming approach via the DSIStreamer
//              application). For best results, set the DSISerialPort
//              environment variable, or use the --DSISerialPort command-line
//              parameter on launch: this allows auto-configuration of most of
//              the necessary BCI2000 parameters using data downloaded from the
//              headset.
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
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_DSISerialADC_H  // makes sure this header is not included more than once
#define INCLUDED_DSISerialADC_H

#include "BufferedADC.h"
#include "Thread.h"
#include "ThreadUtils.h" // for SleepFor()
#include "DSI.h"

class AcquisitionThread: public Thread
{
 public:
  AcquisitionThread( DSI_Headset headset ) { mHeadset = headset; }
 private:
  DSI_Headset mHeadset;
  virtual int OnExecute() override;
};

class DSISerialADC : public BufferedADC
{

 public:
           DSISerialADC();
  virtual ~DSISerialADC();

protected:
  void Publish () override;
  virtual void OnHalt();
  virtual void OnPreflight( SignalProperties& Output ) const;
  virtual void OnInitialize( const SignalProperties& Output );
  virtual void OnStartAcquisition();
  virtual void DoAcquire( GenericSignal& Output );
  virtual void OnStopAcquisition();
  virtual void AutoConfig( const SignalProperties & );
  virtual void Halt() { OnStopAcquisition(); OnHalt(); }
  virtual void Preflight(  const SignalProperties & Input, SignalProperties & Output ) const { OnPreflight( Output ); }
  virtual void Initialize( const SignalProperties & Input, const SignalProperties & Output ) { OnInitialize( Output ); OnStartAcquisition(); }
  virtual void Process( const GenericSignal & Input, GenericSignal & Output ) { DoAcquire( Output ); }

 private:
  mutable DSI_Headset  mHeadset;
  AcquisitionThread *  mThread;
  size_t               mOverflow;
  size_t               mBlock;
  size_t               mBlockZero;
  double               mPrecisionTimeZero;
  size_t               mSamplesPerBlock;
  double               mNominalSecondsPerBlock;
  double               mNominalSamplesPerSecond;
  double               mEstimatedSamplesPerSecond;
  double               mPendingSamplesPerSecond;
  double               mPreviousBufferingError;
  double               mCumulativeBufferingError;
  size_t               mBufferAhead;
  double               mRemainder;
  int                  mBatteryLevel1;
  int                  mBatteryLevel2;
  size_t               mBlocksSinceBatteryChecked;
  size_t               mBufferingController_PeriodInBlocks;
  double               mBufferingController_Smoothing;
  double               mBufferingController_Gain;
  double               mBufferingController_P;
  double               mBufferingController_I;
  double               mBufferingController_D;
 
  GenericSignal *        mImpedances;
  GenericVisualization * mVis;
  std::string            Join( const char * paramName ) const;
};

#endif // INCLUDED_DSISerialADC_H
