////////////////////////////////////////////////////////////////////////////////
// Authors: schalklab@HR18818.wucon.wustl.edu
// Description: ANTTask implementation
////////////////////////////////////////////////////////////////////////////////

#include "ANTTask.h"
#include "BCIStream.h"
#include <random>
#include <QVBoxLayout>
#include <numeric>
#include <iomanip>
#include <Windows.h>
#include "ProgressBarVis.h"


using  std::string; using std::vector; using std::endl;

void
myfunction(int i) {
    bciout << ' ' << i;
}

RegisterFilter( ANTTask, 3 );
     // Change the location if appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations within SignalProcessing modules begin with "2."
     //       (NB: SignalProcessing modules must specify this with a Filter() command in their PipeDefinition.cpp file too)
     //  - filter locations within Application modules begin with "3."


ANTTask::ANTTask() :
    mrDisplay(Window())
{
  // C++ does not initialize simple types such as numbers, or pointers, by default.
  // Rather, they will have random values.
  // Take care to initialize any member variables here, so they have predictable values
  // when used for the first time.
}

ANTTask::~ANTTask()
{
  Halt();
  //layout->removeWidget(myIconU);
  //layout->removeWidget(myIconC);
  //layout->removeWidget(myIconD);
  //if(myIconC)
  //  delete myIconC;
  //if (myIconU)
  //  delete myIconU;
  //if(myIconD)
  //  delete myIconD;
  //if(layout)
  //  delete layout;
  //if(myWindow)
  //  delete myWindow;
  if(myTextStimls)
      delete myTextStimls;
  if (introductTxtStimls)
      delete introductTxtStimls;
  if (correctTxtStimls)
      delete correctTxtStimls;
  if (correctTxtStimlsTop)
      delete correctTxtStimlsTop;
  if (mImgStimlsUp)
      delete mImgStimlsUp;
  if (mImgStimlsCenter)
      delete mImgStimlsCenter;
  if (mImgStimlsDown)
      delete mImgStimlsDown;
  if (mpProgressBar) {
      delete mpProgressBar;
  }
}

void
ANTTask::Publish()
{
    map<string, string> stimulusMap = initStimulus();

  // Define any parameters that the filter needs....
 BEGIN_PARAMETER_DEFINITIONS
    //sequencing
   "Application:Sequencing float EyeOpenTextDuration= 3s 3s 0 % "
   " //Duration of eyes open text",
   "Application:Sequencing float TimerValueOpen= 180s 180s 0 % "
   " //Initial value of the eyes open timer",
   "Application:Sequencing float EyeCloseTextDuration= 3s 3s 0 % "
     " //Duration of eyes close text",
   "Application:Sequencing float TimerValueClose= 180s 180s 0 % "
     " //Initial value of the eyes close timer",
   "Application:Sequencing float TrialDuration= 4000ms 4000ms 0 % "
     " //Duration of one trial",
   "Application:Sequencing float FirstFixationDurationMin= 400ms 400ms 0 % "
     " //Minimum duration of First Fixation",
   "Application:Sequencing float FirstFixationDurationMax= 1600ms 1600ms 0 % "
     " //Maximum duration of First Fixation",
   "Application:Sequencing float CueDuration= 100ms 100ms 0 % "
     " //Duration of cue",
   "Application:Sequencing float MiddleFixationDuration= 400ms 400ms 0 % "
     " //Duration of Middle Fixation",
   "Application:Sequencing float ReactionTimeMin= 40ms 40ms 0 % "
     " //Minimum duration of reaction time",
   "Application:Sequencing float ReactionTimeMax= 1700ms 1700ms 0 % "
     " //Maximum duration of reaction time",
   "Application:Sequencing list CueIconList= " +
     stimulusMap["cue"] + " % % % "
     " //Cue stimulus list ",
   "Application:Sequencing matrix TargetMatrix= "
     "{ Target%20Pointing%20Left Target%20Pointing%20Right }"
     "{ Flankers%20Pointing%20Left Flankers%20Pointing%20Right } "
     "../tasks/ANTTask/resource/icons/TargetLeftFlankerLeft.png ../tasks/ANTTask/resource/icons/TargetLeftFlankerRight.png "
     "../tasks/ANTTask/resource/icons/TargetRightFlankerLeft.png ../tasks/ANTTask/resource/icons/TargetRightFlankerRight.png "
     "//Target stimulus matrix ",
     //experiment
     "Application:Experiment intlist BlockSequence= 3 96 96 96 % % % "
     "//Sequence of Block size",
     "Application:Experiment int PracticeBlockSize= 24 24 0 % "
     "//Practice Block size",
     "Application:Experiment int EnableEyeOpenOrClose= 1 1 0 1 // enable EnableEyeOpenOrClose (boolean)",
     //drop down box, notes for blocks
   /*  "Application:Experiment int BlockIndex= 0 0 0 10 // "
     "block Index  0 practice, 1 block1, 2 block2, 3 block3, 4 block4, 5 block5, 6 block6, 7 block7, 8 block8, 9 block9, 10 block10 (enumeration)",*/
     //black patch size and color
     //"Application:Experiment float PatchLeft= 0.97 0.97 0 1 "
     //" //The x-coordinate of the upper-left corner of the patch",
     //"Application:Experiment float PatchTop= 0.945 0.945 0 1 "
     //" //The y-coordinate of the upper-left corner of the patch",
     //"Application:Experiment float PatchRight= 1 1 0 1 "
     //" //The x-coordinate of the lower-right corner of the patch",
     //"Application:Experiment float PatchBottom= 1 1 0 1 "
     //" //The y-coordinate of the lower-right corner of the patch",
     //"Application:Experiment string PatchColor= 0x000000 0xFFFFFF 0x000000 0xFFFFFF "
     //" //The color of the patch(color)",
     //distance between fixation and cue, fixation and target
     "Application:Experiment float CrossToCueDistance= 0.024 0.024 0 1 "
     " //The distance between fixation cross and cue",
     "Application:Experiment float CrossToTargetDistance= 0.026 0.026 0 1 "
     " //The distance between fixation cross and target",
     //size of cue, target, fixation cross
     "Application:Experiment float CrossWidth= 0.006 0.006 0 1 "
     " //The width of fixation cross",
     "Application:Experiment float CrossHeight= 0.012 0.012 0 1 "
     " //The height of fixation cross",
     "Application:Experiment float CueWidth= 0.006 0.006 0 1 "
     " //The width of cue",
     "Application:Experiment float CueHeight= 0.012 0.012 0 1 "
     " //The height of cue",
     "Application:Experiment float TargetWidth= 0.08 0.08 0 1 "
     " //The width of target",
     "Application:Experiment float TargetHeight= 0.008 0.008 0 1 "
     " //The height of target",
     //progress bar
     "Application:ProgressBar int ProgressBar= 1 1 0 1 // "
     "Display progress bar (boolean)",
     "Application:ProgressBar float ProgressBarHeight= 50 50 0 % //"
     "Progress bar height in pixels",
     "Application:ProgressBar float ProgressBarWidth= 250 250 0 % //"
     "Progress bar width in pixels",
     "Application:ProgressBar string ProgressBarBackgroundColor= 0x00808080 0x00808080 0x00000000 0xFFFFFFFF // "
     "Color of progress bar background (color)",
     "Application:ProgressBar string ProgressBarForegroundColor= 0x00ffff00 0x00f0f0f0 0x00000000 0xFFFFFFFF // "
     "Color of progress bar foreground (color)",
     //photodiode
     "Application:PhotoDiodePatch int PhotoDiodePatch= 1 1 0 1 "
     "// Display photo diode patch (boolean)",
     "Application:PhotoDiodePatch float PhotoDiodePatchHeight= 0.065 1 0 1 "
     "// Photo diode patch height in relative coordinates",
     "Application:PhotoDiodePatch float PhotoDiodePatchWidth= 0.05 1 0 1 "
     "// Photo diode patch width in relative coordinates",
     "Application:PhotoDiodePatch float PhotoDiodePatchLeft= 0 1 0 1 "
     "// Photo diode patch left in relative coordinates",
     "Application:PhotoDiodePatch float PhotoDiodePatchTop= 0.935 1 0 1 "
     "// Photo diode patch top in relative coordinates",
     "Application:PhotoDiodePatch int PhotoDiodePatchShape= 0 1 0 1 "
     "// Photo diode patch shape: 0 rectangle, 1 ellipse (enumeration)",
     "Application:PhotoDiodePatch int PhotoDiodePatchActiveColor= 0x000000 0 0 0xffffffff "
     "// Photo diode patch color when active (color)",
     "Application:PhotoDiodePatch int PhotoDiodePatchInactiveColor= 0xffffff 0 0 0xffffffff "
     "// Photo diode patch color when inactive, use 0xff000000 for transparent (color)",
 END_PARAMETER_DEFINITIONS

 BEGIN_STATE_DEFINITIONS

   "PhaseNumber  8 0 0 0",    
   "TrialNumber  16 0 0 0",
   "FirstFixationDuration  32 0 0 0",
   "ReactionTime  32 0 0 0",
   "LastFixationDuration  32 0 0 0",
   "BlockNum 8 0 0 0",
   "TargetDirection 8 0 0 0",
   "FlankerDirection 8 0 0 0",
   "TargetLocation 8 0 0 0",
   "CueLocation 8 0 0 0",
   "CueVisible 8 0 0 0",
   "TargetVisible 8 0 0 0",

 END_STATE_DEFINITIONS

}

void
ANTTask::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
    //check parameter
    map<string,float> eMap;
    float EyeOpenText = Parameter("EyeOpenTextDuration").InSampleBlocks();
    float TimerValueOpen = Parameter("TimerValueOpen").InSampleBlocks();
    float EyeCloseText = Parameter("EyeCloseTextDuration").InSampleBlocks();
    float TimerValueClose = Parameter("TimerValueClose").InSampleBlocks();
    float TrialDuration = Parameter("TrialDuration").InSampleBlocks();
    float FirstFixationDurationMin = Parameter("FirstFixationDurationMin").InSampleBlocks();
    float FirstFixationDurationMax = Parameter("FirstFixationDurationMax").InSampleBlocks();
    float CueDuration = Parameter("CueDuration").InSampleBlocks();
    float MiddleFixationDuration = Parameter("MiddleFixationDuration").InSampleBlocks();
    float ReactionTimeMin = Parameter("ReactionTimeMin").InSampleBlocks();
    float ReactionTimeMax = Parameter("ReactionTimeMax").InSampleBlocks();
    eMap["EyeOpenTextDuration"] = EyeOpenText;
    eMap["TimerValueOpen"] = TimerValueOpen;
    eMap["EyeCloseTextDuration"] = EyeCloseText;
    eMap["TimerValueClose"] = TimerValueClose;
    eMap["TrialDuration"] = TrialDuration;
    eMap["FirstFixationDurationMin"] = FirstFixationDurationMin;
    eMap["FirstFixationDurationMax"] = FirstFixationDurationMax;
    eMap["CueDuration"] = CueDuration;
    eMap["MiddleFixationDuration"] = MiddleFixationDuration;
    eMap["ReactionTimeMin"] = ReactionTimeMin;
    eMap["ReactionTimeMax"] = ReactionTimeMax;
    float sampleRate = Parameter("SamplingRate");//Sampling rate
    unsigned int blockSize = Parameter("SampleBlockSize");// Sample block size
    float sBlockMs = ((float)(blockSize) / (float)(sampleRate)) * 1000;
    for (map<string, float>::iterator it = eMap.begin(); it!=eMap.end(); it++) {
        if (it->second < 1) {
            bcierr << it->first << " must be >= 1 sampleBlock duration(" + std::to_string(sBlockMs) + " ms." << endl;
        }
        if (fmod(it->second, 1.0f) > 0) {
            bciwarn << "Due to a sample block duration is " + std::to_string(sBlockMs) + " ms. The actual value of " << it->first << " will be "
                << floor(it->second) * sBlockMs << "ms rather than " << it->second * sBlockMs << "ms.";
        }
    }
    //check trial duration
    if ((TrialDuration - FirstFixationDurationMax - CueDuration - MiddleFixationDuration - ReactionTimeMax) < 0) {
        bcierr << "TrialDuration must be larger than the sum of other duration" << endl;
    }
    //check wheter list isNone
    if (Parameter("CueIconList")->NumValues() == 0) {
        bcierr << "CueIconList should't be empty!";
    }
    if (Parameter("TargetMatrix")->NumColumns() == 0) {
        bcierr << "TargetMatrix should't be empty!";
    }


    Parameter("SamplingRate");
    Parameter("SampleBlockSize");
    //Parameter("BlockIndex");

    //Parameter("PatchLeft");
    //Parameter("PatchRight");
    //Parameter("PatchTop");
    //Parameter("PatchBottom");
    //Parameter("PatchColor");
    Parameter("CrossToCueDistance");
    Parameter("CrossToTargetDistance");
    Parameter("CrossWidth");
    Parameter("CueWidth");
    Parameter("TargetWidth");
    Parameter("CrossHeight");
    Parameter("CueHeight");
    Parameter("TargetHeight");
    Parameter("WindowWidth");
    Parameter("WindowHeight");
    Parameter("WindowLeft");
    Parameter("WindowTop");
    Parameter("BlockSequence");
    Parameter("PracticeBlockSize");
    Parameter("EyeCloseTextDuration");
    Parameter("TimerValueClose");
    Parameter("EnableEyeOpenOrClose");


    //check state
    State("PhaseNumber");
    State("TrialNumber");
    State("FirstFixationDuration");
    State("ReactionTime");
    State("LastFixationDuration");
    State("KeyDown");
    State("MouseKeys");
    State("Running");
    State("BlockNum");
    State("TargetDirection");
    State("FlankerDirection");
    State("TargetLocation");
    State("CueLocation");
    State("CueVisible");
    State("TargetVisible");

  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....

}


void
ANTTask::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{


    //sample, block
    mSampleRate = Parameter("SamplingRate");//Sampling rate
    mBlockSize = Parameter("SampleBlockSize");// Sample block size
    mOneBlockMs = ( (float)(mBlockSize) / (float)(mSampleRate) )* 1000; //one block size in msec 
    practiseB  = floor(MeasurementUnits::TimeInSampleBlocks("1s"));//trans time to number of block
    practiseBC = floor(MeasurementUnits::TimeInSampleBlocks("1s"));//trans time to number of block
    halfsecondB = floor(MeasurementUnits::TimeInSampleBlocks("0.5s"));//trans time to number of block
    EnableEyeOpenOrClose = Parameter("EnableEyeOpenOrClose");

  //get paramter values
    //TrialDurationMS = Parameter("TrialDuration").InMilliseconds();
    FirstFixationDurationMinMS = Parameter("FirstFixationDurationMin").InMilliseconds();
    FirstFixationDurationMaxMS = Parameter("FirstFixationDurationMax").InMilliseconds();

    //CueDurationMS = Parameter("CueDuration").InMilliseconds();
    //MiddleFixationDurationMS = Parameter("MiddleFixationDuration").InMilliseconds();
    //ReactionTimeMaxMs = Parameter("ReactionTimeMax").InMilliseconds();

    TrialDurationB  = floor(Parameter("TrialDuration").InSampleBlocks());
    CueDurationB = floor(Parameter("CueDuration").InSampleBlocks());
    MiddleFixationDurationB = floor(Parameter("MiddleFixationDuration").InSampleBlocks());
    ReactionTimeMinB = floor(Parameter("ReactionTimeMin").InSampleBlocks());
    ReactionTimeMaxB = floor(Parameter("ReactionTimeMax").InSampleBlocks());

    //timer related
    eyeCloseTextB = floor(Parameter("EyeCloseTextDuration").InSampleBlocks());
    timerValueCloseB = floor(Parameter("TimerValueClose").InSampleBlocks());
    eyeOpenTextB = floor(Parameter("EyeOpenTextDuration").InSampleBlocks());
    timerValueOpenB = floor(Parameter("TimerValueOpen").InSampleBlocks());

    //trial squen
    trialSequen.clear();
    trialPractice = Parameter("PracticeBlockSize");
    total_trial_num = 0;
    for (int i = 0; i != Parameter("BlockSequence")->NumValues(); i++){
        trialSequen.push_back(Parameter("BlockSequence")(i));
        total_trial_num += Parameter("BlockSequence")(i);
    }
    total_trial_num += trialPractice;

    if (trialPractice != 0) {
        TrialNum = trialPractice;
        currtBlockSeqIdx = -1;
        practiceMode = 1;       
    }
    else {
        TrialNum = trialSequen[0];
        currtBlockSeqIdx = 0;
        practiceMode = 0;
    }


    cueIdxVec.clear();
    targetIdxVecU.clear();
    targetIdxVecD.clear();
    targetIdxVec.clear();
    cueIdxArry.clear();
    cueToTargetIdxMap.clear();
    //initial index of target
    for (int i = 0; i < TrialNum; i++) {
        if (i < TrialNum / 8) {
            targetIdxVecU.push_back(1);
            continue;
        }
        else if (i < TrialNum / 8 * 2) {
            targetIdxVecU.push_back(2);
            continue;
        }
        else if (i < TrialNum / 8 * 3) {
            targetIdxVecU.push_back(3);
            continue;
        }
        else if (i < TrialNum / 8 * 4) {
            targetIdxVecU.push_back(4);
            continue;
        }
        else if (i < TrialNum / 8 * 5) {
            targetIdxVecD.push_back(5);
            continue;
        }
        else if (i < TrialNum / 8 * 6) {
            targetIdxVecD.push_back(6);
            continue;
        }
        else if (i < TrialNum / 8 * 7) {
            targetIdxVecD.push_back(7);
            continue;
        }
        else if (i < TrialNum) {
            targetIdxVecD.push_back(8);
            continue;
        }
    }



    //initial index of cue  & associate cue idx with target value
    for (int i = 0; i < TrialNum / 6; i++) {
        cueIdxVec.push_back(1);
        cueIdxArry.push_back(i);
        int temp = genUniformRandom(0, targetIdxVecD.size() - 1);
        cueToTargetIdxMap[i] = targetIdxVecD[temp];
        targetIdxVecD.erase(targetIdxVecD.begin() + temp);
    }
    for (int i = TrialNum / 6; i < TrialNum / 3; i++) {
        cueIdxVec.push_back(2);
        cueIdxArry.push_back(i);
        int temp = genUniformRandom(0, targetIdxVecU.size() - 1);
        cueToTargetIdxMap[i] = targetIdxVecU[temp];
        targetIdxVecU.erase(targetIdxVecU.begin() + temp);
    }
    for (int i = TrialNum / 3; i < TrialNum / 3 * 2; i++) {
        cueIdxVec.push_back(3);
        cueIdxArry.push_back(i);
    }
    for (int i = TrialNum / 3 * 2; i < TrialNum; i++) {
        cueIdxVec.push_back(4);
        cueIdxArry.push_back(i);
    }

    //concat two vector
    targetIdxVec.reserve(targetIdxVecU.size() + targetIdxVecD.size()); // preallocate memory
    targetIdxVec.insert(targetIdxVec.end(), targetIdxVecU.begin(), targetIdxVecU.end());
    targetIdxVec.insert(targetIdxVec.end(), targetIdxVecD.begin(), targetIdxVecD.end());

    for (int i = TrialNum / 3; i < TrialNum; i++) {
        int temp = genUniformRandom(0, targetIdxVec.size() - 1);
        cueToTargetIdxMap[i] = targetIdxVec[temp];
        targetIdxVec.erase(targetIdxVec.begin() + temp);
    }

    //shuffle cue
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(cueIdxArry.begin(), cueIdxArry.end(), std::default_random_engine(seed));

    // show map content:
    for (int i = 0; i < TrialNum; i++) {
        //bciout << cueIdxVec[cueIdxArry[i]] << " => " << cueToTargetIdxMap[cueIdxArry[i]] << endl;
        //check the map
        if (cueIdxVec[cueIdxArry[i]] == 2 && cueToTargetIdxMap[cueIdxArry[i]] > 4) {
            bcierr << "cue shuffle wrong!! " << endl;
        }
        if (cueIdxVec[cueIdxArry[i]] == 1 && cueToTargetIdxMap[cueIdxArry[i]] <= 4) {
            bcierr << "cue shuffle wrong!! " << endl;
        }
    }


    for (int i = 0; i < Parameter("CueIconList")->NumValues(); ++i) {
        cueIconVect.push_back(Parameter("CueIconList")(i));
    }

    ParamRef TargetStimuli = Parameter("TargetMatrix");
    for (int i = 0; i < TargetStimuli->NumColumns(); ++i) {
        if (TargetStimuli->RowLabels().Exists("Target Pointing Left"))
            targetIconVect.push_back(TargetStimuli("Target Pointing Left",i));
        if (TargetStimuli->RowLabels().Exists("Target Pointing Right"))
            targetIconVect.push_back(TargetStimuli("Target Pointing Right", i));
    }

    //initial the stimulus
    //textstimulus
    mTextRect.left = 0.05f;// 0.2f;
    mTextRect.right = 0.95f;// 0.8f;
    mTextRect.top = 0.1f;// 0.3f;
    mTextRect.bottom = 0.7f;// 0.4f;

    myTextStimls = new TextStimulus(mrDisplay);
    myTextStimls->SetText("Get Ready...");
    myTextStimls->SetTextHeight(0.05f);//text size, adaptive with the window size
    myTextStimls->SetDisplayRect(mTextRect);
    //myTextStimls->SetColor(RGBColor::Black);
    myTextStimls->SetTextColor(RGBColor::Red);
    //myTextStimls->SetPresentationMode(VisualStimulus::ShowHide);
    myTextStimls->Conceal();

    mIntroTxtRect.left = 0.05f;// 0.2f;
    mIntroTxtRect.right = 0.95f;// 0.8f;
    mIntroTxtRect.top = 0.05f;// 0.3f;
    mIntroTxtRect.bottom = 0.95f;// 0.4f;

    introductTxtStimls = new TextStimulus(mrDisplay);
    introductTxtStimls->SetText("");
    introductTxtStimls->SetTextHeight(0.025f);//text size, adaptive with the window size
    introductTxtStimls->SetDisplayRect(mIntroTxtRect);
    //introductTxtStimls->SetColor(RGBColor::Black);
    introductTxtStimls->SetTextColor(RGBColor::Black);
    //introductTxtStimls->SetPresentationMode(VisualStimulus::ShowHide);
    introductTxtStimls->SetAspectRatioMode(1);
    introductTxtStimls->Conceal();

    mCorTxtRect.left = 0.05f;
    mCorTxtRect.right = 0.95f;
    mCorTxtRect.top = 0.3f;
    mCorTxtRect.bottom = 0.9f;

    correctTxtStimls = new TextStimulus(mrDisplay);
    correctTxtStimls->SetText("");
    correctTxtStimls->SetTextHeight(0.05f);//text size, adaptive with the window size
    correctTxtStimls->SetDisplayRect(mCorTxtRect);
    correctTxtStimls->SetTextColor(RGBColor::Blue);
    correctTxtStimls->Conceal();

    mCorTxtRectTop.left = 0.05f;
    mCorTxtRectTop.right = 0.95f;
    mCorTxtRectTop.top = 0.1f;
    mCorTxtRectTop.bottom = 0.7f;

    correctTxtStimlsTop = new TextStimulus(mrDisplay);
    correctTxtStimlsTop->SetText("");
    correctTxtStimlsTop->SetTextHeight(0.05f);//text size, adaptive with the window size
    correctTxtStimlsTop->SetDisplayRect(mCorTxtRectTop);
    correctTxtStimlsTop->SetTextColor(RGBColor::Blue);
    correctTxtStimlsTop->Conceal();


    //imagestimulus
    mImgRectCenter.left   = 0.5f - Parameter("CrossWidth") / 2;// 0.497f;
    mImgRectCenter.right  = 0.5f + Parameter("CrossWidth") / 2;// 0.503f;
    mImgRectCenter.top    = 0.5f - Parameter("CrossHeight") / 2;// 0.489f;
    mImgRectCenter.bottom = 0.5f + Parameter("CrossHeight") / 2;// 0.511f;

    mImgRectCenter_a.left   = 0.5f - Parameter("CueWidth") / 2;// 0.497f;
    mImgRectCenter_a.right  = 0.5f + Parameter("CueWidth") / 2;// 0.503f;
    mImgRectCenter_a.top    = 0.5f - Parameter("CueHeight") / 2;// 0.494f;
    mImgRectCenter_a.bottom = 0.5f + Parameter("CueHeight") / 2;// 0.506f;

    mImgStimlsCenter = new ImageStimulus(mrDisplay);
    mImgStimlsCenter->SetDisplayRect(mImgRectCenter);
    mImgStimlsCenter->SetRenderingMode(GUI::RenderingMode::Transparent);
    //mp_image_stim->SetPresentationMode(VisualStimulus::ShowHide);
    mImgStimlsCenter->SetFile(cueIconVect[1]);
    mImgStimlsCenter->Conceal();

    mImgRectUp.left   = 0.5f - Parameter("TargetWidth") / 2;// 0.46f;
    mImgRectUp.right  = 0.5f + Parameter("TargetWidth") / 2;// 0.54f;
    mImgRectUp.top    = 0.5f - Parameter("TargetHeight") / 2 - Parameter("CrossToTargetDistance");//0.47f;
    mImgRectUp.bottom = mImgRectUp.top + Parameter("TargetHeight");//0.478f;

    mImgRectUp_a.left   = 0.5f - Parameter("CueWidth") / 2;//0.497f;
    mImgRectUp_a.right  = 0.5f + Parameter("CueWidth") / 2;//0.503f;
    mImgRectUp_a.top    = 0.5f - Parameter("CueHeight") / 2 - Parameter("CrossToCueDistance");//0.47f;
    mImgRectUp_a.bottom = mImgRectUp_a.top + Parameter("CueHeight");//0.482f;

    mImgStimlsUp = new ImageStimulus(mrDisplay);
    mImgStimlsUp->SetDisplayRect(mImgRectUp);
    mImgStimlsUp->SetRenderingMode(GUI::RenderingMode::Transparent);
    //mp_image_stim->SetPresentationMode(VisualStimulus::ShowHide);
    mImgStimlsUp->SetFile(cueIconVect[0]);
    mImgStimlsUp->Conceal();

    mImgRectDown.left   = 0.5f - Parameter("TargetWidth") / 2;// 0.46f;
    mImgRectDown.right  = 0.5f + Parameter("TargetWidth") / 2;//0.54f;
    mImgRectDown.top    = 0.5f - Parameter("TargetHeight") / 2 + Parameter("CrossToTargetDistance");// 0.53f;
    mImgRectDown.bottom = mImgRectDown.top + Parameter("TargetHeight");//0.538f;

    mImgRectDown_a.left   = 0.5f - Parameter("CueWidth") / 2;//0.497f;
    mImgRectDown_a.right  = 0.5f + Parameter("CueWidth") / 2;//0.503f;
    mImgRectDown_a.top    = 0.5f - Parameter("CueHeight") / 2 + Parameter("CrossToCueDistance");;// 0.53f;
    mImgRectDown_a.bottom = mImgRectDown_a.top + Parameter("CueHeight");//0.542f;

    mImgStimlsDown = new ImageStimulus(mrDisplay);
    mImgStimlsDown->SetDisplayRect(mImgRectDown);
    mImgStimlsDown->SetRenderingMode(GUI::RenderingMode::Transparent);
    //mp_image_stim->SetPresentationMode(VisualStimulus::ShowHide);
    for (int i = 0; i < targetIconVect.size(); ++i)
        mImgStimlsDown->SetFile(targetIconVect[i]);
    mImgStimlsDown->Conceal();

    //initial black rect
    //RGBColor patchColor = RGBColor(Parameter("PatchColor"));
    //rectBlack = new RectangularShape(mrDisplay);
    //rectBlack->SetColor(patchColor);
    //rectBlack->SetFillColor(patchColor);
    //rectPatch = { (float)Parameter("PatchLeft"),(float)Parameter("PatchTop"), (float)Parameter("PatchRight"), (float)Parameter("PatchBottom") };
    //rectBlack->SetDisplayRect(rectPatch);
    //rectBlack->Hide();


    correctVector.clear();
    Rt_no_cue_sum = 0.0;
    Rt_center_cue_sum = 0.0;
    Rt_spatial_cue_sum = 0.0;
    Rt_congr_sum = 0.0;
    Rt_incongr_sum = 0.0;
    Rt_no_cue_size = 0;
    Rt_center_cue_size = 0;
    Rt_spatial_cue_size = 0;
    Rt_congr_size = 0;
    Rt_incongr_size = 0;
    Rt_no_cue = 0.0;
    Rt_center_cue = 0.0;
    Rt_spatial_cue = 0.0;
    Rt_congr = 0.0;
    Rt_incongr = 0.0;

    //progress bar
    if (mpProgressBar)
    {
        mpProgressBar->Send(CfgID::Visible, false);
    }
    delete mpProgressBar;
    mpProgressBar = nullptr;
    if (Parameter("ProgressBar") != 0)
    {
        mpProgressBar = new ProgressBarVis();
        mpProgressBar->SetBackgroundColor(RGBColor(Parameter("ProgressBarBackgroundColor")));
        mpProgressBar->SetForegroundColor(RGBColor(Parameter("ProgressBarForegroundColor")));
        mpProgressBar->SetHeight(Parameter("ProgressBarHeight"));
        mpProgressBar->SetWidth(Parameter("ProgressBarWidth"));

        mpProgressBar->Send(CfgID::WindowTitle, "Progress");
        mpProgressBar->Send(CfgID::Visible, true);
        mpProgressBar->SendReferenceFrame();
    }

    //photodiode
    delete mPhotoDiodePatch.pShape;
    mPhotoDiodePatch.pShape = nullptr;
    if (Parameter("PhotoDiodePatch") != 0)
    {
        mPhotoDiodePatch.activeColor = Parameter("PhotoDiodePatchActiveColor").ToNumber();
        mPhotoDiodePatch.inactiveColor = Parameter("PhotoDiodePatchinActiveColor").ToNumber();
        enum { rectangle = 0, ellipse = 1 };
        switch (int(Parameter("PhotoDiodePatchShape")))
        {
        case rectangle:
            mPhotoDiodePatch.pShape = new RectangularShape(Display(), -1);
            break;
        case ellipse:
        default:
            mPhotoDiodePatch.pShape = new EllipticShape(Display(), -1);
            break;
        }
        mPhotoDiodePatch.pShape->SetHeight(Parameter("PhotoDiodePatchHeight"))
            .SetWidth(Parameter("PhotoDiodePatchWidth"))
            .SetPositionX(Parameter("PhotoDiodePatchLeft") + Parameter("PhotoDiodePatchWidth") / 2.0)
            .SetPositionY(Parameter("PhotoDiodePatchTop") + Parameter("PhotoDiodePatchHeight") / 2.0);
        mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.inactiveColor)
            .SetFillColor(mPhotoDiodePatch.inactiveColor);
    }
}

void
ANTTask::StartRun()
{
    if(EnableEyeOpenOrClose){
        myPhase = Phase::eyeOpenText;
    }
    else {
        myPhase = Phase::overallIntro;
    }
    mBlockInPhase = 0;
    mCurrentTrial = 0;

    State("PhaseNumber") = PhaseToInt(myPhase);
    State("TrialNumber") = 0;
    State("FirstFixationDuration") = 0;
    State("TargetVisible") = 0;
    State("ReactionTime") = 0;
    State("LastFixationDuration") = 0;

    mImgStimlsCenter->Conceal();
    mImgStimlsUp->Conceal();
    mImgStimlsDown->Conceal();
    myTextStimls->Conceal();
    introductTxtStimls->Conceal();
    correctTxtStimls->Conceal();
    correctTxtStimlsTop->Conceal();
    //rectBlack->Hide();
    if (mPhotoDiodePatch.pShape){
        mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.inactiveColor)
        .SetFillColor(mPhotoDiodePatch.inactiveColor);
    }

    //current cue
    cueCurrt = cueIdxVec[cueIdxArry[mCurrentTrial]];
    State("CueLocation") = cueCurrt;
    //current target
    targetCurrt = cueToTargetIdxMap[cueIdxArry[mCurrentTrial]];
    SetTargetState(targetCurrt);

    reactionTimeMs = 0.0;
    firstFixationMs = 0.0;
    firstFixationDurationB = 0;
    lastFixationB = 0;
    State("CueVisible") = 0;

    correctVector.clear();
    clockVecOpen.clear();
    clockVecClose.clear();
    Rt_no_cue_sum = 0.0;
    Rt_center_cue_sum = 0.0;
    Rt_spatial_cue_sum = 0.0;
    Rt_congr_sum = 0.0;
    Rt_incongr_sum = 0.0;
    Rt_no_cue_size = 0;
    Rt_center_cue_size = 0;
    Rt_spatial_cue_size = 0;
    Rt_congr_size = 0;
    Rt_incongr_size = 0;
    Rt_no_cue = 0.0;
    Rt_center_cue = 0.0;
    Rt_spatial_cue = 0.0;
    Rt_congr = 0.0;
    Rt_incongr = 0.0;

    //progress bar
    if (mpProgressBar)
    {
        mpProgressBar->SetTotal(total_trial_num).SetCurrent(0);
        mpProgressBar->SendDifferenceFrame();
    }

//#ifdef WIN32
//    ShowCursor(false);
//#endif // WIN32
}


void
ANTTask::Process( const GenericSignal& Input, GenericSignal& Output )
{
    //get mouse position
//#ifdef WIN32
//    POINT pt;
//    if(!GetCursorPos(&pt))
//        bcierr << "The cusor position cannot be retreived. Cannot move mouse" << endl;
//    else{
//        //hide the cursor when it is over the application window
//        if (pt.x > Parameter("WindowLeft") && pt.x < Parameter("WindowLeft") + Parameter("WindowWidth") &&
//            pt.y > Parameter("WindowTop") && pt.y < Parameter("WindowTop") + Parameter("WindowHeight")) {
//            //bciout << "Cursor is over the application window  " << mBlockInPhase << endl;            
//            //SetCursor(nullptr);   
//            ShowCursor(false);
//        }
//        else {
//            ShowCursor(true);
//        }
//    }
//#endif // WIN32

    mBlockInPhase++;
    //state
    State("PhaseNumber") = PhaseToInt(myPhase);
    State("BlockNum") = currtBlockSeqIdx + 1;
    int sumCorrect;
    float avrgCorrect;
    int segment_open = 1;
    int segment_close = 1;
    switch (myPhase)
    {
    case Phase::eyeOpenText:
        clockVecOpen.clear();
        SecondsToClock(floor(timerValueOpenB * mOneBlockMs / 1000), &clockVecOpen);

        if (mBlockInPhase == 1) {
            string totalTimerValueOpen = "";
            if (clockVecOpen[0] != "00") {
                totalTimerValueOpen = clockVecOpen[0].erase(0, clockVecOpen[0].find_first_not_of('0')) + " hours ";
            }
            if (clockVecOpen[1] != "00") {
                totalTimerValueOpen = clockVecOpen[1].erase(0, clockVecOpen[1].find_first_not_of('0')) + " minutes ";
            }
            if (clockVecOpen[2] != "00") {
                totalTimerValueOpen = clockVecOpen[2].erase(0, clockVecOpen[2].find_first_not_of('0')) + " seconds ";
            }
            introductTxtStimls->SetText("Please be still and keep your eyes open for the next " + totalTimerValueOpen + ".");
            introductTxtStimls->Present();
        }
        else if (mBlockInPhase >= eyeOpenTextB) {
            myPhase = Phase::timerShowOpen;
            mBlockInPhase = 0;
            //introductTxtStimls->SetText(clockVecOpen[0] + " : " + clockVecOpen[1] + " : " + clockVecOpen[2]);
            AppLog << "0%" << endl;
        }
        break;
    case Phase::timerShowOpen:
        //clockVecOpen.clear();
        //SecondsToClock(floor(timerValueOpenB * mOneBlockMs / 1000) - floor(mBlockInPhase * mOneBlockMs / 1000), &clockVecOpen);
        //
        //if(pre_seconds.compare(clockVecOpen[2])){
        //  //introductTxtStimls->SetText(clockVecOpen[0] + " : " + clockVecOpen[1] + " : " + clockVecOpen[2]);
        //    AppLog << clockVecOpen[0] + " : " + clockVecOpen[1] + " : " + clockVecOpen[2] << endl;
        //}  
        //pre_seconds = clockVecOpen[2];

        segment_open = floor(timerValueOpenB * segment_progress);
        if (mBlockInPhase % segment_open == 0 && mBlockInPhase <= timerValueOpenB) {
            string s_open = std::to_string(std::floor(mBlockInPhase / segment_open * segment_progress * 100));
            s_open = s_open.substr(0, s_open.find("."));
            AppLog << s_open << "%" << endl;
        }

        if (mBlockInPhase == timerValueOpenB) {
            AppLog << "100% done!" << endl;
        }

        if (mBlockInPhase >= (timerValueOpenB + halfsecondB)) {
            myPhase = Phase::eyeOpenOutText;
            mBlockInPhase = 0;
            //pre_seconds = "";
        }
        break;
    case Phase::eyeOpenOutText:
        if (mBlockInPhase == 1) {
            introductTxtStimls->SetText("Good job!");
        }
        else if (mBlockInPhase >= eyeOpenTextB) {
            myPhase = Phase::eyeCloseText;
            mBlockInPhase = 0;
        }
        break;
    case Phase::eyeCloseText:
        clockVecClose.clear();
        SecondsToClock(floor(timerValueCloseB * mOneBlockMs / 1000), &clockVecClose);

        if (mBlockInPhase == 1) {
            string totalTimerValue = "";
            if (clockVecClose[0] != "00") {
                totalTimerValue  = clockVecClose[0].erase(0, clockVecClose[0].find_first_not_of('0')) + " hours ";
            }
            if (clockVecClose[1] != "00") {
                totalTimerValue += clockVecClose[1].erase(0, clockVecClose[1].find_first_not_of('0')) + " minutes ";
            }
            if (clockVecClose[2] != "00") {
                totalTimerValue += clockVecClose[2].erase(0, clockVecClose[2].find_first_not_of('0')) + " seconds";
            }
            introductTxtStimls->SetText("Please be still and keep your eyes close for the next " + totalTimerValue + ".");
            introductTxtStimls->Present();
        }
        else if (mBlockInPhase >= eyeCloseTextB) {
            myPhase = Phase::timerShowClose;
            mBlockInPhase = 0;
            //introductTxtStimls->SetText(clockVecClose[0] + " : " + clockVecClose[1] + " : " + clockVecClose[2]);
            AppLog << "0%" << endl;
        }
        break;
    case Phase::timerShowClose:
        //clockVecClose.clear();
        //SecondsToClock(floor(timerValueCloseB * mOneBlockMs / 1000) - floor(mBlockInPhase * mOneBlockMs / 1000), &clockVecClose);

        //if (pre_seconds.compare(clockVecClose[2])) {
        //    //introductTxtStimls->SetText(clockVecClose[0] + " : " + clockVecClose[1] + " : " + clockVecClose[2]);
        //    AppLog << clockVecClose[0] + " : " + clockVecClose[1] + " : " + clockVecClose[2] << endl;
        //}
        //pre_seconds = clockVecClose[2];
        
        segment_close = floor(timerValueCloseB * segment_progress);
        if (mBlockInPhase % segment_close == 0 && mBlockInPhase <= timerValueCloseB) {
            string s_close = std::to_string(std::floor(mBlockInPhase / segment_close * segment_progress * 100));
            s_close = s_close.substr(0, s_close.find("."));
            AppLog << s_close << "%" << endl;
        }

        if (mBlockInPhase == timerValueCloseB) {
            AppLog << "100% done!" << endl;
        }

        if (mBlockInPhase >= (timerValueCloseB + halfsecondB)) {
            myPhase = Phase::timmUpText;
            mBlockInPhase = 0;
            //pre_seconds = "";
        }
        break;
    case Phase::timmUpText:
        if(mBlockInPhase == 1){
          introductTxtStimls->SetText("Good job! \n\nWhen you are ready, please press SPACE key to start the session.");
        }
        if (checkKeySpace()) {
            myPhase = Phase::overallIntro;
            mBlockInPhase = 0; 
        }
        break;
    case Phase::overallIntro:
        if(mBlockInPhase == 1){
          introductTxtStimls->SetText("INSTRUCTION\n\n"
              "This is an experiment investigating attention. You will be shown an arrow on the screen pointing either to the left or to the right, \n"
              "for example -> or <- . On some trials, the arrow will be flanked by two arrows to the left and two arrows to the right, for example:\n->->->->->\nor\n->-><-->->\n"
              "Your task is to respond to the direction of the CENTRAL arrow. You should press the left mouse button with your left thumb if the \n"
              "central arrow points to the left or press the right mouse button with your right thumb if the central arrow points to the right.\n\n"
              "Please make your response as quickly and accurately as possible. Your reaction time and accuracy will be recorded.\n\n"
              "There will be a cross (\" + \") in the center of the screen and the arrows will appear either above or below the cross. You should \n"
              "try to fixate on the cross throughout the experiment.\n\n "
              "On some trials there will be asterisk cues indicating when or where the arrow will occur. If the cue is at the center or both above\n"
              "and below fixation it indicates that the arrow will appear shortly. If the cue is only above or below fixation it indicates both that\n"
              "the trial will occur shortly and where it will occur. Try to maintain fixation at all times.  However, you may attend when and where \n"
              "indicated by the cues.\n\n"
              "The experiment contains four blocks. The first block is for practice and takes about two minutes. The other three blocks are experimental\n"
              "blocks and each takes about five minutes. After each block there will be a message \"take a break\" and you may take a short rest.\n"
              "After it, you can press the space bar to begin the next block. The whole experiment takes about twenty minutes.\n\n"
              "If you have any question, please ask the experimenter.\n\n"
              "Press SPACE bar to start the practice session.");
          introductTxtStimls->Present();
        }
        if (checkKeySpace()) {
            myPhase = Phase::fistFixation;
            mBlockInPhase = 0;
            introductTxtStimls->Conceal();
            mImgStimlsCenter->Present();
        }
        break;
    case Phase::fistFixation:
        //fist block in phase fistFixation
        if (mBlockInPhase == 1) {
            mCurrentTrial++;//phase fistFixation is the begin of each trial
            State("TrialNumber") = mCurrentTrial;
            AppLog << "=================================" << endl;
            AppLog << "Block #" << currtBlockSeqIdx + 1 << "/" << trialSequen.size() << endl;
            AppLog << "Trial #" << mCurrentTrial << "/" << TrialNum << endl;
            int temp = genUniformRandom(FirstFixationDurationMinMS, FirstFixationDurationMaxMS);
            firstFixationDurationB = floor(MeasurementUnits::TimeInSampleBlocks(std::to_string(temp) + "ms"));//trans time to number of block
            State("FirstFixationDuration") = firstFixationDurationB * mOneBlockMs * 1000;//us(microsecond)        
            firstFixationMs = temp;           
            //show icon
            mImgStimlsCenter->Present();
        }
        else if(mBlockInPhase >= firstFixationDurationB)
        {
            myPhase = Phase::cue;
            mBlockInPhase = 0;
            //reset
            State("FirstFixationDuration") = 0;
        }
        break;
    case Phase::cue:
        if (mBlockInPhase == 1) {
            cueShow(intToCue(cueCurrt));
            State("CueVisible") = 1;
            if (State("CueLocation") != 4 && State("CueLocation") != 0) {
                //rectBlack->Show();
                if (mPhotoDiodePatch.pShape) {
                    mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.activeColor)
                        .SetFillColor(mPhotoDiodePatch.activeColor);
                }
            }
        }
        else if (mBlockInPhase >= CueDurationB) {
            myPhase = Phase::middleFixation;
            mBlockInPhase = 0;
            mImgStimlsDown->Conceal();
            mImgStimlsUp->Conceal();
            //rectBlack->Hide();
            if (mPhotoDiodePatch.pShape) {
                mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.inactiveColor)
                    .SetFillColor(mPhotoDiodePatch.inactiveColor);
            }
            State("CueVisible") = 0;
        }
        break;
    case Phase::middleFixation:
        //show cross
        if(mBlockInPhase == 1){
          instantIcon(mImgStimlsCenter, cueIconVect[1], mImgRectCenter);
        }
        if (mBlockInPhase >= MiddleFixationDurationB) {
            myPhase = Phase::target;
            mBlockInPhase = 0;
        }
        break;
    case Phase::target:
        if (mBlockInPhase == 1) {
            targetShow(intToTarget(targetCurrt));
            State("TargetVisible") = 1;
            //rectBlack->Show();
            if (mPhotoDiodePatch.pShape) {
                mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.activeColor)
                    .SetFillColor(mPhotoDiodePatch.activeColor);
            }
        }
        else if (mBlockInPhase < ReactionTimeMaxB && mBlockInPhase >= ReactionTimeMinB) {
            int keyValue = -1;
            int keyInSampleIndex = checkKeyPress(keyValue);
            if (keyInSampleIndex > 0) {
                reactionTimeMs = (mOneBlockMs * (mBlockInPhase-2) ) + ( ( (float)keyInSampleIndex / (float)(mSampleRate) ) * 1000);
                AppLog << reactionTimeMs << " ms" << "   Response Time" << endl;
                bool objectPress = ((targetCurrt % 2 == 1 && (keyValue == 1)) || (targetCurrt % 2 == 0 && (keyValue == 2)));
                if (objectPress){                    
                    AppLog << "Correct!" << endl;
                    correctVector.push_back(1);
                }else {
                    AppLog << "Incorrect" << endl;
                    correctVector.push_back(0);
                }
                //float reactionBlockTimeMs = mOneBlockMs * mBlockInPhase;
                State("ReactionTime") = reactionTimeMs * 1000; //us(microsecond) 

                //caculate the last Fixation duration
                lastFixationB = TrialDurationB - firstFixationDurationB - CueDurationB - MiddleFixationDurationB - mBlockInPhase;

                //prepare for the next phase
                if(practiceMode){                   
                    myPhase = Phase::performText;
                }
                else {
                    myPhase = Phase::lastFixtion;
                }
                mBlockInPhase = 0;
                mImgStimlsDown->Conceal();
                mImgStimlsUp->Conceal();
                State("TargetVisible") = 0;

                //caculator average correct
                sumCorrect = std::accumulate(correctVector.begin(), correctVector.end(),
                    decltype(correctVector)::value_type(0));
                avrgCorrect = (float)sumCorrect / (float)correctVector.size();
                avrgCorrect = roundf(avrgCorrect * 10000) / 10000;
                AppLog << avrgCorrect * 100 << "%    Average Percent Correct" << endl;           
               
                //caculator average reaction time by trial type
                //float Rt_temp;
                switch (cueCurrt)
                {
                case 1:
                    Rt_spatial_cue_sum += reactionTimeMs;
                    Rt_spatial_cue_size++;

                    Rt_spatial_cue = Rt_spatial_cue_sum / (float)Rt_spatial_cue_size;
                    Rt_spatial_cue = roundf(Rt_spatial_cue * 100) / 100;
                    //AppLog << Rt_spatial_cue << "ms   Average Spatial Cue Response time" << endl;
                    break;
                case 2:
                    Rt_spatial_cue_sum += reactionTimeMs;
                    Rt_spatial_cue_size++;

                    Rt_spatial_cue = Rt_spatial_cue_sum / (float)Rt_spatial_cue_size;
                    Rt_spatial_cue = roundf(Rt_spatial_cue * 100) / 100;
                    //AppLog << Rt_spatial_cue << "ms   Average Spatial Cue Response time" << endl;
                    break;
                case 3:
                    Rt_center_cue_sum += reactionTimeMs;
                    Rt_center_cue_size++;

                    Rt_center_cue = Rt_center_cue_sum / (float)Rt_center_cue_size;
                    Rt_center_cue = roundf(Rt_center_cue * 100) / 100;
                    //AppLog << Rt_center_cue << "ms   Average Center Cue Response time" << endl;
                    break;
                case 4:
                    Rt_no_cue_sum += reactionTimeMs;
                    Rt_no_cue_size++;

                    Rt_no_cue = Rt_no_cue_sum / (float)Rt_no_cue_size;
                    Rt_no_cue = roundf(Rt_no_cue * 100) / 100;
                    //AppLog << Rt_no_cue << "ms   Average No Cue Response time" << endl;
                    break;
                default:
                    break;
                }

                AppLog << Rt_spatial_cue << "ms   Average Spatial Cue Response time" << endl;
                AppLog << Rt_center_cue << "ms   Average Center Cue Response time" << endl;
                AppLog << Rt_no_cue << "ms   Average No Cue Response time" << endl;

                if (targetCurrt == 1 || targetCurrt == 2 || targetCurrt == 5 || targetCurrt == 6) {
                    Rt_congr_sum += reactionTimeMs;
                    Rt_congr_size++;

                    Rt_congr = Rt_congr_sum / (float)Rt_congr_size;
                    Rt_congr = roundf(Rt_congr * 100) / 100;
                    //AppLog << Rt_congr << "ms   Average Congruent Response time" << endl;
                }
                else if (targetCurrt == 3 || targetCurrt == 4 || targetCurrt == 7 || targetCurrt == 8) {
                    Rt_incongr_sum += reactionTimeMs;
                    Rt_incongr_size++;

                    Rt_incongr = Rt_incongr_sum / (float)Rt_incongr_size;
                    Rt_incongr = roundf(Rt_incongr * 100) / 100;
                    //AppLog << Rt_incongr << "ms   Average Incongruent Response time" << endl;
                }

                AppLog << Rt_congr << "ms   Average Congruent Response time" << endl;
                AppLog << Rt_incongr << "ms   Average Incongruent Response time" << endl;

                if (practiceMode) {
                    string temp, tempTop;
                    string s = std::to_string(avrgCorrect * 100);
                    string st = std::to_string(roundf(reactionTimeMs * 100) / 100);

                    s = s.substr(0, s.find("."));
                    st = st.substr(0, st.find("."));                 

                    if (objectPress) {
                        tempTop = "Correct!";
                        temp = st + " ms" + "   Response Time\n\n" +
                            s + "%   Average Percent Correct";
                        correctTxtStimlsTop->SetTextColor(RGBColor::Blue);
                    }
                    else {
                        tempTop = "Incorrect!";
                        temp = st + " ms" + "   Response Time\n\n" +
                            s + "%   Average Percent Correct";
                        correctTxtStimlsTop->SetTextColor(RGBColor::Red);
                    }  
                    correctTxtStimls->SetText(temp);
                    correctTxtStimls->Present();
                    correctTxtStimlsTop->SetText(tempTop);
                    correctTxtStimlsTop->Present();
                }
            }
        }
        else if (mBlockInPhase >= ReactionTimeMaxB) {//miss keypress
            if (practiceMode) {
                if (mBlockInPhase < ReactionTimeMaxB + practiseB) {
                    //show warn
                    myTextStimls->SetText("No response detected!");
                    myTextStimls->Present();
                    mImgStimlsUp->Conceal();
                    mImgStimlsDown->Conceal();
                }
                else
                {
                    //practiseB time out, hide warn
                    myTextStimls->Conceal();
                    //prepare for the next phase
                    myPhase = Phase::lastFixtion;
                    mBlockInPhase = 0;
                    mImgStimlsDown->Conceal();
                    mImgStimlsUp->Conceal();
                    State("TargetVisible") = 0;
                    //caculate the last Fixation duration
                    lastFixationB = TrialDurationB - firstFixationDurationB - CueDurationB - MiddleFixationDurationB - ReactionTimeMaxB;

                    correctVector.push_back(0);

                    //caculator average correct
                    sumCorrect = std::accumulate(correctVector.begin(), correctVector.end(),
                        decltype(correctVector)::value_type(0));
                    avrgCorrect = (float)sumCorrect / (float)correctVector.size();
                    AppLog << std::fixed << std::setprecision(2) << avrgCorrect * 100 << "%   Average Percent Correct" << endl;
                }
            }else{
                //prepare for the next phase
                myPhase = Phase::lastFixtion;
                mBlockInPhase = 0;
                mImgStimlsDown->Conceal();
                mImgStimlsUp->Conceal();
                State("TargetVisible") = 0;
                //caculate the last Fixation duration
                lastFixationB = TrialDurationB - firstFixationDurationB - CueDurationB - MiddleFixationDurationB - ReactionTimeMaxB;

                correctVector.push_back(0);

                //caculator average correct
                sumCorrect = std::accumulate(correctVector.begin(), correctVector.end(),
                    decltype(correctVector)::value_type(0));
                avrgCorrect = (float)sumCorrect / (float)correctVector.size();
                AppLog << std::fixed << std::setprecision(2) << avrgCorrect * 100 << "%   Average Percent Correct" << endl;
            }            
        }
        break;
    case Phase::performText:
        if (mBlockInPhase >= practiseBC) {
            correctTxtStimls->Conceal();
            correctTxtStimlsTop->Conceal();
            myPhase = Phase::lastFixtion;
            mBlockInPhase = 0;
            //rectBlack->Hide();
            if (mPhotoDiodePatch.pShape) {
                mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.inactiveColor)
                    .SetFillColor(mPhotoDiodePatch.inactiveColor);
            }
        }
        break;
    case Phase::lastFixtion:
        if (mBlockInPhase == 1) {
            mImgStimlsDown->Conceal();
            mImgStimlsUp->Conceal(); 
            correctTxtStimls->Conceal();
            correctTxtStimlsTop->Conceal();
            //rectBlack->Hide();
            if (mPhotoDiodePatch.pShape) {
                mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.inactiveColor)
                    .SetFillColor(mPhotoDiodePatch.inactiveColor);
            }
            State("LastFixationDuration") = lastFixationB * mOneBlockMs * 1000; //us(microsecond) 
        }
        else if (mBlockInPhase >= lastFixationB) {
            //reset trail parameters
            State("FirstFixationDuration") = 0;
            State("TargetVisible") = 0;
            State("ReactionTime") = 0;
            State("LastFixationDuration") = 0;
            State("CueVisible") = 0;
            firstFixationMs = 0.0;
            reactionTimeMs = 0.0;
            firstFixationDurationB = 0;
            lastFixationB = 0; 
            //update the progress bar
            if (mpProgressBar)
            {
                Assert(mpProgressBar->Current() < mpProgressBar->Total());
                int current = mpProgressBar->Current();
                mpProgressBar->SetCurrent(current + 1);
                mpProgressBar->SendDifferenceFrame();
            }
            //stop the current block
            if (mCurrentTrial >= TrialNum) {
                myPhase = Phase::introduction;
                mBlockInPhase = 0;
                State("TrialNumber") = 0;
            }
            else {
                myPhase = Phase::fistFixation;
                mBlockInPhase = 0;
                //current cue
                cueCurrt = cueIdxVec[cueIdxArry[mCurrentTrial]];
                State("CueLocation") = cueCurrt;
                //current target
                targetCurrt = cueToTargetIdxMap[cueIdxArry[mCurrentTrial]];
                SetTargetState(targetCurrt);
            }
        }

        break;
    case Phase::introduction:
        practiceMode = 0;
        if (mBlockInPhase == 1){
            mImgStimlsCenter->Conceal();
            switch (currtBlockSeqIdx)
            {
            case -1:
                introductTxtStimls->SetText("You have completed the practice trials.\nNow the experiment can be started. There are three sessions in this "
                    "experiment.\nEach session takes about 5 minutes.Totally, you just need to spend 15 minutes.\n\nWhen you are ready, please press SPACE key to start the first session.");
                introductTxtStimls->Present();
                break;
            case 0:
                introductTxtStimls->SetText("You have finished session 1.\n\nPlease take a rest.\n\n\nTo start next session, please press SPACE key.");
                introductTxtStimls->Present();
                break;
            case 1:
                introductTxtStimls->SetText("You have finished session 2.\n\nPlease take a rest.\n\n\nTo start next session, please press SPACE key.");
                introductTxtStimls->Present();
                break;
            case 2:
                introductTxtStimls->SetText("You have finished the whole experiment(three sessions).\n\nPlease call the experimenter.\n\n\nThank you very much and\nGoodbye!");
                introductTxtStimls->Present();
                break;
            default:
                break;
            }
        }
        else if (checkKeySpace()) {
            //next Block
            if(currtBlockSeqIdx != (trialSequen.size() - 1)){
                TrialNum = trialSequen[++currtBlockSeqIdx];
                myPhase = Phase::fistFixation;
                mBlockInPhase = 0;
                mCurrentTrial = 0;                

                State("PhaseNumber") = PhaseToInt(myPhase);
                State("TrialNumber") = 0;
                State("FirstFixationDuration") = 0;
                State("TargetVisible") = 0;
                State("ReactionTime") = 0;
                State("LastFixationDuration") = 0;
                State("CueVisible") = 0;

                mImgStimlsCenter->Present();
                mImgStimlsUp->Conceal();
                mImgStimlsDown->Conceal();
                myTextStimls->Conceal();
                introductTxtStimls->Conceal();
                correctTxtStimls->Conceal();
                correctTxtStimlsTop->Conceal();
                //rectBlack->Hide();
                if (mPhotoDiodePatch.pShape) {
                    mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.inactiveColor)
                        .SetFillColor(mPhotoDiodePatch.inactiveColor);
                }

                reactionTimeMs = 0.0;
                firstFixationMs = 0.0;
                firstFixationDurationB = 0;
                lastFixationB = 0;

                correctVector.clear();
                Rt_no_cue_sum = 0.0;
                Rt_center_cue_sum = 0.0;
                Rt_spatial_cue_sum = 0.0;
                Rt_congr_sum = 0.0;
                Rt_incongr_sum = 0.0;
                Rt_no_cue_size = 0;
                Rt_center_cue_size = 0;
                Rt_spatial_cue_size = 0;
                Rt_congr_size = 0;
                Rt_incongr_size = 0;
                Rt_no_cue = 0.0;
                Rt_center_cue = 0.0;
                Rt_spatial_cue = 0.0;
                Rt_congr = 0.0;
                Rt_incongr = 0.0;

                //shuffle cue and target
                cueIdxVec.clear();
                targetIdxVecU.clear();
                targetIdxVecD.clear();
                targetIdxVec.clear();
                cueIdxArry.clear();
                cueToTargetIdxMap.clear();
                //initial index of target
                for (int i = 0; i < TrialNum; i++) {
                    if (i < TrialNum / 8) {
                        targetIdxVecU.push_back(1);
                        continue;
                    }
                    else if (i < TrialNum / 8 * 2) {
                        targetIdxVecU.push_back(2);
                        continue;
                    }
                    else if (i < TrialNum / 8 * 3) {
                        targetIdxVecU.push_back(3);
                        continue;
                    }
                    else if (i < TrialNum / 8 * 4) {
                        targetIdxVecU.push_back(4);
                        continue;
                    }
                    else if (i < TrialNum / 8 * 5) {
                        targetIdxVecD.push_back(5);
                        continue;
                    }
                    else if (i < TrialNum / 8 * 6) {
                        targetIdxVecD.push_back(6);
                        continue;
                    }
                    else if (i < TrialNum / 8 * 7) {
                        targetIdxVecD.push_back(7);
                        continue;
                    }
                    else if (i < TrialNum) {
                        targetIdxVecD.push_back(8);
                        continue;
                    }
                }



                //initial index of cue  & associate cue idx with target value
                for (int i = 0; i < TrialNum / 6; i++) {
                    cueIdxVec.push_back(1);
                    cueIdxArry.push_back(i);
                    int temp = genUniformRandom(0, targetIdxVecD.size() - 1);
                    cueToTargetIdxMap[i] = targetIdxVecD[temp];
                    targetIdxVecD.erase(targetIdxVecD.begin() + temp);
                }
                for (int i = TrialNum / 6; i < TrialNum / 3; i++) {
                    cueIdxVec.push_back(2);
                    cueIdxArry.push_back(i);
                    int temp = genUniformRandom(0, targetIdxVecU.size() - 1);
                    cueToTargetIdxMap[i] = targetIdxVecU[temp];
                    targetIdxVecU.erase(targetIdxVecU.begin() + temp);
                }
                for (int i = TrialNum / 3; i < TrialNum / 3 * 2; i++) {
                    cueIdxVec.push_back(3);
                    cueIdxArry.push_back(i);
                }
                for (int i = TrialNum / 3 * 2; i < TrialNum; i++) {
                    cueIdxVec.push_back(4);
                    cueIdxArry.push_back(i);
                }

                //concat two vector
                targetIdxVec.reserve(targetIdxVecU.size() + targetIdxVecD.size()); // preallocate memory
                targetIdxVec.insert(targetIdxVec.end(), targetIdxVecU.begin(), targetIdxVecU.end());
                targetIdxVec.insert(targetIdxVec.end(), targetIdxVecD.begin(), targetIdxVecD.end());

                for (int i = TrialNum / 3; i < TrialNum; i++) {
                    int temp = genUniformRandom(0, targetIdxVec.size() - 1);
                    cueToTargetIdxMap[i] = targetIdxVec[temp];
                    targetIdxVec.erase(targetIdxVec.begin() + temp);
                }

                //shuffle cue
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                shuffle(cueIdxArry.begin(), cueIdxArry.end(), std::default_random_engine(seed));

                // show map content:
                for (int i = 0; i < TrialNum; i++) {
                    //bciout << cueIdxVec[cueIdxArry[i]] << " => " << cueToTargetIdxMap[cueIdxArry[i]] << endl;
                    //check the map
                    if (cueIdxVec[cueIdxArry[i]] == 2 && cueToTargetIdxMap[cueIdxArry[i]] > 4) {
                        bcierr << "cue shuffle wrong!! " << endl;
                    }
                    if (cueIdxVec[cueIdxArry[i]] == 1 && cueToTargetIdxMap[cueIdxArry[i]] <= 4) {
                        bcierr << "cue shuffle wrong!! " << endl;
                    }
                }
                //end

                //current cue
                cueCurrt = cueIdxVec[cueIdxArry[mCurrentTrial]];
                State("CueLocation") = cueCurrt;
                //current target
                targetCurrt = cueToTargetIdxMap[cueIdxArry[mCurrentTrial]];
                SetTargetState(targetCurrt);
            }
            else {
                State("Running") = 0;
            }
        }
        break;
    case Phase::none:
            State("Running") = 0;
        break;
    default:
        break;
    }
        
    Output = Input;
}

void
ANTTask::StopRun()
{
    myPhase = Phase::none;
    mImgStimlsCenter->Conceal();
    mImgStimlsUp->Conceal();
    mImgStimlsDown->Conceal();
    myTextStimls->Conceal();
    introductTxtStimls->Conceal();
    correctTxtStimls->Conceal();
    correctTxtStimlsTop->Conceal();
    //rectBlack->Hide();
    if (mPhotoDiodePatch.pShape) {
        mPhotoDiodePatch.pShape->SetColor(mPhotoDiodePatch.inactiveColor)
            .SetFillColor(mPhotoDiodePatch.inactiveColor);
    }
    reactionTimeMs = 0.0;
    firstFixationMs = 0.0;
    cueCurrt = 0;
    State("CueLocation") = cueCurrt;
    targetCurrt = 0;
    SetTargetState(targetCurrt);
    mBlockInPhase = 0;
    mCurrentTrial = 0;
    firstFixationDurationB = 0;
    lastFixationB = 0;
    correctVector.clear();
    Rt_no_cue_sum = 0.0;
    Rt_center_cue_sum = 0.0;
    Rt_spatial_cue_sum = 0.0;
    Rt_congr_sum = 0.0;
    Rt_incongr_sum = 0.0;
    Rt_no_cue_size = 0;
    Rt_center_cue_size = 0;
    Rt_spatial_cue_size = 0;
    Rt_congr_size = 0;
    Rt_incongr_size = 0;
    Rt_no_cue = 0.0;
    Rt_center_cue = 0.0;
    Rt_spatial_cue = 0.0;
    Rt_congr = 0.0;
    Rt_incongr = 0.0;

    State("PhaseNumber") = 0;
    State("TrialNumber") = 0;
    State("FirstFixationDuration") = 0;
    State("CueVisible") = 0;
    State("TargetVisible") = 0;
    State("ReactionTime") = 0;
    State("LastFixationDuration") = 0;

    clockVecOpen.clear();
    clockVecClose.clear();
}

void
ANTTask::Halt()
{
}

map<string, string> 
ANTTask::initStimulus() {
    map<string, string> stimulusMap;
    string cue, target;

    cue = "2 ../tasks/ANTTask/resource/icons/ANT_cue_asterisk.png ../tasks/ANTTask/resource/icons/ANT_cursor.png";
  /*  target = "4 ../resource/icons/ANT_target_congruent_left.png ../resource/icons/ANT_target_congruent_right.png "
        "../resource/icons/ANT_target_incongruent_left.png ../resource/icons/ANT_target_incongruent_right.png";*/

    stimulusMap["cue"] = cue;
    //stimulusMap["target"] = target;
   
    return stimulusMap;
}


/*
generate random num in [begin,end], uniform distribute
*/
int 
ANTTask::genUniformRandom(int begin, int end) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(begin, end);
   
    return distrib(gen);
}

void
ANTTask::instantIcon(ImageStimulus* icon, string path, GUI::Rect rect) {

    icon->SetFile(path);
    icon->SetDisplayRect(rect);
    icon->Present();
}

void
ANTTask::cueShow(CuePattern cueP) {

    switch (cueP)
    {
    case CuePattern:: cross:
        mImgStimlsDown->Conceal();
        mImgStimlsUp->Conceal();
        break;
    case CuePattern::asterisk:
        mImgStimlsDown->Conceal();
        mImgStimlsUp->Conceal();
        instantIcon(mImgStimlsCenter, cueIconVect[0], mImgRectCenter_a);
        break;
    case CuePattern::as_cro:
        mImgStimlsDown->Conceal();
        instantIcon(mImgStimlsUp, cueIconVect[0], mImgRectUp_a);
        break;
    case CuePattern::cro_as:
        mImgStimlsUp->Conceal();
        instantIcon(mImgStimlsDown, cueIconVect[0], mImgRectDown_a);
        break;
    case CuePattern::none:
        bcierr << "None, wrong cue pattern!";
        break;
    default:
        bcierr << "Default, wrong cue pattern!";
        break;
    }
}

ANTTask::TargetPattern
ANTTask::intToTarget(int i) {
    switch (i)
    {
    case 1:
        return TargetPattern::congruent_u_l;
    case 2:
        return TargetPattern::congruent_u_r;
    case 3:
        return TargetPattern::incongruent_u_l;
    case 4:
        return TargetPattern::incongruent_u_r;
    case 5:
        return TargetPattern::congruent_d_l;
    case 6:
        return TargetPattern::congruent_d_r;
    case 7:
        return TargetPattern::incongruent_d_l;
    case 8:
        return TargetPattern::incongruent_d_r;
    default:
        return TargetPattern::none;
        break;
    }
}

int 
ANTTask::targetShow(ANTTask::TargetPattern targetP) {
    int targetNum = 0;
      

    switch (targetP)
    {
    case ANTTask::TargetPattern::none:
        bcierr << "None,wrong target pattern!" << endl;
        break;
    case ANTTask::TargetPattern::congruent_u_l:
        mImgStimlsDown->Conceal();
        instantIcon(mImgStimlsUp, targetIconVect[0], mImgRectUp);
        targetNum = 1;
        break;
    case ANTTask::TargetPattern::congruent_u_r:
        mImgStimlsDown->Conceal();
        instantIcon(mImgStimlsUp, targetIconVect[3], mImgRectUp);
        targetNum = 2;
        break;
    case ANTTask::TargetPattern::incongruent_u_l:
        mImgStimlsDown->Conceal();
        instantIcon(mImgStimlsUp, targetIconVect[2], mImgRectUp);
        targetNum = 3;
        break;
    case ANTTask::TargetPattern::incongruent_u_r:
        mImgStimlsDown->Conceal();
        instantIcon(mImgStimlsUp, targetIconVect[1], mImgRectUp);
        targetNum = 4;
        break;
    case ANTTask::TargetPattern::congruent_d_l:
        mImgStimlsUp->Conceal();
        instantIcon(mImgStimlsDown, targetIconVect[0], mImgRectDown);
        targetNum = 5;
        break;
    case ANTTask::TargetPattern::congruent_d_r:
        mImgStimlsUp->Conceal();
        instantIcon(mImgStimlsDown, targetIconVect[3], mImgRectDown);
        targetNum = 6;
        break;
    case ANTTask::TargetPattern::incongruent_d_l:
        mImgStimlsUp->Conceal();
        instantIcon(mImgStimlsDown, targetIconVect[2], mImgRectDown);
        targetNum = 7;
        break;
    case ANTTask::TargetPattern::incongruent_d_r:
        mImgStimlsUp->Conceal();
        instantIcon(mImgStimlsDown, targetIconVect[1], mImgRectDown);
        targetNum = 8;
        break;
    default:
        bcierr << "Default,wrong target pattern!" << endl;
        break;
    }
    return targetNum;
}

ANTTask::CuePattern
ANTTask::intToCue(int i) {
    switch (i)
    {
    case 4:
        return CuePattern::cross;
    case 3:
        return CuePattern::asterisk;
    case 2:
        return CuePattern::as_cro;
    case 1:
        return CuePattern::cro_as;
    default:
        return CuePattern::none;
        break;
    }
}

int 
ANTTask::PhaseToInt(ANTTask::Phase p){
    switch (p)
    {
    case ANTTask::Phase::none:
        return 0;
        break;
    case ANTTask::Phase::fistFixation:
        return 1;
        break;
    case ANTTask::Phase::cue:
        return 2;
        break;
    case ANTTask::Phase::middleFixation:
        return 3;
        break;
    case ANTTask::Phase::target:
        return 4;
        break;
    case ANTTask::Phase::lastFixtion:
        return 5;
        break;
    case ANTTask::Phase::introduction:
        return 6;
        break;
    case ANTTask::Phase::performText:
        return 7;
        break;
    case ANTTask::Phase::eyeOpenText:
        return 8;
        break;
    case ANTTask::Phase::timerShowOpen:
        return 9;
        break;
    case ANTTask::Phase::eyeOpenOutText:
        return 10;
        break;
    case ANTTask::Phase::eyeCloseText:
        return 11;
        break;
    case ANTTask::Phase::timerShowClose:
        return 12;
        break;
    case ANTTask::Phase::timmUpText:
        return 13;
        break;
    case ANTTask::Phase::overallIntro:
        return 14;
        break;
    default:
        return 100;
        break;
    }
}

int
ANTTask::checkKeyPress(int& keyV) {
    for (unsigned int i = 0; i < mBlockSize; i++)
    {
        if (State("MouseKeys")(i) == 1 || State("MouseKeys")(i) == 2) {
            keyV = State("MouseKeys")(i);
            return i + 1;
        }
    }

    return -1;
}


bool
ANTTask::checkKeySpace() {
    for (unsigned int i = 0; i < mBlockSize; i++)
    {
        if (State("KeyDown")(i) == 32){
            return true;        
        }        
    }
    return false;
}

void 
ANTTask::SetTargetState(int targetCurrt) {
    switch (targetCurrt)
    {
    case 1:
        State("TargetLocation") = 2;
        State("TargetDirection") = 1;
        State("FlankerDirection") = 1;
        break;
    case 2:
        State("TargetLocation") = 2;
        State("TargetDirection") = 2;
        State("FlankerDirection") = 2;
        break;
    case 3:
        State("TargetLocation") = 2;
        State("TargetDirection") = 1;
        State("FlankerDirection") = 2;
        break;
    case 4:
        State("TargetLocation") = 2;
        State("TargetDirection") = 2;
        State("FlankerDirection") = 1;
        break;
    case 5:
        State("TargetLocation") = 1;
        State("TargetDirection") = 1;
        State("FlankerDirection") = 1;
        break;
    case 6:
        State("TargetLocation") = 1;
        State("TargetDirection") = 2;
        State("FlankerDirection") = 2;
        break;
    case 7:
        State("TargetLocation") = 1;
        State("TargetDirection") = 1;
        State("FlankerDirection") = 2;
        break;
    case 8:
        State("TargetLocation") = 1;
        State("TargetDirection") = 2;
        State("FlankerDirection") = 1;
        break;
    default:
        State("TargetDirection") = 0;
        State("FlankerDirection") = 0;
        State("TargetLocation") = 0;
        break;
    }
}

void
ANTTask::SecondsToClock(int totalT, vector<string> *time) {
    int seconds, hours, minutes;
    minutes = totalT / 60;
    hours = minutes / 60;
    minutes = minutes % 60;
    seconds = totalT % 60;

    if (hours < 10){
        string temp = "0" + std::to_string(hours);
        time->push_back(temp);
    }
    else {
        time->push_back(std::to_string(hours));
    }

    if (minutes < 10) {
        string temp = "0" + std::to_string(minutes);
        time->push_back(temp);
    }
    else {
        time->push_back(std::to_string(minutes));
    }

    if (seconds < 10) {
        string temp = "0" + std::to_string(seconds);
        time->push_back(temp);
    }
    else {
        time->push_back(std::to_string(seconds));
    }

}



