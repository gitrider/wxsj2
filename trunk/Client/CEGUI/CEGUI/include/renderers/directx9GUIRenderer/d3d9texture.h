/************************************************************************
	filename: 	d3d9texture.h
	created:	17/7/2004
	author:		Paul D Turner with D3D 9 Updates by Magnus �sterlind
	
	purpose:	Defines concrete texture class for D3D9.0
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _d3d9texture_h_
#define _d3d9texture_h_

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"
#include "d3d9renderer.h"
#include <d3d9.h>
#include <list>

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Texture class that is created by DirectX9Renderer objects
*/
class DIRECTX9_GUIRENDERER_API DirectX9Texture : public Texture
{
private:
	/*************************************************************************
		Friends (to allow construction and destruction)
	*************************************************************************/
	friend	Texture* DirectX9Renderer::createTexture(void);
	friend	Texture* DirectX9Renderer::createTexture(const String& filename, const String& resourceGroup);
	friend	Texture* DirectX9Renderer::createTexture(float size);
	friend	void	 DirectX9Renderer::destroyTexture(Texture* texture);


	/*************************************************************************
		Construction & Destruction (by Renderer object only)
	*************************************************************************/
	DirectX9Texture(Renderer* owner);
	virtual ~DirectX9Texture(void);

public:
	/*!
	\brief
		Returns the current pixel width of the texture

	\return
		ushort value that is the current width of the texture in pixels
	*/
	virtual	ushort	getWidth(void) const		{return d_width;}


	/*!
	\brief
		Returns the current pixel height of the texture

	\return
		ushort value that is the current height of the texture in pixels
	*/
	virtual	ushort	getHeight(void) const		{return d_height;}


	/*!
	\brief
		Loads the specified image file into the texture.  The texture is resized as required to hold the image.

	\param filename
		The filename of the image file that is to be loaded into the texture

    \param resourceGroup
        Resource group identifier passed to the resource provider.

	\return
		Nothing.
	*/
	virtual void	loadFromFile(const String& filename, const String& resourceGroup);


	/*!
	\brief
		Loads (copies) an image in memory into the texture.  The texture is resized as required to hold the image.

	\param buffPtr
		Pointer to the buffer containing the image data

	\param buffWidth
		Width of the buffer (in 0xAARRGGBB pixels)

	\param buffHeight
		Height of the buffer (in 0xAARRGGBB pixels)

	\return
		Nothing.
	*/
	virtual void	loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight);


	/*!
	\brief
		Return a pointer to the internal Direct3DTexture9 object

	\return
		Pointer to the IDirect3DTexture9 interface currently being used by this Texture object
	*/
	LPDIRECT3DTEXTURE9	getD3DTexture(void) const		{return d_d3dtexture;}


	// 
	/*!
	\brief
		set the size of the internal D3D texture.  Previous D3D texture is lost.

	\param size
		pixel size of the new internal texture.  This will be rounded up to a power of 2.

	\return
		Nothing.
	*/
	void	setD3DTextureSize(uint size);


	/*!
	\brief
		Direct3D support method that must be called prior to a Reset call on the
		Direct3DDevice; this is required so that the GUI renderer can release any
		unmanaged D3D resources as needed for the device reset to succeed.

	\note
		You do not need to call this on DirectX9Texture objects created via the
		DirectX9Renderer object; for such textures the method is called by the
		system when you call the DirectX9Renderer::preD3DReset method.
	*/
	virtual	void	preD3DReset(void);


	/*!
	\brief
		Direct3D support method that must be called after a Reset call on the
		Direct3DDevice; this is required so that the GUI renderer can rebuild any
		unmanaged D3D resources after the device has been reset.

	\note
		You do not need to call this on DirectX9Texture objects created via the
		DirectX9Renderer object; for such textures the method is called by the
		system when you call the DirectX9Renderer::postD3DReset method.
	*/
	virtual	void	postD3DReset(void);


private:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	// safely free direc3d texture (can be called multiple times with no ill effect)
	void	freeD3DTexture(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	LPDIRECT3DTEXTURE9		d_d3dtexture;		//!< The 'real' texture.
	String					d_filename;			//!< name of file used to create the texture, if any.
    String                  d_resourceGroup;    //!< Group from which the file was loaded.
	bool					d_isMemoryTexture;	//!< true if the texture was created from memory (and not a file).

	ushort					d_width;			//!< cached width of the texture
	ushort					d_height;			//!< cached height of the texture
};

} // End of  CEGUI namespace section


#endif	// end of guard _d3d9texture_h_
