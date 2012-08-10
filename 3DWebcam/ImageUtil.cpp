/**
 *  VideoHandler.cpp
 *
 *  This file is part of 3DWebcam
 *	
 *	This file contains some functions that modify IplImages :
 *		- calibration
 *		- format conversion
 *		- layer extraction
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "ImageUtil.h"
//-------------------------------------------------------------------


void removeDist(IplImage* img, const CvMat* mx, const CvMat* my) {
	IplImage* tmp = cvCloneImage(img);
	cvRemap(tmp, img, mx, my);
	cvReleaseImage(&tmp);
}

void bgr2ycrcb(IplImage* img) {
	IplImage* tmp = cvCloneImage(img);
	cvCvtColor(tmp, img, CV_BGR2YCrCb);
	cvReleaseImage(&tmp);
}

void ycrcb2bgr(IplImage* img) {
	IplImage* tmp = cvCloneImage(img);
	cvCvtColor(tmp, img, CV_YCrCb2BGR);
	cvReleaseImage(&tmp);
}

void extractLayer(IplImage* img, int mode) {
	CvSize imageSize = cvGetSize(img);
	IplImage* tmp = cvCloneImage(img);
	IplImage* extractedLayer = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage* tmp_layer = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	cvSetZero(tmp_layer);

	switch(mode) {
	case Y_ONLY:
		cvSplit(tmp, extractedLayer, NULL, NULL, NULL);
		cvMerge(extractedLayer, extractedLayer, extractedLayer, NULL, img);
		break;
	case U_ONLY:
		cvSplit(tmp, NULL, extractedLayer, NULL, NULL);
		cvMerge(extractedLayer, extractedLayer, extractedLayer, NULL, img);
		break;
	case V_ONLY:
		cvSplit(tmp, NULL, NULL, extractedLayer, NULL);
		cvMerge(extractedLayer, extractedLayer, extractedLayer, NULL, img);
		break;
	case B_ONLY:
		cvSplit(tmp, extractedLayer, NULL, NULL, NULL);
		cvMerge(extractedLayer, tmp_layer, tmp_layer, NULL, img);
		break;
	case G_ONLY:
		cvSplit(tmp, NULL, extractedLayer, NULL, NULL);
		cvMerge(tmp_layer, extractedLayer, tmp_layer, NULL, img);
		break;
	case R_ONLY:
		cvSplit(tmp, NULL, NULL, extractedLayer, NULL);
		cvMerge(tmp_layer, tmp_layer, extractedLayer, NULL, img);
		break;
	}

	// Free resources
	cvReleaseImage(&tmp);
	cvReleaseImage(&extractedLayer);
	cvReleaseImage(&tmp_layer);
}

void merge3DImage(const IplImage* left, const IplImage* right, IplImage* result) {
	// Verify the size
	CvSize imageSize = cvGetSize(left);
	if(imageSize.height != cvGetSize(right).height || imageSize.width != cvGetSize(right).width) {
		return;
	}

	// Split left frame's color channels (format : BGR)
	IplImage* leftB = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage* leftG = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage* leftR = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	cvSplit(left, leftB, leftG, leftR, NULL);

	// Split right frame's color channels (format : BGR)
	IplImage* rightR = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	cvSplit(right, NULL, NULL, rightR, NULL);

	// Display frames in the left widget
	cvMerge(leftB, leftG, rightR, NULL, result);

	// Free resources
	cvReleaseImage(&leftG);
	cvReleaseImage(&leftR);
	cvReleaseImage(&leftB);
	cvReleaseImage(&rightR);
}
