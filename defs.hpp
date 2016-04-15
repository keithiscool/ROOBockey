#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
using namespace cv;
using namespace std;


#ifndef DEFS_HPP
#define DEFS_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*GLOBAL VARIABLES TO SHARE WITH GPIO_UART.CPP*/
//flag used to eliminate random noise from when wireless Xbox360 controller connects with all values @ 1
extern int goodData;
//user needs the breakbeam sensor to operate the puck launcher
extern int shootPermissive;
//UART port ID for Tx to motor controller
extern int UART_ID;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//PIN ASSIGNMENTS -- Discrete Inputs/Outputs:
//NOTE: THESE USE BROADCOM NUMBERS SINCE WiringPi DID NOT MAP THEM CORRECTLY
//NOTE: Output at end of variable means "real-world output"
//PLEASE LOOK HERE FOR CORRECT PINOUT DIAGRAM:
//http://wiringpi.com/pins/

extern int breakBeamInput;						//GPIO pin 27 input from break beam (garage-door-like sensor)
extern int shutdownPiSwitchInput;				//GPIO pin 24 input to run script to nicely power off RPi2 PowerLED

//Input at end of variable means "real-world input"
extern int breakBeamLEDOutput;					//GPIO pin 17 output a test output for the Break Beam
extern int shootPinOutput;						//GPIO pin 18 output controls the solenoid discrete output
extern int controllerConnectedLEDOutput;		//GPIO pin 22 output controls the solenoid discrete output
extern int enableAndGateOutput;					//GPIO pin 23 output controls the solenoid discrete output

///////////////////////////////////////////////////////////////////////////////////////////////////////////

//default display capture window frame width and height (640x480 window)
extern const int FRAME_WIDTH;
extern const int FRAME_HEIGHT;

///////////////////////////////////////////////////////////////////////////////////////////////////////////

//THESE ARE THE PROTECTED WIRELESS CONTROLLER VALUES FOR THE XBOX360 WIRELESS CONTROLLER
//THESE VARIABLES ARE THE FINAL OUTPUT VALUES THAT CAN BE USED FOR THE CONTROLLER
//THIS WAS IMPLEMENTED BECAUSE THE ***CONTROLLER INPUTS ALL 1's WHEN IT CONNECTS***
//TO GET PAST THIS ISSUE, THE CODE WAITS UNTIL THE BUTTONS RETURN TO A ZERO STATE BEFORE IT CONTINUES
//Declare all buttons (including select,start along with leftstick & rightstick presses
bool Ba = 0, Bb = 0, Bx = 0, By = 0, BlBump = 0, BrBump = 0, Bsel = 0, Bstart = 0, BlStick = 0, BrStick = 0, BxboxCenterIcon = 0;

//Declare all joysticks (16 bit signed integers)
int Lx = 0, Ly = 0, Rx = 0, Ry = 0, Lt = 0, Rt = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PRINT_CONTROLLER_DATA 1
#define PRINT_SERIAL_DATA 1
//#define SOFTWARE_EMERGENCY_STOP 1
#define PERMIT_SHUTDOWN_PI_USING_GPIO_OR_CONTROLLER 1

//#define ShowDetectedObjects 1
#define SHOW_OPENCV_IMAGES 1
#define USING_WEBCAM 1 //flag that is set to control whether the user uses the camera for input or a still picture as an input
//#define CALIBRATION_MODE 1 //calibrate the HSV filter for a specific color
#define CAMERA_NUMBER 0 //flag to set source of video: "camera 0" is the builtin laptop webcam, "camera 1" is usb webcam
#define MAX_NUM_OBJECTS 30 // Program will only track 30 objects at a time (this is just in case noise becomes a problem)
//#define MIN_OBJECT_AREA 400 //Only allow larger objects //200*200
#define MIN_OBJECT_AREA 1000 //Only allow larger objects
#define JOYSTICK_DEADZONE 10000 //only care about joystick values outside of the deadzone + & - around 0


#define BLUE (Scalar(255, 0, 0)) //BGR, not RGB (I do not know why colors are flipped for OPENCV
#define GREEN (Scalar(0, 255, 0))
#define RED (Scalar(0, 0, 255))
#define YELLOW (Scalar(0, 255, 255))
#define PURPLE (Scalar(255, 0, 255))
#define WHITE (Scalar(255, 255, 255))
#define BLACK (Scalar(0, 0, 0))

#endif /* DEFS_HPP */
