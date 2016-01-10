/***********************************************************
 *
 * Globals.cpp
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
 * This file declares and initialises the global variables
 * that all files have access to if they include Globals.h.
 * Globals are necessary for interop with C, and doing it
 * this way also makes it a bit easier to split the program
 * up.
 *
 ***********************************************************/

#ifndef GLOBALS__H
	#include "Globals.h"
#endif


AppState AppStateVars;
bool Quitting = false;

// Shader variables
GLuint VertexShader = 0;
GLuint FragmentShaderA = 0;
GLuint FragmentShaderB = 0;
GLuint ShaderProgram = 0;

GLuint permTextureID = 0;

int SVar_Timer = -1;
int SVar_PermTexture = -1;

HapticGUI* AppGUI = NULL;
ButtonWidget* OKButton = NULL;
SliderWidget* GlossSlider = NULL;
SliderWidget* DepthSlider = NULL;
HapticGUI* AppGUI2 = NULL;
ButtonWidget* OKButton2 = NULL;
int GlossSliderResolution = 12;
int DepthSliderResolution = 12;

// Variables used in rendering scene
float SphereSize = 0.03f;
int SphereSegs = 50;
float SphereSquash = 0.4f;

float PracticeSeparation = 0.8f;

float Timer = 0.0f;

// Introduction trial to guide proxy to desired start position
IntroTrial* TrialIntro = NULL;

ShaderMan** ShaderList = NULL;
unsigned int NumShaders = 0;

//float StereoSepDist = 0.0325f; //0.054f;
float StereoSepDist = 0.0f;
float CamDist = 0.55f;
//float StereoSepDist = 0.054f;
float CubeCentre = 0.0f;
float CubeSize = 0.2f;
double CamNear = 0.3;
double CamFar = 0.7;
double CamFOV = 26.6;

#ifndef NO_HAPTICS
	// Haptic device and rendering context handles.
	HHD ghHD = HD_INVALID_HANDLE;
	HHLRC ghHLRC = 0;

	// Shape id for shape we will render haptically.
	HLuint gSphereShapeIdL = 0;
	HLuint gSphereShapeIdR = 0;
	HLuint gBlockShapeId = 0;

	HLuint gIntroShapeId = 0;
	HLuint gIntroBoxId = 0;
	HLuint gIntroGravityId = 0;
	HLuint gTrialGravityId = 0;
	HLuint gSliderGravityId = 0;

	double gCursorScale = 0;
	GLuint gCursorDisplayList = 0;

	bool UseInBetweenStage = true;
#endif


////////////////////////////////////////
// Stereo Camera setup variables
////////////////////////////////////////
bool UseHollimanCamera = false; // Whether to use Nick Holliman's 'parallel' camera model
bool SwappedLR = false;

float Stereo_w = 0.3483f;//0.387f;//0.5184f; // Physical display dimensions in mm.
float Stereo_h = 0.2673f;//0.297f;//0.324f; //

float Stereo_half_fov = 0.0f;
float Stereo_fov = 0.0f; // FOV in y direction (not defined here, calculated later)

// Frustrum planes used in perspective definition
float Stereo_L_l = 0.0f;
float Stereo_L_r = 0.0f;
float Stereo_L_b = 0.0f;
float Stereo_L_t = 0.0f;

float Stereo_R_l = 0.0f;
float Stereo_R_r = 0.0f;
float Stereo_R_b = 0.0f;
float Stereo_R_t = 0.0f;

bool CurrentlyDisablingDevice = false;


void SetupStereoGlobals()
{
	float rad2deg = 180.0f / 3.14159265358979323846f;
	float Stereo_A = 2 * StereoSepDist;
	float Stereo_Z = CamDist;
	float Stereo_N = CamNear;

	Stereo_half_fov = atan( (Stereo_h/2.0) / Stereo_Z );
	Stereo_fov = rad2deg * ( 2.0 * Stereo_half_fov ); // fov in y direction.

	Stereo_L_l =  -( Stereo_N * ( ( (Stereo_w / 2.0f) - (Stereo_A / 2.0f) ) / Stereo_Z ) );
	Stereo_L_r =  ( Stereo_N * ( ( (Stereo_w / 2.0f) + (Stereo_A / 2.0f) ) / Stereo_Z ) );
	Stereo_L_b = - ( Stereo_N * ( (Stereo_h / 2.0f) / Stereo_Z) );
	Stereo_L_t =   ( Stereo_N * ( (Stereo_h / 2.0f) / Stereo_Z) );

	Stereo_R_l =  -( Stereo_N * ( ( (Stereo_w / 2.0f) + (Stereo_A / 2.0f) ) / Stereo_Z ) );
	Stereo_R_r =  ( Stereo_N * ( ( (Stereo_w / 2.0f) - (Stereo_A / 2.0f) ) / Stereo_Z ) );
	Stereo_R_b = - ( Stereo_N * ( (Stereo_h / 2.0f) / Stereo_Z) );
	Stereo_R_t =   ( Stereo_N * ( (Stereo_h / 2.0f) / Stereo_Z) );
}


double DistanceToLineSegment(double* start, double* end, double* p, double *out_t)
{
	double start_to_p[] = {(p[0] - start[0]), (p[1] - start[1]), (p[2] - start[2])};
	double start_to_end[] = {(end[0] - start[0]), (end[1] - start[1]), (end[2] - start[2])};

	double t = (start_to_p[0] * start_to_end[0]) + (start_to_p[1] * start_to_end[1]) + (start_to_p[2] * start_to_end[2]);
	t = t / ((start_to_end[0]*start_to_end[0]) + (start_to_end[1]*start_to_end[1]) + (start_to_end[2]*start_to_end[2]));
	
	double dist = 0;

	if (t < 0)
	{
		// Distance is from p to start
		dist = sqrt((start_to_p[0] * start_to_p[0]) + (start_to_p[1] * start_to_p[1]) + (start_to_p[2] * start_to_p[2]));
	}
	else if (t > 1)
	{
		// Distance is from p to end
		double p_to_end[] = {(end[0] - p[0]), (end[1] - p[1]), (end[2] - p[2])};
		dist = sqrt((p_to_end[0] * p_to_end[0]) + (p_to_end[1] * p_to_end[1]) + (p_to_end[2] * p_to_end[2]));
	}
	else
	{
		// Distance is from p to p2
		double p2[] = {(start[0] + (t * start_to_end[0])), (start[1] + (t * start_to_end[1])), (start[2] + (t * start_to_end[2]))};
		double p_to_p2[] = {(p2[0] - p[0]), (p2[1] - p[1]), (p2[2] - p[2])};

		dist = sqrt((p_to_p2[0] * p_to_p2[0]) + (p_to_p2[1] * p_to_p2[1]) + (p_to_p2[2] * p_to_p2[2]));
	}

	if (out_t != NULL)
		*out_t = t;

	return dist;
}


void TransformVec(GLfloat* vec, GLfloat* mat, GLfloat* res)
{
	bool trans = false;
	
	if (trans)
	{
		res[0] = mat[0]*vec[0] + mat[4]*vec[1] + mat[8]*vec[2] + mat[12]*vec[3];
		res[1] = mat[1]*vec[0] + mat[5]*vec[1] + mat[9]*vec[2] + mat[13]*vec[3];
		res[2] = mat[2]*vec[0] + mat[6]*vec[1] + mat[10]*vec[2] + mat[14]*vec[3];
		res[3] = mat[3]*vec[0] + mat[7]*vec[1] + mat[11]*vec[2] + mat[15]*vec[3];
	}
	else
	{
		res[0] = mat[0]*vec[0] + mat[1]*vec[1] + mat[2]*vec[2] + mat[3]*vec[3];
		res[1] = mat[4]*vec[0] + mat[5]*vec[1] + mat[6]*vec[2] + mat[7]*vec[3];
		res[2] = mat[8]*vec[0] + mat[9]*vec[1] + mat[10]*vec[2] + mat[11]*vec[3];
		res[3] = mat[12]*vec[0] + mat[13]*vec[1] + mat[14]*vec[2] + mat[15]*vec[3];
	}
}


void DisableDevice(int disablems)
{
#ifndef NO_HAPTICS
	if (CurrentlyDisablingDevice == false)
	{		
		std::cout << '\a';
		hdDisable(HD_FORCE_OUTPUT);
	}

	CurrentlyDisablingDevice = true;
#endif

	// Start timers
	UINT timerID = 101;
	SetTimer(NULL, timerID, (UINT) disablems, DisableTimerProc);
}

VOID CALLBACK DisableTimerProc(HWND hwnd,
				UINT uMsg,
				UINT_PTR idEvent,
				DWORD dwTime)
{
#ifndef NO_HAPTICS
	//KillTimer(NULL, idEvent);
	hdEnable(HD_FORCE_OUTPUT);
#endif

	CurrentlyDisablingDevice = false;
}
