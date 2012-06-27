#include <assert.h>

#include "QOpenCVWidget.h"
#include "MyCameraWindow.h"
#include "CalibrationWindow.h"

int main(int argc, char **argv) {
	CvCapture *rightCamera = cvCreateCameraCapture(0);
	assert(rightCamera);
	cvGrabFrame(rightCamera);
	IplImage *rightImage = cvRetrieveFrame(rightCamera);
	assert(rightImage);

	CvCapture * leftCamera = cvCreateCameraCapture(0);
	assert(leftCamera);
	cvGrabFrame(leftCamera);
	IplImage *leftImage = cvRetrieveFrame(leftCamera);
	assert(leftImage);

	QApplication app(argc, argv);

	if(rightCamera == NULL || leftCamera == NULL) {
		QMessageBox::critical(NULL, "Problem with the cameras", "The program couldn't find the cameras, and will close now.");
	}
	else {
		CalibrationWindow *caliWin = new CalibrationWindow(rightCamera, leftCamera);

		caliWin->setWindowTitle("Calibration");

		caliWin->showMaximized();
	}

	int retval = app.exec();

	cvReleaseCapture(&rightCamera);
	cvReleaseCapture(&leftCamera);

	return retval;
}
