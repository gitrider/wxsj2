/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#include "OgreWin32Context.h"
#include "OgreException.h"
#include "OgreGLRenderSystem.h"
#include "OgreRoot.h"

namespace Ogre {

    Win32Context::Win32Context(HDC     HDC,
                 HGLRC   Glrc):
        mHDC(HDC),
        mGlrc(Glrc)
    {
    }
    
    Win32Context::~Win32Context()
    {
		// NB have to do this is subclass to ensure any methods called back
		// are on this subclass and not half-destructed superclass
		GLRenderSystem *rs = static_cast<GLRenderSystem*>(Root::getSingleton().getRenderSystem());
		rs->_unregisterContext(this);
    }
        
    void Win32Context::setCurrent()
    {
         wglMakeCurrent(mHDC, mGlrc);      
    }
	void Win32Context::endCurrent()
	{
		wglMakeCurrent(NULL, NULL);
	}

	GLContext* Win32Context::clone() const
	{
		// Create new context based on own HDC
		HGLRC newCtx = wglCreateContext(mHDC);
		
		if (!newCtx)
		{
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
				"Error calling wglCreateContext", "Win32Context::clone");
		}

		HGLRC oldrc = wglGetCurrentContext();
		HDC oldhdc = wglGetCurrentDC();
		wglMakeCurrent(NULL, NULL);
		// Share lists with old context
	    if (!wglShareLists(mGlrc, newCtx))
		{
			String errorMsg = translateWGLError();
			wglDeleteContext(newCtx);
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, String("wglShareLists() failed: ") + errorMsg, "Win32Context::clone");
		}
		// restore old context
		wglMakeCurrent(oldhdc, oldrc);
		

		return new Win32Context(mHDC, newCtx);
	}

	void Win32Context::releaseContext()
	{
		if (mGlrc != NULL)
		{
			wglDeleteContext(mGlrc);
			mGlrc = NULL;
			mHDC  = NULL;
		}		
	}
}

#if OGRE_THREAD_SUPPORT == 1

// declared in OgreGLPrerequisites.h 
WGLEWContext * wglewGetContext()
{
	using namespace Ogre;
	static OGRE_THREAD_POINTER_VAR(WGLEWContext, WGLEWContextsPtr);

	WGLEWContext * currentWGLEWContextsPtr = OGRE_THREAD_POINTER_GET(WGLEWContextsPtr);
	if (currentWGLEWContextsPtr == NULL)
	{
		currentWGLEWContextsPtr = new WGLEWContext();
		OGRE_THREAD_POINTER_SET(WGLEWContextsPtr, currentWGLEWContextsPtr);
		ZeroMemory(currentWGLEWContextsPtr, sizeof(WGLEWContext));
		wglewInit();

	}
	return currentWGLEWContextsPtr;
}

#endif