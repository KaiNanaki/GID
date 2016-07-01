// HealthPickup Class header
#ifndef HEALTH_PICKUP
#define HEALTH_PICKUP
//--------------------------------------------------------------------------------------------
//
//  Name:	HealthPickup.h
//
//  Desc:	Defines health pickups to be collected by the player
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


#include <hge.h>			// Include HGE engine header
#include <hgevector.h>		// Include HGE vector header
#include <hgesprite.h>		// Include HGE texture header

#include "GameObject.h"


extern HGE* hge ;			// External pointer to the HGE interface.

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

class HealthPickup : public GameObject
{
protected:

	HEFFECT	mAppearSnd ;
	HEFFECT mDisappearSnd ;
	HEFFECT mCollectedSnd ;
	double	mLifetime ;

public:

	void Initialise(float xPos, float yPos) ;
	~HealthPickup() ;

	// Check if the Health Pack has been picked up, use return value to increment players HP
	bool CheckForPickup(float frameTime, hgeVector playerPos, float aggroDist) ;

	// Draw Health Pack
	void Render(hgeVector levelPosition) ;

	void SpawnAt(hgeVector spawnPosition, int levelXSize, int levelYSize) ;
	void Collected() ;
	void Reset() ;

};

#endif
// EOF HealthPickup.h