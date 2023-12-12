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
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <exception>
#include "propertyFile.h"
#include "ipAddressFile.h"
#include "loadFile.h"
#include "loaderLog.h"
#include "loadAddressFile.h"
#include "coap.h"
#include "seatMap.h"
#include "system.h"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

bool gWriteLog = true;
bool gSWCheckOnly = false;
bool gDisplayFW = false;
bool gMissing = false;
bool gOverrideSW = false;
bool gOverrideType = false;
bool gVerbose = false;
bool gJustHelp = false;

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void setFlags(int argc, char* argv[])
{
    try {
        boost::program_options::options_description general("General options");
        general.add_options()
            ("help,h", "produce a help message")
            ("firmware,f", "display the load file")
            ("swver,s", "just display the swver of the WDLMs (no log)")
            ("missing,m", "just load the missing (bypass checks and ready) [you really should just reload everything]")
            ("overridesw,o", "override the SW version")
            ("overridetype,t", "override the Type")
            ("dontwritelog,d", "don't write the log file")
            ("verbose,v", "print lots of debug junk");

        boost::program_options::variables_map options_entered;
        store(parse_command_line(argc, argv, general), options_entered);

        for (const auto& it : options_entered) 
        {
            std::string command = it.first;

            if (command == "help") 
            {
                std::cout << general;
                gJustHelp = true;
            }
            if (command == "swver") 
            {
                gSWCheckOnly = true;
                gWriteLog = false;
            }
            if (command == "dontwritelog") 
            {
                gWriteLog = false;
            }
            if (command == "firmware") 
            {
                gDisplayFW = true;
            }
            if (command == "missing") 
            {
                gMissing = true;
            }
            if (command == "overridesw") 
            {
                gOverrideSW = true;
            }
            if (command == "overridetype") 
            {
                gOverrideType = true;
            }
            if (command == "verbose") 
            {
                gVerbose = true;
            }
        }
    }
    catch(std::exception& e) {
        std::cout << e.what() << "\n";
    }
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
int mainy(int argc, char* argv[])
{
    setFlags(argc, argv);

    std::cout << "gWriteLog:" << gWriteLog << "\n";
    std::cout << "gSWCheckOnly:" << gSWCheckOnly << "\n";
    std::cout << "gDisplayFW:" << gDisplayFW << "\n";
    std::cout << "gMissing:" << gMissing << "\n";
    std::cout << "gOverrideSW:" << gOverrideSW << "\n";
    std::cout << "gOverrideType:" << gOverrideType << "\n";
    std::cout << "gVerbose:" << gVerbose << "\n";

    return 0;
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
int main(int argc, char* argv[])
{
    setFlags(argc, argv);

    if(true == gJustHelp)
    {
        return -1;
    }

    LoaderLog theLog;
    System theSystem;
 	uint8_t messageBuffer[MAX_BUFFER_SIZE];
	uint16_t messageLength = MAX_BUFFER_SIZE;
    uint8_t readBuffer[MAX_BUFFER_SIZE];
	uint16_t readLength = MAX_BUFFER_SIZE;
    LoadAddresses readyWDLMs;
    LoadAddresses readyValidatedWDLMs;

    if(true == gVerbose)
    {
        std::cout << "\ngWriteLog:" << gWriteLog << "\n";
        std::cout << "gSWCheckOnly:" << gSWCheckOnly << "\n";
        std::cout << "gDisplayFW:" << gDisplayFW << "\n";
        std::cout << "gMissing:" << gMissing << "\n";
        std::cout << "gOverrideSW:" << gOverrideSW << "\n";
        std::cout << "gOverrideType:" << gOverrideType << "\n";
        std::cout << "gVerbose:" << gVerbose << "\n";
    }

    // Write the flags to the log
    if(false == gWriteLog) theLog.addFlag("dontwritelog");
    if(true == gSWCheckOnly) theLog.addFlag("swver");
    if(true == gDisplayFW) theLog.addFlag("firmware");
    if(true == gMissing) theLog.addFlag("missing");
    if(true == gOverrideSW) theLog.addFlag("overridesw");
    if(true == gOverrideType) theLog.addFlag("overridetype");
    if(true == gVerbose) theLog.addFlag("verbose");

    theLog.setSystem(&theSystem);
    theLog.setWriteFlag(gWriteLog);
    auto beginTime = std::chrono::high_resolution_clock::now();
    auto curTime = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - beginTime);
    theLog.addStartAll();
    theLog.save();

    PropertyFile propertyFile("/etc/ppss/ppss_config.properties");

    std::cout << "IP address file\n";
    IPAddresses addressesFile(propertyFile.getString("ipAddressFile"));
    std::cout << "load file\n";
    LoadFile loadFile(propertyFile.getString("loadFile"));
    LoadFile::sendBlock theSendBlock;
    std::cout << "load address file\n";
    LoadAddresses loadAddressFile("loadAddress.json");
    SeatMap seatMap;
    
    seatMap.setSystem(&theSystem);
    std::cout << "seat DB file:" <<propertyFile.getString("seatDbCsvPath")<<"\n";
    seatMap.loadFile(propertyFile.getString("seatDbCsvPath"));

    if(true == gVerbose)
    {
        std::cout << "\nLoad Addresses:\n";
        loadAddressFile.print();
    }
    std::vector<std::string> wdlmKeys = addressesFile.getKeys();

    std::cout << "load block file\n";
    uint32_t loadBlockWaitTimeMS = propertyFile.getInt("load_block_wait_time");
    uint32_t loadBlankWaitTimeMS = propertyFile.getInt("load_blank_wait_time");
    uint32_t loadBetweenTimeMS = propertyFile.getInt("load_between_time");
    uint32_t loadMissingTimeMS = propertyFile.getInt("load_missinig_time");

    if(true == gVerbose)
    {
        std::cout << "\nloadBlockWaitTimeMS:" << loadBlockWaitTimeMS << "\n";
        std::cout << "loadBlankWaitTimeMS:" << loadBlankWaitTimeMS << "\n";
        std::cout << "loadBetweenTimeMS:" << loadBetweenTimeMS << "\n";
        std::cout << "loadMissingTimeMS:" << loadMissingTimeMS << "\n";
        std::cout << "ipAddressFile:" << propertyFile.getString("ipAddressFile") << "\n";
        std::cout << "loadFile:" << propertyFile.getString("loadFile") << "\n";
        std::cout << "loadLocation:" << propertyFile.getString("load_location") << "\n";
    }

    theLog.addValue("loadBlockWaitTimeMS",std::to_string((int)loadBlockWaitTimeMS));
    theLog.addValue("loadBlankWaitTimeMS",std::to_string((int)loadBlankWaitTimeMS));
    theLog.addValue("loadBetweenTimeMS",std::to_string((int)loadBetweenTimeMS));
    theLog.addValue("loadMissingTimeMS",std::to_string((int)loadMissingTimeMS));
    theLog.addValue("loadLocation",propertyFile.getString("load_location"));


    if(0 == loadAddressFile.getAddresses().size())
    {
        for(std::string key : wdlmKeys)
        {
            loadAddressFile.addAddress(key);
        }
    }

    if(true == gVerbose)
    {
        std::cout << "\nLoad Candidate Addresses:\n";
        loadAddressFile.print();
    }
    std::vector<std::string> wdlmLoadKeys = loadAddressFile.getAddresses();

    if(true == gDisplayFW)
    {
        loadFile.print();
    }

    loadFile.findVersion();
    uint32_t theMajor = loadFile.getMajor();
    uint32_t theMinor = loadFile.getMinor();
    std::string theType = loadFile.findType();
    std::cout << "\nVersion:" << theMajor << "." << theMinor << "\n";
    std::cout << "Type:" <<  theType << "\n";
//    theLog.addVersion(theMajor,theMinor);
    theLog.addValue("Type",theType);
    theLog.addValue("MajorVersion",std::to_string((int)theMajor));
    theLog.addValue("MinorVersion",std::to_string((int)theMinor));

    std::cout << "\nnumber of blocks:" << loadFile.getBlockCount() << "\n";

    int total = loadFile.getBlockCount();
    int loadLines = 0;
    int blankLines = 0;
    for(int i=0;i<total;i++)
    {
        loadFile.getBlock(i,&theSendBlock);
        if(true == loadFile.isBlank(theSendBlock))
        {
            blankLines++;
        }
        else
        {
            loadLines++;
        }
    }
    std::cout << "  load blocks:" << loadLines << "\n";
    std::cout << "  blank blocks:" << blankLines << "\n";

    theLog.addValue("BlankLines",std::to_string((int)blankLines));
    theLog.addValue("LoadLines",std::to_string((int)loadLines));
//    theLog.addBlankLines();
//    theLog.addLoadLines(loadLines);
    theLog.append();

	if(false == buildMessage(messageBuffer,&messageLength,(uint8_t*)"/ota/Start",
        CoapPDU::COAP_GET,CoapPDU::COAP_CONFIRMABLE,0xeac4))
	{
		std::cout << "Bad message\n";
	}

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////                               Sending Start Message
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if(false == gMissing)
    {
        if(true == gVerbose)
        {
            std::cout << "\nReady Check:\n";
        }

        for(std::string key : wdlmLoadKeys)
        {
            if(true == gVerbose)
            {
                std::cout << "  " << key << "\n";
            }

            if (std::find(wdlmKeys.begin(), wdlmKeys.end(), key) == wdlmKeys.end()) 
            {
                std::cout << "eui64:" << key << " is not in this machines seat map.\n";
            }
            else 
            {
                readLength = MAX_BUFFER_SIZE;
                if(false == sendAndGetCoap((uint8_t*)addressesFile.getIPAddress(key).c_str(), 
                    messageBuffer, messageLength, readBuffer, &readLength))
                {
                    std::cout << "  WDLM:" << key << " is not taking to us /ota/Start\n";
                }
                else
                {
                    CoapPDU readMessage(readBuffer, readLength);

                    if(0 == strcmp("READY",(char *)readMessage.getPayloadPointer()))
                    {
                        if(true == gVerbose)
                        {
                            std::cout << "  WDLM:" << key << " is ready\n";
                        }
                        readyWDLMs.addAddress(key);
                    }
                    else
                    {
                        std::cout << "  WDLM:" << key << " is not ready\n";
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(loadBetweenTimeMS));
            }        
        }
    }
    else
    {
        // If we just want missing load all of them
        for(std::string key : wdlmLoadKeys)
        {
            readyWDLMs.addAddress(key);
        }
    }

    if(true == readyWDLMs.isEmpty())
    {
        std::cout << "\nNo loadable WDLMs were in the load file, exiting\n";
        return -1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////                               Get SW Version
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    if(true == gVerbose)
    {
        std::cout << "\nGet SW Version:\n";
    }

    for(std::string key : readyWDLMs.getAddresses())
    {
        if(true == gVerbose)
        {
            std::cout << "  " << key << "\n";
        }

        messageLength = MAX_BUFFER_SIZE;
        if(false == buildMessage(messageBuffer,&messageLength,(uint8_t*)"/ota/swver",
            CoapPDU::COAP_GET,CoapPDU::COAP_CONFIRMABLE,0xeac4))
        {
            std::cout << "Bad message\n";
        }
        else
        {
            readLength = MAX_BUFFER_SIZE;
            if(false == sendAndGetCoap((uint8_t*)addressesFile.getIPAddress(key).c_str(), 
                messageBuffer, messageLength, readBuffer, &readLength))
            {
                std::cout << "  WDLM:" << key << " is not taking to us /ota/swver\n";
                theSystem.addOneToErrorCount(key);
                if(true == theSystem.amIOverErrorCount(key))
                {
                    readyWDLMs.removeAddress(key);
                }
            }
            else
            {
                readLength = MAX_BUFFER_SIZE;
                CoapPDU readMessage(readBuffer, readLength);
                uint32_t fullVersion = 0;
                uint32_t majorVersion = 0;
                uint32_t minorVersion = 0;
                memcpy(&fullVersion,readMessage.getPayloadPointer(),sizeof(uint32_t));
                minorVersion = fullVersion & 0xff;
                majorVersion = fullVersion >> 8;

                theSystem.addVersion(key,majorVersion,minorVersion);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(loadBetweenTimeMS));
        }

        messageLength = MAX_BUFFER_SIZE;
        if(false == buildMessage(messageBuffer,&messageLength,(uint8_t*)"/ota/swtype",
            CoapPDU::COAP_GET,CoapPDU::COAP_CONFIRMABLE,0xfac3))
        {
            std::cout << "Bad message\n";
        }
        else
        {
            readLength = MAX_BUFFER_SIZE;
            if(false == sendAndGetCoap((uint8_t*)addressesFile.getIPAddress(key).c_str(), 
                messageBuffer, messageLength, readBuffer, &readLength))
            {
                std::cout << "  WDLM:" << key << " is not taking to us /ota/swtype\n";
                theSystem.addOneToErrorCount(key);
                if(true == theSystem.amIOverErrorCount(key))
                {
                    readyWDLMs.removeAddress(key);
                }
            }
            else
            {
                readLength = MAX_BUFFER_SIZE;
                CoapPDU readMessage(readBuffer, readLength);
#ifdef DEBUG
                readMessage.printHuman();
#endif                

                std::string type = "unknown";
                if(CoapPDU::COAP_NOT_FOUND != readMessage.getCode())
                {
                    type = std::string((char*)readMessage.getPayloadPointer());
                }
                theSystem.addType(key,type);                
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(loadBetweenTimeMS));
        }
    }

    if(false == gMissing)
    {
        std::cout << "\nChecking:\n";
        for(std::string key : readyWDLMs.getAddresses())
        {
            uint32_t major = 0;
            uint32_t minor = 0;
            theSystem.getVersion(key,&major,&minor);
            std::cout << "  Checking:" << key << " cur SW:" << major << "." << minor;
            std::cout << " cur Type:" << theSystem.getType(key);
            std::cout << " SW:" << (theSystem.isVersionGreater(key,theMajor,theMinor)?"true":"false");
            std::cout << " Type:" << (theSystem.isTypeEqual(key, theType)?"true":"false") << "\n";
    
            if(((true == theSystem.isTypeEqual(key, theType))||(true == gOverrideType)) &&
            ((true == theSystem.isVersionGreater(key,theMajor,theMinor)) || (true == gOverrideSW)))
            {
                readyValidatedWDLMs.addAddress(key);
            }
        }
    }
    else
    {
        for(std::string key : readyWDLMs.getAddresses())
        {
            readyValidatedWDLMs.addAddress(key);
        }
    }

    if(true == readyValidatedWDLMs.isEmpty())
    {
        std::cout << "\nNo loadable WDLMs were in the load file, exiting\n";
        return -1;
    }

    if(true == gVerbose)
    {
        std::cout << "\nFinal Load List:\n";
        for(std::string key : readyValidatedWDLMs.getAddresses())
        {
            std::cout << "  WDLM:" << key << "\n";
        }
    }

    theLog.append();
    if(true == gSWCheckOnly)
    {
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////                               Send all lines
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    if(false == gMissing)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::this_thread::sleep_for(std::chrono::seconds(24));

        curTime = std::chrono::high_resolution_clock::now();
        duration =  std::chrono::duration_cast<std::chrono::milliseconds>(curTime - beginTime);
        theLog.addValue("StartLoadTime",std::to_string((int)duration.count()));
//        theLog.addStartLoading(duration.count());
        theLog.append();

        std::cout << "\nBulk Loading:\n";

        int blankLinesLoaded = 0;
        int loadLinesLoaded = 0;
        for(int i=0;i<total;i++)
        {
    //        if((i < 2)||(i>6))
            {
                loadFile.getBlock(i,&theSendBlock);

                auto curTime = std::chrono::high_resolution_clock::now();
                std::chrono::milliseconds duration =  std::chrono::duration_cast<std::chrono::milliseconds>(curTime - beginTime);
                double theTime = duration.count();
                theTime /= 1000;
                theTime /= 60;
                int minutes = (int)theTime;
                theTime -= minutes;
                int seconds = 60*theTime;

                printf("  %d,%d/%d,%d %7.3f %7.3f %d:%02d %d ",loadLinesLoaded,blankLinesLoaded,
                            loadLines,blankLines,
                            (double)loadLinesLoaded/(double)loadLines*100.0,
                            (double)blankLinesLoaded/(double)blankLines*100.0,
                            minutes,seconds,
                            loadFile.isBlank(theSendBlock));
                loadFile.printBlock(theSendBlock);

                messageLength = MAX_BUFFER_SIZE;

                if(true == loadFile.isBlank(theSendBlock))
                {
                    blankLinesLoaded++;
                    if(false == buildMessage(messageBuffer,&messageLength,(uint8_t*)"/ota/blank",
                        CoapPDU::COAP_PUT,CoapPDU::COAP_NON_CONFIRMABLE,0xea2c,
                        (uint8_t*)&theSendBlock.address,sizeof(uint16_t)))
                    {
                        std::cout << "Bad message\n";
                    }
                }
                else
                {
                    loadLinesLoaded++;
                    if(false == buildMessage(messageBuffer,&messageLength,(uint8_t*)"/ota/hexImage",
                        CoapPDU::COAP_PUT,CoapPDU::COAP_NON_CONFIRMABLE,0xeac5,(uint8_t*)&theSendBlock,sizeof(LoadFile::sendBlock)))
                    {
                        std::cout << "Bad message\n";
                    }
                }
                for(std::string key : readyValidatedWDLMs.getAddresses())
                {
                    if(false == sendCoap((uint8_t*)addressesFile.getIPAddress(key).c_str(), messageBuffer, messageLength))
                    {
                        std::cout << "Bad send\n";
                        theSystem.addOneToErrorCount(key);
                        if(true == theSystem.amIOverErrorCount(key))
                        {
                            readyValidatedWDLMs.removeAddress(key);
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(loadBetweenTimeMS));
                }

                if(true == loadFile.isBlank(theSendBlock))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(loadBlankWaitTimeMS));
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(loadBlockWaitTimeMS));
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////                               Missing
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\nMissing:\n";
    curTime = std::chrono::high_resolution_clock::now();
    duration =  std::chrono::duration_cast<std::chrono::milliseconds>(curTime - beginTime);
    theLog.addValue("FinishLoadTime",std::to_string((int)duration.count()));
//    theLog.addFinishLoading(duration.count());
    theLog.append();

    for(std::string key : readyValidatedWDLMs.getAddresses())
    {
        bool ready = false;
        do
        {
            messageLength = MAX_BUFFER_SIZE;
            if(false == buildMessage(messageBuffer,&messageLength,(uint8_t*)"/ota/missing",
                CoapPDU::COAP_GET,CoapPDU::COAP_CONFIRMABLE,0x1211))
            {
                std::cout << "Bad message\n";
            }

            readLength = MAX_BUFFER_SIZE;
            if(false == sendAndGetCoap((uint8_t*)addressesFile.getIPAddress(key).c_str(), 
                messageBuffer, messageLength, readBuffer, &readLength))
            {
                std::cout << "  WDLM:" << key << " is not taking to us /ota/missing\n";
                theSystem.addOneToErrorCount(key);
                if(true == theSystem.amIOverErrorCount(key))
                {
                    readyValidatedWDLMs.removeAddress(key);
                    break;
                }
               std::this_thread::sleep_for(std::chrono::seconds(3));
            }
            else
            {
                CoapPDU readMessage(readBuffer, readLength);
#ifdef DEBUG
                readMessage.printHuman();
#endif                
                if(0 == strncmp("READ",(char *)readMessage.getPayloadPointer(),4))
                {
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////                               Load It
                    ////////////////////////////////////////////////////////////////////////////////////////////////
                    ////////////////////////////////////////////////////////////////////////////////////////////////

                    std::cout << "  WDLM:" << key << " is ready\n";

                    if(false == buildMessage(messageBuffer,&messageLength,(uint8_t*)"/ota/loadit",
                        CoapPDU::COAP_PUT,CoapPDU::COAP_NON_CONFIRMABLE,0x1122))
                    {
                        std::cout << "Bad message\n";
                    }

//                    std::cout << "send eui64:" << key << " load it "<< addressesFile.getIPAddress(key) <<"\n";
                    readLength = MAX_BUFFER_SIZE;
                    if(false == sendAndGetCoap((uint8_t*)addressesFile.getIPAddress(key).c_str(), 
                        messageBuffer, messageLength, readBuffer, &readLength))
                    {
                        std::cout << "  WDLM:" << key << " is not taking to us /ota/loadit (this is ok)\n";
                    }
                    theSystem.setFinished(key,true);
                    theLog.append();
                    ready = true;
                }
                else
                {
                    uint16_t address = 0;
                    memcpy(&address,readMessage.getPayloadPointer(),2);
                    std::cout << "  key:" << key << " missing:" << address << " ";
                    theSystem.addMissing(key,address);

                    loadFile.getBlockByAddress(address,&theSendBlock);
                    loadFile.printBlock(theSendBlock);
                    messageLength = MAX_BUFFER_SIZE;

                    if(false == buildMessage(messageBuffer,&messageLength,(uint8_t*)"/ota/hexImage",
                        CoapPDU::COAP_PUT,CoapPDU::COAP_NON_CONFIRMABLE,0xeac5,(uint8_t*)&theSendBlock,sizeof(LoadFile::sendBlock)))
                    {
                        std::cout << "Bad message\n";
                    }

                    if(false == sendCoap((uint8_t*)addressesFile.getIPAddress(key).c_str(), 
                            messageBuffer, messageLength))
                    {
                        std::cout << "Bad send /ota/hexImage\n";
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(loadBetweenTimeMS));
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(loadBetweenTimeMS));
        } while (false == ready);
    }
 
    curTime = std::chrono::high_resolution_clock::now();
    duration =  std::chrono::duration_cast<std::chrono::milliseconds>(curTime - beginTime);
    theLog.addValue("FinishTime",std::to_string((int)duration.count()));
//    theLog.addFinishAll();
    theLog.append();

    return 0;
}