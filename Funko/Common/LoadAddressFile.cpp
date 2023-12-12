//----------------------------------------------------------------------------
//
//  $Workfile: loadAddressFile.cpp$
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
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <algorithm>
#include "loadAddressFile.h"

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
LoadAddresses::LoadAddresses()
{

}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
LoadAddresses::LoadAddresses(std::string fileName)
{
    loadFile(fileName);
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void LoadAddresses::loadFile(std::string fileName)
{
    mFileName = fileName;

    Json::Value status;
    std::ifstream ipFile(fileName);
    Json::Reader reader;

    reader.parse(ipFile, status);

    for (auto const& id : status) 
    {
        mLoadAddresses.push_back(id.asString());
    }
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void LoadAddresses::print()
{
    for (const auto& key : mLoadAddresses)
        std::cout << key << "\n";    
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::vector<std::string> LoadAddresses::getAddresses()
{
    return mLoadAddresses;
}    

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void LoadAddresses::addAddress(std::string address)
{
    mLoadAddresses.push_back(address);
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void LoadAddresses::removeAddress(std::string address)
{
    mLoadAddresses.erase(std::remove(mLoadAddresses.begin(), mLoadAddresses.end(), address), mLoadAddresses.end());
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
bool LoadAddresses::isEmpty()
{
    return mLoadAddresses.size()==0;
}
