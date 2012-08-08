/**
 * Modified by Nicolas Kniebihler.
 * Adding of some includes.
 */

#ifndef BL_IMAGEAPI_HPP
#define BL_IMAGEAPI_HPP


//-------------------------------------------------------------------
// FILE:            blImageAPI.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of data structures and image
//                  algorithms to use for Machine Vision in
//                  robotics experiments
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:
//
// NOTES:
//
// DATE CREATED:    May/30/2010
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file and sub-files
//-------------------------------------------------------------------
#include "../SFML/Thread.hpp"
#include <boost/shared_ptr.hpp>
#include <cv.h>
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums used for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
namespace blImageAPI
{
    // Special functions needed to handle the
    // release of various resources such as
    // OpenCV's IplImage
    #include "blCleanResources.hpp"

    // A simple and efficient color structure of three
    // components saved in a Blue,Green,Red sequence
    #include "blColor3.hpp"

    // Based on blColor3, adds an alpha transparency value
    #include "blColor4.hpp"

    // A base class used to wrap OpenCV's image class IplImage
    #include "blImg.hpp"

    // A base class used to wrap OpenCV's image
    // class IplImage with a boost::shared_ptr
    #include "blImage.hpp"

    // A collection of overloaded operators and functions
    // I developed to handle images just like matrices, so
    // as to make code very readable
    #include "blImageOperators.hpp"

    // Very simple functions based on cvEncodeImage and
    // cvDecodeImage used to encode and decode
    // an image to/from a compressed format (specified
    // at run time by user) staying with the blImage
    // format
    //#include "blEncodeAndDecode.hpp"

    // A collection of functions used to augment
    // images in various ways to make one "big"
    // image.
    //#include "blImageTiling.hpp"

    // A collection of simple functions used to
    // serialize/unserialize images to/from a
    // file storage
    //#include "blImageSerialization.hpp"

    // A base class used to wrap OpenCV's CvCapture
    // class with a smart shared_ptr pointer
    #include "blCaptureDevice.hpp"

    // A collection of templated function that
    // take the negative of an image, no matter the
    // depth of the image. (OpenCV does not takes the
    // negative of floating point images)
    //#include "blImageNegative.hpp"

    // A collection of simple functions I created
    // to play with image channels
    #include "blImageChannels.hpp"

    // A collection of simple functions I created to
    // convert images from and to the HSV color space
    //#include "blHSV.hpp"

    // A base class used to wrap OpenCV's CvMemStorage
    // with boost::shared_ptr
    #include "blMemStorage.hpp"

    // A collection of algorithms I wrote to blend
    // two images that emulate adobe photoshop's
    // blending modes
    //#include "blImageBlending.hpp"

    // To use the blTexture class you
    // have to define the following macro
    #ifdef USE_BL_TEXTURE

        // A wrapper for an opengl texture I wrote that
        // generate textures from opencv IplImage or
        // from image files
        #include "blTexture.hpp"

    #endif

    // A simple function used to take an image and
    // make it look like it was sketched using a pencil
    // which can be in B/W or in Color
    //#include "blImagePencilSketch.hpp"

    // A collection of simple functions I wrote
    // to shift functions up, down, left, right.
    // I will be adding more functions as I build them
    //#include "blImageShifting.hpp"

    // A collection of algorithms I wrote to use
    // the blImageAPI for signal processing
    //#include "blSignalProcessing.hpp"

    // A collection of simple functions I created
    // to plot data on an image
    //#include "blImagePlot.hpp"

    // To use the blVideoThread class you
    // have to de fine the following macro
    #ifdef USE_BL_VIDEOTHREAD

        // Based on sf::Thread and blCaptureDevice, it
        // allows us to retrieve frames from a video source,
        // which is usually limited to 30 frames per second,
        // in a parallel thread, allowing the main application
        // to run at much faster rates
        #include "blVideoThread.hpp"

        // Based on blVideoThread, this class has a buffer
        // that is always available and the same size,
        // and that new pixel data is fed serially to the
        // frame buffer
        #include "blVideoThread2.hpp"

    #endif
}
//-------------------------------------------------------------------


#endif // BL_IMAGEAPI_HPP
