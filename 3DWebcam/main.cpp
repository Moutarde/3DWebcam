#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <assert.h>

#include "QOpenCVWidget.h"
#include "MyCameraWindow.h"
#include "CalibrationWindow.h"

int main(int argc, char **argv) {
	// Qt application
	QApplication app(argc, argv);

    // Create the webcam capture device and connect it to the webcam
	// Right
	blVideoThread2* rightVideoThread = new blVideoThread2;
	if(!rightVideoThread->ConnectToWebcam(0)) {
		QMessageBox::critical(NULL, "Problem with the cameras", "The program couldn't find the cameras, and will close now.");
		exit(0);
	}
	// Left
	blVideoThread2* leftVideoThread = new blVideoThread2;
	if(!leftVideoThread->ConnectToWebcam(0)) {
		QMessageBox::critical(NULL, "Problem with the cameras", "The program couldn't find the cameras, and will close now.");
		exit(0);
	}

	// Start the threads
	rightVideoThread->StartCapturingThread();
	leftVideoThread->StartCapturingThread();

	if(rightVideoThread == NULL || leftVideoThread == NULL) {
		QMessageBox::critical(NULL, "Problem with the cameras", "The program couldn't find the cameras, and will close now.");
		exit(0);
	}
	else {
		// Start the calibration window
		CalibrationWindow *caliWin = new CalibrationWindow(rightVideoThread, leftVideoThread);

		caliWin->setWindowTitle("Calibration");

		caliWin->showMaximized();
	}

	int retval = app.exec();

	return retval;
}
