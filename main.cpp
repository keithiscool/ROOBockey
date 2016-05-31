////////////Keith Martin 2015-2016 - ROObockey Senior Design Team E - University of Akron : Design of a floor hockey puck shooting robot
////////////main.cpp - Project Used to Track Various Target Beacons of Different Shapes and Colors
//////////
////////////Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
////////////, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
////////////and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.
//////////
///////////*NOTE THAT I AM USING OPENCV-Version3.0.0 WITH MICROSOFT VISUAL STUDIO 2013*/
////////////Installation guide - https://www.youtube.com/watch?v=et7tLwpsADw
////////////OpenCV3.0.0 Install setup is included in "OpenCV_3_Windows_10_Installation_Tutorial-master" folder within this Github post
//////////
///////////* Object detector program (uses known shapes and colors to track beacons)
//////////* It loads an image and tries to find simple shapes (rectangle, triangle, circle, etc) in it.
//////////* This program is a modified version of `squares.cpp` found in the OpenCV sample dir*/


#define USE_EXTERNS
#define MAIN_CPP
#include "defs.hpp"

#include "GPIO_UART.hpp"
#include "Xbox360Controller.hpp"
#include "main.hpp"
#include "ObjectTracking.hpp"

//Multi-Core Operation Headers
#include <thread>
#include <chrono>
#include <mutex>

std::mutex inputLock;

int main(void) {

//Initialize the Xbox360 Wireless Controller and UART Module on the Raspberry Pi 2
	initController();
	initGPIO_Uart();

//Launch the thread for the Image Processing on the Raspberry Pi 2
	std::thread imageProcessingThread([]() -> void {
		while(1) {
			auto start = std::chrono::high_resolution_clock::now();

			{
				imageProcessingRoutine();
			}

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed = end - start;
			std::chrono::duration<double> second(0.3);

			if(elapsed.count() >= 0.3) {
				 std::this_thread::sleep_for(second - elapsed);
			}
		}
	});

//Launch the thread for the Xbox 360 Wireless Controller and GPIO polling
	std::thread inputOutputThread([]() -> void {
		while(1) {
			auto start = std::chrono::high_resolution_clock::now();

			{
				std::lock_guard<std::mutex> lock(inputLock);

				parseXbox360Controller();
				gpioPinOperations();
			}

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed = end - start;
			std::chrono::duration<double> second(0.1);

			if(elapsed.count() >= 0.1) {
				 std::this_thread::sleep_for(second - elapsed);
			}
		}
	});

//the code should never get to this point because it is stuck in the above while() loops
	imageProcessingThread.join();
	inputOutputThread.join();

	return 1;
}
