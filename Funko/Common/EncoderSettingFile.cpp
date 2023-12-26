//----------------------------------------------------------------------------
//
//  $Workfile: loadAddressFile.cpp$
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
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <algorithm>
#include <chrono>
#include <thread>
#include "EncoderSettingFile.h"
#include "Options.h"

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
EncoderSettingFile::EncoderSettingFile(std::string fileName)
{
    mFileName = fileName;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void EncoderSettingFile::loadFile()
{
    Json::Value status;
    std::ifstream ipFile(mFileName);
    Json::Reader reader;

    reader.parse(ipFile, status);

    mMagnetSize = status[MAGNET_SIZE].asInt();
    mHalfMagnetsize = status[HALF_MAGNET_SIZE].asInt();
    mDiskSize = status[DISK_SIZE].asInt();
    mTicksPerDegree = status[TICKS_PER_DEGREE].asDouble();

    if(true == gVerbose)
    {
        print();
    }
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void EncoderSettingFile::print()
{
    std::cout << "Encoder File:\n";
    std::cout << "  Disk Size:" << mDiskSize << "\n";
    std::cout << "  Magnet Size:" << mMagnetSize << "\n";
    std::cout << "  Magnet Half Size:" << mHalfMagnetsize << "\n";
    std::cout << "  Ticks Per Degree:" << mTicksPerDegree << "\n";
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
int32_t EncoderSettingFile::getMagnetSize()
{
    return mMagnetSize;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
int32_t EncoderSettingFile::getHalfMagnetSize()
{
    return mHalfMagnetsize;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
int32_t EncoderSettingFile::getDiskSize()
{
    return mDiskSize;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
double EncoderSettingFile::getTicksPerDegree()
{
    return mTicksPerDegree;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void EncoderSettingFile::calibrateEncoders(JimsInput* magnet, JimsRobotClaw* motor)
{
    bool value = false;;
    int32_t encoder = 0;
    double fastSpeed = 0.0;
    double normalSpeed = 0.0;
    uint16_t frameWaitTimeMs = 20;

    if(true == gVerbose)
    {
        std::cout << "Filename:" << mFileName << "\n";
    }

    fastSpeed = PropertyFile::getInstance()->getFastSpeed();
    normalSpeed = PropertyFile::getInstance()->getNormalSpeed();
    frameWaitTimeMs = PropertyFile::getInstance()->getFrameWaitTimeMs();

    if(false == magnet->valid())
    {
        std::cout << "Bad magnet setup\n";
    }

    ///////////////////////////////////////////////////////////////////////
    // If the disk is on the magnet move it off
    ///////////////////////////////////////////////////////////////////////
    value = magnet->read();
    motor->setRightMotor(fastSpeed*-1.0);
    while(false == value)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
        encoder = motor->getRightEncoder();
        value = magnet->read();

        if(true == gVerbose)
        {
            std::cout << "encoder:"<< encoder << " magnet:"  << value << "\n";
        }

    }
    std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs*25));

    if(true == gVerbose)
    {
        std::cout << "Moved off magnet\n";
    }

    ///////////////////////////////////////////////////////////////////////
    // Find The magnet
    ///////////////////////////////////////////////////////////////////////
    motor->setRightMotor(fastSpeed);
    value = magnet->read();
    while(true == value)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
        encoder = motor->getRightEncoder();
        value = magnet->read();
    }
    motor->setRightMotor(0);
    motor->setRightEncoder(0);

    if(true == gVerbose)
    {
        std::cout << "Found magnet\n";
    }

    ///////////////////////////////////////////////////////////////////////
    // Find The magnet's size
    ///////////////////////////////////////////////////////////////////////
    motor->setRightMotor(normalSpeed);
    while(false == value)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
        encoder = motor->getRightEncoder();
        value = magnet->read();
    }
    motor->setRightMotor(0);
    mMagnetSize = encoder;
    mHalfMagnetsize = mMagnetSize/2;

    if(true == gVerbose)
    {
        std::cout << "Found magnet size\n";
    }

    ///////////////////////////////////////////////////////////////////////
    // Find The disk's size
    ///////////////////////////////////////////////////////////////////////
    motor->setRightEncoder(0);
    motor->setRightMotor(normalSpeed);
    value = magnet->read();
    while(true == value)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
        encoder = motor->getRightEncoder();
        value = magnet->read();
    }
    mDiskSize = encoder;
    mTicksPerDegree = ((double)mDiskSize+(double)mMagnetSize)/360.0;

    if(true == gVerbose)
    {
        std::cout << "Found disk size\n";
    }

    ///////////////////////////////////////////////////////////////////////
    // Center on the magnet
    ///////////////////////////////////////////////////////////////////////
    motor->setRightEncoder(0);
    encoder = 0;
    motor->setRightMotor(normalSpeed);
    while(encoder < mHalfMagnetsize)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
        encoder = motor->getRightEncoder();
        value = magnet->read();
    }
    motor->setRightMotor(0);
    motor->setRightEncoder(0);

    if(true == gVerbose)
    {
        std::cout << "Centered\n";
    }

    Json::Value status;

    status[MAGNET_SIZE] = mMagnetSize;
    status[HALF_MAGNET_SIZE] = mHalfMagnetsize;
    status[DISK_SIZE] = mDiskSize;
    status[TICKS_PER_DEGREE] = mTicksPerDegree;

    std::ofstream magFile(mFileName);

    Json::StyledWriter styledWriter;
    magFile << styledWriter.write(status);
    magFile.close();
    print();
}