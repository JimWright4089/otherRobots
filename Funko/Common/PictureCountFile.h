//----------------------------------------------------------------------------
//
//  $Workfile: ipAddressFile.h$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for reading ip address files
//
//----------------------------------------------------------------------------
#ifndef PICTURE_COUNT_FILE_H
#define PICTURE_COUNT_FILE_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <stdint.h>
#include <string>
#include <vector>
#include <json/json.h>

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
class PictureCountFile
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    PictureCountFile(std::string fileName);
    virtual ~PictureCountFile() {}
    void loadFile();
    void saveFile();
    void print();
    std::string getFileName(std::string pop, std::string location, std::string camera);
    void addToFileNameCount(std::string pop, std::string location, std::string camera);

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    Json::Value mCounts;
    std::string mFileName;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
