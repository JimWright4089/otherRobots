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
#include "JimsSerial.h"
#include <fcntl.h> // Contains file controls like O_RDWR
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>

//#define SERIAL_DEBUG 1

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
JimsSerial::JimsSerial(std::string portName) :
 mPortName(portName)
{
  openPort();
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
JimsSerial::JimsSerial(std::string portName, uint32_t baud) :
  mPortName(portName),
  mBaud(baud)
{
  openPort();
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void JimsSerial::openPort()
{
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void JimsSerial::end(void)
{
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
bool JimsSerial::flush()
{
  return true;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
bool JimsSerial::readBuffer(uint8_t *buffer, uint16_t len)
{
  size_t received = 0;
  return received;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
bool JimsSerial::writeBuffer(const uint8_t *buffer, uint16_t len)
{
  return true;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
bool JimsSerial::writeThenRead(const uint8_t *write_buffer, uint16_t write_len,
                      uint8_t *read_buffer, uint16_t read_len)
{
  bool results = writeBuffer(write_buffer, write_len);
  std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
  if(true == results)
  {
    results = readBuffer(read_buffer,read_len);
  }
  return results;
}