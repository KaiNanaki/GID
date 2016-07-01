// XInputController Class header
#ifndef XINPUT_CONTROLLER_CLASS
#define XINPUT_CONTROLLER_CLASS
//--------------------------------------------------------------------------------------------
//
//  Name:	XInputController.h
//
//  Desc:	Contain miscellaneous stand alone functions of various uses that cna be used by
//			any class.
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


#include <windows.h>
#include <algorithm>
#include <xinput.h>			// XInput.lib/Xinput9_1_0.lib - Currently using XInput.lib

#include <hge.h>			// HGE engine header
#include <hgevector.h>		// HGE vector class header

using namespace std ;


extern HGE* hge ;			// External pointer to the HGE interface.


class XInputController
{
private:
	XINPUT_STATE		mState ;
	XINPUT_VIBRATION	mVibration ;
	int			mControllerId ;
	double		mDeadzone ;
	 
public:

	hgeVector	mLeftStick ;
	hgeVector	mRightStick ;
	float		mLeftTrigger ;
	float		mRightTrigger ;

	XInputController() ;
	~XInputController() ;
	 
	XINPUT_GAMEPAD *GetState() ;
	bool CheckConnection() ;
	bool Update() ;
	bool ButtonPressed(WORD button) ;
	bool ButtonDown(WORD button) ;
	void Vibrate(float leftDecimalValue = 0.0f, float rightDecimalValue = 0.0f) ;

	int GetPort() ;
	double GetDeadzone() ;
};
#endif
// EOF XInputController.h