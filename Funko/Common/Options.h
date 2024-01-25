//----------------------------------------------------------------------------
//
//  $Workfile: Options.h$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for reading property files
//
//----------------------------------------------------------------------------
#ifndef OPIONS_H
#define OPIONS_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>

//----------------------------------------------------------------------------
//  Externs
//----------------------------------------------------------------------------
extern bool gJustHelp;
extern bool gVerbose;
extern bool gCalibrate;
extern bool gDisplayCameras;
extern bool gCalibrateCameras;
extern bool gTestFrames;
extern uint16_t gStartFrame;
extern std::string gPop;
extern std::string gLoc;

//----------------------------------------------------------------------------
//  Functions
//----------------------------------------------------------------------------
bool setOptionFlags(int argc, char* argv[]);

#endif
