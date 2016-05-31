// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
//Modified by Keith Martin

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


//Choose which color the beacon should be for the ROOBockey LED beacon:
//Comment out the colors that you do not want the beacon to be
#define GREEN_BEACON 1
//#define RED_BEACON 1
//#define BLUE_BEACON 1
//#define YELLOW_BEACON 1


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            11 //I chose to hook Data line for neopixels to pin "D11" with a 330 or 470 ohm resistor

// How many NeoPixels are attached to the Arduino?
//#define NUMPIXELS      10
#define NUMPIXELS      32

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    #ifdef GREEN_BEACON
      pixels.setPixelColor(i, pixels.Color(0,255,0)); // green color
    #endif
    
    #ifdef RED_BEACON
      pixels.setPixelColor(i, pixels.Color(255,0,0)); // red color
    #endif
    
    #ifdef BLUE_BEACON
      pixels.setPixelColor(i, pixels.Color(0,0,255)); // blue color
    #endif

    #ifdef YELLOW_BEACON
      pixels.setPixelColor(i, pixels.Color(255,255,0)); // yellow color
    #endif

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
