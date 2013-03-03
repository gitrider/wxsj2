/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2008 Renato Araujo Oliveira Filho <renatox@gmail.com>
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
--------------------------------------------------------------------------*/

#include "OgreRoot.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreWindowEventUtilities.h"

#include "OgreGLESPrerequisites.h"
#include "OgreGLESRenderSystem.h"

#include "OgreSymbianEGLSupport.h"
#include "OgreSymbianEGLWindow.h"
#include "OgreSymbianEGLContext.h"

#include <iostream>
#include <algorithm>
#include <climits>
#include <w32std.h>
#include <coemain.h>
namespace Ogre {
    SymbianEGLWindow::SymbianEGLWindow(EGLSupport *glsupport)
		: EGLWindow(glsupport)
    {
    }

    SymbianEGLWindow::~SymbianEGLWindow()
    {
		// Destroy the RWindow.
		if( mWindow != NULL )
		{
			((RWindow *)mWindow)->SetOrdinalPosition( KOrdinalPositionSwitchToOwningWindow );
			((RWindow *)mWindow)->Close();
			delete mWindow;
			mWindow = NULL;
		}
    }

	EGLContext * SymbianEGLWindow::createEGLContext() const
	{
		return new SymbianEGLContext(mEglDisplay, mGLSupport, mEglConfig, mEglSurface);
	}

	void SymbianEGLWindow::getLeftAndTopFromNativeWindow( int & left, int & top, uint width, uint height )
	{

	}

	void SymbianEGLWindow::initNativeCreatedWindow(const NameValuePairList *miscParams)
	{

	}

	void SymbianEGLWindow::createNativeWindow( int &left, int &top, uint &width, uint &height, String &title )
	{
		// destroy current window, if any
		if (mWindow)
			destroy();

		mWindow = 0;
		mClosed = false;		
		mIsDepthBuffered = true;
		mColourDepth = 32;



		if (!mIsExternal)
		{
			/** Handle to the Windows Server session */
			RWsSession iWsSession;

			/** Handle to the Window group */
			RWindowGroup iWindowGroup;

			/** The Screen device */
			CWsScreenDevice* iWsScreenDevice;

			CCoeEnv* env = CCoeEnv::Static();
			iWsSession = env->WsSession();
			iWsScreenDevice = env->ScreenDevice();
			iWindowGroup = env->RootWin();

			{

				CCoeEnv* env = CCoeEnv::Static();
				/** Handle to the Windows Server session */
				RWsSession iWsSession;

				/** Handle to the Window group */
				RWindowGroup iWindowGroup;

				/** The Screen device */
				CWsScreenDevice* iWsScreenDevice;
				iWsSession = env->WsSession();
				iWsScreenDevice = env->ScreenDevice();
				iWindowGroup = env->RootWin();

				if( !iWsScreenDevice )
				{
					return;
				}

				RWindow*  iWindow;
				iWindow = new (ELeave) RWindow( iWsSession );

				// Construct the window.
				TInt err2 = iWindow->Construct( iWindowGroup,
					435353
					);
				User::LeaveIfError( err2 );

				// Enable the EEventScreenDeviceChanged event.
				iWindowGroup.EnableScreenChangeEvents();                                      

				TPixelsTwipsAndRotation pixnrot; 
				iWsScreenDevice->GetScreenModeSizeAndRotation( 
					iWsScreenDevice->CurrentScreenMode(),
					pixnrot );

				// Set size of the window (cover the entire screen)
				iWindow->SetExtent( TPoint( 0, 0 ),
					pixnrot.iPixelSize
					);

				iWindow->SetRequiredDisplayMode( iWsScreenDevice->DisplayMode() );

				// Activate window and bring it to the foreground
				iWindow->Activate();
				iWindow->SetVisible( ETrue );
				iWindow->SetNonFading( ETrue ); 
				iWindow->SetShadowDisabled( ETrue ); 
				iWindow->EnableRedrawStore( EFalse ); 
				iWindow->EnableVisibilityChangeEvents();
				iWindow->SetNonTransparent(); 
				iWindow->SetBackgroundColor(); 
				iWindow->SetOrdinalPosition( 0 );   
				mWindow = iWindow;




			}
		}

		mNativeDisplay = EGL_DEFAULT_DISPLAY;
		mEglDisplay = eglGetDisplay(mNativeDisplay);
		mGLSupport->setGLDisplay(mEglDisplay);

		// Choose the buffer size based on the Window's display mode.
		TDisplayMode displayMode = ((RWindow *)mWindow)->DisplayMode();
		TInt bufferSize = 0;

		switch( displayMode )
		{
		case(EColor4K):
			bufferSize = 12;
			break;
		case(EColor64K):
			bufferSize = 16;
			break;
		case(EColor16M):
			bufferSize = 24;
			break;
		case(EColor16MU):
		case(EColor16MA):
			bufferSize = 32;
			break;
		default:
			break;
		}

		// Set the desired properties for the EGLSurface
		const EGLint attributeList[] = { 
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT, 
			EGL_BUFFER_SIZE, bufferSize, 
			EGL_NONE 
		};

		EGLint numConfigs = 0;

		// Choose the best EGLConfig that matches the desired properties. 
		if( eglChooseConfig( mEglDisplay, attributeList, &mEglConfig,
			1, &numConfigs
			) == EGL_FALSE
			)
		{
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
				"eglChooseConfig failed(== EGL_FALSE)!\n" ,
				__FUNCTION__); 
		}

		if( numConfigs == 0 )
		{
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
				"eglChooseConfig failed (numConfigs == 0)!\n" ,
				__FUNCTION__); 
		}

		mEglSurface = createSurfaceFromWindow(mEglDisplay, mWindow);


	}

	void SymbianEGLWindow::reposition( int left, int top )
	{

	}

	void SymbianEGLWindow::resize( unsigned int width, unsigned int height )
	{

	}

	void SymbianEGLWindow::windowMovedOrResized()
	{

	}

	void SymbianEGLWindow::switchFullScreen( bool fullscreen )
	{

	}

	void SymbianEGLWindow::create( const String& name, unsigned int width, unsigned int height, bool fullScreen, const NameValuePairList *miscParams )
	{
		String title = name;
		uint samples = 0;
		int gamma;
		short frequency = 0;
		bool vsync = false;
		::EGLContext eglContext = 0;
		int left = 0;
		int top  = 0;


		mIsFullScreen = fullScreen;

		if (miscParams)
		{
			NameValuePairList::const_iterator opt;
			NameValuePairList::const_iterator end = miscParams->end();

			if ((opt = miscParams->find("currentGLContext")) != end &&
				StringConverter::parseBool(opt->second))
			{
				eglContext = eglGetCurrentContext();
				if (eglContext)
				{
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
						"currentGLContext was specified with no current GL context",
						"EGLWindow::create");
				}

				eglContext = eglGetCurrentContext();
				mEglSurface = eglGetCurrentSurface(EGL_DRAW);
			}

			// Note: Some platforms support AA inside ordinary windows
			if ((opt = miscParams->find("FSAA")) != end)
			{
				samples = StringConverter::parseUnsignedInt(opt->second);
			}

			if ((opt = miscParams->find("displayFrequency")) != end)
			{
				frequency = (short)StringConverter::parseInt(opt->second);
			}

			if ((opt = miscParams->find("vsync")) != end)
			{
				vsync = StringConverter::parseBool(opt->second);
			}

			if ((opt = miscParams->find("gamma")) != end)
			{
				gamma = StringConverter::parseBool(opt->second);
			}

			if ((opt = miscParams->find("left")) != end)
			{
				left = StringConverter::parseInt(opt->second);
			}

			if ((opt = miscParams->find("top")) != end)
			{
				top = StringConverter::parseInt(opt->second);
			}

			if ((opt = miscParams->find("title")) != end)
			{
				title = opt->second;
			}

			if ((opt = miscParams->find("externalGLControl")) != end)
			{
				mIsExternalGLControl = StringConverter::parseBool(opt->second);
			}
		}

		initNativeCreatedWindow(miscParams);

		if (mEglSurface)
		{
			mEglConfig = mGLSupport->getGLConfigFromDrawable (mEglSurface, &width, &height);
		}

		if (!mEglConfig && eglContext)
		{
			mEglConfig = mGLSupport->getGLConfigFromContext(eglContext);

			if (!mEglConfig)
			{
				// This should never happen.
				OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
					"Unexpected failure to determine a EGLFBConfig",
					"EGLWindow::create");
			}
		}

		mIsExternal = (mEglSurface != 0);

		if (!mIsTopLevel)
		{
			mIsFullScreen = false;
			left = top = 0;
		}

		if (mIsFullScreen)
		{
			mGLSupport->switchMode (width, height, frequency);
		}

		if (!mIsExternal)
		{
			createNativeWindow(left, top, width, height, title);
		}

		mContext = createEGLContext();

		::EGLSurface oldDrawableDraw = eglGetCurrentSurface(EGL_DRAW);
		::EGLSurface oldDrawableRead = eglGetCurrentSurface(EGL_READ);
		::EGLContext oldContext  = eglGetCurrentContext();

		EGLint glConfigID;

		mGLSupport->getGLConfigAttrib(mEglConfig, EGL_CONFIG_ID, &glConfigID);
		LogManager::getSingleton().logMessage("EGLWindow::create used FBConfigID = " + StringConverter::toString(glConfigID));

		mName = name;
		mWidth = width;
		mHeight = height;
		mLeft = left;
		mTop = top;
		mActive = true;
		mVisible = true;

		mClosed = false;

	}
}
