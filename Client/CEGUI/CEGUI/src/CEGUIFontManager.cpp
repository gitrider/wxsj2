/************************************************************************
filename: 	CEGUIFontManager.cpp
created:	21/2/2004
author:		Paul D Turner

purpose:	Implements the FontManager class
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
#include "CEGUIFontManager.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIFontBase.h"
#include "CEGUIFont_FreeType.h"
#include "CEGUIFont_Bitmap.h"
#include "CEGUIPixmapFont.h"
#include "CEGUIFontManager_implData.h"
#include "CEGUIFont_implData.h"
#include "CEGUIFont_DataStream.h"
#include "CEGUISystem.h"

#include "CEGUIImageset.h"
#include "CEGUIImage.h"
#include "CEGUITexture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>

// Start of CEGUI namespace section
namespace CEGUI
{
	/*************************************************************************
	Static Data Definitions
	*************************************************************************/
	// singleton instance pointer
	template<> FontManager* Singleton<FontManager>::ms_Singleton	= NULL;

	/********************************************************
	Stream io funcion for FreeType
	********************************************************/
	FT_CALLBACK_DEF( unsigned long )
		ft_stream_io( FT_Stream       stream,
		unsigned long   offset,
		unsigned char*  buffer,
		unsigned long   count )
	{
		FontDataStream* pStream = (FontDataStream*)(stream->descriptor.pointer);
		if(!pStream || buffer==0 || count==0) return 0;

		const unsigned int STACK_BUF_SIZE = 1024*2;
		char pStatckBuf[STACK_BUF_SIZE] = {0};
		char* pTempBuf = pStatckBuf;
		if(count > STACK_BUF_SIZE)
		{
			pTempBuf = new char[count];
			memset(pTempBuf, 0, count);
		}

		unsigned long totalReadSize = 0;
		for(int i=0; i<(int)pStream->d_fileBuf.size(); i++)
		{
			FontDataStream_TTFFile& thisFile = pStream->d_fileBuf[i];
			if(offset < thisFile.d_begin || offset>= thisFile.d_end) continue;

			//begin read
			unsigned long remainCount = count;
			unsigned long currentOffset = offset;
			FontDataStream_TTFFile* currentFile = &thisFile;
			do{
				unsigned long readSize = remainCount;
				if(currentOffset+readSize > currentFile->d_end)
				{
					readSize = currentFile->d_end - currentOffset;
				}

				//seek and read to temp memory
				currentFile->d_fontDataStream->seek(currentOffset-currentFile->d_begin);
				totalReadSize += currentFile->d_fontDataStream->read(pTempBuf+totalReadSize, readSize);

				// check is it end?
				remainCount -= readSize;
				if(remainCount <=0 || i==(int)(pStream->d_fileBuf.size())-1) break;

				// next file
				currentFile = &(pStream->d_fileBuf[++i]);
				currentOffset = currentFile->d_begin;
			}while(true);

			break;
		}

		if(pStream->d_encrypted)
		{
			for(size_t i=0; i<totalReadSize; i++)
			{
				pTempBuf[i] ^= 0xFE;
			}
		}

		memcpy(buffer, pTempBuf, totalReadSize);
		if(count > STACK_BUF_SIZE)
		{
			delete[] pTempBuf;
		}

		return totalReadSize;
	}

	/********************************************************
	Stream close funcion for FreeType
	********************************************************/
	FT_CALLBACK_DEF( void )
		ft_stream_close( FT_Stream  stream )
	{
		FontDataStream* pStream = (FontDataStream*)(stream->descriptor.pointer);
		if(!pStream) return;

		for(int i=0; i<(int)pStream->d_fileBuf.size(); i++)
		{
			FontDataStream_TTFFile& thisFile = pStream->d_fileBuf[i];
			System::getSingleton().getResourceProvider()->closeDataStream(thisFile.d_fontDataStream);
		}

		stream->descriptor.pointer = 0;
		stream->size               = 0;
		stream->base               = 0;
	}

	int	_convertStringToVector(const char* szStrintgSource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty)
	{
		vRet.clear();

		//------------------------------------------------------------
		//合法性
		if(!szStrintgSource || szStrintgSource[0] == '\0') return 0;

		std::string strSrc = szStrintgSource;

		//------------------------------------------------------------
		//查找第一个分割点
		std::string::size_type nLeft = 0;
		std::string::size_type nRight;
		if(bOneOfKey)
		{
			nRight = strSrc.find_first_of(szKey);
		}
		else
		{
			nRight = strSrc.find(szKey);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
		while(1)
		{
			std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
			if(strItem.length() > 0 || !bIgnoreEmpty)
			{
				vRet.push_back(strItem);
			}

			if(nRight == strSrc.length())
			{
				break;
			}

			nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));

			if(bOneOfKey)
			{
				std::string strTemp = strSrc.substr(nLeft);
				nRight = strTemp.find_first_of(szKey);
				if(nRight != std::string::npos) nRight += nLeft;
			}
			else
			{
				nRight = strSrc.find(szKey, nLeft);
			}

			if(nRight == std::string::npos)
			{
				nRight = strSrc.length();
			}
		}

		return (int)vRet.size();
	}

	/*************************************************************************
	constructor
	*************************************************************************/
	FontManager::FontManager(void)
	{
		d_implData = new FontManagerImplData;

		if (FT_Init_FreeType(&d_implData->d_ftlib))
		{
			throw GenericException((utf8*)"FontManager::FontManager - Failed to initialise the FreeType library.");
		}

		Logger::getSingleton().logEvent((utf8*)"CEGUI::FontManager singleton created.");

		d_timeOld = 0.0f;
		d_totalLife = 0.8f;	//blink one cycle need 0.8 sec.
		d_minAlpha = 0.35f;	//alpha smallest value.

		d_curBlinkAlpha = 1.0f;
	}


	/*************************************************************************
	Destructor
	*************************************************************************/
	FontManager::~FontManager(void)
	{
		Logger::getSingleton().logEvent((utf8*)"---- Begining cleanup of Font system ----");
		destroyAllFonts();

		FT_Done_FreeType(d_implData->d_ftlib);
		delete d_implData;

		FontDataStreamRegistry::iterator itFontData;
		for(itFontData=d_fontData.begin(); itFontData!=d_fontData.end(); itFontData++)
		{
			FontDataStream* fontDataStream = itFontData->second;

			//release
			delete fontDataStream;
			itFontData->second = 0;
		}
		d_fontData.clear();

		Logger::getSingleton().logEvent((utf8*)"CEGUI::FontManager singleton destroyed.");
	}


	/*************************************************************************
	Create a font from a definition file
	*************************************************************************/
	FontBase* FontManager::createFont(FontType type, const String& filename, const String& resourceGroup)
	{
		Logger::getSingleton().logEvent((utf8*)"Attempting to create Font from the information specified in file '" + filename + "'.");

		FontBase* temp = 0;
		if(FreeType == type)
		{
			temp = new Font_FreeType(filename, resourceGroup, new Font_FreeType::FontImplData(d_implData->d_ftlib));
		}
		else if (PixelMap == type)
		{
			temp = new PixmapFont(filename, resourceGroup);
		}
		else
		{
			temp = new Font_Bitmap(filename, resourceGroup);
		}

		temp->load(filename, resourceGroup);

		String name = temp->getName();

		if (isFontPresent(name))
		{
			delete temp;

			throw AlreadyExistsException((utf8*)"FontManager::createFont - A font named '" + name + "' already exists.");
		}

		d_fonts[name] = temp;

		// if this was the first font created, set it as the default font
		if (d_fonts.size() == 1)
		{
			System::getSingleton().setDefaultFont(temp);
		}

		return temp; 
	}

	/*************************************************************************
	Destroy the named font
	*************************************************************************/
	void FontManager::destroyFont(const String& name)
	{
		FontRegistry::iterator	pos = d_fonts.find(name);

		if (pos != d_fonts.end())
		{
			String tmpName(name);

			delete pos->second;
			d_fonts.erase(pos);

			Logger::getSingleton().logEvent((utf8*)"Font '" + tmpName +"' has been destroyed.");
		}

	}


	/*************************************************************************
	Destroys the given Font object
	*************************************************************************/
	void FontManager::destroyFont(FontBase* font)
	{
		if (font != NULL)
		{
			destroyFont(font->getName());
		}

	}


	/*************************************************************************
	Destroys all Font objects registered in the system
	*************************************************************************/
	void FontManager::destroyAllFonts(void)
	{
		while (!d_fonts.empty())
		{
			destroyFont(d_fonts.begin()->first);
		}

	}


	/*************************************************************************
	Check to see if a font is available
	*************************************************************************/
	bool FontManager::isFontPresent(const String& name) const
	{
		return (d_fonts.find(name) != d_fonts.end());
	}


	/*************************************************************************
	Return a pointer to the named font
	*************************************************************************/
	FontBase* FontManager::getFont(const String& name) const
	{
		FontRegistry::const_iterator pos = d_fonts.find(name);

		if (pos == d_fonts.end())
		{
			throw UnknownObjectException("FontManager::getFont - A Font object with the specified name '" + name +"' does not exist within the system");
		}

		return pos->second;
	}


	/*************************************************************************
	Notify the FontManager of the current (usually new) display
	resolution.
	*************************************************************************/
	void FontManager::notifyScreenResolution(const Size& size)
	{
		// notify all attached Font objects of the change in resolution
		FontRegistry::iterator pos = d_fonts.begin(), end = d_fonts.end();

		for (; pos != end; ++pos)
		{
			pos->second->notifyScreenResolution(size);
		}

	}

	void FontManager::prepareFontTexture(void)
	{
		// notify all attached Font objects to prepare font texture.
		FontRegistry::iterator pos = d_fonts.begin(), end = d_fonts.end();

		for (; pos != end; ++pos)
		{
			pos->second->prepareFontTexture();
		}
	}

	/*************************************************************************
	Queue the blink txt to be drawn and create a runtime blink 
	in runtime blink list.
	*************************************************************************/
	void FontManager::addRuntimeBlink(const Image* img, unsigned long& quadID, const Vector3& position, const Size& size, const Rect& clip_rect, const ColourRect& col_rect)
	{
		FontBlinkRuntime runtime;

		runtime.d_texture = img->getImageset()->getTexture();
		runtime.d_quadID = quadID;
		runtime.d_colorRect = col_rect;
		runtime.d_originalAlpha = col_rect.d_top_left.getAlpha(); 

		Rect source_rect = img->getSourceTextureArea();
		Rect dest_rect = Rect(position.d_x, position.d_y, position.d_x + size.d_width, position.d_y + size.d_height);

		Size sz = img->getSize();
		Point pt = img->getOffsets();

		// get the rect area that we will actually draw to (i.e. perform clipping)
		Rect final_rect(dest_rect.getIntersection(clip_rect));

		Texture* pTex = img->getImageset()->getTexture();

		float x_scale = 1.0f / (float)pTex->getWidth();
		float y_scale = 1.0f / (float)pTex->getHeight();

		float tex_per_pix_x = source_rect.getWidth() / dest_rect.getWidth();
		float tex_per_pix_y = source_rect.getHeight() / dest_rect.getHeight();

		// calculate final, clipped, texture co-ordinates
		Rect  tex_rect((source_rect.d_left + ((final_rect.d_left - dest_rect.d_left) * tex_per_pix_x)) * x_scale,
			(source_rect.d_top + ((final_rect.d_top - dest_rect.d_top) * tex_per_pix_y)) * y_scale,
			(source_rect.d_right + ((final_rect.d_right - dest_rect.d_right) * tex_per_pix_x)) * x_scale,
			(source_rect.d_bottom + ((final_rect.d_bottom - dest_rect.d_bottom) * tex_per_pix_y)) * y_scale);

		runtime.d_textureRect = tex_rect;

		d_runtimeBlink.push_back(runtime);
	}
	/*************************************************************************
	Clears all runtime blink.
	*************************************************************************/
	void FontManager::clearRuntimeBlinkList(void)
	{
		d_runtimeBlink.clear();
	}
	/*************************************************************************
	Cause all runtime blink in render queue to update itself.
	*************************************************************************/
	void FontManager::updateRuntimeBlinkList(void)
	{
		FontBlinkRegistry::iterator it;
		for(it = d_runtimeBlink.begin(); it != d_runtimeBlink.end(); ++it)
		{
			FontBlinkRuntime& runtime = *it;
			runtime.d_colorRect.setAlpha(getBlinkAlphaValue());

			System::getSingleton().getRenderer()->adjustQuad(
				runtime.d_quadID, runtime.d_texture, runtime.d_textureRect, runtime.d_colorRect);
		}
	}

	/*************************************************************************
	Calculate current alpha for blink. 1.0f -> 0.0f -> 1.0f ...
	*************************************************************************/
	void FontManager::calculateBlinkAlphaValue(void)
	{
		float nowTime = System::getSingleton().getCurTimeElapsed();
		float step = 0.0f;

		static bool dir = true;

		//char stmp[64] = {0};

		step = (nowTime - d_timeOld)/d_totalLife;
		d_timeOld = nowTime;

		d_curBlinkAlpha += (dir)?step:-step;
		if(d_curBlinkAlpha > 1.0f)
		{
			d_curBlinkAlpha = 1.0f;
			dir = false;
		}

		if(d_curBlinkAlpha < d_minAlpha)
		{
			d_curBlinkAlpha = d_minAlpha;
			dir = true;
		}

		//_snprintf(stmp,63, "blink Alpha:%f\n", d_curBlinkAlpha);
		//OutputDebugString(stmp);
	}

	FontManager& FontManager::getSingleton(void)
	{
		return Singleton<FontManager>::getSingleton();
	}


	FontManager* FontManager::getSingletonPtr(void)
	{
		return Singleton<FontManager>::getSingletonPtr();
	}

	/*************************************************************************
	Get font ttf data
	*************************************************************************/
	FontDataStream* FontManager::getFontDataStream(const String& fontname, const String& resourceGroup, bool encrypted)
	{
		char szFontLowCase[260];
		strncpy(szFontLowCase, fontname.c_str(), 260);

		// switch low case.
		_strlwr(szFontLowCase);

		//aready load
		FontDataStreamRegistry::iterator itFind = d_fontData.find(szFontLowCase);
		if(itFind != d_fontData.end())
		{
			return itFind->second;
		}

		//load now!
		FontDataStream* pFontDataStream = new FontDataStream;
		d_fontData.insert(std::make_pair(String(szFontLowCase), pFontDataStream));

		pFontDataStream->d_encrypted = encrypted;

		//is multi ttf file?
		std::vector< std::string > vFileNameBuf;
		_convertStringToVector(szFontLowCase, vFileNameBuf, "|", false, true);

		size_t sizeNow = 0;
		for(int i=0; i<(int)vFileNameBuf.size(); i++)
		{
			// file stream
			DataStream* pStream = 
				System::getSingleton().getResourceProvider()->openDataStream(vFileNameBuf[i], resourceGroup);
			assert(pStream);

			FontDataStream_TTFFile ttf;
			ttf.d_fontDataStream = pStream;
			ttf.d_begin = sizeNow;
			ttf.d_end = sizeNow + pStream->size();
			sizeNow += pStream->size();

			pFontDataStream->d_fileBuf.push_back(ttf);
		}

		//freetype stream rec
		FT_StreamRec& ftStreamRec = pFontDataStream->d_streamRec;
		memset(&ftStreamRec, 0, sizeof(FT_StreamRec));

		ftStreamRec.descriptor.pointer	= pFontDataStream;
		ftStreamRec.size				= sizeNow;
		ftStreamRec.pos					= 0;
		ftStreamRec.read				= ft_stream_io;
		ftStreamRec.close				= ft_stream_close;

		return pFontDataStream;
	}

	/*************************************************************************
	Return a FontManager::FontIterator object to iterate over the
	available Font objects.
	*************************************************************************/
	FontManager::FontIterator FontManager::getIterator(void) const
	{
		return FontIterator(d_fonts.begin(), d_fonts.end());
	}

} // End of  CEGUI namespace section
