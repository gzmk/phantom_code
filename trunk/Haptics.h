/***********************************************************
 *
 * Haptics.h
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
 * This file encapsulates most of the implementation
 * details regarding setting up the Phantom device,
 * responding to changes in view, and drawing the cursor.
 *
 ***********************************************************/


#ifndef HAPTICS__H
#define HAPTICS__H

#ifndef NO_HAPTICS
// OpenHaptics Headers
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
///////////////////
#include <HDU/hduMatrix.h>
#include <HDU/hduBoundBox.h>
#include <HDU/hdu.h>

#include "TrialMan.h"

#include <math.h>


#ifndef GLOBALS__H
	#include "Globals.h"
#endif

#define MAXINPUTVALS	50


class Haptics
{
  public:

	Haptics();

  public:

	static void InitHL();

	static void DrawHapticCursor(bool hidecursor);

	static void CleanupHaptics();

	static void UpdateHapticWorkspace();

	// Utility functions for getting HL material
	// parameters given the squish factor
	static float GetStiffness(int compliance);
	static float GetDamping(int compliance);

	static float GetStaticFriction(int friction);
	static float GetDynamicFriction(int friction);

	// Utility function for getting the current HL proxy position
	static void GetHapticPos(double* pos);

	static void StartSliderGravity();

	static void StopSliderGravity();

	// Gravity Haptic Callbacks
	static void HLCALLBACK ComputeGravForce(HDdouble force[3], HLcache *cache, void *userdata);

	static void HLCALLBACK ComputeInverseGravForce(HDdouble force[3], HLcache *cache, void *userdata);

	static void HLCALLBACK ComputeSliderGravForce(HDdouble force[3], HLcache *cache, void *userdata);

	static void HLCALLBACK StartGravEffect(HLcache *cache, void *userdata);

	static void HLCALLBACK StopGravEffect(HLcache *cache, void *userdata);

public:

	static std::string ParameterFilePath;
};

#endif


#endif