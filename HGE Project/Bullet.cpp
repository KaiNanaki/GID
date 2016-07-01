// Bullet Class Code
#include "Bullet.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void Bullet::Initialise(float xPos, float yPos, float xVel, float yVel, double rotation)
{

	GameObject::Initialise(xPos, yPos, 0.0) ;

	// load textures for the Bullet
	GameObject::mObjectTex		= hge->Texture_Load("Resources/Textures/Bullet.png") ;
	GameObject::mpObjectSpr	= new hgeSprite(GameObject::mObjectTex, 0.0f, 0.0f, 16.0f, 16.0f) ;
	GameObject::mpObjectSpr->SetHotSpot(10.0f, 10.0f) ;

	if(!GameObject::mObjectTex)
	{
		// If one of the data files is not found
		DisplayErrorBox("Bullet initialisation error - Texture.") ;
	}


    // Bullet movement speed factor
	GameObject::mVelocity.x	*= 5.0f ;
    GameObject::mVelocity.y	*= 5.0f ;
 
    // No rotation or rotational speed
    mAngle				= 0.0f ;
    mMaxRotationalSpeed	= 0.0f ;


	GameObject::mDragFactor		= 0.0f ;
	GameObject::mScaleFactor	= 0.4f ;
	GameObject::mVelocityLimit	= 1000.0f ;
	GameObject::mIsActive		= false ;

	mBulletTime = MAX_BULLET_TIME ;

}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
Bullet::~Bullet()
{

}


//---------------------------------------- Update --------------------------------------------
// Updates bullet if it is active. Decrements mBulletTime each frame, when mBulletTime is 
// less than zero the bullet is deactivated.
//--------------------------------------------------------------------------------------------
void Bullet::Update(float frameTime, int levelXSize, int levelYSize)
{
	if (GetIsActive())
	{
		// Update Bullet, if it collides with the edge of the map/level, deactivate it
		GameObject::Update(frameTime, levelXSize, levelYSize, true) ;

		mBulletTime-- ;
		if (mBulletTime < 0)
		{
			mIsActive = false ;

		}
	}// end if
}


//---------------------------------------- Render --------------------------------------------
// Render bullet if it is active.
//--------------------------------------------------------------------------------------------
void Bullet::Render(float xPos, float yPos) 
{
	if (GetIsActive())
	{
		// Draw Bullet
		GameObject::Render(xPos, yPos) ;
	}// end if
}


//---------------------------------------- Reset  --------------------------------------------
// Resets the bullets to required values ready to be fired.
//--------------------------------------------------------------------------------------------
void Bullet::Reset(float xPos, float yPos, float xVel, float yVel, double rotation)
{
	mPosition.x		= xPos ;
	mPosition.y		= yPos ;
	mVelocity.x		= 0.0f ;
	mVelocity.y		= 0.0f ;
	mAngle			= rotation ;

	// Create normalized vector in the direction of travel
	float xN	= (float)(sin(2 * M_PI * (mAngle / 360))) ;
	float yN	= (float)(cos(2 * M_PI * (mAngle / 360))) ;

	//// Add to velocity vector (using minus for Y as HGE uses 0,0 as the top-left corner)
	mVelocity.x += (xN * mVelocityLimit) ; 
	mVelocity.y -= (yN * mVelocityLimit) ;

	// Move the bullet's initial position forward slightly so it renders at the tip of the ship's nose 
    mPosition.x += mVelocity.x * 0.03f;
    mPosition.y += mVelocity.y * 0.03f;


	mBulletTime = MAX_BULLET_TIME ;
}


// EOF Bullet.cpp