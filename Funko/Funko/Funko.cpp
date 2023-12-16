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
#include "Options.h"
#include "PropertyFile.h"
#include "JimsGPIO.h"
#include "JimsRobotclaw.h"
#include "EncoderSettingFile.h"
#include <iostream>
#include <chrono>
#include <thread>

//----------------------------------------------------------------------------
//  Global
//----------------------------------------------------------------------------
JimsInput gMagnet(17);
JimsRobotClaw gMotor("/dev/robotclaw",  38400);

void turnDegrees(int32_t ticks)
{
    int32_t encoder = gMotor.getRightEncoder();

    double slowSpeed = PropertyFile::getInstance()->getSlowSpeed();
    uint16_t frameWaitTimeMs = PropertyFile::getInstance()->getFrameWaitTimeMs();
    int degreeOffset = PropertyFile::getInstance()->getDegreeOffSet();

    int32_t goalTicks = encoder + ticks - degreeOffset;
    gMotor.setRightMotor(slowSpeed);
    while(encoder < goalTicks)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
        encoder = gMotor.getRightEncoder();
        if(true == gVerbose)
        {
            std::cout << encoder << "\n";
        }
    }
    gMotor.setRightMotor(0);

}

void centerOnMagnet(int32_t mHalfMagnetsize)
{
    bool value = false;
    int32_t encoder = 0;
    double fastSpeed = 0.0;
    double normalSpeed = 0.0;
    uint16_t frameWaitTimeMs = 20;

    fastSpeed = PropertyFile::getInstance()->getFastSpeed();
    normalSpeed = PropertyFile::getInstance()->getNormalSpeed();
    frameWaitTimeMs = PropertyFile::getInstance()->getFrameWaitTimeMs();

    if(false == gMagnet.valid())
    {
        std::cout << "Bad magnet setup\n";
    }

    ///////////////////////////////////////////////////////////////////////
    // If the disk is on the magnet move it off
    ///////////////////////////////////////////////////////////////////////
    value = gMagnet.read();
    gMotor.setRightMotor(fastSpeed*-1.0);
    while(false == value)
    {
        encoder = gMotor.getRightEncoder();
        value = gMagnet.read();
        if(true == gVerbose)
        {
            std::cout << value << " "<< encoder << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs*25));

    ///////////////////////////////////////////////////////////////////////
    // Find The magnet
    ///////////////////////////////////////////////////////////////////////
    gMotor.setRightMotor(fastSpeed);
    value = gMagnet.read();
    while(true == value)
    {
        encoder = gMotor.getRightEncoder();
        value = gMagnet.read();
        if(true == gVerbose)
        {
            std::cout << value << " "<< encoder << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
    }
    gMotor.setRightMotor(0);
    gMotor.setRightEncoder(0);

    ///////////////////////////////////////////////////////////////////////
    // Center on the magnet
    ///////////////////////////////////////////////////////////////////////
    gMotor.setRightEncoder(0);
    encoder = 0;
    gMotor.setRightMotor(normalSpeed);
    while(encoder < mHalfMagnetsize)
    {
        encoder = gMotor.getRightEncoder();
        value = gMagnet.read();
        if(true == gVerbose)
        {
            std::cout << value << " "<< encoder << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(frameWaitTimeMs));
    }
    gMotor.setRightMotor(0);
    gMotor.setRightEncoder(0);
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
int main(int argc, char* argv[])
{
    PropertyFile::getInstance()->loadFile("../Data/Funko.properties");
    int32_t encoder = 0;
    bool value = false;
    int32_t magnetSize = 0;
    int32_t halfMagnetsize = 0;
    int32_t diskSize = 0;
    double ticksPerDegree = 0;
    double degreesPerPicture = 0.0;
    uint16_t numberOfPictures = 0;
    EncoderSettingFile encoderFile("../Data/Encoder.json");

    if(false == setOptionFlags(argc, argv))
    {
        return -1;
    }

    if(true == gVerbose)
    {
        std::cout << "\nProperties:\n";
        PropertyFile::getInstance()->printTree();
    }

    degreesPerPicture = PropertyFile::getInstance()->getDegreesPerPicture();
    numberOfPictures = (uint32_t)360/degreesPerPicture;

    if(true == gCalibrate)
    {
        encoderFile.calibrateEncoders(&gMagnet,&gMotor);
        return 0;
    }

    encoderFile.loadFile();

    if(true == gVerbose)
    {
        encoderFile.print();
    }

    centerOnMagnet(encoderFile.getHalfMagnetSize());

    for(int i=0;i<numberOfPictures;i++)
    {
        std::cout << "Picture at ";
        std::cout << i*degreesPerPicture << " ";
        std::cout << gMotor.getRightEncoder() << " ";
        std::cout << (uint32_t)((encoderFile.getTicksPerDegree()*degreesPerPicture)+.5) << "\n";
        turnDegrees((uint32_t)((encoderFile.getTicksPerDegree()*degreesPerPicture)+.5));
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    return 0;
}

