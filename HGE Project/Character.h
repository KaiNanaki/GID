// Character Class header
#ifndef CHARACTER
#define CHARACTER
//--------------------------------------------------------------------------------------------
//
//  Name:	Character.h
//
//  Desc:	Holds a characters profile and name to be displayed by ConvoBox class
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include "hge.h"			// Include HGE engine header
#include "hgesprite.h"		// Include HGE texture header
#include "Utility.h"

extern HGE *hge ;			// External pointer to the HGE interface.
extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

class Character
{
public:
	void Initialise(char characterName[50], char characterImageLocation[]) ; //Max char name = 30 chars
	~Character() ;

	// Draws the character's name and profile pic at the given location (used in convoBoxes)
	void RenderSprite(float xPos, float yPos) ;

	// Returns the character's name so it can be drawn in the convoBox
	const char *GetName() ;

	void FadeUp(float frameTime) ;	// Use on the helmet Sprite to reveal Captain Sarcastic's face
	void FadeIn(float frameTime) ;	// Fades the photo in
	void FadeOut(float frameTime) ;	// fade photo out




private:

	// Char array to hold the characters name
	char characterName[50] ;

	// Character photo handles/objects
	HTEXTURE	characterTex ;
	hgeSprite	*characterSpr ;


}; 
#endif 
//EOF Character.h
