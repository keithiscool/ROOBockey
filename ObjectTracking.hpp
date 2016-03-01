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

//This allows the user to calibrate the HSV threshold color filter to detect an object within the camera image
class MouseCalibrateFilter {
	public:
		//declare variables to use for using mouse rectangle area to get minimum and maximum HSV values automatically
		//NOTE: THIS FEATURE WILL ONLY WORK WHEN IN CALIBRATION_MODE (set in defs.hpp)
		bool mouseIsDragging;//used for showing a rectangle on screen as user clicks and drags mouse
		bool mouseMove;
		bool rectangleSelected;
		//keep track of initial point clicked and current position of mouse
		Point currentMousePoint;
		Point initialClickPoint;
		Rect rectangleROI; //this is the ROI that the user has selected
		vector<int> H_ROI, S_ROI, V_ROI;// HSV values from the click/drag ROI region stored in separate vectors so that we can sort them easily
};


#ifndef MAIN_CPP
	MouseCalibrateFilter MouseInfo; //create class declaration (create the object that stores the mouse information)
	MouseCalibrateFilter *MouseHSVCalibrationPtr = &MouseInfo; //use pointer to modify the values within the functions "clickAndDragRectangle() and mouseRecordHSV_Values()"
#endif // !MAIN_CPP


//functions used for calibrating the HSV values used for filtering the color detections
void clickAndDragRectangle(int event, int x, int y, int flags, void* param);
void mouseRecordHSV_Values(Mat frame, Mat hsv_frame);


//Tracking Library Function Declarations
void on_trackbar(int, void*);
void createObjectTrackingParameterTrackbars(void);
void morphOps(Mat &thresh);
static double angle(Point pt1, Point pt2, Point pt0);
void setLabel(Mat& im, const string label, vector<Point> &contour);
void drawContours(Mat& image, const vector<vector<Point>> &contours, string title);
void shapeDetection(Mat& inputImage, vector<vector<Point>> contours, vector<Vec4i> hierarchy, Mat& outputImage);
size_t calibratingTrackColorFilteredObjects(Mat &InputMat, vector<vector<Point>> &contours, vector<Vec4i> &hierarchy, Mat &threshold);
size_t trackColorFilteredObjects(Mat &InputMat, vector<Beacon> &theBeacon, vector<vector<Point>> &contours, vector<Vec4i> hierarchy, Mat &threshold);
void RecordBeaconPosition(Beacon &theBeacon, vector<vector<Point>> &contours, vector<Beacon> &theBeaconsVector);
void DrawTarget(int x, int y, Mat &frame);
string intToString(int number);



#endif /* OBJECTTRACKING_HPP */