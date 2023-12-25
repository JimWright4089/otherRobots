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
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "PropertyFile.h"
#include "Options.h"
#include "AllCameras.h"

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
AllCameras::AllCameras()
{
    Json::Value cameraJson;
    std::ifstream ipFile(PropertyFile::getInstance()->getFullCamerasProp());
    Json::Reader reader;

    reader.parse(ipFile, cameraJson);

    for(int i=0;i<MAX_CAMERA;i++)
    {
        std::string cameraString = "camera"+ std::to_string(i);
        Json::Value camera = cameraJson[cameraString];
        Camera* newCamera = new Camera(
            camera[MODEL].asString(),
            camera[SERIAL_NUMBER].asString(),
            camera[DEVICE].asString(),
            camera[READ_FRAMES].asInt());
        newCamera->setBox(camera[TOP_X].asInt(),camera[TOP_Y].asInt());
        mCameras.push_back(newCamera);
    }
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
cv::Mat AllCameras::getFrame(uint16_t cameraID)
{
    cv::Mat frame(640, 320, CV_8UC3, cv::Scalar(140, 100, 15));

    if(cameraID < mCameras.size())
    {
        return mCameras[cameraID]->getFrame();
    }
    return frame;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
cv::Mat AllCameras::getCalibrationFrame(uint16_t cameraID)
{
    cv::Mat frame(640, 320, CV_8UC3, cv::Scalar(140, 100, 15));

    if(cameraID < mCameras.size())
    {
        return mCameras[cameraID]->getCalibrationFrame();
    }
    return frame;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void AllCameras::calibrateCameras(void)
{

    for(int i=0;i<mCameras.size();i++)
    {
        cv::Mat frame = mCameras[i]->getCalibrationFrame();
     
        std::string frameFileName = PropertyFile::getInstance()->getPicturesDir();

        if(false == boost::filesystem::is_directory(frameFileName))
        {
            boost::filesystem::create_directory(frameFileName);
        }

        frameFileName += PropertyFile::getInstance()->getCalibrationDir();

        if(false == boost::filesystem::is_directory(frameFileName))
        {
            boost::filesystem::create_directory(frameFileName);
        }

        frameFileName += "camera"+std::to_string(i)+".jpg";
        std::cout << frameFileName << "\n";
        cv::imwrite(frameFileName, frame);
    }
}
