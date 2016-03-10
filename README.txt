/////////////////*
This project is used for my Senior Design Project - ROOBockey

In order to run this code on a Raspberry Pi 2, please do the following

1) follow the guides located in the "InstallationWalkthroughs" folder to install OPENCV 3.0.0 and CMAKE on the Raspberry Pi
2) make sure that "CMakeLists.txt" (located in project directory) has all files you need for your project (all .cpp and .hpp files are added to source includes)
3) open a terminal window on the RaspberryPi and type the following:
	cmake < insert project file path here (example: /home/pi/Desktop/ROOBockey-master) without the brackets "< and >" >
	make (this command will build the project using the makefile CMAKE creates when CMAKE finishes)
	./ROOBockey (this command will run the executable project file created by MAKE)
	
4) NOTE!!!
	When Running the ROOBockey project, only have the webcam, keyboard and mouse in the raspberry pi.
	Anything else plugged in (namely a wireless wifi dongle will prevent the webcam from receiving enough power via USB to run)
	
*//////////////////