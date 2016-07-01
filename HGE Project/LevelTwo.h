// Level Class header
#ifndef LEVEL_TWO
#define LEVEL_TWO
//--------------------------------------------------------------------------------------------
//
//  Name:	LevelTwo.h
//
//  Desc:	Updates the second level's game objects and renders them along with the 
//			intro/outro dialogues
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------

#include <hge.h>			// Include HGE engine header
#include <hgesprite.h>		// Include HGE texture header

#include "level.h"

extern HGE *hge ;			// External pointer to the HGE interface.


class LevelTwo: public Level
{
public:


	void Initialise() ;
	~LevelTwo() ;

	// Update the levels different stages(intro, main, outro)
	void Update(float frameTime, XInputController *controller) ;
	void UpdateGameObjects(float frameTime, XInputController *controller) ;


	// Draw level
	void Render(float frameTime) ;
	void RenderIntro(float frameTime) ;
	void RenderOutro(float frameTime) ;


private:


}; 
#endif 
//EOF LevelTwo.h