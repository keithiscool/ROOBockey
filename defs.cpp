#include "defs.hpp"

/* shared/global variables */
//int this_is_global;


//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*GLOBAL VARIABLES TO SHARE WITH GPIO_UART.CPP*/
//flag used to eliminate random noise from when wireless Xbox360 controller connects with all values @ 1
int goodData = 0;
//user needs the breakbeam sensor to operate the puck launcher
int shootPermissive = 0;
//UART port ID for Tx to motor controller
int UART_ID = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//PIN ASSIGNMENTS -- Discrete Inputs/Outputs:
//NOTE: THESE USE BROADCOM NUMBERS SINCE WiringPi DID NOT MAP THEM CORRECTLY
//NOTE: Output at end of variable means "real-world output"
//PLEASE LOOK HERE FOR CORRECT PINOUT DIAGRAM: 
//http://wiringpi.com/pins/

int breakBeamInput = 2;							//GPIO pin 27 input from break beam (garage-door-like sensor)
int shutdownPiSwitchInput = 5;					//GPIO pin 24 input to run script to nicely power off RPi2 PowerLED

//Input at end of variable means "real-world input"
int breakBeamLEDOutput = 0;						//GPIO pin 17 output a test output for the Break Beam
int shootPinOutput = 1;							//GPIO pin 18 output controls the solenoid discrete output
int controllerConnectedLEDOutput = 3;			//GPIO pin 22 output controls the solenoid discrete output
int enableAndGateOutput = 23;					//GPIO pin 16 output controls the output enable discrete output

///////////////////////////////////////////////////////////////////////////////////////////////////////////


//default display capture window frame width and height (640x480 window)
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;