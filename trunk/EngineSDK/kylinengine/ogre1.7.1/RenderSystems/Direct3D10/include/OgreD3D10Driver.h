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
#ifndef __D3D10DRIVER_H__
#define __D3D10DRIVER_H__

#include "OgreD3D10Prerequisites.h"

namespace Ogre
{

	class D3D10VideoModeList;
	class D3D10VideoMode;

	class D3D10Driver
	{
	private:
		// D3D only allows one device per adapter, so it can safely be stored
		// here as well.
		D3D10Device & mDevice;
		unsigned int mAdapterNumber;
		DXGI_ADAPTER_DESC mAdapterIdentifier;
		DXGI_MODE_DESC mDesktopDisplayMode;
		D3D10VideoModeList* mpVideoModeList;
		unsigned int tempNo;
		static unsigned int driverCount;
		IDXGIAdapter*	mpDXGIAdapter;
		//DXGI_ADAPTER_DESC mAdapterDesc;


	public:
		// Constructors
		D3D10Driver(D3D10Device & device);		// Default
		D3D10Driver( const D3D10Driver &ob );	// Copy
		D3D10Driver(D3D10Device & device,  unsigned int adapterNumber,  IDXGIAdapter* pDXGIAdapter );
		~D3D10Driver();

		// Information accessors
		String DriverName() const;
		String DriverDescription() const;

		// change the device
		void setDevice(D3D10Device & device);
		unsigned int getAdapterNumber() const;
		const DXGI_ADAPTER_DESC& getAdapterIdentifier() const;
		const DXGI_MODE_DESC& getDesktopMode() const;
		IDXGIAdapter* getDeviceAdapter() const;
		D3D10VideoModeList* getVideoModeList();
	};
}
#endif
