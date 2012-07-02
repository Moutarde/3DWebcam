#ifndef BL_VIDEOTHREAD_HPP
#define BL_VIDEOTHREAD_HPP


//-------------------------------------------------------------------
// FILE:            blVideoThread.hpp
// CLASS:           blVideoThread
// BASE CLASS:      sf::Thread,blCaptureDevice
//
// PURPOSE:         Based on sf::Thread and blCaptureDevice, it
//                  allows us to retrieve frames from a video source,
//                  which is usually limited to 30 frames per second,
//                  in a parallel thread, allowing the main application
//                  to run at much faster rates
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    sf::Thread -- Derived from sf::Thread to handle
//                                threading in a cross-platform way
//
//                  blCaptureDevice -- We use a capture device to
//                                     grab frames from a video source
//                  blImage -- We use a blImage to store the
//                             last captured frame
//
// NOTES:
//
// DATE CREATED:    Feb/03/2011
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
class blVideoThread : public sf::Thread,
                      public blCaptureDevice
{
public: // Constructors and destructors

    // Default constructor
    blVideoThread();

    // Destructor
    ~blVideoThread()
    {
        StopCapturingThread();
    }

public: // Public functions

    // Functions used to start/stop/pause/unpause the thread
    void            StartCapturingThread();
    void            StopCapturingThread();
    void            PauseCapturingThread();

    // Function used to know whether
    // there's a new image available
    const bool&     IsNewFrameAvailable()const;

    // Function used to tell the video
    // thread that we grabbed the frame
    // and the it should go ahead and
    // grab another one
    void            LetThreadQueryAnotherFrame();

    // Function that gets called
    // when thread is running
    virtual void    Run();

    // Function used to get the captured frame
    const blImage< blColor3<unsigned char> >&   GetFrame()const;

protected: // Protected variables

    // Frame image used in this thread
    blImage< blColor3<unsigned char> >          m_Frame;

    // Boolean variable used to
    // continue/terminate the thread
    bool                m_IsCapturingThreadToBeTerminated;

    // Boolean variable used for
    // pausing the video grabbing
    bool                m_IsCapturingThreadPaused;

    // Boolean variable used to check if
    // new image is available from this thread
    bool                m_IsNewFrameAvailable;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blVideoThread::blVideoThread()
{
    // Default all the booleans
    m_IsCapturingThreadToBeTerminated = true;
    m_IsCapturingThreadPaused = false;
    m_IsNewFrameAvailable = false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::StartCapturingThread()
{
    // Let's first make sure that we
    // don't try to re-launch the thread
    // after it's already been started
    if(!m_IsCapturingThreadToBeTerminated)
    {
        // Error -- Tried to restart a thread
        //          that had already been started
        return;
    }

    // Check if the capture device has been set
    if(this->IsConnected())
    {
        // Since we have a capture device, we start the thread
        this->Launch();

        // Set boolean so that thread will run
        m_IsCapturingThreadToBeTerminated = false;
        return;
    }

    // Error -- In this case, we don't yet
    // have a capture device so we just return

    // Set the boolean in order
    // to terminate the thread
    m_IsCapturingThreadToBeTerminated = true;

    return;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::StopCapturingThread()
{
    // Set the boolean in order
    // to terminate the thread
    m_IsCapturingThreadToBeTerminated = true;

    // Here we just terminate
    // the running thread
    this->Terminate();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::Run()
{
    while(!m_IsCapturingThreadToBeTerminated)
    {
        if(!m_IsCapturingThreadPaused &&
           !m_IsNewFrameAvailable &&
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
                this->QueryFrame(m_Frame);
                m_IsNewFrameAvailable = true;
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
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::PauseCapturingThread()
{
    // If the thread is currently paused, then
    // we unpause otherwise we pause it
    if(m_IsCapturingThreadPaused)
        m_IsCapturingThreadPaused = false;
    else
        m_IsCapturingThreadPaused = true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const blImage< blColor3<unsigned char> >& blVideoThread::GetFrame()const
{
    return m_Frame;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const bool& blVideoThread::IsNewFrameAvailable()const
{
    return m_IsNewFrameAvailable;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::LetThreadQueryAnotherFrame()
{
    m_IsNewFrameAvailable = false;
}
//-------------------------------------------------------------------


#endif // BL_VIDEOTHREAD_HPP
