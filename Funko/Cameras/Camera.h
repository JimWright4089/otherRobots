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
#ifndef CAMERA_H
#define CAMERA_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>
#include <opencv2/opencv.hpp>

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
class Camera
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    Camera();
    Camera(std::string model, std::string serialNumber, std::string device, uint16_t readFrames);
    virtual ~Camera() {}

    void start();
    cv::Mat getFrame();
    cv::Mat getCalibrationFrame();
    void setBox(uint16_t topX, uint16_t topY);

  protected:

    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    uint16_t mBoxTopX = 0;
    uint16_t mBoxTopY = 0;
    std::string mModel = "unknown";
    std::string mSerialNumber = "unknown";
    std::string mDevice = "unknown";
    uint16_t mReadFrames = 1;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
