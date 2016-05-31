#include "defs.hpp"
#include "ObjectTracking.hpp"
#include "Xbox360Controller.hpp"
#include <thread>
#include <wiringPi.h>
#include <string>

#define OBJECTTRACKING_CPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Color Detection Stuff Here*/
/*Shape Detection Stuff At the Bottom*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


const string trackbarWindowName = "Trackbars";

MouseCalibrateFilter MouseInfo; //create class declaration (create the object that stores the mouse information)
MouseCalibrateFilter *MouseHSVCalibrationPtr = &MouseInfo; //use pointer to modify the values within the functions "clickAndDragRectangle() and mouseRecordHSV_Values()"

//initial min and max HSV filter values.
//these will be changed using trackbars
//NOTE: THESE ARE GLOBAL VARIABLES used with the mouse for calibrating the HSV filter
//by clicking and dragging the mouse
//THESE ARE NOT THE VARIABLES USED TO TUNE EACH INDIVIDUAL CLASS BEACON FILTER
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;


const string mouseWindowName = "Mouse Operations";


/*Define Shapes and Colors for Known Target Beacon Colors and Shapes:
* create some Beacon objects so that we can use their member functions/information
* the text "Color_shape" tells the class definition (In "Beacons.c") what shape and color category the beacon falls in*/
Beacon RedOctagon("RedOctagon");
Beacon BlueRectangle("BlueRectangle");
Beacon GreenTriangle("GreenTriangle");
Beacon CalibrationObject("TestingObjectDetecting_NOT_Recording_Locations");
//Now define the vectors in case multiple beacons need to be tracked
vector<Beacon> RedOctagonVector;
vector<Beacon> BlueRectangleVector;
vector<Beacon> GreenTriangleVector;
vector<Beacon> CalibrationObjectVector;


int averageBeaconPosition = 0;
int averageBeaconPositionUsingCalibrationHSVfilter = 0;


void imageProcessingRoutine(void){
	Mat src0;
	Mat ColorThresholded_Img0, ColorThresholded_Img, outputImg0, outputImg, src, HSV_Input;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

#ifdef USING_WEBCAM
	VideoCapture cap(CAMERA_NUMBER); //Open the Default Camera
	if (!cap.isOpened()) exit(EXIT_FAILURE); //Check if we succeeded in receiving images from camera. If not, quit program.
	cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); //Set height and width of capture frame
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
#else
	//Testing the program using sample images copied into working project directory
	src0 = imread("images.png").clone(); //clone used to pass the Mat around in functions as a "deep copy"
	//src0 = imread("basic-shapes-2.png").clone(); //clone used to pass the Mat around in functions as a "deep copy"
	//src0 = imread("circlesOnWall.png").clone(); //clone used to pass the Mat around in functions as a "deep copy"
	//src0 = imread("pic3.png").clone(); //clone used to pass the Mat around in functions as a "deep copy"
	//src0 = imread("pic5.png").clone(); //clone used to pass the Mat around in functions as a "deep copy"
#endif


#ifdef CALIBRATION_MODE
	//Create trackbars that you can manually change in order to alter the HSV filter minimum & maximum values
	//I commented this out beacuse it will not run in Linux (Raspberry Pi2)
	//The HSV filter is calibrated using the mouse in CALIBRATION_MODE or hardcoded to the class HSV values in Beacons.cpp when each beacon object is created
	createObjectTrackingParameterTrackbars();

	//create a window before setting mouse callback
	namedWindow(mouseWindowName);
	//set mouse callback function to be active on "Webcam Feed" window
	//we pass the handle to our "frame" matrix so that we can draw a rectangle to it as the user clicks and drags the mouse
	//NOTE: THE "OnMouse" function parameter for "setMouseCallback()" for setMouseCallback is a function with parameters: (int, int, int, void*);
	setMouseCallback(mouseWindowName, clickAndDragRectangle, &src);
#endif


#ifdef USING_WEBCAM
	cap >> src0; //get a new frame from camera
#endif

	src = src0.clone(); //get a "deep copy" (physical, not pointer) copy the input video frame
	cvtColor(src, HSV_Input, COLOR_BGR2HSV); //convert the input BGR color image to a HSV image


#ifdef CALIBRATION_MODE
	//set HSV values from user selected region
	mouseRecordHSV_Values(src, HSV_Input);

	if ((calibratingTrackColorFilteredObjects(src, HSV_Input, contours, hierarchy, ColorThresholded_Img0)) > 0) { //number of objects detected > 0 and < "MAX_NUM_OBJECTS"
		ColorThresholded_Img = ColorThresholded_Img0.clone(); //had to clone the image to pass a "deep copy" to the shape detection function
		shapeDetection(ColorThresholded_Img, contours, hierarchy, outputImg0); //search for shapes in the color filtered thresholded image
		outputImg = outputImg0.clone(); //had to clone the image to pass a "deep copy" to the output "imshow"
		averageBeaconPositionUsingCalibrationHSVfilter = chooseBeaconToShootAt();
		alignWithBeacon(averageBeaconPositionUsingCalibrationHSVfilter);
}
#else //CALIBRATION_MODE
	if (Ba == 1) { //check if button on controller for beacon color was pressed
		if ( (trackColorFilteredObjects(src, HSV_Input, GreenTriangleVector, contours, hierarchy, ColorThresholded_Img0) ) > 0) { //number of objects detected > 0 and < MAX_NUM_OBJECTS
			ColorThresholded_Img = ColorThresholded_Img0.clone(); //had to clone the image to pass a "deep copy" to the shape detection function
			shapeDetection(ColorThresholded_Img, contours, hierarchy, outputImg0); //search for shapes in the color filtered thresholded image
			outputImg = outputImg0.clone(); //had to clone the image to pass a "deep copy" to the output "imshow"
			averageBeaconPosition = chooseBeaconToShootAt();
			alignWithBeacon(averageBeaconPosition);
		}
	}
	if (Bx == 1) { //check if button on controller for beacon color was pressed
		if ((trackColorFilteredObjects(src, HSV_Input, BlueRectangleVector, contours, hierarchy, ColorThresholded_Img0)) > 0) { //number of objects detected > 0 and < MAX_NUM_OBJECTS
			ColorThresholded_Img = ColorThresholded_Img0.clone(); //had to clone the image to pass a "deep copy" to the shape detection function
			shapeDetection(ColorThresholded_Img, contours, hierarchy, outputImg0); //search for shapes in the color filtered thresholded image
			outputImg = outputImg0.clone(); //had to clone the image to pass a "deep copy" to the output "imshow"
			averageBeaconPosition = chooseBeaconToShootAt();
			alignWithBeacon(averageBeaconPosition);
		}
	}
	if (Bb == 1) { //check if button on controller for beacon color was pressed
		if ((trackColorFilteredObjects(src, HSV_Input, RedOctagonVector, contours, hierarchy, ColorThresholded_Img0)) > 0) { //number of objects detected > 0 and < MAX_NUM_OBJECTS
			ColorThresholded_Img = ColorThresholded_Img0.clone(); //had to clone the image to pass a "deep copy" to the shape detection function
			shapeDetection(ColorThresholded_Img, contours, hierarchy, outputImg0); //search for shapes in the color filtered thresholded image
			outputImg = outputImg0.clone(); //had to clone the image to pass a "deep copy" to the output "imshow"
			averageBeaconPosition = chooseBeaconToShootAt();
			alignWithBeacon(averageBeaconPosition);
		}
	}


#endif //CALIBRATION_MODE



#ifdef SHOW_OPENCV_IMAGES

	
	if(src.data)
	{
		imshow(mouseWindowName, src); //show Input BGR Mat video frame in new window
	}

	if(ColorThresholded_Img0.data)
	{
		imshow("ColorThresholdedImg", ColorThresholded_Img0);
	}

	if(ColorThresholded_Img.data)
	{
		imshow("OutputColor&ShapeDetectedImg", ColorThresholded_Img);
	}

	if(outputImg.data)
	{
		imshow("OutputImg", outputImg);
	}

	waitKey(5); //delay in milliseconds so OpenCV does not consume all processor time. "imshow" will not appear without this waitKey() command

#endif //SHOW_OPENCV_IMAGES

}



void on_trackbar(int, void*) {
	// This function gets called whenever a trackbar position is changed
}



//create trackbars and insert them into their own window as sliders to control tracking parameters
//these sliders allow the user to tune the HSV to show the intended object color
void createObjectTrackingParameterTrackbars(void) {
	namedWindow("Trackbars", 0); // create window for trackbars
	char TrackbarName[50]; // create memory to store trackbar name on window
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);

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


//filter noise in the HSV image by eroding and dilating the image. this will prevent false color detection in the image
void morphOps(Mat &thresh) {
	//create structuring element that will be used to filter image using "dilate" and "erode" on the image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
	erode(thresh, thresh, erodeElement); //eliminate noise
	//erode(thresh, thresh, erodeElement);
	dilate(thresh, thresh, dilateElement); //enhance groups of pixels in thresholded image
	//dilate(thresh, thresh, dilateElement);
}


//CALIBRATION TEST FUNCTION is used to calibrate HSV filter after input BGR image and output contours detected if there are not too many
size_t calibratingTrackColorFilteredObjects(Mat &InputMat, Mat &HSV, vector<vector<Point> > &contours, vector<Vec4i> &hierarchy, Mat &threshold) {

	//Generate a binary image from the HSV input image
	inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);

	//Dilate and Erode the image frame in order to filter out noise and enhance the desired color
	morphOps(threshold);

	//find contours in filtered image
	findContours(threshold, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	//////////////size_t numObjects = hierarchy.size(); //counts the objects seen after applied threshold
	size_t numObjects = contours.size(); //counts the objects seen after applied threshold

	if ((numObjects > 0) && (numObjects<MAX_NUM_OBJECTS)) { //if number of objects > MAX_NUM_OBJECTS we have a noisy filter
		return numObjects; //function passes if objects are detected, but there are not too many detected objects (from bad filter)
	}else {
		putText(threshold, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2); //too many objects after filter
		//putText(InputMat, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2); //too many objects after filter
	}
	return 0;
}


//Function is used for each Beacon to filter input BGR image and output contours detected if there are not too many
size_t trackColorFilteredObjects(Mat &InputMat, Mat &HSV, vector<Beacon> &theBeaconsVector, vector<vector<Point> > &contours, vector<Vec4i> hierarchy, Mat &threshold) {
	inRange(HSV, theBeaconsVector[0].getHSVmin(), theBeaconsVector[0].getHSVmax(), threshold); //HSV input image and output a thresholded binary (black and white) image
	morphOps(threshold); //filter the thresholded binary image

	//find contours in filtered image
	findContours(threshold, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//use moments method to find our filtered object
	double refArea = 0;
	//////////////size_t numObjects = hierarchy.size(); //counts the objects seen after applied threshold
	size_t numObjects = contours.size(); //counts the objects seen after applied threshold

	if ((numObjects > 0) && (numObjects<MAX_NUM_OBJECTS)) { //if number of objects > MAX_NUM_OBJECTS we have a noisy filter
		return numObjects; //function passes if objects are detected, but there are not too many detected objects (from bad filter)
	}

	else putText(threshold, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2); //too many objects after filter
	return 0;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Mouse Calibration of HSV color filter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//This function is used to calibrate the HSV values for the color filter
//The function works when the user clicks the left mouse button and highlights the input image color area to get the minimum and maximum HSV values
void clickAndDragRectangle(int event, int x, int y, int flags, void* param) {
		//only if calibration mode is on will we use the mouse to change HSV values
#ifdef CALIBRATION_MODE

		//get handle to video feed passed in as "param" and cast as Mat pointer
		Mat* videoFeed = (Mat*)param;

		if (event == CV_EVENT_LBUTTONDOWN && MouseHSVCalibrationPtr->mouseIsDragging == false) {
			MouseHSVCalibrationPtr->initialClickPoint = Point(x, y); //keep track of initial point clicked
			MouseHSVCalibrationPtr->mouseIsDragging = true; //user has begun dragging the mouse
		}
		//user is dragging the mouse
		if (event == CV_EVENT_MOUSEMOVE && MouseHSVCalibrationPtr->mouseIsDragging == true) {
			MouseHSVCalibrationPtr->currentMousePoint = Point(x, y); //keep track of current mouse point
			MouseHSVCalibrationPtr->mouseMove = true; //user has moved the mouse while clicking and dragging
		}
		//user has released left button
		if (event == CV_EVENT_LBUTTONUP && MouseHSVCalibrationPtr->mouseIsDragging == true) {
			MouseHSVCalibrationPtr->rectangleROI = Rect(MouseHSVCalibrationPtr->initialClickPoint, MouseHSVCalibrationPtr->currentMousePoint); //set rectangle ROI to the rectangle that the user has selected
			//reset boolean variables
			MouseHSVCalibrationPtr->mouseIsDragging = false;
			MouseHSVCalibrationPtr->mouseMove = false;
			MouseHSVCalibrationPtr->rectangleSelected = true;
		}
		if (event == CV_EVENT_RBUTTONDOWN) {
			//user has clicked right mouse button, so Reset HSV Values
			H_MIN = 0;
			S_MIN = 0;
			V_MIN = 0;
			H_MAX = 255;
			S_MAX = 255;
			V_MAX = 255;
		}
		if (event == CV_EVENT_MBUTTONDOWN) {
			//user has clicked middle mouse button
			//enter code here if needed.
		}
#endif
}



//This function is used to record the HSV values in the main while loop to constantly check if the mouse was clicked and the callback
//altered the HSV values beacuse a region was selected in the input image
void mouseRecordHSV_Values(Mat frame, Mat hsv_frame) {
	//save HSV values for RegionOfInterest (R.O.I.) that user selected to a vector
	if ((MouseHSVCalibrationPtr->mouseMove == false) && (MouseHSVCalibrationPtr->rectangleSelected == true)) {

		//clear previous vector values
		if (MouseHSVCalibrationPtr->H_ROI.size()>0) MouseHSVCalibrationPtr->H_ROI.clear();
		if (MouseHSVCalibrationPtr->S_ROI.size()>0) MouseHSVCalibrationPtr->S_ROI.clear();
		if (MouseHSVCalibrationPtr->V_ROI.size()>0) MouseHSVCalibrationPtr->V_ROI.clear();

		//if the rectangle has no width or height (user has only dragged a line) then we don't try to iterate over the width or height
		if (MouseHSVCalibrationPtr->rectangleROI.width < 1 || MouseHSVCalibrationPtr->rectangleROI.height < 1) {
			cout << "Please drag a rectangle, not a line" << endl;
		}
		else {
			for (int i = MouseHSVCalibrationPtr->rectangleROI.x; i<MouseHSVCalibrationPtr->rectangleROI.x + MouseHSVCalibrationPtr->rectangleROI.width; i++) {
				//iterate through both x and y direction and save HSV values at each and every point
				for (int j = MouseHSVCalibrationPtr->rectangleROI.y; j<MouseHSVCalibrationPtr->rectangleROI.y + MouseHSVCalibrationPtr->rectangleROI.height; j++) {
					//save HSV value at this point
					MouseHSVCalibrationPtr->H_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[0]);
					MouseHSVCalibrationPtr->S_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[1]);
					MouseHSVCalibrationPtr->V_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[2]);
				}
			}
		}

		MouseHSVCalibrationPtr->rectangleSelected = false; //reset rectangleSelected so user can select another region if necessary

		//set min and max HSV values from min and max elements of each array
		if (MouseHSVCalibrationPtr->H_ROI.size()>0) {
			//NOTE: min_element and max_element return iterators so we must dereference them with "*"
			H_MIN = *std::min_element(MouseHSVCalibrationPtr->H_ROI.begin(), MouseHSVCalibrationPtr->H_ROI.end());
			H_MAX = *std::max_element(MouseHSVCalibrationPtr->H_ROI.begin(), MouseHSVCalibrationPtr->H_ROI.end());
			cout << "MIN 'H' VALUE: " << H_MIN << endl;
			cout << "MAX 'H' VALUE: " << H_MAX << endl;
		}
		if (MouseHSVCalibrationPtr->S_ROI.size()>0) {
			S_MIN = *std::min_element(MouseHSVCalibrationPtr->S_ROI.begin(), MouseHSVCalibrationPtr->S_ROI.end());
			S_MAX = *std::max_element(MouseHSVCalibrationPtr->S_ROI.begin(), MouseHSVCalibrationPtr->S_ROI.end());
			cout << "MIN 'S' VALUE: " << S_MIN << endl;
			cout << "MAX 'S' VALUE: " << S_MAX << endl;
		}
		if (MouseHSVCalibrationPtr->V_ROI.size()>0) {
			V_MIN = *std::min_element(MouseHSVCalibrationPtr->V_ROI.begin(), MouseHSVCalibrationPtr->V_ROI.end());
			V_MAX = *std::max_element(MouseHSVCalibrationPtr->V_ROI.begin(), MouseHSVCalibrationPtr->V_ROI.end());
			cout << "MIN 'V' VALUE: " << V_MIN << endl;
			cout << "MAX 'V' VALUE: " << V_MAX << endl;
		}
	}
	if (MouseHSVCalibrationPtr->mouseMove == true) {
		//if the mouse is held down, we will draw the click and dragged rectangle to the screen
		rectangle(frame, MouseHSVCalibrationPtr->initialClickPoint, Point(MouseHSVCalibrationPtr->currentMousePoint.x, MouseHSVCalibrationPtr->currentMousePoint.y), GREEN, 1, 8, 0);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Shape Detection Stuff Below*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Helper function to display text in the center of a contour
void setLabel(Mat& im, const string label, vector<Point> &contour) {
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;
	Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	Rect r = cv::boundingRect(contour);
	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), WHITE, CV_FILLED);
	putText(im, label, pt, fontface, scale, BLACK, thickness, 8);
}


// the function draws all the contours in the image in a new window (colors in BGR, not RGB)
void drawContours(Mat& image, const vector<vector<Point> > &contours, string title) {
	size_t i = 0;
	string shape;
	for (i = 0; i < contours.size(); i++) {
		const Point* p = &contours[i][0];
		int n = (int)contours[i].size();
		//if (title == "DetectingGreenTriangles") {
		if (contours[i].size() == 3) { //check if each contour is a triangle
			polylines(image, &p, &n, 1, true, GREEN, 3, LINE_AA); //yellow for triangles
		}
		//if (title == "DetectingRedRectangles") {
		if (contours[i].size() == 4) { //check if each contour is a square
			polylines(image, &p, &n, 1, true, BLUE, 3, LINE_8); //blue for rectangles
			//drawContours(image, contours, i, BLUE, FILLED, 8, hierarchy);
			//drawContours(test1, contours, i, BLUE, LINE_8, 8, hierarchy);
			//drawContours(test2, contours, i, BLUE, LINE_4, 8, hierarchy);
			//drawContours(test3, contours, i, BLUE, LINE_AA, 8, hierarchy);
		}
		//if (title == "DetectingPurplePentagons") {
			if (contours[i].size() == 5) { //check if each contour is a pentagon
				polylines(image, &p, &n, 1, true, PURPLE, 3, LINE_AA); //purple for pentagons
			}
		//if (title == "DetectingRedOctagons") {
			if (contours[i].size() == 6) { //check if each contour is a hexagon
				polylines(image, &p, &n, 1, true, RED, 3, LINE_AA); //red for octagons
			}
		//if (title == "DetectingYellowCircles") {
			if (contours[i].size() > 6) { //check if each contour is a circle
				polylines(image, &p, &n, 1, true, YELLOW, 3, LINE_AA); //yellow for circles
			}
	}
}


// Records the (X,Y) position of each Beacon
void RecordBeaconPosition(Beacon &theBeacon, vector<vector<Point> > &contours, vector<Beacon> &theBeaconsVector) {
	//for (int index = 0; index >= 0; index = hierarchy[index][0]) {
	size_t numObjects = contours.size(); //counts the number of shapes detected
	for (int index = 0; index < numObjects; index++) {
		Moments moment = moments((Mat)contours[index]);
		double area = moment.m00;
		Point center((int)(moment.m10 / area), (int)(moment.m01 / area)); //determine the center of the detectd object using moments

		//if the area is less than 20 px by 20px then it is probably just noise
		//if the area is the same as the 3/2 of the image size, probably just a bad filter
		//we only want the object with the largest area so we save a reference area each
		//iteration and compare it to the area in the next iteration.
		if (area > MIN_OBJECT_AREA) {
			theBeacon.setXPos(center.x);
			theBeacon.setYPos(center.y);
			theBeaconsVector.push_back(theBeacon); //add additional element to the end of the BeaconsVector Vector

#ifdef ShowDetectedObjects
			cout<<theBeacon.getShape()<<": "<< theBeacon.getXPos()<<","<<theBeacon.getYPos()<<endl;
#endif //ShowDetectedObjects

		}
	}
}


//Helper function to find a cosine of angle between vectors from pt0->pt1 and pt0->pt2
static double angle(Point pt1, Point pt2, Point pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


//Function to locate all contours (shapes) within a given image
void shapeDetection(Mat& inputImage, vector<vector<Point> > contours, vector<Vec4i> hierarchy, Mat& outputImage) {

	Beacon theBeacon("unknown_shape");

	vector<Point> approx; //each discovered contour (shape) found from approxPolyDP() function
	outputImage = inputImage.clone(); //copy the input Mat image to get the size of the image
	outputImage = Scalar(0, 0, 0); //black background for output image (overwrites the input image cloned values)
	
	for (int i = 0; i < contours.size(); i++) {
		// Approximate contour with accuracy proportional to the contour perimeter
		approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
		size_t vertices = approx.size(); // Number of vertices of polygonal curve
		// Skip small or non-convex objects
		if ((fabs(contourArea(contours[i])) < 100) || (!isContourConvex(approx)))
			continue;

//Beacon CalibrationObject("TestingObjectDetecting_NOT_Recording_Locations");
#ifdef CALIBRATION_MODE
		setLabel(outputImage, "TRI", contours[i]); //label triangles on output image
		drawContours(outputImage, contours, "TestingObjectDetecting_NOT_Recording_Locations");
		RecordBeaconPosition(CalibrationObject, contours, CalibrationObjectVector); //show YellowTriangle x,y pixel coordinates to terminal window		
#endif //CALIBRATION_MODE


		// Detect and label triangles (vertices == 3)
		if (vertices == 3) {
			setLabel(outputImage, "TRI", contours[i]); //label triangles on output image
			drawContours(outputImage, contours, "DetectingTriangles");
			RecordBeaconPosition(GreenTriangle, contours, GreenTriangleVector); //show YellowTriangle x,y pixel coordinates to terminal window
		}

		//if ((vertices >= 4) && (vertices <= 6)) {
		if ((vertices > 3) && (vertices < 7)) {
			// Get the cosines of all corners
			vector<double> cos;
			for (int j = 2; j < vertices + 1; j++)
				cos.push_back(angle(approx[j%vertices], approx[j - 2], approx[j - 1]));
			// Sort ascending the cosine values
			sort(cos.begin(), cos.end());
			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Detect and label squares (vertices == 4)
			// Use the degrees obtained above and the number of vertices to determine the shape of the contour
			//if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3) {
			if ((vertices == 4) && (mincos >= -0.25) && (maxcos <= 0.3125)) { //angles between 72 and 105 are acceptable (90 is ideal)
				setLabel(outputImage, "RECT", contours[i]); //label rectangles on output image
				drawContours(outputImage, contours, "DetectingRectangles");
				RecordBeaconPosition(RedOctagon, contours, BlueRectangleVector); //show RedRectangle x,y pixel coordinates to terminal window
			}

			// Detect and label octagons (vertices == 8)
			else if ((vertices == 8) && (mincos >= -0.875) && (maxcos <= -0.625)) { //angle b/t 128.68 and 151.045 degrees (ideally 135 degrees for octagon)
				setLabel(outputImage, "OCT", contours[i]); //label hexagons on output image
				drawContours(outputImage, contours, "DetectingOctagons");
				RecordBeaconPosition(RedOctagon, contours, RedOctagonVector); //show BlueHexagon x,y pixel coordinates to terminal window
			}

			//Now detect circles using better method that "Houghcircles"
/*		}else {
			// Detect and label circles (vertices > 6)
			double area = contourArea(contours[i]);
			Rect r = boundingRect(contours[i]);
			int radius = r.width / 2;

			if ((abs(1 - ((double)r.width / r.height)) <= 0.2) &&
				(abs(1 - (area / (CV_PI * pow(radius, 2)))) <= 0.2)) {
				setLabel(outputImage, "CIR", contours[i]);
				drawContours(outputImage, contours, "DetectingCircles");
				RecordBeaconPosition(GreenCircle, contours, GreenCircleVector);
			}
*/
		}
	}
}


string intToString(int number) {
	stringstream ss;
	ss << number;
	return ss.str();
}


void DrawTarget(int x, int y, Mat &frame) {
	//use some of the openCV drawing functions to draw crosshairs on your tracked image!

	//'if' and 'else' statements to prevent memory errors from writing off the screen (ie. (-25,-25) is not within the window)
	circle(frame, Point(x, y), 20, GREEN, 2);
	if (y - 25>0)
		line(frame, Point(x, y), Point(x, y - 25), GREEN, 2);
	else line(frame, Point(x, y), Point(x, 0), GREEN, 2);
	if (y + 25<FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), GREEN, 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), GREEN, 2);
	if (x - 25>0)
		line(frame, Point(x, y), Point(x - 25, y), GREEN, 2);
	else line(frame, Point(x, y), Point(0, y), GREEN, 2);
	if (x + 25<FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), GREEN, 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), GREEN, 2);

	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, GREEN, 2);

}


int chooseBeaconToShootAt(void) {
	int i = 0;
	int avgXValue = 0;
	int MillisWaitTime = 100; //milliseconds until the code is run again
	int alignment = 0;

	if ( (Ba && Bx) || (Bb && Bx) || (Ba && Bb) ) { //check if multiple buttons wer pushed. If so, leave function
		return 0;
	}

	if (Ba == 1) { //green beacon was chosen by the user pressing the green button on the wireless controller
		int numGreenTriangleBeacons = GreenTriangleVector.size();
		if (numGreenTriangleBeacons == 0) { //see if beacon is on image frame (if vector is empty, rotate the robot clockwise)
			/*if (millis() > MillisWaitTime) {
				sendMotorControllerSpeedBytes(UART_ID, 80, 176); //rotate the robot clockwise in order to find the beacon
				MillisWaitTime += MillisWaitTime;
				return 0;
			}*/
		}else { //some beacons were found
			for (i = 0; i < numGreenTriangleBeacons; i++) {
				avgXValue += GreenTriangleVector[i].getXPos(); //average the x coordinates of the detected Green Triangles
			}
			avgXValue = avgXValue / numGreenTriangleBeacons; //average the x coordinates of the detected Green Triangles
			alignment = (FRAME_WIDTH / 2) - avgXValue;
			return alignment;
		}
	}

	if (Bx == 1) { //blue beacon was chosen
		int numBlueRectangleBeacons = BlueRectangleVector.size();
		if (numBlueRectangleBeacons == 0) { //see if beacon is on image frame (if vector is empty, rotate the robot clockwise)
			/*if (millis() > MillisWaitTime) {
				sendMotorControllerSpeedBytes(UART_ID, 80, 176); //rotate the robot clockwise in order to find the beacon
				MillisWaitTime += MillisWaitTime;
				return 0;
			}*/
		}else { //some beacons were found
			for (i = 0; i < numBlueRectangleBeacons; i++) {
				avgXValue += BlueRectangleVector[i].getXPos(); //average the x coordinates of the detected Green Triangles
			}
			avgXValue = avgXValue / numBlueRectangleBeacons; //average the x coordinates of the detected Green Triangles
			alignment = (FRAME_WIDTH / 2) - avgXValue;
			return alignment;
		}
	}


	if (Bb == 1) { //red beacon was chosen
		int numRedOctagonBeacons = RedOctagonVector.size();
		if (numRedOctagonBeacons == 0) { //see if beacon is on image frame (if vector is empty, rotate the robot clockwise)
			/*if (millis() >= MillisWaitTime) {
				sendMotorControllerSpeedBytes(UART_ID, 80, 176); //rotate the robot clockwise in order to find the beacon
				MillisWaitTime += MillisWaitTime;
				return 0;
			}*/
		}else { //some beacons were found
			for (i = 0; i < numRedOctagonBeacons; i++) {
				avgXValue += RedOctagonVector[i].getXPos(); //average the x coordinates of the detected Green Triangles
			}
			avgXValue = avgXValue / numRedOctagonBeacons; //average the x coordinates of the detected Green Triangles
			alignment = (FRAME_WIDTH / 2) - avgXValue;
            return alignment;
		}
	}

	return 0;
}

void sendMotorCommand(int right, int left) {
    sendMotorControllerSpeedBytes(UART_ID, right, left + 128);
}

//Resulution for output images are:
//const int FRAME_WIDTH x FRAME_HEIGHT == (640x480 window)
int alignWithBeacon(int pixelsFromCenter) {

	if (pixelsFromCenter > 320) { //check to see if the center of the frame is lined up with the beacon
		//move the robot right to center the beacon with the center of the camera frame
		sendMotorCommand(96, 32);
		return 1;
	}

	else if (pixelsFromCenter > 240) { //check to see if the center of the frame is lined up with the beacon
		//move the robot right to center the beacon with the center of the camera frame
		sendMotorCommand(96, 32);
		return 1;
	}

	else if (pixelsFromCenter > 160) { //check to see if the center of the frame is lined up with the beacon
		//move the robot right to center the beacon with the center of the camera frame
		sendMotorCommand(80, 48);
		return 1;
	}

	else if (pixelsFromCenter > 50) { //check to see if the center of the frame is lined up with the beacon
		//move the robot right to center the beacon with the center of the camera frame
		sendMotorCommand(72, 56);
		return 1;
	}

	else if (pixelsFromCenter > -50) { //check to see if the center of the frame is lined up with the beacon
		//move the robot left to center the beacon with the center of the camera frame
		//void sendMotorControllerSpeedBytes(int UART_PORT_ID, int LeftYvalueControllerInput, int RightYvalueControllerInput)
		sendMotorCommand(56, 72);
			return 1;
	}

	else if (pixelsFromCenter > -160) { //check to see if the center of the frame is lined up with the beacon
		//move the robot left to center the beacon with the center of the camera frame
		sendMotorCommand(48, 80);
		return 1;
	}

	else if (pixelsFromCenter > -240) { //check to see if the center of the frame is lined up with the beacon
		//move the robot left to center the beacon with the center of the camera frame
		sendMotorCommand(32, 96);
		return 1;
	}

	else if (pixelsFromCenter > -320) { //check to see if the center of the frame is lined up with the beacon
		//move the robot left to center the beacon with the center of the camera frame
		sendMotorCommand(32, 96);
		return 1;
	}


	return 0;
}
