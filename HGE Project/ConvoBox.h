// ConvoBox Class header
#ifndef CONVO_BOX
#define CONVO_BOX
//--------------------------------------------------------------------------------------------
//
//  Name:	ConvoBox.h
//
//  Desc:	Defines the boxes that display dialogue and characters  name/photo from the 
//			character class
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include "hge.h"
#include "hgegui.h"
#include "hgefont.h"
#include "MenuItem.h"
#include "XInputController.h"
#include "Character.h"

extern HGE *hge ;			// External pointer to the HGE interface.
extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

class ConvoBox
{
public:
	void Initialise() ;
	~ConvoBox() ;

	void Update(float frameTime, XInputController *controller) ;
	void UpdateNextGUI(float frameTime) ;

	void UpdateController(XInputController *controller) ;

	// Returns 1 when 'next' is pressed and box has been undrawn. This allows the level's 
	// convoCount varibale to be incremented by it, and trigger the next convoBox
	// Use const int variables (EG. member.SPACE_PIRATE) to set the ConvoBox's current character
	// to be displayed
	int Render(float frameTime, bool drawLeft, int seCharacter, const char convoText[]) ;

	// Draws the convo boxes text to the screen
	void RenderText(const char convoText[]) ;

	//
	void RenderCharacter() ;

	// Use const int variables (EG. member.SPACE_PIRATE) to set the ConvoBox's current character
	// to be displayed
	void SetCharacter(int setCharacter) ;

	void Reset () ;
	

	// Accessor varibles
	static const int ALIEN_COMMANDER	= 0 ;
	static const int ALIEN_PILOT		= 1 ;
	static const int ALIENS_GUY			= 2 ;
	static const int CAPTAIN_SARCASTIC	= 3 ;
	static const int CAP_SARC_HELMET	= 4 ;
	static const int GENERAL_OBVIOUS	= 5 ;
	static const int MAJOR_PEDANTIC		= 6 ;
	static const int SPACE_PIRATE		= 7 ;


private:

	bool	boxExit ;		// Tells us when box is fully drawn and the text/character image can be drawn
	bool	textDisplayed ; // Tells us when we can display the "next" gui object
	bool	nextConvoBox ;	// Tells us when the player has pressed next and triggers the box to be undrawn
	bool	boxUndrawn ;	// Tells us when box has undrawn itself and the next convo box can be displayed
	float	scale ;			// Used to expand/strech the
	int		delay ; 
	int		delay2 ;
	bool	drawCapHelmet ;

	// Holds the current character to display
	Character	alienCommander ;
	Character	alienPilot ;
	Character	aliensGuy ;
	Character	captainSarcastic ;
	Character	capSarcHelmet ;
	Character	generalObvious ;
	Character	majorPedantic ;
	Character	spacePirate ;

	Character currentCharacter ;

	// Resource handles
	HEFFECT		clickEffect ;
	HTEXTURE	cursorTex;

	// Pointers to Menu's HGE objects
	hgeSprite	*cursorSpr ;
	hgeFont		*menuFont ;
	hgeFont		*textFont ;
	hgeGUI		*guiNext ;

	// Character photo handles/objects
	HTEXTURE	convoBoxTex ;
	hgeSprite	*convoBoxSpr ;




}; 
#endif 
//EOF ConvoBox.h
