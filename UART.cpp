////// File for using UART TX on pin 8 (ground on pin 6) for Raspberry Pi 2 Model B
////// Reference Code: http://www.raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart
//NOTE: THE USER MUST GRANT PERMISSIONS FOR THE UART TO BE USED ON THE RPi 
//To do this, type: "sudo chmod a+rw /dev/ttyAMA0" in a terminal window
//
//
//ifdef RaspberryPi2Used{
//
//	#include <stdio.h>
//	#include <unistd.h>			//Used for UART
//	#include <fcntl.h>			//Used for UART
//	#include <termios.h>		//Used for UART
//	#include "UART.h"
//	#include "defs.hpp"
//
//
//	void setupUart(void) {
//		//-------------------------
//		//----- SETUP USART 0 -----
//		//-------------------------
//		//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
//		int uart0_filestream = -1;
//
//		//OPEN THE UART
//		//The flags (defined in fcntl.h):
//		//	Access modes (use 1 of these):
//		//		O_RDONLY - Open for reading only.
//		//		O_RDWR - Open for reading and writing.
//		//		O_WRONLY - Open for writing only.
//		//
//		//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
//		//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
//		//											immediately with a failure status if the output can't be written immediately.
//		//
//		//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
//		uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
//		if (uart0_filestream == -1) {
//			//ERROR - CAN'T OPEN SERIAL PORT
//			printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
//		}
//
//		//CONFIGURE THE UART
//		//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
//		//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
//		//	CSIZE:- CS5, CS6, CS7, CS8
//		//	CLOCAL - Ignore modem status lines
//		//	CREAD - Enable receiver
//		//	IGNPAR = Ignore characters with parity errors
//		//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
//		//	PARENB - Parity enable
//		//	PARODD - Odd parity (else even)
//		struct termios options;
//		tcgetattr(uart0_filestream, &options);
//		options.c_cflag = B19200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
//		options.c_iflag = IGNPAR;
//		options.c_oflag = 0;
//		options.c_lflag = 0;
//		tcflush(uart0_filestream, TCIFLUSH);
//		tcsetattr(uart0_filestream, TCSANOW, &options);
//	}
//
//
//
//	//Use Simplified serial to communicate with "Sabertooth 2x25 Motor Controller V2.00"
//	// Motor Left:	Full Forward==127	/ Neutral==64	/ Full Reverse==0
//	// Motor Right: Full Forward==255	/ Neutral==128	/ Full Reverse==128
//	void putCharRaspberryPi2(int *p_tx_buffer) {
//		//----- TX BYTES -----
//
//		if (uart0_filestream != -1)
//		{
//			int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));		//Filestream, bytes to write, number of bytes to write
//			if (count < 0)
//			{
//				printf("UART TX error\n");
//			}
//		}
//
//
//
//
//		//Feed Xbox controller Joystick (16-bit integer using XboxDRV driver in Linux) as an input
//		//and send the scaled output to the "Sabertooth 2x25 Motor Controller V2.00" as a Simplified Serial Input
//		void sendMotorControllerSpeedByte(int &LeftControllerInput, int &RightControllerInput) {
//
//			int LeftMotorSerialOutput = (64 + (LeftControllerInput*(63 / 32768))); //64 is motr controller offset for Left Motor Neutral
//			int LeftMotorSerialOutput = (192 + (RightControllerInput*(63 / 32768))); //192 is motr controller offset for Right Motor Neutral
//			putCharRaspberryPi2(LeftMotorSerialOutput);
//			putCharRaspberryPi2(RightMotorSerialOutput);
//		}
//	}
//}