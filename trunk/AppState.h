/***********************************************************
 *
 * AppState.h
 *
 * Project:
 *
 * GlutGLSL
 *
 * Original Author:
 *
 * Matthew Jones, 2008
 *
 * Description:
 *
 * This file defines the variables representing the state
 * of the application.  This is an alternative method of
 * sharing data with classes (instead of using global
 * variables), by providing the class with a pointer to
 * this class.  Classes can also affect the application
 * state by changing these variables, as they are made
 * public.
 *
 ***********************************************************/


#ifndef APPSTATE__H
#define APPSTATE__H


#ifndef TRIALMAN__H
	#include "TrialMan.h"
#endif

#ifndef PRACTICETRIAL__H
	#include "PracticeTrial.h"
#endif

#ifndef BLOCKPAIR__H
	#include "TrialBlock.h"
#endif


#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <sys/stat.h>

#ifndef NO_HAPTICS
	// Header for hduVector3Dd declaration
	#include <HDU/hduMatrix.h>
#endif



// These are the global variables shared between other objects
class AppState
{
  public:
	AppState();
	~AppState();

  public:
	bool SaveState();
	bool RestoreState(TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier);
	bool FinaliseState();
	bool SaveExcel();
	bool WritePosition();

	static void TempConverter(double *vec3, char* text, int numchars);

  private:
	  
	bool RestoreState(TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier, int state);

	void ClearTrials();

	std::string GenerateFilename(int type);
	
	bool FileExists(std::string strFilename);

	void RandomBlockPermute(std::vector<TrialBlock*>::iterator it, int numpairs);

  public:
	static int SpecialParticipantNumber;

    bool LeftDown;
	bool FirstFrame;
	bool CancelGravity;
	int InitX;
	int InitY;
	int CurrentX;
	int CurrentY;
	int ParticipantNumber;
#ifndef NO_HAPTICS
	hduVector3Dd LastPosition;
	hduVector3Dd Anchor;
#endif

	unsigned int ScreenWidth;
	unsigned int ScreenHeight;

	float SliderGravForce;

	int ActiveShader;

	// These are the variables to save (for crash recovery)
  public:
	std::vector<PracticeTrial*> PracticeTrials;
	std::vector<Trial*> Trials;
	int CurrentTrial;
	bool InIntroTrial;
	int InPracticeTrial;
};


#endif