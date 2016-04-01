#ifndef XBOX360CONTROLLER_HPP
#define XBOX360CONTROLLER_HPP


#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <wiringPi.h> //Utilize the "WiringPi GPIO library"

//Function Declarations
int initController(void);
void parseXbox360Controller(void);
extern void sendMotorControllerSpeedByte(long LeftControllerInput, long RightControllerInput);


//int joy_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0, x;
int joy_fd, num_of_axis = 0, num_of_buttons = 0, x;
//char *button=NULL, name_of_joystick[80];
//bool *button=NULL;
char name_of_joystick[80];
struct js_event js; //Raw input from controller event


//Discrete Inputs/Outputs:
int breakBeam = 17;		//GPIO pin 17 input from break beam (garage-door-like sensor)
int powerOffPi = 21;	//GPIO pin 27 input to run script to nicely power off RPi2 PowerLED
int breakBeamLED = 2;	//GPIO pin 2 output a test output for the Break Beam
int shootPin = 18;		//GPIO pin 18 output controls the solenoid discrete output


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



#endif /* OBJECTTRACKING_HPP */