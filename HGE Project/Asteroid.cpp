// Asteroid Class Code
#include "Asteroid.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void Asteroid::Initialise(float xPos, float yPos, bool large)
{
	GameObject::Initialise(xPos, yPos, 0.0f) ;

	if (large)
	{
		// load textures for the large Asteroid
		mObjectTex		= hge->Texture_Load("Resources/Textures/Asteroid.png") ;
		mpObjectSpr		= new hgeSprite(GameObject::mObjectTex, 0.0f, 0.0f, 256.0f, 256.0f) ;
		mpObjectSpr->SetHotSpot(128.0f, 128.0f) ;
		mDestroyedTex	= hge->Texture_Load("Resources/Textures/DestroyedAsteroid.png") ;
		mpDestroyedSpr	= new hgeSprite(mDestroyedTex, 0.0f, 0.0f, 256.0f, 256.0f) ;
		mpDestroyedSpr->SetHotSpot(128.0f, 128.0f) ;
		mIsActive		= true ;
		mScaleFactor	= 0.7f ;
	}
	else if (!large)
	{
		// load textures for the small Asteroid
		mObjectTex		= hge->Texture_Load("Resources/Textures/SmallAsteroid.png") ;
		mpObjectSpr		= new hgeSprite(GameObject::mObjectTex, 0.0f, 0.0f, 128.0f, 128.0f) ;
		mpObjectSpr->SetHotSpot(64.0f, 64.0f) ;
		mDestroyedTex	= hge->Texture_Load("Resources/Textures/DestroyedSmallAsteroid.png") ;
		mpDestroyedSpr	= new hgeSprite(mDestroyedTex, 0.0f, 0.0f, 128.0f, 128.0f) ;
		mpDestroyedSpr->SetHotSpot(64.0f, 64.0f) ;
		mIsActive		= false ;
		mScaleFactor	= 0.5f ;
	}
	
	if(!mObjectTex || !mDestroyedTex)
	{
		DisplayErrorBox("Asteroid initialisation error - Texture.") ;
	}


	mRotationFactor		= rand()%150 - 75.0f ;
	mVelocity.x			= rand()%600 - 300.0f ;
	mVelocity.y			= rand()%600 - 300.0f ;
	mVelocityLimit		= 350.0f ;
	mDragFactor			= 0.0f ;

}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
Asteroid::~Asteroid()
{

}


//---------------------------------------- Update --------------------------------------------
// Update Asteroid if it is active, set mCollided to false after updating so the asteroid is
// ready to collided with any objects again next frame.
//--------------------------------------------------------------------------------------------
void Asteroid::Update(float frameTime, int levelXSize, int levelYSize)
{
	
	if (GetIsActive())
	{
		GameObject::Update(frameTime, levelXSize, levelYSize, false) ;
		mCollided = false ;

	}// end if

}


//---------------------------------------- Render --------------------------------------------
// Draw the Asteroid if active, or the asteroid's destroyed sprite if it has been destroyed
//--------------------------------------------------------------------------------------------
void Asteroid::Render(float xPos, float yPos) 
{
	if (GetIsActive())
	{
		// Draw Asteroid
		GameObject::Render(xPos, yPos) ;
	}// end if
	else if (mDestroyedCountdown > 0)
	{
		mpDestroyedSpr->SetColor(ARGB(mDestroyedCountdown, 255, 255, 255)) ;
		mpDestroyedSpr->RenderEx(xPos, yPos, (float)(2 * M_PI *(mAngle/360)), mScaleFactor, mScaleFactor) ;
		// Reduce the alpha value of the sprit so it fades from the screen
		mDestroyedCountdown -= 5 ;
	}
}


//---------------------------------------- Reset  --------------------------------------------
// Reset asteroid ready to be drawn/updated after a level reset
//--------------------------------------------------------------------------------------------
void Asteroid::Reset()
{
	mpDestroyedSpr->SetColor(ARGB(255, 255, 255, 255)) ;
	mRotationFactor		= rand()%150 - 75.0f ;

	mVelocity.x			= rand()%600 - 300.0f ;
	mVelocity.y			= rand()%600 - 300.0f ;

	mIsActive			= true ;
	mDestroyedCountdown	= 0 ;
}


//EOF Asteroid.cpp