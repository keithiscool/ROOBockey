//Originally Written by  Kyle Hounslow 2013 - https://www.youtube.com/watch?v=4KYlHgQQAts
//Modified by Keith Martin 2015-2016 - ROObockey Senior Design Team E - University of Akron : Design of a floor hockey puck shooting robot
//main.cpp - Project Used to Track Various Target Beacons of Different Shapes and Sizes

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

/*NOTE THAT I AM USING OPENCV-Version3.0.0 WITH MICROSOFT VISUAL STUDIO 2013*/
//Installation guide - https://www.youtube.com/watch?v=et7tLwpsADw
//Install setup included in "OpenCV_3_Windows_10_Installation_Tutorial-master" folder within this Github post

/* Object detector program (uses known shapes and colors to track beacons)
* It loads an image and tries to find simple shapes (rectangle, triangle, circle, etc) in it.
* This program is a modified version of `squares.cpp` found in the OpenCV sample dir*/

#include "main.hpp"
#define UsingWebcam 1 //flag that is set to control whether the user uses the camera for input or a still picture as an input

int main(void) {
	bool calibrationMode = true; //if we would like to calibrate our filter values, set to true.
	//bool UseWebcamCameraMode = true; //"false" takes camera input from images located in project directory & "true" takes camera input from webcam
	int CameraNumber = 0; //"camera 0" is the builtin webcam, "camera 1" is usb webcam

	Mat ColorThresholded_Img0, ColorThresholded_Img;
	vector<vector<Point>> contours;
	Mat outputImg;
	vector<Vec4i> hierarchy;
	Mat src, src0;

#ifdef UsingWebcam
	VideoCapture cap(CameraNumber); //Open the Default Camera
	if (!cap.isOpened()) return (-1); //Check if we succeeded in receiving images from camera
	cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); //Set height and width of capture frame
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
#else
	//Testing the program using sample images copied into working project directory
	//Mat src = cv::imread("basic-shapes-2.png");
	src0 = imread("images.png").clone(); //clone used to pass the Mat around in functions as a "deep copy"
	//Mat src = cv::imread("circlesOnWall.png");
	//Mat src = cv::imread("pic3.png");
	//Mat src = cv::imread("pic5.png");
#endif

	createObjectTrackingParameterTrackbars();

	while (1) {

#ifdef UsingWebcam
		cap >> src0; //get a new frame from camera
#endif

		src = src0.clone(); //get a "deep" (physical, not pointer) copy the input video frame
		medianBlur(src, src, 5); //smooth out the image with median filter

		if (calibrationMode == true) {
			Beacon DontStore("TestingObjectDetecting_NOT_Recording_Locations"); //create a dummy beacon that so the user can adjust the HSV color filter and detect a specific colored object
			if ((trackColorFilteredObject(src, contours, hierarchy, ColorThresholded_Img0)) > 0) {//number of objects detected > 0
				ColorThresholded_Img = ColorThresholded_Img0.clone();
				shapeDetection(ColorThresholded_Img, contours, hierarchy, outputImg); //search for shapes in the color filtered thresholded image
			}
		} else{
			if ((trackColorFilteredObject(src, contours, hierarchy, ColorThresholded_Img0)) > 0) { //number of objects detected > 0
				ColorThresholded_Img = ColorThresholded_Img0.clone();
				shapeDetection(ColorThresholded_Img, contours, hierarchy, outputImg); //search for shapes in the color filtered thresholded image
			}
		}

		imshow("RGBInput_src", src); //show Output Mat video frame in new window
		imshow("ColorThresholdedImg", ColorThresholded_Img0); //show Output Mat video frame in new window
		imshow("OutputColor&ShapeDetectedImg", ColorThresholded_Img);
		waitKey(30); //delay 30ms so that screen can refresh. image will not appear without this waitKey() command
	}

	//cv::imshow("src", src);
	cv::waitKey(5); //display output until keypress
	return 0;
}



//int main(void) {
//	//Testing the program using sample images copied into working project directory
//	//Mat src = cv::imread("basic-shapes-2.png");
//	Mat src = imread("images.png");
//	//Mat src = cv::imread("circlesOnWall.png");
//	//Mat src = cv::imread("pic3.png");
//	//Mat src = cv::imread("pic5.png");
//
//	Mat gray;
//	Mat outputDetectedShapesImage;
//	Mat threshold;
//	Mat HSV;
//
//	// Use this when testing with webcams (actual video testing)
//	//VideoCapture src(CameraNumber); //Open the Default Camera
//	//if (!src.isOpened()) return (-1); //Check if we succeeded in receiving images from camera
//	//cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); //Set height and width of capture frame
//	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
//
//	createObjectTrackingParameterTrackbars();
//
//	medianBlur(src, src, 5); //Smooth out the image with median filter
//	cvtColor(src, HSV, COLOR_BGR2HSV); //Convert BGR to HSV
//	inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
//	morphOps(threshold);
//
//	trackFilteredObject(DontStore, threshold, HSV, imgOriginal);
//
//	cv::cvtColor(src, gray, CV_BGR2GRAY); //Convert to grayscale
//	shapeDetection(gray, outputDetectedShapesImage);
//
//	cv::imshow("src", src);
//	//cv::imshow("outputDetectedShapesImage", outputDetectedShapesImage);
//	cv::waitKey(0); //display output until keypress
//	//cv::waitKey(25); //display output for 25milliseconds
//	return 0;
//}