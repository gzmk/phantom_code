/***********************************************************
 *
 * HapticGUI.cpp
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
 * This file defines the buttonwidget class and the
 * hapticGUI class, which allow haptic buttons to be used
 * in a user interface to control the program without
 * having to let go of the phantom.  ButtonWidgets consist
 * of a simple cuboid object, that when hit by the Phantom
 * invoke a callback so that the application can respond.
 * Initially, a single instance of HapticGUI should be
 * declared.  Then to build a GUI, declare a new
 * ButtonWidget class, setup its parameters, and then add
 * it to the GUI through the interface:
 *
 *		HapticGUI::AddButton().
 *
 * Also, a callback function should be provided to the
 * HapticGUI class so that the application can respond to
 * button presses.
 *
 ***********************************************************/

#ifndef HAPTICGUI__H
	#include "HapticGUI.h"
#endif

#ifndef NO_HAPTICS
	#ifndef HAPTICS__H
		#include "Haptics.h"
	#endif
#endif



///////////////////////////////////////////////////////////////
// ButtonWidget Methods
///////////////////////////////////////////////////////////////

ButtonWidget::ButtonWidget(int instance, char* text)
{
	_instance = instance;
#ifndef NO_HAPTICS
	_hapticInstance = hlGenShapes(1);
#endif
	_touching = false;
	_activated = false;
	_waiting = false;
	_waitTime = CLOCKS_PER_SEC;

	_numChars = strlen(text);

	if (_numChars < 100)
		strcpy(_text,text);
	else
	{
		strcpy(_text, "---");
		_numChars = 3;
	}

	_font = (int) GLUT_BITMAP_8_BY_13;
	_springForce = 0.01f;  
}


ButtonWidget::~ButtonWidget()
{
#ifndef NO_HAPTICS
	hlDeleteShapes(_hapticInstance,1);
#endif
}


int ButtonWidget::GetInstance()
{
	return _instance;
}


#ifndef NO_HAPTICS
int ButtonWidget::GetHapticInstance()
{
	return _hapticInstance;
}
#endif


void ButtonWidget::SetText(const char* text)
{
	_numChars = strlen(text);

	if (_numChars < 100)
		strcpy(_text,text);
	else
	{
		strcpy(_text, "---");
		_numChars = 3;
	}
}


void ButtonWidget::SetPosition(float x, float y, float z)
{
	_xPos = x;
	_yPos = y;
	_zPos = z;

	_currentZ = z;
}


void ButtonWidget::SetCallback(ButtonCallback callback)
{
	_callback = callback;
}


bool ButtonWidget::GetTouching()
{
	return _touching;
}


void ButtonWidget::SetTouching(bool touching)
{
	_touching = touching;
}


void ButtonWidget::OnPaint()
{
	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);

	if (_waiting)
		glColor3f(Red*0.4f, Green*0.4f, Blue*0.4f);
	else
	{
		if (_touching)
			glColor3f(Red+0.3f, Green+0.3f, Blue+0.3f);
		else
			glColor3f(Red, Green, Blue);
	}

	glTranslatef(_xPos, _yPos, _currentZ);
	glScalef(-Width, Height, Depth);

	glutSolidCube(1.0f);

	glPopMatrix();

	/*// Render the text
	char *c;
	float xpos, ypos;
	xpos = _xPos + (0.001f * (_numChars+1));
	ypos = _yPos - 0.003f;
  
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos3f(xpos, ypos, _currentZ-Depth);

	for (c=_text; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}*/

	RenderText();
}


#ifndef NO_HAPTICS
void ButtonWidget::OnHapticRender()
{
	Update();

	if (!_waiting)
	{
		glPushMatrix();

		glTranslatef(_xPos, _yPos, _currentZ);
		glScalef(-Width, Height, Depth);

		hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, _hapticInstance);
		glutSolidCube(1.0f);
		hlEndShape();

		glPopMatrix();
	}
}
#endif


void ButtonWidget::Reset()
{
	_activated = false;
	_startTime = clock();
	_waiting = true;

	// Set to original position
	_currentZ = _zPos;
}


// Private methods
void ButtonWidget::Update()
{
	// Respond to proxy hits
	if (_waiting)
	{
		clock_t currenttime = clock();
		if ((currenttime - _startTime)>_waitTime)
			_waiting = false;
	}

	if (_touching && (!_waiting))
	{
#ifndef NO_HAPTICS
		HLdouble proxforce[3];
		hlGetDoublev(HL_DEVICE_FORCE, proxforce);

		if ((proxforce[2] < -1) && (!_activated))
		{
			_currentZ+= (-proxforce[2] * (Depth * 0.02f));
			_currentZ+= (_zPos - _currentZ) * _springForce;

			if ((_currentZ - _zPos)>(Depth + 0.01f))
			{
				if (_callback)
				{
					_activated = true;
					_callback(_instance, true);
				}
			}
		}
#endif
	}
	else
	{
		_currentZ+= (_zPos - _currentZ) * _springForce;
	}
}


void ButtonWidget::RenderText()
{
	// Self explanatory
	char *c;
	float x = 0.0f;

	glColor3f(0.0f, 0.0f, 0.0f);

	glPushMatrix();

	glLineWidth(2.5f);
	
	glScalef(-1.0f,1.0f,1.0f);
	glTranslatef(-_xPos  - (0.0015f * (_numChars+1)), _yPos - 0.0045f, _currentZ - 0.01f);
	//glTranslatef(x,570.0f,0.5f);
	glScalef(0.000045f,0.000045f,1.0f);

	for (c=_text; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}

	glPopMatrix();
}


float ButtonWidget::GetIntrusion(float z)
{
	// Calculate how far a point penetrates the object
	// (only consider z direction : 0 means no intrusion)
	// (-ve values mean intrusion from back)
	if ((z > _currentZ) && (z < (_currentZ+Depth)))
	{
		float dz = z - _currentZ;

		if (((_currentZ+Width)-z) < dz)
			return (z - (_currentZ+Depth));
		else
			return dz;
	}

	return 0.0f;
}


float ButtonWidget::GetIntrusion(float x, float y, float z)
{
	// Calculate how far a point penetrates the object
	if ((x > _xPos) && (x < (_xPos+Width)))
	{
		if ((y > _yPos) && (y < (_yPos+Height)))
		{
			if ((z > _currentZ) && (z < (_currentZ+Depth)))
			{
				float dx = x - _xPos;
				if (((_xPos+Width)-x) < dx)
					dx = (_xPos+Width) - x;

				float dy = y - _yPos;
				if (((_yPos+Height)-y) < dy)
					dy = (_yPos+Height) - y;

				float dz = z - _currentZ;
				if (((_currentZ+Width)-z) < dz)
					dz = (_currentZ+Depth) - z;

				return sqrt((dx*dx)+(dy*dy)+(dz*dz));
			}
		}
	}

	return 0.0f;
}


///////////////////////////////////////////////////////////////
// SliderWidget Methods
///////////////////////////////////////////////////////////////

SliderWidget::SliderWidget(int instance, char* text)
{
	_instance = instance;

#ifndef NO_HAPTICS
	_hapticInstance = hlGenShapes(1);
#endif

	_activated = false;
	_value = 0.0;
	_initValue = 0.0;

	_gravityForce = 2.0f; 
	_activeRadius = 0.02f;

	_numChars = strlen(text);

	if (_numChars < 100)
		strcpy(_text,text);
	else
	{
		strcpy(_text, "---");
		_numChars = 3;
	}

	_font = (int) GLUT_BITMAP_8_BY_13;
}


SliderWidget::~SliderWidget()
{	
#ifndef NO_HAPTICS
	hlDeleteShapes(_hapticInstance,1);
#endif
}


int SliderWidget::GetInstance()
{
	return _instance;
}


int SliderWidget::GetHapticInstance()
{
	return _hapticInstance;
}


double SliderWidget::GetValue() const
{
	return _value;
}


void SliderWidget::SetValue(double value, bool update)
{
	if (value < 0.0)
		value = 0.0;
	else if (value > ((double) Ticks))
		value = ((double) Ticks);

	_value = value;
	_initValue = value;

	if (update)		
		_callback(_instance, _value);
}


void SliderWidget::SetText(const char* text)
{
	_numChars = strlen(text);

	if (_numChars < 100)
		strcpy(_text,text);
	else
	{
		strcpy(_text, "---");
		_numChars = 3;
	}
}


void SliderWidget::SetPosition(float x, float y, float z)
{
	_xPos = x;
	_yPos = y;
	_zPos = z;

}


void SliderWidget::SetCallback(SliderCallback callback)
{
	_callback = callback;
}


void SliderWidget::OnPaint()
{
	glPushMatrix();

	glEnable(GL_COLOR_MATERIAL);

	if (_activated)
		glColor3f(Red, Green, Blue);
	else
		glColor3f(Red * 0.4f, Green * 0.4f, Blue * 0.4f);

	glTranslatef(_xPos, _yPos, _zPos);

	glPushMatrix();
	glScalef(Width, Height * 0.5f, 1);

	glLineWidth(5);

	int counter1;
	float currentx = 0;

	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);

		if (Ticks > 0)
		{
			for (counter1=0;counter1<=Ticks;counter1++)
			{
				currentx = ((float) counter1) * (1.0f / ((float)Ticks));
				glVertex3f(currentx,-1,0);
				glVertex3f(currentx,1,0);
			}
		}
	glEnd();

	glPopMatrix();

	glTranslatef(((float) _value) * (Width / ((float)Ticks)), 0, 0);
	glScalef(-(Width * 0.5f) * (1.0f / ((float)Ticks)), Height * 0.8f, Depth);

	glutSolidCube(1.0f);

	glPopMatrix();

	RenderText();
}


#ifndef NO_HAPTICS
void SliderWidget::OnHapticRender()
{
	Update();
}
#endif


void SliderWidget::Reset()
{
	_activated = false;

	_value = _initValue;

	Update();
}


// Private methods
void SliderWidget::Update()
{
	double dist = 0;
	double t = 0;

#ifndef NO_HAPTICS
	// Find out distance to closest point on line	
	//HLdouble proxpos[3];
	hlGetDoublev(HL_PROXY_POSITION, AppStateVars.LastPosition);
	//hlGetDoublev(HL_PROXY_POSITION, proxpos);

	double start[] = {_xPos, _yPos, _zPos};
	double end[] = {_xPos + Width, _yPos, _zPos};
	//double p[] = {proxpos[0], proxpos[1], proxpos[2]};
	double p[] = {AppStateVars.LastPosition[0], AppStateVars.LastPosition[1], AppStateVars.LastPosition[2]};

	dist = DistanceToLineSegment(start, end, p, &t);
#endif
	
	// If within specified radius, change current value
	if (dist < _activeRadius)
	{
		// Find closest value
		double oldvalue = _value;
		_value = t * ((double) Ticks);

		if (_value < 0.0)
			_value = 0.0;
		else if (_value > ((double) Ticks))
			_value = ((double) Ticks);
		
#ifndef NO_HAPTICS
		GLdouble model[16];
		HLdouble viewtouch[16];
		HLdouble touchworkspace[16];
		HLdouble model2workspace[16];

		glGetDoublev(GL_MODELVIEW_MATRIX, model);
		hlGetDoublev(HL_VIEWTOUCH_MATRIX, viewtouch);
		hlGetDoublev(HL_TOUCHWORKSPACE_MATRIX, touchworkspace);

		hluModelToWorkspaceTransform(model, viewtouch, touchworkspace, model2workspace);
	
		// Set gravity to new point
		hduVector3Dd anchor = hduVector3Dd(((double) _xPos) + _value * (((double) Width) / ((double) Ticks)), ((double) _yPos), ((double) _zPos));

		AppStateVars.Anchor = hduVector3Dd((anchor[0] * model2workspace[0]) + (anchor[1] * model2workspace[4]) + (anchor[2] * model2workspace[8]) + model2workspace[12],
						(anchor[0] * model2workspace[1]) + (anchor[1] * model2workspace[5]) + (anchor[2] * model2workspace[9]) + model2workspace[13],
						(anchor[0] * model2workspace[2]) + (anchor[1] * model2workspace[6]) + (anchor[2] * model2workspace[10]) + model2workspace[14]);
#endif

		if (_activated == false)
		{
			// Start gravity if not already started
			
#ifndef NO_HAPTICS
			Haptics::StartSliderGravity();
#endif

			_activated = true;

			_callback(_instance, _value);
		}
		else if (oldvalue != _value)
			_callback(_instance, _value);
	}
	else
	{
		if (_activated)
		{
			// Stop gravity if not already stopped
			
#ifndef NO_HAPTICS
			Haptics::StopSliderGravity();
#endif

			_activated = false;
		}
	}
}


void SliderWidget::RenderText()
{
	// Self explanatory
	char *c;
	float x = 0.0f;

	glColor3f(0.0f, 0.0f, 0.0f);

	glPushMatrix();

	glLineWidth(2.5f);
	
	glScalef(-1.0f,1.0f,1.0f);
	//glTranslatef(-_xPos  - (0.0015f * (_numChars+1)) - (Width * 0.5f), _yPos + 0.0045f + (Height * 0.6f), _zPos - 0.01f);
	glTranslatef(-_xPos  - (0.00282f * (_numChars+1)) - (Width * 0.3f), _yPos + 0.0045f + (Height * 0.6f), _zPos - 0.01f);
	//glTranslatef(x,570.0f,0.5f);
	glScalef(0.000057f,0.000045f,1.0f);  // x = 0.000045f

	for (c=_text; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}

	glPopMatrix();
}


///////////////////////////////////////////////////////////////
// HapticGUI Methods
///////////////////////////////////////////////////////////////


HapticGUI::HapticGUI()
{
	_buttons.clear();
	_sliders.clear();
	_buttonCallback = 0;
	_sliderCallback = 0;
}


HapticGUI::~HapticGUI()
{
	int counter1;

	for (counter1=0;counter1<(int)_buttons.size();counter1++)
	{
		delete _buttons[counter1];
	}

	_buttons.clear();

	for (counter1=0;counter1<(int)_sliders.size();counter1++)
	{
		delete _sliders[counter1];
	}

	_sliders.clear();
}


void HapticGUI::Initialise(ButtonCallback buttoncallback, SliderCallback slidercallback)
{
	_buttonCallback = buttoncallback;
	_sliderCallback = slidercallback;
}


void HapticGUI::AddButton(ButtonWidget *button)
{
	_buttons.push_back(button);
	button->SetCallback(_buttonCallback);

#ifndef NO_HAPTICS
	hlAddEventCallback(HL_EVENT_TOUCH, button->GetHapticInstance(),
		HL_CLIENT_THREAD, &ButtonTouchCallback, button);

	hlAddEventCallback(HL_EVENT_UNTOUCH, button->GetHapticInstance(),
		HL_CLIENT_THREAD, &ButtonUnTouchCallback, button);
#endif
}


void HapticGUI::RemoveButton(int butinstance)
{
	int counter1;

	for (counter1=0;counter1<(int)_buttons.size();counter1++)
	{
		if (_buttons[counter1])
		{
			if (_buttons[counter1]->GetInstance() == butinstance)
			{
#ifndef NO_HAPTICS
				hlRemoveEventCallback (HL_EVENT_TOUCH, _buttons[counter1]->GetHapticInstance(),
					HL_CLIENT_THREAD, &ButtonTouchCallback);

				hlRemoveEventCallback (HL_EVENT_UNTOUCH, _buttons[counter1]->GetHapticInstance(),
					HL_CLIENT_THREAD, &ButtonUnTouchCallback);
#endif

				delete _buttons[counter1];
				_buttons[counter1] = 0;

				_buttons.erase(_buttons.begin() + counter1);

				counter1--;
			}
		}
	}
}


ButtonWidget* HapticGUI::GetButton(int butnum)
{
	if ((butnum>=0) && (butnum < (int) _buttons.size()))
		return _buttons[butnum];

	return NULL;
}


void HapticGUI::AddSlider(SliderWidget *slider)
{
	_sliders.push_back(slider);
	slider->SetCallback(_sliderCallback);
}


void HapticGUI::RemoveSlider(int slidinstance)
{
	int counter1;

	for (counter1=0;counter1<(int)_sliders.size();counter1++)
	{
		if (_sliders[counter1])
		{
			if (_sliders[counter1]->GetInstance() == slidinstance)
			{
				delete _sliders[counter1];
				_sliders[counter1] = 0;

				_sliders.erase(_sliders.begin() + counter1);

				counter1--;
			}
		}
	}
}


SliderWidget* HapticGUI::GetSlider(int slidnum)
{
	if ((slidnum>=0) && (slidnum < (int) _sliders.size()))
		return _sliders[slidnum];

	return NULL;
}


void HapticGUI::OnPaint()
{
	int counter1;

	for (counter1=0;counter1<(int)_buttons.size();counter1++)
	{
		if (_buttons[counter1])
			_buttons[counter1]->OnPaint();
	}

	for (counter1=0;counter1<(int)_sliders.size();counter1++)
	{
		if (_sliders[counter1])
			_sliders[counter1]->OnPaint();
	}
}


#ifndef NO_HAPTICS
void HapticGUI::OnHapticRender()
{
	int counter1;

	for (counter1=0;counter1<(int)_buttons.size();counter1++)
	{
		_buttons[counter1]->OnHapticRender();
	}

	for (counter1=0;counter1<(int)_sliders.size();counter1++)
	{
		_sliders[counter1]->OnHapticRender();
	}
}
#endif


void HapticGUI::Reset()
{
	int counter1;

	for (counter1=0;counter1<(int)_buttons.size();counter1++)
	{
		_buttons[counter1]->Reset();
	}

	for (counter1=0;counter1<(int)_sliders.size();counter1++)
	{
		_sliders[counter1]->Reset();
	}
}


#ifndef NO_HAPTICS
void HLCALLBACK HapticGUI::ButtonTouchCallback(
					HLenum event,
					HLuint object,
					HLenum thread,
					HLcache *cache,
					void *userdata)
{
	// Called when buttons pushed
	ButtonWidget* button = (ButtonWidget*) userdata;
	
	if (button)
	{
		button->SetTouching(true);
	}
}



void HLCALLBACK HapticGUI::ButtonUnTouchCallback(
					HLenum event,
					HLuint object,
					HLenum thread,
					HLcache *cache,
					void *userdata)
{
	// Called when buttons released
	ButtonWidget* button = (ButtonWidget*) userdata;
	
	if (button)
	{
		button->SetTouching(false);
	}
}
#endif