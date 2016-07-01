// Ship Class header
#ifndef SHIP
#define SHIP
//--------------------------------------------------------------------------------------------
//
//  Name:	PlayerShip.h
//
//  Desc:	Base ship class containing default members and class behaviour, inherits from 
//			GameObject base class
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


#include <hge.h>			// Include HGE engine header
#include <hgevector.h>		// Include HGE vector header
#include <hgesprite.h>		// Include HGE texture header
#include <hgeparticle.h>

#include "GameObject.h"


extern HGE* hge ;			// External pointer to the HGE interface.

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

class Ship : public GameObject
{
protected:

	
	HTEXTURE	thrustTex ;		// Resource handle for thrust texture
	hgeSprite	*thrustSpr ;		// Holds the image of the ships thruster

	HTEXTURE			mThrustParticleTex ;
	hgeSprite			*mpThrustParticleSpr ;
	hgeParticleSystem	*mpThrustParticle ;

	float		mThrustOffset ;
	float		mDistanceOffset ;
	hgeVector	thrustPos ;

	// Ship member variables
	bool	isThrusting ;
	bool	firedThisFrame ;
	

public:

	void Initialise(float xPos, float yPos, double rotation) ;
	~Ship() ;

	// Draw ship
	void Render(float xPos, float yPos) ;

	// Accelerate in the direction the ship is facing
	void Accelerate(float frameTime, float thrustPercentage) ;

	void Brake(float frameTime) ;

	bool GetIsThrusting() ;
	bool GetFiredThisFrame() ;

	void ActivateThrust() ;
	void DeactivateThrust() ;

	static const int MAX_BULLETS = 75 ;
	int	activeBullets ;


};

#endif
// EOF Ship.h


