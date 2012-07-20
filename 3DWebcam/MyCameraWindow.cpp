/**
*  MyCameraWindow.cpp
*
*  This file is part of 3DWebcam
*
*  This class is the main window, where the videos are displayed.
*
*  Author: Nicolas Kniebihler
*
*  Inspired by Alban Perli's article "How to use OpenCV with Qt" :
*  http://alban-perli.over-blog.com/article-how-to-use-opencv-with-qt-67901034.html
*
*  Copyright © 2012. All rights reserved.
*
*/

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "MyCameraWindow.h"
//-------------------------------------------------------------------


MyCameraWindow::MyCameraWindow(blVideoThread2 *rightCam, blVideoThread2 *leftCam, QWidget *parent) :
QMainWindow(parent),
	// Cameras and recorders init
	rightCamera(rightCam),
	leftCamera(leftCam),
	rightWriter(0),
	leftWriter(0)
{
	// Initialisation
	init();

	// Set the calibration matrices
	Q = (CvMat *)cvLoad("matrices/Q.xml",NULL,NULL,NULL);
	mx1 = (CvMat *)cvLoad("matrices/mx1.xml",NULL,NULL,NULL);
	my1 = (CvMat *)cvLoad("matrices/my1.xml",NULL,NULL,NULL);
	mx2 = (CvMat *)cvLoad("matrices/mx2.xml",NULL,NULL,NULL);
	my2 = (CvMat *)cvLoad("matrices/my2.xml",NULL,NULL,NULL);

	// Display menu
	initDispMenu();

	// Options menu
	initOptionMenu();

	// Ready
	statBar->showMessage("Ready");

	// Start the timer (call to timerEvent() every 25ms)
	startTimer(25);
}

MyCameraWindow::MyCameraWindow(blVideoThread2 *cam, QWidget *parent) :
QMainWindow(parent),
	// Cameras and recorders init
	rightCamera(cam),
	leftCamera(NULL),
	rightWriter(0),
	leftWriter(0)
{
	// Initialisation
	init();

	// Options menu
	initOptionMenu();

	// Ready
	statBar->showMessage("Ready");

	//Check if the directory exists
	if(!QDir("video_qp32").exists()) {
		QDir().mkdir("video_qp32");
	}

	int isColor = 1;
	int fps     = 25;
	start->setEnabled(false);
	stop->setEnabled(true);
	menuOpt->setEnabled(false);
	rightWriter = cvCreateVideoWriter("video_qp32/video_0.yuv", CV_FOURCC('I','Y','U','V'), fps, cvSize(WIDTH,HEIGHT), isColor);
	statBar->showMessage("Recording...");
	rightFramesNb = 0;

	// Start the timer (call to timerEvent() every 25ms)
	startTimer(25);
}

MyCameraWindow::~MyCameraWindow(void) {
	cvReleaseMat(&Q);
	cvReleaseMat(&mx1);
	cvReleaseMat(&my1);
	cvReleaseMat(&mx2);
	cvReleaseMat(&my2);
	cvReleaseVideoWriter(&rightWriter);
	cvReleaseVideoWriter(&leftWriter);
	delete selectFile;
}

void MyCameraWindow::timerEvent(QTimerEvent*) {
	/*--- STATS ---*/
	nEx_timerEvent++;
	clock_t beginTimeGlobal = clock();
	ostringstream strStream;
	float t = 0;
	/*-------------*/

	frame_cpt++;
	if(frame_cpt == 15) {
		frame_cpt = 0;
	}

	/*--- STATS ---*/
	nEx_grabFrame++;
	clock_t beginTime = clock();
	/*-------------*/

	// Create IplImages with the defined size
	IplImage* rightTmp = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
	IplImage* leftTmp = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
	IplImage* tmp;

#pragma omp parallel sections
	{
		// Require an image to the left camera
		{
			if(leftCamera != NULL) {
				tmp = cvCloneImage(leftCamera->GetFrame());
				// resize the frame
				cvResize(tmp, leftTmp);
			}
		}
#pragma omp section
		// Require an image to the right camera
		{
			tmp = cvCloneImage(rightCamera->GetFrame());
			// resize the frame
			cvResize(tmp, rightTmp);
		}
	}

	/*--- STATS ---*/
	clock_t endTime = clock();
	t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_grabFrame = ((nEx_grabFrame-1)/nEx_grabFrame) * tEx_grabFrame + (1/nEx_grabFrame) * t;
	/*-------------*/

	// remove distorsions
	if (useCalibration) {
#pragma omp parallel sections
		{
			{ removeDist(leftTmp, mx1, my1); }
#pragma omp section
			{ removeDist(rightTmp, mx2, my2); }
		}
	}

	switch (mode) {
	case NORMAL:
		if (convertYCbCr || leftCamera == NULL) {
			// Convert frame into YCbCr format
			/*--- STATS ---*/
			nEx_bgr2ycrcb++;
			beginTime = clock();
			/*-------------*/

#pragma omp parallel sections
			{
				{
					if(leftCamera != NULL) {
						bgr2ycrcb(leftTmp);
					}
				}
#pragma omp section
				{ bgr2ycrcb(rightTmp); }
			}

			/*--- STATS ---*/
			endTime = clock();
			t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
			tEx_bgr2ycrcb = ((nEx_bgr2ycrcb-1)/nEx_bgr2ycrcb) * tEx_bgr2ycrcb + (1/nEx_bgr2ycrcb) * t;
			/*-------------*/

			if(leftCamera != NULL) {
				// Show the selected layers
				switch (modeYUV) {
				case YUV_MODE:
					break;
				case Y_ONLY:
				case U_ONLY:
				case V_ONLY:
#pragma omp parallel sections
					{
						{
							if(leftCamera != NULL) {
								extractLayer(leftTmp, modeYUV);
							}
						}
#pragma omp section
						{ extractLayer(rightTmp, modeYUV); }
					}
					break;
				}
			}
			else {
				switch (modeRGB) {
				case RGB_MODE:
					break;
				case R_ONLY:
				case G_ONLY:
				case B_ONLY:
#pragma omp parallel sections
					{
						{
							if(leftCamera != NULL) {
								extractLayer(leftTmp, modeRGB);
							}
						}
#pragma omp section
						{ extractLayer(rightTmp, modeRGB); }
					}
					break;
				}
			}
		}
		// Display the frames
		if(leftCamera != NULL) {
			dispFrames(leftTmp, rightTmp);
		}
		else {
			dispFrames(rightTmp);
		}
		break;
	case SPLITED_3D:
		// Display the frames side by side with splited color channels
		// right : only red channel
		// left : green and blue channels (cyan)
		disp3DImageSplited(leftTmp, rightTmp);
		break;
	case MERGED_3D:
		// Display the frames one a top of the other with splited color channels
		disp3DImage(leftTmp, rightTmp);
		break;
	}

	// If it is recording, write the video file
	if((rightWriter != 0 && leftWriter != 0) || (rightWriter != 0 && leftCamera == NULL)) {
		// Increment timer and clock
		if(timer == 0) {
			timer++;
			// Display time
			dispTime(clk);
			clk++;
		}
		else if (timer == 49) {
			timer = 0;
		}
		else {
			timer++;
		}
		
		/*--- STATS ---*/
		nEx_cvWriteFrame++;
		beginTime = clock();
		/*-------------*/

#pragma omp parallel sections
		{
			{
				if(leftCamera != NULL) {
					cvWriteFrame(leftWriter, leftTmp);
				}
			}
#pragma omp section
			{ cvWriteFrame(rightWriter, rightTmp); }
		}

		/*--- STATS ---*/
		endTime = clock();
		t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
		tEx_cvWriteFrame = ((nEx_cvWriteFrame-1)/nEx_cvWriteFrame) * tEx_cvWriteFrame + (1/nEx_cvWriteFrame) * t;
		/*-------------*/

		if(frame_cpt == 0 && rightFramesNb < 20 && leftCamera == NULL) {
			cvReleaseVideoWriter(&rightWriter);
			rightWriter = cvCreateVideoWriter("video_qp32/video.yuv", CV_FOURCC('I','Y','U','V'), 25, cvSize(WIDTH,HEIGHT), 1);

			Thread* t = new Thread((Thread::FuncType)encode);
			t->Launch();
		}

		if(leftCamera != NULL) {
			leftFramesNb++;
		}
		rightFramesNb++;
	}

	// Free resources
	if(leftCamera != NULL) {
		cvReleaseImage(&leftTmp);
	}
	cvReleaseImage(&rightTmp);
	cvReleaseImage(&tmp);
	
	/*--- STATS ---*/
	clock_t endTimeGlobal = clock();
	t = (float)(endTimeGlobal - beginTimeGlobal) / (CLOCKS_PER_SEC/1000);
	tEx_timerEvent = ((nEx_timerEvent-1)/nEx_timerEvent) * tEx_timerEvent + (1/nEx_timerEvent) * t;

	strStream << "grabFrame:\t\t" << tEx_grabFrame << " ms" << endl;
	strStream << "removeDist:\t\t" << tEx_removeDist << " ms" << endl;
	strStream << "bgr2ycrcb:\t\t" << tEx_bgr2ycrcb << " ms" << endl;
	strStream << "extractLayer:\t\t" << tEx_extractLayer << " ms" << endl;
	strStream << "dispFrames:\t\t" << tEx_dispFrames << " ms" << endl;
	strStream << "disp3DImageSplited:\t" << tEx_disp3DImageSplited << " ms" << endl;
	strStream << "disp3DImage:\t\t" << tEx_disp3DImage << " ms" << endl;
	strStream << "cvWriteFrame:\t\t" << tEx_cvWriteFrame << " ms" << endl;
	strStream << "timerEvent:\t\t" << tEx_timerEvent << " ms" << endl;

	output->setText(*(new QString(strStream.str().c_str())));
	/*-------------*/
}

void* encode(void* data) {
	/*--- STATS ---*/
	nEx_encode++;
	clock_t beginTime = clock();
	/*-------------*/

	int argc = 4;
	char** argv = (char**)malloc(sizeof(char*)*argc);
	argv[0] = "H264AVCEncoderLibTestStatic.exe";
	argv[1] = "-vf";
	argv[2] = "config.cfg";
	argv[3] = "0";

	H264AVCEncoderTest*         pcH264AVCEncoderTest = NULL;
	H264AVCEncoderTest::create( pcH264AVCEncoderTest );

	pcH264AVCEncoderTest->init(argc, argv);
	pcH264AVCEncoderTest->go();
	pcH264AVCEncoderTest->destroy();

	free(argv);
	
	/*--- STATS ---*/
	clock_t endTime = clock();
	float t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_encode = ((nEx_encode-1)/nEx_encode) * tEx_encode + (1/nEx_encode) * t;
	/*-------------*/

	return NULL;
}

void MyCameraWindow::startRecording(QString rightFile, QString leftFile) {
	int isColor = 1;
	int fps     = 25;
	start->setEnabled(false);
	stop->setEnabled(true);
	menuOpt->setEnabled(false);
	if (convertYCbCr) {
		rightWriter = cvCreateVideoWriter(rightFile.toStdString().c_str(), CV_FOURCC('I','Y','U','V'), fps, cvSize(WIDTH,HEIGHT), isColor);
		leftWriter = cvCreateVideoWriter(leftFile.toStdString().c_str(), CV_FOURCC('I','Y','U','V'), fps, cvSize(WIDTH,HEIGHT), isColor);
	}
	else {
		rightWriter = cvCreateVideoWriter(rightFile.toStdString().c_str(), CV_FOURCC('P','I','M','1'), fps, cvSize(WIDTH,HEIGHT), isColor);
		leftWriter = cvCreateVideoWriter(leftFile.toStdString().c_str(), CV_FOURCC('P','I','M','1'), fps, cvSize(WIDTH,HEIGHT), isColor);
	}
	statBar->showMessage("Recording...");
	rightFramesNb = 0;
	leftFramesNb = 0;
}

void MyCameraWindow::startRecording(QString file) {
	int isColor = 1;
	int fps     = 25;
	start->setEnabled(false);
	stop->setEnabled(true);
	menuOpt->setEnabled(false);
	if (convertYCbCr) {
		rightWriter = cvCreateVideoWriter(file.toStdString().c_str(), CV_FOURCC('I','Y','U','V'), fps, cvSize(WIDTH,HEIGHT), isColor);
	}
	else {
		rightWriter = cvCreateVideoWriter(file.toStdString().c_str(), CV_FOURCC('P','I','M','1'), fps, cvSize(WIDTH,HEIGHT), isColor);
	}
	statBar->showMessage("Recording...");
	rightFramesNb = 0;
}

void MyCameraWindow::stopRecording() {
	start->setEnabled(true);
	stop->setEnabled(false);
	menuOpt->setEnabled(true);
	cvReleaseVideoWriter(&rightWriter);
	rightWriter = 0;
	ostringstream strStream;
	strStream << "Frames saved : " << rightFramesNb;
	if(leftCamera != NULL) {
		cvReleaseVideoWriter(&leftWriter);
		leftWriter = 0;
		strStream << " (right), " << leftFramesNb << " (left)";
	}
	statBar->showMessage(*(new QString(strStream.str().c_str())));
	timer = 0;
	clk = 0;
}

void MyCameraWindow::dispFrames(IplImage *left, IplImage *right) {
	/*--- STATS ---*/
	nEx_dispFrames++;
	clock_t beginTime = clock();
	/*-------------*/

	// Display frames in the widgets
	leftCVWidget->putImage(left);
	rightCVWidget->putImage(right);
	
	/*--- STATS ---*/
	clock_t endTime = clock();
	float t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_dispFrames = ((nEx_dispFrames-1)/nEx_dispFrames) * tEx_dispFrames + (1/nEx_dispFrames) * t;
	/*-------------*/
}

void MyCameraWindow::dispFrames(IplImage *img) {
	/*--- STATS ---*/
	nEx_dispFrames++;
	clock_t beginTime = clock();
	/*-------------*/

	// Display frame in the widget
	rightCVWidget->putImage(img);
	
	/*--- STATS ---*/
	clock_t endTime = clock();
	float t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_dispFrames = ((nEx_dispFrames-1)/nEx_dispFrames) * tEx_dispFrames + (1/nEx_dispFrames) * t;
	/*-------------*/
}

void MyCameraWindow::disp3DImageSplited(IplImage *left, IplImage *right) {
	/*--- STATS ---*/
	nEx_disp3DImageSplited++;
	clock_t beginTime = clock();
	/*-------------*/

	// Split left frame's color channels (format : BGR)
	CvSize imageSize = cvGetSize(left);
	IplImage* leftB = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage* leftG = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage* leftR = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	cvSplit(left, leftB, leftG, leftR, NULL);

	// Split right frame's color channels (format : BGR)
	imageSize = cvGetSize(right);
	IplImage* rightR = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	cvSplit(right, NULL, NULL, rightR, NULL);

	// Display frames in the widgets
	IplImage* finalLeft = cvCreateImage(imageSize, IPL_DEPTH_8U, 3);
	IplImage* finalRight = cvCreateImage(imageSize, IPL_DEPTH_8U, 3);
	IplImage* empty = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	cvSetZero(empty);
	cvMerge(leftB, leftG, empty, NULL, finalLeft);
	cvMerge(empty, empty, rightR, NULL, finalRight);
	dispFrames(finalLeft, finalRight);

	// Free resources
	cvReleaseImage(&finalLeft);
	cvReleaseImage(&finalRight);
	cvReleaseImage(&leftG);
	cvReleaseImage(&leftR);
	cvReleaseImage(&leftB);
	cvReleaseImage(&rightR);
	cvReleaseImage(&empty);
	
	/*--- STATS ---*/
	clock_t endTime = clock();
	float t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_disp3DImageSplited = ((nEx_disp3DImageSplited-1)/nEx_disp3DImageSplited) * tEx_disp3DImageSplited + (1/nEx_disp3DImageSplited) * t;
	/*-------------*/
}

void MyCameraWindow::disp3DImage(IplImage *left, IplImage *right) {
	/*--- STATS ---*/
	nEx_disp3DImage++;
	clock_t beginTime = clock();
	/*-------------*/

	// Split left frame's color channels (format : BGR)
	CvSize imageSize = cvGetSize(left);
	IplImage* leftB = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage* leftG = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	IplImage* leftR = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	cvSplit(left, leftB, leftG, leftR, NULL);

	// Split right frame's color channels (format : BGR)
	imageSize = cvGetSize(right);
	IplImage* rightR = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
	cvSplit(right, NULL, NULL, rightR, NULL);

	// Display frames in the left widget
	IplImage* final = cvCreateImage(imageSize, IPL_DEPTH_8U, 3);
	cvMerge(leftB, leftG, rightR, NULL, final);
	leftCVWidget->putImage(final);

	// Free resources
	cvReleaseImage(&final);
	cvReleaseImage(&leftG);
	cvReleaseImage(&leftR);
	cvReleaseImage(&leftB);
	cvReleaseImage(&rightR);
	
	/*--- STATS ---*/
	clock_t endTime = clock();
	float t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_disp3DImage = ((nEx_disp3DImage-1)/nEx_disp3DImage) * tEx_disp3DImage + (1/nEx_disp3DImage) * t;
	/*-------------*/
}

void MyCameraWindow::dispTime(int c) {
	ostringstream strStream;
	char* timeStr = (char*)malloc(6*sizeof(char));

	int min = c / 60;
	int sec = c % 60;

	sprintf(timeStr, "%.2d:%.2d", min, sec);
	strStream << "Recording... " << timeStr;
	statBar->showMessage(*(new QString(strStream.str().c_str())));

	free(timeStr);
}

void MyCameraWindow::removeDist(IplImage* img, const CvMat* mx, const CvMat* my) {
	/*--- STATS ---*/
	nEx_removeDist++;
	clock_t beginTime = clock();
	/*-------------*/

	IplImage* tmp = cvCloneImage(img);
	cvRemap(tmp, img, mx, my);
	cvReleaseImage(&tmp);
	
	/*--- STATS ---*/
	clock_t endTime = clock();
	float t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_removeDist = ((nEx_removeDist-1)/nEx_removeDist) * tEx_removeDist + (1/nEx_removeDist) * t;
	/*-------------*/
}

void MyCameraWindow::bgr2ycrcb(IplImage* img) {
	/*--- STATS ---*/
	/*nEx_bgr2ycrcb++;
	clock_t beginTime = clock();*/
	/*-------------*/

	IplImage* tmp = cvCloneImage(img);
	cvCvtColor(tmp, img, CV_BGR2YCrCb);
	cvReleaseImage(&tmp);
	
	/*--- STATS ---*/
	/*clock_t endTime = clock();
	float t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_bgr2ycrcb = ((nEx_bgr2ycrcb-1)/nEx_bgr2ycrcb) * tEx_bgr2ycrcb + (1/nEx_bgr2ycrcb) * t;*/
	/*-------------*/
}

void MyCameraWindow::ycrcb2bgr(IplImage* img) {
	IplImage* tmp = cvCloneImage(img);
	cvCvtColor(tmp, img, CV_YCrCb2BGR);
	cvReleaseImage(&tmp);
}

void MyCameraWindow::extractLayer(IplImage* img, int mode) {
	/*--- STATS ---*/
	nEx_extractLayer++;
	clock_t beginTime = clock();
	/*-------------*/

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

	cvReleaseImage(&tmp);
	cvReleaseImage(&extractedLayer);
	cvReleaseImage(&tmp_layer);
	
	/*--- STATS ---*/
	clock_t endTime = clock();
	float t = (float)(endTime - beginTime) / (CLOCKS_PER_SEC/1000);
	tEx_extractLayer = ((nEx_extractLayer-1)/nEx_extractLayer) * tEx_extractLayer + (1/nEx_extractLayer) * t;
	/*-------------*/
}

/*--- STATS ---*/
void MyCameraWindow::printStats() {
	cout << "Encoding exe time : \t" << tEx_encode << " ms" << endl << endl;

	cout << "grabFrame:\t\t" << tEx_grabFrame << " ms" << endl;
	cout << "removeDist:\t\t" << tEx_removeDist << " ms" << endl;
	cout << "bgr2ycrcb:\t\t" << tEx_bgr2ycrcb << " ms" << endl;
	cout << "extractLayer:\t\t" << tEx_extractLayer << " ms" << endl;
	cout << "dispFrames:\t\t" << tEx_dispFrames << " ms" << endl;
	cout << "disp3DImageSplited:\t" << tEx_disp3DImageSplited << " ms" << endl;
	cout << "disp3DImage:\t\t" << tEx_disp3DImage << " ms" << endl;
	cout << "cvWriteFrame:\t\t" << tEx_cvWriteFrame << " ms" << endl;
	cout << "timerEvent:\t\t" << tEx_timerEvent << " ms" << endl;
}
/*-------------*/

void MyCameraWindow::init() {
	/*--- STATS ---*/
	tEx_timerEvent = 0;
	tEx_grabFrame = 0;
	tEx_removeDist = 0;
	tEx_bgr2ycrcb = 0;
	tEx_extractLayer = 0;
	tEx_dispFrames = 0;
	tEx_disp3DImageSplited = 0;
	tEx_disp3DImage = 0;
	tEx_cvWriteFrame = 0;
	tEx_encode = 0;

	nEx_timerEvent = 0;
	nEx_grabFrame = 0;
	nEx_removeDist = 0;
	nEx_bgr2ycrcb = 0;
	nEx_extractLayer = 0;
	nEx_dispFrames = 0;
	nEx_disp3DImageSplited = 0;
	nEx_disp3DImage = 0;
	nEx_cvWriteFrame = 0;
	nEx_encode = 0;
	/*-------------*/

	// Timer init
	timer = 0;
	clk = 0;
	frame_cpt = 0;

	// Options init
	useCalibration = false;
	convertYCbCr = false;
	mode = NORMAL;
	modeRGB = RGB_MODE;
	modeYUV = YUV_MODE;

	// Write the stats at the end of the program
	QObject::connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(printStats()));

	// Central widget
	QWidget *central = new QWidget;
	setCentralWidget(central);

	// Layouts
	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *cameras = new QHBoxLayout;
	QHBoxLayout *buttons = new QHBoxLayout;

	if(leftCamera != NULL) {
		// Create the left widget
		leftCVWidget = new QOpenCVWidget(this);
		cameras->addWidget(leftCVWidget);
	}

	// Create the right widget
	rightCVWidget = new QOpenCVWidget(this);
	cameras->addWidget(rightCVWidget);

	layout->addLayout(cameras);

	// Create select file window
	if(leftCamera != NULL) {
		selectFile = new SelectFile;
		QObject::connect(selectFile, SIGNAL(fileSelected(QString, QString)), this, SLOT(startRecording(QString, QString)));
	}
	else {
		selectFile = new SelectFile(1);
		QObject::connect(selectFile, SIGNAL(fileSelected(QString)), this, SLOT(startRecording(QString)));
	}

	//Create the buttons
	start = new QPushButton("Start");
	start->setToolTip("Start recording");
	QObject::connect(start, SIGNAL(clicked()), selectFile, SLOT(show()));

	stop = new QPushButton("Stop");
	stop->setToolTip("Stop recording");
	stop->setEnabled(false);
	QObject::connect(stop, SIGNAL(clicked()), this, SLOT(stopRecording()));

	exit = new QPushButton("Exit");
	exit->setToolTip("Exit the program");
	QObject::connect(exit, SIGNAL(clicked()), qApp, SLOT(quit()));

	buttons->addWidget(start);
	buttons->addWidget(stop);
	buttons->addWidget(exit);

	layout->addLayout(buttons);

	central->setLayout(layout);

	// Status bar
	statBar = statusBar();

	// Set the output window
	/*--- STATS ---*/
	output = new QTextEdit;
	output->show();
	/*-------------*/
}

void MyCameraWindow::initOptionMenu() {
	menuOpt = menuBar()->addMenu("&Options");

	// Calibration
	if(leftCamera != NULL) {
		QAction *actUseCali = menuOpt->addAction("&Use the calibration");
		actUseCali->setCheckable(true);
		if(Q == NULL || mx1 == NULL || my1 == NULL || mx2 == NULL || my2 == NULL) {
			actUseCali->setEnabled(false);
		}
		else {
			//actUseCali->setChecked(true);
		}

		// Connect events
		QObject::connect(actUseCali, SIGNAL(toggled(bool)), this, SLOT(useCali(bool)));

		menuOpt->addSeparator();
	}

	// RGB format
	QActionGroup *FormatGroup = new QActionGroup(this);
	QAction *actRGBFormat = menuOpt->addAction("&RGB Format");
	actRGBFormat->setCheckable(true);
	actRGBFormat->setChecked(true);
	actRGBFormat->setActionGroup(FormatGroup);
	QActionGroup *RGBFormatGroup = new QActionGroup(this);
	QAction *actDispRGB = menuOpt->addAction("&Display RGB (default)");
	QAction *actDispROnly = menuOpt->addAction("&Display R only");
	QAction *actDispGOnly = menuOpt->addAction("&Display G only");
	QAction *actDispBOnly = menuOpt->addAction("&Display B only");
	actDispRGB->setCheckable(true);
	actDispROnly->setCheckable(true);
	actDispGOnly->setCheckable(true);
	actDispBOnly->setCheckable(true);
	actDispRGB->setChecked(true);
	actDispRGB->setActionGroup(RGBFormatGroup);
	actDispROnly->setActionGroup(RGBFormatGroup);
	actDispGOnly->setActionGroup(RGBFormatGroup);
	actDispBOnly->setActionGroup(RGBFormatGroup);

	menuOpt->addSeparator();

	// YUV format
	QAction *actYUVFormat = menuOpt->addAction("&YUV Format (YCbCr)");
	actYUVFormat->setCheckable(true);
	actYUVFormat->setActionGroup(FormatGroup);
	QActionGroup *YUVFormatGroup = new QActionGroup(this);
	QAction *actDispYUV = menuOpt->addAction("&Display YUV (default)");
	QAction *actDispYOnly = menuOpt->addAction("&Display Y only");
	QAction *actDispUOnly = menuOpt->addAction("&Display U only");
	QAction *actDispVOnly = menuOpt->addAction("&Display V only");
	actDispYUV->setCheckable(true);
	actDispYOnly->setCheckable(true);
	actDispUOnly->setCheckable(true);
	actDispVOnly->setCheckable(true);
	actDispYUV->setChecked(true);
	actDispYUV->setEnabled(false);
	actDispYOnly->setEnabled(false);
	actDispUOnly->setEnabled(false);
	actDispVOnly->setEnabled(false);
	actDispYUV->setActionGroup(YUVFormatGroup);
	actDispYOnly->setActionGroup(YUVFormatGroup);
	actDispUOnly->setActionGroup(YUVFormatGroup);
	actDispVOnly->setActionGroup(YUVFormatGroup);

	// Connect events
	// RGB format
	QObject::connect(actRGBFormat, SIGNAL(toggled(bool)), actDispYUV, SLOT(setDisabled(bool)));
	QObject::connect(actRGBFormat, SIGNAL(toggled(bool)), actDispYOnly, SLOT(setDisabled(bool)));
	QObject::connect(actRGBFormat, SIGNAL(toggled(bool)), actDispUOnly, SLOT(setDisabled(bool)));
	QObject::connect(actRGBFormat, SIGNAL(toggled(bool)), actDispVOnly, SLOT(setDisabled(bool)));
	QObject::connect(actDispRGB, SIGNAL(triggered()), this, SLOT(setRGBMode()));
	QObject::connect(actDispROnly, SIGNAL(triggered()), this, SLOT(setROnlyMode()));
	QObject::connect(actDispGOnly, SIGNAL(triggered()), this, SLOT(setGOnlyMode()));
	QObject::connect(actDispBOnly, SIGNAL(triggered()), this, SLOT(setBOnlyMode()));
	// YUV format
	QObject::connect(actYUVFormat, SIGNAL(toggled(bool)), this, SLOT(convertYUV(bool)));
	QObject::connect(actYUVFormat, SIGNAL(toggled(bool)), actDispRGB, SLOT(setDisabled(bool)));
	QObject::connect(actYUVFormat, SIGNAL(toggled(bool)), actDispROnly, SLOT(setDisabled(bool)));
	QObject::connect(actYUVFormat, SIGNAL(toggled(bool)), actDispGOnly, SLOT(setDisabled(bool)));
	QObject::connect(actYUVFormat, SIGNAL(toggled(bool)), actDispBOnly, SLOT(setDisabled(bool)));
	QObject::connect(actDispYUV, SIGNAL(triggered()), this, SLOT(setYUVMode()));
	QObject::connect(actDispYOnly, SIGNAL(triggered()), this, SLOT(setYOnlyMode()));
	QObject::connect(actDispUOnly, SIGNAL(triggered()), this, SLOT(setUOnlyMode()));
	QObject::connect(actDispVOnly, SIGNAL(triggered()), this, SLOT(setVOnlyMode()));

	menuOpt->addSeparator();

	// Display mode
	if(leftCamera != NULL) {
		QActionGroup *modeGroup = new QActionGroup(this);
		QAction *actModeNormal = menuOpt->addAction("&Default mode");
		QAction *actMode3DSplited = menuOpt->addAction("&Show right and left color images splited");
		QAction *actMode3DMerged = menuOpt->addAction("&Show right and left color images merged");
		actModeNormal->setCheckable(true);
		actMode3DSplited->setCheckable(true);
		actMode3DMerged->setCheckable(true);
		actModeNormal->setChecked(true);
		actModeNormal->setActionGroup(modeGroup);
		actMode3DSplited->setActionGroup(modeGroup);
		actMode3DMerged->setActionGroup(modeGroup);

		// Connect events
		QObject::connect(actModeNormal, SIGNAL(triggered()), this, SLOT(setNormalMode()));
		QObject::connect(actModeNormal, SIGNAL(triggered()), rightCVWidget, SLOT(show()));
		QObject::connect(actMode3DSplited, SIGNAL(triggered()), this, SLOT(setSplitedMode()));
		QObject::connect(actMode3DSplited, SIGNAL(triggered()), rightCVWidget, SLOT(show()));
		QObject::connect(actMode3DMerged, SIGNAL(triggered()), this, SLOT(setMergedMode()));
		QObject::connect(actMode3DMerged, SIGNAL(triggered()), rightCVWidget, SLOT(hide()));
	}
}

void MyCameraWindow::initDispMenu() {
	QMenu *menuDisplay = menuBar()->addMenu("&Display");
	QActionGroup *disGroup = new QActionGroup(this);
	QAction *actDispR = menuDisplay->addAction("&Display right camera only");
	QAction *actDispL = menuDisplay->addAction("&Display left camera only");
	QAction *actDispBoth = menuDisplay->addAction("&Display both cameras");
	actDispR->setCheckable(true);
	actDispL->setCheckable(true);
	actDispBoth->setCheckable(true);
	actDispBoth->setChecked(true);
	actDispR->setActionGroup(disGroup);
	actDispL->setActionGroup(disGroup);
	actDispBoth->setActionGroup(disGroup);

	// Connect events
	QObject::connect(actDispR, SIGNAL(triggered()), rightCVWidget, SLOT(show()));
	QObject::connect(actDispR, SIGNAL(triggered()), leftCVWidget, SLOT(hide()));
	QObject::connect(actDispL, SIGNAL(triggered()), rightCVWidget, SLOT(hide()));
	QObject::connect(actDispL, SIGNAL(triggered()), leftCVWidget, SLOT(show()));
	QObject::connect(actDispBoth, SIGNAL(triggered()), rightCVWidget, SLOT(show()));
	QObject::connect(actDispBoth, SIGNAL(triggered()), leftCVWidget, SLOT(show()));
}

void MyCameraWindow::useCali(bool b) {
	useCalibration = b;
}

void MyCameraWindow::convertYUV(bool b) {
	convertYCbCr = b;
}

void MyCameraWindow::setNormalMode() {
	mode = NORMAL;
}

void MyCameraWindow::setSplitedMode() {
	mode = SPLITED_3D;
}

void MyCameraWindow::setMergedMode() {
	mode = MERGED_3D;
}

void MyCameraWindow::setRGBMode() {
	modeRGB = RGB_MODE;
}

void MyCameraWindow::setROnlyMode() {
	modeRGB = R_ONLY;
}

void MyCameraWindow::setGOnlyMode() {
	modeRGB = G_ONLY;
}

void MyCameraWindow::setBOnlyMode() {
	modeRGB = B_ONLY;
}

void MyCameraWindow::setYUVMode() {
	modeYUV = YUV_MODE;
}

void MyCameraWindow::setYOnlyMode() {
	modeYUV = Y_ONLY;
}

void MyCameraWindow::setUOnlyMode() {
	modeYUV = U_ONLY;
}

void MyCameraWindow::setVOnlyMode() {
	modeYUV = V_ONLY;
}
