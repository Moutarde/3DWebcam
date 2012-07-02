#ifndef BL_VIDEOTHREAD2_HPP
#define BL_VIDEOTHREAD2_HPP


//-------------------------------------------------------------------
// FILE:            blVideoThread2.hpp
// CLASS:           blVideoThread2
// BASE CLASS:      blVideoThread
//
// PURPOSE:         Based on blVideoThread, this class has a buffer
//                  that is always available and the same size,
//                  and that new pixel data is fed serially to the
//                  frame buffer
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
// NOTES:           - The difference in this class is that the
//                    frame buffer is always available and that
//                    new data is continuously fed to the image
//                    one pixel at a time
//
// DATE CREATED:    May/10/2011
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
class blVideoThread2 : public blVideoThread
{
public: // Constructors and destructors

    // Default constructor
    blVideoThread2();

    // Destructor
    ~blVideoThread2()
    {
    }

public: // Public functions

    // Function that gets called
    // when thread is running
    virtual void    Run();

    // Function used to get the captured frame
    const blImage< blColor3<unsigned char> >&   GetFrame()const;

protected: // Protected variables

    // Frame image buffer which is
    // used to always have a buffer
    // available to read continuously
    blImage< blColor3<unsigned char> >          m_FrameBuffer;

private: // Private functions

    // Function used to resize the
    // frame buffer to match the
    // size of the main capturing
    // frame
    void                                        ResizeFrameBuffer();

    // Function used to copy the
    // frame into the frame buffer
    void                                        CopyFrameIntoFrameBuffer();

private: // Private variables

    // Variable used to check whether
    // the frame buffer is being resized.
    // This prevents the main program thread
    // to try to read the buffer while we're
    // resizing it
    bool                                        m_IsFrameBufferBeingResized;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blVideoThread2::blVideoThread2() : blVideoThread()
{
    // Default the booleans
    m_IsFrameBufferBeingResized = false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread2::Run()
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
                this->QueryFrame(m_Frame);

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
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread2::ResizeFrameBuffer()
{
    m_IsFrameBufferBeingResized = true;

    // Resize the frame buffer
    m_FrameBuffer.CreateImage(this->m_Frame.size1(),
                              this->m_Frame.size2());

    // Reset the resizing flag
    m_IsFrameBufferBeingResized = false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread2::CopyFrameIntoFrameBuffer()
{
    for(int i = 0; i < m_FrameBuffer.size1(); ++i)
    {
        for(int j = 0; j < m_FrameBuffer.size2(); ++j)
        {
            m_FrameBuffer[i][j] = this->m_Frame[i][j];
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const blImage< blColor3<unsigned char> >& blVideoThread2::GetFrame()const
{
    // Since this function is being called
    // from the main program thread or another
    // thread, we could be currently resizing
    // the frame buffer, and thus we could
    // have a problem
    // So if we're currently resizing the
    // frame buffer, we wait till the
    // buffer is resized
    while(m_IsFrameBufferBeingResized)
    {
    }

    // By now we know that the frame
    // buffer has been resized, so we
    // simply return it
    return m_FrameBuffer;
}
//-------------------------------------------------------------------


#endif // BL_VIDEOTHREAD2_HPP
