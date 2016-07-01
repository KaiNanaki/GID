// HealthPickup Class Code
#include "HealthPickup.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void HealthPickup::Initialise(float xPos, float yPos)
{

	GameObject::Initialise(xPos, yPos, 0.0f) ;

	// load textures for the large Asteroid
	mObjectTex		= hge->Texture_Load("Resources/Textures/HealthPickup.png") ;
	mpObjectSpr		= new hgeSprite(GameObject::mObjectTex, 0.0f, 0.0f, 64.0f, 64.0f) ;
	mpObjectSpr->SetHotSpot(32.0f, 32.0f) ;
	mIsActive		= false ;
	mScaleFactor	= 0.7f ;

	mLifetime		= 8.0 ;

	if(!mObjectTex)
	{
		DisplayErrorBox("Health Pickup initialisation error - Texture.") ;
	}

	mAppearSnd		= hge->Effect_Load("Resources/Sounds/HPAppear.wav") ;
	mDisappearSnd	= hge->Effect_Load("Resources/Sounds/HPDisappear.wav") ;
	mCollectedSnd	= hge->Effect_Load("Resources/Sounds/HPCollected.wav") ;

	if(!mAppearSnd || !mDisappearSnd || !mCollectedSnd)
	{
		DisplayErrorBox("Health Pickup initialisation error - Audio.") ;
	}
}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
HealthPickup::~HealthPickup()
{
	hge->Effect_Free(mCollectedSnd) ;
	hge->Effect_Free(mDisappearSnd) ;
	hge->Effect_Free(mAppearSnd) ;
}


//------------------------------------ CheckForPickup ----------------------------------------
// Checks if an active health pickup has been collected by the player this frame. Also 
// decrements the health pickup's lifetime and deactivates it when it goes below 
// zero/times out. Plays accompanying sounds for collection/timeout.
//--------------------------------------------------------------------------------------------
bool HealthPickup::CheckForPickup(float frameTime, hgeVector playerPos, float aggroDist)
{
	
	if (mIsActive)
	{
		// Check for collision with player's ship
		if (CheckCollision(playerPos, aggroDist))
		{
			hge->Effect_PlayEx(mCollectedSnd, 35, 0, 1.0f, false) ;
			SetDestroyedCountdown() ;
			Deactivate() ;
			return true ; // return true to tell the level that the players health should be incremented
		}

		mLifetime -= frameTime ;
		if (mLifetime < 0)
		{
			SetDestroyedCountdown() ;
			hge->Effect_PlayEx(mDisappearSnd, 45, 0, 1.0f, false) ;
			Deactivate() ;
		}
	}// end if

	return false ;

}


//---------------------------------------- Render --------------------------------------------
// Renders the health pickup if it is active. if it has been picked up then it will fade the
// sprites alpha level until it reaches zero.
//--------------------------------------------------------------------------------------------
void HealthPickup::Render(hgeVector levelPosition)
{
	if (mIsActive)
	{
		// Draw Asteroid
		GameObject::Render(mPosition.x + levelPosition.x, mPosition.y + levelPosition.y) ;
	}// end if
	else if (mDestroyedCountdown > 0)
	{
		mpObjectSpr->SetColor(ARGB(mDestroyedCountdown, 255, 255, 255)) ;
		mpObjectSpr->RenderEx(mPosition.x, mPosition.y, (float)(2 * M_PI *(mAngle/360)), mScaleFactor, mScaleFactor) ;
		mDestroyedCountdown -= 1 ;
	}
}


//--------------------------------------- SpawnAt  -------------------------------------------
// When called, checks if the spawn position is within the level boundaries. If the position
// is valid and there is no active health pickup then the new pickup activated and spawned
//--------------------------------------------------------------------------------------------
void HealthPickup::SpawnAt(hgeVector spawnPosition, int levelXSize, int levelYSize)
{
	bool withinLevelBounds = false;
	if (spawnPosition.x > 0 + SCREEN_WIDTH/2 && mPosition.x < levelXSize - SCREEN_WIDTH/2
		&& spawnPosition.y > 0 + SCREEN_HEIGHT/2 && spawnPosition.y < levelYSize - SCREEN_HEIGHT/2)
	{
		withinLevelBounds = true ;
	}
	if (!mIsActive && withinLevelBounds)
	{
		Reset() ;
   		hge->Effect_PlayEx(mAppearSnd, 35, 0 , 1.0f, false) ;
		SetPos(spawnPosition.x, spawnPosition.y) ;
		Activate() ;
	}
}


//---------------------------------------- Reset  --------------------------------------------
// Reset the health pickips values to their defaults ready for it to be spawned again
//--------------------------------------------------------------------------------------------
void HealthPickup::Reset()
{
	mpObjectSpr->SetColor(ARGB(255, 255, 255, 255)) ;
	mPosition.x			= 0.0f ;
	mPosition.y			= 0.0f ;
	mIsActive			= false ;
	mDestroyedCountdown	= 0 ;
	mLifetime			= 8.0 ; // Seconds
}

//EOF HealthPickup.cpp