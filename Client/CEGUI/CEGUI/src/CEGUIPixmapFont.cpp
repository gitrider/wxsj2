/***********************************************************************
	filename: 	CEGUIPixmapFont.cpp
	created:	14/6/2006
	author:		Andrew Zabolotny

	purpose:    Implementation of the Font class via static imagesets
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUIPixmapFont.h"
#include "CEGUIImageset.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUILogger.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

static const String BuiltInResourceGroup ("*");
static const String MappingCodepointAttribute ("Codepoint");
static const String MappingImageAttribute ("Image");
static const String MappingHorzAdvanceAttribute ("HorzAdvance");

PixmapFont::PixmapFont (const String& fontname,  const String& resourceGroup) :
    FontBase ( fontname, resourceGroup),
    d_glyphImages (0),
    d_origHorzScaling (1.0)
{
    //reinit ();
}

//PixmapFont::PixmapFont (const XMLAttributes& attributes) :
//    FontBase(attributes),
//    d_glyphImages (0),
//    d_origHorzScaling (1.0)
//{
//    reinit ();
//}

PixmapFont::~PixmapFont ()
{
    if (d_imagesetOwner)
        ImagesetManager::getSingleton ().destroyImageset (d_glyphImages);
}

void PixmapFont::reinit (const String& filename, const String& resourceGroup)
{
    if (d_imagesetOwner)
        ImagesetManager::getSingleton ().destroyImageset (d_glyphImages);

    if (resourceGroup == BuiltInResourceGroup)
    {
        d_glyphImages = ImagesetManager::getSingleton ().getImageset (filename);
        d_imagesetOwner = false;
    }
    else
    {
        d_glyphImages = ImagesetManager::getSingleton ().createImageset (filename, resourceGroup);
        d_imagesetOwner = true;
    }
}

void PixmapFont::updateFont ()
{
    //float factor = (d_autoScale ? d_horzScaling : 1.0f) / d_origHorzScaling;

    //d_ascender = 0;
    //d_descender = 0;
    //d_height = 0;
    //d_maxCodepoint = 0;

    //d_glyphImages->setAutoScalingEnabled (d_autoScale);
    //d_glyphImages->setNativeResolution (Size (d_nativeHorzRes, d_nativeVertRes));

    //for (register INT i=0; i<CODEPOINT_MAP_SIZE; i++)
    //{
    //    if (d_cp_map[i] > 0)
    //        ++d_maxCodepoint ;

    //    ((FontGlyph &)i->second).setAdvance (i->second.getAdvance () * factor);

    //    const Image *img = i->second.getImage ();

    //    if (img->getOffsetY () < d_ascender)
    //        d_ascender = img->getOffsetY ();
    //    if (img->getHeight () + img->getOffsetY () > d_descender)
    //        d_descender = img->getHeight () + img->getOffsetY ();
    //}

    //d_ascender = -d_ascender;
    //d_descender = -d_descender;
    //d_height = d_ascender - d_descender;

    //if (d_autoScale)
    //    d_origHorzScaling = d_horzScaling;
    //else
    //    d_origHorzScaling = 1.0;
}

//void PixmapFont::writeXMLToStream_impl (XMLSerializer& xml_stream) const
//{
//    float advscale = 1.0f / d_origHorzScaling;
//    for (CodepointMap::const_iterator i = d_cp_map.begin (); i != d_cp_map.end (); ++i)
//    {
//        xml_stream.openTag ("Mapping")
//            .attribute (MappingCodepointAttribute, PropertyHelper::uintToString (i->first))
//            .attribute (MappingHorzAdvanceAttribute, PropertyHelper::floatToString (i->second.getAdvance () * advscale))
//            .attribute (MappingImageAttribute, i->second.getImage ()->getName ());
//
//        xml_stream.closeTag();
//    }
//}

void PixmapFont::defineMapping (const XMLAttributes& attributes)
{
    String image_name (attributes.getValueAsString (MappingImageAttribute));
    utf32 codepoint = (utf32)attributes.getValueAsInteger (MappingCodepointAttribute);
    float horzAdvance = attributes.getValueAsFloat (MappingHorzAdvanceAttribute, -1.0);
    defineMapping (image_name, codepoint, horzAdvance);
}

void PixmapFont::defineMapping (String image_name, utf32 codepoint, float horzAdvance)
{
    const Image *image = &d_glyphImages->getImage (image_name);

    // calculate advance width if it was not specified
    if (horzAdvance == -1.0f)
        horzAdvance = (float)(int)(image->getWidth () + image->getOffsetX ());

    if (d_autoScale)
        horzAdvance *= d_origHorzScaling;

    // create a new FontGlyph with given character code
	d_cp_map [codepoint].d_horz_advance = horzAdvance;
	d_cp_map [codepoint].d_image = image;
	d_cp_map [codepoint].d_imageBorder = NULL;
}

void PixmapFont::load (const String& filename, const String& resourceGroup)
{
    // log the start of font creation.
    Logger::getSingleton ().logEvent ("Started creation of Pixmap Font:");
    Logger::getSingleton ().logEvent ("---- CEGUI font name: " + d_name);
    Logger::getSingleton ().logEvent ("----     Source file: " + filename +
                                      " in resource group: " + (resourceGroup.empty () ?
                                                                "(Default)" : resourceGroup));

    updateFont ();

    char tmp [50];
    snprintf(tmp, sizeof(tmp),
             "Succsessfully loaded %d glyphs",
             static_cast<int>(d_maxCodepoint));

    Logger::getSingleton ().logEvent (tmp);
}
void	PixmapFont::createFontGlyph(utf32 code)
{

}
void	PixmapFont::createFontGlyphBorder(utf32 code)
{

}
void	PixmapFont::constructor_impl(const String& name, const String& fontname, const String& resourceGroup, uint size, bool antiAliase, bool encrypted) 
{
	reinit(fontname, BuiltInResourceGroup);
	d_name = name;
}
void	PixmapFont::drawGlyphToBuffer(utf32 code, uint16* buffer, uint buf_width, bool drawBorder)
{

}

void	PixmapFont::resetFontFaces(void) 
{

}

} // End of  CEGUI namespace section
