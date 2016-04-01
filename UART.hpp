#ifndef UART_HPP
#define UART_HPP

int uart0_filestream = 0;
//static unsigned char tx_buffer[200];
//static unsigned char *p_tx_buffer;
unsigned char tx_buffer[200];
unsigned char *p_tx_buffer;

int LeftMotorSerialOutput = 0;
int RightMotorSerialOutput = 0;


void sendMotorControllerSpeedByte(long LeftControllerInput, long RightControllerInput);
void initUart(void);
void putCharRaspberryPi2(int *p_tx_buffer);


#endif /* UART_HPP */