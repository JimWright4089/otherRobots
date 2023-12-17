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
#include <chrono>
#include <thread>
#include "PictureCountFile.h"
#include "Options.h"

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
PictureCountFile::PictureCountFile(std::string fileName)
{
    mFileName = fileName;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void PictureCountFile::loadFile()
{
    std::ifstream ipFile(mFileName);
    Json::Reader reader;

    reader.parse(ipFile, mCounts);

    if(true == gVerbose)
    {
        print();
    }
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void PictureCountFile::saveFile()
{
    std::ofstream magFile(mFileName);

    Json::StyledWriter styledWriter;
    magFile << styledWriter.write(mCounts);
    magFile.close();
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void PictureCountFile::print()
{
    Json::StyledWriter styledWriter;
    std::cout << styledWriter.write(mCounts);
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PictureCountFile::getFileName(std::string pop, std::string location, std::string camera)
{
    int count = mCounts[pop][location][camera].asInt();
    return "file"+std::to_string(count);
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void PictureCountFile::addToFileNameCount(std::string pop, std::string location, std::string camera)
{
    int count = mCounts[pop][location][camera].asInt();
    count++;
    mCounts[pop][location][camera] = count;

}
