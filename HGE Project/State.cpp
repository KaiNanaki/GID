// State.cpp file
#include "State.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
State::State()
{
	mCurrentState		= mainMenu ;	
	mTriggerLevelReset	= false ;
}


//--------------------------------------- SetState -------------------------------------------
// Set the current state
//--------------------------------------------------------------------------------------------
void State::SetState(gameStates setState)
{
	mCurrentState = setState ;
}


//--------------------------------------- GetState -------------------------------------------
// Returns the current state
//--------------------------------------------------------------------------------------------

State::gameStates State::GetState()
{
	return mCurrentState ;
}


//---------------------------------- CheckResetTrigger  --------------------------------------
// Check to see if the mTriggerLevelReset has been triggered (Set by level class to determine
// when the state should be changed/level reloaded)
//--------------------------------------------------------------------------------------------
bool State::CheckResetTrigger()
{
	return mTriggerLevelReset ;
}


//----------------------------------- SetResetTrigger  ---------------------------------------
// Set the level reset trigger member, checking it allows the level to trigger its reset 
// function
//--------------------------------------------------------------------------------------------
void State::SetResetTrigger(bool resetLevel)
{
	mTriggerLevelReset = resetLevel ;
}

//EOF State.cpp