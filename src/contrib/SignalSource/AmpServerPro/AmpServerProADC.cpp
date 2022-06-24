////////////////////////////////////////////////////////////////////////////////
// $Id: AmpServerProADC.cpp 6801 2022-06-22 21:37:37Z wengelhardt $
// Description: A source module that interfaces with EGI's Amp Server Pro SDK.
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
#include "AmpServerProADC.h"

#include "BinaryData.h"
#include "GenericADC.h"
#include "BCIStream.h"

#define bcidbgOff
#define SCALING_FACTOR 0.000155220429


// Size of each packet in bytes
#define SIZEOF_PACKETTYPE_1 1152
#define SIZEOF_PACKETTYPE_2 1264

using namespace std;
using namespace tinyxml2;

RegisterFilter( AmpServerProADC, 1 );

namespace {

class ResponseNode
{
 public:
  typedef list<ResponseNode> List;

  ResponseNode()
    {}
  explicit ResponseNode( istream& is )
    { Parse( is ); }
  const string& Name() const
    { return mName; }
  const string& Value() const
    { return mValue; }
  List Leaves() const;
  istream& Parse( istream& );
  ostream& Print( ostream& ) const;
 private:
  string mName, mValue;
  List mChildren;
};

ResponseNode::List
ResponseNode::Leaves() const
{
  List result;
  if( mChildren.empty() )
    result.push_back( *this );
  else for( List::const_iterator i = mChildren.begin(); i != mChildren.end(); ++i )
  {
    List leaves = i->Leaves();
    result.insert( result.end(), leaves.begin(), leaves.end() );
  }
  return result;
}

istream&
ResponseNode::Parse( istream& is )
{
  if( (is >> ws).peek() != '(' )
    is.setstate( ios::failbit );
  is.ignore();
  is >> mName;
  while( (is >> ws).peek() == '(' )
    mChildren.push_back( ResponseNode( is ) );
  if( mChildren.empty() )
    getline( is, mValue, ')' );
  else
  {
    if( (is >> ws).peek() != ')' )
      is.setstate( ios::failbit );
    is.ignore();
  }
  return is;
}

ostream&
ResponseNode::Print( ostream& os ) const
{
  os << "( " << mName;
  if( !mValue.empty() )
    os << "=" << mValue;
  for( List::const_iterator i = mChildren.begin(); i != mChildren.end(); ++i )
    i->Print( os << " " );
  return os << " )";
}

ostream& operator<<( ostream& os, const ResponseNode& r )
{ return r.Print( os ); }

} // namespace

AmpServerProADC::AmpServerProADC () : 
  mEnableStim ( false ),
  mAmpState( 0 ),
  mStateReset( true ),
  m_kill_acquire_thread ( true ),
  m_stimState ( -1 ),
  mChangeStimPlan ( true )
{
}

AmpServerProADC::~AmpServerProADC()
{
  m_kill_acquire_thread = true;
  SetEvent(m_sync_lock);
  CloseHandle(m_sync_lock);
  m_acquire_thread.join();
  Halt();
}

void
AmpServerProADC::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
  "Source:Signal%20Properties list ChannelNames= 1 auto // list of channel names",
  
  "Source int SourceCh= auto auto % % ",
  "Source list SourceChOffset= 1 auto % % % % ",
  "Source list SourceChGain= 1 auto % % % % ",
  
  "Source:PacketType2 int SampleBlockSize= 40 40 1 % ",
  
  "Source int SamplingRate= auto auto % % ",
  "Source string ServerIP= localhost localhost % % // IP address of amplifier",
  "Source int CommandPort= auto auto 1 65535 // port number corresponding to the command layer",
  "Source int NotificationPort= auto auto 1 65535 // port number corresponding to the notification layer",
  "Source int StreamPort= auto auto 1 65535 // port number corresponding to the stream layer",
  "Source string AmplifierID=  auto auto 0 % // the ID of the Amplifier from which data should be collected",
  "Source int AmpState= 0 0 0 1 // set amp state to: 0 DefaultAcquisitionState, 1 DefaultSignalGeneration (enumeration)",
  

  "Source:GTEN int RunTranscranialStim= 0 0 0 1 // Perform Transcranial Stimulation (boolean)",
  "Source:GTEN string ModulationPlanFile= % // Modulation Plan file made in RVE (inputfile)",
  "Source:GTEN string StimulationExpression= State(StimulusCode)==1 State(StimulusCode)==1 % %  // Expression to stimulate",
  "Source:GTEN string AbortExpression= State(StimulusCode)==0 State(StimulusCode)==0 % %  // Expression to abort stimulation",
  END_PARAMETER_DEFINITIONS

 BEGIN_STREAM_DEFINITIONS
   "EGIDigitalInputStream 16 0",
   "GTENTrainRunning 8 0",
   "GTENStimulationTriggered 1 0",
   "DigitalInput1 1 0",
   "DigitalInput2 1 0",
   "DigitalInput3 1 0",
   "DigitalInput4 1 0",
 END_STREAM_DEFINITIONS
}

void
AmpServerProADC::OnAutoConfig ()
{
  Parameter ("CommandPort")      = 9877;
  Parameter ("NotificationPort") = 9878;
  Parameter ("AmplifierID")      = 0;
  Parameter("SamplingRate")      << 1000 << "Hz";
  Parameter("SampleBlockSize")   = 40;

  int ampID = ActualParameter ("AmplifierID");
  if (!mConnection.IsOpen (ampID))
  {  
    mConnection.Open (
      ActualParameter ("ServerIP"),
      ActualParameter ("CommandPort"),
      ampID
    );
    mConnection.SendCommand ("SetPower 1");
  }

  if( mConnection.IsOpen(ampID) )
  {
    if( mConnection.Amps() < 1 )
      bcierr << "No amplifiers available";
    else if( ampID >= mConnection.Amps() || ampID < 0 )
      bcierr << "Invalid amplifier ID: " << ampID;

    Parameter( "NotificationPort" ) = mConnection.NotificationPort();
    Parameter( "StreamPort" ) = mConnection.StreamPort();

    Parameter( "SourceCh" ) = mConnection.Channels();
    Parameter( "SourceChOffset" )->SetNumValues( mConnection.Channels() );
    Parameter( "ChannelNames" )  ->SetNumValues( mConnection.Channels() );
    Parameter( "SourceChGain" )  ->SetNumValues( mConnection.Channels() );
    for( int ch = 0; ch < mConnection.Channels(); ++ch )
    {
      Parameter( "SourceChOffset" )( ch ) = 0;
      Parameter( "ChannelNames" )( ch ) << "Ch" << (ch+1);
      Parameter( "SourceChGain" )( ch ) << mConnection.Gain_muV() << "muV";
    }
    Parameter( "SamplingRate" ) << mConnection.SamplingRate() << "Hz";
  }
  else
  {
    bcierr << "Could not connect to AmpServerPro at " << mConnection.Address()
           << ", " << mConnection.NotificationPort()
           << ", or " << mConnection.StreamPort();
  }
}

void
AmpServerProADC::OnPreflight( SignalProperties& Output ) const
{
  const Connection& AmpServer = mConnection;
  if (mConnection.PacketType () == 1)
    PreflightCondition( Parameter ( "SampleBlockSize" ) == 40 );
  if ((bool)Parameter ("RunTranscranialStim"))
  {
    if (mConnection.PacketType () == 2)
    {
      tinyxml2::XMLDocument _preflightXML;
      _preflightXML.LoadFile (Parameter ("ModulationPlanFile").c_str ());
      if (_preflightXML.ErrorID () != XML_SUCCESS)
        bciwarn << "Stimulation Error: Unable to open " << Parameter ("ModulationPlanFile") 
                << ". Check that the path to plan.mcf is valid. Stimulation is disabled." << endl;
      else 
        bciout << "Successfully loaded " << Parameter ("ModulationPlanFile") << endl;
      GenericSignal preflightSignals (Output);
      Expression stimExpression = Parameter ("StimulationExpression");
      Expression abortExpression = Parameter ("AbortExpression");
      stimExpression.Compile ();
      abortExpression.Compile ();
      stimExpression.Evaluate (&preflightSignals);
      abortExpression.Evaluate (&preflightSignals);
    }
    else 
      bcierr << "Stimulation not possible on requested device. Device must be a GTEN200 and use packet type 2" << endl;
  }
  PreflightCondition( Parameter( "SourceCh" ) <= AmpServer.Channels() );
  PreflightCondition( Parameter( "SamplingRate" ).InHertz() == AmpServer.SamplingRate() );
  if (Parameter ("ChannelNames")->NumValues () < Parameter ("SourceCh"))
  {
    bcierr << "ChannelNames parameter length (" << Parameter ("ChannelNames")->NumValues () << ") differs from the SourceCh" << endl;
  }
  for( int ch = 0; ch < Parameter( "SourceCh" ); ++ch )
  {
    Output.ChannelLabels()[ch] = Parameter ("ChannelNames")(ch);
    PreflightCondition( Parameter( "SourceChOffset" )( ch ) == 0 );
    double diff = Parameter( "SourceChGain" )( ch ).InMicrovolts() - AmpServer.Gain_muV();
    if( ::fabs( diff ) > 1e-3 )
      bcierr << "Wrong SourceChGain: differs by more than 1/1000";
  }
  Output = SignalProperties (Parameter ("SourceCh") + 6, Parameter ("SampleBlockSize"), SignalType::float32);
  Output.ChannelLabels ()[Parameter ("SourceCh") + 0] = "@EGIDigitalInputStream";
  Output.ChannelLabels ()[Parameter ("SourceCh") + 1] = "@GTENTrainRunning";
  Output.ChannelLabels()[Parameter("SourceCh") + 2] = "@DigitalInput1";
  Output.ChannelLabels()[Parameter("SourceCh") + 3] = "@DigitalInput2";
  Output.ChannelLabels()[Parameter("SourceCh") + 4] = "@DigitalInput3";
  Output.ChannelLabels()[Parameter("SourceCh") + 5] = "@DigitalInput4";


  State("GTENStimulationTriggered");
}

void
AmpServerProADC::OnInitialize( const SignalProperties& )
{
  //mConnection.SendCommand ("SetPower 1");
  mAmpState = Parameter ("AmpState");
  mEnableStim = (bool)Parameter ("RunTranscranialStim");
  if (mEnableStim)
  {
    mStimExpression = (Expression)Parameter ("StimulationExpression");
    mAbortExpression = (Expression)Parameter ("AbortExpression");
    m_blockTime = 1000 * Parameter("SampleBlockSize").ToNumber() / Parameter("SamplingRate").InHertz();
    mStateReset = true;
    mChangeStimPlan = true;
    if (m_kill_acquire_thread)
    {
      m_kill_acquire_thread = false;
      m_sync_lock = CreateEvent(NULL, false, false, NULL);
      m_acquire_thread = thread(&AmpServerProADC::AmpServerProStimThread, this);
    }
    else
    {
      //break thread to update for new config
      m_stimState = -1;
      SetEvent(m_sync_lock);
    }
  }
  
}

void
AmpServerProADC::OnStartAcquisition()
{
  if( !mConnection.SendCommand( "Start" ) )
  {
    bcierr << "Unable to start the amplifier";
    return;
  }
  
  // Always set the amp to the default acquisition state first.
  mConnection.SendCommand( "DefaultAcquisitionState" );
  // Now send the desired start state (if not the already set default acquisition state).
  switch( mAmpState )
  {
    case 0:
      break;
    case 1:
      mConnection.SendCommand( "DefaultSignalGeneration" );
      break;
  }
  if( !mConnection.StartStreaming() )
    bcierr << "Unable to start data streaming";
}

void
AmpServerProADC::OnStopAcquisition()
{
  if( !mConnection.StopStreaming() )
    bcierr << "Unable to stop data streaming";
  else
    mConnection.SendCommand( "Stop" );
}

void
AmpServerProADC::DoAcquire( GenericSignal& Output )
{
  if( !mConnection.ReadData( Output ) )
    throw bcierr << mConnection.Error();
}


void 
AmpServerProADC::OnProcess ()
{
  if (mEnableStim)
  {
    if (State("Running"))
    {
      bool stimRunning = !(((char)State("GTENTrainRunning") >> 2) & 0x1);
      if (mStimExpression.Evaluate() && mStateReset)
      {
        //bciout << "Starting? " << stimRunning << endl;
        if (!stimRunning)
        {
          m_stimState = 1;
          State("GTENStimulationTriggered") = 1;
          mStateReset = false;
          SetEvent(m_sync_lock);
        }
      }
      else if (!mStimExpression.Evaluate() && !mStateReset)
      {
        bciwarn << "Stopping stimulation" << endl;
        mStateReset = true;
        m_stimState = 0;
        State("GTENStimulationTriggered") = 0;
        SetEvent(m_sync_lock);
      }
      else if (mAbortExpression.Evaluate() && State("GTENStimulationTriggered"))
      {
        if (stimRunning)
        {
          bciwarn << "GTEN: Aborting the stimulation" << endl;
          m_stimState = 0;
          State("GTENStimulationTriggered") = 0;
          mStateReset = true;
          SetEvent(m_sync_lock);
        }
      }
      else
        m_stimState = -1;
    }
    else
    {
      if (State("GTENTrainRunning")==1)
      {
        //stop stimulation if not in run
        mConnection.EndStimulation();
        mConnection.LoadGTENPattern("Waveforms");
        mConnection.LoadGTENPattern("Blocks");
      }
    }
  }
}
void
AmpServerProADC::AmpServerProStimThread()
{
  while (!m_kill_acquire_thread)
  {
    if (mChangeStimPlan)
    {
      //will probs want to change this to dependent on if it is connected
      Sleep(1000);
      mEnableStim = mConnection.EnableStimulation(Parameter("ModulationPlanFile"));
      mChangeStimPlan = false;
    }
    DWORD res = WaitForSingleObject(m_sync_lock, 100 * m_blockTime);
    if (res == WAIT_OBJECT_0)
    {
      if (m_stimState == 1)
      {
        mConnection.BeginStimulation();
      }
      else if (m_stimState == 0)
      {
        if (!mConnection.EndStimulation())
        {
          bciwarn << "GTEN: Unable to end Stimulation" << endl;
        }
        mConnection.LoadGTENPattern("Waveforms");
        mConnection.LoadGTENPattern("Blocks");
      }
    }
  }
}

AmpServerProADC::Connection::Connection()
: mSamplesInStream( 0 ),
  mSamplesInOutput( 0 ),
  mFirstRead( false ),
  mNotificationPort( 0 ),
  mStreamPort( 0 ),
  mTimeout( 0 ),
  mInitialTimeout( 0 ),
  mAmpId( 0 ),
  mAmps( 0 ),
  mChannels( 0 ),
  mGain_muV( 0 ),
  mSamplingRate( 0 ),
  mPacketType( 0 ),
  mStimulationEnabled( false ),
  mWaveforms( "" ),
  mBlocks( "" ),
  mUseNotification( false ), 
  mBlocksUploaded( false )
{
}

AmpServerProADC::Connection::~Connection()
{
  Close();
}

bool
AmpServerProADC::Connection::Open( const std::string& inIP, int inPort, int inAmpId)
{
  Close();

  mInitialTimeout = 7000;
  mTimeout = 1000;

  ostringstream oss;
  oss << inIP << ":" << inPort;
  mAddress = oss.str();
  mCommandSocket.open( mAddress );
  mCommands.open( mCommandSocket );

  mNotificationPort = 9878;
  //mNotificationSocket.open( inIP, mNotificationPort );
  //mNotifications.open( mNotificationSocket );

  mStreamPort = 9879;
  mStreamSocket.open( inIP, mStreamPort );
  mStream.open( mStreamSocket );

  mAmps = 0;
  string resultName, resultValue, details;
  if( SendCommand( "NumberOfAmps", resultName, resultValue ) )
    mAmps = ::atoi( resultValue.c_str() );
  mAmpId = inAmpId;
  
  return IsOpen(inAmpId) && ParseAmpDetails();
}

void
AmpServerProADC::Connection::Close()
{
  SendCommand ("SetPower 0");
  mStreamSocket.close();
  mStream.close();
  mStream.clear();
  //mNotificationSocket.close();
  //mNotifications.close();
  //mNotifications.clear();
  mCommandSocket.close();
  mCommands.close();
  mCommands.clear();
}

bool
AmpServerProADC::Connection::IsOpen(int ampID) const
{
  return mCommands.is_open() && mStream.is_open() && ampID == mAmpId;
}

bool 
AmpServerProADC::Connection::EnableStimulation (const std::string& file)
{
  //if (!mStimulationEnabled)
  //{
  bool success = LoadXMLPattern (file);
  if (success)
  {
    // abort the current train, necessary before sending a train
    if (!(mCommands << BuildCommand ("GTENAbortTrain") << endl)) return false;
    if (!mCommandSocket.wait_for_read (mTimeout))                return false;
    ResponseNode parsedcmdResponse (mCommands);
    ResponseNode::List                 leaves = parsedcmdResponse.Leaves ();
    ResponseNode::List::const_iterator i = leaves.begin ();
    success = success && (i->Name () == "status" && i->Value () == "complete");
    success = success && LoadGTENPattern ("Waveforms");
    success = success && LoadGTENPattern ("Blocks");
  }
  //bciout << mBlocks << "\n" << mWaveforms << endl;
  if (!(success))
    return false;
  bciwarn << "Stimulation pattern successfully uploaded" << endl;
  mStimulationEnabled = true;
  return true;
  //}
  //return false;
}

bool
AmpServerProADC::Connection::LoadXMLPattern (const std::string& file)
{
  mStimPlanXML.LoadFile (file.c_str ());
  if (mStimPlanXML.ErrorID () != XML_SUCCESS)  
    return false;

  XMLPrinter streamer(NULL, true);
  XMLElement* ch = NULL;
  XMLElement* trains = NULL;
  XMLElement* train = NULL;
  XMLElement* blocks = NULL;
  XMLElement* waveforms = NULL;
  XMLElement* root = mStimPlanXML.FirstChildElement ("mcf");
  if (root) {
    cout << root->Name () << endl;
    ch = root->FirstChildElement ("channelCount");
    if (ch)
    {
      int nCh = atoi(ch->GetText ());
      if (nCh != mChannels)
        bcierr << "Stimulation Error: Number of channels defined in stimulation file (" 
               << nCh << ") not equal to number of device channels (" << mChannels << ")" << endl;
    }
    trains = root->FirstChildElement ("trains");
    if (trains) {
      train = trains->FirstChildElement ("train");
      if (train)
      {
        // delete train attribute 
        train->DeleteAttribute ("id");

        // delete iterations element
        XMLNode* ch = train->FirstChild ();;
        while (ch) {
          if ((string)ch->Value () == "iterations")  break;
          ch = ch->NextSibling ();
        }
        if (ch) {
          train->DeleteChild (ch);
        }
        blocks = train->FirstChildElement ("blocks");
        XMLPrinter streamer (NULL, true);
        blocks->Accept (&streamer);
        mBlocks = streamer.CStr ();
        streamer.ClearBuffer ();
        waveforms = train->FirstChildElement ("waveforms");
        waveforms->Accept (&streamer);
        mWaveforms = streamer.CStr ();
      }
    }
  }
  return (blocks && waveforms);
}

bool AmpServerProADC::Connection::LoadGTENPattern (const std::string& name)
{
  string pattern = "";
  if (name == "Blocks")
    pattern = mBlocks;
  else if (name == "Waveforms")
    pattern = mWaveforms;
  else
  {
    bciwarn << "Stimulation Error: " << name << " is invalid pattern to upload" << endl;
    return false;
  }
  //while (mNotifications.rdbuf ()->in_avail ()) mNotifications.ignore ();
  string cmd = "(sendCommand cmd_GTENSet" + name + " " + to_string (mAmpId) + " 0 <?xml>" + pattern + "</xml>)";
  bcidbg << cmd << endl;

  if (!(mCommands << cmd << endl)) return false;
  if (!mCommandSocket.wait_for_read (mInitialTimeout))
  {
    bciwarn << "Stimulation Error: " << name << " timeout occured" << endl;
    return false;
  }
  ResponseNode parsedResponse (mCommands);
  //ResponseNode parsedntnResponse (mNotifications);
  //bcidbg << cmd << "\n" << parsedResponse  <<  "\nNTN: " << parsedntnResponse << endl;
  ResponseNode::List                 leaves = parsedResponse.Leaves ();
  ResponseNode::List::const_iterator itr = leaves.begin ();
  const ResponseNode& status = *itr;
  if (!(status.Name () == "status" && status.Value () == "complete"))
  {
    bciwarn << "Stimulation Error: " << name << " command not valid.   "
      << status.Name () << "   " << status.Value () << endl;
    return false;
  }
  if (name == "Blocks")
    mBlocksUploaded = true;
  return true;
}

bool AmpServerProADC::Connection::BeginStimulation ()
{
  if (mBlocksUploaded)
  {
    // trigger stimulation
    bciwarn << "Stimulation Started" << endl; 

    if (!(mCommands << BuildCommand ("GTENStartTrain") << endl)) return false;
    if (!mCommandSocket.wait_for_read (mTimeout))                return false;
    ResponseNode parsedcmdResponse (mCommands);
    ResponseNode::List                 leaves = parsedcmdResponse.Leaves ();
    ResponseNode::List::const_iterator i = leaves.begin ();
    mBlocksUploaded = false;
    return (i->Name () == "status" && i->Value () == "complete");
  }
  else
  {
    bciwarn << "Stimulation not triggered. Stimulation pattern hasn't been re-uploaded" << endl;
  }
  return true;
 }

bool
AmpServerProADC::Connection::EndStimulation ()
{
  if (!(mCommands << BuildCommand ("GTENAbortTrain") << endl)) return false;
  if (!mCommandSocket.wait_for_read (mTimeout))                return false;
  ResponseNode parsedcmdResponse (mCommands);
  ResponseNode::List                 leaves = parsedcmdResponse.Leaves ();
  ResponseNode::List::const_iterator i = leaves.begin ();
  mBlocksUploaded = false;
  return (i->Name () == "status" && i->Value () == "complete");
}

bool 
AmpServerProADC::Connection::ParseAmpDetails()
{
  if (!(mCommands << BuildCommand ("GetAmpDetails") << endl)) return false;
  if (!mCommandSocket.wait_for_read (mTimeout))               return false;
  ResponseNode parsedResponse (mCommands);
  ResponseNode::List                 leaves = parsedResponse.Leaves ();
  ResponseNode::List::const_iterator i = leaves.begin ();
  const ResponseNode& status = *i; 
  bool success = (status.Name () == "status" && status.Value () == "complete");

  while (i != leaves.end ())
  {
    if (i->Name () == "packet_format")      mPacketType = stoi (i->Value ());
    if (i->Name () == "number_of_channels") mChannels = stoi (i->Value ());
    if (i->Name () == "amp_type")           mAmpType = i->Value ();
    i++;
  }
  
  mSamplingRate    = 1000;
  // EGI NA300 amps have a range of +/- 200,000uV that is digitized into
  // a 24bit signed value. The values received in BCI2000 are in float,
  // but use these values when converting back to uV.ss
  mGain_muV = 2e5 / (1 << 23);

  bciout << "Amp Type: " << mAmpType
    << "\nPacket Type: " << mPacketType
    << "\nNumber of Channels: " << mChannels << endl;
  return success;
}

string
AmpServerProADC::Connection::BuildCommand( const string& inCommand ) const
{
  string name, val1, val2;
  istringstream iss( inCommand );
  if( !(iss >> name) )
    return false;
  if( !(iss >> val1) )
    val1 = "";
  if( !(iss >> val2) )
  {
    val2 = val1;
    val1 = "";
  }
  if( val1.empty() )
    val1 = "0";
  if( val2.empty() )
    val2 = "0";

  ostringstream ampId;
  ampId << mAmpId;
  string result = "(sendCommand cmd_" + name + " " + ampId.str() + " " + val1 + " " + val2 + ")";
  //bciout << result;
  return result;
}

bool
AmpServerProADC::Connection::SendCommand( const string& inCommand, string& outResultName, string& outResultValue )
{
  std::string cmd = BuildCommand (inCommand);
  if( !( mCommands << cmd << endl ) )
    return false;
  if( !mCommandSocket.wait_for_read( mTimeout ) )
    return false;
  bool success = false;
  ResponseNode parsedResponse( mCommands );
  bcidbg << "Command: " << cmd << "\nResponse: " << parsedResponse;
  if( parsedResponse.Name() == "sendCommand_return" )
  {
    ResponseNode::List leaves = parsedResponse.Leaves();
    ResponseNode::List::const_iterator i = leaves.begin();
    const ResponseNode& status = *i;
    // check status of the command
    success = ( status.Name() == "status" && status.Value() == "complete" );
    // return the result of the command if necessary
    if (++i != leaves.end ())
    {
      outResultName = i->Name ();
      outResultValue = i->Value ();
    }
  }
  return success;
}

bool
AmpServerProADC::Connection::StartStreaming()
{
  mFirstRead = true;
  mSamplesInStream = 0;
  mSamplesInOutput = 0;
  return (bool)(mStream << BuildCommand( "ListenToAmp" ) << endl);
}

bool
AmpServerProADC::Connection::StopStreaming()
{
  bool result = !!(mStream << BuildCommand( "StopListeningToAmp" ) << endl);
  while( mStream.rdbuf()->in_avail() )
    mStream.ignore();
  return result;
}

bool
AmpServerProADC::Connection::ReadData( GenericSignal& Output )
{
  if( mFirstRead )
  {
    mFirstRead = false;
    if( !mStreamSocket.wait_for_read( mInitialTimeout ) )
      mError = "AmpServer is available, but does not stream live EEG data. "
               "Please turn on the data streaming in Net Station Acquisition. After this, if it still not works, "
               "you need to obtain an AmpServerPro SDK license from EGI, "
               "and update EGI's HASP key.\n"
               "For more information, see http://www.egi.com.";
  }
  else if( !DoRead( Output ) )
    mError = "Lost connection to AmpServerPro";
  return mError.empty();
}

bool
AmpServerProADC::Connection::DoRead( GenericSignal& Output )
{
  if (mPacketType == 2)
  { 
    while (!DoRead2(Output))
    {
      if (!mStreamSocket.wait_for_read ())
        return false;
      BinaryData<int64_t, BigEndian> ampId;
      if (!ampId.Get(mStream) || ampId != mAmpId)
          return false;
      BinaryData<uint64_t, BigEndian> packetSize;
      if (!packetSize.Get(mStream))
          return false;
      const int Packet2Size = SIZEOF_PACKETTYPE_2;
      if (packetSize % Packet2Size != 0)
          return false;
      mSamplesInStream = packetSize / Packet2Size;
      // AmpServerPro will crash if packets are not read immediately in full
      std::string buf(packetSize, '\0');
      mStream.read(const_cast<char*>(buf.data()), packetSize);
      mStringstream.str(buf);
      mStringstream.clear();
    }
  }
  else {
    while( !DoRead1( Output ) )
    {
      if (!mStreamSocket.wait_for_read(mTimeout))
          return false;
      BinaryData<int64_t, BigEndian> ampId;
      if( !ampId.Get( mStream ) || ampId != mAmpId )
          return false;
      BinaryData<uint64_t, BigEndian> packetSize;
      if( !packetSize.Get( mStream ) )
          return false;
      const int Packet1Size = SIZEOF_PACKETTYPE_1;
      if (packetSize % Packet1Size != 0)
          return false;
      mSamplesInStream = packetSize / Packet1Size;
      // AmpServerPro will crash if packets are not read immediately in full
      std::string buf(packetSize, '\0');
      mStream.read(const_cast<char*>(buf.data()), packetSize);
      mStringstream.str(buf);
      mStringstream.clear();
    }
  }
  return !!mStream;
}

bool
AmpServerProADC::Connection::DoRead2(GenericSignal& Output)
{
    BinaryData<int32_t, LittleEndian, ArrayYes> data(256);
    BinaryData<uint8_t, LittleEndian>           trainData;
    BinaryData<uint16_t, LittleEndian>          digitalInputs;
    while (mSamplesInOutput < Output.Elements() && mSamplesInStream > 0)
    {
        digitalInputs.Get(mStringstream);
        trainData.Get(mStringstream);
        mStringstream.ignore(sizeof(PacketFormat2Header) - sizeof(uint16_t) /*digitalInputs*/ - sizeof(uint8_t) /*tr*/);
        data.Get(mStringstream);
        mStringstream.ignore(sizeof(PacketFormat2Footer));

        for (int ch = 0; Output.Properties().ChannelLabels()[ch].front() != '@'; ++ch)
            Output(ch, mSamplesInOutput) = data[ch] * (float)SCALING_FACTOR;

        const int chShift = 6;
        Output(Output.Channels() - chShift, mSamplesInOutput) = digitalInputs;
        Output(Output.Channels() - chShift + 1, mSamplesInOutput) = trainData;
        //for 4 digital inputs
        //anything that is not connected is high, if nothing is connected: dig stream = 2^16-1
        for (int i = 0; i < (chShift - 2); i++)
            Output(Output.Channels() - chShift + i + 2, mSamplesInOutput) = (digitalInputs >> i) & 1;
        mSamplesInOutput += 1;
        mSamplesInStream -= 1;
    }
    bool done = (mSamplesInOutput >= Output.Elements());
    if (done)
        mSamplesInOutput = 0;
    return done || !mStream;
}

bool
AmpServerProADC::Connection::DoRead1( GenericSignal& Output )
{
  BinaryData<float, BigEndian> data;
  while( mSamplesInOutput < Output.Elements() && mSamplesInStream > 0 )
  {
    const int Packet1HeaderLength = 8 * sizeof(uint32_t);
    mStringstream.ignore(Packet1HeaderLength);
    const int Packet1ChannelCount = 256;
    for( int ch = 0; ch < std::min(Output.Channels()-1, Packet1ChannelCount); ++ch )
    {
      data.Get( mStringstream );
      Output( ch, mSamplesInOutput ) = data;
    }
    for (int ch = std::min(Output.Channels()-1, Packet1ChannelCount); ch < Packet1ChannelCount; ++ch)
      data.Get( mStringstream );
    ++mSamplesInOutput;
    const int Packet1FooterLength = 24 * sizeof(float);
    mStringstream.ignore(Packet1FooterLength);
    --mSamplesInStream;
  }
  bool done = ( mSamplesInOutput >= Output.Elements() );
  if(done)
      mSamplesInOutput = 0;
  return done || !mStream;
}
