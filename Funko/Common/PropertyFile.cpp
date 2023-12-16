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
#include "PropertyFile.h"

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
PropertyFile::PropertyFile(std::string fileName)
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

