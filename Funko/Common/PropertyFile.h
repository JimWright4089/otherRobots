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
    PropertyFile();
    PropertyFile(std::string fileName);
    virtual ~PropertyFile() {}
    void loadFile(std::string fileName);

    std::string getString(std::string entry);
    int getInt(std::string entry);
    void printTree();

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mFileName = "";
    boost::property_tree::ptree mProperyTree;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    void printTree(boost::property_tree::ptree &propertyTree, int level);
};
#endif
