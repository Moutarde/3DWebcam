#ifndef QOPENCVWIDGET_H
#define QOPENCVWIDGET_H

#include <cv.h>
#include <QPixmap>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QImage>
#include <stdio.h>

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
		QImage IplImage2QImage(const IplImage *iplImage);

		// Convert QImage to IplImage
		IplImage* qImage2IplImage(const QImage& qImage);

		void putImage(IplImage *);
}; 

#endif // QOPENCVWIDGET_H
