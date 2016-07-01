// PlayerShip Class header
#ifndef PLAYER_SHIP
#define PLAYER_SHIP
//--------------------------------------------------------------------------------------------
//
//  Name:	PlayerShip.h
//
//  Desc:	Defines the ship to be controlled by the player 
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


#include <hge.h>			// Include HGE engine header

#include "XInputController.h"
#include "Ship.h"


extern HGE *hge ;			// External pointer to the HGE interface.

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

class PlayerShip : public Ship
{
private:
	//XInputController	mController ;

	static const int MAX_HEALTH = 6 ; // max 18

	int			mHealth ;
	bool		mInvincibilityOn ;

	HEFFECT		mShootSnd ;		// Shooting sound effect
	HEFFECT		mThrustSnd ;	// Sound effect for rocket thrust
	HEFFECT		mInvincibleSnd ;
	HCHANNEL	mSoundChannel1 ;	// Sound channel, used to alter sounds/use various effects

	HTEXTURE	mHealthBoxTex ;
	HTEXTURE	mHealthNodeTex ;
	hgeSprite	*mHealthBoxSpr ;
	hgeSprite	*mHealthNodeSpr ;


	// Second thurster on top of the default inherited from ship class as players ship has two thrusters
	hgeParticleSystem	*mpThrustParticle2 ;
	float		mThrustOffset2 ;
	hgeVector	thrust2Pos ;
	

public:

	void Initialise(float xPos, float yPos, double rotation) ;
	~PlayerShip() ;

	// Update ship movements
	void Update(float frameTime, int levelXSize, int levelYSize, bool invincibilityAllowed, XInputController *controller) ;

	// Draw ship
	void Render(float frameTime) ;//, float xPos, float yPos) ;

	void RenderHealthBox() ;

	void IncrementHealth() ;
	void DecrementHealth() ;
	bool HasZeroHealth() ;

	void Reset(float xPos, float yPos, float rotation) ;

};

#endif
// EOF PlayerShip.h


