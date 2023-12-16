//----------------------------------------------------------------------------
//
//  $Workfile: ipAddressFile.h$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for reading ip address files
//
//----------------------------------------------------------------------------
#ifndef ENCODER_SETTING_FILE_H
#define ENCODER_SETTING_FILE_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <stdint.h>
#include <string>
#include <vector>
#include <json/json.h>
#include "PropertyFile.h"
#include "JimsGPIO.h"
#include "JimsRobotclaw.h"


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
class EncoderSettingFile
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    EncoderSettingFile(std::string fileName);
    virtual ~EncoderSettingFile() {}
    void loadFile();
    void print();
    int32_t getMagnetSize();
    int32_t getHalfMagnetSize();
    int32_t getDiskSize();
    double getTicksPerDegree();
    void calibrateEncoders(PropertyFile* propFile, JimsInput* magnet, JimsRobotClaw* motor);

  private:

    const std::string MAGNET_SIZE = "magnetSize";
    const std::string HALF_MAGNET_SIZE = "halfMagnetSize";
    const std::string DISK_SIZE = "diskSize";
    const std::string TICKS_PER_DEGREE = "ticksPerDegree";

    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mFileName = "";
    int32_t mMagnetSize = 0;
    int32_t mHalfMagnetsize = 0;
    int32_t mDiskSize = 0;
    double mTicksPerDegree = 0;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
