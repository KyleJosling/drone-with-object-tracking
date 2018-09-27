cmake_minimum_required(VERSION 2.8)

## Compile as C++11, supported in ROS Kinetic and newer
#add_compile_options(-std=c++11)

find_package(raspicam REQUIRED)
find_package(OpenCV REQUIRED)
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

find_library(msp_LIBRARY msp)
find_library(msp_fcu_LIBRARY msp_fcu)
find_library(mspclient_LIBRARY mspclient)


add_executable(testy src/ColourTracking.cpp)
target_link_libraries(testy
    ${msp_LIBRARY}
    ${msp_fcu_LIBRARY}
    ${mspclient_LIBRARY}
    ${OpenCV_LIBS}
    ${raspicam_CV_LIBS}
    ${raspicam_LIBS}
    )
add_definitions(-std=c++11)

ELSE()
MESSAGE(FATAL_ERROR "MISSING RASPICAM")
ENDIF()
