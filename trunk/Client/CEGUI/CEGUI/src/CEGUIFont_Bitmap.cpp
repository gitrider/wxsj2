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
#include "CEGUIFont_Bitmap.h"
#include "CEGUIDataContainer.h"
#include "CEGUISystem.h"
#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"
#include "CEGUILogger.h"
#include "AxBmpFont.h"

#include <windows.h>

// Start of CEGUI namespace section
namespace CEGUI
{

// Invalid code "□" (0xA1F5)
const int INVALID_FIRST		= 0xA1;
const int INVALID_SECOND	= 0xF5;

Font_Bitmap::Font_Bitmap(const String& filename, const String& resourceGroup) 
	:	FontBase(filename, resourceGroup)
	,	d_fontData(0)
{
	d_fontData = new CAxBitmapFont;
}

Font_Bitmap::~Font_Bitmap()
{
	delete d_fontData;
	d_fontData = 0;
}

/*************************************************************************
	Function to do real work of constructor
*************************************************************************/
void Font_Bitmap::constructor_impl(const String& name, const String& fontname, const String& resourceGroup, uint size, bool /*antiAliase*/, bool /*encrypted*/)
{
	// check size valid
	if(12!=size && 14!=size && 16!=size)
	{
		throw GenericException("Font_Bitmap::constructor_impl: Unsupported font size!(" + name + ")");
	}

	d_name = name;
	d_ptSize = size;

	CEGUI::RawDataContainer fontStream;
	System::getSingleton().getResourceProvider()->loadRawDataContainer(fontname, fontStream, resourceGroup);

	// set data point
	if(!(d_fontData->openFromMemory((const char*)(fontStream.getDataPtr()), fontStream.getSize())))
	{
		System::getSingleton().getResourceProvider()->unloadRawDataContainer(fontStream);
		throw GenericException("Font_Bitmap::constructor_impl: Invalid font data!(" + name + ")");
	}

	System::getSingleton().getResourceProvider()->unloadRawDataContainer(fontStream);

	// reprepare font datas.
	resetFontFaces();

	// prepare underline image
	createUnderlineImage();
}

/*************************************************************************
	Recreate font glyph data map and imageset.
*************************************************************************/
void Font_Bitmap::resetFontFaces(void)
{
	// clear font faces and memory
	clearAllFontFaces();

	//insert default data to set font size data
	prepareString((utf8*)DefaultPrepareString);

	d_lineHeight = (float)d_ptSize+InterGlyphPadSpace;

	// calculate spacing and base-line
    d_max_bearingY = (float)d_lineHeight;
	d_lineSpacing = (float)d_ptSize+InterGlyphPadSpace;
}

/*************************************************************************
	Paint the code into the current glyphImageset, This also defines an Image for the glyph in 
	the Imageset for this font, and creates an entry in the code point to Image map.
*************************************************************************/
void Font_Bitmap::createFontGlyph(utf32 code)
{
	//Check is control code
	if((code & 0XFFFF0000) != 0) return;

	int nWidth = (int)d_ptSize;
	int nHeight = (int)d_ptSize;// + InterGlyphPadSpace;

	if(!d_currentGlyphImageset || d_ptNext_Y+d_maxGlyphHeight>d_textureSize)// create new font imageset
	{
		if(d_ptNext_Y+d_maxGlyphHeight>d_textureSize)
		{
			char szTemp[260] = {0};
			_snprintf(szTemp, 260, "*** BugDetected[BI](%d+%d>%d) ***", 
				d_ptNext_Y, d_maxGlyphHeight, d_textureSize);
			Logger::getSingleton().logEvent((utf8*)szTemp);
		}

		d_currentGlyphImageset = createFontImageset(false);
	}
	else
	{
		// Check is enough space to paint new character glyph in this line
		if( d_ptNext_X + nWidth > d_textureSize ) // new line
		{
			d_ptNext_Y += nHeight;
			d_ptNext_X = 0;

			// check is enough space in this imageset.
			if(d_ptNext_Y + d_maxGlyphHeight > d_textureSize)
			{
				d_currentGlyphImageset = createFontImageset(false);
			}
		}
	}

	// calculate offset into buffer for this glyph
	uint16* dest_buff = 
		d_currentGlyphImageset->d_memory + d_ptNext_Y*d_textureSize + d_ptNext_X;

	// draw glyph into buffer
	drawGlyphToBuffer(code, dest_buff, d_textureSize, false);

	// define Image on Imageset for this glyph to save re-rendering glyph later
	Rect rect;
	rect.d_left		= (float)d_ptNext_X;
	rect.d_top		= (float)d_ptNext_Y;
	rect.d_right	= (float)(d_ptNext_X + nWidth);
	rect.d_bottom	= (float)(d_ptNext_Y + nHeight);

	Point offset;
	offset.d_x		=  -1;
	offset.d_y		= -(float)(nHeight-1);

	char imageName[16];
	_snprintf(imageName, 16, "%X", code);
	d_currentGlyphImageset->d_imageset->defineImage(imageName, rect, offset);

	//Next Point
	d_ptNext_X += nWidth;

	if(nHeight > (int)d_maxGlyphHeight) d_maxGlyphHeight = nHeight;

	// create entry in code-point to Image map
	glyphDat	dat;
	dat.d_image = &(d_currentGlyphImageset->d_imageset->getImage(imageName));
	dat.d_horz_advance = d_fontData->getFontWidth(code);
	dat.d_imageBorder = 0;

	//HACK FIX:
	//如果字体宽度等于字体点阵大小，在加宽一个像素，美观一些
	if(dat.d_horz_advance == d_fontData->getFontSize())
	{
		dat.d_horz_advance++;
	}

	d_cp_map[code] = dat;

	//set dirty flag
	d_currentGlyphImageset->d_dirty = true;
}

/*************************************************************************
	Paint the code into the current glyphImageset, This also defines an Image for the glyph in 
	the Imageset for this font, and creates an entry in the code point to Image map.
	[With extern border]
*************************************************************************/
void Font_Bitmap::createFontGlyphBorder(utf32 code)
{
	//Check is control code
	if((code & 0XFFFF0000) != 0) return;

	//Check font glyph
	if(!isCodepointAvailable(code)) return;

	int nWidth = (int)d_ptSize+2;
	int nHeight = (int)d_ptSize+2;

	if(!d_currentBorderGlyphImageset || d_ptBorderNext_Y+nHeight>d_textureSize)// create new font imageset
	{
		if(d_ptBorderNext_Y+nHeight>d_textureSize)
		{
			char szTemp[260] = {0};
			_snprintf(szTemp, 260, "*** BugDetected[BI](%d+%d>%d) ***", 
				d_ptNext_Y, d_maxGlyphHeight, d_textureSize);
			Logger::getSingleton().logEvent((utf8*)szTemp);
		}

		d_currentBorderGlyphImageset = createFontImageset(true);
	}
	else
	{
		// Check is enough space to paint new character glyph in this line
		if( d_ptBorderNext_X + nWidth > d_textureSize ) // new line
		{
			d_ptBorderNext_Y += nHeight;
			d_ptBorderNext_X = 0;

			// check is enough space in this imageset.
			if(d_ptBorderNext_Y + nHeight > d_textureSize)
			{
				d_currentBorderGlyphImageset = createFontImageset(true);
			}
		}
	}

	// calculate offset into buffer for this glyph
	uint16* dest_buff = 
		d_currentBorderGlyphImageset->d_memory + d_ptBorderNext_Y*d_textureSize + d_ptBorderNext_X;

	// draw glyph into buffer
	drawGlyphToBuffer(code, dest_buff, d_textureSize, true);

	// define Image on Imageset for this glyph to save re-rendering glyph later
	Rect rect;
	rect.d_left		= (float)d_ptBorderNext_X;
	rect.d_top		= (float)d_ptBorderNext_Y;
	rect.d_right	= (float)(d_ptBorderNext_X + nWidth);
	rect.d_bottom	= (float)(d_ptBorderNext_Y + nHeight);

	Point offset;
	offset.d_x		=  -1;
	offset.d_y		= -(float)(nHeight-3);

	char imageName[16];
	_snprintf(imageName, 16, "%X", code);
	d_currentBorderGlyphImageset->d_imageset->defineImage(imageName, rect, offset);

	//Next Point
	d_ptBorderNext_X += nWidth;

	//if(nHeight > (int)d_maxGlyphHeight) d_maxGlyphHeight = nHeight;

	// create entry in code-point to Image map
//	glyphDat	dat;
//	dat.d_image = &(d_currentBorderGlyphImageset->d_imageset->getImage(imageName));
//	dat.d_horz_advance = nWidth+1;
//	if(code >= g_nAsciiBegin && code <= g_nAsciiEnd) dat.d_horz_advance /= 2;

	d_cp_map[code].d_imageBorder = &(d_currentBorderGlyphImageset->d_imageset->getImage(imageName));

	//set dirty flag
	d_currentBorderGlyphImageset->d_dirty = true;
}

/*************************************************************************
	Copy the FreeType glyph bitmap into the given memory buffer
*************************************************************************/
void Font_Bitmap::drawGlyphToBuffer(utf32 code, uint16* buffer, uint buf_width, bool drawBorder)
{
	d_fontData->fillFontMemory(code, buffer, buf_width, drawBorder);
}


} // End of  CEGUI namespace section
