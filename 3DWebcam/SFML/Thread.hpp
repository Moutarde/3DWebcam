/**
 *  Thread.hpp
 *
 *  This file is part of 3DWebcam
 *
 *	This is a mixed source of the original Thread.hpp from the SFML Library.
 *	This can be used for Win32 and Unix environments.
 *	All the required files are in the same folder.
 *
 *  Author: Nicolas Kniebihler
 *
 *  Copyright � 2012. All rights reserved.
 *
 */

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_THREADMULTI_HPP
#define SFML_THREADMULTI_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "NonCopyable.hpp"
#if defined(SFML_SYSTEM_WINDOWS)
	#include <windows.h>
#else
	#include <pthread.h>
#endif


namespace sf
{
////////////////////////////////////////////////////////////
/// Thread defines an easy way to manipulate a thread.
/// There are two ways to use Thread :
/// - Inherit from it and override the Run() virtual function
/// - Construct a Thread instance and pass it a function
/// pointer to call
////////////////////////////////////////////////////////////
class SFML_API Thread : NonCopyable
{
public :

    typedef void (*FuncType)(void*);

    ////////////////////////////////////////////////////////////
    /// Construct the thread from a function pointer
    ///
    /// \param Function : Entry point of the thread
    /// \param UserData : Data to pass to the thread function (NULL by default)
    ///
    ////////////////////////////////////////////////////////////
    Thread(FuncType Function, void* UserData = NULL);

    ////////////////////////////////////////////////////////////
    /// Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Thread();

    ////////////////////////////////////////////////////////////
    /// Create and run the thread
    ///
    ////////////////////////////////////////////////////////////
    void Launch();

    ////////////////////////////////////////////////////////////
    /// Wait until the thread finishes
    ///
    ////////////////////////////////////////////////////////////
    void Wait();

    ////////////////////////////////////////////////////////////
    /// Terminate the thread
    /// Terminating a thread with this function is not safe,
    /// you should rather try to make the thread function
    /// terminate by itself
    ///
    ////////////////////////////////////////////////////////////
    void Terminate();

protected :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Thread();

private :

    ////////////////////////////////////////////////////////////
    /// Function called as the thread entry point
    ///
    ////////////////////////////////////////////////////////////
    virtual void Run();

    ////////////////////////////////////////////////////////////
    /// Actual thread entry point, dispatches to instances
    ///
    /// \param UserData : Data to pass to the thread function
    ///
    /// \return Error code
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int __stdcall ThreadFunc(void* UserData);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
	#if defined(SFML_SYSTEM_WINDOWS)
		HANDLE		myHandle;   ///< Win32 thread handle
	#else
		pthread_t	myThread;   ///< Unix thread instance
		bool		myIsActive; ///< Thread state (active or inactive)
	#endif
    FuncType	myFunction; ///< Function to call as the thread entry point
    void*		myUserData; ///< Data to pass to the thread function
};

} // namespace sf


#endif // SFML_THREADMULTI_HPP
