//Originally Written by  Kyle Hounslow 2013 - https://www.youtube.com/watch?v=4KYlHgQQAts
//Modified by Keith Martin 2015-2016 - ROObockey Senior Design Team E - University of Akron : Design of a floor hockey puck shooting robot
//main.cpp - Project Used to Track Various Target Beacons of Different Shapes and Colors

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so.

/*NOTE THAT I AM USING OPENCV-Version3.0.0 WITH MICROSOFT VISUAL STUDIO 2013*/
//Installation guide - https://www.youtube.com/watch?v=et7tLwpsADw
//OpenCV3.0.0 Install setup is included in "OpenCV_3_Windows_10_Installation_Tutorial-master" folder within this Github post

/* Object detector program (uses known shapes and colors to track beacons)
* It loads an image and tries to find simple shapes (rectangle, triangle, circle, etc) in it.
* This program is a modified version of `squares.cpp` found in the OpenCV sample dir*/


#define USE_EXTERNS
#define MAIN_CPP
#include "defs.hpp"
#include "ObjectTracking.hpp"
//////////////#include "main.hpp"
#ifdef RaspberryPi2Used
	#include "UART.h"
#endif



void main(void) {

	Mat src0;
	Mat ColorThresholded_Img0, ColorThresholded_Img, outputImg0, outputImg, src;
	vector<vector<Point>> contours;
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
#endif USING_WEBCAM


#ifdef CALIBRATION_MODE
	createObjectTrackingParameterTrackbars();
	const string mouseWindowName = "Mouse Operations";
	//create a window before setting mouse callback
	namedWindow(mouseWindowName);
	//set mouse callback function to be active on "Webcam Feed" window
	//we pass the handle to our "frame" matrix so that we can draw a rectangle to it as the user clicks and drags the mouse
	//NOTE: THE "OnMouse" function parameter for "setMouseCallback()" for setMouseCallback is a function with parameters: (int, int, int, void*);
	setMouseCallback(mouseWindowName, clickAndDragRectangle, &src0);
	Mat HSV_Calibration;
#endif CALIBRATION_MODE
	

	while (1) {


#ifdef USING_WEBCAM
		cap >> src0; //get a new frame from camera
#endif USING_WEBCAM

		src = src0.clone(); //get a "deep copy" (physical, not pointer) copy the input video frame

#ifdef CALIBRATION_MODE 
		cvtColor(src, HSV_Calibration, COLOR_BGR2HSV);
		//set HSV values from user selected region
		mouseRecordHSV_Values(src, HSV_Calibration);

		putText(outputImg0, "CalibrationMode", Point((FRAME_WIDTH / 2), (FRAME_HEIGHT-3)), 1, 2, Scalar(0, 0, 255), 2); //put text in output window
			
		Beacon HSV_Tuning("TestingObjectDetecting_NOT_Recording_Locations"); //create a dummy beacon that so the user can adjust the HSV color filter and detect a specific colored object
		if ((calibratingTrackColorFilteredObjects(src, contours, hierarchy, ColorThresholded_Img0)) > 0) {//number of objects detected > 0
			ColorThresholded_Img = ColorThresholded_Img0.clone(); //had to clone the image to pass a "deep copy" to the shape detection function
			shapeDetection(ColorThresholded_Img, contours, hierarchy, outputImg0); //search for shapes in the color filtered thresholded image
			outputImg = outputImg0.clone(); //had to clone the image to pass a "deep copy" to the output "imshow"
		}
#endif CALIBRATION_MODE

#ifndef CALIBRATION_MODE
		if ((trackColorFilteredObjects(src, YellowTrianglesVector, contours, hierarchy, ColorThresholded_Img0)) > 0) { //number of objects detected > 0
			ColorThresholded_Img = ColorThresholded_Img0.clone(); //had to clone the image to pass a "deep copy" to the shape detection function
			shapeDetection(ColorThresholded_Img, contours, hierarchy, outputImg0); //search for shapes in the color filtered thresholded image
			outputImg = outputImg0.clone(); //had to clone the image to pass a "deep copy" to the output "imshow"
		}
#endif CALIBRATION_MODE

		imshow("RGBInput_src", src); //show Output Mat video frame in new window
		imshow("ColorThresholdedImg", ColorThresholded_Img0); //show Output Mat video frame in new window
		imshow("OutputColor&ShapeDetectedImg", ColorThresholded_Img);
		imshow("OutputImg", outputImg);
		waitKey(30); //delay in milliseconds so OpenCV does not consume all processor time. "imshow" will not appear without this waitKey() command
	}
}
