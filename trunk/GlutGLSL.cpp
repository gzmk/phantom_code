/***********************************************************
 *
 * GlutGLSL.cpp
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
 * This is the main project file, with main() at the bottom.
 *
 * Acknowledgements:
 *
 * This simple application is based on the Lighthouse3D
 * demo at www.lighthouse3d.com
 *
 ***********************************************************/


#include <stdlib.h>


// GL Headers
#include <GL/glew.h>
#include <GL/glut.h>

// Declare the headers of classes we are using
#ifndef GLOBALS__H
	#include "Globals.h" //global variables
#endif
#ifndef HAPTICGUI__H
	#include "HapticGUI.h" //haptic buttons
#endif

#ifndef SHADERMAN__H
	#include "ShaderMan.h" //we probably do not need this as we don't need real-time rendering
#endif

#ifndef NO_HAPTICS
	#ifndef HAPTICS__H
		#include "Haptics.h" //phantom device setup
	#endif
#endif

#ifndef INTROTRIAL__H
	#include "IntroTrial.h" //trial related variables
#endif

#ifndef PERLINNOISE__H
	#include "PerlinNoise.h" //prob won't need this
#endif

#ifndef RENDERER__H
	#include "Renderer.h" //prob won't need this either
#endif


// Prototype of button callback (forward declaration)
void OnButton(int butnum, bool state);
// Prototype of slider callback (forward declaration)
void OnSlider(int slidnum, double value);
// Prototype of trial callback (forward declaration)
void OnTrialEnd(Trial* trial);
// Prototype of trial stage callback (forward declaration)
void OnTrialStageEnd(Trial* trial);
// Prototype of introtrial callback (forward declaration)
void OnIntroTrialEnd(IntroTrial* trial);
// Prototype of trial setup routine (forward declaration)
void SetupTrials(int method);

GLuint PlaneDisplayList1 = -1;
GLuint PlaneDisplayList2 = -1;
bool NewDisplayList1 = true;
bool NewDisplayList2 = true;
bool Painting = false;


// ///////////////////////////////////////////////////////////
// // For Vertex Shader Perlin Noise
// ///////////////////////////////////////////////////////////

// int perm[256]= {151,160,137,91,90,15,
//   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
//   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
//   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
//   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
//   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
//   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
//   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
//   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
//   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
//   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
//   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
//   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

// /* These are Ken Perlin's proposed gradients for 3D noise. I kept them for
//    better consistency with the reference implementation, but there is really
//    no need to pad this to 16 gradients for this particular implementation.
//    If only the "proper" first 12 gradients are used, they can be extracted
//    from the grad4[][] array: grad3[i][j] == grad4[i*2][j], 0<=i<=11, j=0,1,2
// */
// int grad3[16][3] = {{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},
//                    {1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
//                    {1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0}, // 12 cube edges
//                    {1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}}; // 4 more to make 16

//  These are my own proposed gradients for 4D noise. They are the coordinates
//    of the midpoints of each of the 32 edges of a tesseract, just like the 3D
//    noise gradients are the midpoints of the 12 edges of a cube.

// int grad4[32][4]= {{0,1,1,1}, {0,1,1,-1}, {0,1,-1,1}, {0,1,-1,-1}, // 32 tesseract edges
//                    {0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
//                    {1,0,1,1}, {1,0,1,-1}, {1,0,-1,1}, {1,0,-1,-1},
//                    {-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
//                    {1,1,0,1}, {1,1,0,-1}, {1,-1,0,1}, {1,-1,0,-1},
//                    {-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
//                    {1,1,1,0}, {1,1,-1,0}, {1,-1,1,0}, {1,-1,-1,0},
//                    {-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}};


// /*
// * InitPermTexture(GLuint *texID) - create and load a 2D texture for
// * a combined index permutation and gradient lookup table.
// * This texture is used for 2D and 3D noise, both classic and simplex.
// */
// void InitPermTexture(GLuint *texID)
// {
// 	char *pixels;
// 	int i,j;

// 	glActiveTexture(GL_TEXTURE0); // Activate a different texture unit (unit 1)

// 	glGenTextures(1, texID); // Generate a unique texture ID
// 	glBindTexture(GL_TEXTURE_2D, *texID); // Bind the texture to texture unit 0

// 	pixels = (char*)malloc(256*256*4);

// 	for(i = 0; i<256; i++)
// 	{
// 		for(j = 0; j<256; j++)
// 		{
// 			int offset = (i*256+j)*4;
// 			char value = perm[(j+perm[i]) & 0xFF];
// 			pixels[offset] = grad3[value & 0x0F][0] * 64 + 64;   // Gradient x
// 			pixels[offset+1] = grad3[value & 0x0F][1] * 64 + 64; // Gradient y
// 			pixels[offset+2] = grad3[value & 0x0F][2] * 64 + 64; // Gradient z
// 			pixels[offset+3] = value;                     // Permuted index
// 		}
// 	}

// 	// GLFW texture loading functions won't work here - we need GL_NEAREST lookup.
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
// 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// 	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
// }



/*******************************************************************************
 Set up the view
*******************************************************************************/
void SetupView(bool lefteye)
{
	if (SwappedLR)
		lefteye = !lefteye;

	if (UseHollimanCamera)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Set the correct perspective.
		// (these are swapped, seems to need it as we have reversed
		// the look direction compared to Nick's approach).
		if (lefteye == false)
			glFrustum(Stereo_L_l, Stereo_L_r, Stereo_L_b, Stereo_L_t, CamNear, CamFar);
		else
			glFrustum(Stereo_R_l, Stereo_R_r, Stereo_R_b, Stereo_R_t, CamNear, CamFar);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float xoffset = 0.0f;
	if (UseHollimanCamera)
		xoffset = StereoSepDist;

	if (lefteye)
	{
		gluLookAt(-StereoSepDist, 0.0f, -CamDist,
			-xoffset, 0.0f, CubeCentre,
			0.0f, 1.0f, 0.0f);
	}
	else
	{
		gluLookAt(StereoSepDist, 0.0f, -CamDist,
			xoffset, 0.0f, CubeCentre,
			0.0f, 1.0f, 0.0f);
	}
}


/*******************************************************************************
 Respond to window resizes
*******************************************************************************/
void OnWindowResize(int w, int h)
{
	if (Quitting)
		return;

	static const double kPI = 3.1415926535897932384626433832795;
    static const double kFovY = 40;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;
	double nearDist, farDist, aspect;
	nearDist = 1.0 / tan((kFovY / 2.0) * kPI / 180.0);
    farDist = nearDist + 2.0;
    aspect = (double) w / h;
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (UseHollimanCamera)
		CamFOV = Stereo_fov;

	// Set the correct perspective.
	//if (UseHollimanCamera)
	//	glFrustum(Stereo_L_l, Stereo_L_r, Stereo_L_b, Stereo_L_t, CamNear, CamFar);
	//else
		gluPerspective(CamFOV, aspect, CamNear, CamFar);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,-CamDist, 
		      0.0,0.0,CubeCentre,
			  0.0f,1.0f,0.0f);

	AppStateVars.ScreenWidth = w;
	AppStateVars.ScreenHeight = h;

#ifndef NO_HAPTICS
	// Update the haptic workspace for new projection matrix    
	Haptics::UpdateHapticWorkspace();
#endif
}


/*******************************************************************************
 GLUT callback for idle state.  Use this as an opportunity to request a redraw.
 Checks for HLAPI errors that have occurred since the last idle check.
*******************************************************************************/
void OnIdle()
{
#ifndef NO_HAPTICS
    HLerror error;

    while (HL_ERROR(error = hlGetError()))
    {
        fprintf(stderr, "HL Error: %s\n", error.errorCode);
        
        if (error.errorCode == HL_DEVICE_ERROR)
        {
            hduPrintError(stderr, &error.errorInfo,
                "Error during haptic rendering\n");
        }
    }
#endif
    
	// Request redraw
    glutPostRedisplay();
}


void OnBuildPotato()
{
#ifndef NO_HAPTICS
	hlBeginFrame();
	hlEndFrame();
	hlCheckEvents();
#endif
}


/*******************************************************************************
 Respond to keyboard presses
*******************************************************************************/
void OnKeyboard(unsigned char key, int x, int y)
{
	// Exit if Escape key pressed
	if (key == 27) 
	{
		Quitting = true;
		exit(0);
	}
	else if (key == 65) 
		SwappedLR = !SwappedLR;
	else
	{
		if ((AppStateVars.InIntroTrial) && TrialIntro)
		{
			if ((key > 47) && (key < 58))
				TrialIntro->OnKeyPress(key-48);
			else if (key == 8)
				TrialIntro->OnKeyPress(254);
			else if (key == 13)
				TrialIntro->OnKeyPress(255);
		}
		else
		{
			int numtrials = (int) AppStateVars.Trials.size();
			if ((AppStateVars.CurrentTrial >= 0) && (AppStateVars.CurrentTrial < numtrials))
			{
				if (AppStateVars.Trials[AppStateVars.CurrentTrial] != NULL)
				{
					if (AppStateVars.Trials[AppStateVars.CurrentTrial]->InstructionTrial)
						AppStateVars.Trials[AppStateVars.CurrentTrial]->Notify();
				}
			}
		}
	}
}


/*******************************************************************************
 Respond to mouse button presses
*******************************************************************************/
void OnMouse(int button, int state, int x, int y)
{
	// Respond to mouse presses (not used yet)
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state ==  GLUT_DOWN)
		{
			AppStateVars.LeftDown = true;
			AppStateVars.InitX = x;
			AppStateVars.InitY = y;

			if (AppStateVars.InIntroTrial == false)
			{
				int numtrials = (int) AppStateVars.Trials.size();
				if ((AppStateVars.CurrentTrial >= 0) && (AppStateVars.CurrentTrial < numtrials))
				{
					if (AppStateVars.Trials[AppStateVars.CurrentTrial] != NULL)
					{
						//if (AppStateVars.Trials[AppStateVars.CurrentTrial]->InstructionTrial)
							AppStateVars.Trials[AppStateVars.CurrentTrial]->Notify();
					}
				}
			}
		}
		else
			AppStateVars.LeftDown = false;
	}

	AppStateVars.CurrentX = x;
	AppStateVars.CurrentY = y;
}


/*******************************************************************************
 Respond to mouse movement
*******************************************************************************/
void OnMouseMove(int x, int y)
{
	// Respond to mouse moves (not used yet)
	AppStateVars.CurrentX = x;
	AppStateVars.CurrentY = y;

	if (AppStateVars.LeftDown)
	{
	}
}


#ifndef NO_HAPTICS
/*******************************************************************************
 Haptic Callbacks
*******************************************************************************/
void HLCALLBACK SphereTouchCallback(
					HLenum event,
					HLuint object,
					HLenum thread,
					HLcache *cache,
					void *userdata)
{
	// Called when hit surface of sphere
	int numtrials = (int) AppStateVars.Trials.size();

	Trial* currenttrial = NULL;

	if ((AppStateVars.InPracticeTrial >= 0) && (AppStateVars.InPracticeTrial < (int)AppStateVars.PracticeTrials.size()))
		currenttrial = AppStateVars.PracticeTrials[AppStateVars.InPracticeTrial];
	else if ((AppStateVars.CurrentTrial >= 0) && (AppStateVars.CurrentTrial < numtrials))
		currenttrial = AppStateVars.Trials[AppStateVars.CurrentTrial];

	if (currenttrial != NULL)
	{
		if (/*(currenttrial->VisualPresence == false) && */(currenttrial->InstructionTrial == false))
			currenttrial->SetStarted(true);
	}
}


void HLCALLBACK SphereUnTouchCallback(
					HLenum event,
					HLuint object,
					HLenum thread,
					HLcache *cache,
					void *userdata)
{
	// Called when left surface of sphere
}
#endif


/*******************************************************************************
 Prepare for next Trial
*******************************************************************************/
void SetupNextTrial()
{
	// Increment the currenttrial variable,
	// and set the shader parameters for this new trial
	// (the trial's material properties are set during haptic rendering)

	if (AppStateVars.InIntroTrial)
	{
		AppStateVars.InIntroTrial = false;
		AppStateVars.FirstFrame = true;
		AppStateVars.CancelGravity = true;

		// Start practice trials
		AppStateVars.InPracticeTrial = 0;
		AppStateVars.CurrentTrial = 0;

		if (TrialIntro != NULL)
		{
			AppStateVars.ParticipantNumber = TrialIntro->GetParticipantNumber();
			Trial::ParticipantNumber = TrialIntro->GetParticipantNumber();
		}

		glFrontFace(GL_CCW); // CANT CHANGE THIS WITHOUT AFFECTING OPENHAPTICS
		glCullFace(GL_BACK);  // INSTEAD, WE HAVE TO CULL FRONT DUE TO NEGATIVE SCALE FACTOR
		glEnable(GL_CULL_FACE);
		
		SetupTrials(1);
	}
	else if (AppStateVars.InPracticeTrial >= 0)
	{
		AppStateVars.InPracticeTrial++;

		if (AppStateVars.InPracticeTrial >= (int)AppStateVars.PracticeTrials.size())
		{
			// Increment the trial number
			//AppStateVars.CurrentTrial++;
			AppStateVars.InPracticeTrial = -1;
		}
	}
	else
	{
		if (AppStateVars.CurrentTrial < 0)
			AppStateVars.InIntroTrial = true;
		else
		{
			AppStateVars.CurrentTrial++;
		}
	}

	int numtrials = (int) AppStateVars.Trials.size();

	// Increment the trial number
	//AppStateVars.CurrentTrial++;

	if (AppStateVars.CurrentTrial >= numtrials)
	{
		AppStateVars.SaveState();
		AppStateVars.FinaliseState();

		// Exit if we have finished all trials
		exit(0);
	}
	else
	{
		if ((AppStateVars.CurrentTrial >= 0) && (AppStateVars.InPracticeTrial < 0))
		{
			// Save the last state in case of crash (if not in practice trial)
			AppStateVars.SaveState();
		}

		if ((AppStateVars.ActiveShader >= 0) && (AppStateVars.CurrentTrial >= 0))
		{
			// Set the stereo disparity in terms of camera position L and R
			//float sepdist = StereoSepDist;// * AppStateVars.Trials[AppStateVars.CurrentTrial]->StereoDisparity;
			float sepdist = StereoSepDist;
			float camposL[3] = {-sepdist, 0.0f, -CamDist};
			float camposR[3] = {sepdist, 0.0f, -CamDist};
			Trial* currenttrial = NULL;

			if ((AppStateVars.InPracticeTrial >= 0) && (AppStateVars.InPracticeTrial < (int)AppStateVars.PracticeTrials.size()))
				currenttrial = AppStateVars.PracticeTrials[AppStateVars.InPracticeTrial];
			else
				currenttrial = AppStateVars.Trials[AppStateVars.CurrentTrial];

			if (currenttrial->HapticPresence)
			{
				PerlinNoise::SwapUV = !PerlinNoise::SwapUV;
				PerlinNoise::NewDisplayList = true;

				PerlinNoise::DrawPotato(0, currenttrial->Timer1);
				Sleep(10);
			}

			std::cout << "=== This Trial:\n";
			std::cout << "===   Timer1: " << currenttrial->Timer1 << "\n";
			
			if (AppStateVars.InPracticeTrial < 0)
				OnTrialStageEnd(currenttrial);
		}

		if ((AppStateVars.InIntroTrial) && TrialIntro)
			TrialIntro->Start();
		else
		{
			if (AppStateVars.CurrentTrial < 0)
				AppStateVars.CurrentTrial = 0;

			Trial* currenttrial = NULL;

			if ((AppStateVars.InPracticeTrial >= 0) && (AppStateVars.InPracticeTrial < (int)AppStateVars.PracticeTrials.size()))
				currenttrial = AppStateVars.PracticeTrials[AppStateVars.InPracticeTrial];
			else
				currenttrial = AppStateVars.Trials[AppStateVars.CurrentTrial];

			currenttrial->Start();
		}
	}
}


/*******************************************************************************
 Initialise the list of Trials 
*******************************************************************************/
void SetupIntroTrial()
{
	// Generate the initial trainig trial
	TrialIntro = new IntroTrial(OnIntroTrialEnd);
}

void SetupTrials(int method)
{
	// Generate all the trial parameters based on random numbers
	// within specified limits.
	bool oldfirstframe = AppStateVars.FirstFrame;


	// Set up the practice trials (these are not specified in the trial param file)
	PracticeTrial* newpracticetrial;

	newpracticetrial = new PracticeTrial(0, OnTrialEnd, OnTrialStageEnd);
	newpracticetrial->HapticPresence = false; 
	newpracticetrial->VisualPresence = true;
	newpracticetrial->Timer1 = 1000;
	newpracticetrial->Depth = 0;
	newpracticetrial->Glossiness = 0;

	AppStateVars.PracticeTrials.push_back(newpracticetrial);

	newpracticetrial = new PracticeTrial(1, OnTrialEnd, OnTrialStageEnd);
	newpracticetrial->HapticPresence = true;
	newpracticetrial->VisualPresence = false; 
	newpracticetrial->Timer1 = 2000;
	newpracticetrial->Depth = 1;
	newpracticetrial->Glossiness = 0;

	AppStateVars.PracticeTrials.push_back(newpracticetrial);

	newpracticetrial = new PracticeTrial(2, OnTrialEnd, OnTrialStageEnd);
	newpracticetrial->HapticPresence = true;
	newpracticetrial->VisualPresence = false;  
	newpracticetrial->Timer1 = 2200;
	newpracticetrial->Depth = 0;
	newpracticetrial->Glossiness = 1;

	AppStateVars.PracticeTrials.push_back(newpracticetrial);

		
	// Next try to locate textfile with previous parameters,
	// in case program crashed last time through.
	if (!AppStateVars.RestoreState(OnTrialEnd, OnTrialStageEnd))
	{
		AppStateVars.FirstFrame = oldfirstframe;

		// Couldn't find the textfile with last trial params,
		// so initialise a new set of trials from 
		TrialSetupParams* trialsetup = new TrialSetupParams();
		
		int counter1, counter2;
		int numrepetitions = 1;  // Repeat each trial a number of times
		//std::vector<Trial*> triallist;
		std::vector<Trial*> visualtriallist;
		std::vector<Trial*> vishaptictriallist;
		AppStateVars.Trials.clear();
		//visualtriallist.clear();
		AppStateVars.Trials.push_back(trialsetup->GenerateInstructionTrial(0, OnTrialEnd, OnTrialStageEnd, "Start Visual Trials", 0));
		
		visualtriallist = trialsetup->GenerateAllTrials(0, OnTrialEnd, OnTrialStageEnd, false, true, 0);
		vishaptictriallist = trialsetup->GenerateAllTrials(0, OnTrialEnd, OnTrialStageEnd, true, true, 1);
		
		
		for (counter2=0;counter2<(int)visualtriallist.size();counter2++)
		{
			AppStateVars.Trials.push_back(visualtriallist[counter2]);
		}

		AppStateVars.Trials.push_back(trialsetup->GenerateInstructionTrial(0, OnTrialEnd, OnTrialStageEnd, "Start Visual Haptic Trials", 1));

		for (counter2=0;counter2<(int)vishaptictriallist.size();counter2++)		
		{
			AppStateVars.Trials.push_back(vishaptictriallist[counter2]);
		}


		// Write list of trials out to excel
		AppStateVars.SaveExcel();
	}
	else
	{	
		TrialSetupParams* trialresume = new TrialSetupParams();

		// Decrement currenttrial to repeat it
		AppStateVars.CurrentTrial--;

		//Display instruction trial screen after crash to indicate trial progress
		// Instruction trial number always -1 so it is not written to SaveState() text file in trialMan
		std::stringstream trialstream;
		trialstream << "Continue trials " << AppStateVars.CurrentTrial << " of " << AppStateVars.Trials.size();
		std::string trialstring = trialstream.str();

		AppStateVars.Trials.insert(AppStateVars.Trials.begin() + AppStateVars.CurrentTrial,trialresume->GenerateInstructionTrial(-1, OnTrialEnd, OnTrialStageEnd, trialstring, 0));
	}
}


/*******************************************************************************
 Initialise the GUI
*******************************************************************************/
void SetupGUI()
{
	AppGUI = new HapticGUI();

	// Create the buttons used for moving between trials
	AppGUI->Initialise(OnButton, OnSlider);

	// Set up the OK button
	OKButton = new ButtonWidget(1, "OK");
	OKButton->Red = 0.1f;
	OKButton->Green = 0.6f;
	OKButton->Blue = 0.3f;
	OKButton->Width = 0.04f;
	OKButton->Height = 0.03f;
	OKButton->Depth = 0.01f;
	OKButton->SetPosition(0.0f, -0.09f, 0.02f);

	AppGUI->AddButton(OKButton);

	GlossSlider = new SliderWidget(0, " Matte <- 0 1 2 3 4 5 6 7 8 9 10 11 12 -> Glossy");
	GlossSlider->Red = 0.6f;
	GlossSlider->Green = 0.1f;
	GlossSlider->Blue = 0.9f;
	GlossSlider->Width = 0.13f;
	GlossSlider->Height = 0.015f;
	GlossSlider->Depth = 0.01f;
	GlossSlider->Ticks = GlossSliderResolution;
	GlossSlider->SetPosition(-0.065f, -0.04f, -0.03f);
	GlossSlider->SetValue(GlossSliderResolution / 2, false);
	
	AppGUI->AddSlider(GlossSlider);

	DepthSlider = new SliderWidget(1, "Shallow <- 0 1 2 3 4 5 6 7 8 9 10 11 12 -> Deep ");
	DepthSlider->Red = 0.1f;
	DepthSlider->Green = 0.6f;
	DepthSlider->Blue = 0.3f;
	DepthSlider->Width = 0.13f;
	DepthSlider->Height = 0.015f;
	DepthSlider->Depth = 0.01f;
	DepthSlider->Ticks = DepthSliderResolution;
	DepthSlider->SetPosition(-0.065f, 0.01f, -0.03f);
	DepthSlider->SetValue(DepthSliderResolution / 2, false);

	AppGUI->AddSlider(DepthSlider);


	// Make the GUI for the practice trial too
	AppGUI2 = new HapticGUI();

	// Create the buttons used for moving between trials
	AppGUI2->Initialise(OnButton, OnSlider);

	// Set up the OK button
	OKButton2 = new ButtonWidget(1, "OK");
	OKButton2->Red = 0.6f;
	OKButton2->Green = 0.1f;
	OKButton2->Blue = 0.3f;
	OKButton2->Width = 0.04f;
	OKButton2->Height = 0.03f;
	OKButton2->Depth = 0.01f;
	OKButton2->SetPosition(0.0f, -0.09f, 0.02f);

	AppGUI2->AddButton(OKButton2);
}


/*******************************************************************************
 Initialise the ShaderMan class for our shader 
*******************************************************************************/
void SetShaders()
{
	// Initialise the global list of shaders
	NumShaders = 1;
	ShaderList = new ShaderMan*[NumShaders];

	// Initialise the first shader
	ShaderList[0] = new ShaderMan(0);
	//ShaderList[0]->InitShader("specA.vert","specA.frag");
	ShaderList[0]->InitShader("PerlinZshader.vert","PerlinZshader.frag");
	
	// Declare these variables locally instead of in Globals
	float camposL[3] = {-StereoSepDist, 0.0f, -CamDist};
	float camposR[3] = {StereoSepDist, 0.0f, -CamDist};

	// Now declare each of the shader variables we will want to
	// adjust by registering with the ShaderMan class.
	// Keep hold of the return values (in variables called SVar_XXX)
	// so we can quickly reference these variables again.
	SVar_PermTexture = ShaderList[0]->InitShaderVar("permTexture");
	SVar_Timer = ShaderList[0]->InitShaderVar("Timer");
	ShaderList[0]->SetUniformVar1f(SVar_Timer, Timer);

	// Store the current shader being used for rendering
	// (only one shader so this is zero).
	AppStateVars.ActiveShader = 0;
}


#ifndef NO_HAPTICS
/*******************************************************************************
 The main routine for rendering scene haptics.
*******************************************************************************/
void RenderSceneHaptics()

	
{
	float _conflict = 1.0f;

    // Start haptic frame.  (Must do this before rendering any haptic shapes.)
    hlBeginFrame();

	if (TrialIntro && AppStateVars.InIntroTrial)
	{
		// Intro trial - simple haptic rendering.
		if (TrialIntro->GetStage() > 0)
		{
			// Set up gravity if first frame after stage=1
			if (AppStateVars.FirstFrame)
			{
				hlCallback(HL_EFFECT_COMPUTE_FORCE, (HLcallbackProc) Haptics::ComputeGravForce, &AppStateVars);
				hlCallback(HL_EFFECT_START, (HLcallbackProc) Haptics::StartGravEffect, (void*)&AppStateVars);
				hlCallback(HL_EFFECT_STOP, (HLcallbackProc) Haptics::StopGravEffect, (void*)&AppStateVars);
				//hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.4);
				//hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 0.4);
				hlStartEffect(HL_EFFECT_CALLBACK, gIntroGravityId);
				AppStateVars.FirstFrame = false;
			}

			// Set material properties for the shapes to be drawn.
			hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, 0.9f);	//was 0.7
			hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, 0.1f);
			hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, 0.2f);
			hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, 0.3f);

			glPushMatrix();

			// Start a new haptic shape.  Use the feedback buffer to capture OpenGL 
			// geometry for haptic rendering.
			hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gIntroBoxId);

			TrialIntro->RenderBoundingBox(false);

			// End the shape.
			hlEndShape();
		}

		// End the haptic frame.
		hlEndFrame();
		
		// Call any event callbacks that have been triggered.
		hlCheckEvents();

		// Manual check to see force not exceeded
		HDdouble forcevec[3];
		hdGetDoublev(HD_CURRENT_FORCE, forcevec);
		double force = sqrt((forcevec[0] * forcevec[0]) + (forcevec[1] * forcevec[1]) + (forcevec[2] * forcevec[2]));

		if (force > 20)
			DisableDevice(1000);

		return;
	}

	int numtrials = (int) AppStateVars.Trials.size();
	bool instructiontrial = false;
	Trial* currenttrial = NULL;

	if ((AppStateVars.InPracticeTrial >= 0) && (AppStateVars.InPracticeTrial < (int)AppStateVars.PracticeTrials.size()))
		currenttrial = AppStateVars.PracticeTrials[AppStateVars.InPracticeTrial];
	else if ((AppStateVars.CurrentTrial >= 0) && (AppStateVars.CurrentTrial < (int)AppStateVars.Trials.size()))
		currenttrial = AppStateVars.Trials[AppStateVars.CurrentTrial];

	if (currenttrial != NULL)
	{
		if (currenttrial->InstructionTrial)
			instructiontrial = true;
	}

	if (AppStateVars.FirstFrame || AppStateVars.CancelGravity)
	{
		hlStopEffect(gIntroGravityId);
		hlDeleteEffects(gIntroGravityId, 1);

		AppStateVars.FirstFrame = false;
		AppStateVars.CancelGravity = false;
	}
	
	//=========================================================//
	// WJA - Comment this line to return to normal
	// WJA - leave in to have haptics on, even on visual trials
	//=========================================================//
    //AppStateVars.Trials[AppStateVars.CurrentTrial]->HapticPresence = true; // @@@@

	if (currenttrial->HapticPresence && (instructiontrial == false))
	{
		if (currenttrial->GetStage() < 1)
		{
			HLdouble proxypos[3];

			// Get proxy position in world coordinates.
			hlGetDoublev(HL_PROXY_POSITION, proxypos);

			double proxypos2[3];
			proxypos2[0] = proxypos[0]; proxypos2[1] = proxypos[1]; proxypos2[2] = proxypos[2];
			double rad = currenttrial->GetRadius(proxypos2);

#ifndef NO_HAPTICS
			if (UseInBetweenStage)
#endif
			{
				if (rad > Trial::ZoneRad)
					Trial::OutsideZone = true;
				else
					Trial::OutsideZone = false;
			}

			if (currenttrial->GetBetweenStages())
			{
				if (rad > Trial::ZoneRad)
				{
					if (currenttrial->StartedGravity)
					{
						hlStopEffect(gTrialGravityId);
						//std::cout << "STOPPING GRAV EFFECT\n";
					}
					
					Trial::OutsideZone = true;
					currenttrial->NextStage();
				}
				else if (currenttrial->StartedGravity == false)
				{
					hlCallback(HL_EFFECT_COMPUTE_FORCE, (HLcallbackProc) Haptics::ComputeInverseGravForce, NULL);
					//hlCallback(HL_EFFECT_START, (HLcallbackProc) StartGravEffect, NULL);
					//hlCallback(HL_EFFECT_STOP, (HLcallbackProc) StopGravEffect, NULL);
					//hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.4);
					//hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 0.4);
					hlStartEffect(HL_EFFECT_CALLBACK, gTrialGravityId);
					//std::cout << "STARTING GRAV EFFECT\n";

					currenttrial->StartedGravity = true;
				}
			}
			else
			{
				glPushMatrix();

				if(currenttrial->Conflict == 1)			//conflict parameters are 0 = veridical, 1 = +10%
					_conflict = 1.1f;					// 2 = -10%.  All visual only conflicts are 0
														// _conflict variable initialised to 1.0f (veridical condition)
				else if(currenttrial->Conflict == 2)
					_conflict = 0.9f;

				glScalef(0.07f, 0.07f, 0.07f * _conflict);	// scale Z axis	

				if (currenttrial->IsPracticeTrial)
				{
					float stiffness = 0.65f;
					float damping = 0.15f;
					float staticfriction = 0.5f;
					float dynamicfriction = 0.5f;
					float stiffnessB = stiffness;
					float dampingB = damping;
					float staticfrictionB = staticfriction;
					float dynamicfrictionB = dynamicfriction;

					if (Trial::Compliance > 0)
					{
						stiffness = 0.3f;
						stiffnessB = 1.0f;
						damping = 0.217f;
						dampingB = 0.083f;
					}

					if (Trial::Friction > 0)
					{
						staticfriction = 0.99f;
						staticfrictionB = 0.01f;
						dynamicfriction = 0.99f;
						dynamicfrictionB = 0.01f;
					}

					// Set material properties for the sphere.
					hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, stiffness);
					hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, damping);
					hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, staticfriction);
					hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, dynamicfriction);
				
					// Start a new haptic shape.  Use the feedback buffer to capture OpenGL 
					// geometry for haptic rendering.
					hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gSphereShapeIdL);

					glPushMatrix();

						glTranslatef(-PracticeSeparation,0.0f,0.0f);

						PerlinNoise::DrawPotato(currenttrial->GetStage(), currenttrial->GetTimer());

					glPopMatrix();

					// End the shape.
					hlEndShape();
					
					// Set material properties for the sphere.
					hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, stiffnessB);
					hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, dampingB);
					hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, staticfrictionB);
					hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, dynamicfrictionB);

					// Start a new haptic shape.  Use the feedback buffer to capture OpenGL 
					// geometry for haptic rendering.
					hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gSphereShapeIdR);

					glPushMatrix();

						glTranslatef(PracticeSeparation,0.0f,0.0f);

						PerlinNoise::DrawPotato(currenttrial->GetStage(), currenttrial->GetTimer());

					glPopMatrix();

					// End the shape.
					hlEndShape();
				}
				else
				{
					// Set material properties for the sphere.
					hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, Haptics::GetStiffness(Trial::Compliance));
					hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, Haptics::GetDamping(Trial::Compliance));
					hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, Haptics::GetStaticFriction(Trial::Friction));
					hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, Haptics::GetDynamicFriction(Trial::Friction));
				
					// Start a new haptic shape.  Use the feedback buffer to capture OpenGL 
					// geometry for haptic rendering.
					hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gSphereShapeIdL);

					glPushMatrix();

						glTranslatef(0.0f,0.0f,0.0f);

						PerlinNoise::DrawPotato(currenttrial->GetStage(), currenttrial->GetTimer());

					glPopMatrix();

					// End the shape.
					hlEndShape();
				}

				glPopMatrix();
			}
		}
	}

	/////////////////////////////////////////

    // Set material properties for the GUI.
    hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, 0.7f);
    hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, 0.1f);
    hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, 0.2f);
    hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, 0.3f);

	if (instructiontrial)
	{
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 2.0f);
		glScalef(-3.0f, 3.0f, 4.12f);

		hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gBlockShapeId);

		glutSolidCube(1.0f);
    
		// End the shape.
		hlEndShape();

		glPopMatrix();
	}
	else
	{
		if (AppStateVars.InPracticeTrial >= 0)
		{
			// Haptically render the GUI
			AppGUI2->OnHapticRender();
		}
		else
		{
			bool renderedback = false;

			if (currenttrial->HapticPresence == false && currenttrial->GetStage() < 1) // draw 'haptic wall' during visual only trials
			{																		   // Remove when sliders present (ie > stage 0)
				glPushMatrix();
				glTranslatef(0.0f, 0.2f, 2.0f);   
				//glScalef(-2.0f, 0.53f, 4.12f);	
				glScalef(-2.0f, 1.0f, 4.12f);
				hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gBlockShapeId);

				glutSolidCube(1.0f);

		    
				// End the shape.
				hlEndShape();

				glPopMatrix();

				renderedback = true;
			} 

			if (currenttrial->GetStage() > 0)		
			{	/*
				if (!renderedback)
				{

					glPushMatrix();
					glTranslatef(0.0f, 0.2f, 2.04f); 
					glScalef(-2.0f, 0.53f, 4.12f);

					hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gBlockShapeId);

					glutSolidCube(1.0f); 
			    
					// End the shape.
					hlEndShape();

					glPopMatrix();

					renderedback = true;
				}  */ 

				// Haptically render the GUI
				AppGUI->OnHapticRender();
			}
		}
	}

    // End the haptic frame.
    hlEndFrame();
	
	// Call any event callbacks that have been triggered.
    hlCheckEvents();
}
#endif


/*******************************************************************************
 Render the scene
*******************************************************************************/
void DrawTexturedPlane(float size, int segs, float xoffset)
{
	segs = 128;

	int i, j;
	float x, y;

	for(i = 0; i < segs; i++)
	{
		glBegin(GL_QUAD_STRIP);

			for(j = 0; j <= segs; j++)
			{
				x = ((float) i) / ((float) segs);
				y = ((float) j) / ((float) segs);

				glNormal3f(0, 0, 1);
				glTexCoord2f(x + xoffset, y);
				glVertex3f(size*x, size*y, 0);

				x = ((float) (i + 1)) / ((float) segs);
				y = ((float) j) / ((float) segs);

				glNormal3f(0, 0, 1);
				glTexCoord2f(x + xoffset, y);
				glVertex3f(size*x, size*y, 0);
			}

		glEnd();
	}
}


void RenderSceneGraphics(void)
{
	bool renderingpotato = false;
	Trial* currenttrial = NULL;

	if ((AppStateVars.InPracticeTrial >= 0) && (AppStateVars.InPracticeTrial < (int)AppStateVars.PracticeTrials.size()))
		currenttrial = AppStateVars.PracticeTrials[AppStateVars.InPracticeTrial];
	else if ((AppStateVars.CurrentTrial >= 0) && (AppStateVars.CurrentTrial < (int)AppStateVars.Trials.size()))
		currenttrial = AppStateVars.Trials[AppStateVars.CurrentTrial];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

		glScalef(1.0f, -1.0f, 1.0f);

		int numtrials = (int) AppStateVars.Trials.size();
		bool instructiontrial = false;

		if (currenttrial != NULL)
		{
			if (currenttrial->InstructionTrial)
				instructiontrial = true;
		}

		if (TrialIntro && AppStateVars.InIntroTrial)
		{
			TrialIntro->Render(false);
		}
		else if (instructiontrial)
		{
			currenttrial->RenderText(AppStateVars.ScreenWidth, AppStateVars.ScreenHeight);
		}
		else
		{
			glPushMatrix();
				glScalef(1.0f, -1.0f, -1.0f * SphereSquash);
				glutSolidSphere(SphereSize, SphereSegs, SphereSegs);
			glPopMatrix();
			
			renderingpotato = true;
		}

#ifndef NO_HAPTICS
		// Draw 3D cursor at haptic device position.	
		Haptics::DrawHapticCursor(renderingpotato);
#endif

		if ((AppStateVars.InIntroTrial == false) && (instructiontrial == false))
		{
			
			// Render the GUI
			AppGUI->OnPaint();
		}

	glPopMatrix();
}


void RenderSceneStereoGraphics(void)
{
	ShaderList[0]->Apply();

	// Identify the textures to use.
	ShaderList[0]->SetUniformVar1i(SVar_PermTexture, 0); // Texture unit 0

	ShaderList[0]->UnApply();

	bool renderingpotato = false;

	//////////////////
	// Left Eye
	//////////////////

	glDrawBuffer(GL_BACK_LEFT);

	int numtrials = (int) AppStateVars.Trials.size();
	bool instructiontrial = false;
	int specindex = 0;
	float clear_r = 0.5f;
	Trial* currenttrial = NULL;

	if ((AppStateVars.InPracticeTrial >= 0) && (AppStateVars.InPracticeTrial < (int)AppStateVars.PracticeTrials.size()))
		currenttrial = AppStateVars.PracticeTrials[AppStateVars.InPracticeTrial];
	else if ((AppStateVars.CurrentTrial >= 0) && (AppStateVars.CurrentTrial < (int)AppStateVars.Trials.size()))
		currenttrial = AppStateVars.Trials[AppStateVars.CurrentTrial];

	if (currenttrial != NULL)
	{
		if (currenttrial->HapticPresence && (instructiontrial == false))
		{
			if (currenttrial->GetStage() < 1)
			{
				if (currenttrial->GetBetweenStages())
				{
					clear_r = ((Trial::ZoneRad - currenttrial->LastRadius) / (2 * Trial::ZoneRad)) + 0.5f;
				}
			}
		}
	}

	glClearColor(clear_r, 0.5f, 0.5f, 0.5f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetupView(false);

	if (currenttrial != NULL)
	{
		if (currenttrial->InstructionTrial)
			instructiontrial = true;
	}

	glPushMatrix();
		glScalef(1.0f, -1.0f, 1.0f);
		
		if (TrialIntro && AppStateVars.InIntroTrial)
		{
			TrialIntro->Render(false);
		}
		else if (instructiontrial)
		{
			currenttrial->RenderText(AppStateVars.ScreenWidth, AppStateVars.ScreenHeight);
		}
		else
		{
			if (currenttrial->GetStage() < 1)
			{
				if (currenttrial->HapticPresence && (instructiontrial == false) && (currenttrial->GetBetweenStages()))
				{
					glEnable(GL_COLOR_MATERIAL);

					glColor3f(0.2f, 0.2f, 1.0f);

					glutSolidSphere(Trial::ZoneRad, 32, 32);
				}
				else
				{
					//==============================================================================//
					//WJA - Uncomment this line to kae trial render as a haptic trial (silhouette)
					// tried uncommenting this - expt them just hangs - nothing happens
					//==============================================================================//
					//currenttrial->VisualPresence = false;

					//if (0) // @@@@
					if ((!currenttrial->GetStarted()) || (currenttrial->VisualPresence == false))
					{
						if (currenttrial->IsPracticeTrial)
							currenttrial->Render();

						ShaderList[0]->Apply();
						
						if (currenttrial->GetStage() < 1)
							ShaderList[0]->SetUniformVar1f(SVar_Timer, currenttrial->GetTimer());

						// Identify the textures to use.
						//ShaderList[0]->SetUniformVar1i(SVar_PermTexture, 0); // Texture unit 0

						if (currenttrial->IsPracticeTrial)
						{
							// Let the shader know if we are rendering the L or R eye
							glPushMatrix();
								glScalef(0.07f, 0.07f, 0.07f);

								glActiveTexture(GL_TEXTURE0); // Activate a different texture unit (unit 1)
								glBindTexture(GL_TEXTURE_2D, permTextureID); // Bind the texture to first specular texture

								glPushMatrix();
									glTranslatef(-PracticeSeparation,0.0f,0.0f);
									
									DrawTexturedPlane(0.1, 128, 0.0f);
								glPopMatrix();
								
								glPushMatrix();
									glTranslatef(PracticeSeparation,0.0f,0.0f);
									
									DrawTexturedPlane(0.1, 128, 0.0f);
								glPopMatrix();

								glActiveTexture(GL_TEXTURE0); // Switch active texture unit back to 0 again
							glPopMatrix();
						}
						else
						{
							// Let the shader know if we are rendering the L or R eye
							glPushMatrix();
								glScalef(0.07f, 0.07f, 0.07f);

								glActiveTexture(GL_TEXTURE0); // Activate a different texture unit (unit 1)
								glBindTexture(GL_TEXTURE_2D, permTextureID); // Bind the texture to first specular texture

								glPushMatrix();
									glTranslatef(0.0f,0.0f,0.0f);
									
									DrawTexturedPlane(0.1, 128, 0.0f);
								glPopMatrix();

								glActiveTexture(GL_TEXTURE0); // Switch active texture unit back to 0 again
							glPopMatrix();
						}

						ShaderList[0]->UnApply();
						renderingpotato = true;
					}
					else
					{
						currenttrial->Render();
						renderingpotato = true;
					}
				}
			}
		}

#ifndef NO_HAPTICS
		// Draw 3D cursor at haptic device position.
		Haptics::DrawHapticCursor(renderingpotato);
#endif
		
		if (AppStateVars.InPracticeTrial >= 0)
		{
			AppGUI2->OnPaint();
		}
		else if ((AppStateVars.InIntroTrial == false) && (instructiontrial == false))
		{
			if (currenttrial->GetStage() > 0)
			{
				// Render the GUI
				AppGUI->OnPaint();
			}
		}

	glPopMatrix();

	
	//////////////////
	// Right Eye
	//////////////////

	renderingpotato = false;

	glDrawBuffer(GL_BACK_RIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	SetupView(true);

	glPushMatrix();
		glScalef(1.0f, -1.0f, 1.0f);		

		if (TrialIntro && AppStateVars.InIntroTrial)
		{
			TrialIntro->Render(true);
		}
		else if (instructiontrial)
		{
			currenttrial->RenderText(AppStateVars.ScreenWidth, AppStateVars.ScreenHeight);
		}
		else
		{
			if (currenttrial->GetStage() < 1)
			{
				if (currenttrial->HapticPresence && (instructiontrial == false) && (currenttrial->GetBetweenStages()))
				{
					glEnable(GL_COLOR_MATERIAL);

					glColor3f(0.2f, 0.2f, 1.0f);

					glutSolidSphere(Trial::ZoneRad, 32, 32);
				}
				else
				{
					//==============================================================================//
					//WJA - Uncomment this line to kae trial render as a haptic trial (silhoette)
					// tried uncommenting this - expt then just hangs - nothing happens
					//==============================================================================//
					//currenttrial->VisualPresence = false;

					//if (0) // @@@@
					if ((!currenttrial->GetStarted()) || (currenttrial->VisualPresence == false))
					{
						if (currenttrial->IsPracticeTrial)
							currenttrial->Render();

						ShaderList[0]->Apply();

						if (currenttrial->GetStage() < 1)
							ShaderList[0]->SetUniformVar1f(SVar_Timer, currenttrial->GetTimer());

						// Identify the textures to use.
						//ShaderList[0]->SetUniformVar1i(SVar_PermTexture, 0); // Texture unit 0

						if (currenttrial->IsPracticeTrial)
						{
							// Let the shader know if we are rendering the L or R eye
							glPushMatrix();
								glScalef(0.07f, 0.07f, 0.07f);

								glActiveTexture(GL_TEXTURE0); // Activate a different texture unit (unit 1)
								glBindTexture(GL_TEXTURE_2D, permTextureID); // Bind the texture to first specular texture

								glPushMatrix();
									glTranslatef(-PracticeSeparation,0.0f,0.0f);
									
									DrawTexturedPlane(0.1, 128, 0.0f);
								glPopMatrix();

								glPushMatrix();
									glTranslatef(PracticeSeparation,0.0f,0.0f);
									
									DrawTexturedPlane(0.1, 128, 0.0f);
								glPopMatrix();

								glActiveTexture(GL_TEXTURE0); // Switch active texture unit back to 0 again
							glPopMatrix();
						}
						else
						{
							// Let the shader know if we are rendering the L or R eye
							glPushMatrix();
								glScalef(0.07f, 0.07f, 0.07f);

								glActiveTexture(GL_TEXTURE0); // Activate a different texture unit (unit 1)
								glBindTexture(GL_TEXTURE_2D, permTextureID); // Bind the texture to first specular texture

								glPushMatrix();
									glTranslatef(0.0f,0.0f,0.0f);
									
									DrawTexturedPlane(0.1, 128, 0.0f);
								glPopMatrix();

								glActiveTexture(GL_TEXTURE0); // Switch active texture unit back to 0 again
							glPopMatrix();
						}

						ShaderList[0]->UnApply();
						renderingpotato = true;
					}
					else
					{
						currenttrial->Render();
						renderingpotato = true;
					}
				}
			}
		}

#ifndef NO_HAPTICS
		// Draw 3D cursor at haptic device position.
		Haptics::DrawHapticCursor(renderingpotato);
#endif
		
		if (AppStateVars.InPracticeTrial >= 0)
		{
			AppGUI2->OnPaint();
		}
		else if ((AppStateVars.InIntroTrial == false) && (instructiontrial == false))
		{
			if (currenttrial->GetStage() > 0)
			{
				// Render the GUI
				AppGUI->OnPaint();
			}
		}

	glPopMatrix();
}


/*******************************************************************************
 GLUT callback for redrawing the view.
*******************************************************************************/
void OnPaint()
{
#ifndef NO_HAPTICS
	// Render haptics
	RenderSceneHaptics();
#endif

	// Next render graphics
    RenderSceneStereoGraphics();

    glutSwapBuffers();
	glFlush();
	glFinish();
}


/*******************************************************************************
 GUI callback for haptic button presses
*******************************************************************************/
void OnButton(int butnum, bool state)
{
	if (AppStateVars.InPracticeTrial >= 0)
	{
		SetupNextTrial();
		
		AppGUI2->Reset();
	}
	else
	{
		//double gloss = GlossSlider->GetValue() / ((double)GlossSliderResolution);
		//double depth = DepthSlider->GetValue() / ((double)DepthSliderResolution);
		double gloss = GlossSlider->GetValue();
		double depth = DepthSlider->GetValue();

		// invert slider values to mirrored left tick is 0
		AppStateVars.Trials[AppStateVars.CurrentTrial]->SetResult(GlossSliderResolution - gloss, DepthSliderResolution - depth);

		GlossSlider->SetValue(GlossSliderResolution / 2, true);
		DepthSlider->SetValue(DepthSliderResolution / 2, true);
		
		if (AppStateVars.Trials[AppStateVars.CurrentTrial])
			AppStateVars.Trials[AppStateVars.CurrentTrial]->Stop();

		SetupNextTrial();
		
		AppGUI->Reset();
	}
}


void OnSlider(int slidnum, double value)
{
	/*if (AppStateVars.Trials[AppStateVars.CurrentTrial])
	{
		if (slidnum == 0)
			AppStateVars.Trials[AppStateVars.CurrentTrial]->GlossResult = value;
		else if (slidnum == 1)
			AppStateVars.Trials[AppStateVars.CurrentTrial]->ComplianceResult = value;
		else if (slidnum == 2)
			AppStateVars.Trials[AppStateVars.CurrentTrial]->FrictionResult = value;
	}*/
}


void OnTrialEnd(Trial* trial)
{
	if (trial)
		trial->Stop();

	SetupNextTrial();
}


void OnTrialStageEnd(Trial* trial)
{
	Trial* currenttrial = NULL;

	if ((AppStateVars.InPracticeTrial >= 0) && (AppStateVars.InPracticeTrial < (int)AppStateVars.PracticeTrials.size()))
		currenttrial = AppStateVars.PracticeTrials[AppStateVars.InPracticeTrial];
	else if ((AppStateVars.CurrentTrial >= 0) && (AppStateVars.CurrentTrial < (int)AppStateVars.Trials.size()))
		currenttrial = AppStateVars.Trials[AppStateVars.CurrentTrial];

	if (currenttrial->HapticPresence)
	{
		ShaderList[0]->Apply();

		if (SVar_Timer >= 0)
			ShaderList[0]->SetUniformVar1f(SVar_Timer, currenttrial->GetTimer());
	}

	if (currenttrial->GetStage() < 1)
	{
		if (currenttrial->HapticPresence)
			PerlinNoise::DrawPotato(currenttrial->GetStage(), currenttrial->GetTimer());

		//std::cout << "=== This Stage: " << currenttrial->GetStage() << "\n";
		//std::cout << "===   Is Standard: " << currenttrial->IsStandard() << "\n";
		//std::cout << "===   Timer: " << currenttrial->GetTimer() << "\n";
	}

	if (currenttrial->IsPracticeTrial)
	{
		if (currenttrial->GetStage() > 0)
			SetupNextTrial();
	}
}


void OnIntroTrialEnd(IntroTrial* trial)
{
	if (trial)
		trial->Stop();

	SetupNextTrial();
}



/*******************************************************************************
 Initialise display settings and set up rendering parameters
*******************************************************************************/
void InitGL()
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//glutInitWindowPosition(10,10);
	//glutInitWindowSize(AppStateVars.ScreenWidth,AppStateVars.ScreenHeight);
	//glutCreateWindow("GLUT GLSL");

	glutGameModeString("1280x1024:32@85" ); //the settings for fullscreen mode
	glutEnterGameMode(); //set glut to fullscreen using the settings in the line above

	glutDisplayFunc(OnPaint);
	glutIdleFunc(OnIdle);
	glutReshapeFunc(OnWindowResize);
	glutKeyboardFunc(OnKeyboard);
	glutMouseFunc(OnMouse);
	glutMotionFunc(OnMouseMove);
	glutSetCursor(GLUT_CURSOR_NONE);

	SetupStereoGlobals();

	/*// Light for cursor
	static const GLfloat light_model_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    static const GLfloat light0_diffuse[] = {0.6f, 0.6f, 0.6f, 0.9f};   
    static const GLfloat light0_direction[] = {0.0f, -0.4f, 1.0f, 0.0f}; */
        
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat light_diffuse[] = {0.6f, 0.6f, 0.6f, 0.9f};
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	//glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	
	GLfloat light0Pos[] = {0.0, 10.0, 3.0};//{1.0, 2.0, 3.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	//GLfloat light1Pos[] = {0.2, 0.2, 3.0};//{1.0, 2.0, 3.0, 1.0};
	//glLightfv(GL_LIGHT0, GL_POSITION, light1Pos);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.5,0.5,0.5,0.5);

	// Initialise the culling parameters
	// (which faces are considered as facing away and which to cull)
	glFrontFace(GL_CCW); // CANT CHANGE THIS WITHOUT AFFECTING OPENHAPTICS
	glCullFace(GL_BACK);  // INSTEAD, WE HAVE TO CULL FRONT DUE TO NEGATIVE SCALE FACTOR
	//glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	// Setup other misc features.
    //glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    
    // Setup lighting model.
    /*glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
    glEnable(GL_LIGHT0);*/
}


/*******************************************************************************
 Main entry
*******************************************************************************/
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

	// Initialise GLUT
	InitGL();
	
#ifndef NO_HAPTICS
	// Initialise OpenHaptics library
	Haptics::InitHL();
	
    // Provide a cleanup routine for handling application exit.
    atexit(Haptics::CleanupHaptics);
#endif

	// Check whether Shader extensions supported
	// (only checking for OpenGL 2.0)
	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else
	{
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

	// Load and initialise all shaders
	SetShaders();

	PerlinNoise::NumSegs = 56;
	PerlinNoise::OnBuild = OnBuildPotato;

    // Create and load the textures (generated, not read from a file)
    InitPermTexture(&permTextureID);
		
	// Setup the buttons and GUI
	SetupGUI();

	// Load and initialise all trials
	SetupIntroTrial();

	SetupNextTrial();

		// Load and initialise all trials
		//SetupTrials(1);
		//SetupNextTrial();

	// Enter main loop
	glutMainLoop();
 }
