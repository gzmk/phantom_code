/***********************************************************
 *
 * Globals.h
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
 * This file defines the global variables that all files
 * have access to if they include Globals.h.  Globals are
 * necessary for interop with C, and doing it this way
 * also makes it a bit easier to split the program up.
 *
 ***********************************************************/

#ifndef GLOBALS__H
#define GLOBALS__H


#define CURSOR_SIZE_PIXELS 20


typedef void(*ButtonCallback)(int,bool);
typedef void(*SliderCallback)(int,double);


#include <GL/glew.h>
#include <GL/glut.h>

#ifndef NO_HAPTICS
	// OpenHaptics Headers
	#include <HL/hl.h>
	#include <HDU/hduMatrix.h>
	#include <HDU/hduError.h>
	#include <HLU/hlu.h>
#else
	#include <Windows.h>
#endif


#ifndef APPSTATE__H
	#include "AppState.h"
#endif

class ShaderMan;
class IntroTrial;
class HapticGUI;
class ButtonWidget;
class SliderWidget;

extern AppState AppStateVars;
extern HapticGUI* AppGUI;
extern ButtonWidget* OKButton;
extern SliderWidget* GlossSlider;
extern SliderWidget* DepthSlider;
extern HapticGUI* AppGUI2;
extern ButtonWidget* OKButton2;
extern int GlossSliderResolution;
extern int DepthSliderResolution;
extern bool Quitting;

// Shader variables
extern GLuint VertexShader;
extern GLuint FragmentShaderA;
extern GLuint FragmentShaderB;
extern GLuint ShaderProgram;

extern GLuint permTextureID;

extern int SVar_Timer;
extern int SVar_PermTexture;


// Variables used in rendering scene
//extern float LightPos[4];
//extern float SpinCounter;
extern float SphereSize;
extern int SphereSegs;
extern float SphereSquash;

extern float PracticeSeparation;

extern float Timer;


// Introduction trial to guide proxy to desired start position
extern IntroTrial* TrialIntro;

// Uniform variables passed to the shader
// (uniform variables cannot be changed per vertex)
// - NONE

extern ShaderMan** ShaderList;
extern unsigned int NumShaders;

extern float StereoSepDist;
extern float CamDist;
extern float CubeCentre;
extern float CubeSize;
extern double CamNear;
extern double CamFar;
extern double CamFOV;

#ifndef NO_HAPTICS
	// Haptic device and rendering context handles.
	extern HHD ghHD;
	extern HHLRC ghHLRC;

	// Shape id for shape we will render haptically.
	extern HLuint gSphereShapeIdL;
	extern HLuint gSphereShapeIdR;
	extern HLuint gBlockShapeId;

	extern HLuint gIntroShapeId;
	extern HLuint gIntroBoxId;
	extern HLuint gIntroGravityId;
	extern HLuint gTrialGravityId;
	extern HLuint gSliderGravityId;

	extern double gCursorScale;
	extern GLuint gCursorDisplayList;

	extern bool UseInBetweenStage;
#endif


////////////////////////////////////////
// Stereo Camera setup variables
////////////////////////////////////////
extern bool UseHollimanCamera; // Whether to use Nick Holliman's 'parallel' camera model
extern bool SwappedLR;

extern float Stereo_w; // Physical display dimensions in mm.
extern float Stereo_h; //

extern float Stereo_half_fov;
extern float Stereo_fov; // fov in y direction.

// Frustrum planes used in perspective definition
extern float Stereo_L_l;
extern float Stereo_L_r;
extern float Stereo_L_b;
extern float Stereo_L_t;

extern float Stereo_R_l;
extern float Stereo_R_r;
extern float Stereo_R_b;
extern float Stereo_R_t;

extern bool CurrentlyDisablingDevice;


void SetupStereoGlobals();

double DistanceToLineSegment(double* start, double* end, double* p, double *out_t);

void TransformVec(GLfloat* vec, GLfloat* mat, GLfloat* res);
////////////////////////////////////////


void DisableDevice(int disablems);

VOID CALLBACK DisableTimerProc(HWND hwnd,
				UINT uMsg,
				UINT_PTR idEvent,
				DWORD dwTime);

#endif