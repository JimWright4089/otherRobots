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
#include <string>
#include <vector>
#include <libudev.h>
#include <sys/stat.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <json/json.h>
#include <opencv2/opencv.hpp>
#include "DisplayDevices.h"
#include "Device.h"
#include "Options.h"
#include "PropertyFile.h"

//----------------------------------------------------------------------------
//  Local Variables
//----------------------------------------------------------------------------
const std::string DEVICE = "device";
const std::string MODEL = "model";
const std::string SERIAL_NUMBER = "serialNumber";
const std::string FILE_NAME = "fileName";

//----------------------------------------------------------------------------
//  Local Variables
//----------------------------------------------------------------------------
std::vector<Device*> lDevices;

//#define DEBUG

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void ReadUsbIdentifiers(std::string dev_path, 
  std::string &model, std::string &serialNumber, std::string &capabilities)
{
    auto udev = udev_new();
    if (!udev) { return; }

    struct stat statbuf;
    if (stat(dev_path.c_str(), &statbuf) < 0) { return; }
    auto type =  S_ISBLK(statbuf.st_mode) ? 'b' : S_ISCHR(statbuf.st_mode) ? 'c' : 0;

    auto opened_dev = udev_device_new_from_devnum(udev, type, statbuf.st_rdev);
    auto dev = opened_dev;

    if (dev != nullptr)
    {
        struct udev_list_entry *list_entry;

        udev_list_entry_foreach(list_entry, udev_device_get_properties_list_entry(dev))
        {
#ifdef DEBUG
          printf("'%s' '%s'\n",
                  udev_list_entry_get_name(list_entry),
                  udev_list_entry_get_value(list_entry));
#endif

          if(0 == strcmp("ID_MODEL",udev_list_entry_get_name(list_entry)))
          {
            model = std::string(udev_list_entry_get_value(list_entry));
          }

          if(0 == strcmp("ID_USB_SERIAL_SHORT",udev_list_entry_get_name(list_entry)))
          {
            serialNumber = std::string(udev_list_entry_get_value(list_entry));
          }

          if(0 == strcmp("ID_V4L_CAPABILITIES",udev_list_entry_get_name(list_entry)))
          {
            capabilities = std::string(udev_list_entry_get_value(list_entry));
          }
        }
    }
    if (opened_dev) { udev_device_unref(opened_dev); }
    udev_unref(udev);
}

// --------------------------------------------------------------------
// Purpose:
// Return the value with a dead band where it is zero
//
// Notes:
// None.
// --------------------------------------------------------------------
void displayDevices(void)
{
  std::vector<boost::filesystem::path> files_in_directory;
  std::copy(boost::filesystem::directory_iterator("/dev"), 
  boost::filesystem::directory_iterator(), std::back_inserter(files_in_directory));
  std::sort(files_in_directory.begin(), files_in_directory.end());
  std::string model;
  std::string serialNumber;
  std::string capabilites;

  for (const boost::filesystem::path& filename : files_in_directory) 
  {
    if(-1 != filename.filename().string().find("video"))
    {
      std::string fullFilename = "/dev/"+filename.filename().string();

      ReadUsbIdentifiers(fullFilename,model,serialNumber,capabilites);
      if(-1 != capabilites.find(":capture:"))
      {
        Device* newDevice = new Device();
        newDevice->setDevice(fullFilename);
        newDevice->setModel(model);
        newDevice->setSerialNumber(serialNumber);
        lDevices.push_back(newDevice);
      }
    }
  }

  if(true == gVerbose)
  {
    std::cout << "deivces to try:" << "\n";
    for(Device* fileName : lDevices)
    {
      std::cout << "  " << fileName->getFileName() << "\n";
    }
  }

  Json::Value displayJSON;
  for(Device* fileName : lDevices)
  {
    std::string frameFileName = PropertyFile::getInstance()->getPicturesDir();

    if(false == boost::filesystem::is_directory(frameFileName))
    {
        boost::filesystem::create_directory(frameFileName);
    }

    frameFileName += PropertyFile::getInstance()->getDisplayDir();

    if(false == boost::filesystem::is_directory(frameFileName))
    {
        boost::filesystem::create_directory(frameFileName);
    }

    frameFileName += fileName->getFileName()+".jpg";
    std::cout << frameFileName << "\n";

    cv::Mat frame;
    cv::VideoCapture capture;
    capture.open(fileName->getDevice());

    for(int j=0;j<60;j++)
    {
        capture >> frame;

        if(frame.empty())
        {
          std::cout << "Did not get a frame, skipping\n";
          break;
        }

    }
    if (!frame.empty())
    {
        cv::imwrite(frameFileName, frame);
    }
    capture.release();

    Json::Value device;

    device[MODEL] = fileName->getModel();
    device[SERIAL_NUMBER] = fileName->getSerialNumber();
    device[FILE_NAME] = fileName->getFileName();
    device[DEVICE] = fileName->getDevice();

    displayJSON[fileName->getDevice()] = device;
  }

  std::ofstream displayFile(PropertyFile::getInstance()->getFullDisplayProp());
  Json::StyledWriter styledWriter;
  displayFile << styledWriter.write(displayJSON);
  displayFile.close();
}

