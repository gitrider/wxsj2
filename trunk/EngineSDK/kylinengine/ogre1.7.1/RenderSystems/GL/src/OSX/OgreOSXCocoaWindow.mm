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

#include "OgreOSXCocoaWindow.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreImageCodec.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"

#include "OgreGLRenderSystem.h"
#include "OgreOSXCocoaContext.h"
#include "OgreOSXCGLContext.h"

#include <OpenGL/gl.h>
#define GL_EXT_texture_env_combine 1
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>

namespace Ogre {

    OSXCocoaWindow::OSXCocoaWindow()
    {
		mActive = false;
		mContext = 0;
		mWindow = 0;
    }

    OSXCocoaWindow::~OSXCocoaWindow()
    {
		[glContext clearDrawable];
		CGReleaseAllDisplays();
        //[mWindow close];
		//[mWindow release];
    }
	
	void OSXCocoaWindow::create(const String& name, unsigned int width, unsigned int height,
	            bool fullScreen, const NameValuePairList *miscParams)
    {
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSApplicationLoad();
		//OgreWindowDelegate *delegate = [[OgreWindowDelegate alloc] initWithGLOSXCocoaWindow:this];
		//[window setDelegate:delegate];
		/*
***Key: "title" Description: The title of the window that will appear in the title bar Values: string Default: RenderTarget name

***Key: "colourDepth" Description: Colour depth of the resulting rendering window; only applies if fullScreen is set. Values: 16 or 32 Default: desktop depth Notes: [W32 specific]

***Key: "left" Description: screen x coordinate from left Values: positive integers Default: 'center window on screen' Notes: Ignored in case of full screen

***Key: "top" Description: screen y coordinate from top Values: positive integers Default: 'center window on screen' Notes: Ignored in case of full screen

***Key: "depthBuffer" [DX9 specific] Description: Use depth buffer Values: false or true Default: true

***Key: "externalWindowHandle" [API specific] Description: External window handle, for embedding the OGRE context Values: positive integer for W32 (HWND handle) poslong:posint:poslong (display*:screen:windowHandle) or poslong:posint:poslong:poslong (display*:screen:windowHandle:XVisualInfo*) for GLX Default: 0 (None)

***Key: "FSAA" Description: Full screen antialiasing factor Values: 0,2,4,6,... Default: 0

***Key: "displayFrequency" Description: Display frequency rate, for fullscreen mode Values: 60...? Default: Desktop vsync rate

***Key: "vsync" Description: Synchronize buffer swaps to vsync Values: true, false Default: 0
*/

		BOOL hasDepthBuffer = YES;
		int fsaa_samples = 0;
		NSString *windowTitle = [NSString stringWithCString:name.c_str() encoding:NSASCIIStringEncoding];
		int winx = 0, winy = 0;
		int depth = 32;
		
		if(miscParams)
		{
			NameValuePairList::const_iterator opt(NULL);
			
			opt = miscParams->find("title");
			if(opt != miscParams->end())
				windowTitle = [NSString stringWithCString:opt->second.c_str() encoding:NSASCIIStringEncoding];
				
			opt = miscParams->find("left");
			if(opt != miscParams->end())
				winx = StringConverter::parseUnsignedInt(opt->second);
				
			opt = miscParams->find("top");
			if(opt != miscParams->end())
				winy = NSHeight([[NSScreen mainScreen] frame]) - StringConverter::parseUnsignedInt(opt->second) - height;
				
				
			opt = miscParams->find("depthBuffer");
			if(opt != miscParams->end())
				hasDepthBuffer = StringConverter::parseBool(opt->second);
				
			opt = miscParams->find("FSAA");
			if(opt != miscParams->end())
				fsaa_samples = StringConverter::parseUnsignedInt(opt->second);
			
			opt = miscParams->find( "colourDepth" );
			if( opt != miscParams->end() )
				depth = StringConverter::parseUnsignedInt( opt->second );
		}		
		

		NSOpenGLPixelFormat* openglFormat = nil;
		{
			NSOpenGLPixelFormatAttribute attribs[30];
			int i=0;
			
			if(fullScreen)
			{
				GLRenderSystem *rs = static_cast<GLRenderSystem*>(Root::getSingleton().getRenderSystem());
				OSXContext *mainContext = (OSXContext*)rs->_getMainContext();
		
				CGLContextObj share = NULL;
				if(mainContext == 0)
				{
					share = NULL;
				}
				else if(mainContext->getContextType() == "NSOpenGL")
				{
					OSXCocoaContext* cocoaContext = static_cast<OSXCocoaContext*>(mainContext);
					NSOpenGLContext* nsShare = cocoaContext->getContext();
					share = (CGLContextObj)[nsShare CGLContextObj];
				}
				else if(mainContext->getContextType() == "CGL")
				{
					OSXCGLContext* cglShare = static_cast<OSXCGLContext*>(mainContext);
					share = cglShare->getContext();
				}
		
				// create the context
				createCGLFullscreen(width, height, depth, fsaa_samples, share);	
			}
			else
			{
				// Specifying "NoRecovery" gives us a context that cannot fall back to the software renderer.  This makes the View-based context a compatible with the fullscreen context, enabling us to use the "shareContext" feature to share textures, display lists, and other OpenGL objects between the two.
				attribs[i++] = NSOpenGLPFANoRecovery;
			
				attribs[i++] = NSOpenGLPFAAccelerated;
				attribs[i++] = NSOpenGLPFADoubleBuffer;
				
				attribs[i++] = NSOpenGLPFAAlphaSize;
				attribs[i++] = (NSOpenGLPixelFormatAttribute) 0;
				
				attribs[i++] = NSOpenGLPFAStencilSize;
				attribs[i++] = (NSOpenGLPixelFormatAttribute) 1;
				
				attribs[i++] = NSOpenGLPFAAccumSize;
				attribs[i++] = (NSOpenGLPixelFormatAttribute) 0;

				attribs[i++] = NSOpenGLPFADepthSize;
				attribs[i++] = (NSOpenGLPixelFormatAttribute) (hasDepthBuffer? 16 : 0);
				
				if(fsaa_samples > 0)
				{
					attribs[i++] = NSOpenGLPFASampleBuffers;
					attribs[i++] = (NSOpenGLPixelFormatAttribute) 1;
					
					attribs[i++] = NSOpenGLPFASamples;
					attribs[i++] = (NSOpenGLPixelFormatAttribute) fsaa_samples;
				}
				
				attribs[i++] = (NSOpenGLPixelFormatAttribute) 0;

				openglFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes: attribs] autorelease];
			}
			
			NameValuePairList::const_iterator opt2(NULL);
			if(miscParams)
			{
				opt2 = miscParams->find("pixelFormat");
				if(opt2 != miscParams->end())
					openglFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes: (NSOpenGLPixelFormatAttribute*)StringConverter::parseUnsignedLong(opt2->second)] autorelease];
			}
			GLRenderSystem *rs = static_cast<GLRenderSystem*>(Root::getSingleton().getRenderSystem());
			OSXCocoaContext *mainContext = (OSXCocoaContext*)rs->_getMainContext();
			NSOpenGLContext *shareContext = mainContext == 0? nil : mainContext->getContext();

			glContext = [[NSOpenGLContext alloc] initWithFormat: openglFormat shareContext:shareContext];
			
			NameValuePairList::const_iterator opt(NULL);
			NameValuePairList::const_iterator param_useNSView_pair(NULL);
			if(miscParams) {
				opt = miscParams->find("externalWindowHandle");
				param_useNSView_pair = miscParams->find("macAPICocoaUseNSView") ;

				
			}
			
			if(!miscParams || opt == miscParams->end())
			{
				//Not sure why this should be but it is required for the window to work at fullscreen.
				int styleMask = fullScreen? NSBorderlessWindowMask : NSResizableWindowMask;
			
				mWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(winx, winy, width, height) styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
				[mWindow setTitle:windowTitle];
				
				if(winx == 0 && winy == 0) [mWindow center];
				
				mView = [[OgreView alloc] initWithGLOSXWindow:this];
				[mWindow setContentView:mView];
			}
			else
			{
				bool useNSView = false ;
				if(param_useNSView_pair != miscParams->end())
					if(param_useNSView_pair->second == "true")
						useNSView = true ;

				// If the macAPICocoaUseNSView parmeter was set, use the winhandler as pointer to an NSView
				// Otherwise we assume the user created the inerface with Interface Builder and instantiated an OgreView.
				
				if(useNSView) {
					LogManager::getSingleton().logMessage("Mac Cocoa Window: Rendering on an external plain NSView*") ;
					NSView * nsview = (NSView*)StringConverter::parseUnsignedLong(opt->second);
					mView = nsview ;					
				} else {
					OgreView * view = (OgreView*)StringConverter::parseUnsignedLong(opt->second);
					[view setOgreWindow:this];
					mView = view ;
				
					NSRect b = [mView bounds];
					width = b.size.width;
					height = b.size.height;
				}
			}

				
			[glContext setView:mView];

			mName = name;
			mWidth = width;
			mHeight = height;
            mColourDepth = depth;
            mFSAA = fsaa_samples;
            mIsFullScreen = fullScreen;

			// Create register the context with the rendersystem and associate it with this window
			mContext = new OSXCocoaContext(glContext, openglFormat);
			/*rs->_registerContext(this, newContext);
			
			if (rs->_getMainContext() == 0)
				[glContext makeCurrentContext];
			*/
			//show window
			if(mWindow)
				[mWindow performSelectorOnMainThread:@selector(makeKeyAndOrderFront:) withObject:NULL waitUntilDone:NO];
				
		}
		
		// make active
		mActive = true;
        mClosed = false;

        [pool drain];
    }

    void OSXCocoaWindow::destroy(void)
    {
		if(mIsFullScreen)
			destroyCGLFullscreen();
		
		// Unregister and destroy OGRE GLContext
		delete mContext;
		
		if(mWindow) [mWindow close];
		
        mActive = false;

        Root::getSingleton().getRenderSystem()->detachRenderTarget( this->getName() );
    }

	/*OgreView* OSXCocoaWindow::ogreView() const
	{
		return [window contentView];
	}*/

    bool OSXCocoaWindow::isActive() const
    {
        return mActive;
    }

    bool OSXCocoaWindow::isClosed() const
    {
        return false;
    }

    void OSXCocoaWindow::reposition(int left, int top)
    {
		if(!mWindow) return;
		
		NSRect frame = [mWindow frame];
		frame.origin.x = left;
		frame.origin.y = top-frame.size.height;
		[mWindow setFrame:frame display:YES];
    }

    void OSXCocoaWindow::resize(unsigned int width, unsigned int height)
    {
		if(!mWindow) return;
		
		NSRect frame = [mWindow frame];
		frame.size.width = width;
		frame.size.height = height;
		[mWindow setFrame:frame display:YES];
	/*SHOULD be called from delegate
        for (ViewportList::iterator it = mViewportList.begin(); it != mViewportList.end(); ++it)
        {
            (*it).second->_updateDimensions();
        }*/
    }

    void OSXCocoaWindow::windowMovedOrResized()
    {
		[glContext update];
		NSRect frame = [mView frame];
		mWidth = (unsigned int)frame.size.width;
		mHeight = (unsigned int)frame.size.height;
        mLeft = (int)frame.origin.x;
        mTop = (int)frame.origin.y+(unsigned int)frame.size.height;
		
        for (ViewportList::iterator it = mViewportList.begin(); it != mViewportList.end(); ++it)
        {
            (*it).second->_updateDimensions();
        }
    }

    void OSXCocoaWindow::swapBuffers(bool waitForVSync)
    {
		if(!mIsFullScreen)
			[glContext flushBuffer];
		else
			swapCGLBuffers();
			
		/*if(![[[NSRunLoop currentRunLoop] currentMode] isEqualTo:NSEventTrackingRunLoopMode])
		{
			NSEvent *e = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
			if(e)
				[NSApp sendEvent:e];
		}*/
    }
	
	//-------------------------------------------------------------------------------------------------//
	void OSXCocoaWindow::getCustomAttribute( const String& name, void* pData )
	{
		if( name == "GLCONTEXT" ) 
		{
			*static_cast<OSXContext**>(pData) = mContext;
			return;
		} 
		if( name == "WINDOW" ) 
		{
			*(void**)pData = mWindow;
			return;
		} 
		if( name == "VIEW" ) 
		{
			*(void**)(pData) = mView;
			return;
		} 
	}

    void OSXCocoaWindow::setFullscreen(bool fullScreen, unsigned int width, unsigned int height)
    {
        GLRenderSystem *rs = static_cast<GLRenderSystem*>(Root::getSingleton().getRenderSystem());
        OSXContext *mainContext = (OSXContext*)rs->_getMainContext();
		
        CGLContextObj share = NULL;
        if(mainContext == 0)
        {
            share = NULL;
        }
        else if(mainContext->getContextType() == "NSOpenGL")
        {
            OSXCocoaContext* cocoaContext = static_cast<OSXCocoaContext*>(mainContext);
            NSOpenGLContext* nsShare = cocoaContext->getContext();
            share = (CGLContextObj)[nsShare CGLContextObj];
        }
        else if(mainContext->getContextType() == "CGL")
        {
            OSXCGLContext* cglShare = static_cast<OSXCGLContext*>(mainContext);
            share = cglShare->getContext();
        }
        
        // create the context
        createCGLFullscreen(width, height, getColourDepth(), getFSAA(), share);
    }
}
