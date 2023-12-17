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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include "Common.h"
#include <stdio.h>
#include "PropertyFile.h"


PropertyFile* PropertyFile::mInstance=NULL;

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
PropertyFile* PropertyFile::getInstance()
{
    if(NULL == mInstance)
    {
        mInstance = new PropertyFile();
    }
    return mInstance;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
PropertyFile::PropertyFile()
{
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void PropertyFile::loadFile(std::string fileName)
{
    mFileName = fileName;
    
    try
    {
        boost::property_tree::ini_parser::read_ini(fileName, mProperyTree);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getString(std::string entry)
{
    std::string returnValue = "";
    try
    {
        returnValue = mProperyTree.get<std::string>(entry);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << " for " << entry << '\n';
    }
    return returnValue;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
int PropertyFile::getInt(std::string entry)
{
    int returnValue = -1;
    try
    {
        returnValue = mProperyTree.get<int>(entry);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << " for " << entry << '\n';
    }
    return returnValue;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
double PropertyFile::getDouble(std::string entry)
{
    double returnValue = 0.0;
    try
    {
        returnValue = mProperyTree.get<double>(entry);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << " for " << entry << '\n';
    }
    return returnValue;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void PropertyFile::printTree(boost::property_tree::ptree &propertyTree, int level)
{
    if (propertyTree.empty())
    {
        std::cout << "\"" << propertyTree.data() << "\"";
    }
    else
    {
        if (level) std::cout << std::endl;

        std::cout << indent(level) << "{" << std::endl;

        for (boost::property_tree::ptree::iterator pos = propertyTree.begin(); pos != propertyTree.end();)
        {
            std::cout << indent(level + 1) << "\"" << pos->first << "\": ";

            printTree(pos->second, level + 1);
            ++pos;
        }

        std::cout << indent(level) << " }";
    }
    std::cout << std::endl;
    return;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void PropertyFile::printTree()
{
    printTree(mProperyTree,0);
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
double PropertyFile::getSlowSpeed()
{
    return getDouble("SlowSpeed");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
uint16_t PropertyFile::getFrameWaitTimeMs()
{
    return getInt("FrameWaitTimeMs");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
int16_t PropertyFile::getDegreeOffSet()
{
    return getInt("DegreeOffSet");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
double PropertyFile::getFastSpeed()
{
    return getDouble("FastSpeed");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
double PropertyFile::getNormalSpeed()
{
    return getDouble("NormalSpeed");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
double PropertyFile::getDegreesPerPicture()
{
    return getDouble("DegreesPerPicture");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getDataRoot()
{
    return getString("DataRoot");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getCamerasProp()
{
    return getString("CamerasProp");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getEncoderProp()
{
    return getString("EncoderProp");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getPicturesDir()
{
    return getString("PicturesDir");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getPictureFileCount()
{
    return getString("PictureFileCount");
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getFullCamerasProp()
{
    return getDataRoot()+getCamerasProp();
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getFullEncoderProp()
{
    return getDataRoot()+getEncoderProp();
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getFullPicturesDir()
{
    return getDataRoot()+getPicturesDir();
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
std::string PropertyFile::getFullPictureFileCount()
{
    return getDataRoot()+getPictureFileCount();
}
