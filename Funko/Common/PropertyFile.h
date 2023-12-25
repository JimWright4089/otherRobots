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
#ifndef PROPERTY_FILES_H
#define PROPERTY_FILES_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>
#include <boost/property_tree/ptree.hpp>

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
class PropertyFile
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    static PropertyFile* getInstance();

    void loadFile(std::string fileName);

    void printTree();

    double getSlowSpeed();
    uint16_t getFrameWaitTimeMs();
    int16_t getDegreeOffSet();
    int16_t getImageBoxWidth();
    int16_t getImageBoxHeight();
    double getFastSpeed();
    double getNormalSpeed();
    double getDegreesPerPicture();
    std::string getDataRoot();
    std::string getCamerasProp();
    std::string getEncoderProp();
    std::string getPicturesDir();
    std::string getDisplayDir();
    std::string getCalibrationDir();
    std::string getDisplayProp();
    std::string getPictureFileCount();
    std::string getFullCamerasProp();
    std::string getFullEncoderProp();
    std::string getFullPicturesDir();
    std::string getFullPictureFileCount();
    std::string getFullDisplayDir();
    std::string getFullDisplayProp();
    std::string getFullCalibrationDir();

  private:
    PropertyFile();
    virtual ~PropertyFile() {}
    std::string getString(std::string entry);
    int getInt(std::string entry);
    double getDouble(std::string entry);

    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mFileName = "";
    boost::property_tree::ptree mProperyTree;
    static PropertyFile* mInstance;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    void printTree(boost::property_tree::ptree &propertyTree, int level);
};
#endif
