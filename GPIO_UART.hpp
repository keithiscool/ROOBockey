#ifndef UART_HPP
#define UART_HPP



//flag used to eliminate random noise from when wireless Xbox360 controller connects with all values @ 1
extern short goodData = 0;
//user needs the breakbeam sensor to operate the puck launcher
extern short shootPermissive = 0;
//UART port ID for Tx to motor controller
extern int UART_ID = 0;



#ifndef USE_EXTERNAL_FUNCTIONS_IN_MAIN

	int gpioPinOperations(void);
	int initGPIO_Uart(void);

#endif //USE_EXTERNAL_FUNCTIONS_IN_MAIN

/////////////////////////////////////////////////

#ifndef USE_EXTERNAL_FUNCTIONS_IN_MAIN

	void sendMotorControllerSpeedBytes(int UART_PORT_ID, int LeftYvalueControllerInput, int RightYvalueControllerInput);
	void initUart(void);

#endif //USE_EXTERNAL_FUNCTIONS_IN_MAIN



#endif /* UART_HPP */