// ConvoBox.cpp file
#include "ConvoBox.h"

//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void ConvoBox::Initialise()
{

	boxExit			= false ; // Tells us when text/character image is ready to be drawn
	textDisplayed	= false ; // Tells us when we can display the "next" gui object
	nextConvoBox	= false ; // Tells us when the player has pressed next and triggers the next convo box
	boxUndrawn		= false ; // Tells us when box has undrawn itself and the next convo box can be displayed

	scale	= 0.0f ;
	delay	= 50 ; 
	delay2	= 90 ;

	drawCapHelmet = false ;


	// Load sound effect and cursor for GUI object 'guiNext'
	clickEffect		= hge->Effect_Load("Resources/Sounds/Beep.wav") ;
	cursorTex		= hge->Texture_Load("Resources/Textures/cursor2.png") ;
	cursorSpr		= new hgeSprite(cursorTex, 0, 0, 32, 32) ;

	// Load fonts
	menuFont = new hgeFont("Resources/font1.fnt") ;
	textFont = new hgeFont("Resources/Arial.fnt") ;
	

	// Create and initialize the Next Button GUI
	guiNext = new hgeGUI() ;
	guiNext->AddCtrl(new MenuItem(1, menuFont, clickEffect, (SCREEN_WIDTH-50), (SCREEN_HEIGHT-50), 0.1f, "Next")) ;
	guiNext->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED) ;
	guiNext->SetCursor(cursorSpr) ;
	guiNext->SetFocus(1) ;
	guiNext->Enter() ;

	// Load ConvoBox sprite
	convoBoxTex		= hge->Texture_Load("Resources/Textures/ConvoBox.png") ;
	convoBoxSpr		= new hgeSprite(convoBoxTex, 0.0f, 0.0f, 1280.0f, 288.0f) ;

	// Initialise characters
	alienCommander.Initialise("Alien Commander",			"Resources/Textures/AlienCommander.png") ;
	alienPilot.Initialise("Alien Pilot",					"Resources/Textures/AlienPilot.png") ;
	aliensGuy.Initialise("Chief Science Officer Tsoukalos",	"Resources/Textures/AliensGuy.png") ;
	captainSarcastic.Initialise("Captain Sarcastic",		"Resources/Textures/CaptainSarcastic.png") ;
	capSarcHelmet.Initialise("Captain Sarcastic",			"Resources/Textures/Helmet.png") ;
	generalObvious.Initialise("General Obvious",			"Resources/Textures/GeneralObvious.png") ;
	majorPedantic.Initialise("Major Pedantic",				"Resources/Textures/MajorPedantic.png") ;
	spacePirate.Initialise("Space Pirate",					"Resources/Textures/SpacePirate.png") ;


	if(!convoBoxTex || !cursorTex || !clickEffect || !menuFont || !textFont)
	{
		DisplayErrorBox("ConvoBox initialisation error - Texture.") ;
	}
}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
ConvoBox::~ConvoBox()
{
	// Free loaded resources
	hge->Target_Free(convoBoxTex) ;
	hge->Target_Free(cursorTex) ;
	hge->Effect_Free(clickEffect) ;

	// Free created pointers
	convoBoxSpr	= NULL ;
	delete		convoBoxSpr ;

	guiNext		= NULL ;
	delete		guiNext ;
	textFont	= NULL ;
	delete		textFont ;
	menuFont	= NULL ;
	delete		menuFont ;
	cursorSpr	= NULL ;
	delete		cursorSpr ;

}


//---------------------------------------- Update --------------------------------------------
// Updates the current convo box. Uses 4 bools to determine box phase.
// After a short delay (delay2), updates the convo box (drawn from either the bottom left or
// bottom right of the screen) until its scale value is 1. When the box needs to be undrawn
// its value are updated until the scale value reaches 0.
//--------------------------------------------------------------------------------------------
void ConvoBox::Update(float frameTime, XInputController *controller)
{
	//bool	boxExit ;		// Tells us when box is fully drawn and the text/character image can be drawn
	//bool	textDisplayed ; // Tells us when we can display the "next" button/gui object
	//bool	nextConvoBox ;	// Tells us when the player has pressed next and triggers the box to be undrawn
	//bool	boxUndrawn ;	// Tells us when box has undrawn itself and the next convo box can be displayed
	//float	delay ;			// Used to delay text/photo appearing slightly after the box has been drawn
	//float	delay2 ;		// Used to delay the box appearing for a short time after the previous box

	UpdateController(controller) ;
	if (textDisplayed && !boxExit)
	{
		UpdateNextGUI(frameTime) ;
	}

	// Small delay before drawning convoBox for flow purposes
	delay2-- ;
	if (delay2 < 0)
	{
		if (scale < 1.0f && !boxExit)
		{
			// Makes convo box expand across the screen, text/image may then be displayed
			// Keep expanding until scale = 1.0 (image is expended left to right)
			scale += frameTime*2.5f ;
		}
		else if (scale > 0.0f && boxExit)
		{
			// Makes convoBox retract across the screen once 'next' is pressed
			// Keep retracting until scale = 0.0 (image is retracted right to left)
			scale -= frameTime*4.5f ;
		}
		else if (scale < 0.0f && boxExit)
		{
			// Once convoBox has retracted, set boxUndrawn to true so convoBox can be reset ready
			// for the next convo, returns true so the level knows when it can display the next box
			scale = 0.0 ;
			boxUndrawn = true ;
		}
		else
		{
			scale = 1.0f ;
			// Once ConvoBox is drawn, slightly delay the drawing of text/character photo
			if (delay < 0 && !boxExit)
			{
			}
			else
			{
				delay--;
			}
		}
	}

	if (nextConvoBox)
	{
		boxExit = true ;
	}

}


//------------------------------------ UpdateNextGUI  ----------------------------------------
// If next is selected, nextConvoBox is set to true and the ConvoBox will undraw itself.
//--------------------------------------------------------------------------------------------
void ConvoBox::UpdateNextGUI(float frameTime)
{
	int			id		= -1 ;
	static int	lastId	= 0 ;

	id = guiNext->Update(frameTime) ;


	if(id == -1)
	{
		switch(lastId)
		{
			case 1: // Next
				//If next is selected, trigger the convo box to undraw itself.
				guiNext->Leave() ;
				nextConvoBox = true ;
				guiNext->SetFocus(1) ;
				guiNext->Enter() ;
				break ;
		}
	}
	else if(id) { lastId = id ; guiNext->Leave() ; }

}


//----------------------------------- UpdateController ---------------------------------------
// If connected, takes the controller's left stick position to update the mouse and the 'A'
// button is checked to allow for the accessing of menus
//--------------------------------------------------------------------------------------------
void ConvoBox::UpdateController(XInputController *controller)
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


//---------------------------------------- Render --------------------------------------------
// Renders the convo box depending on its memeber variables
//--------------------------------------------------------------------------------------------
int ConvoBox::Render(float frameTime, bool drawLeft, int setCharacter, const char convoText[])
{
	// Set the character to be rendered for the current convo box.
	SetCharacter(setCharacter) ;

	// Render ConvoBox after a short delay, used for pacing.
	if (delay2 < 0)
	{		
		if (drawLeft)
		{
			// Draw the text box on the bottom left. Incrementing 'scale' expands the box from 0 pixels
			// in size to its full size. (bottom left to top right)
			convoBoxSpr->RenderEx(0.0f, SCREEN_HEIGHT, 0.0f, scale, -scale) ;
		}
		else
		{
			// Uses minus before scale so image is drawn bottom right to top left. 
			// (used when talking to other characters)
			convoBoxSpr->RenderEx(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -scale, -scale) ;
		}
	}

	// Draw character sprite, name and text
	// If the convo box is exiting (because 'next' has been pressed) then text/photo 
	// will not be drawn
	if (delay < 0 && !boxExit)
	{
		// Draw Character profile pic, their name, and the convo text
		RenderCharacter() ;
		RenderText(convoText) ;
	}
	if (textDisplayed && !boxExit)
	{
		guiNext->Render() ;
	}

	if (boxUndrawn)
	{
		boxExit			= false ; // Tells us when text/character image is ready to be drawn
		textDisplayed	= false ; // Tells us when we can display the "next" gui object
		nextConvoBox	= false ; // Tells us when the player has pressed next and triggers the next convo box
		boxUndrawn		= false ; // Tells us when box has undrawn itself and the next convo box can be displayed
		
		// Used to delay text/photo appearing for a short period after the box has been drawn
		delay	= 50 ;
		delay2	= 90 ;
		drawCapHelmet = false ;

		return 1 ;
	}
	else
	{
		return 0 ;
	}


}


//-------------------------------------- RenderText ------------------------------------------
// Renders the passed text on to the convo box
//--------------------------------------------------------------------------------------------
void ConvoBox::RenderText( const char convoText[])
{
	textFont->SetColor(0xFFFFFFFF) ;
	textFont->printfb(310.0f, SCREEN_HEIGHT-220, 890.0f, 100.0f, HGETEXT_LEFT, convoText) ;
	textDisplayed = true ;
}


//----------------------------------- RenderCharacter  ---------------------------------------
// Renders the current character's profile picture on to the convo box
//--------------------------------------------------------------------------------------------
void ConvoBox::RenderCharacter()
{

	// Draw the captain's sprite before drawing her helmet. 
	// This is done so the helmet can be removed/taken off later in the game
	if (drawCapHelmet)
	{
		captainSarcastic.RenderSprite(30.0f, SCREEN_HEIGHT-265) ;
	}
	currentCharacter.RenderSprite(30.0f, SCREEN_HEIGHT-265) ;
	textFont->printf(310.0f, SCREEN_HEIGHT-260, HGETEXT_LEFT, currentCharacter.GetName()) ;
}


//------------------------------------- SetCharacter -----------------------------------------
// Uses static const int passed to it to set the current character
//--------------------------------------------------------------------------------------------
void ConvoBox::SetCharacter(int setCharacter)
{
	switch (setCharacter)
	{
		case ALIEN_COMMANDER:
			currentCharacter = alienCommander ;
			break;
		case ALIEN_PILOT:
			currentCharacter = alienPilot ;
			break;
		case ALIENS_GUY:
			currentCharacter = aliensGuy ;
			break ;
		case CAPTAIN_SARCASTIC:
			currentCharacter = captainSarcastic ;
			break ;
		case CAP_SARC_HELMET:
			currentCharacter = capSarcHelmet ;
			drawCapHelmet = true ;
			break ;
		case GENERAL_OBVIOUS:
			currentCharacter = generalObvious ;
			break ;
		case MAJOR_PEDANTIC:
			currentCharacter = majorPedantic ;
			break ;
		case SPACE_PIRATE:
			currentCharacter = spacePirate ;
			break ;
	}
}


//---------------------------------------- Reset  --------------------------------------------
// Resets the convo boxes members to their default values.
//--------------------------------------------------------------------------------------------
void ConvoBox::Reset()
{
	boxExit			= false ; // Tells us when text/character image is ready to be drawn
	textDisplayed	= false ; // Tells us when we can display the "next" gui object
	nextConvoBox	= false ; // Tells us when the player has pressed next and triggers the next convo box
	boxUndrawn		= false ; // Tells us when box has undrawn itself and the next convo box can be displayed

	scale	= 0.0f ;
	delay	= 50 ; 
	delay2	= 90 ;

	drawCapHelmet = false ;
	
	guiNext->SetFocus(1) ;
	guiNext->Enter() ;

}

//EOF ConvoBox.cpp