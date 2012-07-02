#ifndef BL_CLEANRESOURCES_HPP
#define BL_CLEANRESOURCES_HPP


//-------------------------------------------------------------------
// FILE:            blCleanResources.hpp
// CLASS:           None
// BASE CLASS:      None
// PURPOSE:         Special functions needed to handle the release
//                  of various resources such as OpenCV's IplImage
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    IplImage -- Image structure from opencv
//                  cvReleaseImage -- To release the image
//                  CvCapture -- Capture device structure from opencv
//                  cvReleaseCapture -- To release the capture device
//                  CvMemStorage -- Memory storage structure from opencv
//                  cvReleaseMemStorage -- To realase the memory storage
// NOTES:
// DATE CREATED:    May/30/2010
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums used for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to delete a shared_ptr without
// destroying the objects it's pointing to
//-------------------------------------------------------------------
class null_deleter
{
public:

    // Overloaded operator used to release an IplImage
    void operator()(void const*)const
    {
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to release an IplImage
//-------------------------------------------------------------------
class ReleaseImage
{
public:

    // Overloaded operator used to release an IplImage
    void operator()(IplImage* Image)
    {
        // Check if we have an image
        if(Image == NULL)
            return;

        // Release the image
        cvReleaseImage(&Image);

        // Nullify the pointer
        Image = NULL;
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to release an OpenCV Capture Device
//-------------------------------------------------------------------
class ReleaseCaptureDevice
{
public:

    // Overloaded operator used to
    // release a capture device
    void operator()(CvCapture* CaptureDevice)
    {
        // Check if we have a capture device
        if(CaptureDevice == NULL)
            return;

        // Release the image
        cvReleaseCapture(&CaptureDevice);

        // Nullify the pointer
        CaptureDevice = NULL;
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to release an OpenCV memory storage
//-------------------------------------------------------------------
class ReleaseMemStorage
{
public:

    // Overloaded operator used to
    // release a memory storage
    void operator()(CvMemStorage* MemoryStorage)
    {
        // Check if we have a capture device
        if(MemoryStorage == NULL)
            return;

        // Release the image
        cvReleaseMemStorage(&MemoryStorage);

        // Nullify the pointer
        MemoryStorage = NULL;
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to release an OpenCV file storage
//-------------------------------------------------------------------
class ReleaseFileStorage
{
public:

    // Overloaded operator used to
    // release a memory storage
    void operator()(CvFileStorage* FileStorage)
    {
        // Check if we have a capture device
        if(FileStorage == NULL)
            return;

        // Release the image
        cvReleaseFileStorage(&FileStorage);

        // Nullify the pointer
        FileStorage = NULL;
    }
};
//-------------------------------------------------------------------


#endif // BL_CLEANRESOURCES_HPP
