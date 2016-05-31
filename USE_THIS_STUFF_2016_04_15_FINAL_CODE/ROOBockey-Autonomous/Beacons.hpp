#ifndef BEACONS_HPP
#define BEACONS_HPP

#include "defs.hpp"
#include <string>

class Beacon {

public:
	Beacon(std::string name);
	Beacon() = delete;
	~Beacon();

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




#endif /* BEACONS_HPP */
