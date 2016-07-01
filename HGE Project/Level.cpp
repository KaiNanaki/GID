// Level.cpp file
#include "Level.h"


//----------------------------------------- Ctor ---------------------------------------------
//--------------------------------------------------------------------------------------------
void Level::Initialise(float playerXPos, float playerYPos)
{
	mHighScore		= 0 ;
	mCurrentScore	= 0 ;

	/////////////////////////////////////////////////////////////////

	// Level stage triggers
	introCompleted		= false ;	// Set true once intro to level has been completed
									// (Once intro completed, Start level)
	levelWon			= false ;	// Set true once the level win condition has been met
									// (Used to trigger the outro)
	levelLost			= false ;	// Set true if player reaches 0 health
									// (Used to trigger Game Over convo)
	gameOverCompleted	= false ;	// Set to true when GameOverConvo has finished
									// (Used to trigger level exit/hi-score screen after a game over)
	outroCompleted		= false ;	// Set true once outro to level has been completed
									// (Used to trigger next level start)
	allowInvincibility	= false ;
	skipIntro			= false ;

	// Level coords
	Level::mLevelPosition.x = 0.0f ;
	Level::mLevelPosition.y = 0.0f ;

	convoBox.Initialise() ;

	// Initialise Player's Ship and flag an error if anything goes wrong
	player.Initialise(playerXPos, playerYPos, 0.0f) ;

	// Initialise the Player's Bullets and flag an error if anything goes wrong
	for (int i = 0 ; i < player.MAX_BULLETS ; i++)
	{
		// Flag an error if anything goes wrong
		playerBullet[i].Initialise(player.GetXPos(), player.GetYPos(),
			player.GetXVel(), player.GetYVel(), player.GetRotation()) ;
	}

	// Initialise the levels Health Pack pickup
	mHealthPickup.Initialise(0.0f, 0.0f) ;
}


//----------------------------------------- Dtor ---------------------------------------------
//--------------------------------------------------------------------------------------------
Level::~Level()
{
	// Free loaded resources
	hge->Target_Free(levelTex) ;

	// Free created pointers
	levelSpr	= NULL;
	delete		levelSpr ;
}


//---------------------------------------- Update --------------------------------------------
//--------------------------------------------------------------------------------------------
void Level::Update(float frameTime)
{
	// Update the level position
	// (Moves the level background sprite in relation to the Player's Ship position)
	mLevelPosition.x = -(player.GetXPos() - (SCREEN_WIDTH/2)) ;
	mLevelPosition.y = -(player.GetYPos() - (SCREEN_HEIGHT/2)) ;

}


//-------------------------------- UpdateBaseGameObjects  ------------------------------------
// Updates the levels base game objects (player ship, player bullets, health pickups)
//--------------------------------------------------------------------------------------------
void Level::UpdateBaseGameObjects(float frameTime, XInputController *controller)
{
	// Update Player's ship position within the level
	player.Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE, allowInvincibility, controller) ;

	static bool bulletFound = false;
	// Player has attempted to fire a bullet
	if (player.GetFiredThisFrame())
	{
		// Loop through bullets to find any free (inactive) bullets
		for (int i = 0 ; i < player.MAX_BULLETS ; i++)
		{
			// If an inactive bullet is found (and a bullet was not previously found) reset 
			// its values, and then activate it so it will be fired/updated
			if (!playerBullet[i].GetIsActive() && !bulletFound)
			{
				playerBullet[i].Reset(player.GetXPos(), player.GetYPos(),
					player.GetXVel(), player.GetYVel(), player.GetRotation()) ;
				playerBullet[i].Activate();
				// Break from the loop when a bullet has been fired so that all bullets are not
				// fired in one frame.
				break ;
			}
		}
	}// end if - fired this frame


	// Update an active bullet's positions on screen/deactivate them if they go off screen
	for (int j = 0 ; j < player.MAX_BULLETS ; j++)
	{
		if (playerBullet[j].GetIsActive())
		{
			// Update active bullets
			playerBullet[j].Update(frameTime, LEVEL_X_SIZE, LEVEL_Y_SIZE) ;

			// Deactivate bullets that go off screen
			if (!IsOnScreen(playerBullet[j].GetXPos(), playerBullet[j].GetYPos()))
			{
  				playerBullet[j].Deactivate() ;
			}
		}
	}// end for - bullet positions

	if(mHealthPickup.CheckForPickup(frameTime, player.GetPos(), (float)PLAYER_SHIP_SIZE))
	{
		player.IncrementHealth() ;
	}

}


//---------------------------------------- Render --------------------------------------------
// Renders the base level objects - The level itself, any health pickups, the player's 
// bullets and finally the player's ship
//--------------------------------------------------------------------------------------------
void Level::Render(float frameTime)
{
	// Render the level's background
	levelSpr->RenderEx(mLevelPosition.x, mLevelPosition.y, 0, 1, 1) ;

	// Render the health pickup
	mHealthPickup.Render(mLevelPosition) ;

	// Render active Bullets
	for (int i = 0 ; i < player.MAX_BULLETS ; i++)
	{
		playerBullet[i].Render(playerBullet[i].GetXPos()+mLevelPosition.x, playerBullet[i].GetYPos()+mLevelPosition.y) ;
	}

	// Render Player's Ship
	player.Render(frameTime) ;//, SCREEN_WIDTH/2, SCREEN_HEIGHT/2) ;

}


//------------------------------------ IncrementScore ----------------------------------------
// Uses the 'SCORE_TYPE' const int's within the Level Class to determine how much should be 
// added to the current score
//--------------------------------------------------------------------------------------------
void Level::IncrementScore(int scoreType)
{
 	switch (scoreType)
	{
		case SCORE_TYPE_SMALL:
			mCurrentScore += SCORE_TYPE_SMALL ;
			break ;
		case SCORE_TYPE_MEDIUM:
			mCurrentScore += SCORE_TYPE_MEDIUM ;
			break ;
		case SCORE_TYPE_HIGH:
			mCurrentScore += SCORE_TYPE_HIGH ;
			break ;
	}
}


//------------------------------------- UpdateScore  -----------------------------------------
// The players score is deremented by 1 every 0.2 seconds
//--------------------------------------------------------------------------------------------
void Level::UpdateScore(float frameTime)
{
	// Only update score if it is greater than 0 ;
	if (mCurrentScore > 0)
	{
		static double time = 0.0 ; // Used to count to a given time
		// Add to time until it is greater than 0.2 seconds
		if (time < 0.2)
		{
			time += frameTime ;
		}
		// Once the set time is reached, remove a point from the players score and reset time to 0
		else
		{
			mCurrentScore-- ;
			time = 0.0 ;
		}
	}
}


//------------------------------------ SaveHighScore  ----------------------------------------
// Saves the high score before resetting the level variables at gameover/completion of level
// Do not use to save high score if level is restarted or when quitting out to the main menu
//--------------------------------------------------------------------------------------------
void Level::SaveHighScore()
{
	if (mCurrentScore > mHighScore)
	{
		mHighScore = mCurrentScore ;
	}
}


//-------------------------------------- IsOnScreen ------------------------------------------
// Checks if a given objects position values fall inside the players viewable screen 
// boundaries and returns a bool
//--------------------------------------------------------------------------------------------
bool Level::IsOnScreen(float xPos, float yPos)
{
	if (xPos > player.GetXPos()-(SCREEN_WIDTH/2)-128 
		&& xPos < player.GetXPos()+(SCREEN_WIDTH/2)+128
		&& yPos > player.GetYPos()-(SCREEN_HEIGHT/2)-128 
		&& yPos < player.GetYPos()+(SCREEN_HEIGHT/2)+128)
	{
		return true ;		
	}
	else
	{
 		return false ;
	}
}


//----------------------------------- Accessor Methods ---------------------------------------
//--------------------------------------------------------------------------------------------
float Level::GetXPos()
{
	return Level::mLevelPosition.x ;
}

float Level::GetYPos()
{
	return Level::mLevelPosition.y ;
}

bool Level::CheckLevelCompleted()
{
	return outroCompleted ;
}

bool Level::CheckGameOver()
{
	return gameOverCompleted ;
}

int Level::GetCurrentScore()
{
	return mCurrentScore ;
}

int Level::GetHighScore()
{
	return mHighScore ;
}
//EOF Level.cpp