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
//#if defined(__linux__)
#ifdef RaspberryPi2Used


#include "Xbox360Controller.hpp"
#define OnlyUARTFunctions 1
#include "UART.hpp"

//#define bool _Bool //I had to use booleans ("bool"), but Linux uses "_Bool" for boolean variables
#define JOY_DEV "/dev/input/js0" //Define the device that the controller data is pulled from


void *calloc(size_t nitems, size_t size);


int initController(void) {

	//Initialize the Configuration the desired GPIO pin (set pin to low state as first value)
	system("gpio export 18 low");
	system("gpio export 17 low");
	system("gpio export 2 low");
	system("gpio export 21 low");
	//Initialize WiringPi -- using Broadcom processor pin numbers
	wiringPiSetupGpio();
	//Initialize the Wiring Pi Libary
	pinMode(breakBeam, INPUT);
	pinMode(powerOffPi, INPUT);
	pinMode(breakBeamLED, OUTPUT);
	pinMode(shootPin, OUTPUT);
	pullUpDnControl(breakBeam, PUD_UP); // Enable pull-up resistor on button
	pullUpDnControl(powerOffPi, PUD_UP); // Enable pull-up resistor on button


	if ((joy_fd = open(JOY_DEV, O_RDONLY)) == -1) {
		printf("Couldn't open joystick\n");
		return (-1);
	}

	ioctl(joy_fd, JSIOCGAXES, &num_of_axis);
	ioctl(joy_fd, JSIOCGBUTTONS, &num_of_buttons);
	ioctl(joy_fd, JSIOCGNAME(80), &name_of_joystick);

	//axis = (int *) calloc( num_of_axis, sizeof( int ) );
	//button = (char *) calloc( num_of_buttons, sizeof( char ) );

	printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
		, name_of_joystick
		, num_of_axis
		, num_of_buttons);

	fcntl(joy_fd, F_SETFL, O_NONBLOCK);   /* use non-blocking mode */

	return 1;
}



void parseXbox360Controller(void) {
	int i = 0;
	bool goodData = 0;

	/* read the joystick state */
	read(joy_fd, &js, sizeof(struct js_event));

	/* see what to do with the event */
	switch (js.type & ~JS_EVENT_INIT) {

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

		case JS_EVENT_AXIS:
			axis[js.number] = js.value;
			break;
		case JS_EVENT_BUTTON:
			button[js.number] = js.value;
			break;
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
			if (!BlBump && !BrBump) {
				goodData = 1;
			}
		}
	}else {
		goodData = 0;
	}



	if (goodData == 1) {

#ifdef SOFTWARE_EMERGENCY_STOP
		while (BxboxCenterIcon); //if center button is pressed, dont do anything
#endif

		if (Ba == 1) {
			digitalWrite(shootPin, HIGH);
		}

		if (Ba == 0) {
			digitalWrite(shootPin, LOW);
		}
		
		if (digitalRead(breakBeam)) {
			digitalWrite(breakBeamLED, HIGH);
		}
		else {
			digitalWrite(breakBeamLED, LOW);
		}
		if (digitalRead(powerOffPi)) {
			system("sudo shutdown - h now");
		}
		


#ifdef PRINT_CONTROLLER_DEBUG_DATA
		printf("\r\n%d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d: ", Ba, Bb, Bx, By, BlBump, BrBump, Bsel, Bstart, BlStick, BxboxCenterIcon, BrStick);
		printf("\r\n%d, %d, %d, %d, %d, %d", Lx, Ly, Lt, Rx, Ry, Rt);
#endif
			

		if (((abs(Ly)) >= JOYSTICK_DEADZONE) || ((abs(Ry)) >= JOYSTICK_DEADZONE)) {
			sendMotorControllerSpeedByte(Ly, Ry); //send left and right joystick scaled values to Sabertooth 2x25 motor controller using UART
			usleep(10000); //delay using scaled input in microseconds (10 milliseconds)
		}

		printf("  \r\n");
		fflush(stdout);
	}

	//In case the user wants to turn off the controller events:
	//close(joy_fd);        /* too bad we never get here */
}


#endif