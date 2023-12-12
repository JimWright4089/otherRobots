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
#ifndef LOAD_ADDRESS_FILE_H
#define LOAD_ADDRESS_FILE_H

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
class LoadAddresses
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    LoadAddresses();
    LoadAddresses(std::string fileName);
    virtual ~LoadAddresses() {}
    void loadFile(std::string fileName);
    void print();
    void addAddress(std::string address);
    std::vector<std::string> getAddresses();
    void removeAddress(std::string address);
    bool isEmpty();

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mFileName = "";
    std::vector<std::string> mLoadAddresses;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
