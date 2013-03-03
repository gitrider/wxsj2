/************************************************************************
	filename: 	CEGUIFontBase.cpp
	created:	15/2/2006
	author:		Jean
	
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
#include "CEGUIFontBase.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIFontManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUITexture.h"
#include "CEGUILogger.h"
#include "CEGUITextUtils.h"
#include "CEGUIFont_xmlHandler.h"
#include "CEGUIResourceProvider.h"
#include "CEGUIXMLParser.h"
#include "CEGUIAnimateManager.h"
#include "CEGUIHyperLinkManager.h"
#include "CEGUIWindow.h"

#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	static data definitions
*************************************************************************/
const argb_t	FontBase::DefaultColour			= 0xFFFFFFFF;
const uint		FontBase::InterGlyphPadSpace	= 2;
const uint		FontBase::DefaultFontGlyphImagesetSize	= 512;
const uint		FontBase::LineSpace				= 3;

static colour	g_curTextColor					= 0;	
static colour	g_curTextBakColor				= 0;
static colour	g_curGridentTopCol				= 0;
static colour	g_curGridenBotCol				= 0;
static int		g_isHyperLinkNow				= 0;
static String32	g_keyHyperLink					= "";
static int		g_isKeyHyperLink				= 0;

static bool		g_isBlink						= false;
static bool		g_isIgnoreAlpha					= false;
static float	g_ignoreAlpha					= 0.0f;
static bool		g_isUnderLine					= false;

// XML related strings
const char		FontBase::FontSchemaName[]		= "Font.xsd";
// String32 objects hold some worlds to prepare font size.
const char		FontBase::DefaultPrepareString[]= "a\xE5\x95\x8A"; //"a啊"

const utf32		WORLD_WRAP[] = {(utf32)' ', (utf32)',', (utf32)'.', (utf32)'?', (utf32)'!' };
const utf32*	WORLD_WRAP_END = WORLD_WRAP + sizeof(WORLD_WRAP)/sizeof(utf32);
const int		MAX_WORD_LENGTH = 12;

class Window;
/*************************************************************************
	Constructs a new Font object from a font definition file
*************************************************************************/
FontBase::FontBase(const String& filename, const String& resourceGroup) :
	d_currentGlyphImageset(0),
	d_currentBorderGlyphImageset(0),
    d_lineHeight(0),
    d_lineSpacing(0),
    d_max_bearingY(0),
    d_maxGlyphHeight(0),
	d_ptSize(0),
	d_ptNext_X(0),
	d_ptNext_Y(0),
	d_ptBorderNext_X(0),
	d_ptBorderNext_Y(0),
	d_autoScale(false),
    d_horzScaling(1.0f),
    d_vertScaling(1.0f),
    d_nativeHorzRes(DefaultNativeHorzRes),
    d_nativeVertRes(DefaultNativeVertRes),
	d_textureSize(DefaultFontGlyphImagesetSize),
	d_underlineImageset(0),
	d_underlineImage(0)
{
	memset(d_cp_map, 0, sizeof(glyphDat)*CODEPOINT_MAP_SIZE);
}

/*************************************************************************
	Load and complete construction of 'this' via an XML file
*************************************************************************/
void FontBase::load(const String& filename, const String& resourceGroup)
{
	// unload old data
	unload();

	if (filename.empty() || (filename == (utf8*)""))
	{
		throw InvalidRequestException((utf8*)"FontBase::load - Filename supplied for Font loading must be valid");
	}

    // create handler object
    Font_xmlHandler handler(this);

	// do parse (which uses handler to create actual data)
	try
	{
        System::getSingleton().getXMLParser()->parseXMLFile(handler, filename, FontSchemaName, resourceGroup);
	}
	catch(...)
	{
		unload();

        Logger::getSingleton().logEvent("FontBase::load - loading of Font from file '" + filename +"' failed.", Errors);
		throw;
	}
}


/*************************************************************************
	Destroys a Font object
*************************************************************************/
FontBase::~FontBase(void)
{
	unload();
}

/*************************************************************************
	Unload data associated with the font (font is then useless.
	this is intended for cleanup).
*************************************************************************/
void FontBase::unload(void)
{
	clearAllFontFaces();

	//delete underline image
	ImagesetManager::getSingleton().destroyImageset(d_underlineImageset);
	d_underlineImageset = 0;
	d_underlineImage = 0;
}

/*************************************************************************
	Notify the Font of the current (usually new) display resolution.
*************************************************************************/
void FontBase::notifyScreenResolution(const Size& size)
{
	// notification should come from System which would have notified the Imageset anyway, but
	// in case client code decides to call us, we must pass on notification here too.
	GlyphImagesetList::iterator it, end = d_listGlyphImagesets.end();
	
	for(it=d_listGlyphImagesets.begin(); it!=end; it++)
	{
		glyphImageset* thisGlyph = *it;

		if(thisGlyph->d_imageset)
		{
			thisGlyph->d_imageset->notifyScreenResolution(size);
		}
	}

	d_horzScaling = size.d_width / d_nativeHorzRes;
	d_vertScaling = size.d_height / d_nativeVertRes;

	if (FreeType == getType() && d_autoScale)
	{
		resetFontFaces();
	}
}

/*************************************************************************
	Set the native resolution for this Font
*************************************************************************/
void FontBase::setNativeResolution(const Size& size)
{
	d_nativeHorzRes = size.d_width;
	d_nativeVertRes = size.d_height;

	// set native resolution for underlying imageset for bitmap fonts
	if(Bitmap == getType())
	{
		GlyphImagesetList::iterator it, end = d_listGlyphImagesets.end();
		
		for(it=d_listGlyphImagesets.begin(); it!=end; it++)
		{
			glyphImageset* thisGlyph = *it;

			if(thisGlyph->d_imageset)
			{
				thisGlyph->d_imageset->setNativeResolution(size);
			}
		}
	}
	// re-calculate scaling factors & notify images as required
	notifyScreenResolution(System::getSingleton().getRenderer()->getSize());
}

/*************************************************************************
	Set the texture size
*************************************************************************/
void FontBase::setTextureSize(uint size)
{
	//64, 128, 256, 512, 1024, 2048
	if(size!=64 && size!=128 && size!=256 && size!=512 && size!=1024 && size!=2048) return;

	d_textureSize = size;
}

/*************************************************************************
	Enable or disable auto-scaling for this Font.
*************************************************************************/
void FontBase::setAutoScalingEnabled(bool setting)
{
	if (FreeType == getType() && setting != d_autoScale)
	{
		d_autoScale = setting;
		resetFontFaces();
	}
}

/*************************************************************************
	Prepare the character that need display use this font.
*************************************************************************/
void FontBase::prepareString(const String32& text)
{
	if(text.empty()) return;
	size_t char_count = text.length();

	for (size_t c = 0; c < char_count; ++c)
	{
		utf32 code = text[c];

		// check control code.
		if(code==(utf32)'\r' || code==(utf32)'\n') continue;
		utf32 control_value = (code>>24)&0XFF;
		if(control_value > 0)	continue;

		// check is in font map
		if(!isCodepointAvailable(code))
		{
			createFontGlyph(code);
			//createFontGlyphBorder(code);
		}
	}
}

/*************************************************************************
	Prepare the font texture.
*************************************************************************/
void FontBase::prepareFontTexture(void)
{
	GlyphImagesetList::iterator it, end = d_listGlyphImagesets.end();
	for(it=d_listGlyphImagesets.begin(); it!=end; it++)
	{
		glyphImageset* thisGlyph = *it;

		// font glyph need update to texture.
		if(thisGlyph->d_dirty)
		{
			thisGlyph->d_imageset->getTexture()->loadFromMemoryLA(
				thisGlyph->d_memory, d_textureSize, d_textureSize);
			thisGlyph->d_dirty = false;
		}
	}
}

/*************************************************************************
	Clean all font glyph imagesets and memory.
*************************************************************************/
void FontBase::clearAllFontFaces(void)
{
	// Clear font map
	memset(d_cp_map, 0, sizeof(glyphDat)*CODEPOINT_MAP_SIZE);

	// Clear font glyph imagesets.
	GlyphImagesetList::iterator it, end = d_listGlyphImagesets.end();
	for(it=d_listGlyphImagesets.begin(); it!=end; it++)
	{
		glyphImageset* thisGlyph = *it;

		delete[] thisGlyph->d_memory;
		thisGlyph->d_memory = NULL;

		thisGlyph->d_imageset->undefineAllImages();
		ImagesetManager::getSingleton().destroyImageset(thisGlyph->d_imageset->getName());

		delete thisGlyph;
	}

	d_listGlyphImagesets.clear();
	d_currentGlyphImageset = 0;
	d_ptNext_X = d_ptNext_Y = 0;
	d_maxGlyphHeight = 0;
	d_max_bearingY = 0;
	d_lineSpacing = 0;
	d_lineHeight = 0;
//	d_isHyperLink = 0;
//	d_keyHyperLink = "";
}

/*************************************************************************
	Create a new font glyph struct.
*************************************************************************/
FontBase::glyphImageset* FontBase::createFontImageset(bool forBorder)
{
	// create new font glyph.
	glyphImageset* newGlyphImageset = new glyphImageset;

	newGlyphImageset->d_dirty = true;

	// alloca font glyph memory.
	newGlyphImageset->d_memory = new uint16[d_textureSize * d_textureSize];
	memset(newGlyphImageset->d_memory, 0, ((d_textureSize * d_textureSize) * sizeof(uint16)));

	// create font texture(Imageset)
	char szTemp[260] = {0};
	_snprintf(szTemp, 260, "%s_bigchar%s_%d", d_name.c_str(), forBorder ? "_B" : "", (int)d_listGlyphImagesets.size());
	newGlyphImageset->d_imageset = 
		ImagesetManager::getSingleton().createImageset( 
			String(szTemp), 
			System::getSingleton().getRenderer()->createTexture(float(d_textureSize)));
	newGlyphImageset->d_imageset->disableAutoFree();
	newGlyphImageset->d_imageset->defineImage(
		"view", Point( 0.0f, 0.0f ), 
		Size((float)d_textureSize, (float)d_textureSize ), 
		Point(0.0f, 0.0f));

	//insert imageset list
	d_listGlyphImagesets.push_back(newGlyphImageset);

	if(forBorder)
	{
		// set current imageset
		d_currentBorderGlyphImageset = newGlyphImageset;
		// reset next pos
		d_ptBorderNext_X = d_ptBorderNext_Y = 0;
	}
	else
	{
		// set current imageset
		d_currentGlyphImageset = newGlyphImageset;
		// reset next pos
		d_ptNext_X = d_ptNext_Y = 0;
	}

	return newGlyphImageset;
}

/*************************************************************************
	Return the pixel width of the specified text if rendered with this Font.
*************************************************************************/
float FontBase::getTextExtent(const String32& text, float x_scale, bool bPrepareString) const
{
	// prepare string
	if(bPrepareString)
	{
		((FontBase*)this)->prepareString(text);
	}

    float cur_extent = 0, adv_extent = 0;
	const String32 KeyParserBegin	= "{";
	const String32 KeyParserEnd	= "}";

	bool bIsKeyHyperLink = false,bIsHyperLink = false;

    size_t char_count = text.length();

    for (size_t c = 0; c < char_count; ++c)
    {
		utf32 code = text[c];
		const glyphDat* theGlyphDat = getGlyphData(code);

        if (theGlyphDat && theGlyphDat->d_image != 0)
        {
			if(bIsHyperLink)
			{
				if(code == KeyParserBegin[0])
				{
					bIsKeyHyperLink = true;
					continue;
				}
				else if(code ==KeyParserEnd[0])
				{
					bIsKeyHyperLink = false;
					continue;
				}
				else if(bIsKeyHyperLink)
				{
					continue;
				}
			}
            //adv_extent += (pos->second.d_image->getWidth() + pos->second.d_image->getOffsetX()) * x_scale;
            adv_extent += (float)theGlyphDat->d_horz_advance * x_scale;
			
        }
		else
		{
			if(code==0x20)	//blank
			{
	            adv_extent += (float)this->d_ptSize * x_scale / 2.f;
				continue;
			}

			utf32 value = code & 0x00FFFFFF;
			utf32 type = (code >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;

					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					adv_extent += (float)animate->getSize().d_width * x_scale;
				}
				break;
			case 0xFD:
				{
					bIsHyperLink = true;
				}
				break;
			case 0xFE:
				{
					bIsHyperLink = false;
				}
				break;
			case 0xFC:
				break;
			case 0xFB:
				break;
			case 0xD0:
				break;
			default:
				break;
			}
		}

    }

    return ceguimax(adv_extent, cur_extent);
}

/*************************************************************************
	Return the horizontal pixel extent given text would be formatted to.	
*************************************************************************/
float FontBase::getFormattedTextExtent(const String32& text, const Rect& format_area, TextFormatting fmt, float x_scale) const
{
	float lineWidth;
	float widest = 0;

	size_t lineStart = 0, lineEnd = 0;
	String32	currLine;

	((FontBase*)this)->prepareString(text);

	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of('\n', lineStart)) == String32::npos)
		{
			lineEnd = text.length();
		}

		currLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;	// +1 to skip \n char

		switch(fmt)
		{
		case Centred:
		case RightAligned:
		case LeftAligned:
			lineWidth = getTextExtent(currLine, x_scale, false);
			break;

		case Justified:
			// usually we use the width of the rect but we have to ensure the current line is not wider than that
			lineWidth = ceguimax(format_area.getWidth(), getTextExtent(currLine, x_scale, false));
			break;

		case WordWrapLeftAligned:
		case WordWrapRightAligned:
		case WordWrapCentred:
			lineWidth = getWrappedTextExtent(currLine, format_area.getWidth(), x_scale);
			break;

		case WordWrapJustified:
			// same as above
			lineWidth = ceguimax(format_area.getWidth(), getWrappedTextExtent(currLine, format_area.getWidth(), x_scale));
			break;

		default:
			throw InvalidRequestException((utf8*)"Font::getFormattedTextExtent - Unknown or unsupported TextFormatting value specified.");
		}

		if (lineWidth > widest)
		{
			widest = lineWidth;
		}

	}

	return widest;
}


/*************************************************************************
	returns extent of widest line of wrapped text.
*************************************************************************/
float FontBase::getWrappedTextExtent(const String32& text, float wrapWidth, float x_scale) const
{
	float lineWidth = getTextExtent(text, x_scale);
	if( lineWidth > wrapWidth )
		return wrapWidth;
	return lineWidth;
}

/*************************************************************************
	Return the index of the closest text character in String32 'text' that
	corresponds to pixel location 'pixel' if the text were rendered.
*************************************************************************/
size_t FontBase::getCharAtPixel(const String32& text, size_t start_char, float pixel, float x_scale) const
{
	float cur_extent = 0;
	size_t char_count = text.length();

	// handle simple cases
	if ((pixel <= 0) || (char_count <= start_char))
	{
		return start_char;
	}

	for (size_t c = start_char; c < char_count; ++c)
	{
		utf32 code = text[c];
		const glyphDat* theGlyphDat = getGlyphData(code);

		if(theGlyphDat && theGlyphDat->d_image != 0)
		{
			cur_extent += (float)theGlyphDat->d_horz_advance * x_scale;

			if (pixel < cur_extent)
			{
				return c;
			}

		}
		//extern control code
		else
		{
			utf32 value = code & 0x00FFFFFF;
			utf32 type  = (code >> 24)&0xFF;

			switch(type)
			{
			//face motion
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;

					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;

					const Animate *animate = AnimateManager::getSingleton().getAnimate(id);
					cur_extent += animate->getSize().d_width;

					if (pixel < cur_extent)
					{
						return c;
					}
				}
				break;

			default:
				break;
			}

		}

	}

	return char_count;
}

/*************************************************************************
	Return the size of the given text would be formatted to.
*************************************************************************/
Size FontBase::getFormattedSize(const String32& text, const Rect& draw_area, TextFormatting fmt, float x_scale, float y_scale, float* first_line_height) const
{
	Size szRet(0.0f, 0.0f), sz;
	if(text.empty()) return szRet;

	Rect tmpDrawArea(
		PixelAligned(draw_area.d_left),
		PixelAligned(draw_area.d_top),
		PixelAligned(draw_area.d_right),
		PixelAligned(draw_area.d_bottom)
		);

	size_t lineStart = 0, lineEnd = 0;
	String32	currLine;
	int lineCounts=0;

	while (lineEnd < text.length())
	{
		if(lineStart != 0) 
		{
			szRet.d_height += LineSpace;
		}

		if ((lineEnd = text.find_first_of('\n', lineStart)) == String32::npos)
		{
			lineEnd = text.length();
		}
		if( lineEnd == lineStart )
		{
			szRet.d_height += getFontHeight();
			lineStart = lineEnd + 1;
			continue;
		}

		currLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;	// +1 to skip \n char

		switch(fmt)
		{
		case LeftAligned:
		case RightAligned:
		case Centred:
		case Justified:
			sz = getLineSize(currLine, x_scale, y_scale);
			if(first_line_height && lineCounts==0)
			{
				*first_line_height = sz.d_height;
			}
			break;

		case WordWrapLeftAligned:
		case WordWrapRightAligned:
		case WordWrapCentred:
		case WordWrapJustified:
			sz = getWrappedSize(currLine, tmpDrawArea, x_scale, y_scale, lineCounts==0 ? first_line_height : 0);
			tmpDrawArea.d_top += sz.d_height;
			break;

		default:
			throw InvalidRequestException((utf8*)"Font::drawText - Unknown or unsupported TextFormatting value specified.");
		}

		lineCounts++;

		szRet.d_height += sz.d_height;
		szRet.d_width = ceguimax(szRet.d_width, sz.d_width);
	}

	return szRet;
}

/*************************************************************************
	Return the size of the given wrapped text.
*************************************************************************/
Size FontBase::getWrappedSize(const String32& text, const Rect& draw_area, float x_scale, float y_scale, float* first_line_height) const
{
	Size szRet( 0, 0 );
	float	wrap_width = draw_area.getWidth();
	((FontBase*)this)->prepareString(text);

    float cur_extent = 0;
	float maxHeight = getFontHeight(), height;

    size_t char_count = text.length();
	if( char_count == 0 )
		return szRet;
	String32 thisLine;
	int lineCounts = 0;
    for (size_t c = 0; c < char_count; ++c)
    {
		utf32 code = text[c];
		const glyphDat* theGlyphDat = getGlyphData(code);

		String32 tempLine = thisLine + code;
		float adv_extent = getTextExtent(tempLine, x_scale, false);
        
        if ( adv_extent > cur_extent)
        {
            cur_extent = adv_extent;
        }

		if(theGlyphDat && theGlyphDat->d_image !=0)
		{
			const Image* img = theGlyphDat->d_image;
			height = img->getHeight() * y_scale;
			if(height > maxHeight) maxHeight = height;
		}
		else
		{
			utf32 value = code & 0x00FFFFFF;
			utf32 type = (code >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
 
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					height = animate->getSize().d_height;
					if(height > maxHeight) maxHeight = height;
				}
				break;
			}
		}
		
		if( adv_extent > wrap_width )
		{
			// for english like type-set
			if(System::getSingleton().getCodePage()!=936)
			{
				//is last world not a 'BLANK'
				if(WORLD_WRAP_END != std::find(WORLD_WRAP, WORLD_WRAP_END, code))
				{
					goto BREAK_RIGHT;
				}
					
				//find prev blank
				int to_search_length = std::min(MAX_WORD_LENGTH, (int)thisLine.size());
				for(int i=0; i<to_search_length; i++)
				{
					utf32 checkCode = thisLine[thisLine.size()-1-i];
					//is last world not a 'BLANK'
					if(WORLD_WRAP_END != std::find(WORLD_WRAP, WORLD_WRAP_END, checkCode))
					{
						thisLine = thisLine.substr(0, thisLine.size()-i-1);
						c -= i+1;
						goto BREAK_RIGHT;
					}
				}
			}

	BREAK_RIGHT:
			cur_extent = wrap_width;
			adv_extent = theGlyphDat ? (float)theGlyphDat->d_horz_advance * x_scale : 0.f;
			szRet.d_height += maxHeight + LineSpace;
			maxHeight = getFontHeight();
			cur_extent = wrap_width;

			if(first_line_height && lineCounts==0)
			{
				*first_line_height = szRet.d_height;
			}

			lineCounts++;
			if(thisLine.size() > 1) --c;
			thisLine.clear(); 
		}
		else thisLine += code;
    }

	szRet.d_width = cur_extent;
	szRet.d_height += maxHeight;
	return szRet;

}

int FontBase::getWrapedCharAtPoint(const String32& text, const Point& pt, float max_width, float x_scale, float y_scale) const
{
	Size szRet( 0, 0 );
	float	wrap_width = max_width;
	((FontBase*)this)->prepareString(text);

	float cur_extent = 0;
	float maxHeight = getFontHeight(), height;

	size_t char_count = text.length();
	String32 thisLine;

	bool bCheck = false;	//if true. should check pt.x to find char
	int iRet = 0;

	for (size_t c = 0; c < char_count; ++c)
	{
		utf32 code = text[c];
		const glyphDat* theGlyphData = getGlyphData(code);

		String32 tempLine = thisLine + code;
		
		//0.check txt extent with the wrap_width
		float adv_extent = getTextExtent( tempLine, x_scale, false );
		if( adv_extent > wrap_width )
		{
			cur_extent = wrap_width;
			szRet.d_height += maxHeight + LineSpace;

			//check szRet height with pt.y
			if(szRet.d_height > pt.d_y)
			{
				//ok, should check this line 's txt.
				bCheck = true;
				break;
			}
			maxHeight = getFontHeight();

			//save the preline's character number.
			iRet += thisLine.length();
			if(thisLine.size()>1) --c;
			thisLine.clear();
			continue;
		}

		//1.caculate current line extent
		if ( adv_extent > cur_extent)
		{
			cur_extent = adv_extent;
		}

		//2.caculate height of this text[c], and change max height
		if( theGlyphData && theGlyphData->d_image != 0)
		{
			const Image* img = theGlyphData->d_image;
			height = img->getHeight() * y_scale;
			if(height > maxHeight) maxHeight = height;
		}
		else
		{
			utf32 value = code & 0x00FFFFFF;
			utf32 type = (code >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;

					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					height = animate->getSize().d_height;
					if(height > maxHeight) maxHeight = height;
				}
				break;
			}
		}

		thisLine += code;
	}

	if(!bCheck)
	{
		szRet.d_width = cur_extent;
		szRet.d_height += maxHeight;
		if(szRet.d_height > pt.d_y && szRet.d_width > pt.d_x)
		{
			bCheck = true;
		}
	}

	if(bCheck)
	{
		//ok, should check this line's txt.
        size_t pos = getCharAtPixel(thisLine,0, pt.d_x, x_scale);
		iRet += (int)pos;
		if(pos >= thisLine.length()) iRet -= 1;
		return iRet;
	}

	return -1;
}

/*************************************************************************
	Return the size of the given single line text would be formatted to.
*************************************************************************/
Size FontBase::getLineSize(const String32& text, float x_scale, float y_scale) const
{
	// prepare character in this tring.
	((FontBase*)this)->prepareString(text);

    float cur_extent = 0, adv_extent = 0, width;
	float maxHeight = getFontHeight(), height;

	const String32 KeyParserBegin	= "{";
	const String32 KeyParserEnd	= "}";

	bool bIsKeyHyperLink = false,bIsHyperLink = false;

    size_t char_count = text.length();

    for (size_t c = 0; c < char_count; ++c)
    {
		utf32 code = text[c];
		const glyphDat* theGlyphData = getGlyphData(code);

        if (theGlyphData && theGlyphData->d_image != 0)
        {
//			if(g_isHyperLinkNow || bIsHyperLink)
			if(bIsHyperLink)
			{
				if(code == KeyParserBegin[0])
				{
					bIsKeyHyperLink = true;
					continue;
				}
				else if(code ==KeyParserEnd[0])
				{
					bIsKeyHyperLink = false;
					continue;
				}
				else 
//				if((bIsKeyHyperLink) || (g_isHyperLinkNow))
				if(bIsKeyHyperLink)
				{
					continue;
				}
			}
			const Image* img = theGlyphData->d_image;
            width = (img->getWidth() + img->getOffsetX()) * x_scale;
            
            if (adv_extent + width > cur_extent)
            {
                cur_extent = adv_extent + width;
            }
            
            adv_extent += (float)theGlyphData->d_horz_advance * x_scale;

			height = img->getHeight() * y_scale;
			if(height > maxHeight) maxHeight = height;
			if(height/y_scale > d_lineHeight) *((float*)&d_lineHeight) = height/y_scale;
        }
		else
		{
			if(code==0x20)	//blank
			{
	            adv_extent += (float)this->d_ptSize * x_scale / 2.f;
				continue;
			}

			utf32 value = code & 0x00FFFFFF;
			utf32 type = (code >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
 
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					height = animate->getSize().d_height;
					if(height > maxHeight) maxHeight = height;

					adv_extent += (float)animate->getSize().d_width * x_scale;
				}
				break;
			case 0xFD:
				{
					bIsHyperLink = true;
				}
				break;
			case 0xFE:
				{
					bIsHyperLink = false;
				}
				break;
 

			default:
				break;
			}
		}

    }

	Size szRet;
	szRet.d_width = ceguimax(adv_extent, cur_extent); 
	szRet.d_height = maxHeight * y_scale;

	return szRet;
}

/*************************************************************************
	helper function for renderWrappedText to get next word of a string
*************************************************************************/
size_t FontBase::getNextWord(const String32& in_string, size_t start_idx, String32& out_string) const
{
	out_string = in_string.substr(start_idx, 1);
	return 1;
}

/*************************************************************************
	Renders text on the display.  Return number of lines output.
*************************************************************************/
DrawTextResult FontBase::drawText(const Window* OwnerWindow, const String32& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours, float x_scale, float y_scale ) const
{
	size_t thisCount;
	size_t lineCount = 0;

	float	y_top = draw_area.d_top;

	Rect tmpDrawArea(
		PixelAligned(draw_area.d_left),
		PixelAligned(draw_area.d_top),
		PixelAligned(draw_area.d_right),
		PixelAligned(draw_area.d_bottom)
		);

	size_t lineStart = 0, lineEnd = 0;
	String32	currLine;
	float totalHeight = 0.0f;
	DrawTextResult dtmp;

	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of('\n', lineStart)) == String32::npos)
		{
			lineEnd = text.length();
		}

		currLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;	// +1 to skip \n char

		switch(fmt)
		{
		case LeftAligned:
			dtmp = drawTextLine(OwnerWindow, currLine, Vector3(tmpDrawArea.d_left, y_top, z), clip_rect, colours, x_scale, y_scale);
			thisCount = 1;
			y_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case RightAligned:
			dtmp = drawTextLine(OwnerWindow, currLine, Vector3(tmpDrawArea.d_right - getTextExtent(currLine, x_scale), y_top, z), clip_rect, colours, x_scale, y_scale);
			thisCount = 1;
			y_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case Centred:
			dtmp = drawTextLine(OwnerWindow, currLine, Vector3(PixelAligned(tmpDrawArea.d_left + ((tmpDrawArea.getWidth() - getTextExtent(currLine, x_scale)) / 2.0f))+1, y_top, z), clip_rect, colours, x_scale, y_scale);
			thisCount = 1;
			y_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;
		case WordWrapLeftAligned:
			dtmp = drawWrappedText( OwnerWindow, currLine, tmpDrawArea, z, clip_rect, LeftAligned, colours, x_scale, y_scale);
			thisCount = dtmp.d_lineCount;
			tmpDrawArea.d_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case WordWrapRightAligned:
			dtmp = drawWrappedText( OwnerWindow, currLine, tmpDrawArea, z, clip_rect, RightAligned, colours, x_scale, y_scale);
			thisCount = dtmp.d_lineCount;
			tmpDrawArea.d_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case WordWrapCentred:
			dtmp = drawWrappedText( OwnerWindow,currLine, tmpDrawArea, z, clip_rect, Centred, colours, x_scale, y_scale);
			thisCount = dtmp.d_lineCount;
			tmpDrawArea.d_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case WordWrapJustified:
			// no change needed
			dtmp = drawWrappedText(OwnerWindow, currLine, tmpDrawArea, z, clip_rect, Justified, colours, x_scale, y_scale);
			thisCount = dtmp.d_lineCount;
			tmpDrawArea.d_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		default:
			throw InvalidRequestException((utf8*)"Font::drawText - Unknown or unsupported TextFormatting value specified.");
		}

		lineCount += thisCount;

	}

	DrawTextResult dret;
    // should not return 0
	dret.d_lineCount = ceguimax(lineCount, (size_t)1);
	dret.d_totalHeight = totalHeight;

    return dret;
}

/*************************************************************************
	draws wrapped text
*************************************************************************/
DrawTextResult FontBase::drawWrappedText(const Window* OwnerWindow, const String32& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours, float x_scale, float y_scale ) const
{
	size_t	line_count = 0;
	Rect	dest_area(draw_area);
	float	wrap_width = draw_area.getWidth();
	float	total_height = 0.0f;
	DrawTextResult dtmp;
	size_t char_count = text.length();

	String32 thisLine, tempLine;
	thisLine.reserve(char_count+8);
	tempLine.reserve(char_count+8);

	((FontBase*)this)->prepareString(text);

	for (size_t c = 0; c < char_count; ++c)
	{
		utf32 code = text[c];
		//const glyphDat* theGlyphDat = getGlyphData(code);

		tempLine = thisLine;
		tempLine += code;

		float warp_extern = getTextExtent(tempLine, x_scale, false );
		if( warp_extern > wrap_width )
		{
			// for english like type-set
			if(System::getSingleton().getCodePage()!=936)
			{
				//is last world not a 'BLANK'
				if(WORLD_WRAP_END != std::find(WORLD_WRAP, WORLD_WRAP_END, code))
				{
					goto BREAK_RIGHT;
				}
				
				//find prev blank
				int to_search_length = std::min(MAX_WORD_LENGTH, (int)thisLine.size());
				for(int i=0; i<to_search_length; i++)
				{
					utf32 checkCode = thisLine[thisLine.size()-1-i];
					//is last world not a 'BLANK'
					if(WORLD_WRAP_END != std::find(WORLD_WRAP, WORLD_WRAP_END, checkCode))
					{
						thisLine = thisLine.substr(0, thisLine.size()-i-1);
						c -= i+1;
						goto BREAK_RIGHT;
					}
				}
			}

BREAK_RIGHT:
			// output what we had until this new word
			dtmp = drawText(OwnerWindow, thisLine, dest_area, z, clip_rect, fmt, colours, x_scale, y_scale);
			line_count += dtmp.d_lineCount;

			// reset for a new line.
			if(thisLine.size() > 1) --c;
			thisLine.clear();

			// update y co-ordinate for next line
			dest_area.d_top += dtmp.d_totalHeight;
			dest_area.d_bottom += dtmp.d_totalHeight;
			total_height += dtmp.d_totalHeight;
			warp_extern = 0;
		}
		else 
		{
			// add the next word to the line
			thisLine += code;
		}
	}

	// Last line is left aligned
	TextFormatting last_fmt = (fmt == Justified ? LeftAligned : fmt);
	// output last bit of string
	dtmp = drawText(OwnerWindow, thisLine, dest_area, z, clip_rect, last_fmt, colours, x_scale, y_scale );
	line_count += dtmp.d_lineCount;
	total_height += dtmp.d_totalHeight;

	DrawTextResult dret;
	dret.d_lineCount = line_count;
	dret.d_totalHeight = total_height;
	return dret;
}

void FontBase::drawExternBorder(utf32 code, const Vector3& cur_pos, const Rect& clip_rect, float x_scale, float y_scale) const
{
	if(!isCodepointAvailable(code)) return;
	const glyphDat& glyData = d_cp_map[(unsigned short)(code&0xFFFF)];

	if(!(glyData.d_imageBorder))
	{
		//create border image
		((FontBase*)this)->createFontGlyphBorder(code);

		//request redraw
		System::getSingleton().signalRedraw();
	}

	//check again
	if(!(glyData.d_imageBorder)) return;

	//Draw exten border
	if((g_curTextBakColor.getRed() != 0.0f ||g_curTextBakColor.getGreen() != 0.0f || g_curTextBakColor.getBlue() != 0.0f) )
	{
		float blinkAlpha = 0;
		if(FontManager::getSingletonPtr())
			blinkAlpha = FontManager::getSingleton().getBlinkAlphaValue();

		colour col = g_curTextBakColor;
		col.setAlpha(g_isBlink ? blinkAlpha : g_curTextColor.getAlpha());

		Vector3 vBorderPos = cur_pos;
		vBorderPos.d_x--;
		vBorderPos.d_y--;
		vBorderPos.d_z += System::getSingleton().getRenderer()->getZElementStep() * 0.1f;

		Rect rectClip = clip_rect;
		rectClip.d_left -= 2;
		rectClip.d_top -= 2;
		rectClip.d_bottom += 2;
		rectClip.d_top += 2;

		Size szOfBorder(glyData.d_imageBorder->getWidth()*x_scale, glyData.d_imageBorder->getHeight()*y_scale);
		glyData.d_imageBorder->draw(vBorderPos, szOfBorder, rectClip, ColourRect(col, col, col, col));
	}

}

/*************************************************************************
	Draw a line of text.  No formatting is applied. returns draw result struct.
*************************************************************************/
DrawTextResult FontBase::drawTextLine(const Window* OwnerWindow, const String32& text, const Vector3& position, const Rect& clip_rect, const ColourRect& colours, float x_scale, float y_scale) const
{
	Vector3	cur_pos(position);

	float top_y = position.d_y;
	const String32	KeyParser	= "{}";

	size_t char_count = text.length();

	float			blinkAlpha = 1.0f;
	unsigned long	lquid = 0;

	//Caculate max height and line base.
	float total_height = getFontHeight();
	for (size_t c = 0; c < char_count; ++c)
	{
		utf32 code = text[c];
		const glyphDat* theGlyphDat = getGlyphData(code);
		
		if (theGlyphDat && theGlyphDat->d_image)
		{
			const Image* img = theGlyphDat->d_image;
			float height = img->getHeight() * y_scale;
			if(height > total_height) total_height = height;
			if(height/y_scale > d_lineHeight) *((float*)&d_lineHeight) = height/y_scale;
		}
		else
		{
			utf32 value = code & 0x00FFFFFF;
			utf32 type = (code >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);
					
					float height = animate->getSize().d_height;
					if(height > total_height) total_height = height;
				}
				break;
			default:
				break;
			}
		}

	}//for (size_t c = 0; c < char_count; ++c)

	float base_y = top_y+(getType() == PixelMap ? 0 : total_height-InterGlyphPadSpace-1);

	if(FontManager::getSingletonPtr())
		blinkAlpha = FontManager::getSingleton().getBlinkAlphaValue();

	for (size_t c = 0; c < char_count; ++c)
	{
		utf32 code = text[c];
		const glyphDat* theGlyphDat = getGlyphData(code);

		if(theGlyphDat && theGlyphDat->d_image)
		{
			const Image* img = theGlyphDat->d_image;
			cur_pos.d_y = base_y - (img->getOffsetY() - img->getOffsetY() * y_scale);
			Size sz(img->getWidth() * x_scale, img->getHeight() * y_scale);

			if(g_isHyperLinkNow)
			{
				if(code == KeyParser[0] )
				{
					g_isKeyHyperLink = 1;
					continue;
				}
				else if(code == KeyParser[1] )
				{
					g_isKeyHyperLink = 0;
					continue;
				}
				else if (g_isKeyHyperLink == 1)
				{
					g_keyHyperLink += code;
					continue;
				}
				else
				{
					if( HyperLinkManager::getSingletonPtr() )
					{
						Vector2 vRange;
						vRange.d_x = cur_pos.d_x;
						vRange.d_y = top_y;
						Rect newRect(vRange,sz);

						colour	curTextBakColor = 0;
						utf32	curValue = 0;
						curValue |= utf32(0xFF00CCFF);
						
						HyperLinkManager::getSingleton().addHyperLink(OwnerWindow,g_keyHyperLink,newRect, curValue);						
						curTextBakColor.setARGB(curValue);
						
						drawExternBorder(code, cur_pos, clip_rect, x_scale, y_scale);

						if(g_isBlink && FontManager::getSingletonPtr())
						{
							curTextBakColor.setAlpha(blinkAlpha);
							lquid = img->draw(cur_pos, sz, clip_rect, ColourRect(curTextBakColor, curTextBakColor, curTextBakColor, curTextBakColor), TopLeftToBottomRight, true);
							//draw underline
							if(g_isUnderLine && d_underlineImage)
							{
								d_underlineImage->draw(
									Vector3(cur_pos.d_x, cur_pos.d_y+img->getOffsetY(), cur_pos.d_z), 
									Size((float)theGlyphDat->d_horz_advance * x_scale, sz.d_height), 
									clip_rect, ColourRect(curTextBakColor, curTextBakColor, curTextBakColor, curTextBakColor));
							}
							FontManager::getSingleton().addRuntimeBlink(img, lquid, cur_pos, sz, clip_rect, 
								ColourRect(curTextBakColor, curTextBakColor, curTextBakColor, curTextBakColor));
						}
						else
						{
							img->draw(cur_pos, sz, clip_rect, ColourRect(curTextBakColor, curTextBakColor, curTextBakColor, curTextBakColor));
							//draw underline
							if(g_isUnderLine && d_underlineImage)
							{
								d_underlineImage->draw(
									Vector3(cur_pos.d_x, cur_pos.d_y+img->getOffsetY(), cur_pos.d_z), 
									Size((float)theGlyphDat->d_horz_advance * x_scale, sz.d_height), 
									clip_rect, ColourRect(curTextBakColor, curTextBakColor, curTextBakColor, curTextBakColor));
							}
						}
						cur_pos.d_x += (float)theGlyphDat->d_horz_advance * x_scale;

						continue;
					}
				}

			}//if(g_isHyperLinkNow)

			drawExternBorder(code, cur_pos, clip_rect, x_scale, y_scale);

			if(g_isBlink && FontManager::getSingletonPtr())
			{
				//adjust alpha
				if(g_curGridentTopCol == g_curGridenBotCol)
				{
					colour blinkColor(g_curTextColor);
					blinkColor.setAlpha(blinkAlpha);

					//draw blink txt
					lquid = img->draw(cur_pos, sz, clip_rect, 
						ColourRect(blinkColor, blinkColor, blinkColor, blinkColor), TopLeftToBottomRight, true);
					//draw underline
					if(g_isUnderLine && d_underlineImage)
					{
						d_underlineImage->draw(
							Vector3(cur_pos.d_x, cur_pos.d_y+img->getOffsetY(), cur_pos.d_z), 
							Size((float)theGlyphDat->d_horz_advance * x_scale, sz.d_height), 
							clip_rect, ColourRect(blinkColor, blinkColor, blinkColor, blinkColor));
					}
					FontManager::getSingleton().addRuntimeBlink(img, lquid, cur_pos, sz, clip_rect, 
						ColourRect(blinkColor, blinkColor, blinkColor, blinkColor));
				}
				else
				{
					colour blinkTopColor(g_curGridentTopCol);
					blinkTopColor.setAlpha(blinkAlpha);

					colour blinkBotColor(g_curGridenBotCol);
					blinkBotColor.setAlpha(blinkAlpha);

					//draw blink txt
					lquid = img->draw(cur_pos, sz, clip_rect, 
			 			ColourRect(blinkTopColor, blinkTopColor, blinkBotColor, blinkBotColor), TopLeftToBottomRight, true);
					//draw underline
					if(g_isUnderLine && d_underlineImage)
					{
						d_underlineImage->draw(
							Vector3(cur_pos.d_x, cur_pos.d_y+img->getOffsetY(), cur_pos.d_z), 
							Size((float)theGlyphDat->d_horz_advance * x_scale, sz.d_height), 
							clip_rect, ColourRect(blinkBotColor, blinkBotColor, blinkBotColor, blinkBotColor));
					}
					FontManager::getSingleton().addRuntimeBlink(img, lquid, cur_pos, sz, clip_rect, 
						ColourRect(blinkTopColor, blinkTopColor, blinkBotColor, blinkBotColor));

				}//if(g_curGridentTopCol == g_curGridenBotCol)
			}
			else
			{
				if(g_curGridentTopCol != g_curGridenBotCol)
				{
					img->draw(cur_pos, sz, clip_rect, ColourRect(g_curGridentTopCol, g_curGridentTopCol, g_curGridenBotCol, g_curGridenBotCol));
					//draw underline
					if(g_isUnderLine && d_underlineImage)
					{
						d_underlineImage->draw(
							Vector3(cur_pos.d_x, cur_pos.d_y+img->getOffsetY(), cur_pos.d_z), 
							Size((float)theGlyphDat->d_horz_advance * x_scale, sz.d_height), 
							clip_rect, ColourRect(g_curGridenBotCol, g_curGridenBotCol, g_curGridenBotCol, g_curGridenBotCol));
					}
				}
				else
				{
					img->draw(cur_pos, sz, clip_rect, ColourRect(g_curTextColor, g_curTextColor, g_curTextColor, g_curTextColor));
					//draw underline
					if(g_isUnderLine && d_underlineImage)
					{
						d_underlineImage->draw(
							Vector3(cur_pos.d_x, cur_pos.d_y+img->getOffsetY(), cur_pos.d_z), 
							Size((float)theGlyphDat->d_horz_advance * x_scale, sz.d_height), 
							clip_rect, ColourRect(g_curTextColor, g_curTextColor, g_curTextColor, g_curTextColor));
					}

				}//if(g_curGridentTopCol != g_curGridenBotCol)

			}//if(g_isBlink && FontManager::getSingletonPtr())

			cur_pos.d_x += (float)theGlyphDat->d_horz_advance * x_scale;
		}
		else
		{
			if(code==0x20)	//blank
			{
	            cur_pos.d_x += (float)this->d_ptSize * x_scale / 2.f;
				continue;
			}

			utf32 value = code & 0x00FFFFFF;
			utf32 type = (code >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
 
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					cur_pos.d_y = base_y - animate->getSize().d_height+LineSpace;
					Size sz(animate->getSize());
					//超链接的{}中不允许有#1~#99这种表情出现。过滤掉，不显示。
					if( g_isHyperLinkNow == 1)
					{
						if( g_isKeyHyperLink == 1 )
							break;
						if( HyperLinkManager::getSingletonPtr() )
						{
							Vector2 vRange;
							vRange.d_x = cur_pos.d_x;
							vRange.d_y = cur_pos.d_y;
							Rect newRect(vRange,sz);
							utf32 ocolor;
							HyperLinkManager::getSingleton().addHyperLink(OwnerWindow,g_keyHyperLink,newRect, ocolor);
						}
					}

					//Defulat: 0.1sec per frame
					int time_total = (int)(animate->getFrameNumber()*33.33f);
					int time_now = (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);
					colour col( 1,1,1,1 );
					col.setAlpha( g_curTextColor.getAlpha() );
					AnimateManager::getSingleton().draw(id, 0, time_now, time_total, 
						cur_pos, sz, clip_rect, col );
					cur_pos.d_x += animate->getSize().d_width;
				}
				break;
 
			case 0xFB:
				{
					value |= utf32( colours.d_top_left.getAlpha() * 255 ) << 24; 
					g_curTextColor.setARGB( value );
					if(g_isIgnoreAlpha) g_curTextColor.setAlpha(g_ignoreAlpha);
				}
				break;
 
			case 0xFC:
				{
					
					value |= utf32( colours.d_top_left.getAlpha() * 255 ) << 24;
					g_curTextBakColor.setARGB( value );
					if(g_isIgnoreAlpha) g_curTextBakColor.setAlpha(g_ignoreAlpha);

					g_curGridentTopCol = g_curGridenBotCol = 0;
				}
				break;

			case 0xD0:
				{
					g_curGridentTopCol.setARGB(
						utf32(colours.d_top_left.getAlpha() * 255)<<24 |
						(((value&0x00F00000)>>20)*17 << 16) |
						(((value&0x000F0000)>>16)*17 << 8) |
						(((value&0x0000F000)>>12)*17 << 0) );

					g_curGridenBotCol.setARGB(
						utf32(colours.d_top_left.getAlpha() * 255)<<24 |
						(((value&0x00000F00)>>8)*17 << 16) |
						(((value&0x000000F0)>>4)*17 << 8) |
						(((value&0x0000000F)>>0)*17 << 0) );
					if(g_isIgnoreAlpha) 
					{
						g_curGridentTopCol.setAlpha(g_ignoreAlpha);
						g_curGridenBotCol.setAlpha(g_ignoreAlpha);
					}
				}
				break;

			case 0xFD:
				{
					g_isHyperLinkNow = 1;
				}
				break;
			case 0xFE:
				{
					g_isHyperLinkNow = 0;
					g_keyHyperLink = "";
				}
				break;
			case 0xE0:
				{
					g_isBlink = true;
				}
				break;
			case 0xE1:
				{
					g_isBlink = false;
				}
				break;
			case 0xE2:
				{
					if(value > 0xFF)
					{
						g_isIgnoreAlpha = false;
						g_ignoreAlpha = colours.d_top_left.getAlpha();

						g_curTextColor.setAlpha(g_ignoreAlpha);
						g_curTextBakColor.setAlpha(g_ignoreAlpha);
					}
					else
					{
						g_isIgnoreAlpha = true;
						value &= 0xFF;
						g_ignoreAlpha = (float)value/255;

						g_curTextColor.setAlpha(g_ignoreAlpha);
						g_curTextBakColor.setAlpha(g_ignoreAlpha);
					}
				}
				break;
			case 0xE4:
			case 0xE5:
				break;

			case 0xE6:
				g_isUnderLine = true;
				break;

			case 0xE7:
				g_isUnderLine = false;
				break;
				
			default:
				break;

			}//switch(type)

		}//if(theGlyphDat && theGlyphDat->d_image)

	}//for (size_t c = 0; c < char_count; ++c)
 
	DrawTextResult dret;
	dret.d_lineCount = 1;
	dret.d_totalHeight = total_height + LineSpace;
	return dret;
}

int FontBase::debug_GetFontImageset(std::vector< Imageset* >& vTexture)
{
	if((d_listGlyphImagesets.size()+1) > vTexture.size()) 
		return (int)d_listGlyphImagesets.size()+1;

	GlyphImagesetList::iterator it;
	int i=0;
	for(it=d_listGlyphImagesets.begin(); it!=d_listGlyphImagesets.end(); it++, i++)
	{
		vTexture[i] = (*it)->d_imageset;
	}

	//add underline image
	if(d_underlineImageset)
	{
		vTexture[i] = d_underlineImageset;
	}

	return(int)d_listGlyphImagesets.size();
}

void FontBase::createUnderlineImage(void)
{
	//round up to pow of 2
	int UNDERLINE_IMAGE_SIZE = d_ptSize;
	while(!(UNDERLINE_IMAGE_SIZE==4 || 
			UNDERLINE_IMAGE_SIZE==8 ||
			UNDERLINE_IMAGE_SIZE==16 ||
			UNDERLINE_IMAGE_SIZE==32 ||
			UNDERLINE_IMAGE_SIZE==64 ||
			UNDERLINE_IMAGE_SIZE==128 ||
			UNDERLINE_IMAGE_SIZE>=256))
	{
		UNDERLINE_IMAGE_SIZE++;
	}

	// alloca font glyph memory.
	uint16* pTempMemory = new uint16[UNDERLINE_IMAGE_SIZE * UNDERLINE_IMAGE_SIZE];
	memset(pTempMemory, 0, ((UNDERLINE_IMAGE_SIZE * UNDERLINE_IMAGE_SIZE) * sizeof(uint16)));

	//draw underline
	//for(int i=d_ptSize; i<d_ptSize+1; i++)
	for(int j=0; j<UNDERLINE_IMAGE_SIZE; j++)
	{
		pTempMemory[UNDERLINE_IMAGE_SIZE*(d_ptSize-1) + j] = 0xFFFF;
	}

	// create font texture(Imageset)
	char szTemp[260] = {0};
	_snprintf(szTemp, 260, "%s_bigchar_underline", d_name.c_str());

	d_underlineImageset = 
		ImagesetManager::getSingleton().createImageset( 
			String(szTemp), 
			System::getSingleton().getRenderer()->createTexture(float(UNDERLINE_IMAGE_SIZE)));
	d_underlineImageset->disableAutoFree();
	d_underlineImageset->defineImage(
		"view", Point( 0.0f, 0.0f ), 
		Size((float)d_ptSize, (float)d_ptSize ), 
		Point(0.0f, 0.0f));

	//insert imageset list
	d_underlineImage = &(d_underlineImageset->getImage("view"));

	//load memory to texture
	d_underlineImageset->getTexture()->loadFromMemoryLA(
				pTempMemory, UNDERLINE_IMAGE_SIZE, UNDERLINE_IMAGE_SIZE);

	//free temp memory
	delete[] pTempMemory; pTempMemory=0;
}


} // End of  CEGUI namespace section

