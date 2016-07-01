// State Class header
#ifndef STATE
#define STATE
//--------------------------------------------------------------------------------------------
//
//  Name:	State.h
//
//  Desc:	Defines the games different states and allows the changing of states. Mostly used
//			for changing menu states and going into/out of the game levels.
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


class State
{
public:

	// Accessor varibles
	static const int STATE_MENU			= 0 ;
	static const int STATE_PAUSE		= 1 ;
	static const int STATE_LEVEL		= 2 ;
	static const int STATE_INSTR		= 3 ;
	static const int STATE_CREDITS		= 4 ;
	static const int STATE_HIGH_SCORE	= 5 ;
	static const int STATE_GAME_OVER	= 6 ;
	static const int STATE_EXIT			= 7 ;

	// Enumerated type for the current game state
	enum gameStates { 
		mainMenu	= STATE_MENU,
		pause		= STATE_PAUSE,
		level		= STATE_LEVEL, 
		instr		= STATE_INSTR,
		credits		= STATE_CREDITS,
		mHighScore	= STATE_HIGH_SCORE,
		gameOver	= STATE_GAME_OVER,
		exit		= STATE_EXIT } ;


	State() ;
	void SetState(gameStates setState) ;
	gameStates GetState() ;
	bool CheckResetTrigger() ;
	void SetResetTrigger(bool resetLevel) ;

private:

	// Holds the current game state
	gameStates mCurrentState ;

	bool mTriggerLevelReset ;

}; 
#endif 
//EOF State.h
