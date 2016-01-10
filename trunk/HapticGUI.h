/***********************************************************
 *
 * HapticGUI.h
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
#define HAPTICGUI__H


#ifndef NO_HAPTICS
// OpenHaptics Headers
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
#endif

#include "Globals.h"

// GL Headers
#include <GL/glut.h>

#include <math.h>
#include <time.h>
#include <ctime>
#include <vector>


///////////////////////////////////////////////////////////////
// ButtonWidget class
///////////////////////////////////////////////////////////////

class ButtonWidget
{
	public:
		ButtonWidget(int instance, char* text); // Must include callback!
		~ButtonWidget();
		
	public:
		float Width;
		float Height;
		float Depth;
		float Red;
		float Green;
		float Blue;

	public:
		int GetInstance();
#ifndef NO_HAPTICS
		int GetHapticInstance();
#endif
		bool GetTouching();
		void SetTouching(bool touching);
		void SetText(const char* text);
		void SetPosition(float x, float y, float z);
		void SetCallback(ButtonCallback callback);
		void OnPaint(); // glutSolidCube() stretched to size
#ifndef NO_HAPTICS
		void OnHapticRender();
#endif
		void Reset();

	private:
		void Update();
		void RenderText();
		float GetIntrusion(float z);		
		float GetIntrusion(float x, float y, float z);

	private:
		int _instance;
		int _hapticInstance;
		int _numChars;
		int _font;
		char _text[100];
		bool _touching;
		bool _activated;
		bool _waiting;
		clock_t _startTime;
		clock_t _waitTime;
		float _xPos;
		float _yPos;
		float _zPos;
		float _currentZ;
		float _springForce;
		ButtonCallback _callback;
};


///////////////////////////////////////////////////////////////
// SliderWidget class
///////////////////////////////////////////////////////////////

class SliderWidget
{
	public:
		SliderWidget(int instance, char* text); // Must include callback!
		~SliderWidget();
		
	public:
		float Width;
		float Height;
		float Depth;
		float MarkerWidth;
		float Red;
		float Green;
		float Blue;
		int Ticks;

	public:
		int GetInstance();
		int GetHapticInstance();
		double GetValue() const;
		void SetValue(double value, bool update);
		void SetText(const char* text);
		void SetPosition(float x, float y, float z);
		void SetCallback(SliderCallback callback);
		void OnPaint();

#ifndef NO_HAPTICS
		void OnHapticRender();
#endif

		void Reset();

	private:
		void Update();
		void RenderText();

	private:
		int _instance;
		int _hapticInstance;
		int _numChars;
		int _font;
		double _value;
		double _initValue;
		char _text[100];
		bool _activated;
		float _xPos;
		float _yPos;
		float _zPos;
		float _gravityForce;
		float _activeRadius;
		SliderCallback _callback;
};


///////////////////////////////////////////////////////////////
// HapticGUI Methods
///////////////////////////////////////////////////////////////

class HapticGUI
{
  public:
	  HapticGUI();
	  ~HapticGUI();

  public:
	  void Initialise(ButtonCallback buttoncallback, SliderCallback slidercallback); // Pass app callback as parameter

	  void AddButton(ButtonWidget* button);

	  void RemoveButton(int butinstance);

	  ButtonWidget* GetButton(int butnum);
	  
	  void AddSlider(SliderWidget* button);

	  void RemoveSlider(int slidinstance);

	  SliderWidget* GetSlider(int slidnum);

	  void OnPaint();

#ifndef NO_HAPTICS
	  void OnHapticRender();
#endif

	  void Reset();

  public:
#ifndef NO_HAPTICS
	  static void HLCALLBACK ButtonTouchCallback(
					HLenum event,
					HLuint object,
					HLenum thread,
					HLcache *cache,
					void *userdata);

	  static void HLCALLBACK ButtonUnTouchCallback(
					HLenum event,
					HLuint object,
					HLenum thread,
					HLcache *cache,
					void *userdata);
#endif

	  //static void AppCallback;

  private:
	  std::vector<ButtonWidget*> _buttons;
	  std::vector<SliderWidget*> _sliders;
	  ButtonCallback _buttonCallback;
	  SliderCallback _sliderCallback;
};


#endif

