/**
 *  MyCameraWindow.h
 *
 *  This file is part of 3DWebcam
 *
 *  Author: Nicolas Kniebihler
 *
 *  Inspired by Alban Perli's article "How to use OpenCV with Qt" :
 *  http://alban-perli.over-blog.com/article-how-to-use-opencv-with-qt-67901034.html
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef MYCAMERAWINDOW_H
#define MYCAMERAWINDOW_H

#define USE_BL_VIDEOTHREAD

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <highgui.h>
#include <omp.h>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QSizePolicy>
#include <QTextEdit>

#include "QOpenCVWidget.h"
#include "SelectFile.h"
#include "blImageAPI/blImageAPI.hpp"
#include "JMVC/H264Extension/src/test/H264AVCEncoderLibTest/H264AVCEncoderLibTest.h"
#include "JMVC/H264Extension/src/test/H264AVCEncoderLibTest/H264AVCEncoderTest.h"
#include "H264AVCCommonLib/CommonBuffers.h"
#include "SFML/Thread.hpp"
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Global constants
//-------------------------------------------------------------------
#define NORMAL		0
#define SPLITED_3D	1
#define MERGED_3D	2

#define YUV_MODE	0
#define Y_ONLY		1
#define U_ONLY		2
#define V_ONLY		3

#define RGB_MODE	4
#define R_ONLY		5
#define G_ONLY		6
#define B_ONLY		7

#define WIDTH	160
#define HEIGHT	120
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Global variables
//-------------------------------------------------------------------
static int timer;
static int clk;
static int frame_cpt;

/*--- STATS ---*/
static float tEx_timerEvent;
static float tEx_grabFrame;
static float tEx_removeDist;
static float tEx_bgr2ycrcb;
static float tEx_extractLayer;
static float tEx_dispFrames;
static float tEx_disp3DImageSplited;
static float tEx_disp3DImage;
static float tEx_cvWriteFrame;
static float tEx_encode;

static float nEx_timerEvent;
static float nEx_grabFrame;
static float nEx_removeDist;
static float nEx_bgr2ycrcb;
static float nEx_extractLayer;
static float nEx_dispFrames;
static float nEx_disp3DImageSplited;
static float nEx_disp3DImage;
static float nEx_cvWriteFrame;
static float nEx_encode;
/*-------------*/
//-------------------------------------------------------------------


using namespace blImageAPI;
using namespace std;
using namespace cv;
using namespace sf;

class MyCameraWindow : public QMainWindow
{
	Q_OBJECT

	private:
		// The widgets where the videos are displayed
		QOpenCVWidget *rightCVWidget;
		QOpenCVWidget *leftCVWidget;
		// Select file window
		SelectFile *selectFile;
		// Status bar
		QStatusBar *statBar;
		// Options menu
		QMenu *menuOpt;
		// Buttons
		QPushButton *start;
		QPushButton *stop;
		QPushButton *exit;
		// This allows to deal with the camera's stream
		blVideoThread2 *rightCamera;
		blVideoThread2 *leftCamera;
		// This allows to record the videos
		CvVideoWriter *rightWriter;
		CvVideoWriter *leftWriter;
		// The number of frames saved
		int rightFramesNb;
		int leftFramesNb;
		// Calibration matrices
		CvMat *Q, *mx1, *my1, *mx2, *my2;
		// Options variables
		bool useCalibration, convertYCbCr;
		int mode, modeRGB, modeYUV;
		// Output window
		/*--- STATS ---*/
		QTextEdit* output;
		/*-------------*/

	public slots:
		void startRecording(QString rightFile, QString leftFile);
		void startRecording(QString file);
		void stopRecording();
		void useCali(bool b);
		void convertYUV(bool b);
		void setNormalMode();
		void setSplitedMode();
		void setMergedMode();
		void setRGBMode();
		void setROnlyMode();
		void setGOnlyMode();
		void setBOnlyMode();
		void setYUVMode();
		void setYOnlyMode();
		void setUOnlyMode();
		void setVOnlyMode();
		/*--- STATS ---*/
		void printStats();
		/*-------------*/
		
	// Functions
	public:
		// Constructor for one camera
		MyCameraWindow(blVideoThread2 *cam, QWidget *parent = 0);
		// Constructor for two cameras
		MyCameraWindow(blVideoThread2 *rightCam, blVideoThread2 *leftCam, QWidget *parent = 0);
		// Destructor
		~MyCameraWindow(void);

		// Initialisation functions
		void init();
		void initOptionMenu();
		void initDispMenu();

		// Display functions
		// Display two frames
		void dispFrames(IplImage *left, IplImage *right);
		// Display one frame
		void dispFrames(IplImage *img);
		// Display cyan and red frames splitted
		void disp3DImageSplited(IplImage *left, IplImage *right);
		// Display cyan and red frames merged
		void disp3DImage(IplImage *left, IplImage *right);

		// Display recording time
		void dispTime(int c);

		// Convert from BGR to YCrCb
		void bgr2ycrcb(IplImage* img);
		// Convert from YCrCb to BGR
		void ycrcb2bgr(IplImage* img);

		// Remove the distortions with the calibration datas
		void removeDist(IplImage* img, const CvMat* mx, const CvMat* my);

		// Extract a color layer
		void extractLayer(IplImage* img, int mode);

	protected:
		// this method displays the image from the camera
		void timerEvent(QTimerEvent*);
};

// Encoding thread
void* encode(void* data);

#endif // MYCAMERAWINDOW_H