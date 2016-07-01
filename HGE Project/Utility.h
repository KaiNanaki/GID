// Utility header
#ifndef UTILITY
#define UTILITY
//--------------------------------------------------------------------------------------------
//
//  Name:	Utility.h
//
//  Desc:	Contain miscellaneous stand alone functions of various uses that cna be used by
//			any class.
//
//  Author:	Kris Stevenson 2016
//
//--------------------------------------------------------------------------------------------


#include <iostream>

#include "hge.h"
#include "hgevector.h"

extern HGE *hge ;			// External pointer to the HGE interface.

// Used to display error messages (EG, If something does not do not initialise properly)
// and trigger the engine to shutdown.
int DisplayErrorBox(char label[100]) ;

// Used as a work around - synthesizes a keypress for the HGE menus from an XInput controller.
void SynthesizeInput(WORD key) ;

float DistanceSquared(hgeVector a, hgeVector b) ;
float DistanceBetween(hgeVector a, hgeVector b) ;


#endif 
//EOF Utility.h