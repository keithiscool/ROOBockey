#ifndef UART_HPP
#define UART_HPP


#ifdef GPIO_UART_CPP

#include "defs.hpp"

#endif //GPIO_UART_CPP


#ifdef GPIO_UART_CPP

	//flag used to eliminate random noise from when wireless Xbox360 controller connects with all values @ 1
	extern int goodData;
	//user needs the breakbeam sensor to operate the puck launcher
	extern int shootPermissive;
	//UART port ID for Tx to motor controller
	extern int UART_ID;
	
	
	//Discrete Inputs/Outputs:
	//NOTE: THESE USE BROADCOM NUMBERS SINCE WiringPi DID NOT MAP THEM CORRECTLY
	//NOTE: Output at end of variable means "real-world output" 
	extern int breakBeamInput;						//GPIO pin 17 input from break beam (garage-door-like sensor)
	extern int shutdownPiSwitchInput;				//GPIO pin 27 input to run script to nicely power off RPi2 PowerLED
	
	//Input at end of variable means "real-world input"
	extern int breakBeamLEDOutput;					//GPIO pin 2 output a test output for the Break Beam
	extern int shootPinOutput;						//GPIO pin 18 output controls the solenoid discrete output
	extern int controllerConnectedLEDOutput;		//GPIO pin 18 output controls the solenoid discrete output

#endif //GPIO_UART_CPP


#ifdef MAIN_CPP

	int gpioPinOperations(void);
	int initGPIO_Uart(void);

#endif //MAIN_CPP

/////////////////////////////////////////////////


#ifdef XBOX360CONTROLLER

	void sendMotorControllerSpeedBytes(int UART_PORT_ID, int LeftYvalueControllerInput, int RightYvalueControllerInput);
//	int initGPIO_Uart(void);

#endif //XBOX360CONTROLLER



#endif /* UART_HPP */