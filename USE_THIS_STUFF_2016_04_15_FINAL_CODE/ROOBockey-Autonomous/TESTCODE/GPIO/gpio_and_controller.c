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

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#include <wiringPi.h>
//#include <wiringSerial.h>
#include <errno.h>


#define bool _Bool //I had to use booleans ("bool"), but Linux uses "_Bool" for boolean variables
#define JOY_DEV "/dev/input/js0" //Define the device that the controller data is pulled from


#define PRINT_CONTROLLER_DATA 1
#define PRINT_SERIAL_DATA 1



//Feed Xbox controller Joystick (16-bit integer using built-in xpad driver in Linux) as an input
//and send the scaled output to the "Sabertooth 2x25 Motor Controller V2.00" as a Simplified Serial Input
void sendMotorControllerSpeedBytes(int UART_PORT_ID, int LeftYvalueControllerInput, int RightYvalueControllerInput) {
	//Left Motor:		0: Full reverse			64: Neutral		127: Full Forward
	//Right Motor:		128: Full reverse		192: Neutral		255: Full Forward

	static int maxControllerJoystickInput = 32767;
	static int DEADZONE = 16384; //uses only half of the joystick range as usable values (32768/2 == 2^14 == 16384)
	unsigned char RightMotorSerialOutput=0, LeftMotorSerialOutput=0;
	static int Ldelta = 127-63; //will have actual slope of ((127-64)/(2^14)) or ((127-64)>>14), but that will be later using fixed point and bit shifting
	static int Rdelta = 255-191; //will have actual slope of ((255-192)/(2^14)) or ((255-192)>>14), but that will be later using fixed point and bit shifting
	static int nextMilliSecondCountLeftUART = 0, nextMilliSecondCountRightUART = 0;

	//check if the controller is outside the y-axis dead zone for each joystick
	if(abs(LeftYvalueControllerInput) > DEADZONE) {

		bool negLeftInput = 0;

		if(LeftYvalueControllerInput < 0) { //set negative input flag and convert to positive value
			LeftYvalueControllerInput = (unsigned int)LeftYvalueControllerInput; //make joystick positive
			negLeftInput = 1;
		}
		//input is positive
		if(negLeftInput == 0) { //input is positive
			LeftMotorSerialOutput = (unsigned char)(64 + (((LeftYvalueControllerInput-DEADZONE)*Ldelta)>>14)); //64 is motr controller offset for Left Motor Neutral
		}
		//input is negative
		if(negLeftInput == 1) { //input is negative
			LeftMotorSerialOutput = (unsigned char)(-(64 - (((LeftYvalueControllerInput-DEADZONE)*Ldelta)>>14))); //64 is motr controller offset for Left Motor Neutral
		}

		//clip the maximum allowed magnitude for y-axis joystick at their expected maximum values
  		if (LeftMotorSerialOutput > 126) LeftMotorSerialOutput = 127;

		//clip the minimum allowed magnitude for y-axis joystick at their expected minimum values
  		if (LeftMotorSerialOutput < 2) LeftMotorSerialOutput = 1;
	}else {
		LeftMotorSerialOutput = 64;
	}

	//check if the controller is outside the y-axis dead zone for each joystick
	if(abs(RightYvalueControllerInput) > DEADZONE) {

		bool negRightInput = 0;

		if(RightYvalueControllerInput < 0) { //set negative input flag and convert to positive value
			RightYvalueControllerInput = (unsigned int)RightYvalueControllerInput; //make joystick positive
			negRightInput = 1;
		}

		if(negRightInput == 0) { //input is positive
			RightMotorSerialOutput = (unsigned char)(192 + (((RightYvalueControllerInput-DEADZONE)*Rdelta)>>14)); //192 is motr controller offset for Right Motor Neutral
		}

		if(negRightInput == 1) { //input is negative
			RightMotorSerialOutput = (unsigned char)(-(192 - (((RightYvalueControllerInput-DEADZONE)*Rdelta)>>14))); //192 is motr controller offset for Right Motor Neutral
		}

		//clip the maximum allowed magnitude for y-axis joystick at their expected maximum values
  		if (RightMotorSerialOutput > 254) RightMotorSerialOutput = 255;

		//clip the minimum allowed magnitude for y-axis joystick at their expected minimum values
  		if (RightMotorSerialOutput < 129) RightMotorSerialOutput = 128;
	}else {
		RightMotorSerialOutput = 192;
	}

#ifdef PRINT_SERIAL_DATA
	printf("Serial_L:%d\r\n",LeftMotorSerialOutput);
	printf("Serial_R:%d\r\n",RightMotorSerialOutput);
#endif //PRINT_SERIAL_DATA	

	if (millis () > nextMilliSecondCountLeftUART) {
	      serialPutchar(UART_PORT_ID,LeftMotorSerialOutput);
	      nextMilliSecondCountLeftUART += 150;
	}

	if (millis () > nextMilliSecondCountRightUART) {
	      serialPutchar(UART_PORT_ID,RightMotorSerialOutput);
	      nextMilliSecondCountRightUART += 100;
	}
}



int main() {

	int joy_fd, num_of_axis=0, num_of_buttons=0, x;
	char name_of_joystick[80];
	struct js_event js;

	//Declare all buttons (including select,start along with leftstick & rightstick presses
	bool Ba=0,Bb=0,Bx=0,By=0,BlBump=0,BrBump=0,Bsel=0,Bstart=0,BlStick=0,BrStick=0,BxboxCenterIcon=0;

	//Declare all joysticks (16 bit signed integers)
	int Lx=0,Ly=0,Rx=0,Ry=0,Lt=0,Rt=0;

	//Discrete Inputs & Outputs:
//	int breakBeam = 27; //actual GPIO pin number
	int breakBeam = 2;  //broadcom pin number from wiringSerial.h
	system("gpio mode 2 in"); //force shootPin to input, configure pin using terminal command
	int shutdownPiSwitch = 21;
//	int shootPin = 18; //actual GPIO pin number
	int shootPin = 1;  //broadcom pin number from wiringSerial.h
	system("gpio mode 1 out"); //force shootPin to output, configure pin using terminal command
	int breakBeamLED = 2;
	int controllerConnectedLED = 3;

	int shootPermissive = 0;
	int launchedPuck = 0;

	//Inputs are active low
	pinMode(breakBeam, INPUT);
//	pullUpDnControl(breakBeam, PUD_UP);
	pullUpDnControl(breakBeam, PUD_DOWN);
	pinMode(shutdownPiSwitch, INPUT);
	pullUpDnControl(shutdownPiSwitch, PUD_UP);

	pinMode(shootPin, OUTPUT);
	pinMode(breakBeamLED, OUTPUT);
	pinMode(controllerConnectedLED, OUTPUT);
	//pinMode(beaconBlueRectangleLED, OUTPUT);
	//pinMode(beaconGreenTriangleLED, OUTPUT);
	//pinMode(beaconRedOctagonLED, OUTPUT);
	wiringPiSetupGpio();

	int nextMilliSecondCountLEDS = 0;
	int nextMilliSecondCountGPIO = 0;
	int ShutdownCount = 0;
	int nextMilliSecondCountBreakBeam = 0;

	int firstBreakBeamValue = 0;
	int lastBreakBeamValue = 0;

	int axis[6];
	bool button[11];

	int i = 0;
	bool goodData = 0;
	
	int UART_ID=0;
	
	usleep(2000000); //wait 2 seconds (in microseconds) to act as a power up delay for the Sabertooth Motor Controller
	serialPutchar(UART_ID,0xAA); //Send the autobauding character to Sabertooth first!
	usleep(100000); //wait 100ms (in microseconds) before commanding motors
	

	//if ((UART_ID = serialOpen ("/dev/ttyAMA0", 115200)) < 0) {
	if ((UART_ID = serialOpen ("/dev/ttyAMA0", 19200)) < 0) {
	    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
	    return 1 ;
	}
	

	if (wiringPiSetup () == -1) {
	    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
	    return 1 ;
	}


	if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 ) {
			printf( "Couldn't open joystick\n" );
			return -1;
	}

	ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
	ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
	ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );


	printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
			, name_of_joystick
			, num_of_axis
			, num_of_buttons );


	fcntl( joy_fd, F_SETFL, O_NONBLOCK );   /* use non-blocking mode */
	
	printf("BEGIN WHILE LOOP\r\n");

	//Initialize break beam value to detect change in state later
	firstBreakBeamValue = digitalRead(breakBeam); //active low signal

	
//you can use this code to locate the pin (broadcom GPIO pins work with WiringPi, not the GPIO pinout diagram)
/*Test to check GPIO pin numbers
	for(i=0; i<50; i++){
		printf("%d\r\n");
		digitalWrite(i, HIGH);
		usleep(10000000);
	}

//	while(1);

	for(i=0; i<50; i++){
		pullUpDnControl(i, PUD_DOWN);
	}	

	for(i=0; i<50; i++){
		if(digitalRead(i) == 1){
			printf("%d\r\n",i);}
		usleep(1000000);
	}
*/	

	while( 1 ) {

	
		/* read the joystick state */
		read(joy_fd, &js, sizeof(struct js_event));

		/* see what to do with the event */
		switch (js.type & ~JS_EVENT_INIT) {

		/*Check if the data coming in is "Good Data"*/
		if(goodData == 0) {
			for(i = 0; i < sizeof(axis); i++) { //clear all joystick data when data flag shows bad data
				axis[i] = 0;
			}
		
			for(i = 0; i < sizeof(button); i++) { //clear all button data when data flag shows bad data
				button[i] = 0;
			}
			js.value = 0; //clear input data from the input struct that reads the joystick "js0 event file"
		}

		case JS_EVENT_AXIS:
			axis   [ js.number ] = js.value;
			break;
		case JS_EVENT_BUTTON:
			button [ js.number ] = js.value;
			break;
		}

		//Assign Variables
		Lx = axis[0];
		Ly = -axis[1];
		if(num_of_axis > 2) 
			Lt = axis[2];
		if(num_of_axis > 3) {
			Rx = axis[3];
			Ry = -axis[4];
		}
		if( num_of_axis > 4 ) 
			Rt = axis[5];

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
		
		if(!BxboxCenterIcon) { //if any buttons are pressed, dont do anything
			if(!Bstart && !Bsel && !BlStick && !BrStick) {
				if(!BlBump && !BrBump) {
					goodData = 1;
				}
			}
		}
		if(BxboxCenterIcon) { //if any buttons are pressed, dont do anything
			if(Bstart || Bsel || BlStick || BrStick) {
				if(BlBump) {
					goodData = 0;
				}
			}
		}
		if(millis() > nextMilliSecondCountBreakBeam) {
			
			if(goodData == 1) {
				digitalWrite(controllerConnectedLED, HIGH);
				
				//BrBump is override for shooting permissive
				if((BrBump) || (shootPermissive)) {
					if(Ba == 1) {
						digitalWrite(shootPin, HIGH);
						launchedPuck++;
						printf("launchedPuck %d times\r\n",launchedPuck);
						shootPermissive = 0;
					}
				}
		
				if(Ba == 0) {	
					digitalWrite(shootPin, LOW);
				}
	
	#ifdef PRINT_CONTROLLER_DATA
				printf("\r\n%d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d: ",Ba,Bb,Bx,By,BlBump,BrBump,Bsel,Bstart,BlStick,BxboxCenterIcon,BrStick);
				printf("\r\n%d, %d, %d, %d, %d, %d",Lx,Ly,Lt,Rx,Ry,Rt);
				printf("\r\n");
				fflush(stdout);
	#endif //PRINT_CONTROLLER_DATA
			
			sendMotorControllerSpeedBytes(UART_ID,Ly,Ry);

			}

			nextMilliSecondCountBreakBeam += 150;

		}
		
		/*Now read the input GPIO pins for control necessary pins that need a higher update rate*/
		if (millis() > nextMilliSecondCountLEDS) {

			static int timeThroughIFstatement = 0;

			//goodData not set, so controller not connected
	      		if(goodData == 0) {
				printf("BadData\r\n");
				digitalWrite(controllerConnectedLED, LOW);
			}

			lastBreakBeamValue = digitalRead(breakBeam); //active low signal

			if(firstBreakBeamValue != lastBreakBeamValue) { 
				printf("BreakBeam ON\r\n");
				digitalWrite(breakBeamLED, HIGH);
				shootPermissive = 1;
			}else {
				digitalWrite(breakBeamLED, LOW);
				printf("BreakBeam OFF\r\n");
			}
			nextMilliSecondCountLEDS += 150 ;
		}
		

		/*Now read the input GPIO pins for things that dont need a high update rate*/
		if(millis () > nextMilliSecondCountGPIO) {
			if(digitalRead(shutdownPiSwitch) == 0) {
				//if shutdown counter reaches 5 seconds, shutdown the Pi
				if(ShutdownCount > 5) {
					system("sudo shutdown -k now"); //send test shutdown message
					//system("sudo shutdown -P now"); //shutdown Pi and power off immediately
				}
				ShutdownCount++;
			}else {
				ShutdownCount = 0; //reset shutdown counter
			}
			
			nextMilliSecondCountGPIO += 1000; //check every 1000ms (1 time every second)
		}
	
		//usleep(2000); //wait 2 milliseconds (in microseconds)
	
	}
	return 0;
}