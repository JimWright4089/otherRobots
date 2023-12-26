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

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
//#include <boost/tokenizer.hpp>
//#include <boost/token_functions.hpp>

const std::string ALL_POPS[] = {"leaf", "boba", "luke", "hup", "twin", "phin", "ezra", "boo", "sull", "ches"};
const std::string ALL_LOCS[] = {"ktch", "tv", "dine", "bhal", "fhal", "lliv", "rliv", "bliv"};

bool gJustHelp = false;
bool gVerbose = false;
bool gCalibrate = false;
bool gDisplayCameras = false;
bool gCalibrateCameras = false;
bool gTestFrames = false;
std::string gPop = "leaf";
std::string gLoc = "ktch";

// --------------------------------------------------------------------
// Purpose:
//   Set and check the command line options
//
// Notes:
//   None.
// --------------------------------------------------------------------
bool setOptionFlags(int argc, char* argv[])
{
    boost::program_options::options_description general("General options");
    general.add_options()
        ("help,h", "produce help message")
        ("calibrate,t", "Just calibrate the table")
        ("display,d", "display all of the capture video devices")
        ("camera,c", "create the calibration frames")
        ("frames,f", "create the test frames")
        ("verbose,v", "print lots of debug junk")
        ("pop,p", boost::program_options::value<std::string>(&gPop)->default_value("leaf"), "four char Funko Pop (leaf, boba, luke, hup, twin, phin, ezra, boo, sull, ches)")
        ("loc,l", boost::program_options::value<std::string>(&gLoc)->default_value("ktch"), "four char Location (ktch, tv, dine, bhal, fhal, lliv, rliv, bliv)")
    ;

    try {
        boost::program_options::variables_map optionsEntered;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, general), optionsEntered);
        boost::program_options::notify(optionsEntered);

        if (optionsEntered.count("help")) {
            std::cout << general << "\n";
            gJustHelp = true;
        }

        if (optionsEntered.count("calibrate")) {
            gCalibrate = true;
        }

        if (optionsEntered.count("camera")) {
            gCalibrateCameras = true;
        }

        if (optionsEntered.count("frames")) {
            gTestFrames = true;
        }

        if (optionsEntered.count("display")) {
            gDisplayCameras = true;
        }

        if (optionsEntered.count("verbose")) {
            gVerbose = true;
        }
    }
    catch(std::exception& e) {
        std::cout << "Error:" << e.what() << "\n";
        std::cout << general << "\n";
        return false;
    }

    if(true == gVerbose)
    {
        std::cout << "gVerbose:" << gVerbose << "\n";
        std::cout << "gCalibrate:" << gCalibrate << "\n";
        std::cout << "gPop:" << gPop << "\n";
        std::cout << "gLoc:" << gLoc << "\n";
    }

    bool popFound = false;
    for(std::string item : ALL_POPS)
    {
        if(gPop == item)
        {
            popFound = true;
            break;
        }
    }

    if(false == popFound)
    {
        std::cout << "Bad Pop name:" << gPop <<"\n";
        std::cout << general << "\n";
    }

    bool locFound = false;
    for(std::string item : ALL_LOCS)
    {
        if(gLoc == item)
        {
            locFound = true;
            break;
        }
    }

    if(false == locFound)
    {
        std::cout << "Bad location name:" << gLoc << "\n";
        std::cout << general << "\n";
    }

    if((false == popFound)||(false == locFound))
    {
        return false;
    }

    if(true == gJustHelp)
    {
        return false;
    }

    return true;
}

