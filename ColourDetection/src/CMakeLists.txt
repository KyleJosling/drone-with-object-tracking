cmake_minimum_required(VERSION 3.5)

## Compile as C++11, supported in ROS Kinetic and newer
#add_compile_options(-std=c++11)

find_package(raspicam REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

IF (OpenCV_FOUND AND raspicam_CV_FOUND)
MESSAGE(STATUS "COMPILING")

include_directories(
  include
)
include_directories(
  ${OpenCV_INCLUDE_DIRS}
)
include_directories(
  ${raspicam_CV_INCLUDE_DIRS}
)

# If installing with ROS' version of OpenCV, have to add contrib modules seperately
link_directories(/opt/ros/kinetic/lib/arm-linux-gnueabihf/)
find_library(opencv_tracking3_LIBRARY opencv_tracking3)

# Libraries
link_directories(/usr/local/lib)
find_library(msp_LIBRARY msp)
find_library(msp_LIBRARY msp)
find_library(msp_fcu_LIBRARY msp_fcu)
find_library(mspclient_LIBRARY mspclient)
find_library(pid_LIBRARY pid)

# Local libraries
add_library(detect-Object-lib detectObject.cpp)
add_library(image-Handler-lib imageHandler.cpp)
add_library(fc-lib fc.cpp)

add_executable(Object-Tracking-Drone main.cpp)
target_link_libraries(Object-Tracking-Drone
    ${OpenCV_LIBS}
    ${pid_LIBRARY}
    ${msp_LIBRARY}
    ${msp_fcu_LIBRARY}
    ${mspclient_LIBRARY}
    ${raspicam_CV_LIBS}
    ${raspicam_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${opencv_tracking3_LIBRARY}
    image-Handler-lib
    detect-Object-lib
    fc-lib
    )
add_definitions(-std=c++11)

#Preprocessor macros
add_definitions(-DDISPLAY)
#add_definitions(src FLIGHT_CONTROLLER)

ELSE()
MESSAGE(FATAL_ERROR "MISSING RASPICAM")
ENDIF()
