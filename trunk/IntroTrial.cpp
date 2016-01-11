/***********************************************************
 *
 * IntroTrial.cpp
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

#ifndef INTROTRIAL__H
	#include "IntroTrial.h"
#endif


IntroTrial* IntroTrial::CurrentTrial = NULL;
UINT IntroTrial::TimerID = 11;

const float IntroTrial::HSize = 0.02f;
const float IntroTrial::VSize = 0.02f;
const float IntroTrial::DSize = 0.02f;
const float IntroTrial::HSizeBox = 0.1f;
const float IntroTrial::VSizeBox = 0.2f;
const float IntroTrial::DSizeBox = 0.2f;
const double IntroTrial::ZoneRad = 0.01;
const double IntroTrial::ZoneOffset = -0.1;
const int IntroTrial::RefreshPeriod = 50;


///////////////////////////////////////////
// Class holding the variables per trial
///////////////////////////////////////////
// Constructor
IntroTrial::IntroTrial(IntroTrialEndNotifier notifier)
{
	// Setup default values when class is constructed
	TestTime = 60000;
	ZoneTime = 2000;

	// Results
	TrialTime = 0;

	_notifier = notifier;
	_stage = -1;
	_lastZoneTicks.QuadPart = 0;
	_inZone = false;

	_numVerts = 0;
	_numVertsBox = 0;

	_verts = NULL;
	_vertsBox = NULL;

	_participantNumber = 0;

	BuildBoundingBox(); //line 495
	BuildSurface();
}


// Destructor
IntroTrial::~IntroTrial()
{
	KillTimer(NULL, IntroTrial::TimerID);

	delete _verts;
	delete _vertsBox;
}

int IntroTrial::GetStage()
{
	return _stage;
}

void IntroTrial::SetStage(int stage)
{
	_stage = stage;
	_inZone = false;
}

bool IntroTrial::InZone()
{
	return _inZone;
}

void IntroTrial::Start()
{
	IntroTrial::CurrentTrial = this;

	return;
	TrialTime = 0;
	_inZone = false;

	QueryPerformanceCounter(&_lastZoneTicks);

	// Start timers
	SetTimer(NULL, IntroTrial::TimerID, (UINT) IntroTrial::RefreshPeriod, TimerProc);
}


void IntroTrial::Stop()
{
	// No need to stop timer as only called after last timer called
	IntroTrial::CurrentTrial = NULL;
}


void IntroTrial::Notify()
{
	if (_notifier)
		_notifier(this);
	
	_stage = 2;
}


void IntroTrial::OnKeyPress(unsigned char key)
{
	if (key == 255)
	{
		if (_stage < -1)
			_stage++;
		else /*if (_stage < 0)*/
		{
			_stage = 0;
			TrialTime = 0;
			_inZone = false;

			QueryPerformanceCounter(&_lastZoneTicks);

			// Start timers
			SetTimer(NULL, IntroTrial::TimerID, (UINT) IntroTrial::RefreshPeriod, TimerProc);
		}
	}
	else if (key == 254)
	{
		if (_stage == -1)
			RemoveFromNumber(_participantNumber);
	}
	else
	{
		if (_stage == -1)
			AddToNumber((int) key, _participantNumber);
	}
}


int IntroTrial::GetParticipantNumber()
{
	return _participantNumber;
}


void IntroTrial::Render(bool righteye)
{
	// Just render both the bounding box and the surface
	// (ignore the stereo parameter for now).

	//glScalef(-1.0f,1.0f,1.0f);
	RenderBoundingBox(true);
	RenderSurface();

	RenderText();
}

void IntroTrial::RenderSurface()
{
	if (_stage > 0)
	{
		if (_inZone)
			glColor4f(0.8f, 0.3f, 0.3f, 0.5f);
		else			
			glColor4f(0.6f, 0.4f, 0.4f, 0.5f);
		
		glPushMatrix();

		glScalef(1.0f, -1.0f, 1.0f);
		glTranslatef(0.0f, 0.0f, ZoneOffset);
		glutSolidSphere(ZoneRad, 32, 16);

		glPopMatrix();

		/*if (_inZone)
			glColor3f(0.8f, 0.3f, 0.3f);
		else			
			glColor3f(0.6f, 0.4f, 0.4f);

		// Render a touchable surface in the centre
		glEnableClientState(GL_VERTEX_ARRAY);

		// Draw vertex array
		glVertexPointer(3, GL_FLOAT, 0, _verts);
		glDrawArrays(GL_TRIANGLES, 0, _numVerts);*/
	}
}

void IntroTrial::RenderBoundingBox(bool flip)
{
	if (_stage > 0)
	{
		if (_inZone)
			glColor3f(0.3f, 0.8f, 0.3f);
		else
			glColor3f(0.4f, 0.6f, 0.4f);

		// Render a bounding box
		/*glEnableClientState(GL_VERTEX_ARRAY);

		// Draw vertex array
		glVertexPointer(3, GL_FLOAT, 0, _vertsBox);
		glDrawArrays(GL_TRIANGLES, 0, _numVertsBox);*/

		glPushMatrix();

		if (flip)
			glScalef(1.0f, -1.0f, 1.0f);

		glTranslatef(0.0f, 0.0f, ZoneOffset - HSizeBox);
		glutSolidSphere(HSizeBox, 32, 16);

		glPopMatrix();
	}
}


VOID CALLBACK IntroTrial::TimerProc(HWND hwnd,
				UINT uMsg,
				UINT_PTR idEvent,
				DWORD dwTime)
{
	//KillTimer(hwnd, idEvent);

	if (IntroTrial::CurrentTrial)
	{
#ifndef NO_HAPTICS
		HLdouble proxypos[3];

		// Get proxy position in world coordinates.
		hlGetDoublev(HL_PROXY_POSITION, proxypos);
#endif

		int stage = IntroTrial::CurrentTrial->GetStage();

		if (stage == 0)
		{
			// Check if within sensible place
			// If so, set up gravity and move to stage 1
#ifndef NO_HAPTICS
			if (proxypos[2] < -0.02)
#endif
			{
				IntroTrial::CurrentTrial->SetStage(1);
				AppStateVars.FirstFrame = true;
			}
		}
		else if (stage == 1)
		{
#ifndef NO_HAPTICS
			double dxsq = ((double) proxypos[0]) * ((double) proxypos[0]);
			double dysq = ((double) proxypos[1]) * ((double) proxypos[1]);
			double dz = ((double) proxypos[2]) - IntroTrial::ZoneOffset;
			double dzsq = dz * dz;

			double rad = sqrt(dxsq + dysq + dzsq);

			bool withinzone = (rad < IntroTrial::ZoneRad);

			// Check if within zone for long enough
			if (IntroTrial::CurrentTrial->InZone())
			{
				if (withinzone)
				{
					if (IntroTrial::CurrentTrial->CalcZoneTime())
					{
						// Within zone for long enough!
						AppStateVars.WritePosition();
						// Notify end of trial.
						IntroTrial::CurrentTrial->Notify();
					}
				}
				else
				{
					IntroTrial::CurrentTrial->ZoneLeft();
				}
			}
			else
			{
				if (withinzone)
				{
					IntroTrial::CurrentTrial->ZoneEntered();
				}
			}
		}
#else
		}

		IntroTrial::CurrentTrial->Notify();
#endif
	}
}


void IntroTrial::ZoneEntered()
{
	_inZone = true;
	QueryPerformanceCounter(&_lastZoneTicks);
}

void IntroTrial::ZoneLeft()
{
	_inZone = false;
}

bool IntroTrial::CalcZoneTime()
{
	LARGE_INTEGER endtouchticks;
	LARGE_INTEGER perfreq;

	if (QueryPerformanceCounter(&endtouchticks))
	{
		if (QueryPerformanceFrequency(&perfreq))
		{
			endtouchticks.QuadPart-= _lastZoneTicks.QuadPart;
			double tickms = 1000 * ((double) endtouchticks.QuadPart) / ((double) perfreq.QuadPart);

			if (tickms > 0)
			{
				int zonems = (int) tickms;
				if (zonems >= ZoneTime)
				{
					return true;
				}
			}
		}
	}

	return false;
}


void IntroTrial::BuildSurface()
{
	_numVerts = 30; // (frustrum)

	_verts = new float[_numVerts * 3];

	float surfacedepth = ZoneOffset;//-0.8f * ZoneRad;
	float expandfac = 1.8f;

	int helpcounter = 0;

	//
	// Build the surface
	//

	// Front surface
	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	// Left surface
	_verts[helpcounter++] = -(HSize * expandfac);
	_verts[helpcounter++] = -(VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = -(HSize * expandfac);
	_verts[helpcounter++] = (VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = -(HSize * expandfac);
	_verts[helpcounter++] = (VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	// Right surface
	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = (HSize * expandfac);
	_verts[helpcounter++] = -(VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = (HSize * expandfac);
	_verts[helpcounter++] = -(VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = (HSize * expandfac);
	_verts[helpcounter++] = (VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	// Top surface
	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = -(HSize * expandfac);
	_verts[helpcounter++] = (VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = -(HSize * expandfac);
	_verts[helpcounter++] = (VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = (HSize * expandfac);
	_verts[helpcounter++] = (VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	// Bottom surface
	_verts[helpcounter++] = -(HSize * expandfac);
	_verts[helpcounter++] = -(VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = (HSize * expandfac);
	_verts[helpcounter++] = -(VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = -HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;

	_verts[helpcounter++] = (HSize * expandfac);
	_verts[helpcounter++] = -(VSize * expandfac);
	_verts[helpcounter++] = surfacedepth + DSize;

	_verts[helpcounter++] = HSize;
	_verts[helpcounter++] = -VSize;
	_verts[helpcounter++] = surfacedepth;
}

void IntroTrial::BuildBoundingBox()
{
	_numVertsBox = 30; // (cube)

	_vertsBox = new float[_numVertsBox * 3];

	float boxdepth = (ZoneOffset * 0.5);//0.01f;

	int helpcounter = 0;

	//
	// Build the bounding box
	//

	// Front surface
	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	// Left surface
	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	// Right surface
	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	// Top surface
	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	// Bottom surface
	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth;

	_vertsBox[helpcounter++] = -HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;

	_vertsBox[helpcounter++] = HSizeBox;
	_vertsBox[helpcounter++] = -VSizeBox;
	_vertsBox[helpcounter++] = boxdepth - DSizeBox;
}


/*******************************************************************************
 Render the instruction text
*******************************************************************************/
void IntroTrial::RenderText()
{
	char *c;
	char text[120];
	float xfac = 1.0f;
	float x = 0.0f;

	switch (_stage)
	{
		case -1:
			if (_participantNumber == 0)
				sprintf_s(text,119,"Participant Number: ");
			else
				sprintf_s(text,119,"Participant Number: %i",_participantNumber);

			xfac = 510.0f / 1280.0f;
			break;
		case 0:
			strcpy_s(text,119,"Please lift the cursor towards you to start");
			xfac = 360.0f / 1280.0f;
			break;
		case 1:
			strcpy_s(text,119,"Please move the cursor towards the centre and hold it there");
			xfac = 218.0f / 1280.0f;
			break;
	}

	x = xfac * AppStateVars.ScreenWidth;

	SetOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.0f, 0.0f, 0.0f);

	glLineWidth(2.5f);
	glTranslatef(x, (570.0f / 1280.0f) * AppStateVars.ScreenWidth, 0.5f);
	glScalef(0.2f * (AppStateVars.ScreenWidth / 1280.0f),0.2f * (AppStateVars.ScreenWidth / 1280.0f),1.0f);

	for (c=text; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}

	glPopMatrix();
	ResetPerspectiveProjection();
}


// Function to update the participant/primetype number, one character at a time
void IntroTrial::AddToNumber(int n, int &numref)
{
	int currentorder = 0;
	float res = (float) numref;
	int ires = numref;

	while (ires>0)
	{
		res = res / 10;
		ires = (int) res;
		currentorder++;
	}

	numref = (numref*10) + n;//(int) ( n * pow(10.0f, currentorder) );
}


// To handle delete key for participant/primetype number
void IntroTrial::RemoveFromNumber(int &numref)
{
	numref = (numref/10);//(int) ( n * pow(10.0f, currentorder) );
}


/*******************************************************************************
 Set projection to be othographic
*******************************************************************************/
void IntroTrial::SetOrthographicProjection()
{
	// Switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// Save previous matrix which contains the 
	// settings for the perspective projection
	glPushMatrix();
	// Reset matrix
	glLoadIdentity();
	// Set a 2D orthographic projection
	gluOrtho2D(0, AppStateVars.ScreenWidth, 0, AppStateVars.ScreenHeight);
	// Invert the y axis, down is positive
	glScalef(1, -1, 1);
	// Move the origin from the bottom left corner
	// to the upper left corner
	int h = (int) AppStateVars.ScreenHeight;
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}


/*******************************************************************************
 Return to perspective projection after othographic
 
 Since we saved the settings of the perspective projection before we
 set the orthographic projection, all we have to do is to change the
 matrix mode to GL_PROJECTION, pop the matrix, i.e. restore the settings,
 and finally change the matrix mode again to GL_MODELVIEW.
*******************************************************************************/
void IntroTrial::ResetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}