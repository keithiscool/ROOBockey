#ifndef BEACONS_HPP
#define BEACONS_HPP

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



/*Define Shapes and Colors for Known Target Beacon Colors and Shapes:
* create some Beacon objects so that we can use their member functions/information
* the text "Color_shape" tells the class definition (In "Beacons.c") what shape and color category the beacon falls in*/
extern Beacon RedOctagon("RedOctagon");
extern Beacon BlueRectangle("BlueRectangle");
extern Beacon GreenTriangle("GreenTriangle");
//Now define the vectors in case multiple beacons need to be tracked
extern vector<Beacon> RedOctagonVector;
extern vector<Beacon> BlueRectangleVector;
extern vector<Beacon> GreenTriangleVector;



#endif /* BEACONS_HPP */