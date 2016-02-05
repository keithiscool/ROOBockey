#include "Beacons.h"

/*Define Shapes and Colors for Known Target Beacons:
Green Circle
Red Square
Yellow Triangle
*/

Beacon::Beacon() { // Must have a "simple" declaration for the class...
	// nothing here in the constructor
}

Beacon::~Beacon() { // I never have to free-up the class, so this is not run...
	// nothing here in the deconstructor
}

Beacon::Beacon(string name) { // this is the class declaration that I am actually using...
	if (name == "GreenCircle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for Green:
		setColor(Scalar(0, 255, 0));
		setShape("Circle");
	}

	if (name == "YellowTriangle") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for Yellow:
		setColor(Scalar(0, 255, 255));
		setShape("Triangle");
	}

	if (name == "RedSquare") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 0, 0));
		setHSVmax(Scalar(255, 255, 255));

		//BGR value for Red:
		setColor(Scalar(0, 0, 255));
		setShape("Square");
	}

	if (name == "DontStore") {
		setShape("DontStore");
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