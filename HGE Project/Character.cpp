// Character.cpp file
#include "Character.h"

//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void Character::Initialise(char setCharacterName[50], char characterImageLocation[])
{
	// load texture for character
	characterTex	= hge->Texture_Load(characterImageLocation) ;
	characterSpr	= new hgeSprite(characterTex, 0.0f, 0.0f, 256.0f, 256.0f) ;

	// Checks size of passed name, copies name into character name, and makes sure it is null terminated
	//size_t arraySize = sizeof (setCharacterName) ;
	//snprintf(characterName, arraySize, "%s", setCharacterName);
	strncpy_s(characterName, 50, setCharacterName, 50) ;
	//characterName[arraySize - 1] = '\0' ;


	if(!characterTex)
	{
		DisplayErrorBox("Character initialisation error - Texture.") ;
	}
}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
Character::~Character()
{
	// Free loaded resources
	hge->Target_Free(characterTex) ;

	// Free created pointers
	characterSpr	= NULL;
	delete			characterSpr ;
}


//------------------------------------- RenderSprite -----------------------------------------
// Renders character sprite at given position
//--------------------------------------------------------------------------------------------
void Character::RenderSprite(float xPos, float yPos)
{
	characterSpr->RenderEx(xPos, yPos, 0.0f, 1.0f, 1.0f) ;
}

//--------------------------------------- GetName  -------------------------------------------
// Returns the chracters name (to be drawn on the convo box)
//--------------------------------------------------------------------------------------------
const char *Character::GetName()
{
	return characterName ;
}


//EOF Character.cpp