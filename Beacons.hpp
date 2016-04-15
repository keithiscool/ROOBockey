#ifndef BEACONS_HPP
#define BEACONS_HPP

#include "defs.hpp"
#include <string>

class Beacon {

public:
	Beacon(std::string name);

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
	Beacon(); //constructor for class
	~Beacon(); //deconstructor for class
	int xPos, yPos;
	String Shape;
	Scalar Color;
	Scalar HSVmin, HSVmax;
};




#endif /* BEACONS_HPP */