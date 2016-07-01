// XInputController.cpp file
#include "XInputController.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
XInputController::XInputController()
{
	 mDeadzone = 0.2 ;
}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
XInputController::~XInputController()
{
}


//--------------------------------------- GetState -------------------------------------------
// Retrieve the controller's current state
//--------------------------------------------------------------------------------------------
XINPUT_GAMEPAD *XInputController::GetState()
{
	return &mState.Gamepad ;
}


//----------------------------------- CheckConnection  ---------------------------------------
// Check if a controller is connected and set its ID
//--------------------------------------------------------------------------------------------
bool XInputController::CheckConnection()
{
	int id = -1 ;

	// Find the first connected controller
	for (int i = 0 ; i < XUSER_MAX_COUNT && mControllerId == -1 ; i++)
	{
		ZeroMemory(&mState, sizeof(XINPUT_STATE)) ;
		 
		if (XInputGetState(i, &mState) == ERROR_SUCCESS)
		{
			// If controller is found, set its ID
			id = i ;
		}
	}
	 
	mControllerId = id ;
	if (mControllerId != -1)
	{
		return true ;
	}
	else
	{
		return false ;
	}
}


//---------------------------------------- Update --------------------------------------------
// Retrieve and update the controller stick/trigger variables
//--------------------------------------------------------------------------------------------
bool XInputController::Update()
{
	if (mControllerId == -1)
	{
		CheckConnection() ;
	}

	if (mControllerId != -1)
	{
		ZeroMemory(&mState, sizeof(XINPUT_STATE)) ;
		if (XInputGetState(mControllerId, &mState) != ERROR_SUCCESS)
		{
			// Returns false if the controller has been disconnected
			mControllerId = -1 ;
			return false ;
		}

		// Set left stick deadzone - radial deadzone (values of -10 to 10)
		mLeftStick.x = (mState.Gamepad.sThumbLX/3277) ;
		mLeftStick.y = (mState.Gamepad.sThumbLY/3277) ;
		if (mLeftStick.Length() < mDeadzone)
		{
			mLeftStick.x = 0 ;
			mLeftStick.y = 0 ;
		}
		else
		{
			mLeftStick = *mLeftStick.Normalize() * ((mLeftStick.Length() - mDeadzone) / (1 - mDeadzone)) ;
		}


		// Set right stick deadzone - radial deadzone (values of -10 to 10)
		mRightStick.x = (mState.Gamepad.sThumbRX/3277) ;
		mRightStick.y = (mState.Gamepad.sThumbRY/3277) ;
		if (mRightStick.Length() < mDeadzone)
		{
			mRightStick.x = 0 ;
			mRightStick.y = 0 ;
		}
		else
		{
			mRightStick = *mRightStick.Normalize() * ((mRightStick.Length() - mDeadzone) / (1 - mDeadzone)) ;
		}

		// Set trigger values (values of -1 to 1)
		mLeftTrigger	= (float)mState.Gamepad.bLeftTrigger / 255 ;
		mRightTrigger	= (float)mState.Gamepad.bRightTrigger / 255 ;

		return true ;
	}
	return false ;
}


//------------------------------------ ButtonPressed  ----------------------------------------
// Compare the controller state to the button that has been passed and return true if it was 
// pressed this frame
//--------------------------------------------------------------------------------------------
bool XInputController::ButtonPressed(WORD button)
{
	return (mState.Gamepad.wButtons & button) != 0 ;
}


//--------------------------------------- Vibrate  -------------------------------------------
// Takes a decimal value of 0.0-1.0 for each motor and sets the controllers vibration state
//--------------------------------------------------------------------------------------------
void XInputController::Vibrate(float leftDecimalValue, float rightDecimalValue)
{
	// Right motor is the high-frequency motor, left motor is the low-frequency motor
	ZeroMemory(&mVibration, sizeof(XINPUT_VIBRATION)) ;
	
	// Set each motors speed (Decimal values of 0.0 - 1.0)
	mVibration.wLeftMotorSpeed	= 65535 * leftDecimalValue ;
	mVibration.wRightMotorSpeed	= 65535 * rightDecimalValue ;

	// Set the current controllers vibration rate
	XInputSetState(mControllerId, &mVibration) ;
}


//--------------------------------------- GetPort  -------------------------------------------
// Retrieve the current controller port
//--------------------------------------------------------------------------------------------
int XInputController::GetPort()
{
	return mControllerId + 1 ;
}

//------------------------------------- GetDeadzone  -----------------------------------------
// Return the controllers deadzone value
//--------------------------------------------------------------------------------------------
double XInputController::GetDeadzone()
{
	return mDeadzone ;
}

//EOF XInputController.cpp