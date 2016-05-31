#define USE_EXTERNS
#include "Beacons.hpp"


/*Define Shapes and Colors for Known Target Beacons:
Green Triangle
Blue Rectangle
Red Octagon
*/


Beacon::~Beacon() { // I never have to free-up the class, so this is not run...
	// nothing here in the deconstructor
}

Beacon::Beacon(string name) { // this is the class declaration that I am actually using...
	if (name == "GreenTriangle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(35, 5, 233));
		setHSVmax(Scalar(54, 14, 237));

		setColor(GREEN);
		setShape("Triangle");
/*
MIN 'H' VALUE: 35
MAX 'H' VALUE: 54
MIN 'S' VALUE: 5
MAX 'S' VALUE: 14
MIN 'V' VALUE: 233
MAX 'V' VALUE: 237
*/
	}

	if (name == "BlueRectangle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(35, 5, 233));
		setHSVmax(Scalar(80, 21, 254));

		setColor(BLUE);
		setShape("Rectangle");
/*MIN 'H' VALUE: 35
MAX 'H' VALUE: 80
MIN 'S' VALUE: 5
MAX 'S' VALUE: 21
MIN 'V' VALUE: 233
MAX 'V' VALUE: 254
*/
	}


	if (name == "RedOctagon") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(35, 14, 233));
		setHSVmax(Scalar(36, 17, 235));

		setColor(RED);

/*		setShape("Octagon");
MIN 'H' VALUE: 35
MAX 'H' VALUE: 36
MIN 'S' VALUE: 14
MAX 'S' VALUE: 17
MIN 'V' VALUE: 233
MAX 'V' VALUE: 235
*/
	}


	if (name == "TestingObjectDetecting_NOT_Recording_Locations") { //do not record any locations of the beacons (this beacon is used for testing)
		setShape("TestingObjectDetecting_NOT_Recording_Locations");
	}
}


int Beacon::getXPos() {
	return Beacon::xPos;
}
int Beacon::getYPos() {
	return Beacon::yPos;
}
void Beacon::setXPos(int x) {
	Beacon::xPos = x;
}
void Beacon::setYPos(int y) {
	Beacon::yPos = y;
}


Scalar Beacon::getHSVmin() {
	return Beacon::HSVmin;
}
Scalar Beacon::getHSVmax() {
	return Beacon::HSVmax;
}
void Beacon::setHSVmin(Scalar min) {
	Beacon::HSVmin = min;
}
void Beacon::setHSVmax(Scalar max) {
	Beacon::HSVmax = max;
}


Scalar Beacon::getColor() {
	return Color;
}
void Beacon::setColor(Scalar s) {
	Color = s;
}
String Beacon::getShape() {
	return Shape;
}
void Beacon::setShape(string s) {
	Shape = s;
}
