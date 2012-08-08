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
#include <QtGui>

#include "QOpenCVWidget.h"
#include "SelectFile.h"
#include "VideoHandler.h"
#include "blImageAPI/blImageAPI.hpp"
#include "SFML/Thread.hpp"
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Global constants
//-------------------------------------------------------------------
#define NORMAL		0
#define SPLITED_3D	1
#define MERGED_3D	2
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Global variables
//-------------------------------------------------------------------
static int timer;
static int clk;
static int frame_cnt;

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
		// The main vertical layout
		QVBoxLayout* mainLayout;
		// The widgets where the videos are displayed
		QOpenCVWidget *rightCVWidget;
		QOpenCVWidget *leftCVWidget;
		// Select file window
		SelectFile *selectFile;
		// Status bar
		QStatusBar *statBar;
		// Options menu
		QMenu *menuOpt;
		// Extern buttons
		QPushButton *start;
		QPushButton *stop;
		QPushButton *encode;
		QPushButton *exit;
		// This allows to deal with the camera's stream
		VideoHandler* handler;
		// Display mode
		int mode;
		// Output window
		/*--- STATS ---*/
		QTextEdit* output;
		/*-------------*/

	public slots:
		// Recording of the video
		void startRecording(QString rightFile, QString leftFile);
		void startRecording(QString file);
		void stopRecording();

		// Encoding of the video
		void startEncoding();
		
		// Changing of the display mode
		void setNormalMode();
		void setSplitedMode();
		void setMergedMode();

		// Changing of the handler's mode
		void useCali(bool b);
		void convertYUV(bool b);
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
		// Constructor
		MyCameraWindow(VideoHandler* h, QWidget *parent = 0);
		// Destructor
		~MyCameraWindow(void);

		// Get the main layout
		QVBoxLayout* getMainLayout() const;

		// Initialisation functions
		void init();
		void initExternButtonsWindow();
		void initOptionMenu();
		void initDispMenu();

		// Display functions
		// Display two frames
		void dispFrames(const blImage< blColor3<unsigned char> >& left, const blImage< blColor3<unsigned char> >& right);
		// Display one frame
		void dispFrames(const blImage< blColor3<unsigned char> >& img);
		// Display cyan and red frames splitted
		void disp3DImageSplited(const blImage< blColor3<unsigned char> >& left, const blImage< blColor3<unsigned char> >& right);
		// Display cyan and red frames merged
		void disp3DImage(const blImage< blColor3<unsigned char> >& left, const blImage< blColor3<unsigned char> >& right);

		// Display recording time
		void dispTime(int c);
		
		// Call this function to stop the encoding
		void stopEncoding();

	protected:
		// this method displays the image from the camera
		void timerEvent(QTimerEvent*);
};

#endif // MYCAMERAWINDOW_H