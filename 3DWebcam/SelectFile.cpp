/**
 *  SelectFile.cpp
 *
 *  This file is part of 3DWebcam
 *
 *  This is a window that allows the user to select the paths
 *  where he wants to save the video files.
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "SelectFile.h"
//-------------------------------------------------------------------


SelectFile::SelectFile(int nbCam, QWidget *parent) : QWidget(parent), nbCam(nbCam) {
	QVBoxLayout *sFLayout = new QVBoxLayout;
	QFrame *dialog = new QFrame;
    dialog->setFrameShape(QFrame::StyledPanel);
	QVBoxLayout *dialLayout = new QVBoxLayout;
	QHBoxLayout *rightPath = new QHBoxLayout;
	QHBoxLayout *leftPath = new QHBoxLayout;
	QHBoxLayout *buttons = new QHBoxLayout;
	
	QLabel* text;
	if(nbCam == 1) {
		text = new QLabel("Select the path to the video file :");
	}
	else {
		text = new QLabel("Select the paths to the video files :");
	}
	dialLayout->addWidget(text);

	// Right path
	QLabel* rightText;
	if(nbCam == 1) {
		rightText = new QLabel("Path :");
	}
	else if(nbCam == 2) {
		rightText = new QLabel("Right video :");
	}
	rightLineEdit = new QLineEdit;
	rightLineEdit->setMinimumWidth(300);
    QPushButton *rightBrowse = new QPushButton("Browse...");
	QObject::connect(rightBrowse, SIGNAL(clicked()), this, SLOT(rightSaveAs()));
	rightPath->addWidget(rightText);
	rightPath->addWidget(rightLineEdit);
	rightPath->addWidget(rightBrowse);
	
	// Left path
	if(nbCam == 2) {
		QLabel* leftText = new QLabel("Left video :");
		leftLineEdit = new QLineEdit;
		leftLineEdit->setMinimumWidth(300);
		QPushButton *leftBrowse = new QPushButton("Browse...");
		QObject::connect(leftBrowse, SIGNAL(clicked()), this, SLOT(leftSaveAs()));
		leftPath->addWidget(leftText);
		leftPath->addWidget(leftLineEdit);
		leftPath->addWidget(leftBrowse);
	}

	dialLayout->addLayout(rightPath);
	if(nbCam == 2) {
		dialLayout->addLayout(leftPath);
	}
	dialog->setLayout(dialLayout);
	sFLayout->addWidget(dialog);

	QPushButton *enter = new QPushButton("OK");
	QObject::connect(enter, SIGNAL(clicked()), this, SLOT(validate()));
	buttons->addWidget(enter);

	QPushButton *cancel = new QPushButton("Cancel");
	QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(hide()));
	buttons->addWidget(cancel);

	sFLayout->addLayout(buttons);

	setLayout(sFLayout);
}

SelectFile::~SelectFile(void) {}

void SelectFile::rightSaveAs() {
	QString file = QFileDialog::getSaveFileName(this,
		tr("Save as..."),
		QDir::homePath(),
		"Video AVI (*.avi);;Video YUV (*.yuv);;All files (*.*)"
	);

	rightLineEdit->setText(file);
}

void SelectFile::leftSaveAs() {
	QString file = QFileDialog::getSaveFileName(this,
		tr("Save as..."),
		QDir::homePath(),
		"Video AVI (*.avi);;Video YUV (*.yuv);;All files (*.*)"
	);

	leftLineEdit->setText(file);
}

void SelectFile::validate() {
	QString rightFile = rightLineEdit->text();
	if(nbCam == 2) {
		QString leftFile = leftLineEdit->text();
		emit fileSelected(rightFile, leftFile);
		hide();
		return;
	}
	emit fileSelected(rightFile);
	hide();
}
