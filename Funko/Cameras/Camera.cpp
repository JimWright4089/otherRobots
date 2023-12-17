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
Camera::Camera(std::string name) :
    mName(name)
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
cv::Mat* Camera::getFrame()
{
    return new  cv::Mat(640, 480, cv::CV_8UC3, Scalar(10, 100, 150));
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
cv::Mat* Camera::getCalibrationFrame()
{
    return new  cv::Mat(640, 480, cv::CV_8UC3, Scalar(10, 100, 150));
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void Camera::setRes(uint16_t h, uint16_t v)
{
    uint16_t mResH = h;
    uint16_t mResV = v;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void Camera::setBox(uint16_t topX, uint16_t topY, uint16_t botX, uint16_t botY)
{
    mBoxTopX = topX;
    mBoxTopY = topY;
    mBoxBottomX = botX;
    mBoxbottomY = botY;
}
