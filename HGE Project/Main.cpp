//--------------------------------------------------------------------------------------------
//
//  Name:	Generic Interplanetary Destruction (GID)
//
//  Desc:	A simple space shooter game written in C++ using Microsoft Visual Studio 2008.
//			Uses the HGE engine and its basic helper classes. 
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include <hge.h>
#include <hgesprite.h>

#include <ctime>

#include "Utility.h"
#include "XInputController.h"
#include "State.h"
#include "Menu.h"
#include "LevelOne.h"
//#include "LevelTwo.h"


HGE *hge = NULL ;

// 16:9 ratio, 720p
const int SCREEN_WIDTH	= 1280 ;
const int SCREEN_HEIGHT	= 720 ;

XInputController	controller ;
State				gameState ;
Menu				gameMenu ;
LevelOne			level1 ;
//LevelTwo			level2 ;


//-------------------------------------- FrameFunc  ------------------------------------------
// This function is called by HGE once per frame (Main game loop).
//--------------------------------------------------------------------------------------------
bool FrameFunc()
{
	controller.Vibrate() ;
	controller.Update() ;
	float frameTime = hge->Timer_GetDelta() ;

	// If ESCAPE was pressed, tell the engine to shutdown
	if(hge->Input_GetKeyState(HGEK_ESCAPE) || controller.ButtonPressed(XINPUT_GAMEPAD_BACK))
	{
		return true ;
	}

	int currentState = gameState.GetState() ;

	switch (currentState)
	{
		case gameState.STATE_MENU:
			// Main Menu code
			gameMenu.UpdateMainMenu(frameTime, &gameState, &controller) ;
			break ;
		case gameState.STATE_PAUSE:
			// Pause Menu code
			gameMenu.UpdatePauseMenu(frameTime, &gameState, &controller) ;
			break ;
		case gameState.STATE_INSTR:
			// Instruction Menu code
			gameMenu.UpdateInstrMenu(frameTime, &gameState, &controller) ;
			break ;
		case gameState.STATE_CREDITS:
			// Credits Menu code
			gameMenu.UpdateCreditsMenu(frameTime, &gameState, &controller) ;
			break ;
		case gameState.STATE_HIGH_SCORE:
			// High Score Menu code
			gameMenu.UpdateHighScoreMenu(frameTime, &gameState, &controller) ;
			break ;
		case gameState.STATE_GAME_OVER:
			// GameOver Menu code
			gameMenu.UpdateGameOverMenu(frameTime, &gameState, &controller) ;
			break ;
		case gameState.STATE_LEVEL:
			if (hge->Input_KeyDown(HGEK_P) || controller.ButtonPressed(XINPUT_GAMEPAD_START))
			{
				gameState.SetState(gameState.pause) ;
			}
			if (!level1.CheckLevelCompleted())
			{
				if (gameState.CheckResetTrigger())
				{
					level1.Reset() ;
					gameState.SetResetTrigger(false) ;
				}
				if (level1.CheckGameOver())
				{
					gameState.SetState(gameState.gameOver) ;
					level1.Reset() ;
				}
				level1.Update(frameTime, &controller) ;
			}
			//else if (!level2.CheckLevelCompleted())
			//{
			//	level2.Update(frameTime) ;
			//}
			else
			{
				gameState.SetState(gameState.credits) ;
				level1.Reset() ;
			}
			break ;
		case gameState.STATE_EXIT:
			// Game exit
			return true ;
		default:
			return false ;
	}	

	// Continue execution
	return false ;
}


//-------------------------------------- RenderFunc ------------------------------------------
// This function is also called by HGE once per frame after FrameFunc (Main render loop).
//--------------------------------------------------------------------------------------------
bool RenderFunc()
{
	float frameTime = hge->Timer_GetDelta() ;

	hge->Gfx_BeginScene() ;
	hge->Gfx_Clear(0) ;


	int currentState = gameState.GetState() ;

	switch (currentState)
	{
		case gameState.mainMenu:
			// Main Menu code
			gameMenu.RenderMainMenu() ;
			break ;
		case gameState.pause:
			// Pause Menu code
			gameMenu.RenderPauseMenu() ;
			break ;
		case gameState.instr:
			// Instruction Menu code
			gameMenu.RenderInstrMenu() ;
			break ;
		case gameState.credits:
			// Credits Menu code
			gameMenu.RenderCreditsMenu() ;
			break ;
		case gameState.mHighScore:
			// High Score Menu code
			gameMenu.RenderHighScoreMenu(level1.GetHighScore()) ;
			break ;
		case gameState.gameOver:
			// GameOver Menu code
			gameMenu.RenderGameOverMenu() ;
			break ;
		case gameState.level:
			// Level code
			if (!level1.CheckLevelCompleted())
			{
				level1.Render(frameTime) ;
			}
			//else if (!level2.CheckLevelCompleted())
			//{
			//	level2.Render(frameTime) ;
			//}
			break ;
		case gameState.exit:
			// Exit the game
			return true ;
		default:
			return false ;
	}	

	hge->Gfx_EndScene() ;
	return false ;
}


//--------------------------------------- WinMain  -------------------------------------------
//--------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// Global pointer to HGE interface.
	hge = hgeCreate(HGE_VERSION) ;

	hge->System_SetState(HGE_LOGFILE, "GID LogFile.log");

	// Set window title
	hge->System_SetState(HGE_TITLE, "Kris Stevenson - Generic Interplanetary Destruction") ;
	hge->System_SetState(HGE_FPS, 60);

	// Run in windowed mode
	hge->System_SetState(HGE_WINDOWED, true) ;
	hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH) ;
	hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT) ;

	// Set screen bitdepth
	hge->System_SetState(HGE_SCREENBPP, 32) ;

	// Show splash screen
	hge->System_SetState(HGE_SHOWSPLASH, true) ;

	// Use BASS for sound
	hge->System_SetState(HGE_USESOUND, true) ;

	// Set frame function
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc) ;

	// Set render function
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc) ;


	// Tries to initiate HGE with the states set.
	// If something goes wrong, "false" is returned.
	if(hge->System_Initiate())
	{

		// Seed for random number generation
		srand((unsigned int)time(NULL)) ;

		// If game objects do not initialise properly (image files missing etc), they will 
		// display an error message box and trigger the engine to shutdown.
		gameMenu.Initialise() ;
		level1.Initialise() ;

		// Starts running FrameFunc().
		// Note that the execution "stops" here
		// until "true" is returned from FrameFunc().
		hge->System_Start() ;

	}
	else
	{	
		// If HGE initialization fails, show error message
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL) ;
	}

	// When this point has been reached ESC has been pressed or the user
	// has closed the window by other means.
	
	// Restore video mode and free
	// all allocated resources
	hge->System_Shutdown() ;

	// Release the HGE interface.
	// If there are no more references,
	// the HGE object will be deleted.
	hge->Release() ;

	return 0 ;
}