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
#include "CEGUIFont_FreeType.h"
#include "CEGUIFont_ImplData.h"
#include "CEGUIFont_DataStream.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFontManager.h"
#include "CEGUISystem.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"

// Start of CEGUI namespace section
namespace CEGUI
{

Font_FreeType::Font_FreeType(const String& filename, const String& resourceGroup, FontImplData* dat) :
	FontBase(filename, resourceGroup),
	d_impldat(dat),
	d_antiAliase(true)
{

}

/*************************************************************************
    Destroys a Font object
*************************************************************************/
Font_FreeType::~Font_FreeType(void)
{
    unload();
    delete d_impldat;
}

/*************************************************************************
	Function to do real work of constructor
*************************************************************************/
void Font_FreeType::constructor_impl(const String& name, const String& fontname, const String& resourceGroup, uint size, bool antiAliase, bool encrypted)
{
	String		errMsg;

	FontDataStream* fontDataStream = 
		FontManager::getSingleton().getFontDataStream(fontname, resourceGroup, encrypted);

	// create face using input font
	FT_Open_Args  args;
	args.flags	= FT_OPEN_STREAM;
	args.stream	= &(fontDataStream->d_streamRec);

	if (FT_Open_Face(d_impldat->library, &args, 0, &d_impldat->fontFace) == 0)
	{

		// check that default Unicode character map is available
		if (d_impldat->fontFace->charmap != NULL)	
		{
			try
			{
				d_name = name;
				d_ptSize = size;
				d_antiAliase = antiAliase;

				// reprepare font datas.
				resetFontFaces();

				// prepare underline image
				createUnderlineImage();

				return;
			}
			catch(...)
			{
				FT_Done_Face(d_impldat->fontFace);

				// re-throw
				throw;
			}

		}
		// missing Unicode character map
		else
		{
			FT_Done_Face(d_impldat->fontFace);

			errMsg = (utf8*)"Font_FreeType::constructor_impl - The source font '" + fontname +"' does not have a Unicode charmap, and cannot be used.";
		}

	}
	// failed to create face (a problem with the font file?)
	else
	{
		errMsg = (utf8*)"Font_FreeType::constructor_impl - An error occurred while trying to create a FreeType face from source font '" + fontname + "'.";
	}

	throw GenericException(errMsg);
}


/*************************************************************************
    Unload data associated with the font (font is then useless.
    this is intended for cleanup).
*************************************************************************/
void Font_FreeType::unload(void)
{
	memset(d_cp_map, 0, sizeof(glyphDat)*CODEPOINT_MAP_SIZE);

    // cleanup FreeType face if this is a FreeType based font.
	if(d_impldat)
	{
		FT_Done_Face(d_impldat->fontFace);
    }
}

/*************************************************************************
	Recreate font glyph data map and imageset.
*************************************************************************/
void Font_FreeType::resetFontFaces(void)
{
	if(d_impldat->fontFace == 0) return;

	uint horzDpi = System::getSingleton().getRenderer()->getHorzScreenDPI();
	uint vertDpi = System::getSingleton().getRenderer()->getVertScreenDPI();

	if (d_autoScale)
	{
		horzDpi = (uint)(((float)horzDpi) * d_horzScaling);
		vertDpi = (uint)(((float)vertDpi) * d_vertScaling);
	}

	if (FT_Set_Char_Size(d_impldat->fontFace, 0, d_ptSize * 64, horzDpi, vertDpi) != 0)
	{
		// failed to set size for font
		throw GenericException((utf8*)"Font_FreeType::createFontFromFT_Face - An error occurred while creating a source font with the requested size.");
	}

	// clear font faces and memory
	clearAllFontFaces();

	//insert default data to set font size data
	prepareString((utf8*)DefaultPrepareString);

	d_lineHeight = (float)d_maxGlyphHeight;

	// calculate spacing and base-line
    d_max_bearingY = ((float)d_impldat->fontFace->ascender / (float)d_impldat->fontFace->units_per_EM) * (float)d_impldat->fontFace->size->metrics.y_ppem;
	d_lineSpacing = ((float)d_impldat->fontFace->height / (float)d_impldat->fontFace->units_per_EM) * (float)d_impldat->fontFace->size->metrics.y_ppem;
}

/*************************************************************************
	Paint the code into the current glyphImageset, This also defines an Image for the glyph in 
	the Imageset for this font, and creates an entry in the code point to Image map.
*************************************************************************/
void Font_FreeType::createFontGlyph(const utf32 code)
{
	//Check is control code
	if((code &0XFFFF0000) != 0) return;

	//Load character glyph data from freetype 
	if (FT_Load_Char(d_impldat->fontFace, code, 
		FT_LOAD_RENDER|FT_LOAD_NO_AUTOHINT| 
		(d_antiAliase?FT_LOAD_TARGET_NORMAL:(FT_LOAD_MONOCHROME|FT_LOAD_TARGET_MONO))))
	{
		return;
	}

	// update font maxheight
	if ((uint)d_impldat->fontFace->glyph->bitmap.rows > d_maxGlyphHeight)
	{
		d_maxGlyphHeight = d_impldat->fontFace->glyph->bitmap.rows;
	}

	// font glyph width
	uint nWidth = d_impldat->fontFace->glyph->bitmap.width;// + InterGlyphPadSpace;

	if(!d_currentGlyphImageset || d_ptNext_Y+d_maxGlyphHeight>d_textureSize)// create new font imageset
	{
		if(d_ptNext_Y+d_maxGlyphHeight>d_textureSize)
		{
			char szTemp[260] = {0};
			_snprintf(szTemp, 260, "*** BugDetected[FT](%d+%d>%d) ***", 
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
			d_ptNext_Y += d_maxGlyphHeight;
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
	rect.d_bottom	= (float)(d_ptNext_Y + d_maxGlyphHeight);

	Point offset;
	offset.d_x		=  (float)(d_impldat->fontFace->glyph->metrics.horiBearingX >> 6);
	offset.d_y		= -(float)(d_impldat->fontFace->glyph->metrics.horiBearingY >> 6);

	char imageName[16];
	_snprintf(imageName, 16, "%X", code);
	d_currentGlyphImageset->d_imageset->defineImage(imageName, rect, offset);

	//Next Point
	d_ptNext_X += nWidth;

	// create entry in code-point to Image map
	glyphDat	dat;
	dat.d_image = &(d_currentGlyphImageset->d_imageset->getImage(imageName));
	dat.d_imageBorder = 0;
	dat.d_horz_advance = d_impldat->fontFace->glyph->advance.x >> 6;

	d_cp_map[code] = dat;

	//set dirty flag
	d_currentGlyphImageset->d_dirty = true;
}

/*************************************************************************
	Paint the code into the current glyphImageset, This also defines an Image for the glyph in 
	the Imageset for this font, and creates an entry in the code point to Image map.
	[With extern border]
*************************************************************************/
void Font_FreeType::createFontGlyphBorder(const utf32 code)
{
	//Check is control code
	if((code &0XFFFF0000) != 0) return;

	//Check font glyph
	if(!isCodepointAvailable(code)) return;

	//Load character glyph data from freetype 
	if (FT_Load_Char(d_impldat->fontFace, code, 
		FT_LOAD_RENDER|FT_LOAD_NO_AUTOHINT| 
		(d_antiAliase?FT_LOAD_TARGET_NORMAL:(FT_LOAD_MONOCHROME|FT_LOAD_TARGET_MONO))))
	{
		return;
	}

	// font glyph size
	int nWidth = (int)d_impldat->fontFace->glyph->bitmap.width+2;
	int nHeight = (int)d_impldat->fontFace->glyph->bitmap.rows+2;

	int nMaxGlyphHeight = d_maxGlyphHeight+2;

	if(!d_currentBorderGlyphImageset || d_ptBorderNext_Y+nMaxGlyphHeight>d_textureSize)// create new font imageset
	{
		if(d_ptBorderNext_Y+nMaxGlyphHeight>d_textureSize)
		{
			char szTemp[260] = {0};
			_snprintf(szTemp, 260, "*** BugDetected[FT](%d+%d>%d) ***", 
				d_ptNext_Y, nMaxGlyphHeight, d_textureSize);
			Logger::getSingleton().logEvent((utf8*)szTemp);
		}
		d_currentBorderGlyphImageset = createFontImageset(true);
	}
	else
	{
		// Check is enough space to paint new character glyph in this line
		if( d_ptBorderNext_X + nWidth > d_textureSize ) // new line
		{
			d_ptBorderNext_Y += nMaxGlyphHeight;
			d_ptBorderNext_X = 0;

			// check is enough space in this imageset.
			if(d_ptBorderNext_Y + nMaxGlyphHeight > d_textureSize)
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
	rect.d_bottom	= (float)(d_ptBorderNext_Y + nMaxGlyphHeight);

	Point offset;
	offset.d_x		=  (float)(d_impldat->fontFace->glyph->metrics.horiBearingX >> 6);
	offset.d_y		= -(float)(d_impldat->fontFace->glyph->metrics.horiBearingY >> 6);

	char imageName[16];
	_snprintf(imageName, 16, "%X", code);
	d_currentBorderGlyphImageset->d_imageset->defineImage(imageName, rect, offset);

	//Next Point
	d_ptBorderNext_X += nWidth;

	//Set border image
	d_cp_map[code].d_imageBorder = &(d_currentBorderGlyphImageset->d_imageset->getImage(imageName));

	//set dirty flag
	d_currentBorderGlyphImageset->d_dirty = true;
}

/*************************************************************************
	Copy the FreeType glyph bitmap into the given memory buffer
*************************************************************************/
void Font_FreeType::drawGlyphToBuffer(utf32 /*code*/, uint16* buffer, uint buf_width, bool drawBorder)
{
	FT_Bitmap* glyph_bitmap = &d_impldat->fontFace->glyph->bitmap;

	//clear memory
	if(drawBorder)
	{
		for(register int i=0; i<glyph_bitmap->rows+2; i++)
		{
			memset(buffer+buf_width*i, 0, sizeof(uint16)*(glyph_bitmap->width+2));
		}
	}

	for (int i = 0; i < glyph_bitmap->rows; ++i)
	{
		for (int j = 0; j < glyph_bitmap->width; ++j)
		{
			switch (glyph_bitmap->pixel_mode)
			{
			case FT_PIXEL_MODE_GRAY:
				{
					uchar col = glyph_bitmap->buffer[(i * glyph_bitmap->pitch) + j];
					if(drawBorder) //draw 3X3 block
					{
						for(int ii=0; ii<3; ii++)
						{
							uchar* bytebuff = (unsigned char*)(buffer + ii*buf_width + j);
							for(int jj=0; jj<3; jj++)
							{
								*bytebuff++ = 0xFF;
								int colAdd = (int)(*bytebuff) + (int)col;
								if(colAdd > 0xFF) colAdd = 0xFF;
								*bytebuff++ = (uchar)colAdd;
							}
						}
					}
					else
					{
						uchar* bytebuff = (unsigned char*)(buffer + j);
						*bytebuff++ = 0xFF;
						*bytebuff = col;
					}
				}
				break;

			case FT_PIXEL_MODE_MONO:
				{
					bool isDot = ((glyph_bitmap->buffer[(i * glyph_bitmap->pitch) + j / 8] << (j % 8)) & 0x80) != 0;;

					if(drawBorder) //draw 3X3 block
					{
						for(int ii=0; ii<3; ii++)
						{
							uchar* bytebuff = (unsigned char*)(buffer + ii*buf_width + j);

							for(int jj=0; jj<3; jj++)
							{
								*bytebuff++ = 0xFF;
								if(isDot) *bytebuff++ = 0xFF;
								else bytebuff++;
							}
						}
					}
					else
					{
						buffer[j] = isDot ? 0xFFFF : 0x00FF;
					}
				}
				break;

			default:
				throw InvalidRequestException((utf8*)"Font::drawGlyphToBuffer - The glyph could not be drawn because the pixel mode is unsupported.");
				break;
			}

		}

		buffer += buf_width;
	}

}

} // End of  CEGUI namespace section
