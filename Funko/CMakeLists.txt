cmake_minimum_required(VERSION 3.10)

# set the project name
project(PPSSLoader)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(./Common)
find_package(jsoncpp REQUIRED)

# Find Boost libs.
FIND_PACKAGE( Boost 1.74.0 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(Funko ./Funko/Funko.cpp ./Common/Options.cpp)
target_link_libraries(Funko jsoncpp ${Boost_LIBRARIES})

