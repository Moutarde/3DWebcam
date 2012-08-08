/**
 *  ImageUtil.h
 *
 *  This file is part of 3DWebcam
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <highgui.h>
#include <cv.h>
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Global constants
//-------------------------------------------------------------------
#define YUV_MODE	0
#define Y_ONLY		1
#define U_ONLY		2
#define V_ONLY		3

#define RGB_MODE	4
#define R_ONLY		5
#define G_ONLY		6
#define B_ONLY		7
//-------------------------------------------------------------------


using namespace cv;

// Convert from BGR to YCrCb
void bgr2ycrcb(IplImage* img);
// Convert from YCrCb to BGR
void ycrcb2bgr(IplImage* img);

// Remove the distortions with the calibration datas
void removeDist(IplImage* img, const CvMat* mx, const CvMat* my);

// Extract a color layer
void extractLayer(IplImage* img, int mode);

#endif // IMAGEUTIL_H
