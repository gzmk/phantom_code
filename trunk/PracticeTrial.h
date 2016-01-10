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


#ifndef PRACTICETRIAL__H
#define PRACTICETRIAL__H


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

#include "TrialMan.h"


// These are the variables for the practice trial
class PracticeTrial : public Trial
{
  public:
	PracticeTrial(int trialnum, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier);
	~PracticeTrial();

  public:

	void Start();
	void Notify();
	void Render();
	void NextStage();
	void LoadImages();

  private:
	
	void RenderText();

  private:
    static VOID CALLBACK TimerProc(HWND hwnd,
				UINT uMsg,
				UINT_PTR idEvent,
				DWORD dwTime);

public:
	
	static int PracticeStageTime;
	std::string _imageFile2;

private:
	GLuint _image2;
};


#endif