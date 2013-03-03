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
#ifndef _CEGUIFont_DataStream_h_
#define _CEGUIFont_DataStream_h_

#include "CEGUIBase.h"
#include "CEGUISystem.h"
#include "CEGUIDataContainer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// Start of CEGUI namespace section
namespace CEGUI
{

//一个ttf子文件
struct FontDataStream_TTFFile
{
	DataStream*		d_fontDataStream;
	size_t			d_begin;
	size_t			d_end;
};

typedef std::vector< FontDataStream_TTFFile > TTFFileBuf;

struct FontDataStream
{
	TTFFileBuf		d_fileBuf;
	FT_StreamRec	d_streamRec;
	bool			d_encrypted;
};


}

#endif