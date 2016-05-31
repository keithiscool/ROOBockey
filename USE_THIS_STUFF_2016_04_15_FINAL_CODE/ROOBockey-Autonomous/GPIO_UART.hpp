#ifndef UART_HPP
#define UART_HPP
	

int gpioPinOperations(void);
int initGPIO_Uart(void);
void sendMotorControllerSpeedBytes(int UART_PORT_ID, int LeftYvalueControllerInput, int RightYvalueControllerInput);

#endif //UART_HPP