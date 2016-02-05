//ROObockey Senior Design Team E - University of Akron : Design of a floor hockey shooting robot
//main.cpp - Project Used to Track Various Target Beacons of Different Shapes and Sizes
//Originally Written by  Kyle Hounslow 2013 - https://www.youtube.com/watch?v=4KYlHgQQAts
//Modified by Keith Martin 2015

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

/*NOTE THAT I AM USING OPENCV-Version3.0.0 WITH MICROSOFT VISUAL STUDIO 2013*/

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "Beacons.h"

#define CameraNumber 0
#define DEBUGMODE 1
#define RUNMODE 0
bool OperationMode = DEBUGMODE;


//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//default display capture window frame width and height (640x480 window)
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = int(FRAME_HEIGHT*FRAME_WIDTH / 1.5);
//names that will appear at the top of each window
const string trackbarWindowName = "Trackbars";


void on_trackbar(int, void*){
	//This function gets called whenever a
	// trackbar position is changed
}

string intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void createTrackbars(){
	//create window for trackbars
	namedWindow(trackbarWindowName, 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
}

void drawObject(vector<Beacon> theBeacons, Mat &frame){
	int sz = theBeacons.size();
	for (int i = 0; i<sz; i++){
		cv::circle(frame, cv::Point(theBeacons.at(i).getXPos(), theBeacons.at(i).getYPos()), 10, cv::Scalar(0, 0, 255));
		cv::putText(frame, intToString(theBeacons.at(i).getXPos()) + " , " + intToString(theBeacons.at(i).getYPos()), cv::Point(theBeacons.at(i).getXPos(), theBeacons.at(i).getYPos() + 20), 1, 1, Scalar(0, 255, 0));
		cv::putText(frame, theBeacons.at(i).getShape(), cv::Point(theBeacons.at(i).getXPos(), theBeacons.at(i).getYPos() - 30), 1, 2, theBeacons.at(i).getColor());
	}
}

void morphOps(Mat &thresh){
	//create structuring element that will be used to filter image using "dilate" and "erode" on the image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);

	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
}


void trackFilteredObject(Beacon theBeacon, Mat &threshold, Mat HSV, Mat &imgOriginal){
	
	vector <Beacon> TrackedBeacon;
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //find contours of filtered image
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		
		if (numObjects<MAX_NUM_OBJECTS){ //if number of objects > MAX_NUM_OBJECTS we have a noisy filter
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we save a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area>MIN_OBJECT_AREA) {
					Beacon GreenCircle;
					//GreenCircle.setXPos(int(moment.m10 / area));
					//GreenCircle.setYPos(int(moment.m01 / area));
					theBeacon.setXPos(int(moment.m10 / area));
					theBeacon.setYPos(int(moment.m01 / area));

					if (theBeacon.getShape() != "DontStore") {
						GreenCircle.setShape(theBeacon.getShape());
						GreenCircle.setColor(theBeacon.getColor());
						TrackedBeacon.push_back(GreenCircle); //add additional element to the end of TrackedBeacon Vector
					}
					objectFound = true;
				}
				else objectFound = false;
			}
			//let user know you found an object
			if (objectFound == true){
				//draw object location on screen
				drawObject(TrackedBeacon, imgOriginal);
			}
		}
		else putText(imgOriginal, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}


vector<Vec3f> LocateCircles(Mat &InputFrame, Mat &grayInputframe){

	vector<Vec3f> circles; //this is a vector of detected circles that are retreived from the HoughCircles() function
	/// Apply the Hough Transform to find the circles
	HoughCircles(grayInputframe, circles, CV_HOUGH_GRADIENT, 2, 20, 100, 155, 20, 300); // change the last two parameters to search for minRadius,maxRadius circles
	
	if (OperationMode == DEBUGMODE) { // If debugging, show where the circles are in output windows
		for (int i = 0; i < circles.size(); i++) {		// for each detected circle . . .

			int x = circles[i][0], y = circles[i][1], radius = circles[i][2];
			// show ball position x, y, and radius to command line
			std::cout << "GreenCircle position x = " << x					// x position of center point of circle
				<< ", y = " << y									// y position of center point of circle
				<< ", radius = " << radius << "\n";					// radius of circle
			// draw small green circle at center of detected object
			circle(InputFrame,										// draw on original image
				Point(x, y),										// center point of circle
				3,													// radius of circle in pixels (really tiny circle)
				Scalar(0, 255, 0),									// draw pure green (remember, its BGR, not RGB)
				CV_FILLED);											// thickness, fill in the circle
			// draw green circle around the detected object
			circle(InputFrame,										// draw on original image
				Point(x, y),										// center point of circle
				radius,												// radius of drawn circle is the detected circle's radius in pixels
				Scalar(0, 255, 0),									// draw pure green circle (remember, its BGR, not RGB)
				3);													// thickness of circle in pixels
			putText(InputFrame, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2); // Indicate that circle is being tracked if detected
			putText(InputFrame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2); // Show coordinates of detected circle
		}
	}
		return circles;
}


// helper function: finds a cosine of angle between vectors from pt0->pt1 and from pt0->pt2
static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


vector<Vec3f> LocateSquares(Mat &InputFrame, Mat &grayInputframe) {

	vector<Vec3f> squares; //this is a vector of detected circles that are retrieved from the HoughCircles() function
	vector<Point> approx;
	// find contours and store them all as a list
	findContours(grayInputframe, squares, RETR_LIST, CHAIN_APPROX_SIMPLE);

	// test each contour
	for (size_t i = 0; i < squares.size(); i++)
	{
		// approximate contour with accuracy proportional
		// to the contour perimeter
		approxPolyDP(Mat(squares[i]), approx, arcLength(Mat(squares[i]), true)*0.02, true);

		// square contours should have 4 vertices after approximation
		// relatively large area (to filter out noisy contours) and be convex.
		// Note: absolute value of an area is used because
		// area may be positive or negative - in accordance with the contour orientation
		if (approx.size() == 4 && //4 vertices detected
			fabs(contourArea(Mat(approx))) > 1000 &&
			isContourConvex(Mat(approx)))
		{
			double maxCosine = 0;

			for (int j = 2; j < 5; j++)
			{
				// find the maximum cosine of the angle between joint edges
				double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = MAX(maxCosine, cosine);
			}
			// if cosines of all angles are small
			// (all angles are ~90 degree) then write quandrange
			// vertices to resultant sequence
			if (maxCosine < 0.3)
				squares.push_back(approx);
		}
	}

	if (OperationMode == DEBUGMODE) { // If debugging, show where the circles are in output windows
		for (int i = 0; i < squares.size(); i++) {		// for each detected circle . . .

			int x = squares[i][0], y = squares[i][1], width = squares[i][2];
			// show ball position x, y, and radius to command line
			std::cout << "RedSquare position x = " << x					// x position of center point of circle
				<< ", y = " << y									// y position of center point of circle
				<< ", width = " << width << "\n";					// radius of square
			// draw small red circle at center of detected object
			circle(InputFrame,										// draw on original image
				Point(x, y),										// center point of square
				3,													// radius of circle in pixels (really tiny circle)
				Scalar(0, 0, 255),									// draw pure red (remember, its BGR, not RGB)
				CV_FILLED);											// thickness, fill in the circle
			// draw red square around the detected object
			circle(InputFrame,										// draw on original image
				Point(x, y),										// center point of square
				width,												// radius of drawn square is the detected circle's radius in pixels
				Scalar(0, 0, 255),									// draw pure red square (remember, its BGR, not RGB)
				3);													// thickness of circle in pixels
			putText(InputFrame, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2); // Indicate that circle is being tracked if detected
			putText(InputFrame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2); // Show coordinates of detected square
		}
	}
	return squares;
}


int main(int argc, char* argv[]) {
	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = false;

	//Matrix to store each frame of the webcam feed
	Mat imgOriginal;
	Mat threshold;
	Mat HSV;
	Mat GRAYSCALE; //Used for HoughCircles for Circle Detection
	Mat GreenCircleOUTPUT, RedSquareOUTPUT; //Used for Detecting Shapes
	vector<Vec3f> circles; //Used for HoughCircles for GreenCircle Detection

	if (calibrationMode) {
		createTrackbars(); //create slider bars for HSV filtering
	}

	//Open the Default Camera
	VideoCapture cap(CameraNumber);
	if (!cap.isOpened())  // check if we succeeded in receiving images from camera
		return -1;

	//set height and width of capture frame
	cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	//start an infinite loop where webcam feed is copied to imgOriginal matrix
	//all of our operations will be performed within this loop
	while (1) {
		cap >> imgOriginal; //get a new frame from camera
		medianBlur(imgOriginal, imgOriginal, 5); //smooth out the image with median filter
		cvtColor(imgOriginal, GRAYSCALE, COLOR_BGR2GRAY); // Append GRAYSCALE Mat with grayscale conversion of imgOriginal

		if (calibrationMode == true) {
			Beacon DontStore("DontStore");
			//if in calibration mode, we track objects based on the HSV slider values.
			cvtColor(imgOriginal, HSV, COLOR_BGR2HSV);
			inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
			morphOps(threshold);

			imshow("HSV_ThresholdedImage", threshold);
			trackFilteredObject(DontStore, threshold, HSV, imgOriginal);
		}
		else{
			/*Define Shapes and Colors for Known Target Beacons: Green Circle, Red Square, Yellow Triangle*/
			Beacon GreenCircle("GreenCircle");	//create some temp Beacon objects so that we can use their member functions/information
												//the text "Color_shape" tells the class definition what shape and color category the beacon falls in
			Beacon YellowTriangle("YellowTriangle");
			Beacon RedSquare("RedSquare");

			GreenCircleOUTPUT = GRAYSCALE.clone(); // Clone the input image frame to GreenCircleOUTPUT
			RedSquareOUTPUT = GRAYSCALE.clone(); // Clone the input image frame to GreenCircleOUTPUT

			//first find GreenCircles using color recognition
			cvtColor(GreenCircleOUTPUT, HSV, COLOR_BGR2HSV);
			inRange(HSV, GreenCircle.getHSVmin(), GreenCircle.getHSVmax(), threshold);
			morphOps(threshold);
			trackFilteredObject(GreenCircle, threshold, HSV, GreenCircleOUTPUT);

			//Test the code to look for the GreenCircle Beacon (HoughCircles function used here using grayscale input)
			LocateCircles(GreenCircleOUTPUT, GRAYSCALE);

			//first find RedSquares using color recognition
			cvtColor(GreenCircleOUTPUT, HSV, COLOR_BGR2HSV);
			inRange(HSV, GreenCircle.getHSVmin(), GreenCircle.getHSVmax(), threshold);
			morphOps(threshold);
			trackFilteredObject(GreenCircle, threshold, HSV, GreenCircleOUTPUT);

			//Test the code to look for the RedSquare Beacon (function here used grayscale input)
			LocateCircles(RedSquareOUTPUT, GRAYSCALE);
		}

		//show all Mat video frames in new windows
		imshow("RGB_Input", imgOriginal);
		imshow("Grayscale Converted Input", GRAYSCALE);
		imshow("RGB_CircleDetectedOutput", GreenCircleOUTPUT);
		imshow("RGB_CircleDetectedOutput", RedSquareOUTPUT);

		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}
	return 0;
}
