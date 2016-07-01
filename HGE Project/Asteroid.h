// Asteroid Class header
#ifndef ASTEROID
#define ASTEROID
//--------------------------------------------------------------------------------------------
//
//  Name:	Asteroid.h
//
//  Desc:	Asteroid class, defines small/large asteroids
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


#include <hge.h>			// Include HGE engine header
#include <hgevector.h>		// Include HGE vector header

#include "GameObject.h"


extern HGE* hge ;			// External pointer to the HGE interface.

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

class Asteroid : public GameObject 
{
protected:

public:
	void Initialise(float xPos, float yPos, bool large) ;
	~Asteroid() ;

	// Update Asteroid movements
	void Update(float frameTime, int levelXSize, int levelYSize) ;

	// Draw Asteroid
	void Render(float xPos, float yPos) ;
	void RenderCrackedSprite(float xPos, float yPos) ;

	void Reset() ;

};


#endif
//EOF Asteriod.h


