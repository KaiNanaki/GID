// Menu.cpp file
#include "Menu.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::Initialise()
{
	// Load menu sounds, background and cursor textures/sprite
	menuEffect		= hge->Effect_Load("Resources/Sounds/Button3.wav") ;
	gameOverEffect	= hge->Effect_Load("Resources/Sounds/GameOver.wav") ;
	menuMusic		= hge->Stream_Load("Resources/Sounds/Whoohoolala(MenuEdit).mp3") ;
	background.tex	= hge->Texture_Load("Resources/Textures/STARRY.png") ;
	cursorTex		= hge->Texture_Load("Resources/Textures/cursor2.png") ;
	cursorSpr		= new hgeSprite(cursorTex, 0, 0, 32, 32) ;

	// Load fonts
	menuFont = new hgeFont("Resources/font1.fnt") ;
	textFont = new hgeFont("Resources/Arial.fnt") ;


	// Check for missing files before progressing
	if(!background.tex || !cursorTex || !menuFont || !textFont)
	{
		DisplayErrorBox("Menu initialisation error - Texture.") ;
	}
	if(!menuEffect || !menuMusic || !gameOverEffect)
	{
		DisplayErrorBox("Menu initialisation error - Audio.") ;
	}
	// Start playing music as soon as the menu is loaded.
	mSoundChannel1 = hge->Stream_Play(menuMusic, true, 40) ;

	
	// Set up background blending modes for the background animation quad
	background.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE ;

	for(int i = 0; i < 4; i++)
	{
		// Set up z-coordinate of quad vertices
		background.v[i].z		= 0.5f ;
		// Set up quad vert color. '.col' in hex format, 0xAARRGGBB
		background.v[i].col	= 0xFFFFFFFF ;
	}

	background.v[0].x = 0 ;				background.v[0].y = 0 ; 
	background.v[1].x = SCREEN_WIDTH ;	background.v[1].y = 0 ; 
	background.v[2].x = SCREEN_WIDTH ;	background.v[2].y = SCREEN_HEIGHT ; 
	background.v[3].x = 0 ;				background.v[3].y = SCREEN_HEIGHT ; 
	

	// Create and initialize the Main Menu GUI
	guiMenu = new hgeGUI() ;
	guiMenu->AddCtrl(new MenuItem(1, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+120, 0.1f, "Play")) ;
	guiMenu->AddCtrl(new MenuItem(2, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+160, 0.2f, "High Scores")) ;
	guiMenu->AddCtrl(new MenuItem(3, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+200, 0.3f, "Instructions")) ;
	guiMenu->AddCtrl(new MenuItem(4, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+240, 0.4f, "Credits")) ;
	guiMenu->AddCtrl(new MenuItem(5, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+280, 0.5f, "Exit")) ;
	guiMenu->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED) ;
	guiMenu->SetCursor(cursorSpr) ;
	guiMenu->SetFocus(3) ; // Deafult menu option - Play (1), instr (3)
	guiMenu->Enter() ;


	// Create and initialize the Pause Menu GUI
	guiPause = new hgeGUI() ;
	guiPause->AddCtrl(new MenuItem(1, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+160, 0.1f, "Resume")) ;
	guiPause->AddCtrl(new MenuItem(2, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+200, 0.2f, "Instructions")) ;
	guiPause->AddCtrl(new MenuItem(3, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+240, 0.3f, "Restart Level")) ;
	guiPause->AddCtrl(new MenuItem(4, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+280, 0.4f, "Main Menu")) ;
	guiPause->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED) ;
	guiPause->SetCursor(cursorSpr) ;

	
	// Create and initialize the Instructions Menu GUI
	guiInstr = new hgeGUI() ;
	guiInstr->AddCtrl(new MenuItem(1, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+280, 0.1f, "Back")) ;
	guiInstr->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED) ;
	guiInstr->SetCursor(cursorSpr) ;

	
	// Create and initialize the Credits Menu GUI
	guiCredits = new hgeGUI() ;
	guiCredits->AddCtrl(new MenuItem(1, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+280, 0.1f, "Back")) ;
	guiCredits->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED) ;
	guiCredits->SetCursor(cursorSpr) ;

	// Create and initialise the High Score Menu GUI
	guiHighScore = new hgeGUI() ;
	guiHighScore->AddCtrl(new MenuItem(1, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+280, 0.1f, "Back")) ;
	guiHighScore->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED) ;
	guiHighScore->SetCursor(cursorSpr) ;

	// Create and initialise the Game Over Menu GUI
	guiGameOver = new hgeGUI() ;
	guiGameOver->AddCtrl(new MenuItem(1, menuFont, menuEffect, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+280, 0.1f, "Return to Main Menu")) ;
	guiGameOver->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED) ;
	guiGameOver->SetCursor(cursorSpr) ;


	// Default to 0
	flashValue		= 0 ;
	previousMenu	= 0 ;

}

//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
Menu::~Menu()
{
	// Free loaded resources
	hge->Target_Free(cursorTex) ;
	hge->Effect_Free(menuMusic) ;
	hge->Effect_Free(gameOverEffect) ;
	hge->Effect_Free(menuEffect) ;
	hge->Target_Free(background.tex) ;

	// Free font/mouse sprite pointers
	textFont	= NULL ;
	delete		textFont ;
	menuFont	= NULL ;
	delete		menuFont ;
	cursorSpr	= NULL;
	delete		cursorSpr ;

	// Free GUI pointers
	guiGameOver		= NULL ;
	delete			guiGameOver ;
	guiHighScore	= NULL ;
	delete			guiHighScore ;
	guiCredits		= NULL ;
	delete			guiCredits ;
	guiInstr		= NULL ;
	delete			guiInstr ;
	guiPause		= NULL ;
	delete			guiPause ;
	guiMenu			= NULL ;
	delete			guiMenu ;

}


//--------------------------------- UpdateMenuBackground -------------------------------------
// Updates the scrolling space background each frame
//--------------------------------------------------------------------------------------------
void Menu::UpdateMenuBackground(float frameTime)
{
	static float	bgTime = 0.0f ;
	float			bgX, bgY ;

	// Update menu background animation
	// Update X/Y coords by cos/sin of time
	bgTime	+=	frameTime ;
	bgX		=	2*cosf(bgTime/60) ;
	bgY		=	1*sinf(bgTime/60) ;

	// Set background tile image X/Y coords
	background.v[0].tx = bgX ;						background.v[0].ty = bgY ;
	background.v[1].tx = bgX+SCREEN_WIDTH/1000 ;	background.v[1].ty = bgY ;
	background.v[2].tx = bgX+SCREEN_WIDTH/1000 ;	background.v[2].ty = bgY+SCREEN_HEIGHT/500 ;
	background.v[3].tx = bgX ;						background.v[3].ty = bgY+SCREEN_HEIGHT/500 ;

}


//------------------------------------ UpdateMainMenu ----------------------------------------
// HUB for all the menus - Updates the Menu background, controller and MainMenu GUI and 
// transitions between menu states
//--------------------------------------------------------------------------------------------
void Menu::UpdateMainMenu(float frameTime, State *gameState, XInputController *controller)
{
	UpdateMenuBackground(frameTime) ;
	UpdateController(controller) ;

	if (!hge->Channel_IsPlaying(mSoundChannel1))
	{
		hge->Channel_Resume(mSoundChannel1) ; 
	}

	int			id		= -1 ;
	static int	lastid	= 0 ;

	id = guiMenu->Update(frameTime) ;

	if(id == -1)
	{
		switch(lastid)
		{
			case 1: // Play
				hge->Channel_Pause(mSoundChannel1) ;
				gameState->SetState(gameState->level) ;  //Play selected, start level 1
				guiMenu->Leave() ;
				guiPause->SetFocus(0) ;
				guiPause->Enter() ;
				guiGameOver->SetFocus(0) ;
				guiGameOver->Enter() ;

				// Set focus and trigger 'enter()' to "reset" the menu. This must be done to 
				// ensure the main menu works correctly when entering it from the GameOver or 
				// EndOfGame conditions
				guiMenu->SetFocus(0) ;
				guiMenu->Enter() ; 
				break ;
			case 2: //High Score
				gameState->SetState(gameState->mHighScore) ;
				guiMenu->SetFocus(0) ;
				guiMenu->Leave() ;
				guiHighScore->SetFocus(0) ;
				guiHighScore->Enter() ;
				break;
			case 3: // Instructions
				previousMenu = gameState->mainMenu ;
				gameState->SetState(gameState->instr) ;
				guiMenu->SetFocus(0) ;
				guiMenu->Leave() ;
				guiInstr->SetFocus(0) ;
				guiInstr->Enter() ;
				break;
			case 4: // Credits
				gameState->SetState(gameState->credits) ;
				guiMenu->SetFocus(0) ;
				guiMenu->Leave() ;
				guiCredits->SetFocus(0) ;
				guiCredits->Enter() ;
				break;
			case 5:// Exit game
				gameState->SetState(gameState->exit) ;
		}
	}
	else if(id) { lastid = id ; guiMenu->Leave() ; }

}


//----------------------------------- UpdatePauseMenu  ---------------------------------------
// Updates the Menu background, controller and PauseMenu GUI. Allows the transitions back to
// the previous level state or to one of the menu options selected
//--------------------------------------------------------------------------------------------
void Menu::UpdatePauseMenu(float frameTime, State *gameState, XInputController *controller)
{
	UpdateMenuBackground(frameTime) ;
	UpdateController(controller) ;

	if (!hge->Channel_IsPlaying(mSoundChannel1))
	{
		hge->Channel_Resume(mSoundChannel1) ;
	}

	int			pauseId		= -1 ;
	static int	lastPauseId	= 0 ;

	pauseId = guiPause->Update(frameTime) ;

	if(pauseId == -1)
	{
		switch(lastPauseId)
		{
			case 1: // Resume
				hge->Channel_Pause(mSoundChannel1) ;
				gameState->SetState(gameState->level) ;
				guiPause->SetFocus(0) ;
				guiPause->Leave() ;
				guiPause->SetFocus(0) ;
				guiPause->Enter() ;
				break ;
			case 2: // Instructions
				previousMenu = gameState->pause ;
				gameState->SetState(gameState->instr) ;
				guiPause->SetFocus(0) ;
				guiPause->Leave() ;
				guiInstr->SetFocus(0) ;
				guiInstr->Enter() ;
				break;
			case 3: // Restart level 
				hge->Channel_Pause(mSoundChannel1) ;
				gameState->SetResetTrigger(true) ;
				gameState->SetState(gameState->level) ;
				guiPause->Leave() ;
				guiPause->SetFocus(0) ;
				guiPause->Enter() ;
				break;
			case 4: // Main menu 
				gameState->SetResetTrigger(true) ;
				gameState->SetState(gameState->mainMenu) ;
				guiPause->SetFocus(0) ;
				guiPause->Leave() ;
				guiMenu->SetFocus(0) ;
				guiMenu->Enter() ;
				break;
		}
	}
	else if(pauseId) { lastPauseId = pauseId ; guiPause->Leave() ; }
}


//----------------------------------- UpdateInstrMenu  ---------------------------------------
// Updates the Menu background, controller and InstructionsMenu GUI, transitions back to the 
// previous menu state (MainMenu or game level)
//--------------------------------------------------------------------------------------------
void Menu::UpdateInstrMenu(float frameTime, State *gameState, XInputController *controller)
{
	UpdateMenuBackground(frameTime) ;
	UpdateController(controller) ;

	int			instrId		= -1 ;
	static int	lastInstrId	= 0 ;

	instrId = guiInstr->Update(frameTime) ;

	if(instrId == -1)
	{
		switch(lastInstrId)
		{
			case 1: // Back
				//If  back is selected, go back to the previous menu.
				if (previousMenu == gameState->STATE_MENU)
				{
					gameState->SetState(gameState->mainMenu) ;
					guiInstr->SetFocus(0) ;
					guiInstr->Leave() ;
					guiMenu->SetFocus(0) ;
					guiMenu->Enter() ;
				}
				else if (previousMenu == gameState->STATE_PAUSE)
				{
					gameState->SetState(gameState->pause) ;
					guiInstr->SetFocus(0) ;
					guiInstr->Leave() ;
					guiPause->SetFocus(0) ;
					guiPause->Enter() ;
				}
				break ;
		}
	}
	else if(instrId) { lastInstrId = instrId ; guiInstr->Leave() ; }


}


//---------------------------------- UpdateCreditsMenu  --------------------------------------
// Updates the Menu background, controller and CreditsMenu GUI, transitions back to the main
// menu state
//--------------------------------------------------------------------------------------------
void Menu::UpdateCreditsMenu(float frameTime, State *gameState, XInputController *controller)
{
	UpdateMenuBackground(frameTime) ;
	UpdateController(controller) ;

	int			creditsId		= -1 ;
	static int	lastCreditsId	= 0 ;

	creditsId = guiCredits->Update(frameTime) ;

	if(creditsId == -1)
	{
		switch(lastCreditsId)
		{
			case 1: // Back
				//If  back is selected, go back to the previous menu.
				gameState->SetState(gameState->mainMenu) ;
				guiCredits->SetFocus(0) ;
				guiCredits->Leave() ;
				guiMenu->SetFocus(0) ;
				guiMenu->Enter() ;
				guiCredits->Enter() ; //resets this menu
				break ;
		}
	}
	else if(creditsId) { lastCreditsId = creditsId ; guiCredits->Leave() ; }
}


//--------------------------------- UpdateHighScoreMenu  -------------------------------------
// Updates the Menu background, controller and HighScore Menu GUI, transitions back to the 
// previous menu
//--------------------------------------------------------------------------------------------
void Menu::UpdateHighScoreMenu(float frameTime, State *gameState, XInputController *controller)
{
	UpdateMenuBackground(frameTime) ;
	UpdateController(controller) ;

	int			highScoreId		= -1 ;
	static int	lastHighScoreId	= 0 ;

	highScoreId = guiHighScore->Update(frameTime) ;

	if(highScoreId == -1)
	{
		switch(lastHighScoreId)
		{
			case 1: // Back
				//If  back is selected, go back to the previous menu.
				gameState->SetState(gameState->mainMenu) ;
				guiHighScore->SetFocus(0) ;
				guiHighScore->Leave() ;
				guiMenu->SetFocus(0) ;
				guiMenu->Enter() ;
				guiHighScore->Enter() ; //reset menu
				break ;
		}
	}
	else if(highScoreId) { lastHighScoreId = highScoreId ; guiHighScore->Leave() ; }
}


//---------------------------------- UpdateGameOverMenu --------------------------------------
// Updates the Menu background, controller and HighScore Menu GUI, transitions back to the 
// main menu
//--------------------------------------------------------------------------------------------
void Menu::UpdateGameOverMenu(float frameTime, State *gameState, XInputController *controller)
{
	UpdateMenuBackground(frameTime) ;
	UpdateController(controller) ;

	static bool gameOverPlayed = false ;
	if (!gameOverPlayed)
	{
		hge->Effect_PlayEx(gameOverEffect, 75, 0, 1.0f, false) ;
		gameOverPlayed = true ;
	}

	int			highScoreId		= -1 ;
	static int	lastHighScoreId	= 0 ;

	highScoreId = guiGameOver->Update(frameTime) ;

	if(highScoreId == -1)
	{
		switch(lastHighScoreId)
		{
			case 1: // Back
				// If back is selected, go back to the previous menu.
				gameOverPlayed = false ; // reset 
				gameState->SetState(gameState->mainMenu) ;
				guiGameOver->SetFocus(0) ;
				guiGameOver->Leave() ;
				guiMenu->SetFocus(0) ;
				guiMenu->Enter() ;
				guiGameOver->Enter() ; // reset
				break ;
		}
	}
	else if(highScoreId) { lastHighScoreId = highScoreId ; guiGameOver->Leave() ; }
}


//------------------------------------ RenderMainMenu ----------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderMainMenu()
{
	// Render background
	hge->Gfx_RenderQuad(&background) ;

	menuFont->SetScale(1.0f) ;
	// Render Main Menu
	guiMenu->Render() ;

	// Render GID  Menu Title
	menuFont->SetColor(0xFFAA10FF) ;
	menuFont->SetScale(2.0f) ;
	menuFont->printf((SCREEN_WIDTH/2), 180.0f, HGETEXT_CENTER,"Generic Interplanetary Destruction" ) ;
	menuFont->SetColor(0xFFFFFFFF) ;
	menuFont->SetScale(0.6f) ;
	menuFont->printf((SCREEN_WIDTH/2), 260.0f, HGETEXT_CENTER, "A game by Kris Stevenson" ) ;
}


//----------------------------------- RenderPauseMenu  ---------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderPauseMenu()
{	
	// Render background
	hge->Gfx_RenderQuad(&background) ;

	menuFont->SetScale(1.0f) ;
	// Render Pause Menu
	guiPause->Render() ;

	// Render GID  Menu Title
	menuFont->SetColor(0xFFAA10FF) ;
	menuFont->SetScale(2.0f) ;
	menuFont->printf((SCREEN_WIDTH/2), 180.0f, HGETEXT_CENTER, "Generic Interplanetary Destruction" ) ;

	// Keep track of frames in a counter
	static int flashCounter = 0 ;
	// Holds the absolute value for alpha 0-255. Range of 0-510, counts up to 255 then back down
	flashValue = abs((((flashCounter/2) + 255) % 510) - 255) ;
	// Set Alpha and RGB values using ARGB() to convert them into hex;
	menuFont->SetColor(ARGB(flashValue, 170, 16, 255)) ; 
	// Display the flashing text "paused" on screen
	menuFont->printf((SCREEN_WIDTH/2), 340.0f, HGETEXT_CENTER, "PAUSED" ) ;
	// Increment flash counter
	flashCounter ++ ;

	menuFont->SetColor(0xFFFFFFFF) ;
	menuFont->SetScale(0.6f) ;
	menuFont->printf((SCREEN_WIDTH/2), 260.0f, HGETEXT_CENTER, "A game by Kris Stevenson" ) ;

	
}

//----------------------------------- RenderInstrMenu  ---------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderInstrMenu()
{
	// Render background
	hge->Gfx_RenderQuad(&background) ;

	menuFont->SetScale(1.0f) ;
	// Render Pause Menu
	guiInstr->Render() ;

	// Render GID  Menu Title
	menuFont->SetColor(0xFFAA10FF);
	menuFont->SetScale(2.0f) ;
	menuFont->printf((SCREEN_WIDTH/2), 180.0f, HGETEXT_CENTER,"Generic Interplanetary Destruction" ) ;
	menuFont->SetColor(0xFFFFFFFF) ;
	menuFont->SetScale(0.6f) ;
	menuFont->printf((SCREEN_WIDTH/2), 260.0f, HGETEXT_CENTER, "A game by Kris Stevenson" ) ;

	textFont->SetColor(0xFFFFFFFF) ;
	textFont->SetScale(0.8f) ;
	textFont->printfb(100.0f, 300.0f, (SCREEN_WIDTH-200.0f), (SCREEN_HEIGHT/2), HGETEXT_CENTER,
		"Thanks for trying out Generic Interplanetary Destruction!\nYou play as Captain "
		"Sarcastic, the greatest (and obviously most sarcastic) pilot in the Space Security "
		"Division. \nSpace pirates have recently been spotted near an archaic shipping route "
		"filled with asteroids. Your assignment is to clear the shipping route and bring any "
		"pirates to justice!"
		"\n\nPoints: Large Asteroids - 10, Small Asteroids - 25, Space Pirates - 150"
		"\n\nKeyboard/(Xbox Controller) Controls:") ;

	textFont->printfb(((SCREEN_WIDTH/2)-200.0f), 430.0f, (SCREEN_WIDTH-200.0f), (SCREEN_HEIGHT/2), HGETEXT_LEFT,
		"\n\nW (RT)"
		"\nS (LT)"
		"\nA/D (Left Stick)"
		"\nSpace (A)"
		"\nP (Start)"
		"\nReturn/Enter (A)"
		"\nEscape (Back)") ;
	textFont->printfb(((SCREEN_WIDTH/2)), 430.0f, (SCREEN_WIDTH-200.0f), (SCREEN_HEIGHT/2), HGETEXT_LEFT,
		"\n\n- Thrust"
		"\n- Thrust reversal(Brake)"
		"\n- Turn(Yaw) Left/Right"
		"\n- Fire"
		"\n- Pause"
		"\n- Next (During Conversations)"
		"\n- Quit game") ;

}

//---------------------------------- RenderCreditsMenu  --------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderCreditsMenu()
{
	// Render background
	hge->Gfx_RenderQuad(&background) ;

	menuFont->SetScale(1.0f) ;
	// Render Pause Menu
	guiCredits->Render() ;

	// Render GID  Menu Title
	menuFont->SetColor(0xFFAA10FF);
	menuFont->SetScale(2.0f) ;
	menuFont->printf((SCREEN_WIDTH/2), 180.0f, HGETEXT_CENTER,"Generic Interplanetary Destruction" ) ;
	menuFont->SetColor(0xFFFFFFFF) ;
	menuFont->SetScale(0.6f) ;
	menuFont->printf((SCREEN_WIDTH/2), 260.0f, HGETEXT_CENTER, "A game by Kris Stevenson" ) ;

	textFont->SetColor(0xFFFFFFFF) ;
	textFont->SetScale(0.7f) ;
	textFont->printfb(100.0f, 300.0f, (SCREEN_WIDTH-200.0f), (SCREEN_HEIGHT/2), HGETEXT_CENTER,
		"Credits"
		"\n\n Generic Interplanetary Destruction (GID) was programmed by Kris Stevenson in C++ using MS Visual Studio 2008"
		"\nAll game art was produced by either myself or my wife Sarah (Ta pet!)"
		"\n\nLevel music 'Anticipation' by James Calocerinos (MC Jimmy) http://mcjimmy.net/"
		"\nMenu music 'Whoohoolala' by Service Lab"
		"\nGID uses Haff's Game Engine (HGE, a hardware accelerated 2D game engine using DirectX)"
		"\nHaff's Game Engine - Copyright (c) 2003-2008 Relish Games. hge.relishgames.com"
		"\n\nIn loving memory of Harley"
		) ;

}


//--------------------------------- RenderHighScoreMenu  -------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderHighScoreMenu(int mHighScore)
{
	// Render background
	hge->Gfx_RenderQuad(&background) ;

	menuFont->SetScale(1.0f) ;
	// Render Pause Menu
	guiHighScore->Render() ;

	// Render GID  Menu Title
	menuFont->SetColor(0xFFAA10FF);
	menuFont->SetScale(2.0f) ;
	menuFont->printf((SCREEN_WIDTH/2), 180.0f, HGETEXT_CENTER,"Generic Interplanetary Destruction" ) ;
	menuFont->SetColor(0xFFFFFFFF) ;
	menuFont->SetScale(0.6f) ;
	menuFont->printf((SCREEN_WIDTH/2), 260.0f, HGETEXT_CENTER, "A game by Kris Stevenson" ) ;

	textFont->SetColor(0xFFFFFFFF) ;
	textFont->SetScale(1.0f) ;
	textFont->printfb(100.0f, 300.0f, (SCREEN_WIDTH-200.0f), (SCREEN_HEIGHT/2), HGETEXT_CENTER,
		"High Scores") ;
	textFont->printfb(100.0f, 400.0f, (SCREEN_WIDTH-200.0f), (SCREEN_HEIGHT/2), HGETEXT_CENTER,
		"Level 1: %d", mHighScore) ;

}


//---------------------------------- RenderGameOverMenu --------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderGameOverMenu()
{
	// Render background
	hge->Gfx_RenderQuad(&background) ;

	menuFont->SetScale(1.0f) ;
	// Render Pause Menu
	guiGameOver->Render() ;

	// Render GID  Menu Title
	menuFont->SetColor(0xFFAA10FF) ;
	menuFont->SetScale(2.0f) ;
	menuFont->printf((SCREEN_WIDTH/2), 180.0f, HGETEXT_CENTER, "Generic Interplanetary Destruction" ) ;

	// Keep track of frames in a counter
	static int flashCounter = 0 ;
	// Holds the absolute value for alpha 0-255. Range of 0-510, counts up to 255 then back down
	flashValue = abs((((flashCounter/2) + 255) % 510) - 255) ;
	// Set Alpha and RGB values using ARGB() to convert them into hex;
	menuFont->SetColor(ARGB(flashValue, 170, 16, 255)) ; 
	// Display the flashing text "Game Over" on screen
	menuFont->printf((SCREEN_WIDTH/2), 340.0f, HGETEXT_CENTER, "Game Over" ) ;
	// Increment flash counter
	flashCounter ++ ;


	menuFont->SetColor(0xFFFFFFFF) ;
	menuFont->SetScale(0.6f) ;
	menuFont->printf((SCREEN_WIDTH/2), 260.0f, HGETEXT_CENTER, "A game by Kris Stevenson" ) ;

	textFont->SetScale(0.8f) ;
	textFont->printfb(100.0f, 470.0f, (SCREEN_WIDTH-200.0f), (SCREEN_HEIGHT/2), HGETEXT_CENTER,
		"Invincibility option has been activated!\n When in game press 'I' to turn on Invincibility.") ;

}


//----------------------------------- UpdateController ---------------------------------------
// If connected, takes the controller's left stick position to update the mouse and the 'A'
// button is checked to allow for the accessing of menus
//--------------------------------------------------------------------------------------------
void Menu::UpdateController(XInputController *controller)
{
	// Retrieve the mouse position
	hgeVector mousePos(0.0f, 0.0f) ;
	hge->Input_GetMousePos(&mousePos.x, &mousePos.y) ;

	if (controller->CheckConnection())
	{
		// If controller is connected, add its left stick displacement to the mouse position
		// and update it
		hge->Input_SetMousePos(
			mousePos.x+(controller->mLeftStick.x/5), mousePos.y - (controller->mLeftStick.y/5)) ;
	}
	
	// If the controllers 'A' button is pressed, simulate the return key being pressed (for accessing menus)
	if (controller->ButtonPressed(XINPUT_GAMEPAD_A))
	{
		SynthesizeInput(VK_RETURN) ;
	}
}


//---------------------------------- RenderGameObjectXY --------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderGameObjectXY(float labelXpos, float labelYpos, char label[20], float x, float y)
{
	textFont->SetColor(0xFFFFFFFF) ;
	textFont->SetScale(0.7f) ;
	textFont->printf(labelXpos, labelYpos, HGETEXT_LEFT, "%s X: %.3f\n%s Y: %.3f", label, x, label, y) ;
}


//-------------------------------- RenderGameObjectValue  ------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderGameObjectValue(float labelXpos, float labelYpos, char label[20], int x)
{
	textFont->SetColor(0xFFFFFFFF) ;
	textFont->SetScale(0.7f) ;
	textFont->printf(labelXpos, labelYpos, HGETEXT_LEFT, "%s: %d", label, x) ;
}


//------------------------------------- RenderLabel  -----------------------------------------
//--------------------------------------------------------------------------------------------
void Menu::RenderLabel(float labelXpos, float labelYpos, char label[100])
{
	textFont->SetColor(0xFFFFFFFF) ;
	textFont->SetScale(0.7f) ;
	textFont->printf(labelXpos, labelYpos, HGETEXT_LEFT, "%s", label) ;
}


//EOF Menu.cpp