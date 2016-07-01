// Utility.cpp file
#include "Utility.h"


//----------------------------------- DisplayErrorBox  ---------------------------------------
// Creates an error box displaing the passed label and triggers the engine to release any
// assets/resources and shutdown
//--------------------------------------------------------------------------------------------
int DisplayErrorBox(char label[100])
{
	MessageBox(NULL, ("%s", label), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL) ;
	hge->System_Shutdown() ;
	hge->Release() ;
	return 0 ;
}


//----------------------------------- SynthesizeInput  ---------------------------------------
// Synthesize a key press. Created to act as a work around for HGE's menu system now allowing
// for changing/detection of key input
//--------------------------------------------------------------------------------------------
void SynthesizeInput(WORD requestedKey)//VK_RETURN
{
    INPUT inputEvent;

    // Set up a generic keyboard event.
    inputEvent.type = INPUT_KEYBOARD;
    inputEvent.ki.wScan = 0; // hardware scan code for key
    inputEvent.ki.time = 0;
    inputEvent.ki.dwExtraInfo = 0;

    // Press the "A" key
    inputEvent.ki.wVk = requestedKey ; // virtual-key code
    inputEvent.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &inputEvent, sizeof(INPUT));

    // Release the key
    inputEvent.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &inputEvent, sizeof(INPUT));
}


//----------------------------------- DistanceSquared  ---------------------------------------
// Returns the squared distance between two vectors
//--------------------------------------------------------------------------------------------
float DistanceSquared(hgeVector a, hgeVector b)
{
  float xSquared = a.x - b.x ;
  float ySquared = a.y - b.y ;

  return xSquared * xSquared + ySquared * ySquared ;
}


//----------------------------------- DistanceBetween  ---------------------------------------
// Returns the distance between two vectors
//--------------------------------------------------------------------------------------------
float DistanceBetween(hgeVector a, hgeVector b)
{
  float xDistance = a.x - b.x ;
  float yDistance = a.y - b.y ;

  return xDistance + yDistance ;
}

//EOF Utility.cpp