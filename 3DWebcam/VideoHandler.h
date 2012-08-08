/**
 *  VideoHandler.h
 *
 *  This file is part of 3DWebcam
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef VIDEOHANDLER_H
#define VIDEOHANDLER_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <QtGui>
#include <cv.h>
#include <vector>
#include <omp.h>

#include "VideoThread.h"
#include "JMVC/H264Extension/src/test/H264AVCEncoderLibTest/H264AVCEncoderLibTest.h"
#include "JMVC/H264Extension/src/test/H264AVCEncoderLibTest/H264AVCEncoderTest.h"
#include "H264AVCCommonLib/CommonBuffers.h"
//-------------------------------------------------------------------


class VideoHandler
{
	// Public functions
	public:
		// Constructor
		VideoHandler(vector<VideoThread*> const& cams);
		// Destructor
		~VideoHandler();

		// Recording the video
		void	startRecording(vector<QString> const& files);
		int		stopRecording();

		// Encoding the video
		void startEncoding();
		void stopEncoding();

		// Write a frame in a file thanks to a writer
		void saveFrame();

		// Change the VideoThreads' mode
		void useCali(bool b);
		void convertYUV(bool b);
		void setMode(int m);

		// Getters
		int											getNbCam() const;
		int											getFrameNb() const;
		const blImage< blColor3<unsigned char> >&	getFrame(int pos) const;
		bool										isRecording() const;
		bool										isEncoding() const;
		vector<VideoThread*>						getCameras() const;

	// Private variables
	private:
		// The video threads from which we grab the frames
		vector<VideoThread*>	cameras;

		// This allows to record the videos
		vector<CvVideoWriter*>	writers;

		// The number of frames saved
		int		framesNb;

		// True if the handler is recording
		bool	recording;

		// True if the handler is encoding
		bool	encoding;
};

/**
 *	Encoding thread
 *
 *	As this function is used as a thread,
 *	it is not declared as a function of the class VideoHandler.
 *
 *	To use this function, do :
 *
 *	Thread* t = new Thread((Thread::FuncType)encode);
 *	t->Launch();
 */
void* encode(void* data);

#endif // VIDEOHANDLER_H
