/* this is the linux 2.2.x way of handling joysticks. It allows an arbitrary
* number of axis and buttons. It's event driven, and has full signed int
* ranges of the axis (-32768 to 32767). It also lets you pull the joysticks
* name. The only place this works of that I know of is in the linux 1.x
* joystick driver, which is included in the linux 2.2.x kernels
*/

/* keithiscool update!!!
oh, by the way, keith is cool!
1) added functionality for xbox360 wireless controller
2) variables are assigned so this test code can be used by others
*/


/* Be sure to install the Wiring Pi library on the Raspberry pi
This code works on Raspberry Pi 2, but I am not sure about RPi1 or RPi3
Guide: http://wiringpi.com/download-and-install/
*/


#include "defs.hpp"
#include <wiringPi.h> //Utilize the "WiringPi GPIO library"
#include "Xbox360Controller.hpp"
#include "GPIO_UART.hpp"




//Joystick Interfacing with Linux Event File js0
int joy_fd, num_of_axis = 0, num_of_buttons = 0, x;
char name_of_joystick[80];
struct js_event js; //Raw input from controller event

//These are the raw input values from the controller (copied from "struct js_event js")
int axis[6];
bool button[11];


//#if defined(__linux__)


#define JOY_DEV "/dev/input/js0" //Define the device that the controller data is pulled from



int initController(void) {

	if ((joy_fd = open(JOY_DEV, O_RDONLY)) == -1) {
		printf("Couldn't open joystick\n");
		return (-1);
	}

	ioctl(joy_fd, JSIOCGAXES, &num_of_axis);
	ioctl(joy_fd, JSIOCGBUTTONS, &num_of_buttons);
	ioctl(joy_fd, JSIOCGNAME(80), &name_of_joystick);

	printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
		, name_of_joystick
		, num_of_axis
		, num_of_buttons);

	fcntl(joy_fd, F_SETFL, O_NONBLOCK);   /* use non-blocking mode */

	//Declare all buttons (including select,start along with leftstick & rightstick presses
	Ba = 0, Bb = 0, Bx = 0, By = 0, BlBump = 0, BrBump = 0, Bsel = 0, Bstart = 0, BlStick = 0, BrStick = 0, BxboxCenterIcon = 0;

	//Declare all joysticks (16 bit signed integers)
	Lx = 0, Ly = 0, Rx = 0, Ry = 0, Lt = 0, Rt = 0;


	return 1;
}



int parseXbox360Controller(void) {
	
	//index used for parsing the input wireless Xbox360 controller data from js0 event
	int i = 0;
	//used to run non-blocking delay for the GPIO pins
	static int nextMilliSecondCountGPIO = 0;
	static short launchedPuck = 0;
	//used to control the shutdown of the RPi2 using either GPIO or the Xbox360 controller
	static short shutdownCount = 0;

	/* read the joystick state */
	read(joy_fd, &js, sizeof(struct js_event));

	/* see what to do with the event */
	switch (js.type & ~JS_EVENT_INIT) {

		case JS_EVENT_AXIS:
			axis[js.number] = js.value;
			break;
		case JS_EVENT_BUTTON:
			button[js.number] = js.value;
			break;
	}


	//Check to see if the controller is spitting back useful data (if not, reset/ignore the incoming data)
	if (goodData == 0) {
		for (i = 0; i < sizeof(axis); i++) {
			axis[i] = 0;
		}
		for (i = 0; i < sizeof(button); i++) {
			button[i] = 0;
		}
		js.value = 0;
	}


	//Assign Variables
	Lx = axis[0];
	Ly = -axis[1];
	if (num_of_axis > 2) Lt = axis[2];
	if (num_of_axis > 3) {
		Rx = axis[3];
		Ry = -axis[4];
	}
	if (num_of_axis > 4) Rt = axis[5];

	Ba = button[0];
	Bb = button[1];
	Bx = button[2];
	By = button[3];
	BlBump = button[4];
	BrBump = button[5];
	Bsel = button[6];
	Bstart = button[7];
	BlStick = button[8];
	BxboxCenterIcon = button[9];
	BrStick = button[10];

	//Check to see if the buttons are in their neutral state
	if (!BxboxCenterIcon) { //if center button is pressed, dont do anything (center button is software E-Stop for robot)
		if (!Bstart && !Bsel && !BlStick && !BrStick) {
			if (!BlBump) {
				goodData = 1;
			}
		}
	}else {
		goodData = 0;
		return 0;
	}



	//check to see if the wireless Xbox360 controller is giving valid data and if so, start using the GPIO pins in the RPi2
	if (goodData == 1) {

#ifdef SOFTWARE_EMERGENCY_STOP
		while (BxboxCenterIcon == 1) { //if center button is pressed, dont do anything
			sendMotorControllerSpeedBytes(UART_ID, 64, 192); //halt motors
			digitalWrite(shootPinOutput, LOW);
			int breakBeamLEDOutput = 0;					//GPIO pin 17 output a test output for the Break Beam
			int shootPinOutput = 1;						//GPIO pin 18 output controls the solenoid discrete output
			int controllerConnectedLEDOutput = 3;				//GPIO pin 22 output controls the solenoid discrete output
			int enableAndGateOutput = 4;					//GPIO pin 23 output controls the solenoid discrete output
		}
#endif

		if (millis() > nextMilliSecondCountGPIO) {

			digitalWrite(controllerConnectedLEDOutput, HIGH);

			//BrBump is override for shooting permissive
			if ((BrBump) || (shootPermissive)) {
				if (Ba == 1) {
					digitalWrite(shootPinOutput, HIGH);
					launchedPuck++;
					printf("launchedPuck %d times\r\n", launchedPuck);
					shootPermissive = 0;
				}
			}

			nextMilliSecondCountGPIO += 300;
		}

		if (Ba == 0) {
			digitalWrite(shootPinOutput, LOW);
		}
		
		//Read active high input for breakBeam sensor (garage door obstruction sensor)
		if (digitalRead(breakBeamInput) == 1) {
			digitalWrite(breakBeamLEDOutput, HIGH);
		}
		else {
			digitalWrite(breakBeamLEDOutput, LOW);
		}

		//shutdown the RPi2 safely using either the controller or the controller buttons
		if ((digitalRead(shutdownPiSwitchInput) == 1) || (Ba && Bb && Bx && By)) {
			if (shutdownCount > 5) {
				system("sudo shutdown -P now");
			}
			shutdownCount++;
		}else {
			shutdownCount = 0; //reset shutdown counter
		}
		
		
		sendMotorControllerSpeedBytes(UART_ID, Ly, Ry); //send left and right joystick scaled values to Sabertooth 2x25 motor controller using UART


#ifdef PRINT_CONTROLLER_DEBUG_DATA
		printf("\r\n%d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d: ", Ba, Bb, Bx, By, BlBump, BrBump, Bsel, Bstart, BlStick, BxboxCenterIcon, BrStick);
		printf("\r\n%d, %d, %d, %d, %d, %d", Lx, Ly, Lt, Rx, Ry, Rt);
		printf("\r\n");
		fflush(stdout);
#endif //PRINT_CONTROLLER_DEBUG_DATA

		
		printf("  \r\n");
		fflush(stdout);

		return 1;
	}

	//In case the user wants to turn off the controller events by closing the controller event js0 file:
	//close(joy_fd);        /* too bad we never get here */
}

