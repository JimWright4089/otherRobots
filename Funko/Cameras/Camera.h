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
    Camera(std::string name);
    virtual ~Camera() {}

    void start();
    cv::Mat* getFrame();
    cv::Mat* getConfigFrame();
    void setRes(uint16_t h, uint16_t v);
    void setBox(uint16_t topX, uint16_t topY, uint16_t botX, uint16_t botY);

  protected:

    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    uint16_t mResH = 1920;
    uint16_t mResV = 1080;
    uint16_t mBoxTopX = 100;
    uint16_t mBoxTopY = 100;
    uint16_t mBoxBottomX = 400;
    uint16_t mBoxbottomY = 400;
    std::string mName = "unknown"

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
