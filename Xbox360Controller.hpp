#ifndef XBOX360CONTROLLER_HPP
#define XBOX360CONTROLLER_HPP


#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>


#ifdef GPIO_UART_CPP

/*GLOBAL VARIABLES TO SHARE WITH GPIO_UART.CPP*/
//flag used to eliminate random noise from when wireless Xbox360 controller connects with all values @ 1
int goodData = 0;
//user needs the breakbeam sensor to operate the puck launcher
int shootPermissive = 0;
//UART port ID for Tx to motor controller
int UART_ID = 0;

#endif //GPIO_UART_CPP



#ifdef XBOX360CONTROLLER


/*GLOBAL VARIABLES TO SHARE WITH GPIO_UART.CPP*/
//flag used to eliminate random noise from when wireless Xbox360 controller connects with all values @ 1
int goodData = 0;
//user needs the breakbeam sensor to operate the puck launcher
int shootPermissive = 0;
//UART port ID for Tx to motor controller
int UART_ID = 0;


//Discrete Inputs/Outputs:
//NOTE: THESE USE BROADCOM NUMBERS SINCE WiringPi DID NOT MAP THEM CORRECTLY
//NOTE: Output at end of variable means "real-world output" 
extern int breakBeamInput;						//GPIO pin 17 input from break beam (garage-door-like sensor)
extern int shutdownPiSwitchInput;				//GPIO pin 27 input to run script to nicely power off RPi2 PowerLED

//Input at end of variable means "real-world input"
extern int breakBeamLEDOutput;					//GPIO pin 2 output a test output for the Break Beam
extern int shootPinOutput;						//GPIO pin 18 output controls the solenoid discrete output
extern int controllerConnectedLEDOutput;		//GPIO pin 18 output controls the solenoid discrete output

#endif //XBOX360CONTROLLER


#ifdef MAIN_CPP

//Function Declarations
int initController(void);
int parseXbox360Controller(void);
extern void sendMotorControllerSpeedBytes(int UART_PORT_ID, int LeftYvalueControllerInput, int RightYvalueControllerInput);

#endif //MAIN_CPP



#ifndef MAIN_CPP


#ifdef XBOX360CONTROLLER

//Function Declarations
int initController(void);
int parseXbox360Controller(void);
extern void sendMotorControllerSpeedBytes(int UART_PORT_ID, int LeftYvalueControllerInput, int RightYvalueControllerInput);

//Joystick Interfacing with Linux Event File js0
int joy_fd, num_of_axis = 0, num_of_buttons = 0, x;
char name_of_joystick[80];
struct js_event js; //Raw input from controller event


//These are the raw input values from the controller (copied from "struct js_event js")
int axis[6];
bool button[11];


//THESE ARE THE PROTECTED CONTROLLER VALUES
//THESE VARIABLES ARE THE FINAL OUTPUT VALUES THAT CAN BE USED FOR THE CONTROLLER
//THIS WAS IMPLEMENTED BECAUSE THE ***CONTROLLER INPUTS ALL 1's WHEN IT CONNECTS***
//TO GET PAST THIS ISSUE, THE CODE WAITS UNTIL THE BUTTONS RETURN TO A ZERO STATE BEFORE IT CONTINUES
//Declare all buttons (including select,start along with leftstick & rightstick presses
bool Ba = 0, Bb = 0, Bx = 0, By = 0, BlBump = 0, BrBump = 0, Bsel = 0, Bstart = 0, BlStick = 0, BrStick = 0, BxboxCenterIcon = 0;

//Declare all joysticks (16 bit signed integers)
int Lx, Ly, Rx, Ry, Lt, Rt;

#endif //XBOX360CONTROLLER


#endif //MAIN_CPP


#endif /* XBOX360CONTROLLER_HPP */