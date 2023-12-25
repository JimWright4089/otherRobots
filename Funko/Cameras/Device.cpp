//----------------------------------------------------------------------------
//
//  $Workfile: propertyFile.cpp$
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
#include <boost/algorithm/string/replace.hpp>
#include "Device.h"

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
Device::Device()
{
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void Device::setModel(std::string model)
{
    mModel = model;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void Device::setSerialNumber(std::string serialNumber)
{
    mSerialNumber = serialNumber;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void Device::setDevice(std::string device)
{
    mDevice = device;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string Device::getModel(void)
{
    return mModel;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string Device::getSerialNumber(void)
{
    return mSerialNumber;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string Device::getDevice(void)
{
    return mDevice;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string Device::getFileName(void)
{
    std::string fileName = getModel()+"-"+getSerialNumber()+"-"+getDevice();
    boost::replace_all(fileName, "/", "-");
    return fileName;
}

