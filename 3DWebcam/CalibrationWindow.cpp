#include "CalibrationWindow.h"

CalibrationWindow::CalibrationWindow(blVideoThread2 *rightCam, blVideoThread2 *leftCam, QWidget *parent) : QWidget(parent) {
	rightCamera = rightCam;
	leftCamera = leftCam;
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
	IplImage* rightTmp;
	IplImage* leftTmp;

	#pragma omp parallel sections
	{
		// Require an image to the right camera
		{ rightTmp = cvCloneImage(rightCamera->GetFrame()); }
		#pragma omp section
		// Require an image to the left camera
		{ leftTmp = cvCloneImage(leftCamera->GetFrame()); }
	}

	// Display the right frame in the widget
	rightCVWidget->putImage(rightTmp);

	// Display the left frame in the widget
	leftCVWidget->putImage(leftTmp);

	if(saveReq) {
		savePicture(rightTmp, leftTmp);
		saveReq = false;
	}
	
	// Free resources
	cvReleaseImage(&leftTmp);
	cvReleaseImage(&rightTmp);
}

void CalibrationWindow::savePicture(IplImage *rightTmp, IplImage *leftTmp) {
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
	close();

	QMessageBox::information(this, "Calibration running", "Please wait until the calibration is over. This can take a while...");

	StereoCalib("list.txt", 9, 6, 0, 2.5);

	MyCameraWindow *mainWin = new MyCameraWindow(rightCamera, leftCamera);
	mainWin->setWindowTitle("Camera");
	mainWin->showMaximized();
}

void CalibrationWindow::signalEndWithoutCali() {
	close();

	MyCameraWindow *mainWin = new MyCameraWindow(rightCamera, leftCamera);
	mainWin->setWindowTitle("Camera");
	mainWin->showMaximized();
}
