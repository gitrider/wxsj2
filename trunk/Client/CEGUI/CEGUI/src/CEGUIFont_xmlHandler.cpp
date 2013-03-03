/************************************************************************
filename: 	CEGUIFont.cpp
created:	21/2/2004
author:		Paul D Turner

purpose:	Implements Font class
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
#include "CEGUIFont_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUISystem.h"
#include "CEGUIPixmapFont.h"

#include <ft2build.h>
#include FT_FREETYPE_H


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
static data definitions
*************************************************************************/

// XML related strings
const String Font_xmlHandler::FontElement( (utf8*)"Font" );
const String Font_xmlHandler::FontsElement( (utf8*)"Fonts" );
const char	Font_xmlHandler::FontNameAttribute[]			= "Name";
const char	Font_xmlHandler::FontFilenameAttribute[]		= "Filename";
const char	Font_xmlHandler::FontResourceGroupAttribute[]   = "ResourceGroup";
const char	Font_xmlHandler::FontTypeAttribute[]			= "Type";
const char	Font_xmlHandler::FontSizeAttribute[]			= "Size";
const char	Font_xmlHandler::FontNativeHorzResAttribute[]	= "NativeHorzRes";
const char	Font_xmlHandler::FontNativeVertResAttribute[]	= "NativeVertRes";
const char	Font_xmlHandler::FontAutoScaledAttribute[]		= "AutoScaled";
const char	Font_xmlHandler::FontAntiAliasedAttribute[]		= "AntiAlias";
const char	Font_xmlHandler::FontTextureSizeAttribute[]		= "TextureSize";
const char	Font_xmlHandler::FontCodePageAttribute[]		= "CodePage";
const char	Font_xmlHandler::FontEncrypt[]					= "NeedCode";
const char	Font_xmlHandler::FontMapping[]					= "Mapping";

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void Font_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
{
	if (element == FontElement)
	{
		// get code page for
		unsigned int codepage = (int)attributes.getValueAsInteger(FontCodePageAttribute, 936);
		if(codepage != System::getSingleton().getCodePage()) return;

		// get name of font we are creating
		String font_name(attributes.getValueAsString(FontNameAttribute));

		// get filename for the font
        String filename(attributes.getValueAsString(FontFilenameAttribute));
        // get resource group for font file.
        String resourceGroup(attributes.getValueAsString(FontResourceGroupAttribute));

		Logger::getSingleton().logEvent("Started creation of Font '" + font_name + "' via XML file.", Informative);

		//
		// load auto-scaling configuration
		//
		float hres, vres;
		bool auto_scale;

		// get native horizontal resolution
        hres = (float)attributes.getValueAsInteger(FontNativeHorzResAttribute, 640);
		// get native vertical resolution
        vres = (float)attributes.getValueAsInteger(FontNativeVertResAttribute, 480);
		// get size of font
		uint size = (uint)attributes.getValueAsInteger(FontSizeAttribute, 12);
		// get antialiase attribute
		bool antiAliase = (bool)attributes.getValueAsBool(FontAntiAliasedAttribute, true);
		// get size of texture size
		uint texSize = (uint)attributes.getValueAsInteger(FontTextureSizeAttribute, 512);

		// perform pre-initialisation
		d_font->setNativeResolution(Size(hres, vres));

		// set texture size
		d_font->setTextureSize(texSize);

		// get auto-scaling setting
		auto_scale = attributes.getValueAsBool(FontAutoScaledAttribute, false);
		d_font->setAutoScalingEnabled(auto_scale);

		// is the font data be encrypted
		bool encrypt = attributes.getValueAsBool(FontEncrypt, false);

		// Finalise construction of font.
		d_font->constructor_impl(font_name, filename, resourceGroup, size, antiAliase, encrypt);
	}
	else if(element == FontsElement)
	{
		//Do nothing...
	}
	else if(element == FontMapping && d_font->getType() == PixelMap)
	{
		((PixmapFont*)d_font)->defineMapping(attributes);
	}
	// anything else is an error which *should* have already been caught by XML validation
	else
	{
		throw FileIOException("Font::xmlHandler::startElement - Unexpected data was found while parsing the Font file: '" + element + "' is unknown.");
	}

}

void Font_xmlHandler::elementEnd(const String& element)
{
	if (element == FontElement)
	{
		Logger::getSingleton().logEvent("Finished creation of Font '" + d_font->d_name + "' via XML file.", Informative);
	}

}

} // End of  CEGUI namespace section
