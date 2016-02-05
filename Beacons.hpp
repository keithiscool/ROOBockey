//#pragma once //only compile this header once

#ifndef BEACONS_HPP
#define BEACONS_HPP

#include "defs.hpp"

class Beacon {

public:
	Beacon::Beacon();
	Beacon::~Beacon();
	Beacon(string name);

	int getXPos();
	void setXPos(int x);
	int getYPos();
	void setYPos(int y);

	Scalar getHSVmin();
	Scalar getHSVmax();
	void setHSVmin(Scalar min);
	void setHSVmax(Scalar max);

	Scalar getColor();
	void setColor(Scalar c);
	String getShape();
	void setShape(string s);


private:
	int xPos, yPos;
	String Shape;
	Scalar Color;
	Scalar HSVmin, HSVmax;
};

#ifndef USE_EXTERNS
	/*Define Shapes and Colors for Known Target Beacon Colors and Shapes:
	* create some Beacon objects so that we can use their member functions/information
	* the text "Color_shape" tells the class definition (In "Beacons.c") what shape and color category the beacon falls in*/
	Beacon YellowTriangle("YellowTriangle");
	Beacon RedSquare("RedRectangle");
	Beacon PurplePentagon("PurplePentagon");
	Beacon BlueHexagon("BlueHexagon");
	Beacon GreenCircle("GreenCircle");
	//Now define the vectors in case multiple beacons need to be tracked
	vector<Beacon> YellowTrianglesVector;
	vector<Beacon> RedSquaresVector;
	vector<Beacon> PurplePentagonsVector;
	vector<Beacon> BlueHexagonsVector;
	vector<Beacon> GreenCirclesVector;
#endif

#endif /* BEACONS_HPP */