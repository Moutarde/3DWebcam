/**
 *  VideoThread.h
 *
 *  This file is part of 3DWebcam
 *
 *	This class extends blVideoThread2.
 *	It does exactly the same, but it also modifies the frames grabbed
 *	according to the options given :
 *		- resize
 *		- calibration
 *		- format conversion
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#define USE_BL_VIDEOTHREAD


//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <highgui.h>
#include <iostream>

#include "blImageAPI/blImageAPI.hpp"
#include "ImageUtil.h"
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Global constants
//-------------------------------------------------------------------
// Size of the frames
#define WIDTH	160
#define HEIGHT	120

// Position of the camera
#define ALONE	0
#define LEFT	1
#define RIGHT	2
//-------------------------------------------------------------------


using namespace blImageAPI;
using namespace cv;
using namespace sf;

class VideoThread : public blVideoThread2
{
	// Public functions
	public:
		// Constructor
		VideoThread(int pos);
		// Destructor
		~VideoThread()
		{
			// Free the matrices
			cvReleaseMat(&Q);
			cvReleaseMat(&mx1);
			cvReleaseMat(&my1);
			cvReleaseMat(&mx2);
			cvReleaseMat(&my2);
		}

		// Function that gets called
		// when thread is running
		inline virtual void    Run();

		// Changing the mode
		inline void useCali(bool b);
		inline void convertYUV(bool b);
		inline void setMode(int m);

		// Getters
		inline int getRecordingFormat();
		inline int getPosition();

	// Private variables
	private:
		// Calibration matrices
		CvMat *Q, *mx1, *my1, *mx2, *my2;

		// Options variables
		bool useCalibration, convertYCbCr;
		int mode;

		// Relative position of the camera
		int position;
};

inline VideoThread::VideoThread(int pos) : blVideoThread2(), position(pos) {
	// Options init
	useCalibration = false;
	convertYCbCr = false;
	mode = YUV_MODE;
}

/**
 *  Inspired by the Run function of the class blVideoThread2
 *	from the blImageAPI by Vincenzo Barbato :
 *	http://www.barbatolabs.com
 */
inline void VideoThread::Run()
{
	while(!m_IsCapturingThreadToBeTerminated)
	{
		if(!m_IsCapturingThreadPaused &&
			!m_IsCapturingThreadToBeTerminated)
		{
			// If we cannot successfully query
			// a frame, then we just stop the
			// capturing thread
			if(this->IsConnected())
			{
				// Query a new frame and
				// signal that a new frame
				// is available
				blImage< blColor3<unsigned char> > tmp;
				this->QueryFrame(tmp);

				// Now we want to modify the frame
				// according to the mode

				if (position != ALONE && useCalibration) {
					// Remove distorsions
					if(position == LEFT) {
						cout << "removeDist from left video thread" << endl;
						removeDist(tmp, mx1, my1);
					}
					else if(position == RIGHT) {
						cout << "removeDist from right video thread" << endl;
						removeDist(tmp, mx2, my2);
					}
				}

				// Resize the frame according to
				// the WIDTH and HEIGHT constants
				// set in ImageUtil.h
				m_Frame.CreateImage(HEIGHT, WIDTH);
				cvResize(tmp, m_Frame);

				if (convertYCbCr) {
					// Convert frame into YCbCr format
					bgr2ycrcb(m_Frame);
				}

				if (mode != YUV_MODE && mode != RGB_MODE) {
					// Show the selected layers
					extractLayer(m_Frame, mode);
				}

				// Now that we have a frame
				// we simply copy it to the
				// buffer
				// Let's make sure that the
				// frame buffer is the same
				// size as the frame
				if(m_FrameBuffer.size1() != this->m_Frame.size1() ||
					m_FrameBuffer.size2() != this->m_Frame.size2())
				{
					ResizeFrameBuffer();
				}

				// Copy the frame into
				// the frame buffer
				CopyFrameIntoFrameBuffer();
			}
			else
			{
				// The capturing device got
				// disconnected, so we stop
				// this capturing thread
				m_IsCapturingThreadToBeTerminated = true;
			}
		}
	}
}

inline void VideoThread::useCali(bool b) {
	if(b) {
		// Set the calibration matrices
		if(position == LEFT) {
			cout << "useCali from left video thread" << endl;
			mx1 = (CvMat *)cvLoad("matrices/mx1.xml",NULL,NULL,NULL);
			my1 = (CvMat *)cvLoad("matrices/my1.xml",NULL,NULL,NULL);
		}
		else if(position == RIGHT) {
			cout << "useCali from right video thread" << endl;
			mx2 = (CvMat *)cvLoad("matrices/mx2.xml",NULL,NULL,NULL);
			my2 = (CvMat *)cvLoad("matrices/my2.xml",NULL,NULL,NULL);
		}
	}
	useCalibration = b;
}

inline void VideoThread::convertYUV(bool b) {
	convertYCbCr = b;
}

inline void VideoThread::setMode(int m) {
	mode = m;
}

inline int VideoThread::getRecordingFormat() {
	if(convertYCbCr) {
		return CV_FOURCC('I','Y','U','V');
	}
	else {
		return CV_FOURCC('P','I','M','1');
	}
}

inline int VideoThread::getPosition() {
	return position;
}

#endif // VIDEOTHREAD_H