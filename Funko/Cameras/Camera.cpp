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
#include "PropertyFile.h"
#include "Camera.h"

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
Camera::Camera()
{
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
Camera::Camera(std::string model, std::string serialNumber, std::string device, uint16_t readFrames) :
    mModel(model),
    mSerialNumber(serialNumber),
    mDevice(device),
    mReadFrames(readFrames)
{

}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void Camera::start()
{
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
cv::Mat Camera::getFrame()
{
    cv::Mat frame(640, 320, CV_8UC3, cv::Scalar(140, 100, 15));
    cv::Mat frameReturn(640, 320, CV_8UC3, cv::Scalar(140, 100, 15));
    cv::VideoCapture capture;
    capture.open(mDevice);

    for(int j=0;j<mReadFrames;j++)
    {
        capture >> frame;
    }
    if (!frame.empty())
    {
      cv::Rect rect(    mBoxTopX, 
                        mBoxTopY, 
                        PropertyFile::getInstance()->getImageBoxWidth(),
                        PropertyFile::getInstance()->getImageBoxHeight());

        frameReturn = frame(rect);
    }
    capture.release();

    return frameReturn;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
cv::Mat Camera::getCalibrationFrame()
{
    cv::Mat frame(640, 320, CV_8UC3, cv::Scalar(140, 100, 15));
    cv::VideoCapture capture;
    capture.open(mDevice);

    for(int j=0;j<mReadFrames;j++)
    {
        capture >> frame;
    }
    if (!frame.empty())
    {
      cv::Rect rect(    mBoxTopX, 
                        mBoxTopY, 
                        PropertyFile::getInstance()->getImageBoxWidth(),
                        PropertyFile::getInstance()->getImageBoxHeight());
      cv::rectangle(frame, rect, cv::Scalar(0, 0, 255));
    }
    capture.release();

    return frame;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void Camera::setBox(uint16_t topX, uint16_t topY)
{
    mBoxTopX = topX;
    mBoxTopY = topY;
}
