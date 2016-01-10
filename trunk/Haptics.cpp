/***********************************************************
 *
 * Haptics.cpp
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
	#include "Haptics.h"
#endif

#ifndef NO_HAPTICS

std::string Haptics::ParameterFilePath = "ParamFiles/Params_P";


// Forward declaration of callbacks
void HLCALLBACK SphereTouchCallback(
					HLenum event,
					HLuint object,
					HLenum thread,
					HLcache *cache,
					void *userdata);


void HLCALLBACK SphereUnTouchCallback(
					HLenum event,
					HLuint object,
					HLenum thread,
					HLcache *cache,
					void *userdata);


/*******************************************************************************
 Initialize the HDAPI.  This involves initing a device configuration, enabling
 forces, and scheduling a haptic thread callback for servicing the device.
*******************************************************************************/
void Haptics::InitHL()
{
    HDErrorInfo error;

    ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
        fprintf(stderr, "Press any key to exit");
        getchar();
        exit(-1);
    }
    
    ghHLRC = hlCreateContext(ghHD);
    hlMakeCurrent(ghHLRC);

    // Enable optimization of the viewing parameters when rendering
    // geometry for OpenHaptics.
    hlEnable(HL_HAPTIC_CAMERA_VIEW);

    // Generate an ID for the sphere shape.
    gSphereShapeIdL = hlGenShapes(1);
    gSphereShapeIdR = hlGenShapes(1);
	// Generate an ID for the block (for trials without haptics)
    gBlockShapeId = hlGenShapes(1);

    // Generate ID's for the intro shapes.
    gIntroShapeId = hlGenShapes(1);
    gIntroBoxId = hlGenShapes(1);

	// Generate an ID for the gravity effect at start
	gIntroGravityId = hlGenEffects(1);
	gTrialGravityId = hlGenEffects(1);
	gSliderGravityId = hlGenEffects(1);

    //hlTouchableFace(HL_FRONT_AND_BACK);
    hlTouchableFace(HL_BACK);


	hlAddEventCallback(HL_EVENT_TOUCH, gSphereShapeIdL,
		HL_CLIENT_THREAD, &SphereTouchCallback, NULL);

	hlAddEventCallback(HL_EVENT_TOUCH, gSphereShapeIdR,
		HL_CLIENT_THREAD, &SphereTouchCallback, NULL);


	/*hlAddEventCallback(HL_EVENT_UNTOUCH, gSphereShapeIdL,
		HL_CLIENT_THREAD, &SphereUnTouchCallback, NULL);

	hlAddEventCallback(HL_EVENT_UNTOUCH, gSphereShapeIdR,
		HL_CLIENT_THREAD, &SphereUnTouchCallback, NULL);*/
	

	/*CustomSphere = new HapticSphere();
	CustomSphere->setRadius(SphereSize);
	CustomSphere->setSquash(SphereSquash);*/

	/*HLfloat vlim, flim;
	hdGetFloatv(HD_SOFTWARE_VELOCITY_LIMIT, &vlim); // Default 1200 mm/s
	hdGetFloatv(HD_SOFTWARE_FORCE_IMPULSE_LIMIT, &flim); // Default 4 N

	vlim = 600.0f;
	flim = 1;
	hdSetFloatv(HD_SOFTWARE_VELOCITY_LIMIT, &vlim); // Default 1200 mm/s
	hdSetFloatv(HD_SOFTWARE_FORCE_IMPULSE_LIMIT, &flim); // Default 4 N*/
}


/*******************************************************************************
 Draws a 3D cursor for the haptic device using the current local transform,
 the workspace to world transform and the screen coordinate scale.
*******************************************************************************/
void Haptics::DrawHapticCursor(bool hidecursor)
{
    static const double kCursorRadius = 0.5;
    static const double kCursorHeight = 1.5;
    static const int kCursorTess = 15;
    HLdouble proxyxform[16];
	double pos[3];
	double hidescale = 1.0;
	double rad, radfac;
	
	if (hidecursor)
	{
		// Hide cursor behind the ball		
		GetHapticPos(pos);
		rad = sqrt((pos[0] * pos[0]) + (pos[1] * pos[1]));
		radfac = (rad / SphereSize);
	}

    GLUquadricObj *qobj = 0;

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushMatrix();

    if (!gCursorDisplayList)
    {
        gCursorDisplayList = glGenLists(1);
        glNewList(gCursorDisplayList, GL_COMPILE);
        qobj = gluNewQuadric();
               
        gluCylinder(qobj, 0.0, kCursorRadius, kCursorHeight,
                    kCursorTess, kCursorTess);
        glTranslated(0.0, 0.0, kCursorHeight);
        gluCylinder(qobj, kCursorRadius, 0.0, kCursorHeight / 5.0,
                    kCursorTess, kCursorTess);
    
        gluDeleteQuadric(qobj);
        glEndList();
    }

	/*if (hidecursor)
	{
		// Hide cursor behind the ball		
		if ((pos[2] > 0) && (rad < SphereSize))
			glTranslated(0.0, 0.0, 0.2 * (1.0 - radfac));
	}*/
    
    // Get the proxy transform in world coordinates.
    hlGetDoublev(HL_PROXY_TRANSFORM, proxyxform);
    glMultMatrixd(proxyxform);

	/////////////////////////////////////
	//hduMatrix proxyxformB;//Declare as hduMatrix instead of double[16]
	//hlGetDoublev(HL_PROXY_TRANSFORM, proxyxformB);
	//proxyxformB = proxyxformB * hduMatrix::createScale(1,-1,1);//Scale this matrix locally
	//glMultMatrixd(proxyxformB);
	/////////////////////////////////////

    // Apply the local cursor scale factor.
    glScaled(gCursorScale, gCursorScale, gCursorScale);

	if (hidecursor)
	{
		// Hide cursor behind the ball
		if ((pos[2] > 0) && (rad < SphereSize))
		{
			hidescale*= (radfac * radfac);
			hidescale/= (1.0 + (10.0 * pos[2]));			
			glScaled(hidescale, hidescale, hidescale);
		}
	}

    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0.0, 0.5, 1.0);

    glCallList(gCursorDisplayList);

    glPopMatrix(); 
    glPopAttrib();
}


/*******************************************************************************
 This handler is called when the application is exiting.  Deallocates any state 
 and cleans up.
*******************************************************************************/
void Haptics::CleanupHaptics()
{
	Quitting = true;

    // Deallocate the sphere shape id we reserved in initHL.
    hlDeleteShapes(gSphereShapeIdL, 1);
    hlDeleteShapes(gSphereShapeIdR, 1);
    hlDeleteShapes(gIntroShapeId, 1);
    hlDeleteShapes(gIntroBoxId, 1);

    hlDeleteEffects(gIntroGravityId, 1);
    hlDeleteEffects(gTrialGravityId, 1);
    hlDeleteEffects(gSliderGravityId, 1);

    // Free up the haptic rendering context.
    hlMakeCurrent(NULL);
    if (ghHLRC != NULL)
    {
        hlDeleteContext(ghHLRC);
    }

    // Free up the haptic device.
    if (ghHD != HD_INVALID_HANDLE)
    {
        hdDisableDevice(ghHD);
    }
}


/*******************************************************************************
 Use the current OpenGL viewing transforms to initialize a transform for the
 haptic device workspace so that it's properly mapped to world coordinates.
*******************************************************************************/
void Haptics::UpdateHapticWorkspace()
{
	if (Quitting)
		return;

    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
	/////////////////////////////////////////////////////////////////////////
	// Set the VIEWTOUCH matrix using the utility function: hluFeelFrom()
	// We do this to rotate the workspace 90° so the phantom acts as though
	// it is to the right of you (instead of in front of you, the default).
	/////////////////////////////////////////////////////////////////////////
	hlMatrixMode(HL_VIEWTOUCH);
    hlLoadIdentity();

	// hluFeelFrom() is similar to gluLookAt().
	// We specify where the camera is, where we are looking,
	// and also an up vector.

	// Camera placed looking from left
	HLdouble handx = -1, handy = 0, handz = 0;

	// Looking at scene origin:
	HLdouble centerx = 0, centery = 0, centerz = 0;

	// Up vector:
	// We use a rotated Up Vector to compensate for the tilt of the mirror.
	double tiltback = 60.0;
	tiltback *= (3.1415927 / 180.0); // Convert to radians

	HLdouble upx = 0, upy = cos(tiltback), upz = sin(tiltback);
	hluFeelFrom(handx, handy, handz, centerx, centery, centerz, upx, upy, upz);
	

	//////////////////////////////////////////////////////////////////////////////////
	// Set the TOUCHWORKSPACE matrix using the utility function: hluFitWorkspaceBox()
	// This correctly maps the real world to GL units, and ensures uniform
	// size (so objects are not stretched or distorted).
	//////////////////////////////////////////////////////////////////////////////////
	hlMatrixMode(HL_TOUCHWORKSPACE);
    hlLoadIdentity();

	// The extents controls the 'haptic size' of the objects:
	// A larger value means the Phantom arm has greater reach of GL positions,
	// so objects are therefore smaller.
	HLdouble extents = 0.12; // Plus/Minus 12cm minimum range
	HLdouble minPoint[3] = {-extents, -extents, -extents};
	HLdouble maxPoint[3] = {extents, extents, extents};
    hluFitWorkspaceBox(modelview,minPoint,maxPoint);
    //hluFitWorkspace(projection);

	// Finally translate the TOUCHWORKSPACE matrix to move
	// the origin to just below the mirror, so the sphere is
	// in the correct place.
	//hlTranslatef(0.11f, -0.1f, 0.01f);
	hlTranslatef(0.015f, -0.11f, 0.0f);//-0.06f);
	// These are actually the [Y, Z, X] coordinates

    // Compute cursor scale.
    gCursorScale = hluScreenToModelScale(modelview, projection, viewport);
    gCursorScale *= CURSOR_SIZE_PIXELS;
}



/*******************************************************************************
 Utility functions for getting HL material
 parameters given the squish factor
*******************************************************************************/
float Haptics::GetStiffness(int compliance)
{
	if (compliance < 0)
		return Trial::StiffnessValues[0];

	if (compliance >= Trial::NumValues)
		return Trial::StiffnessValues[Trial::NumValues-1];

	return Trial::StiffnessValues[compliance];
}

float Haptics::GetDamping(int compliance)
{
	if (compliance < 0)
		return Trial::DampingValues[0];

	if (compliance >= Trial::NumValues)
		return Trial::DampingValues[Trial::NumValues-1];

	return Trial::DampingValues[compliance];
}

float Haptics::GetStaticFriction(int friction)
{
	if (friction < 0)
		return Trial::StaticFrictionValues[0];

	if (friction >= Trial::NumValues)
		return Trial::StaticFrictionValues[Trial::NumValues-1];

	return Trial::StaticFrictionValues[friction];
}

float Haptics::GetDynamicFriction(int friction)
{
	if (friction < 0)
		return Trial::DynamicFrictionValues[0];

	if (friction >= Trial::NumValues)
		return Trial::DynamicFrictionValues[Trial::NumValues-1];

	return Trial::DynamicFrictionValues[friction];
}


/*******************************************************************************
 Utility function for getting current HL proxy position
*******************************************************************************/
void Haptics::GetHapticPos(double* pos)
{
	if (pos)
	{
		// Get the proxy transform in world coordinates.
		hlGetDoublev(HL_PROXY_POSITION, pos);
	}
}


/*******************************************************************************
 Gravity controls
*******************************************************************************/
void Haptics::StartSliderGravity()
{
	hlCallback(HL_EFFECT_COMPUTE_FORCE, (HLcallbackProc) ComputeSliderGravForce, &AppStateVars);
	hlCallback(HL_EFFECT_START, (HLcallbackProc) StartGravEffect, (void*)&AppStateVars);
	hlCallback(HL_EFFECT_STOP, (HLcallbackProc) StopGravEffect, (void*)&AppStateVars);
	//hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.4);
	//hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 0.4);
	hlStartEffect(HL_EFFECT_CALLBACK, gSliderGravityId);
}


void Haptics::StopSliderGravity()
{
	hlStopEffect(gSliderGravityId);
}


/*******************************************************************************
 Gravity Effect callbacks
*******************************************************************************/

void HLCALLBACK Haptics::ComputeGravForce(HDdouble outforce[3], HLcache *cache, void *userdata)
{
	//AppStateVars *appstate = (AppStateVars *) userdata;
	double forcemag = 1;

	// Get the current proxy position from the state cache.
	// Note: the effect state cache for effects is
	// maintained in workspace coordinates, so its data can
	// be used without transformation for computing forces.
	//hduVector3Dd currentPos;
	//hlCacheGetDoublev(cache, HL_PROXY_POSITION, currentPos);
	hlCacheGetDoublev(cache, HL_PROXY_POSITION, AppStateVars.LastPosition);

	// Compute a constant force pointing towards the anchor point
	hduVector3Dd anchor;

	// Define the anchor point to be the centre
	//anchor[0] = 29; anchor[1] = 100; anchor[2] = 6;
	// NEWER: anchor[0] = 74; anchor[1] = 182; anchor[2] = 14;

	// From lastcentrepos.txt:
	anchor[0] = 76.5988; anchor[1] = 184.559; anchor[2] = 23.9805;

	hduVector3Dd force = anchor - AppStateVars.LastPosition;
	double forcelen = ((force[0] * force[0]) + (force[1] * force[1]) + (force[2] * force[2]));
	forcelen = sqrt(forcelen);

	// To prevent instability, scale down forces
	// when we are close to the anchor point
	if (forcelen <  20)
	{
		double scal2 = 20 / forcelen;
		forcelen*= scal2 * scal2;
	}

	double scalefac = forcemag / forcelen;
	force[0]*= scalefac;
	force[1]*= scalefac;
	force[2]*= scalefac;

	// Accumulate our computed force with the current force
	// from the haptic rendering pipeline.
	outforce[0] += force[0];
	outforce[1] += force[1];
	outforce[2] += force[2];
}


void HLCALLBACK Haptics::ComputeInverseGravForce(HDdouble outforce[3], HLcache *cache, void *userdata)
{
	//AppStateVars *appstate = (AppStateVars *) userdata;
	double forcemag = 1;

	// Get the current proxy position from the state cache.
	// Note: the effect state cache for effects is
	// maintained in workspace coordinates, so its data can
	// be used without transformation for computing forces.
	//hduVector3Dd currentPos;
	//hlCacheGetDoublev(cache, HL_PROXY_POSITION, currentPos);
	//hlCacheGetDoublev(cache, HL_PROXY_POSITION, AppStateVars.LastPosition);

	// Compute a constant force pointing towards the anchor point
	hduVector3Dd anchor;

	// Define the anchor point to be the centre
	anchor[0] = 30.5; anchor[1] = 79.25; anchor[2] = 18;
	// NEWER: anchor[0] = 74; anchor[1] = 182; anchor[2] = 14;

	// From lastcentrepos.txt:
	//anchor[0] = 76.5988; anchor[1] = 184.559; anchor[2] = 23.9805;

	hduVector3Dd proxypos;
	hlCacheGetDoublev(cache, HL_PROXY_POSITION, proxypos);

	// Get proxy position in world coordinates.
	//hlGetDoublev(HL_PROXY_POSITION, proxypos);

	hduVector3Dd force; // = anchor - AppStateVars.LastPosition;
	force[0] = anchor[0]-proxypos[0]; force[1] = anchor[1]-proxypos[1]; force[2] = anchor[2]-proxypos[2];
	
	double forcelen = ((force[0] * force[0]) + (force[1] * force[1]) + (force[2] * force[2]));
	forcelen = sqrt(forcelen);

	// To prevent instability, scale down forces
	// when we are close to the anchor point
	/*if (forcelen <  20)
	{
		double scal2 = 0.1 * forcelen;
		forcelen*= scal2 * scal2;
	}*/

	double scalefac = 0.1;
	
	if (forcelen != 0)
		scalefac = -forcemag / forcelen;

	force[0]*= scalefac;
	force[1]*= scalefac;
	force[2]*= scalefac;

	// Accumulate our computed force with the current force
	// from the haptic rendering pipeline.
	outforce[0] += force[0];
	outforce[1] += force[1];
	outforce[2] += force[2];
}


void HLCALLBACK Haptics::ComputeSliderGravForce(HDdouble outforce[3], HLcache *cache, void *userdata)
{
	//AppStateVars *appstate = (AppStateVars *) userdata;
	double forcemag = AppStateVars.SliderGravForce;

	// Get the current proxy position from the state cache.
	// Note: the effect state cache for effects is
	// maintained in workspace coordinates, so its data can
	// be used without transformation for computing forces.
	//hduVector3Dd currentPos;
	//hlCacheGetDoublev(cache, HL_PROXY_POSITION, currentPos);
	hlCacheGetDoublev(cache, HL_PROXY_POSITION, AppStateVars.LastPosition);

	// Compute a constant force pointing towards the anchor point
	hduVector3Dd anchor = AppStateVars.Anchor;

	hduVector3Dd force = anchor - AppStateVars.LastPosition;
	double forcelen = ((force[0] * force[0]) + (force[1] * force[1]) + (force[2] * force[2]));
	forcelen = sqrt(forcelen);

	double scalefac = forcemag * forcelen;
	force[0]*= scalefac;
	force[1]*= scalefac;
	force[2]*= scalefac;

	// Accumulate our computed force with the current force
	// from the haptic rendering pipeline.
	outforce[0] += force[0];
	outforce[1] += force[1];
	outforce[2] += force[2];
}


void HLCALLBACK Haptics::StartGravEffect(HLcache *cache, void *userdata)
{
	// Do nothing
}


void HLCALLBACK Haptics::StopGravEffect(HLcache *cache, void *userdata)
{
	// Do nothing
}
#endif
