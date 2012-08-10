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


MyCameraWindow::MyCameraWindow(VideoHandler* h, QWidget *parent) :
	QMainWindow(parent),
	// Handler init
	handler(h)
{
	// Initialisation
	init();

	// Initialise the extern buttons window
	initExternButtonsWindow();

	if(handler->getNbCam() == 2) {
		// Display menu
		initDispMenu();
	}

	// Options menu
	initOptionMenu();

	// Ready
	statBar->showMessage("Ready");

	// Start the timer (call to timerEvent() every 25ms)
	startTimer(25);
}

MyCameraWindow::~MyCameraWindow(void) {
	delete selectFile;
}

void MyCameraWindow::timerEvent(QTimerEvent*) {
	// We want to know the display mode
	switch (mode) {
		case NORMAL:
			// Display the frames
			if(handler->getNbCam() == 2) {	// If there is two cameras
				dispFrames(handler->getFrame(LEFT), handler->getFrame(RIGHT));
			}
			else {
				dispFrames(handler->getFrame(ALONE));
			}
			break;
		case SPLITED_3D:
			// Display the frames side by side with splited color channels
			// right : only red channel
			// left : green and blue channels (cyan)
			disp3DImageSplited(handler->getFrame(LEFT), handler->getFrame(RIGHT));
			break;
		case MERGED_3D:
			// Display the frames one a top of the other with splited color channels
			disp3DImage(handler->getFrame(LEFT), handler->getFrame(RIGHT));
			break;
	}

	if(handler->isRecording()) {	// If it is recording
		// Increment timer and clock
		if(timer == 0) {
			timer++;
			// Display time
			dispTime(clk);
			clk++;
		}
		else if (timer == 39) {
			timer = 0;
		}
		else {
			timer++;
		}
		
		// Write the video file(s)
		handler->saveFrame();
	}
	else if(handler->isEncoding()) {	// If it is encoding
		frame_cnt++;
		
		// Write the video file(s)
		handler->saveFrame();
		
		// Test if we have saved 15 frames, and stop the encoding if it is the case
		if(frame_cnt == 15 && handler->getFrameNb() < 20) {
			stopEncoding();
		}
	}
}

void MyCameraWindow::startRecording(QString rightFile, QString leftFile) {
	// Enable and disable buttons
	start->setEnabled(false);
	stop->setEnabled(true);
	encode->setEnabled(false);
	menuOpt->setEnabled(false);

	// Create a vector with the names of the files
	// where we want to save the videos
	vector<QString> files;
	files.push_back(rightFile);
	files.push_back(leftFile);

	handler->startRecording(files);

	statBar->showMessage("Recording...");
}

void MyCameraWindow::startRecording(QString file) {
	start->setEnabled(false);
	stop->setEnabled(true);
	encode->setEnabled(false);
	menuOpt->setEnabled(false);

	vector<QString> files;
	files.push_back(file);

	handler->startRecording(files);

	statBar->showMessage("Recording...");
}

void MyCameraWindow::stopRecording() {
	start->setEnabled(true);
	stop->setEnabled(false);
	encode->setEnabled(true);
	menuOpt->setEnabled(true);

	int framesNb = handler->stopRecording();

	// Display the number of frames that have been saved
	ostringstream strStream;
	strStream << "Frames saved : " << framesNb;

	statBar->showMessage(*(new QString(strStream.str().c_str())));

	timer = 0;
	clk = 0;
}

void MyCameraWindow::startEncoding() {
	start->setEnabled(false);
	stop->setEnabled(false);
	encode->setEnabled(false);
	menuOpt->setEnabled(false);

	handler->startEncoding();
	
	statBar->showMessage("Encoding...");
}

void MyCameraWindow::stopEncoding() {
	start->setEnabled(true);
	stop->setEnabled(false);
	encode->setEnabled(true);
	menuOpt->setEnabled(true);

	handler->stopEncoding();

	statBar->showMessage("Ready");
	frame_cnt = 0;
}

void MyCameraWindow::dispFrames(const blImage< blColor3<unsigned char> >& left, const blImage< blColor3<unsigned char> >& right) {
	// Display frames in the widgets
	leftCVWidget->putImage(left);
	rightCVWidget->putImage(right);
}

void MyCameraWindow::dispFrames(const blImage< blColor3<unsigned char> >& img) {
	// Display frame in the widget
	rightCVWidget->putImage(img);
}

void MyCameraWindow::disp3DImageSplited(const blImage< blColor3<unsigned char> >& left, const blImage< blColor3<unsigned char> >& right) {
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
}

void MyCameraWindow::disp3DImage(const blImage< blColor3<unsigned char> >& left, const blImage< blColor3<unsigned char> >& right) {
	IplImage* result = cvCreateImage(cvGetSize(left), left.GetDepth(), left.GetNumOfChannels());
	merge3DImage(left, right, result);

	leftCVWidget->putImage(result);

	// Free resources
	cvReleaseImage(&result);
}

void MyCameraWindow::dispTime(int c) {
	ostringstream strStream;
	char* timeStr = (char*)malloc(6*sizeof(char));

	int min = c / 60;	// minutes
	int sec = c % 60;	// seconds

	sprintf(timeStr, "%.2d:%.2d", min, sec);
	strStream << "Recording... " << timeStr;
	statBar->showMessage(*(new QString(strStream.str().c_str())));

	free(timeStr);
}

void MyCameraWindow::init() {
	// Timer init
	timer = 0;
	clk = 0;
	frame_cnt = 0;

	// Write the stats at the end of the program
	QObject::connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(printStats()));

	// Central widget
	QWidget *central = new QWidget;
	setCentralWidget(central);

	// Layouts
	mainLayout = new QVBoxLayout;
	QHBoxLayout *cameras = new QHBoxLayout;

	if(handler->getNbCam() == 2) {
		// Create the left widget
		leftCVWidget = new QOpenCVWidget(this);
		cameras->addWidget(leftCVWidget);
	}

	// Create the right widget
	rightCVWidget = new QOpenCVWidget(this);
	cameras->addWidget(rightCVWidget);

	mainLayout->addLayout(cameras);

	// Create select file window
	if(handler->getNbCam() == 2) {
		selectFile = new SelectFile;
		QObject::connect(selectFile, SIGNAL(fileSelected(QString, QString)), this, SLOT(startRecording(QString, QString)));
	}
	else {
		selectFile = new SelectFile(1);
		QObject::connect(selectFile, SIGNAL(fileSelected(QString)), this, SLOT(startRecording(QString)));
	}

	central->setLayout(mainLayout);

	// Status bar
	statBar = statusBar();

	// Set the output window
	/*output = new QTextEdit;
	output->setWindowTitle("OUTPUT");
	output->show();*/
}

void MyCameraWindow::initExternButtonsWindow() {
	QWidget* externButtonsWindow = new QWidget;
	externButtonsWindow->setWindowTitle("Extern Buttons");

	QVBoxLayout *layout = new QVBoxLayout;

	QLabel* label = new QLabel("These are some buttons you can use\nto control and test the application :");
	layout->addWidget(label);
	
	QHBoxLayout* frameLayout = new QHBoxLayout;

	QFrame* buttonsFrame = new QFrame;
    buttonsFrame->setFrameShape(QFrame::StyledPanel);

	QVBoxLayout *buttons = new QVBoxLayout;

	//Create the buttons
	start = new QPushButton("Start");
	start->setToolTip("Start recording");
	start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QObject::connect(start, SIGNAL(clicked()), selectFile, SLOT(show()));

	stop = new QPushButton("Stop");
	stop->setToolTip("Stop recording");
	stop->setEnabled(false);
	stop->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QObject::connect(stop, SIGNAL(clicked()), this, SLOT(stopRecording()));

	encode = new QPushButton("Encode");
	encode->setToolTip("Encode 15 frames");
	encode->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QObject::connect(encode, SIGNAL(clicked()), this, SLOT(startEncoding()));

	exit = new QPushButton("Exit");
	exit->setToolTip("Exit the program");
	exit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QObject::connect(exit, SIGNAL(clicked()), qApp, SLOT(quit()));

	// Add the buttons to the widget
	buttons->addWidget(start);
	buttons->addWidget(stop);
	buttons->addWidget(encode);
	buttons->addWidget(exit);

	buttonsFrame->setLayout(buttons);

	frameLayout->addStretch();
	frameLayout->addWidget(buttonsFrame);
	frameLayout->addStretch();

	layout->addLayout(frameLayout);

	externButtonsWindow->setLayout(layout);
	externButtonsWindow->show();
}

void MyCameraWindow::initOptionMenu() {
	menuOpt = menuBar()->addMenu("&Options");

	// Calibration
	if(handler->getNbCam() == 2) {
		QAction *actUseCali = menuOpt->addAction("&Use the calibration");
		actUseCali->setCheckable(true);
		//actUseCali->setChecked(true);

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
	if(handler->getNbCam() == 2) {
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

QVBoxLayout* MyCameraWindow::getMainLayout() const {
	return mainLayout;
}

void MyCameraWindow::useCali(bool b) {
	handler->useCali(b);
}

void MyCameraWindow::convertYUV(bool b) {
	handler->convertYUV(b);
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
	handler->setMode(RGB_MODE);
}

void MyCameraWindow::setROnlyMode() {
	handler->setMode(R_ONLY);
}

void MyCameraWindow::setGOnlyMode() {
	handler->setMode(G_ONLY);
}

void MyCameraWindow::setBOnlyMode() {
	handler->setMode(B_ONLY);
}

void MyCameraWindow::setYUVMode() {
	handler->setMode(YUV_MODE);
}

void MyCameraWindow::setYOnlyMode() {
	handler->setMode(Y_ONLY);
}

void MyCameraWindow::setUOnlyMode() {
	handler->setMode(U_ONLY);
}

void MyCameraWindow::setVOnlyMode() {
	handler->setMode(V_ONLY);
}
