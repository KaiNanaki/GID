// LevelTwo.cpp file
#include "LevelTwo.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void LevelTwo::Initialise()
{
	Level::Initialise(5500.0f, 3500.0f) ;

	// load texture for the level
	levelTex	= hge->Texture_Load("Resources/Textures/Space-V1 medium.png") ;
	levelSpr	= new hgeSprite(levelTex, 0.0f, 0.0f, (float)LEVEL_X_SIZE, (float)LEVEL_Y_SIZE) ;

	if(!levelTex)
	{
		DisplayErrorBox("Level 2 initialisation error - Texture.") ;
	}

}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
LevelTwo::~LevelTwo()
{

}


//---------------------------------------- Update --------------------------------------------
// Updates the levels intro/outro during those sequences, otherwise updates the level and
// its game objects
//--------------------------------------------------------------------------------------------
void LevelTwo::Update(float frameTime, XInputController *controller)
{
	Level::Update(frameTime) ;

	// Update ConvoBoxes if during Intro/Outro sequence
	if (!introCompleted || levelWon)
	{
		convoBox.Update(frameTime, controller) ;
	}
	else
	{
		UpdateGameObjects(frameTime, controller) ;
	}
}


//---------------------------------- UpdateGameObjects  --------------------------------------
// Updates the levels base game objects (player ship, player bullets, health pickups)
//--------------------------------------------------------------------------------------------
void LevelTwo::UpdateGameObjects(float frameTime, XInputController *controller)
{

	Level::UpdateBaseGameObjects(frameTime, controller) ;

	if (player.GetXPos() != 5500.0f || player.GetYPos() != 3500.0f)
	{// test - end level if ship moves from its y position
		levelWon = true ;
	}


}


//---------------------------------------- Render --------------------------------------------
// Render levels base game objects and the intro/outro sequence if required
//--------------------------------------------------------------------------------------------
void LevelTwo::Render(float frameTime)
{
	Level::Render(frameTime) ;

	if (!introCompleted)
	{
		RenderIntro(frameTime) ;
	}
	if (levelWon)
	{
		RenderOutro(frameTime) ;
	}
}


//------------------------------------- RenderIntro  -----------------------------------------
//--------------------------------------------------------------------------------------------
void LevelTwo::RenderIntro(float frameTime)
{
	static int convoCount = 0 ;
	if (convoCount == 0)
	{
		convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET, 
			"Phew, no Game Over yet.") ;
	}
	else if (convoCount == 1)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.GENERAL_OBVIOUS,
			"Indeed, carry on Captain!") ;
	}
	else if (convoCount == 2)
	{
		convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET, 
			"That's not a Carry On film") ;
	}
	else if (convoCount == 3)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.MAJOR_PEDANTIC, 
			"Actually, since this game has not given any sort of time frame which can be BLAH BLAH BLAH related to the players current time or their understanding BLAH BLAH BLAH of that passage of time, BLAH BLAH BLAH BLAH BLAH BLAH you cannot make such wide sweeping statements. The only thing you can say for certain BLAH BLAH BLAH BLAH BLAH BLAH is that YOU have not heard of such a Carry On film. I quite enjoyed Carry On Captain, BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH even if the film was a tad scrupulous for my tastes.") ;
	}
	else if (convoCount == 4)
	{
		convoCount += convoBox.Render(frameTime, true, convoBox.CAPTAIN_SARCASTIC, 
			"*Removes Helmet* ...Fair enough.") ;
	}
	else if (convoCount == 5)
	{
		introCompleted = true ;
	}
}


//------------------------------------- RenderOutro  -----------------------------------------
//--------------------------------------------------------------------------------------------
void LevelTwo::RenderOutro(float frameTime)
{
	static int convoCount = 0 ;
	if (convoCount == 0)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.CAPTAIN_SARCASTIC, 
			"Game Over!") ;
	}
	else if (convoCount == 1)
	{
		outroCompleted = true ;
	}
}


//EOF LevelTwo.cpp