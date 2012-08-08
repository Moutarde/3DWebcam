/**
 *  CalibrationWindow.cpp
 *
 *  This file is part of 3DWebcam
 *
 *  This class is the calibration window.
 *  The user can calibrate two cameras by taking pictures
 *  of the OpenCV "chess board" in different positions.
 *
 *  Author: Nicolas Kniebihler
 *
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "CalibrationWindow.h"
//-------------------------------------------------------------------


CalibrationWindow::CalibrationWindow(VideoHandler* h, QWidget *parent) :
	QWidget(parent),
	handler(h)
{
	num = 1;
	saveReq = false;

	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *cameras = new QHBoxLayout;
	QHBoxLayout *buttons = new QHBoxLayout;
	QHBoxLayout *info = new QHBoxLayout;
	
	// Create the left widget
	leftCVWidget = new QOpenCVWidget(this);
	cameras->addWidget(leftCVWidget);

	// Create the right widget
	rightCVWidget = new QOpenCVWidget(this);
	cameras->addWidget(rightCVWidget);

	layout->addLayout(cameras);

	//Create the buttons
	save = new QPushButton("Save");
	save->setToolTip("Save picture");
	QObject::connect(save, SIGNAL(clicked()), this, SLOT(requireSave()));

	finish = new QPushButton("Finished");
	QObject::connect(finish, SIGNAL(clicked()), this, SLOT(signalEnd()));

	pass = new QPushButton("Use existing settings");
	QObject::connect(pass, SIGNAL(clicked()), this, SLOT(signalEndWithoutCali()));

	buttons->addWidget(save);
	buttons->addWidget(finish);
	buttons->addWidget(pass);

	layout->addLayout(buttons);

	infoText = new QLabel;

	info->addWidget(infoText);

	layout->addLayout(info);

	setLayout(layout);

	// Start the timer (call to timerEvent() every 20ms)
	startTimer(20);
}

CalibrationWindow::~CalibrationWindow(void) {}

void CalibrationWindow::timerEvent(QTimerEvent*) {
	// Display the left frame in the widget
	leftCVWidget->putImage(handler->getFrame(LEFT));

	// Display the right frame in the widget
	rightCVWidget->putImage(handler->getFrame(RIGHT));

	if(saveReq) {
		savePicture(handler->getFrame(RIGHT), handler->getFrame(LEFT));
		saveReq = false;
	}
}

void CalibrationWindow::savePicture(const blImage< blColor3<unsigned char> >& rightTmp, const blImage< blColor3<unsigned char> >& leftTmp) {
	// Check if the directory exists
	if(!QDir("imagenes").exists()) {
		QDir().mkdir("imagenes");
	}

	FILE* list;

	// Delete old files
	if(num == 1) {
		int tmp = 1;
		int res1, res2;
		char* tmpRightFile = (char*)malloc(21*sizeof(char));
		char* tmpLeftFile = (char*)malloc(20*sizeof(char));
		do {
			infoText->setText("Deleting files : " + tmp);
			sprintf(tmpRightFile, "imagenes\\right%.2d.ppm", tmp);
			sprintf(tmpLeftFile, "imagenes\\left%.2d.ppm", tmp);
			res1 = remove(tmpRightFile);
			res2 = remove(tmpLeftFile);
			tmp++;
		} while(!res1 || !res2);
		list = fopen("list.txt", "w+");
		free(tmpRightFile);
		free(tmpLeftFile);
	}
	else {
		list = fopen("list.txt", "a");
	}

	char* rightFile = (char*)malloc(21*sizeof(char));
	char* leftFile = (char*)malloc(20*sizeof(char));

	sprintf(rightFile, "imagenes\\right%.2d.ppm", num);
	sprintf(leftFile, "imagenes\\left%.2d.ppm", num);

	// Save the images
	cvSaveImage(rightFile, rightTmp);
	cvSaveImage(leftFile, leftTmp);

	fprintf(list, "%s\n", rightFile);
	fprintf(list, "%s\n", leftFile);

	infoText->setText("Saved as : " + QString::fromStdString(rightFile) + " and " + QString::fromStdString(leftFile));

	num++;
	free(rightFile);
	free(leftFile);
	fclose(list);
}

void CalibrationWindow::requireSave() {
	saveReq = true;
}

void CalibrationWindow::signalEnd() {
	QMessageBox::information(this, "Calibration running", "Please wait until the calibration is over. This can take a while...");

	StereoCalib("list.txt", 9, 6, 0, 2.5);

	this->deleteLater();
}

void CalibrationWindow::signalEndWithoutCali() {
	this->deleteLater();
}
