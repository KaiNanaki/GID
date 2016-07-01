// EnemyShip Class header
#ifndef ENEMY_SHIP
#define ENEMY_SHIP
//--------------------------------------------------------------------------------------------
//
//  Name:	EnemyShip.h
//
//  Desc:	Defines an autonomous agent for the game 
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


#include <hge.h>			// Include HGE engine header
#include <hgesprite.h>		// Include HGE texture header
#include <hgevector.h>
#include <math.h>

#include "Utility.h"

#include "PlayerShip.h"
#include "AI.h"


extern HGE* hge ;			// External pointer to the HGE interface.

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;


class EnemyShip : public Ship
{
private:
	AI mPirateAI ;
	//PlayerShip *mpPlayerShip ;

	hgeVector mAcceleration ;


public:

	void Initialise() ;
	~EnemyShip() ;

	// Update ship movements
	void Update(float frameTime, int levelXSize, int levelYSize, hgeVector playerPos) ;

	// Draw ship
	void Render(float levelX, float levelY) ;

	void CheckForRespawn(int levelXSize, int levelYSize) ;


};

#endif
// EOF EnemyShip.h


