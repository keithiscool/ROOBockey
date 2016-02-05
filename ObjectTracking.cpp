#include "ObjectTracking.hpp"

//This file has functions that detects whether shapes of predetermined color 
//and shape are within the camera frames

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Color Detection Stuff Here*/
/*Shape Detection Stuff At the Bottom*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void on_trackbar(int, void*) {
	// This function gets called whenever a trackbar position is changed
}


//create trackbars and insert them into their own window as sliders to control tracking parameters
void createObjectTrackingParameterTrackbars(void) {
	namedWindow("Trackbars", 0); // create window for trackbars
	char TrackbarName[50]; // create memory to store trackbar name on window
	sprintf(TrackbarName, "ThreshMAX", ThreshMAX);
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);

	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	createTrackbar("CannyThresh", trackbarWindowName, &CannyThresh, ThreshMAX, on_trackbar);
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
}


//filter noise in the HSV image by eroding and dilating the image
//this will prevent false color detection in the image
void morphOps(Mat &thresh) {
	//create structuring element that will be used to filter image using "dilate" and "erode" on the image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8)); //dilate with larger element so make sure object is nicely visible
	erode(thresh, thresh, erodeElement); //eliminate noise
	erode(thresh, thresh, erodeElement);
	dilate(thresh, thresh, dilateElement); //enhance groups of pixels in thresholded image
	dilate(thresh, thresh, dilateElement);
}


//Function is used to filter input BGR image and output contours detected if there are not too many
size_t trackColorFilteredObject(Mat &InputMat, vector<vector<Point>> &contours, vector<Vec4i> hierarchy, Mat &threshold) {

	Mat HSV;
	cvtColor(InputMat, HSV, COLOR_BGR2HSV);
	inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
	morphOps(threshold);

	//find contours in filtered image
	findContours(threshold, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	size_t numObjects = hierarchy.size(); //counts the objects seen after applied threshold
	if ((numObjects > 0) && (numObjects<MAX_NUM_OBJECTS)) { //if number of objects > MAX_NUM_OBJECTS we have a noisy filter
		return numObjects; //function passes if objects are detected, but there are not too many detected objects (from bad filter)
	}
	else putText(threshold, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2); //too many objects after filter
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Shape Detection Stuff Below*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Helper function to find a cosine of angle between vectors from pt0->pt1 and pt0->pt2
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


//Helper function to display text in the center of a contour
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour) {
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;
	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);
	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}


// the function draws all the contours in the image in a new window (colors in BGR, not RGB)
void drawContours(Mat& image, const vector<vector<Point> >& contours, vector<Vec4i> hierarchy, string title) {
	Mat test1, test2,test3,test4;
	size_t i = 0;
	string shape;
	for (i = 0; i < contours.size(); i++) {
		const Point* p = &contours[i][0];
		int n = (int)contours[i].size();
		if (title == "DetectingYellowTriangles") {
			polylines(image, &p, &n, 1, true, YELLOW, 3, LINE_AA); //yellow for triangles
		}
		if (title == "DetectingRedRectangles") {
			//polylines(image, &p, &n, 1, true, RED, 3, LINE_8); //red for rectangles
			drawContours(image, contours, i, RED, FILLED, 8, hierarchy);
			drawContours(test1, contours, i, RED, LINE_8, 8, hierarchy);
			drawContours(test2, contours, i, RED, LINE_4, 8, hierarchy);
			drawContours(test3, contours, i, RED, LINE_AA, 8, hierarchy);
		}
		if (title == "DetectingPurplePentagons") {
			polylines(image, &p, &n, 1, true, PURPLE, 3, LINE_AA); //purple for pentagons
		}
		if (title == "DetectingBlueHexagons") {
			polylines(image, &p, &n, 1, true, BLUE, 3, LINE_AA); //blue for hexagons
		}
		if (title == "DetectingGreenCircles") {
			polylines(image, &p, &n, 1, true, GREEN, 3, LINE_AA); //green for circles
		}
	}
}


// Records the (X,Y) position of each Beacon
void RecordBeaconPosition(Beacon &theBeacon, vector<Vec4i> &hierarchy, vector<vector<Point>> &contours, vector<Beacon> &theBeaconsVector) {
	//for (int index = 0; index >= 0; index = hierarchy[index][0]) {
	size_t numObjects = contours.size(); //counts the number of shapes detected
	for (int index = 0; index < numObjects; index++) {
		Moments moment = moments((Mat)contours[index]);
		double area = moment.m00;
		Point center(moment.m10/area, moment.m01/area); //determine the center of the detectd object using moments

		//if the area is less than 20 px by 20px then it is probably just noise
		//if the area is the same as the 3/2 of the image size, probably just a bad filter
		//we only want the object with the largest area so we save a reference area each
		//iteration and compare it to the area in the next iteration.
		if (area > MIN_OBJECT_AREA) {
			theBeacon.setXPos(center.x);
			theBeacon.setYPos(center.y);
			theBeaconsVector.push_back(theBeacon); //add additional element to the end of the BeaconsVector Vector
			cout<<theBeacon.getShape()<<": "<< theBeacon.getXPos()<<","<<theBeacon.getYPos()<<endl;
		}
	}
}



//Function to locate all contours (shapes) within a given image
void shapeDetection(Mat& inputImage, vector<vector<Point>> contours, vector<Vec4i> hierarchy, Mat& outputImage) {

	Beacon theBeacon;
	vector<Point> approx; //each discovered contour (shape) found from approxPolyDP() function
	Mat imgDrawContours(inputImage.size(), CV_8UC1); //output image
	imgDrawContours = Scalar(255, 255, 255); //white background for output image

	//// Use Canny instead of threshold to catch squares with gradient shading
	//cv::Mat getcontours;
	//cv::Canny(inputGrayscaleImage, getcontours, 0, 50, 5);
	//cv::findContours(getcontours, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		// Approximate contour with accuracy proportional to the contour perimeter
		approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
		size_t vertices = approx.size(); // Number of vertices of polygonal curve
		// Skip small or non-convex objects 
		if ((fabs(contourArea(contours[i])) < 100) || (!isContourConvex(approx)))
			continue;

		// Detect and label triangles (vertices == 3)
		if (vertices == 3) {
			setLabel(imgDrawContours, "TRI", contours[i]);    // Triangles
			drawContours(imgDrawContours, contours, hierarchy, "DetectingTriangles");
			RecordBeaconPosition(YellowTriangle, hierarchy, contours, YellowTrianglesVector);
		}

		if ((vertices >= 4) && (vertices <= 6)) {
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
				setLabel(imgDrawContours, "RECT", contours[i]);
				drawContours(imgDrawContours, contours, hierarchy, "DetectingRectangles");
				RecordBeaconPosition(RedSquare, hierarchy, contours, RedSquaresVector);
			}
			// Detect and label pentagons (vertices == 5)
			//else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27) {
			else if ((vertices == 5) && (mincos >= -0.42) && (maxcos <= -0.17)) { //angles between 100 & 115 are acceptable (108 is ideal)
				setLabel(imgDrawContours, "PENT", contours[i]);
				drawContours(imgDrawContours, contours, hierarchy, "DetectingPentagons");
				RecordBeaconPosition(PurplePentagon, hierarchy, contours, PurplePentagonsVector);
			}
			else if ((vertices == 6) && (mincos >= -0.55) && (maxcos <= -0.45)) {
				setLabel(imgDrawContours, "HEX", contours[i]);
				drawContours(imgDrawContours, contours, hierarchy, "DetectingHexagons");
				RecordBeaconPosition(BlueHexagon, hierarchy, contours, BlueHexagonsVector);
			}
		}
		else {
			// Detect and label circles (vertices > 6)
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if ((abs(1 - ((double)r.width / r.height)) <= 0.2) &&
				(abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)) {
				setLabel(imgDrawContours, "CIR", contours[i]);
				drawContours(imgDrawContours, contours, hierarchy, "DetectingCircles");
				RecordBeaconPosition(GreenCircle, hierarchy, contours, GreenCirclesVector);
			}
		}
	}
}
