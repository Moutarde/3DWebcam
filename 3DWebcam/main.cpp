/**
 *  main.c
 *
 *  This file is part of 3DWebcam
 *
 *  This is the main file.
 *  It is the base of the program, and contains the main function.
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <assert.h>
#include <QStringList>
#include <QInputDialog>

#include "QOpenCVWidget.h"
#include "MyCameraWindow.h"
#include "CalibrationWindow.h"
//-------------------------------------------------------------------


/**
 *	This is the main function.
 *	This function is called when the program is launched.
 */
int main(int argc, char **argv) {
	// Qt application
	QApplication app(argc, argv);

	// Ask the user how many cameras he wants to use
	QStringList list;		// list of the diferent choices the user can make
	list << "Only one camera (classic)" << "Two cameras (3D stéréoscopy)";
	bool ok = false;
	QString nbCam = QInputDialog::getItem(NULL, "Mode", "How many cameras do you want to use ?", list, 0, false, &ok);

	if(ok && !nbCam.isEmpty()) {		// If ok was pressed and a choice was made
		if(nbCam == "Only one camera (classic)") {		// The user choosed only one camera
			// Create the webcam capture device and connect it to the webcam
			blVideoThread2* videoThread = new blVideoThread2;
			if(!videoThread->ConnectToWebcam(0)) {
				QMessageBox::critical(NULL, "Problem with the camera", "The program couldn't find the camera, and will close now.");
				exit(0);
			}

			// Start the thread
			// Now the capturing runs by itself, in another thread
			videoThread->StartCapturingThread();

			if(videoThread == NULL) {
				QMessageBox::critical(NULL, "Problem with the camera", "The program couldn't find the camera, and will close now.");
				exit(0);
			}
			else {
				// Start the camera window
				MyCameraWindow *mainWin = new MyCameraWindow(videoThread);
				mainWin->setWindowTitle("Camera");
				mainWin->showMaximized();
			}
		}
		else if(nbCam == "Two cameras (3D stéréoscopy)") {
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
			// Now the capturing runs by itself, in two different threads
			rightVideoThread->StartCapturingThread();
			leftVideoThread->StartCapturingThread();

			if(rightVideoThread == NULL || leftVideoThread == NULL) {
				QMessageBox::critical(NULL, "Problem with the cameras", "The program couldn't find the cameras, and will close now.");
				exit(0);
			}
			else {
				// Ask the user if he wants to calibrate the cameras
				int calibrate = QMessageBox::question(NULL, "Calibration", "Do you want to calibrate the cameras ?", QMessageBox::Yes | QMessageBox::No);

				if (calibrate == QMessageBox::Yes) {
					// Start the calibration window
					CalibrationWindow *caliWin = new CalibrationWindow(rightVideoThread, leftVideoThread);
					caliWin->setWindowTitle("Calibration");
					caliWin->showMaximized();
				}
				else if (calibrate == QMessageBox::No) {
					// Start the camera window
					MyCameraWindow *mainWin = new MyCameraWindow(rightVideoThread, leftVideoThread);
					mainWin->setWindowTitle("Camera");
					mainWin->showMaximized();
				}
			}
		}
	}
	else {
		exit(0);
	}

	int retval = app.exec();

	return retval;
}
