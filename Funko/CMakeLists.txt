cmake_minimum_required(VERSION 3.10)

# set the project name
project(PPSSLoader)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(./Common)
include_directories(./Drivers)
find_package(jsoncpp REQUIRED)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)

# Find Boost libs.
FIND_PACKAGE( Boost 1.74.0 COMPONENTS program_options filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(Funko ./Funko/Funko.cpp ./Common/Options.cpp ./Common/PropertyFile.cpp ./Common/Common.cpp 
             ./Common/PictureCountFile.cpp ./Common/EncoderSettingFile.cpp
             ./Drivers/JimsGPIO.cpp ./Drivers/JimsSerial.cpp ./Drivers/JimsRobotclaw.cpp)
target_link_libraries(Funko jsoncpp ${Boost_LIBRARIES} wiringPi ${OpenCV_LIBS})

