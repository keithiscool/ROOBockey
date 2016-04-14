#ifndef XBOX360CONTROLLER_HPP
#define XBOX360CONTROLLER_HPP


#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>


//Function Declarations
int initController(void);
int parseXbox360Controller(void);
extern void sendMotorControllerSpeedBytes(int UART_PORT_ID, int LeftYvalueControllerInput, int RightYvalueControllerInput);


//THESE ARE THE PROTECTED CONTROLLER VALUES
//THESE VARIABLES ARE THE FINAL OUTPUT VALUES THAT CAN BE USED FOR THE CONTROLLER
//THIS WAS IMPLEMENTED BECAUSE THE ***CONTROLLER INPUTS ALL 1's WHEN IT CONNECTS***
//TO GET PAST THIS ISSUE, THE CODE WAITS UNTIL THE BUTTONS RETURN TO A ZERO STATE BEFORE IT CONTINUES
//Declare all buttons (including select,start along with leftstick & rightstick presses
extern bool Ba, Bb, Bx, By, BlBump, BrBump, Bsel, Bstart, BlStick, BrStick, BxboxCenterIcon;

//Declare all joysticks (16 bit signed integers)
extern int Lx, Ly, Rx, Ry, Lt, Rt;


endif //XBOX360CONTROLLER_HPP
