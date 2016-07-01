// GameObject Class header
#ifndef GAME_OBJECT
#define GAME_OBJECT
//--------------------------------------------------------------------------------------------
//
//  Name:	GameObject.h
//
//  Desc:	Base game object class containing default members and class behaviour
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include <hge.h>			// Include HGE engine header
#include <hgevector.h>		// Include HGE vector header
#include <hgesprite.h>		// Include HGE texture header
#include "Utility.h"


extern HGE* hge ;			// External pointer to the HGE interface.
extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;

class GameObject
{
protected:

	// Texture handle for GameObject sprite
	HTEXTURE mObjectTex ;
	HTEXTURE mDestroyedTex ;

	// Pointer to the sprite of the GameObject
	hgeSprite *mpObjectSpr ;
	hgeSprite *mpDestroyedSpr ;

	// The Game Object's position in the game world
	hgeVector mPosition ;

	// The current velocity vector of the Game Object
	hgeVector mVelocity ;

	hgeVector mHeading ;

	// Set to true if the Game Object should be rendered
	bool mIsActive ;

	// The Game Object's current mAngle of rotation
	double mAngle ;

	// The drag co-efficient acting on the Game Object
	float mDragFactor ;

	// The rotational speed of the Game Object (degrees per frame)
	float mMaxRotationalSpeed ;

	// The acceleration factor of the Game Object
	float mAccelerationFactor ;

	// The rotation factor of the Game Object
	float mRotationFactor ;

	// The scale factor of the Game Object
	float mScaleFactor ;

	// The maximum velocity of the Game Object

	float mVelocityLimit ;

	bool mCollided ;

	int mDestroyedCountdown ;

	void LimitVelocity() ;
	void CheckMapEdges(int levelXSize, int levelYSize, bool remove) ;
	

public:	

	void Initialise(float xPos, float yPos, double rotation) ;
	~GameObject() ;

	// Update object movements
	void Update(float frameTime, int levelXSize, int levelYSize, bool remove) ;

	// Draw object
	void Render(float xPos, float yPos) ;

	// Rotate the object
	void Rotate(int direction, float frameTime) ;

	// Check for collisions between current objects position and passed position
	bool CheckCollision(hgeVector position, float aggroDist) ;

	// Used when objects collide so they bounce off each other
	void BounceBack(float bounceForce, bool bounceX, bool bounceY) ;

	// Return object's state
	bool GetIsActive() ;

	// Activate/Deactivate the Game Object
	void Activate() ;
	void Deactivate() ;

	double GetRotation() ;
	float GetScaleFactor() ;
	float GetXPos() ;
	float GetYPos() ;
	float GetXVel() ;
	float GetYVel() ;
	float GetMaxVel() ;
	void SetCollided() ;
	bool HasCollided() ;
	void SetDestroyedCountdown() ;
	int GetDestroyedCountdown() ;
	hgeVector GetPos() ;
	hgeVector GetVel() ;
	hgeVector GetHeading() ;
	void SetPos(float xPos, float yPos) ;
	void SetVel(hgeVector velocity) ;
	

};

#endif
//EOF GameObject.h


