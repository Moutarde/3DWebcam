#ifndef BL_CAPTUREDEVICE_HPP
#define BL_CAPTUREDEVICE_HPP


//-------------------------------------------------------------------
// FILE:            blCaptureDevice.hpp
// CLASS:           blCaptureDevice
// BASE CLASS:      None
//
// PURPOSE:         A base class used to wrap OpenCV's CvCapture
//                  class with a smart shared_ptr pointer
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImage
//                  boost::shared_ptr -- From boost c++ libraries
//
// NOTES:
//
// DATE CREATED:    Sep/22/2009
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums used for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
class blCaptureDevice
{
public: // Constructors and destructors

    // Default constructor
    blCaptureDevice();

    // Copy constructor
    blCaptureDevice(const blCaptureDevice& CaptureDevice);

    // Virtual destructor
    virtual ~blCaptureDevice()
    {
    }

public: // Public functions

    // Function used to connect to a webcam
    bool            ConnectToWebcam(const int& WhichWebcam,
                                    const int& Width = 320,
                                    const int& Height = 240);

    // Function used to connect to an AVI file
    bool            ConnectToAVIFile(const string& AVIFile);

    // Function used to know if capture device is connected
    const bool      IsConnected()const;

    // Funtions used to query images from a video
    // source directly into a blImage structure
    template<typename blType>
    const void      QueryFrame(blImage<blType>& Frame)const;

    // Function used to get the capture device
    const boost::shared_ptr<CvCapture>&     GetCaptureDevice()const;

    // Function used to convert this object to a pure OpenCV CvCapture device
    operator CvCapture* const()
    {
        if(m_CaptureDevice.use_count() > 0)
            return m_CaptureDevice.get();
        else
            return NULL;
    }

    // Function used to convert this const object to a pure OpenCV CvCapture device
    operator const CvCapture* const()const
    {
        if(m_CaptureDevice.use_count() > 0)
            return m_CaptureDevice.get();
        else
            return NULL;
    }

protected: // Protected variables

    // Image variable
    boost::shared_ptr<CvCapture>        m_CaptureDevice;

private: // Private variables
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blCaptureDevice::blCaptureDevice()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blCaptureDevice::blCaptureDevice(const blCaptureDevice& CaptureDevice)
{
    // Grab the capture device from the passed object
    m_CaptureDevice = CaptureDevice.GetCaptureDevice();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const boost::shared_ptr<CvCapture>& blCaptureDevice::GetCaptureDevice()const
{
    return m_CaptureDevice;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline bool blCaptureDevice::ConnectToWebcam(const int& WhichWebcam,
                                             const int& Width,
                                             const int& Height)
{
    // Create a capture device and connect it to the webcam
    CvCapture* TempCaptureDevice = NULL;
    TempCaptureDevice = cvCreateCameraCapture(WhichWebcam);

    // Check if the capture device was connected successfully
    if(TempCaptureDevice)
    {
        // The capture device was successfully connected to the AVI file
        m_CaptureDevice = boost::shared_ptr<CvCapture>(TempCaptureDevice,ReleaseCaptureDevice());

        // Set the desired resolution of the webcam capture
        cvSetCaptureProperty(m_CaptureDevice.get(),CV_CAP_PROP_FRAME_WIDTH,Width);
        cvSetCaptureProperty(m_CaptureDevice.get(),CV_CAP_PROP_FRAME_HEIGHT,Height);

        return true;
    }
    else
    {
        // Error
        return false;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline bool blCaptureDevice::ConnectToAVIFile(const string& AVIFile)
{
    // Create a capture device and connect it to the AVI file
    CvCapture* TempCaptureDevice = NULL;
    TempCaptureDevice = cvCaptureFromAVI(AVIFile.c_str());

    // Check if the capture device was connected successfully
    if(TempCaptureDevice)
    {
        // The capture device was successfully connected to the AVI file
        m_CaptureDevice = boost::shared_ptr<CvCapture>(TempCaptureDevice,ReleaseCaptureDevice());

        return true;
    }
    else
    {
        // Error
        return false;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const bool blCaptureDevice::IsConnected()const
{
    if(m_CaptureDevice.use_count() <= 0)
        return false;
    else
        return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const void blCaptureDevice::QueryFrame(blImage<blType> &Frame)const
{
    // First check if the capture device has
    // been connected
    if(this->IsConnected())
    {
        Frame.LoadImage(cvQueryFrame(m_CaptureDevice.get()));
    }
    else
    {
        // Error
        return;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
#endif // BL_CAPTUREDEVICE_HPP
