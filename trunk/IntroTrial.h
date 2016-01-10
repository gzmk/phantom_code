/***********************************************************
 *
 * IntroTrial.h
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


#ifndef INTROTRIAL__H
#define INTROTRIAL__H


#ifndef NO_HAPTICS
	#ifndef HAPTICS__H
		#include "Haptics.h"
	#endif
#else
	#include "Globals.h"
#endif

// GL Headers
#include <GL/glew.h>
//#include <GL/wglew.h>
#include <GL/glut.h>

#include <math.h>
//#include <stdlib.h>
#include <time.h>
#include <vector>
#include <windows.h>

class IntroTrial;
typedef void(*IntroTrialEndNotifier)(IntroTrial*);


// These are the variables for the intro trial
class IntroTrial
{
  public:
	IntroTrial(IntroTrialEndNotifier notifier);
	~IntroTrial();

  public:
	void Start();
	void Stop();
    int GetStage();
    void SetStage(int stage);
	bool InZone();
	void Notify();

	void OnKeyPress(unsigned char key);
	int GetParticipantNumber();

	void Render(bool righteye);
	void RenderSurface();
	void RenderBoundingBox(bool flip);

  private:
    static VOID CALLBACK TimerProc(HWND hwnd,
				UINT uMsg,
				UINT_PTR idEvent,
				DWORD dwTime);

	void BuildSurface();
	void BuildBoundingBox();

	void RenderText();

	void ZoneEntered();
	void ZoneLeft();
	bool CalcZoneTime();

	void AddToNumber(int n, int &numref);
	void RemoveFromNumber(int &numref);

	void SetOrthographicProjection();
	void ResetPerspectiveProjection();

  public:

	static IntroTrial* CurrentTrial;
	static UINT TimerID;

	static const float HSize;
	static const float VSize;
	static const float DSize;
	static const float HSizeBox;
	static const float VSizeBox;
	static const float DSizeBox;
	static const double ZoneRad;
	static const double ZoneOffset;
	static const int RefreshPeriod;

	// Test Time is the length of time, in ms, the initial test image is shown
	int TestTime;
	// Zone Time is the length of time, in ms, the subject must be within the start zone to end trial
	int ZoneTime;
	
	// Results
	
	// Trial Time is the total time spent on this intro trial
	int TrialTime;

  private:

	bool _inZone;
	int _stage;
	int _numVerts;
	int _numVertsBox;
	float* _verts;
	float* _vertsBox;
	int _participantNumber;
	LARGE_INTEGER _lastZoneTicks;
    IntroTrialEndNotifier _notifier;
};


#endif