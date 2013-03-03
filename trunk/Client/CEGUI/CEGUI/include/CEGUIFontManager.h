/************************************************************************
filename: 	CEGUIFontManager.h
created:	21/2/2004
author:		Paul D Turner

purpose:	Defines interface for the FontManager class
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
#ifndef _CEGUIFontManager_h_
#define _CEGUIFontManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUIIteratorBase.h"
#include "CEGUIFontBase.h"
#include "CEGUIDataContainer.h"
#include <map>
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

	struct CEGUIEXPORT FontBlinkRuntime
	{
		Texture*		d_texture;			//!< The texture of this blink Image
		Rect			d_textureRect;		//!< The texture rect for render quad
		unsigned long	d_quadID;			//!< The id of render quad in render queue.
		ColourRect		d_colorRect;		//!< The Color and Alpha info.
		float			d_originalAlpha;			//!< The Original Alpha of this Runtime's Color.
	};

	struct FontDataStream;

	/*!
	\brief
	Class providing a shared library of Font objects to the system.

	The FontManager is used to create, access, and destroy Font objects.  The idea is that the
	FontManager will function as a central repository for Font objects used within the GUI system,
	and that those Font objects can be accessed, via a unique name, by any interested party within
	the system.
	*/
	class CEGUIEXPORT FontManager : public Singleton<FontManager>
	{
	public:
		/*!
		\brief
		Constructor for FontManager objects
		*/
		FontManager(void);


		/*!
		\brief
		Destructor for FontManager objects
		*/
		~FontManager(void);


		/*!
		\brief
		Return singleton FontManager object

		\return
		Singleton FontManager object
		*/
		static	FontManager&	getSingleton(void);


		/*!
		\brief
		Return pointer to singleton FontManager object

		\return
		Pointer to singleton FontManager object
		*/
		static	FontManager*	getSingletonPtr(void);


		/*!
		\brief
		Creates a new font from a font definition file, and returns a pointer to the new Font object.

		\param type
		String object containing the type of font

		\param filename
		String object containing the filename of a 'font definition file' what will be used to create the new font

		\param resourceGroup
		Resource group identifier to pass to the resource provider when loading the font definition file.

		\return
		Pointer the the newly created Font object

		\exception	FileIOException				thrown if there was some problem accessing or parsing the file \a filename
		\exception	InvalidRequestException		thrown if an invalid filename was provided.
		\exception	AlreadyExistsException		thrown if a Font already exists with the name specified, or if a font Imageset clashes with one already defined in the system.
		\exception	GenericException			thrown if something goes wrong while accessing a true-type font referenced in file \a filename.
		\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the requested glyph imagery.
		\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
		*/
		FontBase*	createFont(FontType type, const String& filename, const String& resourceGroup = "");

		/*!
		\brief
		Destroy's the font with the given name

		\param name
		String object containing the name of the font to be destroyed.  If the specified font does not exist, nothing happens.

		\return
		Nothing
		*/
		void	destroyFont(const String& name);


		/*!
		\brief
		Destroys the given Font object

		\param font
		Pointer to the Font to be destroyed.  If no such Font exists, nothing happens.

		\return
		Nothing.
		*/
		void	destroyFont(FontBase* font);


		/*!
		\brief
		Destroys all Font objects registered in the system

		\return
		Nothing
		*/
		void	destroyAllFonts(void);


		/*!
		\brief
		Checks the existence of a given font.

		\param name
		String object holding the name of the Font object to look for.

		\return
		true if a Font object named \a name exists in the system, false if no such font exists.
		*/
		bool	isFontPresent(const String& name) const;


		/*!
		\brief
		Returns a pointer to the font object with the specified name

		\param name
		String object containing the name of the Font object to be returned

		\return
		Pointer to the requested Font object

		\exception UnknownObjectException	Thrown if no font with the given name exists.
		*/
		FontBase*	getFont(const String& name) const;


		/*!
		\brief
		Notify the FontManager of the current (usually new) display resolution.

		\param size
		Size object describing the display resolution

		\return
		Nothing
		*/
		void	notifyScreenResolution(const Size& size);

		/*!
		\brief
		Writes all font graph to texture.

		\return
		Nothing.
		*/
		void prepareFontTexture(void);

		/*!
		\brief
		Queue the blink txt to be drawn and create a runtime blink in runtime blink list. 

		\param img
		Image* that point to the txt's img.

		\param quadID
		id of render quad in render queue.

		\param position
		Vector3 object containing the location where the Image is to be drawn

		\param size
		Size object describing the size that the Image is to be drawn at.

		\param clip_rect
		Rect object that defines an on-screen area that the Image will be clipped to when drawing.

		\param col_rect
		Colour rect that contain the colours and alpha value.

		\return
		Nothing
		*/
		void	addRuntimeBlink( const Image* img,unsigned long& quadID,const Vector3& position, const Size& size, const Rect& clip_rect, const ColourRect& col_rect);

		/*!
		\brief
		Clears all runtime blink.

		\return
		Nothing
		*/
		void	clearRuntimeBlinkList(void);

		/*!
		\brief 
		Cause all runtime blink in render queue to update itself.

		\return
		Nothing
		*/
		void	updateRuntimeBlinkList(void);

		/*
		\brief 
		Calculate current alpha for blink.

		*/
		void	calculateBlinkAlphaValue(void);

		/*
		\brief
		Get current alpha for blink.

		\return
		current blink alpha value.
		*/
		float	getBlinkAlphaValue(void){ return d_curBlinkAlpha;}

		/*
		\brief
		Set blink one cycle needs how much time. 20s is toooooo big enough for blink's max time.

		\return
		Nothing
		*/
		void	setFontBlinkTime(const float& time = 0.8){ if(time > 0.5f && time < 20.0f) d_totalLife = time; }

		/*
		\brief
		Get font TTF datastream

		\return
		Nothing
		*/
		FontDataStream* getFontDataStream(const String& fontname, const String& resourceGroup, bool encrypted);

	private:
		/*************************************************************************
		Implementation Data
		*************************************************************************/
		typedef	std::map<String, FontBase*>		FontRegistry;
		FontRegistry		d_fonts;

		struct FontManagerImplData;
		FontManagerImplData*	d_implData;

		typedef std::vector< FontBlinkRuntime >		FontBlinkRegistry;

		float					d_timeOld;		//old time that previous caculate the alpha value.
		float					d_totalLife;	//blink one cycle needs how much time -- total life
		float					d_minAlpha;		//alpha smallest value.

		float					d_curBlinkAlpha;		//keep current alapha value.

		FontBlinkRegistry		d_runtimeBlink;

		typedef std::map<String, FontDataStream*> FontDataStreamRegistry;
		FontDataStreamRegistry	d_fontData;

	public:
		/*************************************************************************
		Iterator stuff
		*************************************************************************/
		typedef	ConstBaseIterator<FontRegistry>	FontIterator;

		/*!
		\brief
		Return a FontManager::FontIterator object to iterate over the available Font objects.
		*/
		FontIterator	getIterator(void) const;
	};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIFontManager_h_
