#ifndef MYCAMERAWINDOW_H
#define MYCAMERAWINDOW_H

#include <iostream>
#include <cstdio>
#include <highgui.h>
#include <pthread.h>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QSizePolicy>
#include <QTextEdit>

#include "QOpenCVWidget.h"
#include "SelectFile.h"

#define NORMAL 0
#define SPLITED_3D 1
#define MERGED_3D 2

#define YUV 0
#define Y_ONLY 1
#define U_ONLY 2
#define V_ONLY 3

#define RGB 4
#define R_ONLY 5
#define G_ONLY 6
#define B_ONLY 7

using namespace std;
using namespace cv;

static int timer;
static int clk;

static float tEx_grabFrame;
static float tEx_removeDist;
static float tEx_bgr2ycrcb;
static float tEx_extractLayer;
static float tEx_dispFrames;
static float tEx_disp3DImageSplited;
static float tEx_disp3DImage;
static float tEx_cvWriteFrame;

static float nEx_grabFrame;
static float nEx_removeDist;
static float nEx_bgr2ycrcb;
static float nEx_extractLayer;
static float nEx_dispFrames;
static float nEx_disp3DImageSplited;
static float nEx_disp3DImage;
static float nEx_cvWriteFrame;

class MyCameraWindow : public QMainWindow
{
	Q_OBJECT

	private:
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
		CvCapture *rightCamera;
		CvCapture *leftCamera;
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
		QTextEdit* output;

	public slots:
		void startRecording(QString rightFile, QString leftFile);
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

	public:
		MyCameraWindow(CvCapture *rightCam, CvCapture *leftCam, QWidget *parent=0);
		~MyCameraWindow(void);
		void dispFrames(IplImage *left, IplImage *right);
		void disp3DImageSplited(IplImage *left, IplImage *right);
		void disp3DImage(IplImage *left, IplImage *right);
		void dispTime(int c);
	protected:
		// this method displays the image from the camera
		void timerEvent(QTimerEvent*);
};

IplImage* removeDist(IplImage* img, CvMat* mx, CvMat* my);
IplImage* bgr2ycrcb(IplImage* img);
IplImage* extractLayer(IplImage* img, int mode);

#endif // MYCAMERAWINDOW_H