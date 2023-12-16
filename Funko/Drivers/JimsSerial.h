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
#ifndef Jims_Serial_h
#define Jims_Serial_h

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>
#include <stdint.h>

const int WAIT_TIME = 1;

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
class JimsSerial {
public:
  JimsSerial(std::string portName);
  JimsSerial(std::string portName, uint32_t baud);
  void end(void);

  bool flush();
  bool readBuffer(uint8_t *buffer, uint16_t len);
  bool writeBuffer(const uint8_t *buffer, uint16_t len);
  bool writeThenRead(const uint8_t *write_buffer, uint16_t write_len,
                       uint8_t *read_buffer, uint16_t read_len);
  bool setBaud(uint32_t baud);

private:

  void openPort();

  std::string mPortName;
  int mPort = 0;
  uint32_t mBaud = 115200;
  bool mBegun;
  uint16_t mMaxBufferSize;
  bool _read(uint8_t *buffer, uint16_t len, bool stop);
};
#endif