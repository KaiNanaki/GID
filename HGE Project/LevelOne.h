// Level Class header
#ifndef LEVEL_ONE
#define LEVEL_ONE
//--------------------------------------------------------------------------------------------
//
//  Name:	LevelOne.h
//
//  Desc:	Updates the first level's game objects/collisions and renders them along with the 
//			intro/outro dialogues
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include <hge.h>			// Include HGE engine header
#include <hgesprite.h>		// Include HGE texture header

#include "level.h"
#include "EnemyShip.h"
#include "Asteroid.h"

extern HGE *hge ;			// External pointer to the HGE interface.


class LevelOne: public Level
{
public:
	void Initialise() ;
	~LevelOne() ;

	// Update the level stages
	void Update(float frameTime, XInputController *controller) ;
	void CheckLevelCollisions(float frameTime) ;

	// Draw level
	void Render(float frameTime) ;
	void RenderIntro(float frameTime) ;
	void RenderOutro(float frameTime) ;
	void RenderGameOver(float frameTime) ;

	void Reset() ;
	int GetLevelTime() ;

	static const int MAX_SPACE_PIRATES		= 5 ;
	static const int MAX_LARGE_ASTEROIDS	= 150 ;
	static const int MAX_SMALL_ASTEROIDS	= (MAX_LARGE_ASTEROIDS*3)+2 ;

private:

	EnemyShip	pirateShip[MAX_SPACE_PIRATES] ;

	Asteroid	largeAsteroid[MAX_LARGE_ASTEROIDS] ;
	Asteroid	smallAsteroid[MAX_SMALL_ASTEROIDS] ;

	static const int	playerStartingXPos	= 3750 ;
	static const int	playerStartingYPos	= 6000 ;
	static const int	HP_CHANCE_HIGH		= 10 ;
	static const int	HP_CHANCE_LOW		= 20 ;
	static const int	MAX_LEVEL_TIME		= 120 ;

	double		mTime ;

	int			convoCount ;

	bool		midPointReached ;
	bool		midPointCompleted ;

	HEFFECT		mAsteroidCollide1Snd ;
	HEFFECT		mAsteroidCollide2Snd ;
	HEFFECT		mAsteroidCollide3Snd ;
	HEFFECT		mAsteroidCrack1Snd ;
	HEFFECT		mAsteroidCrack2Snd ;
	HEFFECT		mAsteroidCrack3Snd ;
	HEFFECT		mShipHitSnd ;
	HEFFECT		mLevelLostSnd ;
	HSTREAM		mLevel1Music ;
	HCHANNEL	mSoundChannel1 ;

	hgeFont		*textFont ;

}; 
#endif 
//EOF LevelOne.h
