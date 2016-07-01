// LevelOne.cpp file
#include "LevelOne.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void LevelOne::Initialise()
{
	Level::Initialise(playerStartingXPos, playerStartingYPos) ;

	// Load the level texture
	levelTex	= hge->Texture_Load("Resources/Textures/Space-V1 medium.jpg") ;
	levelSpr	= new hgeSprite(levelTex, 0.0f, 0.0f, (float)LEVEL_X_SIZE, (float)LEVEL_Y_SIZE) ;

	// Load text font and set defaults
	textFont = new hgeFont("Resources/Arial.fnt") ;
	textFont->SetColor(0xFFFFFFFF) ;
	textFont->SetScale(0.7f) ;

	if(!levelTex || !textFont)
	{
		DisplayErrorBox("Level 1 initialisation error - Texture.") ;
	}

	mLevel1Music			= hge->Stream_Load("Resources/Sounds/Anticipation.mp3") ;
	mAsteroidCollide1Snd	= hge->Effect_Load("Resources/Sounds/Bounce1.wav") ;
	mAsteroidCollide2Snd	= hge->Effect_Load("Resources/Sounds/Bounce2.wav") ;
	mAsteroidCollide3Snd	= hge->Effect_Load("Resources/Sounds/Bounce3.wav") ;
	mAsteroidCrack1Snd		= hge->Effect_Load("Resources/Sounds/Cracking1.wav") ;
	mAsteroidCrack2Snd		= hge->Effect_Load("Resources/Sounds/Cracking2.wav") ;
	mAsteroidCrack3Snd		= hge->Effect_Load("Resources/Sounds/Cracking3.wav") ;
	mLevelLostSnd			= hge->Effect_Load("Resources/Sounds/Crash.wav") ;

	if(!mLevel1Music || !mAsteroidCollide1Snd || !mAsteroidCollide2Snd || !mAsteroidCollide3Snd 
		|| !mAsteroidCrack1Snd || !mAsteroidCrack2Snd || !mAsteroidCrack3Snd || !mLevelLostSnd)
	{
		DisplayErrorBox("Level 1 initialisation error - Audio.") ;
	}
	// Start playing music as soon as the menu is loaded.
	mSoundChannel1 = hge->Stream_Play(mLevel1Music, true, 30) ;
	hge->Channel_Pause(mSoundChannel1) ;


	mTime		= MAX_LEVEL_TIME ;
	convoCount	= 0 ;

	// Initialise space pirates
	for (int i = 0 ; i < MAX_SPACE_PIRATES ; i++)
	{
		hgeVector randomPiratePos(0.0f, 0.0f) ;
		do
		{
			randomPiratePos.x = rand()%(LEVEL_X_SIZE - SCREEN_WIDTH) + (SCREEN_WIDTH/2) ;
			randomPiratePos.y = rand()%(LEVEL_Y_SIZE - SCREEN_HEIGHT) + (SCREEN_HEIGHT/2) ;
		}
		while (IsOnScreen(randomPiratePos.x, randomPiratePos.y)) ;
		pirateShip[i].Initialise() ;
		pirateShip[i].SetPos(randomPiratePos.x, randomPiratePos.y) ;

	}


	// Initialise Large Asteroids
	for (int i = 0 ; i < MAX_LARGE_ASTEROIDS ; i++)
	{
		hgeVector randomPos(0.0f, 0.0f) ;
		bool onAnotherAsteroid  = false ;

		// Make sure asteroids dont spawn inside the screen bounds near the ship
		// Asteroids can still spawn on the edges of the screen.
		do
		{
			onAnotherAsteroid  = false ;
			randomPos.x = rand()%(LEVEL_X_SIZE - SCREEN_WIDTH) + (SCREEN_WIDTH/2) ;
			randomPos.y = rand()%(LEVEL_Y_SIZE - SCREEN_HEIGHT) + (SCREEN_HEIGHT/2) ;

			for (int j = 0 ; j < MAX_LARGE_ASTEROIDS ; j++)
			{
				if (largeAsteroid[j].CheckCollision(randomPos, ((LARGE_ASTEROID_SIZE*2.5)*largeAsteroid[j].GetScaleFactor())))
				{
					onAnotherAsteroid = true ;
				}
			}
		}
		while (IsOnScreen(randomPos.x, randomPos.y) || onAnotherAsteroid) ;

		// Initialise large asteroids
		largeAsteroid[i].Initialise(randomPos.x, randomPos.y, true) ;
	}// end for

	// Initialise small Asteroids
	for (int i = 0 ; i < MAX_SMALL_ASTEROIDS ; i++)
	{
		hgeVector randomPos(0.0f, 0.0f) ;
		bool onAnotherAsteroid  = false ;

		// Make sure asteroids dont spawn inside the screen bounds near the ship
		// Asteroids can still spawn on the edges of the screen.
		do
		{
			onAnotherAsteroid  = false ;
			randomPos.x = rand()%(LEVEL_X_SIZE - SCREEN_WIDTH) + (SCREEN_WIDTH/2) ;
			randomPos.y = rand()%(LEVEL_Y_SIZE - SCREEN_HEIGHT) + (SCREEN_HEIGHT/2) ;

			for (int j = 0 ; j < MAX_LARGE_ASTEROIDS ; j++)
			{
				if (smallAsteroid[j].CheckCollision(randomPos, ((SMALL_ASTEROID_SIZE*2.5)*smallAsteroid[j].GetScaleFactor())))
				{
					onAnotherAsteroid = true ;
				}
			}
		}
		while (IsOnScreen(randomPos.x, randomPos.y) || onAnotherAsteroid) ;

		// Initialise large asteroids
		smallAsteroid[i].Initialise(randomPos.x, randomPos.y, false) ;
		smallAsteroid[i].Deactivate() ;
		// initialise small asteroids
	}// end for

}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
LevelOne::~LevelOne()
{
	// Free loaded resources
	hge->Stream_Free(mLevel1Music) ;
	hge->Effect_Free(mLevelLostSnd) ;
	hge->Effect_Free(mAsteroidCrack3Snd) ;
	hge->Effect_Free(mAsteroidCrack2Snd) ;
	hge->Effect_Free(mAsteroidCrack1Snd) ;
	hge->Effect_Free(mAsteroidCollide3Snd) ;
	hge->Effect_Free(mAsteroidCollide2Snd) ;
	hge->Effect_Free(mAsteroidCollide1Snd) ;

	// Delete pointers
	textFont	= NULL ;
	delete		textFont ;
	
}


//---------------------------------------- Update --------------------------------------------
// Updates the levels intro/outro during those sequences, otherwise updates the level and
// its game objects along with level one member variables (time, score, win condition)
//--------------------------------------------------------------------------------------------
void LevelOne::Update(float frameTime, XInputController *controller)
{
	// Update level position in relation to player's ship
	Level::Update(frameTime) ;

	// Update ConvoBoxes if during Intro/Outro/GameOver sequence
	if (!introCompleted || levelWon || levelLost)
	{
		//draw black image over everything, fade from 255 alpha to 0? 
		convoBox.Update(frameTime, controller) ;
	}
	else if (midPointReached && !midPointCompleted)
	{
	}
	else // If not in an intro/outro etc then update the level objects
	{
		if (!hge->Channel_IsPlaying(mSoundChannel1))
		{
			hge->Channel_Resume(mSoundChannel1) ;
		}

		// Update player ship and its bullets, as well as checking if health has been picked up
		Level::UpdateBaseGameObjects(frameTime, controller) ;


		// Update pirate ships and check if they need to respawn (after death/going out of level bounds)
		for (int i = 0 ; i < MAX_SPACE_PIRATES ; i++)
		{
			pirateShip[i].Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE, player.GetPos()) ;
			if (!pirateShip[i].GetIsActive() && pirateShip[i].GetDestroyedCountdown()  < 1)
			{
				hgeVector randomPiratePos(0.0f, 0.0f) ;
				// Make sure the pirate ship respawns outside of the screen bounds but within level bounds
				do
				{
					randomPiratePos.x = rand()%(LEVEL_X_SIZE - SCREEN_WIDTH) + (SCREEN_WIDTH/2) ;
					randomPiratePos.y = rand()%(LEVEL_Y_SIZE - SCREEN_HEIGHT) + (SCREEN_HEIGHT/2) ;
				}
				while (IsOnScreen(randomPiratePos.x, randomPiratePos.y)) ;

				pirateShip[i].SetPos(randomPiratePos.x, randomPiratePos.y) ;
				pirateShip[i].SetVel(hgeVector(0.0f, 0.0f)) ;
				pirateShip[i].Activate() ;
			}
		}

		// Update the level's active large Asteroids
		for (int i = 0 ; i < MAX_LARGE_ASTEROIDS ; i++)
		{
			largeAsteroid[i].Rotate(-1, frameTime) ;
			largeAsteroid[i].Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE) ;
		}
		// Update the level's active small Asteroids
		for (int h = 0 ; h < MAX_SMALL_ASTEROIDS ; h++)
		{
			smallAsteroid[h].Rotate(-1, frameTime) ;
			smallAsteroid[h].Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE) ;
		}

		// Check for collisions between the levels objects and react accordingly
		// (swap velocities, remove health, remove game objects, etc)
		CheckLevelCollisions(frameTime) ;
		UpdateScore(frameTime) ;
		mTime -= frameTime ;
	}

	if (!levelLost && player.HasZeroHealth())
	{
		player.Deactivate() ;
    	player.SetDestroyedCountdown() ;
		levelLost			= true ;
		allowInvincibility	= true ;
		hge->Channel_Pause(mSoundChannel1) ;
		hge->Effect_Play(mLevelLostSnd) ;
	}
	if (hge->Input_GetKeyState(HGEK_P) || controller->ButtonPressed(XINPUT_GAMEPAD_START))
	{
		hge->Channel_Pause(mSoundChannel1) ;
	}

	if (mTime <= 0 && !player.HasZeroHealth())
	{
		levelWon = true ;
	}

}


//--------------------------------- CheckLevelCollisions -------------------------------------
// Loops through each of the levels game objects to check for collisions between them, if a 
// collision has occured then react acordingly (swap velocities, remove health, remove game
// objects, etc)
//--------------------------------------------------------------------------------------------
void LevelOne::CheckLevelCollisions(float frameTime)
{

	// Loop through large asteroids ------------------------------
	for (int l = 0 ; l < MAX_LARGE_ASTEROIDS ; l++)
	{
		// Check current large asteroid colliding with another large asteroid
		for (int m = 0 ; m < MAX_LARGE_ASTEROIDS ; m++)
		{
			// Dont check the same asteroid against itself
			// & make sure both asteroids are currently active
			if (l != m && largeAsteroid[l].GetIsActive() && largeAsteroid[m].GetIsActive()
				&& largeAsteroid[l].HasCollided() == false && largeAsteroid[m].HasCollided() == false)
			{
				if (largeAsteroid[l].CheckCollision(largeAsteroid[m].GetPos(),
					((LARGE_ASTEROID_SIZE*2)*largeAsteroid[l].GetScaleFactor())))
				{
					if (IsOnScreen(largeAsteroid[l].GetXPos(), largeAsteroid[l].GetYPos())
						&& IsOnScreen(largeAsteroid[m].GetXPos(), largeAsteroid[m].GetYPos()))
					{
						int effectChoice = rand()%3 ;
						if (effectChoice == 0)
						{
							hge->Effect_Play(mAsteroidCollide1Snd) ;
						}
						else if (effectChoice == 1)
						{
							hge->Effect_Play(mAsteroidCollide2Snd) ;
						}
						else
						{
							hge->Effect_Play(mAsteroidCollide3Snd) ;
						}
					}
					// If they collide, make them bounce off one another by
					// swapping their velocities and slowing them slightly
					hgeVector temp = largeAsteroid[l].GetVel() ;
					largeAsteroid[l].SetVel(largeAsteroid[m].GetVel()) ;
					largeAsteroid[m].SetVel(temp) ;
					// Count down so asteroids dont collide again for a few frames
					largeAsteroid[l].SetCollided() ;
					largeAsteroid[m].SetCollided() ;
					
				}
			}
		}// end for	- large colliding with large

		

		// Check current large asteroids colliding with player bullets
		for (int n = 0 ; n < player.MAX_BULLETS ; n++)
		{
			// Make sure both asteroids are active
			if (largeAsteroid[l].GetIsActive() && playerBullet[n].GetIsActive())
			{
				if (largeAsteroid[l].CheckCollision(playerBullet[n].GetPos(),
					(LARGE_ASTEROID_SIZE*largeAsteroid[l].GetScaleFactor()) 
					+ (BULLET_SIZE*playerBullet[n].GetScaleFactor())))
				{
					if (IsOnScreen(largeAsteroid[l].GetXPos(), largeAsteroid[l].GetYPos())
						&& IsOnScreen(playerBullet[n].GetXPos(), playerBullet[n].GetYPos()))
					{
						int effectChoice = rand()%3 ;
						if (effectChoice == 0)
						{
							hge->Effect_Play(mAsteroidCrack1Snd) ;
						}
						else if (effectChoice == 1)
						{
							hge->Effect_Play(mAsteroidCrack2Snd) ;
						}
						else
						{
							hge->Effect_Play(mAsteroidCrack3Snd) ;
						}

						if (rand()%HP_CHANCE_LOW == 0)
						{
							mHealthPickup.SpawnAt(largeAsteroid[l].GetPos(), LEVEL_X_SIZE, LEVEL_Y_SIZE) ;
						}	
					}// end if - IsOnScreen
					largeAsteroid[l].Deactivate() ;
					largeAsteroid[l].SetDestroyedCountdown() ;
					smallAsteroid[(l*3)].SetPos(largeAsteroid[l].GetXPos(), largeAsteroid[l].GetYPos()) ;
					smallAsteroid[(l*3)].SetVel(smallAsteroid[(l*3)].GetVel() - largeAsteroid[l].GetVel()) ;
					smallAsteroid[(l*3)].Activate() ;
					smallAsteroid[(l*3)].Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE) ;
					smallAsteroid[(l*3)+1].SetPos(largeAsteroid[l].GetXPos(), largeAsteroid[l].GetYPos()) ;
					smallAsteroid[(l*3)+1].SetVel(smallAsteroid[(l*3)+1].GetVel() + largeAsteroid[l].GetVel()) ;
					smallAsteroid[(l*3)+1].Activate() ;
					smallAsteroid[(l*3)+1].Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE) ;

					hgeVector Ast1 = smallAsteroid[(l*3)].GetPos() ;
					hgeVector Ast2 = smallAsteroid[(l*3)+1].GetPos() ;
					//hgeVector pushAway = smallAsteroid[(l*3)].GetPos() - smallAsteroid[(l*3)+1].GetPos() ;
					hgeVector pushAway = Ast2 - Ast1 ;
					hgeVector pushAwayNorm = *pushAway.Normalize() ;

					float requiredDistance	= ((SMALL_ASTEROID_SIZE*2.1)*smallAsteroid[(l*3)].GetScaleFactor()) ;
					//requiredDistance		-= (DistanceBetween(smallAsteroid[(l*3)].GetPos(), smallAsteroid[(l*3)+1].GetPos()))  ;
					requiredDistance		-= DistanceBetween(Ast2, Ast1)  ;

					//smallAsteroid[(l*3)].SetPos(smallAsteroid[(l*3)].GetPos().x+(pushAway.x*requiredDistance), 
					//	smallAsteroid[(l*3)].GetPos().y+(pushAway.y*requiredDistance)) ;
					//smallAsteroid[(l*3)].SetPos(smallAsteroid[(l*3)].GetPos().x-(pushAway.x*requiredDistance), 
					//	smallAsteroid[(l*3)].GetPos().y-(pushAway.y*requiredDistance)) ;
					smallAsteroid[(l*3)].SetPos(Ast1.x - (pushAwayNorm.x*requiredDistance), 
						Ast1.y - (pushAwayNorm.y*requiredDistance)) ;
					smallAsteroid[(l*3)].SetPos(Ast2.x + (pushAwayNorm.x*requiredDistance), 
						Ast2.y + (pushAwayNorm.y*requiredDistance)) ;

					smallAsteroid[(l*3)].SetVel(-pushAway * smallAsteroid[(l*3)].GetVel().Length()) ;
					smallAsteroid[(l*3)+1].SetVel(pushAway * smallAsteroid[(l*3)+1].GetVel().Length()) ;

					smallAsteroid[(l*3)].SetCollided() ;
					smallAsteroid[(l*3)+1].SetCollided() ;

					playerBullet[n].Deactivate() ;
					IncrementScore(SCORE_TYPE_SMALL) ;
				}
			}
		}// end for - large colliding with bullets


		// Check large asteroids colliding with player ship
		if (largeAsteroid[l].GetIsActive() && largeAsteroid[l].CheckCollision(player.GetPos(),
			(LARGE_ASTEROID_SIZE*largeAsteroid[l].GetScaleFactor()) + (PLAYER_SHIP_SIZE*player.GetScaleFactor())))
		{
			hge->Effect_Play(mShipHitSnd) ;

			// Dont need to check if asteroid/player are on screen as ship is always in the centre
			int effectChoice = rand()%3 ;
			if (effectChoice == 0)
			{
				hge->Effect_Play(mAsteroidCrack1Snd) ;
			}
			else if (effectChoice == 1)
			{
				hge->Effect_Play(mAsteroidCrack2Snd) ;
			}
			else
			{
				hge->Effect_Play(mAsteroidCrack3Snd) ;
			}

			hgeVector temp1 = largeAsteroid[l].GetVel() ;
			largeAsteroid[l].Deactivate() ;
			largeAsteroid[l].SetDestroyedCountdown() ;
			smallAsteroid[(l*3)].SetPos(largeAsteroid[l].GetXPos(), largeAsteroid[l].GetYPos()) ;
			smallAsteroid[(l*3)].SetVel(smallAsteroid[(l*3)].GetVel() - largeAsteroid[l].GetVel()) ;
			smallAsteroid[(l*3)].Activate() ;
			smallAsteroid[(l*3)].Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE) ;
			smallAsteroid[(l*3)+1].SetPos(largeAsteroid[l].GetXPos(), largeAsteroid[l].GetYPos()) ;
			smallAsteroid[(l*3)+1].SetVel(smallAsteroid[(l*3)+1].GetVel() + largeAsteroid[l].GetVel()) ;
			smallAsteroid[(l*3)+1].Activate() ;
			smallAsteroid[(l*3)+1].Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE) ;

			hgeVector Ast1 = smallAsteroid[(l*3)].GetPos() ;
			hgeVector Ast2 = smallAsteroid[(l*3)+1].GetPos() ;
			hgeVector pushAway = Ast2 - Ast1 ;
			hgeVector pushAwayNorm = *pushAway.Normalize() ;

			float requiredDistance	= ((SMALL_ASTEROID_SIZE*2.1)*smallAsteroid[(l*3)].GetScaleFactor()) ;
			requiredDistance		-= DistanceBetween(Ast2, Ast1)  ;

			smallAsteroid[(l*3)].SetPos(Ast1.x - (pushAwayNorm.x*requiredDistance), 
				Ast1.y - (pushAwayNorm.y*requiredDistance)) ;
			smallAsteroid[(l*3)].SetPos(Ast2.x + (pushAwayNorm.x*requiredDistance), 
				Ast2.y + (pushAwayNorm.y*requiredDistance)) ;

			smallAsteroid[(l*3)].SetVel(pushAway * smallAsteroid[(l*3)].GetVel().Length()) ;
			smallAsteroid[(l*3)+1].SetVel(pushAway * smallAsteroid[(l*3)+1].GetVel().Length()) ;


			player.SetVel(largeAsteroid[l].GetVel()) ;
			player.DecrementHealth() ;
		}// end for - large colliding with ship

	}// end for - large asteroid collisions

	// Small asteroid collisions -------------------------------------
	for (int l = 0 ; l < MAX_SMALL_ASTEROIDS ; l++)
	{
		
		// Check current small asteroid colliding with another small asteroid
		for (int m = 0 ; m < MAX_SMALL_ASTEROIDS ; m++)
		{
			// Dont check the same asteroid against itself
			// & make sure both asteroids are currently active
			if (l != m && smallAsteroid[l].GetIsActive() && smallAsteroid[m].GetIsActive()
				&& smallAsteroid[l].HasCollided() == false && smallAsteroid[m].HasCollided() == false)
			{
				if (smallAsteroid[l].CheckCollision(smallAsteroid[m].GetPos(),
					(SMALL_ASTEROID_SIZE*smallAsteroid[l].GetScaleFactor())))
				{
					if (IsOnScreen(smallAsteroid[l].GetXPos(), smallAsteroid[l].GetYPos())
						&& IsOnScreen(smallAsteroid[m].GetXPos(), smallAsteroid[m].GetYPos()))
					{
						int effectChoice = rand()%3 ;
						if (effectChoice == 0)
						{
							hge->Effect_Play(mAsteroidCollide1Snd) ;
						}
						else if (effectChoice == 1)
						{
							hge->Effect_Play(mAsteroidCollide2Snd) ;
						}
						else
						{
							hge->Effect_Play(mAsteroidCollide3Snd) ;
						}
					}// end if - IsOnScreen

					// If they collide, make them bounce off one another by
					// swapping their velocities
					hgeVector temp = smallAsteroid[l].GetVel() ;
					smallAsteroid[l].SetVel(smallAsteroid[m].GetVel()) ;
					smallAsteroid[m].SetVel(temp) ;
					// Count down so asteroids dont collide again for a few frames
					smallAsteroid[l].SetCollided() ;
					smallAsteroid[m].SetCollided() ;
				}
			}
		}// end for	- small colliding with small	
		

		// Check current small asteroids colliding with player bullets
		for (int n = 0 ; n < player.MAX_BULLETS ; n++)
		{
			// Make sure both asteroids are active
			if (smallAsteroid[l].GetIsActive() && playerBullet[n].GetIsActive())
			{
				if (smallAsteroid[l].CheckCollision(playerBullet[n].GetPos(),
					(SMALL_ASTEROID_SIZE*smallAsteroid[l].GetScaleFactor()) + (BULLET_SIZE*playerBullet[n].GetScaleFactor())))
				{
					if (IsOnScreen(smallAsteroid[l].GetXPos(), smallAsteroid[l].GetYPos())
						&& IsOnScreen(playerBullet[n].GetXPos(), playerBullet[n].GetYPos()))
					{
						int effectChoice = rand()%3 ;
						if (effectChoice == 0)
						{
							hge->Effect_Play(mAsteroidCrack1Snd) ;
						}
						else if (effectChoice == 1)
						{
							hge->Effect_Play(mAsteroidCrack2Snd) ;
						}
						else
						{
							hge->Effect_Play(mAsteroidCrack3Snd) ;
						}

						if (rand()%HP_CHANCE_HIGH == 0)
						{
							mHealthPickup.SpawnAt(smallAsteroid[l].GetPos(), LEVEL_X_SIZE, LEVEL_Y_SIZE) ;
						}
					}// end if - IsOnScreen

					smallAsteroid[l].Deactivate() ;
					smallAsteroid[l].SetDestroyedCountdown() ;
					playerBullet[n].Deactivate() ;
					IncrementScore(SCORE_TYPE_MEDIUM) ;
				}
			}
		}// end for - small colliding with player bullets


		// Check small asteroids colliding with player ship
		if (smallAsteroid[l].GetIsActive() && smallAsteroid[l].CheckCollision(player.GetPos(),
			(SMALL_ASTEROID_SIZE*smallAsteroid[l].GetScaleFactor()) + (PLAYER_SHIP_SIZE*player.GetScaleFactor())))
		{
			hge->Effect_Play(mShipHitSnd) ;
			int effectChoice = rand()%3 ;
			if (effectChoice == 0)
			{
				hge->Effect_Play(mAsteroidCrack1Snd) ;
			}
			else if (effectChoice == 1)
			{
				hge->Effect_Play(mAsteroidCrack2Snd) ;
			}
			else
			{
				hge->Effect_Play(mAsteroidCrack3Snd) ;
			}
			
			smallAsteroid[l].Deactivate() ;
			smallAsteroid[l].SetDestroyedCountdown() ;
			player.SetVel(smallAsteroid[l].GetVel()) ;
			player.DecrementHealth() ;
		}// end if - small colliding with ship

	}// end for - small asteroid collisions

	// Loop through large and small asteroids to check for collisions between them
	for (int l = 0 ; l < MAX_SMALL_ASTEROIDS ; l++)
	{	
		// Check current small asteroid colliding with all large asteroids
		for (int m = 0 ; m < MAX_LARGE_ASTEROIDS ; m++)
		{
			// Make sure both asteroids are currently active and havnt collided this frame
			if (smallAsteroid[l].GetIsActive() && largeAsteroid[m].GetIsActive()
				&& smallAsteroid[l].HasCollided() == false && largeAsteroid[m].HasCollided() == false)
			{
				if (smallAsteroid[l].CheckCollision(largeAsteroid[m].GetPos(),
					((SMALL_ASTEROID_SIZE*smallAsteroid[l].GetScaleFactor())
					+ LARGE_ASTEROID_SIZE*largeAsteroid[m].GetScaleFactor())))
				{
					if (IsOnScreen(smallAsteroid[l].GetXPos(), smallAsteroid[l].GetYPos())
						&& IsOnScreen(largeAsteroid[m].GetXPos(), largeAsteroid[m].GetYPos()))
					{
						int effectChoice = rand()%3 ;
						if (effectChoice == 0)
						{
							hge->Effect_Play(mAsteroidCollide1Snd) ;
						}
						else if (effectChoice == 1)
						{
							hge->Effect_Play(mAsteroidCollide2Snd) ;
						}
						else
						{
							hge->Effect_Play(mAsteroidCollide3Snd) ;
						}
					}
					// If they collide, make them bounce off one another by
					// swapping their velocities
					hgeVector temp = smallAsteroid[l].GetVel() ;
					smallAsteroid[l].SetVel(largeAsteroid[m].GetVel()) ;
					largeAsteroid[m].SetVel(temp) ;
					// Count down so asteroids dont collide again for a few frames
					smallAsteroid[l].SetCollided() ;
					largeAsteroid[m].SetCollided() ;
				}
			}
		}// end for	
	}

	//Check pirateShip colliding with player bullets
	for (int i = 0 ; i < MAX_SPACE_PIRATES ; i++)
	{
		if (pirateShip[i].GetIsActive())
		{
			for (int j = 0 ; j < player.MAX_BULLETS ; j++)
			{
				// Make sure the pirate ship and bullets are both active, and then check for 
				// collisoins between them both
				if (playerBullet[j].GetIsActive() && 
					pirateShip[i].CheckCollision(playerBullet[j].GetPos(), 
						(PIRATE_SHIP_SIZE*pirateShip[i].GetScaleFactor()) + (BULLET_SIZE*playerBullet[j].GetScaleFactor())))
				{
					hge->Effect_Play(mShipHitSnd) ;
					pirateShip[i].Deactivate() ;
					pirateShip[i].SetDestroyedCountdown() ;
					playerBullet[j].Deactivate() ;
					IncrementScore(SCORE_TYPE_HIGH) ;
				}
			}
		}
	}// end for
}// end - void LevelOne::CheckLevelCollisions


//---------------------------------------- Render --------------------------------------------
// Render the levels active objects and GUI elements
//--------------------------------------------------------------------------------------------
void LevelOne::Render(float frameTime)
{
	Level::Render(frameTime) ;

	for (int i = 0 ; i < MAX_SPACE_PIRATES ; i++)
	{
		pirateShip[i].Render(mLevelPosition.x, mLevelPosition.y) ;
	}
	// Render active Asteroids
	for (int j = 0 ; j < MAX_LARGE_ASTEROIDS ; j++)
	{
		largeAsteroid[j].Render((largeAsteroid[j].GetXPos()+mLevelPosition.x), (largeAsteroid[j].GetYPos()+mLevelPosition.y)) ;
	}
	for (int k = 0 ; k < MAX_SMALL_ASTEROIDS ; k++)
	{
		smallAsteroid[k].Render((smallAsteroid[k].GetXPos()+mLevelPosition.x), (smallAsteroid[k].GetYPos()+mLevelPosition.y)) ;
	}


	player.RenderHealthBox() ;

	int minutes = 0 ;
	int seconds = 0 ;
	minutes = mTime / 60 ;
	seconds = mTime - (minutes*60) ;

	textFont->SetColor(0xFFFFFFFF) ;
	textFont->SetScale(0.7f) ;
	textFont->printf(5.0f, 5.0f, HGETEXT_LEFT, "Level 1 - Clear the shipping route of Asteroids and Space Pirates.") ;
	textFont->printf(SCREEN_WIDTH/2, 5.0f, HGETEXT_LEFT, "Time: %d:%02d", minutes, seconds) ; 
	textFont->printf(SCREEN_WIDTH-120.0f, 5.0f, HGETEXT_LEFT, "Score: %d", mCurrentScore) ;


	// Render story event convo boxes
	if (!introCompleted)
	{
		RenderIntro(frameTime) ;
	}
	if (levelLost && !gameOverCompleted)
	{
		RenderGameOver(frameTime) ;
	}
	if (levelWon)
	{
		RenderOutro(frameTime) ;
	}
}


//------------------------------------- RenderIntro  -----------------------------------------
//--------------------------------------------------------------------------------------------
void LevelOne::RenderIntro(float frameTime)
{
	// Checks of the skipIntro bool has been triggered 
	// (triggers if game over is called so player does not have to view intro again)
	if (skipIntro)
	{
		if (convoCount == 0)
		{
			convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET, 
				"*ahem* ...Lets try that again.") ;
		}
		else
		{
			introCompleted	= true ;
			convoCount		= 0 ; // reset
		}
	}
	// Outwise plays intro as normal
	else if (convoCount == 0)
	{
		//playerShip.jumpin(originalPos, targetPos, frameTime) ;

		convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET, 
			"General, I have dropped out of FTL and arrived at the designated coordinates.") ;
			//"General, I have arrived at the designated coordinates.") ;
	}
	else if (convoCount == 1)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.GENERAL_OBVIOUS,
			"Good, we will be tracking your progress from HQ via a nearby relay. As per your mission instructions...") ;
	}
	else if (convoCount == 2)
	{
		convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET, 
			"Which I obviously read.... Sir.") ;
	}
	else if (convoCount == 3)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.GENERAL_OBVIOUS,
			"...we need you to clear out any space pirates and reduce the number of "
			"asteroids in this sector so the Alliance can open the shipping route. "
			"This should be an easy mission for you!") ;
	}
	else if (convoCount == 4)
	{
		convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET,
			"Do you know how many asteroids my sensors are picking up? "
			"I'm going to be torn into pieces.") ;
	}
	else if (convoCount == 5)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.MAJOR_PEDANTIC,
			"You'll be fine Captain. Your ship is tougher than the paper it was drawn on now "
			"that it has been made digital.") ;
	}
	else if (convoCount == 6)
	{
		convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET,
			"Lucky me... Remind me why I joined the Space Security Division again?") ;
	}
	else if (convoCount == 7)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.MAJOR_PEDANTIC,
			"Largely insignificant backstory the programmer couldnt be bothered to fill us "
			"in with. This IS just a short demo afterall.") ;
	}
	else if (convoCount == 8)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.GENERAL_OBVIOUS,
			"Exactly! This isn't a Metal Gear game Captain, your mission is time critical and we "
			"don't have time for all this chit chat. We have freight en route and it's getting "
			"closer by the second. Get on with it!") ;
	}
	else if (convoCount == 9)
	{
		convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET,
			"Sir, Yes Sir! ") ;
	}
	else if (convoCount == 10)
	{
		introCompleted = true ;
		convoCount = 0 ; // reset
	}
}


//------------------------------------- RenderOutro  -----------------------------------------
//--------------------------------------------------------------------------------------------
void LevelOne::RenderOutro(float frameTime)
{
	if (convoCount == 0)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.GENERAL_OBVIOUS, 
			"Hmm, our sensors arn't picking up any more activity. The route looks pretty clear. Guess its game over then!") ;
	}
	else if (convoCount == 1)
	{
		convoCount += convoBox.Render(frameTime, true, convoBox.CAP_SARC_HELMET, 
			"No wait! There might be a second level!") ;
	}
	else if (convoCount == 2)
	{
		outroCompleted = true ;
		convoCount = 0 ; // reset
		SaveHighScore() ;
	}
}


//------------------------------------ RenderGameOver ----------------------------------------
// Once game convo over is complete, triggers gameOverCompleted and skipIntro bool so
// invincibility is allowed and the intro dialogue is skipped after the players first death.
// Also saves the players high score.
//--------------------------------------------------------------------------------------------
void LevelOne::RenderGameOver(float frameTime)
{
	if (convoCount == 0)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.GENERAL_OBVIOUS,
			"Captain!? NOOOOOOOO!") ;
	}
	else if (convoCount == 1)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.MAJOR_PEDANTIC, 
			"General... The Captain will be fine once the level resets.") ;
	}
	else if (convoCount == 2)
	{
		convoCount += convoBox.Render(frameTime, false, convoBox.GENERAL_OBVIOUS, 
			"Well then... What are you wating for?!") ;
	}
	else if (convoCount == 3)
	{
		gameOverCompleted	= true ;
		skipIntro			= true ;
		convoCount = 0 ; // reset
		SaveHighScore() ;
	}
}


//----------------------------------------- Reset --------------------------------------------
// Resets the levels members to their default values. 
//--------------------------------------------------------------------------------------------
void LevelOne::Reset()
{

	mCurrentScore	= 0 ;
	mTime			= MAX_LEVEL_TIME ;


	introCompleted		= false ;	// Set true once intro to level has been completed
									// (Once intro completed, Start level)
	midPointReached		= false ;	// Set true once second part of level has been reached 
									// (Intro to smugger)
	midPointCompleted	= false ;	// Set true once second part of level has been completed
									// (Start smuggler chase)
	levelWon			= false ;	// Set true once the level win condition has been met
									// (Used to trigger the outro)
	levelLost			= false ;	// Set true if player reaches 0 health
									// (Used to trigger Game Over screen)
	gameOverCompleted	= false ;	// Set to true when GameOverConvo has finished
									// (Used to trigger level exit/hi-score screen after a game over)
	outroCompleted		= false ;	// Set true once outro to level has been completed
									// (Used to trigger next level start)

	// Level coords
	Level::mLevelPosition.x = 0.0f ;
	Level::mLevelPosition.y = 0.0f ;

	convoBox.Reset() ;
	convoCount = 0 ;


	mSoundChannel1 = hge->Stream_Play(mLevel1Music, true, 50) ;
	hge->Channel_Pause(mSoundChannel1) ;


	// Reset Player's Ship
	player.Reset(playerStartingXPos, playerStartingYPos, 0.0f) ;

	// Reset the Player's Bullets
	for (int i = 0 ; i < player.MAX_BULLETS ; i++)
	{
		playerBullet[i].Reset(player.GetXPos(), player.GetYPos(),
			player.GetXVel(), player.GetYVel(), player.GetRotation()) ;
	}

	// Reset Health pack pickup
	mHealthPickup.Reset() ;
	
	// Reset Space Pirates
	for (int i = 0 ; i < MAX_SPACE_PIRATES ; i++)
	{
		hgeVector randomPiratePos(0.0f, 0.0f) ;
		do
		{
			randomPiratePos.x = rand()%(LEVEL_X_SIZE - SCREEN_WIDTH) + (SCREEN_WIDTH/2) ;
			randomPiratePos.y = rand()%(LEVEL_Y_SIZE - SCREEN_HEIGHT) + (SCREEN_HEIGHT/2) ;
		}
		while (IsOnScreen(randomPiratePos.x, randomPiratePos.y)) ;
		pirateShip[i].SetPos(randomPiratePos.x, randomPiratePos.y) ;
		pirateShip[i].SetVel(hgeVector(0.0f, 0.0f)) ;
		pirateShip[i].Activate() ;
	}


	// Reset Asteroids 
	// Reset Large/small Asteroids
	for (int i = 0 ; i < MAX_LARGE_ASTEROIDS ; i++)
	{
		hgeVector randomPos(0.0f, 0.0f) ;
		bool onAnotherAsteroid  = false ;

		// Make sure asteroids dont spawn inside the screen bounds near the ship
		// Asteroids can still spawn on the edges of the screen.
		do
		{
			onAnotherAsteroid  = false ;
			randomPos.x = rand()%(LEVEL_X_SIZE - SCREEN_WIDTH) + (SCREEN_WIDTH/2) ;
			randomPos.y = rand()%(LEVEL_Y_SIZE - SCREEN_HEIGHT) + (SCREEN_HEIGHT/2) ;

			for (int j = 0 ; j < MAX_LARGE_ASTEROIDS ; j++)
			{
				if (largeAsteroid[j].CheckCollision(randomPos, ((LARGE_ASTEROID_SIZE*2)*largeAsteroid[j].GetScaleFactor())))
				{
					onAnotherAsteroid = true ;
				}
			}
		}
		while (IsOnScreen(randomPos.x, randomPos.y) || onAnotherAsteroid) ;

		largeAsteroid[i].Reset() ;
		largeAsteroid[i].SetPos(randomPos.x, randomPos.y) ;
	}// end for

	for (int k = 0 ; k < MAX_SMALL_ASTEROIDS ; k++)
	{
		smallAsteroid[k].Reset() ;
		smallAsteroid[k].Deactivate() ;
	}
}

//EOF LevelOne.cpp