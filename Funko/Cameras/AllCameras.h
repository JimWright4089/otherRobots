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
#ifndef ALL_CAMERAS_H
#define ALL_CAMERAS_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <vector>
#include <opencv2/opencv.hpp>
#include "Camera.h"
#include "PictureCountFile.h"

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
class AllCameras
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    AllCameras();
    virtual ~AllCameras() {}
    cv::Mat getFrame(uint16_t cameraID);
    cv::Mat getCalibrationFrame(uint16_t cameraID);
    void calibrateCameras(void);
    void testFrames(void);
    void captureFrames(PictureCountFile* countFile);

  private:
    const std::string DEVICE = "device";
    const std::string MODEL = "model";
    const std::string SERIAL_NUMBER = "serialNumber";
    const std::string READ_FRAMES = "readFrames";
    const std::string TOP_X = "topX";
    const std::string TOP_Y = "topY";

    const uint16_t MAX_CAMERA = 6;

    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::vector<Camera*> mCameras;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
