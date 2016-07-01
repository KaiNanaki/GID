// EnemyShip Class Code
#include "EnemyShip.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void EnemyShip::Initialise()
{
	// Set EnemyShip member values
	mIsActive		= true ;

	mPosition.x		= 0 ;
	mPosition.y		= 0 ;

	mVelocity.x		= 0 ;
	mVelocity.y		= 0 ;

	mHeading.x		= 0 ;
	mHeading.y		= 0 ;

	// The drag co-efficient acting on the ship
	mDragFactor		= 0.30f ;

	// The max rotational speed of the ship
	mMaxRotationalSpeed	= 250.0f ;

	// The max acceleration speed of the ship
	mAcceleration.x	= 0 ;
	mAcceleration.y	= 0 ;

	// The scale factor of the EnemyShip, used to scale the ship's sprite
	mScaleFactor	= 0.3f ;

	// The max speed of the EnemyShip
	mVelocityLimit	= 450.0f ;


	mObjectTex		= hge->Texture_Load("Resources/Textures/PirateShip.png") ;
	mpObjectSpr		= new hgeSprite(mObjectTex, 0.0f, 0.0f, 256.0f, 256.0f) ;
	// Use 144 as Y pivot point so turning is centered slightly towards the rear of the ship
	mpObjectSpr->SetHotSpot(128.0f, 144.0f) ;

	mDestroyedTex	= hge->Texture_Load("Resources/Textures/DestroyedPirateShip.png") ;
	mpDestroyedSpr	= new hgeSprite(mDestroyedTex, 0.0f, 0.0f, 256.0f, 256.0f) ;
	mpDestroyedSpr->SetHotSpot(128.0f, 144.0f) ;


	if(!mObjectTex || !mDestroyedTex)
	{
		DisplayErrorBox("EnemyShip initialisation error - Texture.") ;
	}

}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
EnemyShip::~EnemyShip()
{

}


//---------------------------------------- Update --------------------------------------------
//--------------------------------------------------------------------------------------------
void EnemyShip::Update(float frameTime, int levelXSize, int levelYSize, hgeVector playerPos)
{
	if (mIsActive)
	{
		// Flee behaviour - Ship will flee on aggro
		mAcceleration	= mPirateAI.FleeOnAggro(playerPos, mPosition, mVelocity, mVelocityLimit, 500.0f) ;
	
		//// Arrive behaviour
		//mAcceleration	= mPirateAI.Arrive(player.GetPos(), mPosition, mVelocity, mVelocityLimit, mPirateAI.fast) ;
		//// Pursuit behaviour
		//mAcceleration = mPirateAI.Pursuit(player.GetPos(), player.GetHeading(), player.GetVel(), 
		//// Seek behaviour
		//mAcceleration = mPirateAI.Seek(playerPos, mPosition, mVelocity, mVelocityLimit) * frameTime ;
		//	player.GetMaxVel(), mPosition, mHeading, mVelocity, mVelocityLimit) ;
		//// Evade behaviour
		//mAcceleration = mPirateAI.Evade(player.GetPos(), player.GetVel(), player.GetMaxVel(), 
		//	mPosition, mVelocity, mVelocityLimit) ;
		//// Wander behaviour
		//mAcceleration = mPirateAI.Wander(mPosition, mVelocity) ;
		
		mVelocity		+= mAcceleration * frameTime ;
		mPosition		+= mVelocity * frameTime ;

		// Apply drag
		mVelocity = mVelocity - mVelocity * mDragFactor * frameTime ;

		// Rotate ship in the direction is is accelerating
		if (mVelocity.Length() > 0.000000001)
		{
			hgeVector temp = mVelocity ;
			mHeading = *temp.Normalize() ;
		}

		// Check if the ship has moved out of level bounds, and deactivates it if so
		CheckForRespawn(levelXSize, levelYSize) ;
	}
	
}


//---------------------------------------- Render --------------------------------------------
// Render the enemy ship if it is active. Render the destroyed sprite if the ship has been
// destroyed.
//--------------------------------------------------------------------------------------------
void EnemyShip::Render(float levelX, float levelY) 
{
	if (mIsActive)
	{
		mpObjectSpr->RenderEx(mPosition.x+levelX, mPosition.y+levelY, 
			atan2(mHeading.y, mHeading.x), mScaleFactor, mScaleFactor) ;
	}
	else if (mDestroyedCountdown > 0)
	{
		mpDestroyedSpr->SetColor(ARGB(mDestroyedCountdown, 255, 255, 255)) ;
		mpDestroyedSpr->RenderEx(mPosition.x+levelX, mPosition.y+levelY, 
			(float)(2 * M_PI *(mAngle/360)), mScaleFactor, mScaleFactor) ;
		mDestroyedCountdown -= 5 ;
	}
}


//----------------------------------- CheckForRespawn  ---------------------------------------
// Checks if the ship has moved out of level bounds and deactivates it if so
//--------------------------------------------------------------------------------------------
void EnemyShip::CheckForRespawn(int levelXSize, int levelYSize)
{
	float shipSize = (128.0f * mScaleFactor) ;
	if (mPosition.x < (0 - shipSize) || 
		mPosition.x > (levelXSize + shipSize) ||
		mPosition.y < (0 - shipSize) || 
		mPosition.y > (levelYSize + shipSize))
	{
		Deactivate() ;
	}

}


//EOF EnemyShip.cpp
