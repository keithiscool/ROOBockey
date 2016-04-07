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

#define bool _Bool //I had to use booleans ("bool"), but Linux uses "_Bool" for boolean variables
#define JOY_DEV "/dev/input/js0" //Define the device that the controller data is pulled from

void *calloc(size_t nitems, size_t size);


int main() {
        //int joy_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0, x;
        int joy_fd, num_of_axis=0, num_of_buttons=0, x;
	//char *button=NULL, name_of_joystick[80];
	//bool *button=NULL;
	char name_of_joystick[80];
        struct js_event js;

	//Declare all buttons (including select,start along with leftstick & rightstick presses
	bool Ba=0,Bb=0,Bx=0,By=0,BlBump=0,BrBump=0,Bsel=0,Bstart=0,BlStick=0,BrStick=0,BxboxCenterIcon=0;

	//Declare all joysticks (16 bit signed integers)
	int Lx,Ly,Rx,Ry,Lt,Rt;

	int shootPin = 18;
	pinMode(shootPin, OUTPUT);
	wiringPiSetupGpio();

        if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 ) {
                printf( "Couldn't open joystick\n" );
                return -1;
        }

        ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
        ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
        ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

        //axis = (int *) calloc( num_of_axis, sizeof( int ) );
        //button = (char *) calloc( num_of_buttons, sizeof( char ) );

	int axis[6];
	bool button[11];	

        printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
                , name_of_joystick
                , num_of_axis
                , num_of_buttons );

        fcntl( joy_fd, F_SETFL, O_NONBLOCK );   /* use non-blocking mode */


	int i = 0;
	bool goodData = 0;


        while( 1 ) {


                        /* read the joystick state */
                read(joy_fd, &js, sizeof(struct js_event));

                        /* see what to do with the event */
                switch (js.type & ~JS_EVENT_INIT) {

			if(goodData == 0) {
				for(i = 0; i < sizeof(axis); i++) {
					axis[i] = 0;
				}
			
				for(i = 0; i < sizeof(button); i++) {
					button[i] = 0;
				}
				js.value = 0;
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
			if(num_of_axis > 2) Lt = axis[2];
			if(num_of_axis > 3) {
				Rx = axis[3];
				Ry = -axis[4];
			}
			if( num_of_axis > 4 ) Rt = axis[5];

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
			
			if(!BxboxCenterIcon) { //if center button is pressed, dont do anything
				if(!Ba && !Bb && !Bx && !By) {
					if(!BlBump && !BrBump) {
						goodData = 1;
					}
				}
			}else {
				goodData = 0;
			}

			if(goodData == 1) {

				while(BxboxCenterIcon); //if center button is pressed, dont do anything
		
				if(Ba == 1) {
					digitalWrite(shootPin, HIGH);
				}
		
				if(Ba == 0) {	
					digitalWrite(shootPin, LOW);
				}

				usleep(10000); //scale input to milliseconds (10 milliseconds)
				printf("\r\n%d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d: ",Ba,Bb,Bx,By,BlBump,BrBump,Bsel,Bstart,BlStick,BxboxCenterIcon,BrStick);
				printf("\r\n%d, %d, %d, %d, %d, %d",Lx,Ly,Lt,Rx,Ry,Rt);
		
		                printf("  \r\n");
		                fflush(stdout);
			}

			usleep(2000000); //wait 2 seconds (in microseconds)
		}
        }

        close( joy_fd );        /* too bad we never get here */
        return 0;
}
