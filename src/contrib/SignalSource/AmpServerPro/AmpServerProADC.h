////////////////////////////////////////////////////////////////////////////////
// $Id: AmpServerProADC.h 6801 2022-06-22 21:37:37Z wengelhardt $
// Description: A source module that interfaces withEGI's Amp Server Pro SDK.
// Authors: Joshua Fialkoff, Robert Bell (EGI),
//          juergen.mellinger@uni-tuebingen.de
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
#ifndef AmpServerProADCH
#define AmpServerProADCH

#include <sstream>
#include "BufferedADC.h"
#include "Expression/Expression.h"
#include "SockStream.h"
#include "NetworkPacketTypes.h"
#include <Win32Defs.h>
#include <tinyxml2.h>

class AmpServerProADC : public BufferedADC
{
 public:
  AmpServerProADC();
  ~AmpServerProADC();

 protected:
  void Publish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& ) const override;
  void OnInitialize( const SignalProperties& ) override;
  void OnStartAcquisition() override;
  void OnStopAcquisition() override;
  void OnProcess () override;
  void DoAcquire( GenericSignal& ) override;
  void AmpServerProStimThread();
  
 private:
   bool        mEnableStim,
               mStateReset;
   int         mAmpState;
   Expression  mStimExpression;
   Expression  mAbortExpression;

   //stimulation thread
   std::thread				                    m_acquire_thread;
   std::atomic_bool                      m_kill_acquire_thread;
   int                    m_stimState;
   HANDLE                 m_sync_lock;
   int m_blockTime;
   bool mChangeStimPlan;

   class Connection 
  {
    
   public:
    Connection();
    ~Connection();

    const std::string& Error() const { return mError; }
    bool Open( const std::string& inIP, int inPort, int inAmpId);
    void Close();

    bool IsOpen(int ampID) const;
    const std::string& Address() const { return mAddress; }
    int NotificationPort() const       { return mNotificationPort; }
    int StreamPort() const             { return mStreamPort; }

    int Amps() const                { return mAmps; }
    int Channels() const            { return mChannels; }
    int PacketType() const          { return mPacketType; }
    double Gain_muV() const         { return mGain_muV; }
    double SamplingRate() const     { return mSamplingRate; }
    bool StimulationEnabled() const { return mStimulationEnabled; }

    
    bool EnableStimulation (const std::string& file);
    bool BeginStimulation ();
    bool EndStimulation ();
    bool SendCommand( const std::string&, std::string& resultName = std::string(), std::string& resultValue = std::string() );
    bool StartStreaming();
    bool StopStreaming();
    bool ReadData( GenericSignal& );
    bool LoadGTENPattern (const std::string& name);

  private:
    std::string BuildCommand( const std::string& ) const;
    bool        ParseAmpDetails();
    bool        LoadXMLPattern(const std::string& file);
    bool        DoRead1(GenericSignal&);
    bool        DoRead2(GenericSignal&);
    bool        DoRead(GenericSignal&);


    std::string mError;

    int64_t     mSamplesInStream;
    int         mSamplesInOutput;
    bool        mFirstRead,
                mStimulationEnabled,
                mUseNotification, 
                mBlocksUploaded;
    int         mTimeout, 
                mInitialTimeout;
    std::string mAddress,
                mAmpType,
                mBlocks,
                mWaveforms;
    int         mNotificationPort,
                mStreamPort,
                mAmpId,
                mAmps,
                mChannels,
                mPacketType;
    double      mGain_muV,
                mSamplingRate;
    sockstream  mCommands,
                mStream,
                mNotifications;
    client_tcpsocket mCommandSocket, 
                     mStreamSocket, 
                     mNotificationSocket;
    std::istringstream mStringstream;
    tinyxml2::XMLDocument mStimPlanXML;
  } mConnection;
};

#endif // AmpServerProADCH

