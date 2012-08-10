/**
 *  QOpenCVWidget.h
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

#ifndef QOPENCVWIDGET_H
#define QOPENCVWIDGET_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <cv.h>
#include <QtGui>
#include <stdio.h>
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Global constants
//-------------------------------------------------------------------
#define DISPLAY_WIDTH	320
#define DISPLAY_HEIGHT	240
//-------------------------------------------------------------------


class QOpenCVWidget : public QWidget {
	private:
		QLabel *imagelabel;
		QVBoxLayout *layout;
		QHBoxLayout *hLayout;
		// Image from the camera
		QImage image;

	public:
		QOpenCVWidget(QWidget *parent = 0);
		~QOpenCVWidget(void);

		// Convert IplImage to QImage
		QImage IplImage2QImage(const IplImage* iplImage);

		// Convert QImage to IplImage
		IplImage* qImage2IplImage(const QImage& qImage);

		void putImage(const IplImage*);
		void setImage(QImage img);
};

#endif // QOPENCVWIDGET_H
