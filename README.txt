/////////////////*
This project is used for my Senior Design Project - ROOBockey

I placed a "autorun command in "/home/pi/ .bashrc
the ".bashrc" file can be seen by going into the /home/pi folder and pressing cntrl+h
the code at the bottom is:
cd /home/pi/Downloads/ROOBockey-master
sudo ./ROOBockey

Helpful link for WiringPi with CMAKE: 
http://stackoverflow.com/questions/30424236/add-wiringpi-lib-to-cmake-on-raspberrypi
In order to assemble the CMake items, I placed a file called "WiringPiConfig.cmake" into
/usr/share/cmake-2.8/Modules

The code in that file was:
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)

Then I used the included CMakeLists.txt included with the project that points to the directory of the project:
cmake /home/pi/Downloads/ROOBockey-master 

then type:
make
=================================================================================================

In order to run this code on a Raspberry Pi 2, please do the following

1) follow the guides located in the "InstallationWalkthroughs" folder to install OPENCV 3.0.0 and CMAKE on the Raspberry Pi
2) install WiringPi for Raspberry Pi 2
	sudo apt-get install wiringpi
3) make sure that "CMakeLists.txt" (located in project directory) has all files you need for your project (all .cpp and .hpp files are added to source includes)
4) open a terminal window on the RaspberryPi and type the following:
	cmake < insert project file path here (example: /home/pi/Desktop/ROOBockey-master) without the brackets "< and >" >
	make (this command will build the project using the makefile CMAKE creates when CMAKE finishes)
	./ROOBockey (this command will run the executable project file created by MAKE)
	
5) NOTE!!!
	When Running the ROOBockey project, only have the webcam, keyboard and mouse in the raspberry pi.
	Anything else plugged in (namely a wireless wifi dongle will prevent the webcam from receiving enough power via USB to run)
	
6) NOTE!!!
	When running the code on a Raspberry Pi, make sure a heat sync and a fan are mounted to the RaspberryPi processor
	
*//////////////////