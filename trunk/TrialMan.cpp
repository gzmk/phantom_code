/***********************************************************
 *
 * TrialMan.cpp
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
 * This file defines the variables representing each trial.
 * It also contains a definition of a class used to hold
 * all the parameters that define how a set of trials should
 * be constructed.  This last class also contains a utility
 * function for constructing a trial set based on the member
 * data.
 *
 ***********************************************************/

#ifndef TRIALMAN__H
	#include "TrialMan.h"
#endif

const double Trial::ZoneRad = 0.1;
const double Trial::ZoneOffset = -0.1;

Trial* Trial::CurrentTrial = NULL;
LARGE_INTEGER Trial::LastTicks;
UINT Trial::TimerID = 21;
int Trial::RefreshTime = 10;
//WJA to change presentation time //
//int Trial::StageTime = 2000;
//int Trial::StageTime = 3000;  // naives found haptics too hard at 2secs // @@@@
int Trial::StageTime = 10000;//4000;  // naives found haptics too hard at 2secs // @@@@
//int Trial::StageTime = 10000;
bool Trial::OutsideZone = true;
int Trial::ParticipantNumber = 0;
std::string Trial::ImageFilename = "Textures/Renders/Potato_t";
//float Trial::StiffnessValues[Trial::NumValues] = {0.3f, 0.3875f, 0.475f, 0.5625f, 0.65f, 0.7375f, 0.825f, 0.9125f, 1.0f};
//float Trial::DampingValues[Trial::NumValues] = {0.217f, 0.2002f, 0.1835f, 0.1668f, 0.15f, 0.1332f, 0.1165f, 0.0997f, 0.083f};
//float Trial::StaticFrictionValues[Trial::NumValues] = {0.99f, 0.8675f, 0.745f, 0.6225f, 0.5f, 0.3775f, 0.255f, 0.1325f, 0.1f};
//float Trial::DynamicFrictionValues[Trial::NumValues] = {0.99f, 0.8675f, 0.745f, 0.6225f, 0.5f, 0.3775f, 0.255f, 0.1325f, 0.1f};
float Trial::StiffnessValues[Trial::NumValues] = {0.65f};
float Trial::DampingValues[Trial::NumValues] = {0.15f};
//float Trial::StiffnessValues[Trial::NumValues] = {0.3f};
//float Trial::DampingValues[Trial::NumValues] = {0.217f};
float Trial::StaticFrictionValues[Trial::NumValues] = {0.5f};
float Trial::DynamicFrictionValues[Trial::NumValues] = {0.5f};

int TrialSetupParams::DTimer = 3000;


///////////////////////////////////////////
// Class holding the variables per trial
///////////////////////////////////////////
// Constructor
Trial::Trial(int trialnum, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier)
{
	// Setup default values when class is constructed
	VisualPresence = true;
	HapticPresence = true;
	Environment = -1;
	Glossiness = -1.0;
	Depth = -1.0;
	Conflict = -1.0;
	Timer1 = 0.0;

	GlossResult = -1.0;
	DepthResult = -1.0;
	InstructionTrial = false;
	IsPracticeTrial = false;

	LastRadius = 0.0;
	StartedGravity = false;
	Text[0] = '\0';

	_stage = 0;
	_betweenStages = false;
	_firstResponse = false;
	_started = false;
	_waiting = false;
	_trialNum = trialnum;
	_notifier = notifier;
	_stageNotifier = stagenotifier;

	_image1 = 0;
}


// Destructor
Trial::~Trial()
{
}


void Trial::Start()
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
		outside = OutsideZone;
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


void Trial::Stop()
{
	//if (InstructionTrial == false)
	{
		// Called when you want to end trial
		KillTimer(NULL, Trial::TimerID);
	}

	SetStarted(false);

	// No need to stop timer as only called after last timer called
	Trial::CurrentTrial = NULL;
}


int Trial::GetStage()
{
	return _stage;
}


void Trial::SetResult(double glossresult, double depthresult)
{	
	// Only accept first response
	if (_firstResponse == false)
	{
		GlossResult = glossresult;
		DepthResult = depthresult;

		_firstResponse = true;

		std::cout << "\a";
	}
}


void Trial::Notify()
{
	if (InstructionTrial)
	{
		_waiting = true;

		QueryPerformanceCounter(&LastTicks);
		
		SetTimer(NULL, Trial::TimerID, (UINT) Trial::RefreshTime, TimerProc);
	}
	else
	{
		if (_notifier != NULL)
			_notifier(this);
	}

	// Start the timer
}


TrialEndNotifier Trial::GetNotifier()
{
	return _notifier;
}


void Trial::Render()
{
	if (VisualPresence)
	{
		GLuint texID = 0;

		switch (_stage)
		{
		case 0:
			texID = _image1;
			break;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glBindTexture(GL_TEXTURE_2D, texID);

		glPushMatrix();

		float s = 0.086676f * 0.695f;

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-s, -s, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(s, -s, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(s, s, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-s, s, 0.0f);
		glEnd();

		glPopMatrix();

		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
}


// Render the instruction text
void Trial::RenderText(int screenwidth, int screenheight)
{
	char *c;
	float x = ((640.0f / 1280.0f) * screenwidth) - ( ((float) strlen(Text)) * 6.5f );

	SetOrthographicProjection(screenwidth, screenheight);
	glPushMatrix();
	glLoadIdentity();

	if (_waiting == false)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor3f(0.6f, 0.1f, 0.2f);

	glLineWidth(2.5f);
	glTranslatef(x,570.0f * (screenwidth / 1280.0f),0.5f);
	glScalef(0.2f * (screenwidth / 1280.0f), 0.2f * (screenwidth / 1280.0f), 1.0f);

	for (c=Text; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}

	glPopMatrix();
	ResetPerspectiveProjection();
}

void Trial::NextStage()
{
	SetStarted(false);

	if (_stage > 2)
	{
		//Notify();
		return;
	}
	else if (_stage >= 0)//> 1)
	{
		_stage++;
		_betweenStages = false;
	}
	else
	{
		bool outside = true;

#ifndef NO_HAPTICS
		if (HapticPresence)
			outside = OutsideZone;
#endif

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


bool Trial::GetBetweenStages()
{
	return _betweenStages;
}


double Trial::GetRadius(double* proxypos)
{
#ifndef NO_HAPTICS
	double dxsq = ((double) proxypos[0]) * ((double) proxypos[0]);
	double dysq = ((double) proxypos[1]) * ((double) proxypos[1]);
	double dz = ((double) proxypos[2]);// - Trial::ZoneOffset;
	double dzsq = dz * dz;

	LastRadius = sqrt(dxsq + dysq + dzsq);
#else
	LastRadius = 0.0;
#endif

	return LastRadius;
}


bool Trial::IsWaiting()
{
	return _waiting;
}


GLuint Trial::GetImage()
{
	switch (_stage)
	{
	case 0:
		return _image1;
	}

	return 0;
}


double Trial::GetTimer()
{
	switch (_stage)
	{
	case 0:
		return Timer1;
	}

	return 0;
}


int Trial::GetTrialNum()
{
	return _trialNum;
}


void Trial::SetTrialNum(int trial)
{
	_trialNum = trial;
}


bool Trial::GetStarted()
{
	return _started;
}


void Trial::SetStarted(bool started)
{
	if ((_started == false) && started)
		QueryPerformanceCounter(&LastTicks);

	_started = started;
}


void Trial::LoadImages()
{
	int trialnum = _trialNum;
	std::string filestr = ImageFilename;

	std::cout << "Loading images" << std::endl;

	std::stringstream ss1;
	ss1 << filestr << (trialnum+1) << "_1" << ".tga";
	_imageFile1 = ss1.str();
	_image1 = LoadTexture(ss1.str());
}


GLuint Trial::LoadTexture(std::string filename)
{
	int i;
	unsigned short header[9];
	unsigned long * texturedata;//tga image data
	unsigned long datared, datagreen, datablue, dataalpha;
	FILE * f;
	errno_t temperr;
	GLuint texID = 0;

	temperr = fopen_s(&f, filename.c_str(), "rb");

	if (temperr == 0)
	{
		fread( header, 2, 9, f );
		texturedata = new unsigned long[ header[6]*header[7] ];
		fread(texturedata, 4*header[6], header[7], f);
		fclose(f);
		
		for (i=0; i<(header[6]*header[7]); i++) // Correct from ARGB to ABGR used by opengl
		{
			dataalpha = (texturedata[i] & 0xFF000000) >> 24;
			datared = (texturedata[i] & 0xFF0000) >> 16;
			datagreen = (texturedata[i] & 0xFF00) >> 8;
			datablue = (texturedata[i] & 0xFF);
			texturedata[i] = (dataalpha << 24) | (datablue << 16) | (datagreen << 8) | datared; // Actually ABGR
			//data[i] = 0xFF0000FF; // Red
			//data[i] = 0xFF00FF00; // Green
			//data[i] = 0xFFFF0000; // Blue
		}

		glActiveTexture(GL_TEXTURE1); // Activate a different texture unit (unit 1)

		glGenTextures(1, &texID); // Generate a unique texture ID
		glBindTexture(GL_TEXTURE_2D, texID); // Bind the texture to texture unit 2

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // no interpolation when image smaller than texture
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header[6], header[7], 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);

		glActiveTexture(GL_TEXTURE0); // Switch active texture unit back to 0 again

		delete texturedata;
	}

	return texID;
}


VOID CALLBACK Trial::TimerProc(HWND hwnd,
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
		else if ((Trial::CurrentTrial->GetBetweenStages() == false) && Trial::CurrentTrial->GetStarted())
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
						if (zonems >= StageTime)
						{
							//Trial::OutsideZone = false;
							Trial::CurrentTrial->NextStage();
						}
					}
				}
			}
		}
	}
}


// Set projection to be othographic
void Trial::SetOrthographicProjection(int screenwidth, int screenheight)
{
	// Switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// Save previous matrix which contains the 
	// settings for the perspective projection
	glPushMatrix();
	// Reset matrix
	glLoadIdentity();
	// Set a 2D orthographic projection
	gluOrtho2D(0, screenwidth, 0, screenheight);
	// Invert the y axis, down is positive
	glScalef(1, -1, 1);
	// Move the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -screenheight, 0);
	glMatrixMode(GL_MODELVIEW);
}


// Return to perspective projection after othographic
/* 
 Since we saved the settings of the perspective projection before we
 set the orthographic projection, all we have to do is to change the
 matrix mode to GL_PROJECTION, pop the matrix, i.e. restore the settings,
 and finally change the matrix mode again to GL_MODELVIEW.
*/
void Trial::ResetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


/////////////////////////////////////////////////////////////////////
// Class holding the variables governing the creation of all trials
/////////////////////////////////////////////////////////////////////
// Constructor
TrialSetupParams::TrialSetupParams()
{
	MinTimer = -80000.0f;
	MaxTimer = 80000.0f;

	// Initialize random seed
	srand((unsigned int) time(NULL));
}


// Destructor
TrialSetupParams::~TrialSetupParams()
{
}


//////////////////////////////////////
// Public functions for this class
//////////////////////////////////////

// Generate randomised list of trials
std::vector<Trial*> TrialSetupParams::RandomiseTrials(int numtrials, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier, int block)
{
	Trial* newtrial;
	std::vector<Trial*> trialvars;
	trialvars.clear();
	
	int counter1;

	for (counter1=0;counter1<numtrials;counter1++)
	{
		newtrial = new Trial(counter1, notifier, stagenotifier);
		
		if (newtrial)
		{
			newtrial->InstructionTrial = false;
			//newtrial->LoadImages();
			trialvars.push_back(newtrial);
		}
	}

	numtrials = (int)trialvars.size();

	for (counter1=0;counter1<numtrials;counter1++)
	{
		if (trialvars[counter1] != 0)
		{
			// Generate random number
			int temprand;
			int randscale = 30000;
			float randscalef = (float) randscale;
			float tempfloat;
			int conflictnum;

			temprand = rand() % 2;
			if (temprand > 0)
			{
				trialvars[counter1]->HapticPresence = true;
				conflictnum = 3;
			}
			else
			{
				trialvars[counter1]->HapticPresence = false;
				conflictnum = 1;
			}

			temprand = rand() % Trial::NumEnvironments;
			trialvars[counter1]->Environment  = temprand;

			temprand = rand() % Trial::NumValues;
			trialvars[counter1]->Glossiness  = temprand;

			temprand = rand() % Trial::NumValues;
			trialvars[counter1]->Depth  = temprand;

			temprand = rand() % conflictnum;
			trialvars[counter1]->Conflict  = temprand;
			
			temprand = rand() % randscale;
			tempfloat = ((float) temprand) / randscalef;
			trialvars[counter1]->Timer1 = (tempfloat * (MaxTimer - MinTimer)) + MinTimer;

			trialvars[counter1]->Block = block;
		}
	}

	return trialvars;
}


// Generate list of trials for all parameter combinations
std::vector<Trial*> TrialSetupParams::GenerateAllTrials(int trialoffsetnum, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier, bool hapticsenabled, bool visualenabled, int block)
{
	std::vector<Trial*> trialvars;
	trialvars.clear();

	TrialSetupParams* trialsetup = new TrialSetupParams();

	int temprand;
	int randscale = 30000;
	int conflictnum = 0;
	float tempfloat;
	float randscalef = (float) randscale;

	int env_counter, gloss_counter, depth_counter, conflict_counter;
	float timer;
	Trial* newtrial = NULL;
	int totalcounter = 0;

	if(hapticsenabled)			// generate 3 conflicts for haptic trials only
		conflictnum = 3;		// conflict parameters are 0 = veridical, 1 = +10%
	else						// 2 = -10%.  All visual only conflicts are 0
		conflictnum = 1;


	// Firstly generate all trials with a specular highlight
	for (env_counter = 0;env_counter < Trial::NumEnvironments; env_counter++)
	{
		for (gloss_counter = 0;gloss_counter < Trial::NumValues; gloss_counter++)
		{
			for (depth_counter = 0;depth_counter < Trial::NumValues; depth_counter++)
			{			
				for(conflict_counter = 0;conflict_counter < conflictnum; conflict_counter++)
				{
					
					newtrial = new Trial(totalcounter + trialoffsetnum, notifier, stagenotifier);

					if (newtrial)
					{
						newtrial->InstructionTrial = false;
						newtrial->VisualPresence = visualenabled;
						newtrial->HapticPresence = hapticsenabled;
						newtrial->Environment = env_counter;
						newtrial->Glossiness = gloss_counter + 2;	//9 glossiness levels (2:10)
						newtrial->Depth = depth_counter + 2;		//9 depth levels (2:10)
						newtrial->Conflict = conflict_counter;

						temprand = rand() % randscale;
						tempfloat = ((float) temprand) / randscalef;
						timer = (tempfloat * (MaxTimer - MinTimer)) + MinTimer;
						newtrial->Timer1 = timer;

						newtrial->Block = block;

						trialvars.push_back(newtrial);

						totalcounter++;
					}
				}
			}
		}
	}
	
	RandomPermute(trialvars.begin(), trialvars.size());

		// add break trial after every 81 trials

			for(totalcounter = 1; totalcounter < trialvars.size(); totalcounter++)  // prime counter to 1 to avoid inserting trial at the beginning
		{
			if(totalcounter % 82 == 0)
			{
				trialvars.insert(trialvars.begin()+ totalcounter, trialsetup->GenerateInstructionTrial(0, notifier, stagenotifier, "Left mouse click to continue", 0));
			}
		}
	
	return trialvars;
}


Trial* TrialSetupParams::GenerateInstructionTrial(int trialnum, TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier, std::string text, int block)
{
	Trial* newtrial = new Trial(trialnum, notifier, stagenotifier);

	newtrial->InstructionTrial = true;
	newtrial->Block = block;

	strcpy_s(newtrial->Text,100,text.c_str());

	return newtrial;
}

// Public function to randomise order of trials
void TrialSetupParams::RandomPermute(std::vector<Trial*>::iterator it, int numtrials)
{
	int counter1;
	int swapind;
	Trial* tempvars;
	
	for (counter1=0;counter1<numtrials;counter1++)
	{
		// Get index of next element in list
		swapind = rand() % numtrials;

		// Swap elements
		tempvars = *(it+counter1);
		*(it+counter1) = *(it+swapind);
		*(it+swapind) = tempvars;
	}
}


///////////////////////////////
// Private Utility functions
///////////////////////////////

// Private function to check the values are valid
void TrialSetupParams::CheckVals()
{
	float tempfloat;

	// Check Timer
	if (MinTimer > MaxTimer)
	{
		tempfloat = MaxTimer;
		MaxTimer = MinTimer;
		MinTimer = tempfloat;
	}

	if (MinTimer < 0)
		MinTimer = 0.0f;

	if (MaxTimer < 0)
		MaxTimer = 0.0f;
}
