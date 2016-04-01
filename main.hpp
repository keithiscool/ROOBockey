#ifndef UART_HPP
#define UART_HPP

#define USE_EXTERNAL_FUNCTIONS 1 //only pull the functions from the respective files (exclude local global variables for each cpp-file)

#include "UART.hpp"
#include "Xbox360Controller.hpp"

extern void initUart(void);
extern int initController(void);
extern void parseXbox360Controller(void);

#endif /* UART_HPP */