#ifndef UART_HPP
#define UART_HPP

#define USE_EXTERNAL_FUNCTIONS_IN_MAIN 1 //only pull the functions from the respective files (exclude local global variables for each cpp-file)

#include "GPIO_UART.hpp"
#include "Xbox360Controller.hpp"

extern int initController(void);
extern void parseXbox360Controller(void);
extern void imageProcessingRoutine(void);

extern int gpioPinOperations(void);
extern int initGPIO_Uart(void);

#endif /* UART_HPP */