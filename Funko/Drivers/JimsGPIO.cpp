//----------------------------------------------------------------------------
//
//  $Workfile: propertyFile.h$
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

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <wiringPi.h>
#include "JimsGPIO.h"

const int16_t MAX_PINS = 32;
//                           0   1   2   3  4   5   6   7   8   9  
const int16_t PIN_MAP[] = { -1, -1, -1, -1, 7, -1, -1, 11, 10, 13, 
//                          10  11  12  13  14  15  16  17  18  19
                            12, 14, -1, -1, 15, 16, -1,  0,  1, -1,
//                          20  21  22  23  24  25  26  27  28  29
                            -1, -1,  3,  4,  5,  6, -1, -1, 17, 18,
//                          30  31
                            19, 20};

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
JimsInput::JimsInput()
{
  wiringPiSetup();  
  mPin = -1;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
JimsInput::JimsInput(int16_t pin)
{
  wiringPiSetup();  
  mPin = -1;
  if((pin>0)&&(pin<MAX_PINS))
  {
    mPin = PIN_MAP[pin];
    pinMode(mPin, INPUT) ;
  }
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
bool JimsInput::read()
{
  if(true == valid())
  {
    return (1 == digitalRead(mPin));
  }
  return false;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
bool JimsInput::valid()
{
  return (mPin != -1);
}

