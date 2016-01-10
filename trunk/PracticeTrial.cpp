/***********************************************************
 *
 * PracticeTrial.cpp
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
 * This file extends the Trial class to allow two potatoes
 * to be displayed side-by-side to demonstrate the extreme
 * values.
 *
 ***********************************************************/

#ifndef PRACTICETRIAL__H
	#include "PracticeTrial.h"
#endif


int PracticeTrial::PracticeStageTime = 4000;


///////////////////////////////////////////
// Class holding the variables per trial
///////////////////////////////////////////
// Constructor
PracticeTrial::PracticeTrial(int trialnum, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier) :
	Trial(trialnum, notifier, stagenotifier)
{
	InstructionTrial = false;
	IsPracticeTrial = true;

	_image2 = 0;
}


// Destructor
PracticeTrial::~PracticeTrial()
{
}


void PracticeTrial::Start()
{
	// Called when trial starts presenting
	Trial::CurrentTrial = this;

	if (InstructionTrial == false)
		LoadImages();

	_firstResponse = false;

	GlossResult = 0.0;
	DepthResult = 0.0;

	bool outside = true;

#ifndef NO_HAPTICS
	if (HapticPresence)
		outside = false;//OutsideZone;
#endif

	if (outside)
	{
		_stage = 0;
		_betweenStages = false;
	}
	else
	{
		_stage = -1;
		StartedGravity = false;
		_betweenStages = true;
	}

	if (!HapticPresence)//VisualPresence)
		SetStarted(true);
	
	std::cout << "\n\n~~~~~~   STAGE: " << _stage << "\n";

	switch (_stage)
	{
	case 0:
		std::cout << "~~~~~~   Displaying image file " << _imageFile1 << "\n";
		break;
	}

	if (InstructionTrial == false)
		SetTimer(NULL, Trial::TimerID, (UINT) Trial::RefreshTime, TimerProc);
	else
		_waiting = false;
}


void PracticeTrial::Notify()
{
	if (_notifier != NULL)
		_notifier(this);

	// Start the timer
}


void PracticeTrial::Render()
{
	if (VisualPresence)
	{
		float s = 0.086676f * 0.695f;
		float x;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glBindTexture(GL_TEXTURE_2D, _image1);

		glPushMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		x = -s;

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x-s, -s, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x+s, -s, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x+s, s, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x-s, s, 0.0f);
		glEnd();

		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, _image2);
		
		glPushMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		x = s;

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x-s, -s, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x+s, -s, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x+s, s, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x-s, s, 0.0f);
		glEnd();

		glPopMatrix();

		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	RenderText();
}


void PracticeTrial::NextStage()
{
	SetStarted(false);

	if (_stage > 2)
	{
		//Notify();
		return;
	}
	else if (_stage > 1)
	{
		_stage++;
		_betweenStages = false;
	}
	else
	{
		bool outside = true;

/*#ifndef NO_HAPTICS
		if (HapticPresence)
			outside = OutsideZone;
#endif*/

		if (outside)
		{
			_stage++;
			_betweenStages = false;

			if (_stageNotifier)
				_stageNotifier(this);

			if (!HapticPresence)//VisualPresence)
				SetStarted(true);

			std::cout << "\n\n======   STAGE: " << _stage << "\n";

			switch (_stage)
			{
			case 0:
				std::cout << "======   Displaying image file " << _imageFile1 << "\n";
				break;
			}
		}
		else
		{
			StartedGravity = false;
			_betweenStages = true;
		}
	}
}


void PracticeTrial::LoadImages()
{
	int trialnum = _trialNum;
	std::string filestr = ImageFilename;

	if (VisualPresence)
	{
		std::cout << "Loading images" << std::endl;

		std::stringstream ss1;
		ss1 << "Textures/Renders/Practice_1a" << ".tga";
		_imageFile1 = ss1.str();
		_image1 = LoadTexture(ss1.str());

		std::stringstream ss2;
		ss2 << "Textures/Renders/Practice_1b" << ".tga";
		_imageFile2 = ss2.str();
		_image2 = LoadTexture(ss2.str());
	}
}


/*******************************************************************************
 Render the instruction text
*******************************************************************************/
void PracticeTrial::RenderText()
{
	char *c;
	char text[120];
	float xfac = 1.0f;
	float x = 0.0f;

	switch (_trialNum)
	{
		case 0:
			strcpy_s(text,119,"Glossiest                 Most Matte");
			xfac = 360.0f / 1280.0f;
			break;
		case 1:
			strcpy_s(text,119,"Hardest                 Most Squashy");
			xfac = 340.0f / 1280.0f;
			break;
		case 2:
			strcpy_s(text,119,"Most Slippery                Stickiest");
			xfac = 340.0f / 1280.0f;
			break;
	}

	x = xfac * 1280.0f;

	SetOrthographicProjection(1280, 1024);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.0f, 0.0f, 0.0f);

	glLineWidth(2.5f);
	glTranslatef(x, 800.0f, 0.5f);
	glScalef(0.2f, 0.2f, 1.0f);

	for (c=text; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}

	glPopMatrix();
	ResetPerspectiveProjection();
}


VOID CALLBACK PracticeTrial::TimerProc(HWND hwnd,
				UINT uMsg,
				UINT_PTR idEvent,
				DWORD dwTime)
{
	LARGE_INTEGER ticks;
	LARGE_INTEGER perfreq;

	if (Trial::CurrentTrial)
	{
		if (Trial::CurrentTrial->InstructionTrial)
		{
			if (Trial::CurrentTrial->IsWaiting())
			{
				if (QueryPerformanceCounter(&ticks))
				{
					if (QueryPerformanceFrequency(&perfreq))
					{
						ticks.QuadPart-= Trial::LastTicks.QuadPart;
						
						double tickms = 1000 * ((double) ticks.QuadPart) / ((double) perfreq.QuadPart);

						if (tickms > 0)
						{
							int zonems = (int) tickms;
							if (zonems >= 2000)//StageTime) // @@@@
							{
								// This is used to tell the main program
								// that this trial has finished (start next trial)
								TrialEndNotifier notifier = Trial::CurrentTrial->GetNotifier();

								if (notifier)
									notifier(Trial::CurrentTrial);
							}
						}
					}
				}
			}
		}
		/*else if ((Trial::CurrentTrial->GetBetweenStages() == false) && Trial::CurrentTrial->GetStarted())
		{
			if (QueryPerformanceCounter(&ticks))
			{
				if (QueryPerformanceFrequency(&perfreq))
				{
					ticks.QuadPart-= Trial::LastTicks.QuadPart;
					
					double tickms = 1000 * ((double) ticks.QuadPart) / ((double) perfreq.QuadPart);

					if (tickms > 0)
					{
						int zonems = (int) tickms;
						if (zonems >= PracticeStageTime)
						{
							//Trial::OutsideZone = false;
							Trial::CurrentTrial->NextStage();
						}
					}
				}
			}
		}*/
	}
}

