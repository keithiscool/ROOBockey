#define USE_EXTERNS
#include "Beacons.hpp"
#include "defs.hpp"

/*Define Shapes and Colors for Known Target Beacons:
Yellow Triangle
Red Square
Purple Pentagon
Blue Hexagon
Green Circle*/

Beacon::Beacon() { // Must have a "simple" declaration for the class...
	// nothing here in the constructor
}

Beacon::~Beacon() { // I never have to free-up the class, so this is not run...
	// nothing here in the deconstructor
}

Beacon::Beacon(string name) { // this is the class declaration that I am actually using...
	if (name == "YellowTriangle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for Yellow:
		setColor(YELLOW);
		setShape("Triangle");
	}

	if (name == "RedRectangle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for Red:
		setColor(RED);
		setShape("Square");
	}

	if (name == "PurplePentagon") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for Purple:
		setColor(PURPLE);
		setShape("Pentagon");
	}

	if (name == "BlueHexagon") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for Blue:
		setColor(BLUE);
		setShape("Hexagon");
	}

	if (name == "GreenCircle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for Green:
		setColor(GREEN);
		setShape("Circle");
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