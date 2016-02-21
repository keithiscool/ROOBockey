//#pragma once //only compile this header once

#ifndef DEFS_HPP
#define DEFS_HPP

//#define RaspberryPi2Used 1

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
using namespace cv;
using namespace std;


#define MAX_NUM_OBJECTS 30 // Program will only track 30 objects at a time (this is just in case noise becomes a problem)
#define MIN_OBJECT_AREA 200 //Only allow larger objects //200*200

#define BLUE (Scalar(255, 0, 0)) //BGR, not RGB (I do not know why colors are flipped for OPENCV
#define GREEN (Scalar(0, 255, 0))
#define RED (Scalar(0, 0, 255))
#define YELLOW (Scalar(0, 255, 255))
#define PURPLE (Scalar(255, 0, 255))

#endif /* DEFS_HPP */