//Menu Class header
#ifndef MENU
#define MENU
//--------------------------------------------------------------------------------------------
//
//  Name:	Menu.h
//
//  Desc:	Defines each of the games menu pages and their behaviour
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


#include <ctime>
#include <math.h>

#include "hge.h"
#include "hgegui.h"
#include "hgefont.h"
#include "hgecolor.h"


#include "Utility.h"
#include "XInputController.h"
#include "MenuItem.h"
#include "State.h"

extern HGE *hge ;			// External pointer to the HGE interface.
extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;


class Menu
{
public:
	void Initialise() ;
	~Menu() ;
	//void Update(float frameTime, State *gameState) ;
	void UpdateMenuBackground(float frameTime) ;
	void UpdateMainMenu(float frameTime, State *gameState, XInputController *controller) ;
	void UpdatePauseMenu(float frameTime, State *gameState, XInputController *controller) ;
	void UpdateInstrMenu(float frameTime, State *gameState, XInputController *controller) ;
	void UpdateCreditsMenu(float frameTime, State *gameState, XInputController *controller) ;
	void UpdateHighScoreMenu(float frameTime, State *gameState, XInputController *controller) ;
	void UpdateGameOverMenu(float frameTime, State *gameState, XInputController *controller) ;
	void RenderMainMenu() ;
	void RenderPauseMenu() ;
	void RenderInstrMenu() ;
	void RenderCreditsMenu() ;
	void RenderHighScoreMenu(int mHighScore) ;
	void RenderGameOverMenu() ;

	void UpdateController(XInputController *controller) ;


	// Testing functions
	void RenderGameObjectXY(float labelXpos, float labelYpos, char label[30], float x, float y) ;
	void RenderGameObjectValue(float labelXpos, float labelYpos, char label[30], int x) ;
	void RenderLabel(float labelXpos, float labelYpos, char label[100]) ;

private:

	hgeQuad		background ;
	HEFFECT		menuEffect ;
	HEFFECT		gameOverEffect ;
	HSTREAM		menuMusic ;
	HCHANNEL	mSoundChannel1 ;
	HTEXTURE	cursorTex ;
	hgeSprite	*cursorSpr ;


	HEFFECT		mShootSnd ;		// Shooting sound effect
	HEFFECT		mThrustSnd ;	// Sound effect for rocket thrust

	// Pointers to Menu's HGE objects
	hgeFont		*menuFont ;
	hgeFont		*textFont ;
	hgeGUI		*guiMenu ;
	hgeGUI		*guiPause ;
	hgeGUI		*guiInstr ;
	hgeGUI		*guiCredits ;
	hgeGUI		*guiHighScore ;
	hgeGUI		*guiGameOver ;

	
	float		flashValue ;	// Holds alpha value, used for flashing text
	int			previousMenu ;	// Holds previous menu, used so the instructions menu knows to 
							// return to the game or to the main menu


}; 
#endif 
//EOF Menu.h
