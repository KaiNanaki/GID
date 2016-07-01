// AI.cpp file
#include "AI.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
AI::AI()
{
	mWanderDisplacement = 10.0 ;
	mIsFleeing			= false ;
}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
AI::~AI()
{

}


//----------------------------------------- Seek ---------------------------------------------
// Returns vector that directs an agent towards a given target's position (ie, the player's 
// ship) at maximum velocity until that position is reached
//--------------------------------------------------------------------------------------------
hgeVector AI::Seek(hgeVector targetPos, hgeVector currentPos, hgeVector currentVel, float maxVel)
{
	hgeVector desiredVelocity = (targetPos - currentPos) ;
	desiredVelocity.Normalize() ;
	desiredVelocity = desiredVelocity * maxVel ;

	return (desiredVelocity - currentVel) ;
}


//---------------------------------------- Arrive --------------------------------------------
// Returns vector that directs an agent towards a given target's position (ie, the player's 
// ship) at maximum speed, slowing down when near, and coming to a stop at the target position
//--------------------------------------------------------------------------------------------
hgeVector AI::Arrive(hgeVector targetPos, hgeVector currentPos, hgeVector currentVel, 
					 float maxVel, Deceleration deceleration)
{
	hgeVector toTarget = targetPos - currentPos ;

	// Distance to target
	double distance = toTarget.Length() ;
	
	if (distance > 0.1)
	{
		// Calculate speed to target position and use decelerationValue to slow down according
		// to the distance to the target position
		const double decelerationValue = 0.5 ;
		double speed = distance / ((double)deceleration * decelerationValue) ;

		// Make sure speed is equal to max velocity at most
		speed = min(speed, maxVel) ;

		// Calculate the velocity to target
		hgeVector desiredVelocity = toTarget * speed / distance ;

		return (desiredVelocity - currentVel) ;
	}
	
	return hgeVector(0, 0) ;

}


//--------------------------------------- Pursuit  -------------------------------------------
// Returns vector that directs an agent towards a given target's predicted position depending
// on it's direction and speed
//--------------------------------------------------------------------------------------------
hgeVector AI::Pursuit(hgeVector targetPos, const hgeVector &targetHeading, 
					  hgeVector targetCurrentVel, float targetMaxVel, hgeVector currentPos, 
					  const hgeVector &currentHeading, hgeVector currentVel, float maxVel)
{	
	hgeVector toTarget = targetPos - currentPos ;

	double relativeHeading = currentHeading.Dot(&targetHeading) ;

	// If target is ahead and facing the agent, seek directly towards the target
	if ((toTarget.Dot(&currentHeading) > 0) 
		&& (relativeHeading < -0.95)) // acos 0.95 radians, within 18 degrees
	{
		return Seek(targetPos, currentPos, currentVel, maxVel) ;	
	}

	// If not ahead, estimate where target will be by predicting agent and target speeds along the
	// toTarget vector
	double lookAheadTime = toTarget.Length() / (maxVel + targetMaxVel) ;

	// Seek to estimated future position of target
	return Seek((targetPos + targetCurrentVel * lookAheadTime), currentPos, currentVel, maxVel) ;
}


//----------------------------------------- Flee ---------------------------------------------
// Returns vector that directs an enemyShip away from a given target
//--------------------------------------------------------------------------------------------
hgeVector AI::Flee(hgeVector targetPos, hgeVector currentPos, hgeVector currentVel, 
				   float maxVel)
{
	hgeVector desiredVelocity = (currentPos - targetPos) ;
	desiredVelocity.Normalize() ;
	desiredVelocity = desiredVelocity * maxVel ;

	return (desiredVelocity - currentVel) ;
}


//------------------------------------- FleeOnAggro  -----------------------------------------
// Returns vector that directs an enemyShip away from a given target that is within the
// passed aggro distance
//--------------------------------------------------------------------------------------------
hgeVector AI::FleeOnAggro(hgeVector targetPos, hgeVector currentPos, hgeVector currentVel, 
						  float maxVel, float aggroDist)
{
	// Check if enemyShip is outside of aggro distance, if so, do nothing.
	const float aggroDistSq = aggroDist * aggroDist ;
	if (DistanceSquared(currentPos, targetPos) > aggroDistSq)
	{
		mIsFleeing = false ;
		return hgeVector(0, 0) ;
	}

	hgeVector fromTarget = (currentPos - targetPos) ;
	fromTarget.Normalize() ;
	fromTarget = fromTarget * maxVel ;

	mIsFleeing = true ;
	return (fromTarget - currentVel) ;
}


//----------------------------------------- Evade  -------------------------------------------
// Returns vector that directs an agent away from a given target's predicted position 
// depending on it's direction and speed
//--------------------------------------------------------------------------------------------
hgeVector AI::Evade(hgeVector targetPos, hgeVector targetCurrentVel, float targetMaxVel, 
					hgeVector currentPos, hgeVector currentVel, float maxVel)
{
	hgeVector fromTarget = (currentPos - targetPos) ;

	// Estimate where target will be by predicting agent and target speeds along the
	// fromTarget vector
	double lookAheadTime = fromTarget.Length() / (maxVel + targetMaxVel) ;

	//return Flee((targetPos + targetCurrentVel * lookAheadTime), currentPos, currentVel, maxVel) ;
	return FleeOnAggro((targetPos + targetCurrentVel * lookAheadTime), currentPos, currentVel, maxVel, 400.0f) ;

}


//---------------------------------------- Wander --------------------------------------------
// Returns a random vector that is in front of the target and which is slightly altered each 
// frame to allow for random (but smooth) wandering movements
//--------------------------------------------------------------------------------------------
hgeVector AI::Wander(hgeVector currentPos, hgeVector currentVel)
{
	mWanderTarget = currentPos ;
	// Add a random small amount to the vector and normalise it.
	mWanderTarget += hgeVector((rand()%2-1) * mWanderDisplacement, (rand()%2-1) * mWanderDisplacement) ;
	mWanderTarget = *mWanderTarget.Normalize() ;

	// Set target to the distance required
	mWanderTarget *= mWanderRadius ;

	hgeVector localTarget = mWanderTarget + hgeVector(mWanderDistance, 0) ;

	return localTarget - currentPos ;
}


//------------------------------------- GetFunctions -----------------------------------------
//--------------------------------------------------------------------------------------------
bool AI::GetIsFleeing()
{
	return mIsFleeing ;
}


//EOF AI.cpp