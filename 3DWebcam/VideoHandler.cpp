/**
 *  VideoHandler.cpp
 *
 *  This file is part of 3DWebcam
 *	
 *	This class is the video handler.
 *	It can require frames from the video threads,
 *	save the video in a file, and encode a set of frames.
 *	Moreover, it can use several video threads if the user
 *	wants to use multiview.
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "VideoHandler.h"
//-------------------------------------------------------------------


VideoHandler::VideoHandler(vector<VideoThread*> const& cams) :
	cameras(cams),
	recording(false),
	encoding(false),
	framesNb(0)
{
}

VideoHandler::~VideoHandler() {
	// Free the writers
	for(int i(0) ; i < (int)writers.size() ; i++) {
		cvReleaseVideoWriter(&writers[i]);
	}
}

void VideoHandler::startRecording(vector<QString> const& files) {
	// Return if the number of files names given is different from the number of video threads
	if (files.size() != cameras.size()) {
		return;
	}

	int isColor = 1;
	int fps     = 25;
	int fourcc;

	// Create a writer to the given file for each video thread
	for(int i(0) ; i < (int)cameras.size() ; i++) {
		fourcc = cameras[i]->getRecordingFormat();
		writers.push_back(cvCreateVideoWriter(files[i].toStdString().c_str(), fourcc, fps, cvSize(WIDTH,HEIGHT), isColor));
	}

	framesNb = 0;
	recording = true;
}

int VideoHandler::stopRecording() {
	// Free the writers
	for(int i(0) ; i < (int)writers.size() ; i++) {
		cvReleaseVideoWriter(&writers[i]);
	}

	writers.clear();

	recording = false;

	return framesNb;
}

void VideoHandler::startEncoding() {
	//Check if the directory exists
	if(!QDir("video_qp32").exists()) {
		QDir().mkdir("video_qp32");
	}

	int isColor = 1;
	int fps     = 25;
	
	// Create a writer for each video thread
	for(int i(0) ; i < (int)cameras.size() ; i++) {
		char str[23];
		sprintf(str, "video_qp32/video_%d.yuv", i);
		writers.push_back(cvCreateVideoWriter(str, CV_FOURCC('I','Y','U','V'), fps, cvSize(WIDTH,HEIGHT), isColor));
	}
	
	framesNb = 0;
	encoding = true;
}

void VideoHandler::stopEncoding() {
	// Launch the encoding thread
	Thread* t = new Thread((Thread::FuncType)encode);
	t->Launch();

	// Free the writers
	for(int i(0) ; i < (int)writers.size() ; i++) {
		cvReleaseVideoWriter(&writers[i]);
	}

	writers.clear();

	encoding = false;
}

void VideoHandler::saveFrame() {
	// Write frames in the files with the writers
	for(int i = 0 ; i < (int)cameras.size() ; i++) {
		// getFrame(i) grabs a frame from the VideoThread with the index "i" in the "cameras" vector
		cvWriteFrame(writers[i], getFrame(i));
	}

	framesNb++;
}

void* encode(void* data) {
	int argc = 4;
	char** argv = (char**)malloc(sizeof(char*)*argc);
	argv[0] = "H264AVCEncoderLibTestStatic.exe";
	argv[1] = "-vf";
	argv[2] = "config.cfg";
	argv[3] = "0";

	H264AVCEncoderTest*         pcH264AVCEncoderTest = NULL;
	H264AVCEncoderTest::create( pcH264AVCEncoderTest );

	pcH264AVCEncoderTest->init(argc, argv);
	pcH264AVCEncoderTest->go();
	pcH264AVCEncoderTest->destroy();

	free(argv);

	return NULL;
}

void VideoHandler::useCali(bool b) {
	cout << "useCali from video handler" << endl;
	for(int i(0) ; i < (int)cameras.size() ; i++) {
		cameras[i]->useCali(b);
	}
}

void VideoHandler::convertYUV(bool b) {
	for(int i(0) ; i < (int)cameras.size() ; i++) {
		cameras[i]->convertYUV(b);
	}
}

void VideoHandler::setMode(int m) {
	for(int i(0) ; i < (int)cameras.size() ; i++) {
		cameras[i]->setMode(m);
	}
}

int VideoHandler::getNbCam() const {
	return (int)cameras.size();
}

int VideoHandler::getFrameNb() const {
	return framesNb;
}

const blImage< blColor3<unsigned char> >& VideoHandler::getFrame(int pos) const {
	for(int i(0) ; i < (int)cameras.size() ; i++) {
		if(cameras[i]->getPosition() == pos) {
			return cameras[i]->GetFrame();
		}
	}

	return NULL;
}

bool VideoHandler::isRecording() const {
	return recording;
}

bool VideoHandler::isEncoding() const {
	return encoding;
}

vector<VideoThread*> VideoHandler::getCameras() const {
	return cameras;
}

