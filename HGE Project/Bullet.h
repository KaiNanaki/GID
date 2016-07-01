// Bullet Class header
#ifndef BULLET
#define BULLET
//--------------------------------------------------------------------------------------------
//
//  Name:	Bullet.h
//
//  Desc:	Bullet class, defines Bullets to be used by the player
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include <hge.h>			// Include HGE engine header
#include <hgevector.h>		// Include HGE vector header
#include <hgesprite.h>		// Include HGE texture header

#include "GameObject.h"


extern HGE* hge ;			// External pointer to the HGE interface.

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

class Bullet : public GameObject 
{
private:
	
	int mBulletTime ;
	static const int MAX_BULLET_TIME = 500 ;

public:
	

	void Initialise(float xPos, float yPos, float xVel, float yVel, double rotation) ;
	~Bullet() ;

	// Update Bullet movements
	void Update(float frameTime, int levelXSize, int levelYSize) ;

	// Draw Bullet
	void Render(float xPos, float yPos) ;

	void Reset(float xPos, float yPos, float xVel, float yVel, double rotation) ;


};

#endif
//EOF Bullet.h


