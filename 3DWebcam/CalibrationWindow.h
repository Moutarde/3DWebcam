#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <iostream>

#include "StereoCalib.h"
#include "MyCameraWindow.h"

using namespace std;
using namespace cv;

class CalibrationWindow : public QWidget {
	Q_OBJECT

	public:
		CalibrationWindow(blVideoThread2 *rightCam, blVideoThread2 *leftCam, QWidget *parent=0);
		~CalibrationWindow(void);
		void savePicture(IplImage *rightTmp, IplImage *leftTmp);

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
		blVideoThread2 *rightCamera;
		blVideoThread2 *leftCamera;
		// Buttons
		QPushButton *save;
		QPushButton *finish;
		QPushButton *pass;
};

#endif // CALIBRATIONWINDOW_H