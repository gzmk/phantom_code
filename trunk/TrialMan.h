/***********************************************************
 *
 * TrialMan.h
 *
 * Project:
 *
 * GlutGLSL
 *
 * Original Author:
 *
 * Matthew Jones, 2009
 *
 * Description:
 *
 * This file defines the variables representing each trial.
 * It also contains a definition of a class used to hold
 * all the parameters that define how a set of trials should
 * be constructed.  This last class also contains a utility
 * function for constructing a trial set based on the member
 * data.
 *
 ***********************************************************/


#ifndef TRIALMAN__H
#define TRIALMAN__H


// GL Headers
#include <GL/glew.h>
//#include <GL/wglew.h>
#include <GL/glut.h>

#include <math.h>
//#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <vector>
#include <float.h>
#include <windows.h>

class Trial;
typedef void(*TrialEndNotifier)(Trial*);
typedef void(*TrialStageEndNotifier)(Trial*);


// These are the variables per trial
class Trial
{
  public:

	Trial(int trialnum, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier);
	~Trial();

  public:

	virtual void Start();
	void Stop();
	int GetStage();
	void SetResult(double glossresult, double depthresult);
	virtual void Notify();
	TrialEndNotifier GetNotifier();
	virtual void Render();
	void RenderText(int screenwidth, int screenheight);
	virtual void NextStage();
	bool GetBetweenStages();
	double GetRadius(double* proxypos);
	bool IsWaiting();
	GLuint GetImage();
	double GetTimer();
	int GetTrialNum();
	void SetTrialNum(int trial);
	bool GetStarted();
	void SetStarted(bool started);
	virtual void LoadImages();

  protected:

	static GLuint LoadTexture(std::string filename);

    static VOID CALLBACK TimerProc(HWND hwnd,
				UINT uMsg,
				UINT_PTR idEvent,
				DWORD dwTime);

  public:

	static const double ZoneRad;
	static const double ZoneOffset;
	static const int NumValues = 9;//9; EWG
	static const int NumEnvironments = 2;//EWG

	static Trial* CurrentTrial;
	static LARGE_INTEGER LastTicks;

	static UINT TimerID;
	static int RefreshTime;
	static int StageTime;
	static bool OutsideZone;
	
	static int ParticipantNumber;
	static std::string ImageFilename;

	static float StiffnessValues[Trial::NumValues];
	static float DampingValues[Trial::NumValues];
	static float StaticFrictionValues[Trial::NumValues];
	static float DynamicFrictionValues[Trial::NumValues];

	// Visual presence is whether or not any images are rendered
	bool VisualPresence;
	// Haptic presence is whether or not any haptics are rendered
	bool HapticPresence;
	// Environment is the light field used to render the object
	int Environment;
	// Material parameters
	int Glossiness;
	int Depth;
	int Conflict;
	static const int Compliance = 0;	// fix compliance and friction values for these trials.  
	static const int Friction = 0;		// Friction and Compliance arrays (TrialMan.cpp) only have 1 element currently
	// Timers: Float time values which control the random potato drawn
	// Different values for each stage
	double Timer1;
	// The block number (changes with change from haptic to visual, and repetitions)
	int Block;
	// GlossResult: Participant's response for the perceived glossiness
	double GlossResult;
	// DepthResult: Participant's response for the perceived depth
	double DepthResult;
	// Instruction Text
	char Text[100];
	// Whether this is an instruction page or actual trial
	bool InstructionTrial;
	// Whether this is a practice trial or a main trial
	bool IsPracticeTrial;

	double LastRadius;
	bool StartedGravity;

	// Debugging variables to help checking trial params match output
	std::string _imageFile1;

  protected:
	
	void SetOrthographicProjection(int screenwidth, int screenheight);
	void ResetPerspectiveProjection();

  protected:

	int _stage;
	bool _betweenStages;
	bool _firstResponse; // To allow only first response to be counted
	bool _started;
	bool _waiting; // Only set to true when instruction trial receives a key or mouse press
	int _trialNum;
	// Images - indexes to OpenGL textures for each stage
    GLuint _image1;
	TrialEndNotifier _notifier;
	TrialStageEndNotifier _stageNotifier;
};


// These are the variables governing the creation of all trials
class TrialSetupParams
{
  public:

	  TrialSetupParams();
	  ~TrialSetupParams();

  public:

	  // Utility functions for constructing trial sets
	  std::vector<Trial*> RandomiseTrials(int numtrials, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier, int block);
	  std::vector<Trial*> GenerateAllTrials(int trialoffsetnum, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier, bool hapticsenabled, bool visualenabled, int block);
	  Trial* GenerateInstructionTrial(int trialnum, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier, std::string text, int block);

	  // Private utility function to randomise order of trials
	  static void RandomPermute(std::vector<Trial*>::iterator it, int numtrials);

  private:
		// Private utility function to check the values are valid
	  void CheckVals();

  public:
	  float MinTimer;
	  float MaxTimer;

	  static int DTimer;
};


#endif