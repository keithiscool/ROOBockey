#define USE_EXTERNS
#include "Beacons.hpp"


/*Define Shapes and Colors for Known Target Beacons:
Green Triangle
Blue Rectangle
Red Octagon
*/



Beacon::Beacon() { // Must have a "simple" declaration for the class...
	// nothing here in the constructor
}

Beacon::~Beacon() { // I never have to free-up the class, so this is not run...
	// nothing here in the deconstructor
}

Beacon::Beacon(string name) { // this is the class declaration that I am actually using...
	if (name == "GreenTriangle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(0, 255, 0));

		setColor(GREEN);
		setShape("Triangle");
	}

	if (name == "BlueRectangle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 0, 0));

		setColor(BLUE);
		setShape("Rectangle");
	}


	if (name == "RedOctagon") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(0, 0, 255));

		setColor(RED);
		setShape("Octagon");
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


