////////////////////////////////////////////////////////////////////////////////
// Authors: schalklab@HR18818.wucon.wustl.edu
// Description: ANTTask header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_ANTTASK_H  // makes sure this header is not included more than once
#define INCLUDED_ANTTASK_H

#include "ApplicationBase.h"
#include "TextStimulus.h"
#include "ImageStimulus.h"
#include "shapes.h"

using std::map; using std::string; using std::vector;

class ProgressBarVis;

class ANTTask : public ApplicationBase
{
 public:
  ANTTask();
  ~ANTTask();
  void Publish() override;
  void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
  void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
  void StartRun() override;
  void Process( const GenericSignal& Input, GenericSignal& Output ) override;
  void StopRun() override;
  void Halt() override;

 private:
	 enum class Phase
	 {
		 none,
		 fistFixation,
		 cue,
		 middleFixation,
		 target,
		 lastFixtion,
		 introduction,
		 performText,
		 eyeOpenText,
		 timerShowOpen,
		 eyeOpenOutText,
		 eyeCloseText,
		 timerShowClose,
		 timmUpText,
		 overallIntro
	 };
	 enum class CuePattern
	 {
		 none,
		 cro_as,
		 as_cro,
		 asterisk,
		 cross
		 
	 };
	 enum class TargetPattern
	 {
		 none,
		 congruent_u_l,
		 congruent_u_r,
		 incongruent_u_l,
		 incongruent_u_r,
		 congruent_d_l,
		 congruent_d_r,
		 incongruent_d_l,
		 incongruent_d_r,
	 };

	 Phase myPhase = Phase::none;
	 unsigned int TrialDurationB, firstFixationDurationB,  CueDurationB, MiddleFixationDurationB, ReactionTimeMinB, ReactionTimeMaxB, lastFixationB;
	 unsigned int eyeCloseTextB, timerValueCloseB, eyeOpenTextB, timerValueOpenB;
	 vector<string> clockVecClose, clockVecOpen;
	 //string pre_seconds = "";
	 float FirstFixationDurationMinMS, FirstFixationDurationMaxMS, firstFixationMs, reactionTimeMs;
	 int practiseB, practiseBC;
	 int halfsecondB;
	 vector<string> cueIconVect, targetIconVect;
	 int mBlockInPhase, mCurrentTrial, TrialNum;
	 //map<CuePattern, vector<TargetPattern>> cueTargetMap;
	 //QWidget* myWindow = new QWidget;
	 //QLabel* myIconU = new QLabel(myWindow);
	 //QLabel* myIconC = new QLabel(myWindow);
	 //QLabel* myIconD = new QLabel(myWindow);
	 //QLabel* myText  = new QLabel(myWindow);
	 //QVBoxLayout* layout = new QVBoxLayout(myWindow);
	 //unsigned int cueScale = 12, targetScale = 2;
	 unsigned int  mSampleRate, mBlockSize;
	 float mOneBlockMs;
	 int cueCurrt = 0;
	 int targetCurrt = 0;
	 bool practiceMode = false;
	 bool EnableEyeOpenOrClose = true;
	 double segment_progress = 0.05;
	 //average reaction time by trial type
	 float Rt_no_cue_sum, Rt_center_cue_sum, Rt_spatial_cue_sum, Rt_congr_sum, Rt_incongr_sum;
	 int Rt_no_cue_size, Rt_center_cue_size, Rt_spatial_cue_size, Rt_congr_size, Rt_incongr_size;
	 float Rt_no_cue, Rt_center_cue, Rt_spatial_cue, Rt_congr, Rt_incongr;

	 //GUI
	 ApplicationWindow& mrDisplay;
	 TextStimulus* myTextStimls;
	 TextStimulus* introductTxtStimls;
	 TextStimulus* correctTxtStimls;
	 TextStimulus* correctTxtStimlsTop;
	 ImageStimulus* mImgStimlsUp;
	 ImageStimulus* mImgStimlsCenter;
	 ImageStimulus* mImgStimlsDown;
	 GUI::Rect	 mImgRectUp, mImgRectCenter, mImgRectDown; //target
	 GUI::Rect	 mImgRectUp_a, mImgRectCenter_a, mImgRectDown_a;//cue
	 GUI::Rect	 mTextRect , mIntroTxtRect, mCorTxtRect, mCorTxtRectTop;
	 GUI::Rect   rectPatch;
	 //RectangularShape* rectBlack;
	 ProgressBarVis* mpProgressBar = nullptr;
	 int total_trial_num = 0;

	 // Photo diode patch
	 struct {
		 Shape* pShape = nullptr;
		 RGBColor activeColor, inactiveColor;
	 } mPhotoDiodePatch;

	 // Access to the Display property.
	 GUI::GraphDisplay& Display()
	 {
		 return mrDisplay;
	 }
	 const GUI::GraphDisplay& Display() const
	 {
		 return mrDisplay;
	 }

	 vector<int> correctVector;
	 vector<int> cueIdxVec, targetIdxVecU, targetIdxVecD, targetIdxVec;
	 vector<int> cueIdxArry;
	 vector<int> trialSequen;
	 int trialPractice;
	 int currtBlockSeqIdx;
	 map<int, int> cueToTargetIdxMap;


	 map<string, string> initStimulus();
	 int genUniformRandom(int begin, int end); // uniform distribution in range [begin, end]
	 void instantIcon(ImageStimulus* icon, string path, GUI::Rect rect);
	 void cueShow(CuePattern cueP);
	 int targetShow(TargetPattern targetP);
	 CuePattern intToCue(int i);
	 int PhaseToInt(Phase p);
	 int checkKeyPress(int& keyV);
	 TargetPattern intToTarget(int i);
	 bool checkKeySpace();
	 void SetTargetState(int targetCurrt);
	 void SecondsToClock(int totalT, vector<string>* time);
};

#endif // INCLUDED_ANTTASK_H
