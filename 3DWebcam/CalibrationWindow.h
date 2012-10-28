/**
 *  CalibrationWindow.h
 *
 *  This file is part of 3DWebcam
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <QtGui>
#include <iostream>

#include "StereoCalib.h"
#include "VideoThread.h"
#include "VideoHandler.h"
//-------------------------------------------------------------------


using namespace std;
using namespace cv;

class CalibrationWindow : public QWidget {
	Q_OBJECT

	public:
		CalibrationWindow(VideoHandler* h, QWidget *parent = 0);
		~CalibrationWindow(void);
		void savePicture(const blImage< blColor3<unsigned char> >& rightTmp, const blImage< blColor3<unsigned char> >& leftTmp);

	protected:
		// this method displays the image from the camera
		void timerEvent(QTimerEvent*);
	
	signals:
		void finished();

	public slots:
		void signalEnd();
		void signalEndWithoutCali();
		void requireSave();

	private:
		int num;
		bool saveReq;
		QLabel *infoText;
		QOpenCVWidget *rightCVWidget;
		QOpenCVWidget *leftCVWidget;
		// This allows to deal with the camera's stream
		VideoHandler* handler;
		// Buttons
		QPushButton *save;
		QPushButton *finish;
		QPushButton *pass;
};

#endif // CALIBRATIONWINDOW_H