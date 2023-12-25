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
#ifndef DEVICES_H
#define DEVICES_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>

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
class Device
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    Device();

    void setModel(std::string model);
    void setSerialNumber(std::string serialNumber);
    void setDevice(std::string device);
    std::string getModel(void);
    std::string getSerialNumber(void);
    std::string getDevice(void);
    std::string getFileName(void);

  protected:

    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mModel = "unknown";
    std::string mSerialNumber = "unknown";
    std::string mDevice = "unknown";

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
