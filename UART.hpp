#ifndef UART_HPP
#define UART_HPP


#ifdef USE_EXTERNAL_FUNCTIONS

void initUart(void);

#endif //USE_EXTERNAL_FUNCTIONS


#ifdef OnlyUARTFunctions

void sendMotorControllerSpeedByte(long LeftControllerInput, long RightControllerInput);

#endif //OnlyUARTFunctions



#ifndef USE_EXTERNAL_FUNCTIONS
#ifndef OnlyUARTFunctions

void sendMotorControllerSpeedByte(long LeftControllerInput, long RightControllerInput);
void initUart(void);
void putCharRaspberryPi2(int *p_tx_buffer);

int uart0_filestream = 0;
//static unsigned char tx_buffer[200];
//static unsigned char *p_tx_buffer;
unsigned char tx_buffer[200];
unsigned char *p_tx_buffer;

int LeftMotorSerialOutput = 0;
int RightMotorSerialOutput = 0;

#endif //OnlyUARTFunctions
#endif //USE_EXTERNAL_FUNCTIONS


#endif /* UART_HPP */