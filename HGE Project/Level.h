// Level Class header
#ifndef LEVEL
#define LEVEL
//--------------------------------------------------------------------------------------------
//
//  Name:	Level.h
//
//  Desc:	Base class for levels, contains default functions and members  
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include <hge.h>			// Include HGE engine header
#include <hgesprite.h>		// Include HGE texture header

#include "XInputController.h"
#include "PlayerShip.h"
#include "Bullet.h"
#include "HealthPickup.h"

#include "ConvoBox.h"

extern HGE *hge ;			// External pointer to the HGE interface.


class Level
{
public:


	void Initialise(float playerXPos, float playerYPos) ;
	~Level() ;

	// Update the level stages
	void Update(float frameTime) ;
	void UpdateBaseGameObjects(float frameTime, XInputController *controller) ;

	// Draw level
	void Render(float frameTime) ;


	static const int LEVEL_X_SIZE		= 7496 ;
	static const int LEVEL_Y_SIZE		= 7496 ;
	// Scoring Values
	static const int SCORE_TYPE_SMALL	= 10 ;
	static const int SCORE_TYPE_MEDIUM	= 25 ;
	static const int SCORE_TYPE_HIGH	= 150 ;

	void IncrementScore(int scoreType) ;
	void UpdateScore(float frameTime) ;
	void SaveHighScore() ;

	// testing functions
	float GetXPos() ;
	float GetYPos() ;

	// Used to check if the level is complete/ready to progress to next level.
	// Returns 'bool outroCompleted' as upon level completion the outro will render before the 
	// next level should start.
	bool CheckLevelCompleted() ;
	bool CheckGameOver() ;
	bool IsOnScreen(float xPos, float yPos) ;
	int GetCurrentScore() ;
	int GetHighScore() ;

	

protected:

	//XInputController	mController ;

	PlayerShip		player ;
	ConvoBox		convoBox ;

	Bullet			playerBullet[Ship::MAX_BULLETS] ;

	HealthPickup	mHealthPickup ;
	
	// Resource handles
	HTEXTURE	levelTex ;

	hgeSprite	*levelSpr ;			// Holds the levels background image

	// The Levels's center position in the game world
	hgeVector	mLevelPosition ;

	int			mHighScore ;
	int			mCurrentScore ;

	bool		introCompleted ;
	bool		levelWon ;
	bool		levelLost ;
	bool		gameOverCompleted ;
	bool		outroCompleted ;
	bool		allowInvincibility ;
	bool		skipIntro ;

	static const int		SMALL_ASTEROID_SIZE	= 70 ;
	static const int		LARGE_ASTEROID_SIZE	= 115 ;
	static const int		BULLET_SIZE			= 1 ;
	static const int		PLAYER_SHIP_SIZE	= 75 ;
	static const int		PIRATE_SHIP_SIZE	= 55 ;

	// radius size of game objects
		
		
				
			
			


}; 
#endif 

//EOF Level.h