// PlayerShip Class Code
#include "PlayerShip.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void PlayerShip::Initialise(float xPos, float yPos, double rotation)
{

	// Initialise ship with passed/default member values
	Ship::Initialise(xPos, yPos, rotation) ;

	// Set ship texture
	mObjectTex		= hge->Texture_Load("Resources/Textures/PlayerShip.png") ;
	mpObjectSpr		= new hgeSprite(mObjectTex, 0.0f, 0.0f, 256.0f, 256.0f) ;
	// Use 144 as Y pivot point so turning is centered slightly towards the rear of the ship
	mpObjectSpr->SetHotSpot(128.0f, 144.0f) ;
	mDestroyedTex	= hge->Texture_Load("Resources/Textures/DestroyedPlayerShip.png") ;
	mpDestroyedSpr	= new hgeSprite(mDestroyedTex, 0.0f, 0.0f, 256.0f, 256.0f) ;
	mpDestroyedSpr->SetHotSpot(128.0f, 144.0f) ;

	// Set ship's thrust texture
	thrustTex		= hge->Texture_Load("Resources/Textures/PlayerThrust.png") ;
	thrustSpr		= new hgeSprite(thrustTex, 0.0f, 0.0f, 256.0f, 256.0f) ;
	thrustSpr->SetHotSpot(128.0f, 144.0f) ;

	mThrustParticleTex	= hge->Texture_Load("Resources/ParticleTexture.png") ;
	mpThrustParticleSpr	= new hgeSprite(mThrustParticleTex, 0.0f, 0.0f, 32.0f, 32.0f) ;
	mpThrustParticleSpr->SetHotSpot(16.0f, 16.0f) ;
	mpThrustParticleSpr->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE) ;
	mpThrustParticle	= new hgeParticleSystem("Resources/particle6.psi", mpThrustParticleSpr);
	mpThrustParticle2	= new hgeParticleSystem("Resources/particle6.psi", mpThrustParticleSpr);

	mThrustOffset	= 110.0f ;
	mThrustOffset2	= 70.0f ;
	mDistanceOffset	= 35.0f ;
	thrustPos.x		= 0.0f ;
	thrustPos.y		= 0.0f ;
	thrust2Pos.x	= 0.0f ;
	thrust2Pos.y	= 0.0f ;


	// Set ship health box textures
	mHealthBoxTex	= hge->Texture_Load("Resources/Textures/HealthBox.png") ;
	mHealthBoxSpr	= new hgeSprite(mHealthBoxTex, 0.0f, 0.0f, 512.0f, 128.0f) ;

	mHealthNodeTex	= hge->Texture_Load("Resources/Textures/HealthNode.png") ;
	mHealthNodeSpr	= new hgeSprite(mHealthNodeTex, 0.0f, 0.0f, 128.0f, 64.0f) ;

	mHealth				= MAX_HEALTH ;
	mInvincibilityOn	= false ;

	if(!mObjectTex || !mDestroyedTex || !thrustTex || !mThrustParticleTex || !mHealthBoxTex || !mHealthNodeTex)
	{
		DisplayErrorBox("PlayerShip initialisation error - Texture.") ;
	}

	//Load sound effects
	mShootSnd		= hge->Effect_Load("Resources/Sounds/Shoot.wav") ;
	mThrustSnd		= hge->Effect_Load("Resources/Sounds/Thruster.wav") ;
	mInvincibleSnd	= hge->Effect_Load("Resources/Sounds/Invincible.wav") ;

	if(!mShootSnd || !mThrustSnd)
	{
		DisplayErrorBox("PlayerShip initialisation error - Audio.") ;
	}
}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
PlayerShip::~PlayerShip()
{
	// Free loaded resources
	hge->Effect_Free(mInvincibleSnd) ;
	hge->Effect_Free(mThrustSnd);
	hge->Effect_Free(mShootSnd);

	hge->Target_Free(mHealthNodeTex) ;
	hge->Target_Free(mHealthBoxTex) ;
	hge->Target_Free(thrustTex) ;


	// Free created pointers
	mpThrustParticle2	= NULL ;
	delete				mpThrustParticle2 ;

	mHealthNodeSpr	= NULL ;
	delete			mHealthNodeSpr ;
	mHealthBoxSpr	= NULL ;
	delete			mHealthBoxSpr ;
	thrustSpr		= NULL ;
	delete			thrustSpr ;
}


//---------------------------------------- Update --------------------------------------------
// Updates the player's ship each frame. Checks for key/controller input and updates ship
// values accordingly. Vibrates the controller when thrusting/shooting. Keeps the ship within
// the level bounds
//--------------------------------------------------------------------------------------------
void PlayerShip::Update(float frameTime, int levelXSize, int levelYSize, bool invincibilityAllowed, XInputController *controller)
{
	controller->Vibrate() ;
	
	// If ship is active check player control options
	if (GetIsActive() && !HasZeroHealth())
	{

		firedThisFrame = false ;	// Reset at the start of each frame
		static bool aPressed			= false ;
		static bool aPressedLastFrame	= false ;
		static bool leftPressed			= false ;
		static bool rightPressed		= false ;
		
		if (controller->ButtonPressed(XINPUT_GAMEPAD_A))
		{
			aPressed = true ;
		}
		else
		{
			aPressed = false ;
			aPressedLastFrame = false ;
		}
		if (hge->Input_KeyDown(HGEK_SPACE) || (aPressed && !aPressedLastFrame))//|| controller->ButtonPressed(XINPUT_GAMEPAD_A)) /*&& activeBullets < MAX_BULLETS)*/
		{
			// If bullet fired, increment activeBullets and trigger firedThisFrame bool
			// so Bullet class can create a new bullet.
			//activeBullets++ ;
			firedThisFrame = true ;
			hge->Effect_PlayEx(mShootSnd, 50, 0, 1.0f, false) ;
			controller->Vibrate(0.0f, 1.0f); 
			aPressedLastFrame = true ;
		}
		

		if ((hge->Input_KeyDown(HGEK_I) || controller->ButtonPressed(XINPUT_GAMEPAD_Y)) && invincibilityAllowed)
		{
			if (!mInvincibilityOn)
			{
				mInvincibilityOn = true ;
				hge->Effect_Play(mInvincibleSnd) ;
			}
		}

		if (controller->mLeftStick.x > 1)
		{
			rightPressed = true ;
		}
		else
		{
			rightPressed = false ;
		}
		if (hge->Input_GetKeyState(HGEK_D) || rightPressed)
		{// Rotate right/clockwise
			GameObject::Rotate(1, frameTime) ;
		}

		if (controller->mLeftStick.x < -1)
		{
			leftPressed = true ;
		}
		else
		{
			leftPressed = false ;
		}
		if (hge->Input_GetKeyState(HGEK_A) || leftPressed)
		{// Rotate left/counter clockwise
			GameObject::Rotate(-1, frameTime) ;
			// add some sort of drop off for rotation so rotating is smoother?
		}


		if (hge->Input_GetKeyState(HGEK_W) || controller->mRightTrigger > 0)
		{// Thrust forwards
			Accelerate(frameTime, controller->mRightTrigger) ;
			ActivateThrust() ;
			controller->Vibrate(0.08f, 0.0f) ; 

			// Calculate the thrust particle's origin point and direction in relation to the ship
			thrustPos.x = cos((mAngle+mThrustOffset)/180*M_PI)*mDistanceOffset ;
			thrustPos.y = sin((mAngle+mThrustOffset)/180*M_PI)*mDistanceOffset ;
			thrust2Pos.x = cos((mAngle+mThrustOffset2)/180*M_PI)*mDistanceOffset ;
			thrust2Pos.y = sin((mAngle+mThrustOffset2)/180*M_PI)*mDistanceOffset ;

			// Fire the thrust particles from their origin point
			mpThrustParticle->FireAt(SCREEN_WIDTH/2+thrustPos.x, SCREEN_HEIGHT/2+thrustPos.y) ;
			mpThrustParticle2->FireAt(SCREEN_WIDTH/2+thrust2Pos.x, SCREEN_HEIGHT/2+thrust2Pos.y) ;
			//mpThrustParticle->info.fSpeedMax = mpThrustParticle->info.fSpeedMax *

			if (!hge->Channel_IsPlaying(mSoundChannel1))
			{// Play thruster sound effect
			 // mSoundChannel is used so that the sound effect does not start to play each frame
				mSoundChannel1 = hge->Effect_PlayEx(mThrustSnd, 100, 0, 1.0f, false) ;
			}
		}
		else
		{
			DeactivateThrust() ;
			mpThrustParticle->Stop(false) ;
			mpThrustParticle2->Stop(false) ;
			hge->Channel_Stop(mSoundChannel1) ;
		}
		mpThrustParticle->info.fDirection = ((180+mAngle) * (M_PI / 180)) ;
		mpThrustParticle->Update(frameTime) ;
		mpThrustParticle2->info.fDirection = ((180+mAngle) * (M_PI / 180)) ;
		mpThrustParticle2->Update(frameTime) ;

		if (hge->Input_GetKeyState(HGEK_S) || controller->mLeftTrigger > 0)
		{// Brake/Thrust reversal
			Brake(frameTime) ;
		}


		// Update the ships GameObject members (calculates position/velocity/direction/drag etc)
		GameObject::Update(frameTime, levelXSize, levelYSize, false) ;

		// Stop ship moving outside the edge of the level area and sets velocity to 0 so the ship
		// does not get "stuck" slowing down on the edges of the level (Also keeps ship in middle
		// of screen so the edges of the level cannot be seen)
		if (mPosition.x < 0 + SCREEN_WIDTH/2)
		{
			mPosition.x = 0 + (float)SCREEN_WIDTH/2 ;
			mVelocity.x = 0 ;
		}
		if (mPosition.x > levelXSize - SCREEN_WIDTH/2)
		{
			mPosition.x = (float)levelXSize  - SCREEN_WIDTH/2 ;
			mVelocity.x = 0 ;
		}
		if (mPosition.y < 0 + SCREEN_HEIGHT/2)
		{
			mPosition.y = 0 + (float)SCREEN_HEIGHT/2 ;
			mVelocity.y = 0 ;
		}
		if (mPosition.y > levelYSize - SCREEN_HEIGHT/2)
		{
			mPosition.y = (float)levelYSize - SCREEN_HEIGHT/2 ;
			mVelocity.y = 0 ;
		}
	}//end if - PlayerShip.GetIsActive()
}


//---------------------------------------- Render --------------------------------------------
// Render the ship in the middle of the screen, along with the ship's thruster 
// sprite/particles when thrusting, and the destroyed sprite if the ship dies
//--------------------------------------------------------------------------------------------
void PlayerShip::Render(float frameTime) 
{
	float tempX, tempY ;
	tempX = (float)SCREEN_WIDTH/2 ;
	tempY = (float)SCREEN_HEIGHT/2 ;

	if (mIsActive)
	{
		mpThrustParticle->Render() ;
		mpThrustParticle2->Render() ;
	}
	Ship::Render(tempX, tempY) ;
	if (mDestroyedCountdown > 0)
	{
		mpDestroyedSpr->SetColor(ARGB(mDestroyedCountdown, 255, 255, 255)) ;
		mpDestroyedSpr->RenderEx(tempX, tempY, (float)(2 * M_PI *(mAngle/360)), mScaleFactor, mScaleFactor) ;
		mDestroyedCountdown-- ;
	}
	else if (mIsActive && isThrusting)
	{
		thrustSpr->RenderEx(tempX, tempY, (float)(mAngle * (M_PI / 180)), 
			GameObject::mScaleFactor, GameObject::mScaleFactor) ;
	}

}


//----------------------------------- RenderHealthBox  ---------------------------------------
// Displays the players current health in the bottom left corner of the screen. Health is 
// coloured according to how much is remaining and flashes when low. Can currently display 
// HP up to a value of 18 (3 rows of 6)
//--------------------------------------------------------------------------------------------
void PlayerShip::RenderHealthBox()
{

	// Draw Health box at bottom left of screen
	mHealthBoxSpr->Render(10.0f, ((float)SCREEN_HEIGHT-128.0f)-10.0f) ;
	// Keep track of frames in a counter
	static int flashCounter = 0 ;
	static float flashValue = 0;
	// from 150-255 and back to 150 again (in multiples of 5 for faster flashing)
	flashValue = abs((((flashCounter*5)) % 210) - 105) + 150 ;
	flashCounter ++ ;

	//Colour health nodes according to remaining health
	if (mInvincibilityOn)
	{
		mHealthNodeSpr->SetColor(ARGB(flashValue, 255, 210, 0)) ; // Flashes gold for invincibility
	}
	else if (mHealth > 4)
	{
		mHealthNodeSpr->SetColor(ARGB(255, 0, 180, 255)) ; // Blue - high hp
	}
	else if (mHealth > 2)
	{
		mHealthNodeSpr->SetColor(ARGB(255, 255, 125, 25)) ; // Orange - medium hp		
	}
	else
	{
		mHealthNodeSpr->SetColor(ARGB(flashValue, 255, 0, 0)) ; // Red - low hp (flashes)
	}

	// Display health nodes according to the health value
	float spacing = 75.0f ;
	for (int i = 0; i < mHealth; i++)
	{
		if (i < 6)
		{
			mHealthNodeSpr->Render(10.0f+(i*spacing), ((float)SCREEN_HEIGHT-64.0f)-10.0f) ;
		}
		else if (i < 12)
		{
			mHealthNodeSpr->Render(10.0f+(i*spacing)-450.0f, ((float)SCREEN_HEIGHT-94.0f)-10.0f) ;
		}
		else if (i < 18)
		{
			mHealthNodeSpr->Render(10.0f+(i*spacing)-900.0f, ((float)SCREEN_HEIGHT-124.0f)-10.0f) ;
		}
	}

}


//----------------------------------- IncrementHealth  ---------------------------------------
// Increases health by one if it can be. Used when a health pickip is collected
//--------------------------------------------------------------------------------------------
void PlayerShip::IncrementHealth()
{
	// Only add HP if it is within a valid range/player is not invincible
	if (!mInvincibilityOn && mHealth < MAX_HEALTH)
	{
		mHealth++ ;
	}
}


//----------------------------------- DecrementHealth  ---------------------------------------
// Decreases health by one. Used when ship collides with an object
//--------------------------------------------------------------------------------------------
void PlayerShip::DecrementHealth()
{
	// Remove health if not invincible
	if (!mInvincibilityOn)
	{
		mHealth-- ;
	}
}


//------------------------------------ HasZeroHealth  ----------------------------------------
// Checks if player's health is is within a valid range, true is returned if the ship should
//--------------------------------------------------------------------------------------------
bool PlayerShip::HasZeroHealth()
{
	if (mHealth > 0 && mHealth <= MAX_HEALTH)
	{
		return false ;
	}
	else
	{
		return true ;
	}
}


//---------------------------------------- Reset  --------------------------------------------
// Reset ship values to their defaults. used when player dies/level is reset
//--------------------------------------------------------------------------------------------
void PlayerShip::Reset(float xPos, float yPos, float rotation)
{
	mInvincibilityOn	= false ;

	// Reset position/rotation
	mPosition.x		= xPos ;
	mPosition.y		= yPos ;
	mAngle			= rotation ;

	// Reset other values to defaults
	mVelocity.x		= 0 ;
	mVelocity.y		= 0 ;

	mHeading.x		= 0 ;
	mHeading.y		= 0 ;

	mHealth			= MAX_HEALTH ;

	mIsActive		= true ;
	mCollided		= false ;
	isThrusting		= false ;
	firedThisFrame	= false ;

	// Stop any particles on screen from rendering/updating next frame
	mpThrustParticle->Stop(true) ;
	mpThrustParticle2->Stop(true) ;
}

//EOF PlayerShip.cpp
