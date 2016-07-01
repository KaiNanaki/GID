// GameObject Class Code
#include "GameObject.h"

//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void GameObject::Initialise(float xPos, float yPos, double rotation)
{
	mIsActive = false ;
	mCollided = false ;

	// Set Game Object orientation
	mAngle = rotation ;

	// Velocity
	mVelocity.x = 0 ;
	mVelocity.y = 0 ;

	// Game Object position
	mPosition.x = xPos ;
	mPosition.y = yPos ;

	mHeading.x = 0 ;
	mHeading.y = 0 ;


	// The acceleration factor of the Game Object
	mAccelerationFactor = 250.0f;

	// Default drag factor	
	mDragFactor = 0.10f ;

	// The rotational speed of the Game Object (degrees per frame)
	mMaxRotationalSpeed = 0.0f ;

	// The rotation factor of the Game Object
	mRotationFactor = 250.0f ;

	// The scale factor of the Game Object, used to scale the object's sprite
	mScaleFactor = 0.3f ;

	mVelocityLimit = 250.0f ;

	mDestroyedCountdown	= 0 ;

}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
GameObject::~GameObject()
{
	// Free loaded resources
	hge->Target_Free(mDestroyedTex) ;
	hge->Target_Free(mObjectTex) ;

	// Free created pointers
	mpDestroyedSpr	= NULL ;
	delete			mpDestroyedSpr ;
	mpObjectSpr		= NULL ;
	delete			mpObjectSpr ;

}


//---------------------------------------- Update --------------------------------------------
// Updates the objects position/velocity/heading based on the base member variables (checks
// for level boundaries and velocity limits also)
//--------------------------------------------------------------------------------------------
void GameObject::Update(float frameTime, int levelXSize, int levelYSize, bool remove)
{
	// Update rotation based on rotational speed
	mAngle += mMaxRotationalSpeed * frameTime ;

	// Update position based on movement speed (velocity vector)
	mPosition += mVelocity * frameTime ;

	// Apply drag to the game object
	mVelocity = mVelocity - mVelocity * mDragFactor * frameTime ; 

	//// Update heading
	mHeading.x = cos(mAngle * (M_PI / 180)) ;
	mHeading.y = sin(mAngle * (M_PI / 180)) ;

	// Stop object moving outside the edge of the mission/level area 
	CheckMapEdges(levelXSize, levelYSize, remove) ;

	// Limit object velocity to its maximum speed
	LimitVelocity() ;
}


//---------------------------------------- Render --------------------------------------------
// Render the objects sprite at the given posiition and at its angle/scale
//--------------------------------------------------------------------------------------------
void GameObject::Render(float xPos, float yPos)
{
	mpObjectSpr->RenderEx(xPos, yPos, (float)(2 * M_PI *(mAngle/360)), mScaleFactor, mScaleFactor) ;
}


//---------------------------------------- Rotate --------------------------------------------
// Alter the objects angle by the given rotation
//--------------------------------------------------------------------------------------------
void GameObject::Rotate(int direction, float frameTime)
{
	mAngle += direction * mRotationFactor * frameTime ;	
}


//------------------------------------ CheckCollision ----------------------------------------
// Takes the position of another object and compares the squared distance between it two to
// its own.
// If the squared distance betwen the objects is less than the given aggro distance, they have
// collided so true is returned, otherwise the distance between them is greater and false
// is returned.
//--------------------------------------------------------------------------------------------
bool GameObject::CheckCollision(hgeVector position, float aggroDist)
{ 

	const float aggroDistSq = aggroDist * aggroDist ;
	if (DistanceSquared(mPosition, position) < aggroDistSq)
	{
		return true ;
	}
	else
	{
		return false ;
	}
}


//------------------------------------ CheckMapEdges  ----------------------------------------
// Makes sure the object does not go outside the given level size.
// If the remove flag is true, the object is removed when it raches the edge of the level
// (eg, bullets should be removed when reachign the edge of the map)
// If the remove flag is false, the object bounces back into the level at the opposite 
// velocity (eg, asteroid hits the left wall of the level and its X vel is reversed)
//--------------------------------------------------------------------------------------------
void GameObject::CheckMapEdges(int levelXSize, int levelYSize, bool remove)
{
	if (mPosition.x < 0)
	{
		mPosition.x = 0 ;
		if (remove)
		{
			Deactivate() ;
		}
		else
		{
			BounceBack(2, true, false) ;
		}
	}
	if (mPosition.x > levelXSize)
	{
		mPosition.x = levelXSize ;
		if (remove)
		{
			Deactivate() ;
		}
		else
		{
			BounceBack(2, true, false) ;
		}
	}
	if (mPosition.y < 0 )
	{
		mPosition.y = 0 ;
		if (remove)
		{
			Deactivate() ;
		}
		else
		{
			BounceBack(2, false, true) ;
		}
	}
	if (mPosition.y > levelYSize )
	{
		mPosition.y = levelYSize ;
		if (remove)
		{
			Deactivate() ;
		}
		else
		{
			BounceBack(2, false, true) ;
		}
	}
}


//------------------------------------ LimitVelocity  ----------------------------------------
// Limit the objects maximum velocity in the x and y plane
//--------------------------------------------------------------------------------------------
void GameObject::LimitVelocity()
{
	// Limit X Velocity
	if (mVelocity.x <= -mVelocityLimit)
	{
		mVelocity.x = -mVelocityLimit ;
	}
	else if (mVelocity.x >= mVelocityLimit)
	{
		mVelocity.x = mVelocityLimit ;
	}
	// Limit Y Velocity
	if (mVelocity.y <= -mVelocityLimit)
	{
		mVelocity.y = -mVelocityLimit ;
	}
	else if (mVelocity.y >= mVelocityLimit)
	{
		mVelocity.y = mVelocityLimit ;
	}
}


//-------------------------------------- BounceBack ------------------------------------------
// Can be used to change the velocity of the object in the x or y direction. Mainly used to
// reverse an objects x or y velocity when it bounces off the levels edges
//--------------------------------------------------------------------------------------------
void GameObject::BounceBack(float bounceForce, bool bounceX, bool bounceY)
{
	// bounceForce value of 1 stops the objects velocity, 
	// bounceForce value of 2 reverses the velocity from its previous value
	if (bounceX)
	{
		mVelocity.x = mVelocity.x - mVelocity.x * bounceForce ;
	}
	if (bounceY)
	{
		mVelocity.y = mVelocity.y - mVelocity.y * bounceForce ;
	}
}


//------------------------------- Accessor/Mutator Methods -----------------------------------
//--------------------------------------------------------------------------------------------
void GameObject::Activate()
{
	mIsActive = true ;
}

void GameObject::Deactivate()
{
	mIsActive = false ;
}

bool GameObject::GetIsActive()
{
	return mIsActive ;
}

float GameObject::GetXPos()
{
	return mPosition.x ;
}

float GameObject::GetYPos()
{
	return mPosition.y ;
}

float GameObject::GetXVel()
{
	return mVelocity.x ;
}

float GameObject::GetYVel()
{
	return mVelocity.y ;
}

double GameObject::GetRotation()
{
	return mAngle ;
}

float GameObject::GetScaleFactor()
{
	return mScaleFactor ;
}

float GameObject::GetMaxVel()
{
	return mVelocityLimit ;
}

void GameObject::SetCollided()
{
	mCollided = true ;
}

bool GameObject::HasCollided()
{
	return mCollided ;
}

void GameObject::SetDestroyedCountdown()
{
	mDestroyedCountdown = 255 ;
}

int GameObject::GetDestroyedCountdown()
{
	return mDestroyedCountdown ;
}

hgeVector GameObject::GetPos()
{
	return mPosition ;
}

hgeVector GameObject::GetVel()
{
	return mVelocity ;
}

hgeVector GameObject::GetHeading()
{
	return mHeading ;
}

void GameObject::SetPos(float xPos, float yPos)
{
	mPosition.x = xPos ;
	mPosition.y = yPos ;
}

void GameObject::SetVel(hgeVector velocity)
{
	mVelocity = velocity ;
}
//EOF GameObject.cpp
