// AI Class header
#ifndef AI_CLASS
#define AI_CLASS
//--------------------------------------------------------------------------------------------
//
//  Name:	AI.h
//
//  Desc:	Contains steering behavours to be used by autonomous agents
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include <hge.h>			// Include HGE engine header
#include <hgevector.h>		// Include HGE vector header

#include "Utility.h"


extern HGE* hge ;			// External pointer to the HGE interface.


class AI
{
protected:

	hgeVector	mWanderTarget ;
	double		mWanderDistance ;
	double		mWanderRadius ;
	double		mWanderDisplacement ;

	bool		mIsFleeing ;


public:

	enum Deceleration { fast = 3, normal = 4, slow = 5} ;

	AI() ;
	~AI() ;

	hgeVector Seek(hgeVector targetPos, hgeVector currentPos, hgeVector currentVel, float maxVel) ;

	hgeVector Pursuit(hgeVector targetPos, const hgeVector &targetHeading, hgeVector targetCurrentVel, 
		float targetMaxVel,	hgeVector currentPos, const hgeVector &currentHeading, 
		hgeVector currentVel, float maxVel) ;

	hgeVector Arrive(hgeVector targetPos, hgeVector currentPos, hgeVector currentVel, float maxVel,
		Deceleration deceleration) ;

	hgeVector Flee(hgeVector targetPos, hgeVector currentPos, hgeVector currentVel, float maxVel) ;

	hgeVector FleeOnAggro(hgeVector targetPos, hgeVector currentPos, hgeVector currentVel, float maxVel, float aggroDist) ;

	hgeVector Evade(hgeVector targetPos, hgeVector targetCurrentVel, float targetMaxVel, hgeVector currentPos, 
		hgeVector currentVel, float maxVel) ;

	hgeVector Wander(hgeVector currentPos, hgeVector currentVel) ;

	bool GetIsFleeing() ;

	
};
#endif
// EOF AI.h