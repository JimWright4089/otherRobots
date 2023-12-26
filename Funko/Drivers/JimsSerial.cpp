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
  mPort = open(mPortName.c_str(), O_RDWR );
  if (mPort == -1)
  {
    perror(mPortName.c_str());
    return;
  }

  // Flush away any bytes previously read or written.
  int result = tcflush(mPort, TCIOFLUSH);
  if (result)
  {
    std::cout << "tcflush failed\n";  // just a warning, not a fatal error
  }

  // Get the current configuration of the serial port.
  struct termios options;
  result = tcgetattr(mPort, &options);
  if (result)
  {
    std::cout << "tcgetattr failed\n";
    close(mPort);
    return;
  }

  options.c_cflag=CS8|CLOCAL|CREAD;
  options.c_iflag=IGNPAR;
  options.c_oflag=0;
  options.c_lflag=0;
  options.c_cc[VMIN]=0;
  options.c_cc[VTIME]=1;
  options.c_lflag &= ~ICANON;
  options.c_iflag &= ~(IXON | IXOFF | IXANY);

  cfsetispeed(&options, mBaud);
  cfsetospeed(&options, mBaud); 

  result = tcsetattr(mPort, TCSANOW, &options);
  if (result)
  {
    std::cout << "tcsetattr failed\n";
    close(mPort);
    return;
  }  
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
  close(mPort);  
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
  return(tcflush(mPort, TCIOFLUSH));
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
  while (received < len)
  {
    ssize_t r = read(mPort, buffer + received, len - received);
    if (r < 0)
    {
      std::cout << "failed to read from port\n";
      return -1;
    }
    if (r == 0)
    {
      // Timeout
      break;
    }
    received += r;
  }

#ifdef SERIAL_DEBUG
  printf("Read %d %ld:",len,received);
  for(int i=0;i<received;i++)
  {
    printf("%2x ",buffer[i]);
  }
  printf("\n");
#endif 

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
#ifdef SERIAL_DEBUG
  printf("Write %d:",len);
  for(int i=0;i<len;i++)
  {
    printf("%2x ",buffer[i]);
  }
  printf("\n");
#endif

	size_t sent = write(mPort, buffer, len);
  std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));

  return sent == len;
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