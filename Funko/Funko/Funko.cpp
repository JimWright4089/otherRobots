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
#include <opencv2/opencv.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "Options.h"
#include "PropertyFile.h"
#include "JimsGPIO.h"
#include "JimsRobotclaw.h"
#include "EncoderSettingFile.h"
#include "PictureCountFile.h"
#include "DisplayDevices.h"
#include "AllCameras.h"

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

    if(true == gVerbose)
    {
        std::cout << "Centering Pop\n";
    }

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
int CAMS[] = {0,8,26,28,30};
int main(int argc, char* argv[])
{
    auto beginTime = std::chrono::high_resolution_clock::now();
    PropertyFile::getInstance()->loadFile("../Data/Funko.properties");
    int32_t encoder = 0;
    bool value = false;
    int32_t magnetSize = 0;
    int32_t halfMagnetsize = 0;
    int32_t diskSize = 0;
    double ticksPerDegree = 0;
    double degreesPerPicture = 0.0;
    uint16_t numberOfPictures = 0;
    EncoderSettingFile encoderFile(PropertyFile::getInstance()->getFullEncoderProp());
    PictureCountFile pictureCountFile(PropertyFile::getInstance()->getFullPictureFileCount());
    AllCameras allCameras;
    uint16_t startingFrameFile = 0;
    uint16_t curFrame=0;

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
        if(true == gVerbose)
        {
            std::cout << "Calibrate:\n";
        }

        encoderFile.calibrateEncoders(&gMagnet,&gMotor);
        return 0;
    }

    if(true == gDisplayCameras)
    {
        displayDevices();
        return 0;
    }

    if(true == gCalibrateCameras)
    {
        allCameras.calibrateCameras();
        return 0;
    }

    if(true == gTestFrames)
    {
        allCameras.testFrames();
        return 0;
    }

    encoderFile.loadFile();
    pictureCountFile.loadFile();

    centerOnMagnet(encoderFile.getHalfMagnetSize());

    for(curFrame=0;curFrame<gStartFrame;curFrame++)
    {
        if(true == gVerbose)
        {
            auto curTime = std::chrono::high_resolution_clock::now();
            std::chrono::milliseconds duration =  std::chrono::duration_cast<std::chrono::milliseconds>(curTime - beginTime);
            double theTime = duration.count();
            theTime /= 1000;
            theTime /= 60;
            int minutes = (int)theTime;
            theTime -= minutes;
            int seconds = 60*theTime;
            char timeString[30];
            double percent = ((double)curFrame/(double)numberOfPictures)*100.0;

            snprintf(timeString,30,"%d:%02d %6.2f",minutes,seconds,percent);

            std::cout << "Skipping Frame#" << curFrame << " ";
            std::cout << "Picture at ";
            std::cout << curFrame*degreesPerPicture << "° ";
            std::cout << gMotor.getRightEncoder() << " ";
            std::cout << (uint32_t)((encoderFile.getTicksPerDegree()*degreesPerPicture)+.5) << "  ";
            std::cout << timeString << "%\n";
        }

        for(int cam=0;cam<6;cam++)
        {
            std::string camera = "camera"+std::to_string(cam);
            pictureCountFile.addToFileNameCount(gPop,gLoc,camera);
        }

        turnDegrees((uint32_t)((encoderFile.getTicksPerDegree()*degreesPerPicture)+.5));
    }



    for(;curFrame<numberOfPictures;curFrame++)
    {
        if(true == gVerbose)
        {
            auto curTime = std::chrono::high_resolution_clock::now();
            std::chrono::milliseconds duration =  std::chrono::duration_cast<std::chrono::milliseconds>(curTime - beginTime);
            double theTime = duration.count();
            theTime /= 1000;
            theTime /= 60;
            int minutes = (int)theTime;
            theTime -= minutes;
            int seconds = 60*theTime;
            char timeString[30];
            double percent = ((double)curFrame/(double)numberOfPictures)*100.0;

            snprintf(timeString,30,"%d:%02d %6.2f",minutes,seconds,percent);

            std::cout << "Frame#" << curFrame << " ";
            std::cout << "Picture at ";
            std::cout << curFrame*degreesPerPicture << "° ";
            std::cout << gMotor.getRightEncoder() << " ";
            std::cout << (uint32_t)((encoderFile.getTicksPerDegree()*degreesPerPicture)+.5) << "  ";
            std::cout << timeString << "%\n";
        }
        allCameras.captureFrames(&pictureCountFile);
        turnDegrees((uint32_t)((encoderFile.getTicksPerDegree()*degreesPerPicture)+.5));
    }
    if(true == gVerbose)
    {
        std::cout << "Final ticks ";
        std::cout << gMotor.getRightEncoder() << " ";
        std::cout << (uint32_t)((encoderFile.getTicksPerDegree()*degreesPerPicture)+.5) << "\n";
    }
    pictureCountFile.saveFile();
    return 0;
}

