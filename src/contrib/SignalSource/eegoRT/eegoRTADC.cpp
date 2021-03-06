////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: eegoRTADC implementation
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
#include "eegoRTADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"

// In order to help you write a source module, exchange of information
// between amplifier and the BCI2000 source module is indicated by the use of
// macros.
// Once you are done with writing the source module, each occurrence of
// GET_FROM_AMP_API(), CALL_AMP_API(), and AMP_API_SYNC_GET_DATA() should
// have been replaced with actual calls to the amplifier API, or constants.
// By removing or disabling the two macro definitions below, you can then
// make sure that the compiler will notify you of any oversights.

// Depending on the kind of amplifier you have, occurrences of GET_FROM_AMP_API()
// may be read through the amplifier API, or obtained from documentation.
#define GET_FROM_AMP_API( what, example ) (what = example)
#define CALL_AMP_API( what ) (true)
#include "ThreadUtils.h"
#define AMP_API_SYNC_GET_DATA( dataptr ) (ThreadUtils::SleepForMs(50)) // simulate blocking call
#define AMP_API_INVALID_DEVICE_HANDLE (0)

using namespace std;

// Make the source filter known to the framework.
RegisterFilter( eegoRTADC, 1 ); // ADC filters must be registered at location "1" in order to work.

eegoRTADC::eegoRTADC()
: mDeviceHandle( AMP_API_INVALID_DEVICE_HANDLE ), // Each plain data member should appear in the initializer list.
  mpBuffer( 0 ) // Make sure we can call delete[] without crashing even if we never called new[].
{
}

eegoRTADC::~eegoRTADC()
{
  // The destructor is where to make sure that all acquired resources are released.
  // Memory deallocation (calling delete[] on a NULL pointer is OK).
  delete[] mpBuffer;
  // Closing connection to device.
  // The amplifier's API might choke on an invalid device handle.
  if( mDeviceHandle != AMP_API_INVALID_DEVICE_HANDLE )
    CALL_AMP_API( CloseDevice( mDeviceHandle ) );
}

void
eegoRTADC::OnPublish()
{
  // Declare any parameters that the filter needs....

 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= auto "
       "auto 1 % // number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= auto "
       "auto 1 % // number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= auto "
       "auto 0.0 % // sample rate",

    "Source:Signal%20Properties list SourceChGain= 1 auto "
       " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 1 auto "
       " // raw A/D offset to subtract, typically 0",

    "Source:Signal%20Properties list ChannelNames= 1 auto "
       " // names of amplifier channels",

    "Source:Signal%20Properties int MyAmpSetting= 3 "
       " // some user setting specific to your amp",

 END_PARAMETER_DEFINITIONS

  // ...and likewise any state variables.

  // IMPORTANT NOTE ABOUT BUFFEREDADC AND STATES:
  // * BCI2000 States, or "state variables", are additional data channels stored alongside signal data,
  //   with a resolution of one value per source signal sample.
  // * A State is much like a source signal channel, but differs from a source signal channel in the
  //   following respects:
  //   + You choose the number of bits used to represent the State's value, up to 64.
  //   + Unlike source signals, States are transmitted through all stages of processing, and their values
  //     may be modified during processing, allowing all parts of the system to store state information in
  //     data files.
  //   + When loading a BCI2000 data file for analysis, States appear separately, with names, which is
  //     typically more useful for trigger information than being just another channel in the signal.
  //   + States may be set synchronously from inside a filter's Process() handler, or asynchronously using
  //     a "bcievent" interface.
  //   + If your amplifier provides a digital input, or another kind of "trigger" information, it makes sense
  //     to store this information in form of one or more States. From a user perspective, it is probably most
  //     useful if physically distinct amplifier input sockets correspond to States, and single bits to certain
  //     lines or values communicated through such a socket.
  //   + If the amplifier API asynchronously reports trigger information through a callback mechanism, you
  //     should register a callback that uses the "bcievent" interface to set states asynchronously.
  //     This example provides a "MyAsyncTriggers" "event state", and a sample callback function.
  //   + If the amplifier API sends trigger information in the same way as it does for signal data, you should
  //     use a "State channel" to represent it in your source module. This example provides a "MySyncTriggers"
  //     state, and writes to it when acquiring data.

  BEGIN_STATE_DEFINITIONS
    "eegoRTADCSyncTriggers 8 0", // <name> <bit width> <initial value>
  END_STATE_DEFINITIONS

  BEGIN_EVENT_DEFINITIONS
    "eegoRTADCAsyncTriggers 8 0", // <name> <bit width> <initial value>
  END_EVENT_DEFINITIONS
}

// For asynchronous trigger notification, register this callback with the amplifier API.
static void
TriggerCallback( void* pData, int trigger )
{
  reinterpret_cast<eegoRTADC*>( pData )->OnTrigger( trigger );
}

void
eegoRTADC::OnTrigger( int trigger )
{
  // The following line will create a time-stamped entry into an event queue.
  // Once the next block of data arrives, the queued trigger value will be applied
  // to the eegoRTADCAsyncTriggers state variable at the sample location that
  // corresponds to the time stamp.
  bcievent << "eegoRTADCAsyncTriggers " << trigger;
}

void
eegoRTADC::OnAutoConfig()
{
  // The user has pressed "Set Config" and some parameters may be set to "auto",
  // indicating that they should be set from the current amplifier configuration.
  // In this handler, we behave as if any parameter were set to "auto", and the
  // framework will transparently make sure to preserve user-defined values.

  // Device availability (or connection parameters) may have changed, so close
  // and reopen the connection to the device before proceeding.
  if( mDeviceHandle != AMP_API_INVALID_DEVICE_HANDLE )
    CALL_AMP_API( CloseDevice( mDeviceHandle ) );
  GET_FROM_AMP_API( mDeviceHandle, (void*)(1) );
  if( mDeviceHandle == AMP_API_INVALID_DEVICE_HANDLE )
    throw bciexception << "Could not connect to device";

  int channels = 0;
  GET_FROM_AMP_API( channels, 2 );
  Parameter( "SourceCh" ) = channels; // Set SourceCh in case of "auto"
  // If SourceCh != auto but e.g. SourceChGain != auto, we need to use the actual
  // rather than the auto-set value of SourceCh in order to consistently configure
  // SourceChGain.
  // For this purpose, an ActualParameter() call will retrieve a parameter's current
  // value, no matter whether auto-configured or user-configured.
  channels = ActualParameter( "SourceCh" );
  Parameter( "ChannelNames" )->SetNumValues( channels );
  Parameter( "SourceChGain" )->SetNumValues( channels );
  Parameter( "SourceChOffset" )->SetNumValues( channels );
  for( int i = 0; i < channels; ++i )
  {
    // For EEG amplifiers, channel names should use 10-20 naming if possible.
    const char* name = 0;
    GET_FROM_AMP_API( name, "SomeChannelName" );
    Parameter( "ChannelNames" )( i ) << name << i; // Omit "<< i" if channel names are unique.

    double gainFactor = 1.0;
    GET_FROM_AMP_API( gainFactor, 1.0 );
    Parameter( "SourceChGain" )( i ) << gainFactor << "muV"; // Always provide correct physical unit for documentation and display purposes.
    // For most amplifiers, offset removal is done within the amp hardware or driver. Otherwise, you may set this to a nonzero value.
    Parameter( "SourceChOffset" )( i ) = 0;
  }
  double samplingRate = 1.0;
  GET_FROM_AMP_API( samplingRate, 250 );
  Parameter( "SamplingRate" ) << samplingRate << "Hz";
  int samplesPerPacket = 1;
  GET_FROM_AMP_API( samplesPerPacket, 32 );
  Parameter( "SampleBlockSize" ) = samplesPerPacket;
}

void
eegoRTADC::OnPreflight( SignalProperties& Output ) const
{
  // The user has pressed "Set Config" and we need to sanity-check everything.
  // For example, check that all necessary parameters and states are accessible:
  State( "eegoRTADCAsyncTriggers" );
  State( "eegoRTADCSyncTriggers" );


  // >>> begin insert code here to initialize amplifier 
  factory fact("eego-SDK.dll"); // Make sure that eego-SDK.dll resides in the working directory

  // Also check that the values of any parameters are sane:
  bool ok;
  ok = CALL_AMP_API( VerifySamplingRate( Parameter( "SamplingRate" ).InHertz() ) );
  if( !ok )
    bcierr << "Amplifier does not support a sampling rate of " << Parameter( "SamplingRate" );
  // If there is no verification function provided by the amplifier API, you might also
  // just try to call a configuration function, and check for an error code:
  ok = CALL_AMP_API( SetDataBlockSize( Parameter( "SampleBlockSize" ).ToNumber() ) );
  if( !ok )
    bcierr << "Amplifier does not support a sample block size of " << Parameter( "SampleBlockSize" );
  //
  // Errors issued in this way, during the Preflight phase, still allow the user
  // to open the Config dialog box, fix bad parameters and re-try.  By contrast,
  // errors and C++ exceptions at any other stage (outside Preflight) will make
  // the system stop, such that BCI2000 will need to be relaunched entirely.

  // Internally, signals are always represented by GenericSignal::ValueType == double.
  // Here, you choose the format in which data will be stored, which may be
  // int16, int32, or float32.
  // Typically, you will choose the format that your amplifier natively provides, in
  // order to avoid loss of precision when converting, e.g., int32 to float32.
  SignalType sigType = SignalType::float32;

  int samplesPerBlock = Output.Elements();
  int numberOfSignalChannels = Output.Channels();
  int numberOfSyncStates = 1;
  Output = SignalProperties( numberOfSignalChannels + numberOfSyncStates, samplesPerBlock, sigType );
  // A channel name starting with @ indicates a trigger channel.
  Output.ChannelLabels()[numberOfSignalChannels] = "@eegoRTADCSyncTriggers";
  // Note that the eegoRTADC instance itself, and its members, are read-only during the
  // Preflight phase---note the "const" at the end of the OnPreflight prototype above.
  // Any methods called by OnPreflight must also be declared as "const" in the same way.
}

void
eegoRTADC::OnInitialize( const SignalProperties& Output )
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The system will now transition into an "Initialized" state.

  // >>> begin insert code here to initialize amplifier 
  factory fact("eego-SDK.dll"); // Make sure that eego-SDK.dll resides in the working directory


  // If the user should be able to control amplifier configuration through BCI2000,
  // you will need to use the amplifier's API here in order to apply parameter settings.
  bool ok = CALL_AMP_API( SetSamplingRate( Parameter( "SamplingRate" ).InHertz() ) );
  ok = ok && CALL_AMP_API( SetDataBlockSize( Parameter( "SampleBlockSize" ).ToNumber() ) );
  ok = ok && CALL_AMP_API( SetNumberOfChannels( Parameter( "SourceCh" ).ToNumber() ) );
  // Preflight() checks should ensure that Initialize() never fails due to misconfiguration.
  // Still, unforeseen cases or hardware conditions may lead to failure.
  if( !ok ) 
    bcierr << "Configuration failed unexpectedly";

  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the eegoRTADC instance itself can be modified. Allocate any memory you need
  // store any information you need in private member variables.
  mNumberOfSignalChannels = Parameter( "SourceCh" );
  int bufferBytes = Output.Channels() * Output.Elements() * sizeof( float );
  delete[] mpBuffer;
  mpBuffer = new char[bufferBytes];
  ::memset( mpBuffer, 0, bufferBytes );
  // Buffer allocation may happen in OnStartAcquisition as well, if more appropriate.
}

void
eegoRTADC::OnStartAcquisition()
{
  // This method is called from the acquisition thread once the system is initialized.
  // You should use this space to start up your amplifier using its API.  Any initialization
  // here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.
  CALL_AMP_API( StartAcquisition );
  CALL_AMP_API( SetTriggerCallback( &TriggerCallback, this ) );
}

void
eegoRTADC::DoAcquire( GenericSignal& Output )
{
  // Now we're acquiring a single SampleBlock of data in the acquisition thread, which is stored
  // in an internal buffer until the main thread is ready to process it.

  // Internally, BufferedADC writes this data to a buffer, then locks a mutex and pushes the data
  // into the GenericSignal output in the main thread.  The size of this buffer is parameterized by
  // "SourceBufferSize" declared by BufferedADC, which will be interpreted as follows:
  // * When set to a naked number, the number will be rounded to the nearest integer, and specify
  //   the number of BCI2000 data blocks (cf the SampleBlockSize parameter). The buffer's
  //   duration in time will vary with changes to the SamplingRate and SampleBlockSize parameters.
  // * When set to a number followed with an SI time unit, the buffer's size will be automatically
  //   chosen to match the specified time duration as close as possible. By default, the value is 2s.
  //   SI time units must be appended without white space, and consist of
  //   a valid SI prefix (such as m for milli=1e-3, mu for micro=1e-6, k for kilo=1e3),
  //   followed with a lowercase s (for seconds).

  // Keep in mind that even though we're writing this data from another thread, the main thread
  // cannot continue without data, so be sure this is done in a timely manner
  // or the system will not be able to perform in real-time.

  // IMPORTANT NOTES ABOUT BUFFERING

  // Ideally, the BCI2000 processing time ("Roundtrip time") is always shorter than the physical
  // duration of a sample block, i.e. every single block of data has been processed before its
  // successor arrives. In that ideal case, buffering makes no difference, because the main thread will
  // always wait for the acquisition thread to deliver the next block of data into the internal
  // buffer, and copy it from there immediately.

  // If, on average, processing takes longer than the physical duration of a sample block, buffering
  // will not help to improve things either, because the buffer will be filled faster than
  // it is being emptied, and thus it will overflow after a certain time. Depending on buffering strategy,
  // buffer overflow may be dealt with by looping (i.e., overwriting data that has not been delivered yet),
  // or by blocking (not accepting any new data before there is space in the buffer).
  // For scientific purposes -- as opposed to, e.g., entertainment applications -- silently omitting
  // data is not an option, so BufferedADC will use the blocking strategy, and deliver all data blocks,
  // but delayed by the physical duration of its buffer.

  // So the _only_ situation in which buffering is actually useful is for cases when processing is not
  // able to keep up with data acquisition for short amounts of time. Typical examples are lengthy 
  // computations that happen from time to time, such as updating a classifier matrix, or initialization
  // work in algorithm implementations that are not factored out into initialization and update operations
  // (otherwise, you would just do lengthy operations inside Initialize()).
  // In such cases, you should set the SourceBufferSize parameter to an estimate of the longest expected
  // delay.

  // Call the amplifier API's function for synchronous data transfer here.
  AMP_API_SYNC_GET_DATA( mpBuffer );
  const float* pSignalData = reinterpret_cast<float*>( mpBuffer );
  // Copy values from raw buffer into output signal.
  for( int ch = 0; ch < mNumberOfSignalChannels; ch++ )
  {
    for( int sample = 0; sample < Output.Elements(); sample++ )
    {
      // Check the amplifier's documentation whether data are sent in
      // channel-major or sample-major format.
      Output( ch, sample ) = pSignalData[ch + sample * mNumberOfSignalChannels];
      // When getting garbage, try this instead:
      // Output( ch, el ) = pSignalData[el + ch * Output.Elements()];
    }
  }
  const char* pSyncTriggerData = 0;
  GET_FROM_AMP_API( pSyncTriggerData, mpBuffer );
  for( int sample = 0; sample < Output.Elements(); sample++ )
  {
    // just an example, unlikely to work for a certain amplifier
    Output( mNumberOfSignalChannels, sample ) = pSyncTriggerData[sample];
  }
}

void
eegoRTADC::OnStopAcquisition()
{
  // This method is called from the acquisition thread just before it exits.  Use this method
  // to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
  // Immediately after this returns, the system will go into an un-initialized state and
  // OnHalt will be called in the main thread.
  CALL_AMP_API( StopAcquisition );
  CALL_AMP_API( RemoveTriggerCallback );
}

