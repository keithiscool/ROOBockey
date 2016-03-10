# CMAKE file for generating a "release version" executable for the ROOBockey OPENCV 3.0.0 project
# This template was taken from: http://docs.opencv.org/3.0-beta/doc/tutorials/introduction/linux_gcc_cmake/linux_gcc_cmake.html
# Also Looking off here: http://stackoverflow.com/questions/33749216/compiling-an-opencv-project-using-cmake-on-cygwin-with-opencv-installed-for-win


# To use this file to build the C++ OpenCV3 Project:
# cd into the project directory (where this file is located) and type cmake, then make
# 		EXAMPLE:
# cd ~/Desktop/ROOBockey
# cmake
# make


cmake_minimum_required(VERSION 2.8)
project( ROOBockey )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


set( NAME_SRC
	main.cpp
	ObjectTracking.cpp
	Beacons.cpp
	#UART.cpp
	#Xbox360Controller.cpp
)

set( NAME_HEADERS       
    #include/mytest.hpp
	main.hpp
	ObjectTracking.hpp
	Beacons.hpp
	#UART.hpp
	#Xbox360Controller.hpp
	defs.hpp
)


SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")


# NOW ADD THE OUTPUT EXECUTABLE FILE NAME FOLLOWED BY ALL SOURCE FILES TO THE CMAKE OUTPUT
add_executable( ROOBockey ${NAME_SRC} ${NAME_HEADERS} )


##################################################################################
#I used the following libraries (located in def.hpp)
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>

target_link_libraries(ROOBockey ${OpenCV_LIBS} )
