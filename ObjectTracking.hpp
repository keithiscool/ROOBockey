//#pragma once //only compile this header once

#ifndef OBJECTTRACKING_HPP
#define OBJECTTRACKING_HPP

#include "Beacons.hpp"

#ifdef USE_EXTERNS
	//initial min and max HSV filter values.
	//these will be changed using trackbars
	extern int H_MIN;
	extern int H_MAX;
	extern int S_MIN;
	extern int S_MAX;
	extern int V_MIN;
	extern int V_MAX;
#else
	//initial min and max HSV filter values.
	//these will be changed using trackbars
	int H_MIN = 0;
	int H_MAX = 256;
	int S_MIN = 0;
	int S_MAX = 256;
	int V_MIN = 0;
	int V_MAX = 256;
#endif

const string trackbarWindowName = "Trackbars";

//Tracking Library Function Declarations
void on_trackbar(int, void*);
void createObjectTrackingParameterTrackbars(void);
void morphOps(Mat &thresh);
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
void drawContours(Mat& image, const vector<vector<Point> >& contours, vector<Vec4i> hierarchy, string title);
void shapeDetection(Mat& inputImage, vector<vector<Point>> contours, vector<Vec4i> hierarchy, Mat& outputImage);
size_t calibratingTrackColorFilteredObjects(Mat &InputMat, vector<vector<Point>> &contours, vector<Vec4i> &hierarchy, Mat &threshold);
size_t trackColorFilteredObjects(Mat &InputMat, vector<Beacon> &theBeacon, vector<vector<Point>> &contours, vector<Vec4i> hierarchy, Mat &threshold);
void RecordBeaconPosition(Beacon &theBeacon, vector<Vec4i> &hierarchy, vector<vector<Point>> &contours, vector<Beacon> &theBeaconsVector);


#endif /* OBJECTTRACKING_HPP */