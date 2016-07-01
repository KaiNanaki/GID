// Ship Class Code
#include "Ship.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void Ship::Initialise(float xPos, float yPos, double rotation)
{

	// Initialise GameObject with passed/default member values
	GameObject::Initialise(xPos, yPos, rotation) ;  

	mIsActive = true ;

	isThrusting		= false ;
	firedThisFrame	= false ;
	activeBullets	= 0 ;
}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
Ship::~Ship()
{
	hge->Target_Free(mThrustParticleTex) ;

	mpThrustParticle	= NULL ;
	delete				mpThrustParticle ;
	mpThrustParticleSpr	= NULL ;
	delete				mpThrustParticleSpr ;

}


//---------------------------------------- Render --------------------------------------------
// Renders ship at the given point (if it is active)
//--------------------------------------------------------------------------------------------
void Ship::Render(float xPos, float yPos) 
{
	if (mIsActive)
	{

		GameObject::Render(xPos, yPos) ;
	}

}


//-------------------------------------- Accelerate ------------------------------------------
// Used to accelerate the ship in the direction it is facing.
//--------------------------------------------------------------------------------------------
void Ship::Accelerate(float frameTime, float thrustPercentage)
{

	// Create normalized vector in the direction of travel
	float xN = (float)(sin(mAngle * (M_PI / 180))) ;
	float yN = (float)(cos(mAngle * (M_PI / 180))) ;

	// Add to velocity vector (using minus for Y as HGE uses 0,0 as the top-left corner)
	if (thrustPercentage > 0)
	{
		mVelocity.x += (xN * (mAccelerationFactor * thrustPercentage)) * frameTime ; 
		mVelocity.y -= (yN * (mAccelerationFactor * thrustPercentage)) * frameTime ;
	}
	else
	{
		mVelocity.x += (xN * mAccelerationFactor) * frameTime ; 
		mVelocity.y -= (yN * mAccelerationFactor) * frameTime ;
	}
}


//---------------------------------------- Brake  --------------------------------------------
// Slows the ship down
//--------------------------------------------------------------------------------------------
void Ship::Brake(float frameTime)
{
	mVelocity = mVelocity - mVelocity * (mDragFactor*10) * frameTime ;
}


//------------------------------- Accessor/Mutator Methods -----------------------------------
//--------------------------------------------------------------------------------------------
bool Ship::GetIsThrusting()
{
	return Ship::isThrusting ;
}

bool Ship::GetFiredThisFrame()
{
	return firedThisFrame ;
}

void Ship::ActivateThrust()
{
	isThrusting = true ;
}

void Ship::DeactivateThrust()
{
	isThrusting = false ;
}
//EOF Ship.cpp
