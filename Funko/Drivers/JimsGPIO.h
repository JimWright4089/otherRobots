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
#ifndef Jims_GPIO_h
#define Jims_GPIO_h

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <stdint.h>

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: Connection
// 
// Purpose:
//      The storage location for the SQL Connection
//
//----------------------------------------------------------------------------
class JimsInput {
public:
  JimsInput();
  JimsInput(int16_t pin);

  bool read();
  bool valid();

private:
  int16_t mPin = 0;
};
#endif