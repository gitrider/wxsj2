#include "AxBmpFont.h"

#include <assert.h>

/** 目前支持的code区域
*/
CAxBitmapFont::CODEAREA_DEFINE g_theCodeArea[] = 
{
	{0x0000, "Basic Latin",						0x0020, 0x007E },	//基本拉丁语		排除0x0000-0x001F之间的控制字符
	{0x0080, "Lating-1",						0x00A1, 0x00FF },	//拉丁语-1			排除0x0080-0x00A0之间的控制字符
	{0x0100, "Lating Extended-A",				0x0100, 0x017F },	//拉丁语扩充-A	
	{0x0180, "Lating Extended-B",				0x0180, 0x024F },	//拉丁语扩充-B	
	{0x0250, "IPA Extensions",					0x0250,	0x02AF },	//国际音标扩充
	{0x02B0, "Spacing Modifier Letters",		0x02B0, 0x02FF },	//进格的修饰字符
	{0x0370, "Greek and Coptic",				0x0391, 0x03FF },	//希腊语和科普特语	排除0x0370-0x0390之间的不常用字符
	{0x0400, "Cyrillic",						0x0400, 0x0451 },	//西里尔字母		排除0x0452-0x04FF之间的不常用字符
	{0x1E00, "Lating Extended Additional",		0x1E00, 0x1EF9 },	//拉丁语附加扩充	排除0x1EFA-0x1EFF之间的未定义字符
	{0x2000, "General Punctuation",				0x2010, 0x205E },	//广义标点			排除0x2000-0x200F,0x205F-0x206F之间的控制字符
	{0x2100, "Letterlike Symbols",				0x2100, 0x214F },	//类似字母的符号	
	{0x2150, "Number Forms",					0x2153, 0x2184 },	//数字形式			排除0x2150-0x2152,0x2185-0x218F之间的未定义字符
	{0x2190, "Arrows",							0x2190, 0x21FF },	//箭头
	{0x2200, "Mathematical Operators",			0x2200,	0x22FF },	//数学运算符
	{0x2300, "Miscellaneous Technical",			0x2300, 0x231F },	//零杂技术符号		排除0x2320-0x23FF之间的不常用字符
	{0x2460, "Enclosed Alphanumerics",			0x2460, 0x24FF },	//带括号的字母数字
	{0x2500, "Box Drawing",						0x2500, 0x257F },	//制表符
	{0x2580, "Block Elements",					0x2580, 0x259F },	//方块元素
	{0x25A0, "Geometrical Shapes",				0x25A0, 0x25FF },	//几何图形符
	{0x2600, "Miscellaneous Symbols",			0x2600, 0x26B2 },	//其他符号			排除0x26B3-0x26FF之间的未定义字符
	{0x3000, "CJK Symbols and Punctuation",		0x3000, 0x303F },	//CJK符号和标点
	{0x3040, "Hiragana",						0x3040, 0x309F },	//平假名
	{0x30A0, "Katakana",						0x30A0, 0x30FF },	//片假名
	{0x3100, "Bopomofo",						0x3100, 0x312F },	//注音
	{0x3220, "Enclosed CJK Letters and Months",	0x3220, 0x32FF },	//带括号的CJK字母和月份
	{0x3300, "CJK Compatibility",				0x3380, 0x33DF },	//CJK兼容字符		排除0x3300-0x337F,0x33E0-0x33FF之间的控制字符
	{0x4E00, "Unified CJK Ideographs",			0x4E00, 0x9FBF },	//CJK统一汉字		
	{0xF900, "CJK Compatibility Ideographs",	0xF900, 0xFA2D },	//CJK兼容汉字		排除0xFA2E-0xFAFF之间的不常用字符
	{0xFE30, "CJK Compatibility Forms",			0xFE30, 0xFE4F },	//CJK兼容形式
	{0xFE50, "Small Form Variants",				0xFE50, 0xFE6F },	//小写变体
	{0xFF00, "Halfwidth and Fullwidth Forms",	0xFF00, 0xFFEF },	//半角及全角字符
};

int g_sustainingAreaCounts = sizeof(g_theCodeArea)/sizeof(CAxBitmapFont::CODEAREA_DEFINE);

//---------------------------------------------------------------------------------
CAxBitmapFont::CAxBitmapFont()
	: m_fontSize(0)
	, m_pBitMemory(0)
	, m_nBitMemoryBytes(0)
#ifdef AX_BMPFONT_EDIT
	, m_bDirty(false)
	, m_nFontHeight(0)
	, m_hTrueTypeFont(0)
#endif
{
	memset(m_codeTable, 0, sizeof(CODE_TABLE));

	//缺省为12点阵字库
	m_fontSize=12;
}

//---------------------------------------------------------------------------------
CAxBitmapFont::~CAxBitmapFont()
{
	if(m_pBitMemory)
	{
		delete[] m_pBitMemory; m_pBitMemory=0;
	}
#ifdef AX_BMPFONT_EDIT
	if(m_hTrueTypeFont)
	{
		::DeleteObject(m_hTrueTypeFont);
		m_hTrueTypeFont = 0;
	}
#endif
}

//---------------------------------------------------------------------------------
void CAxBitmapFont::getSustainingCodeArea(CODEAREA_DEFINE_BUF& allCodeArea) const
{
	allCodeArea.clear();

	for(int i=0; i<g_sustainingAreaCounts; i++)
	{
		allCodeArea.push_back(g_theCodeArea+i);
	}
}

#ifdef AX_BMPFONT_EDIT

//---------------------------------------------------------------------------------
void CAxBitmapFont::setTrueType(const char* szFontName, int fontHeight)
{
	m_strFontName = szFontName;
	m_nFontHeight = fontHeight;

	if(m_hTrueTypeFont)
	{
		::DeleteObject(m_hTrueTypeFont);
	}

	m_hTrueTypeFont = ::CreateFont(-fontHeight,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		szFontName);

	//置脏标记
	m_bDirty = true;
}

//---------------------------------------------------------------------------------
void CAxBitmapFont::setFontSize(FONT_SIZE size)
{
	switch(size)
	{
	case FS_12:
		m_fontSize = 12;
		break;

	case FS_14:
		m_fontSize = 14;
		break;

	case FS_16:
		m_fontSize = 16;
		break;

	default:
		assert(false && "Not support yet!");
		return;
	}

	//置脏标记
	m_bDirty = true;
}

//---------------------------------------------------------------------------------
std::pair< const char*, int > CAxBitmapFont::getTrueType(void) const 
{ 
	return std::make_pair(m_strFontName.c_str(), m_nFontHeight); 
} 

//---------------------------------------------------------------------------------
void CAxBitmapFont::insertCodeArea(wchar_t wAreaID)
{
	//是否已经加入
	CODE_AREA_BUF::iterator itFind = m_allCodeArea.find(wAreaID);
	if(itFind!=m_allCodeArea.end()) return;

	//是否是支持的区域
	const CODEAREA_DEFINE* pAreaDefine=0;
	for(int i=0; i<g_sustainingAreaCounts; i++)
	{
		if(g_theCodeArea[i].wID == wAreaID)
		{
			pAreaDefine = g_theCodeArea+i;
			break;
		}
	}
	//不支持的区域
	if(!pAreaDefine) return;

	//加入新的区域
	CODE_AREA newArea;
	newArea.wID = wAreaID;
	newArea.wCodeStart = pAreaDefine->wCodeStart;
	newArea.wCodeEnd = pAreaDefine->wCodeEnd;
	newArea.reserve = 0;
	
	m_allCodeArea.insert(std::make_pair(wAreaID, newArea));

	//置脏标记
	m_bDirty = true;
}

//---------------------------------------------------------------------------------
void CAxBitmapFont::removeCodeArea(wchar_t wAreaID)
{
	//是否已经加入
	CODE_AREA_BUF::iterator itFind = m_allCodeArea.find(wAreaID);
	if(itFind==m_allCodeArea.end()) return;

	m_allCodeArea.erase(itFind);

	//置脏标记
	m_bDirty = true;
}

//---------------------------------------------------------------------------------
void CAxBitmapFont::rebuild(void)
{
	//每一个字符需要的内存(bytes)
	int CHAR_BYTES = 0;
	switch(m_fontSize)
	{
	case 12: CHAR_BYTES = 12*12/8+2; break;
	case 14: CHAR_BYTES = 16*14/8+2; break;
	case 16: CHAR_BYTES = 16*16/8+2; break;
	default: assert(false); return;
	}

	//是否需要重新生成数据
	if(!m_bDirty) return;

	//重新计算需要的点阵区域数量
	int countsOfCode=0;

	//查看所有区域
	CODE_AREA_BUF::const_iterator it, end=m_allCodeArea.end();
	for(it=m_allCodeArea.begin(); it!=end; ++it)
	{
		const CODE_AREA& area = it->second;
		countsOfCode += area.wCodeEnd-area.wCodeStart+1;
	}

	//重新分配内存
	m_nBitMemoryBytes = countsOfCode*CHAR_BYTES;

	if(m_pBitMemory) delete[] m_pBitMemory;
	m_pBitMemory = new char[m_nBitMemoryBytes];
	ZeroMemory(m_pBitMemory, m_nBitMemoryBytes);
	ZeroMemory(m_codeTable, sizeof(CODE_TABLE));

	//生成临时的Bitmap
	const int canvasSize = 128;
	const int canvasPitch = 128*3;
	const char* pCanvasBitbuf = 0;

	BITMAPINFO bmpInfo; 
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize		= sizeof (BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth		= canvasSize; 
	bmpInfo.bmiHeader.biHeight		= -canvasSize;
	bmpInfo.bmiHeader.biPlanes		= 1;
	bmpInfo.bmiHeader.biBitCount	= 24;
	bmpInfo.bmiHeader.biSizeImage	= canvasPitch * canvasSize;

	HDC hScreenDC = GetDC(GetDesktopWindow());
	HBITMAP hCanvasBmp = CreateDIBSection(hScreenDC, &bmpInfo, DIB_RGB_COLORS, (VOID**)&(pCanvasBitbuf), 0, 0);
	if(hCanvasBmp==0)
	{
		//Create DIB error
		throw std::exception("Create dib error!");
	}

	//创建Canvas DC
	HDC hCanvasDC = ::CreateCompatibleDC(hScreenDC);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hCanvasDC, hCanvasBmp);
	HFONT hOldFont = (HFONT)::SelectObject(hCanvasDC, m_hTrueTypeFont);

	//释放临时dc
	ReleaseDC(GetDesktopWindow(), hScreenDC);	hScreenDC = 0;

	//设置dc参数
	::SetTextColor(hCanvasDC, RGB(255, 255, 255));
	::SetBkColor(hCanvasDC, RGB(0, 0, 0));

	//Canvas 大小
	RECT rect;
	::SetRect(&rect, 0, 0, canvasSize, canvasSize);

	//----------------
	//FILE* fp = fopen("d:\\out.txt", "w");
	//----------------

	wchar_t tempBuf[2] = {0};
	countsOfCode=0;
	for(it=m_allCodeArea.begin(); it!=end; ++it)
	{
		const CODE_AREA& area = it->second;

		for(wchar_t code=area.wCodeStart; code<=area.wCodeEnd; code++)
		{
			char* pCodeMemory = m_pBitMemory + (countsOfCode++)*CHAR_BYTES;
			m_codeTable[code].pBuf = pCodeMemory;

			//清空
			::FillRect(hCanvasDC, &rect, (HBRUSH)::GetStockObject(BLACK_BRUSH));

			tempBuf[0]=code;

			//记录字符宽度
			RECT rect;
			::SetRect(&rect, 0, 0, 128,128);
			::DrawTextW(hCanvasDC, tempBuf, 1, &rect, DT_LEFT|DT_TOP|DT_CALCRECT);
			*((unsigned short*)pCodeMemory) = (unsigned short)(rect.right-rect.left);
			pCodeMemory+=sizeof(unsigned short);

			//画字符
			::TextOutW(hCanvasDC, 0, 0, tempBuf, 1);

			//生成点阵数据
			if(12==m_fontSize)
			{
				rebuild_12pix(pCanvasBitbuf, canvasPitch, (unsigned char*)pCodeMemory);
			}
			else if(14==m_fontSize)
			{
				rebuild_14pix(pCanvasBitbuf, canvasPitch, (unsigned char*)pCodeMemory);
			}
			else if(16==m_fontSize)
			{
				rebuild_16pix(pCanvasBitbuf, canvasPitch, (unsigned char*)pCodeMemory);
			}
			//fprintf(fp, "\n");
		}
	}

	//fclose(fp);

	//释放DIB对象
	::SelectObject(hCanvasDC, hOldBmp);
	::SelectObject(hCanvasDC, hOldFont);
	::DeleteDC(hCanvasDC);
	//::DeleteObject(hCanvasBmp);

	m_bDirty = false;
}

//---------------------------------------------------------------------------------
void CAxBitmapFont::rebuild_12pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory)
{
	for(register int i=0; i<12; i++)
	{
		if(i&1) //1,3,5...
		{
			const char* pDIBBuf = pDIBBitBuf+nDIBPitch*i+12;

			unsigned char& byTemp = 
				*((unsigned char*)(pCodeMemory + (((unsigned int)(i-1))>>1)*3 + 2));

			for(register int j=0; j<8; j++)
			{
				bool isDot = *(pDIBBuf+j*3)!=0;

				if(isDot) 
				{
					byTemp |= ((unsigned char)1)<<(7-j);
					//fprintf(fp, "#");
				}
				else
				{
					//fprintf(fp, "~");
				}
			}
			//fprintf(fp, "\n");
		}
		else //2,4,6,...
		{
			const char* pDIBBuf = pDIBBitBuf+nDIBPitch*i;

			unsigned short& wTemp = 
				*((unsigned short*)(pCodeMemory + (((unsigned int)i)>>1)*3));

			for(register int j=0; j<12; j++)
			{
				bool isDot = *(pDIBBuf+j*3)!=0;
				if(isDot) 
				{
					wTemp |= ((unsigned short)1)<<(15-j);
					//fprintf(fp, "#");
				}
				else
				{
					//fprintf(fp, "~");
				}
			}
			//fprintf(fp, "\n");

			pDIBBuf = pDIBBitBuf+nDIBPitch*(i+1);
			for(register int j=0; j<4; j++)
			{
				bool isDot = *(pDIBBuf+j*3)!=0;
				if(isDot) 
				{
					wTemp |= ((unsigned short)1)<<(3-j);
					//fprintf(fp, "#");
				}
				else
				{
					//fprintf(fp, "~");
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
void CAxBitmapFont::rebuild_14pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory)
{
	for(int i=0; i<14; i++)
	{
		const char* pDIBBuf = pDIBBitBuf+nDIBPitch*i;
		unsigned short& wTemp = 
			*((unsigned short*)(pCodeMemory + i*2));
		wTemp=0;

		for(int j=0; j<14; j++)
		{
			bool isDot = *(pDIBBuf+j*3)!=0;
			if(isDot)
			{
				wTemp |= ((unsigned short)1)<<(15-j);
			}
		}
	}
}

//---------------------------------------------------------------------------------
void CAxBitmapFont::rebuild_16pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory)
{
	for(int i=0; i<16; i++)
	{
		const char* pDIBBuf = pDIBBitBuf+nDIBPitch*i;
		unsigned short& wTemp = 
			*((unsigned short*)(pCodeMemory + i*2));
		wTemp=0;

		for(int j=0; j<16; j++)
		{
			bool isDot = *(pDIBBuf+j*3)!=0;
			if(isDot)
			{
				wTemp |= ((unsigned short)1)<<(15-j);
			}
		}
	}
}

//---------------------------------------------------------------------------------
bool CAxBitmapFont::saveToFile(const char* szFileName)
{
	assert(szFileName);
	assert(!m_bDirty);

	FILE* fp = fopen(szFileName, "wb");
	if(!fp) return false;

	//write file head
	FILE_HEAD fileHead;
	ZeroMemory(&fileHead, sizeof(FILE_HEAD));

	fileHead.identity = FILE_HEAD_IDENTITY;
	fileHead.version = 1;
	fileHead.fontSize = m_fontSize;
	fileHead.areaCounts = (int)m_allCodeArea.size();
	fileHead.areaDefineOffset = sizeof(FILE_HEAD);
	fileHead.codeTableOffset = fileHead.areaDefineOffset+sizeof(CODE_AREA)*fileHead.areaCounts;
	fileHead.bitDataOffset = fileHead.codeTableOffset+sizeof(int)*CODE_COUNTS;
	fileHead.bitDataSize = m_nBitMemoryBytes;

	wchar_t wFontName[MAX_PATH]={0};
	::MultiByteToWideChar(0, 0, m_strFontName.c_str(), m_strFontName.size(), wFontName, MAX_PATH);
	wcsncpy(fileHead.szFontName, wFontName, sizeof(fileHead.szFontName)/sizeof(wchar_t));
	fileHead.nFontHeight = m_nFontHeight;

	fwrite(&fileHead, sizeof(FILE_HEAD), 1, fp);

	//write code area
	CODE_AREA_BUF::iterator itArea, areaEnd=m_allCodeArea.end(); 
	for(itArea=m_allCodeArea.begin(); itArea!=areaEnd; ++itArea)
	{
		CODE_AREA& area = itArea->second;
		fwrite(&area, sizeof(CODE_AREA), 1, fp);
	}

	//write code table
	{
		int* tempCodeTable = new int[CODE_COUNTS];
		ZeroMemory(tempCodeTable, sizeof(int)*CODE_COUNTS);

		for(int i=0; i<CODE_COUNTS; i++)
		{
			tempCodeTable[i] = (m_codeTable[i].pBuf==0)? 0xFFFFFFFF : (int)(INT_PTR)(m_codeTable[i].pBuf-m_pBitMemory);
		}

		fwrite(tempCodeTable, sizeof(int)*CODE_COUNTS, 1, fp);
		delete[] tempCodeTable; tempCodeTable=0;
	}

	//write bit memory
	fwrite(m_pBitMemory, m_nBitMemoryBytes, 1, fp);

	fclose(fp); fp=0;
	return true;
}

//---------------------------------------------------------------------------------
void CAxBitmapFont::debug(void)
{

}

#endif

//---------------------------------------------------------------------------------
unsigned short CAxBitmapFont::getFontWidth(wchar_t code) const
{
	const char* pCodeMemory = m_codeTable[code].pBuf;
	if(pCodeMemory==0)
	{
		return (unsigned short)m_fontSize;
	}

	return *((unsigned short*)pCodeMemory);
}

//---------------------------------------------------------------------------------
bool CAxBitmapFont::openFromMemory(const char* pMemory, int nMemoryBytesSize)
{
	assert(pMemory);

	const char* pData = pMemory;

	//read file head
	FILE_HEAD fileHead;
	memcpy(&fileHead, pData, sizeof(FILE_HEAD)); pData+=sizeof(FILE_HEAD);

	if(fileHead.identity!=FILE_HEAD_IDENTITY) 
	{
		return false;
	}

	//init data
	m_fontSize = fileHead.fontSize;
	m_nBitMemoryBytes = fileHead.bitDataSize;
#ifdef AX_BMPFONT_EDIT
	char szFontTemp[MAX_PATH]={0};
	::WideCharToMultiByte(0, 0, fileHead.szFontName, sizeof(fileHead.szFontName)/sizeof(wchar_t), szFontTemp, MAX_PATH, 0, 0);
	m_strFontName = szFontTemp;
	m_nFontHeight = fileHead.nFontHeight;
#endif

	//read code area
	m_allCodeArea.clear();
	for(int i=0; i<(int)fileHead.areaCounts; i++)
	{
		CODE_AREA area;
		memcpy(&area, pData, sizeof(CODE_AREA)); pData+=sizeof(CODE_AREA);
		m_allCodeArea.insert(std::make_pair(area.wID, area));
	}

	//allocate bit memory
	if(m_pBitMemory)
	{
		delete[] m_pBitMemory; 
	}
	m_pBitMemory = new char[m_nBitMemoryBytes];

	//read code table
	memset(m_codeTable, 0, sizeof(CODE_TABLE));
	memcpy(m_codeTable, pData, sizeof(CODE_TABLE)); pData+=sizeof(CODE_TABLE);
	for(int i=0; i<CODE_COUNTS; i++)
	{
		if((unsigned int)(m_codeTable[i].pBuf)==0xFFFFFFFF) m_codeTable[i].pBuf=0;
		else m_codeTable[i].pBuf = m_pBitMemory+(unsigned int)(m_codeTable[i].pBuf);
	}

	//read bit memory
	memcpy(m_pBitMemory, pData, m_nBitMemoryBytes);

#ifdef AX_BMPFONT_EDIT
	m_bDirty = false;
#endif

	return true;
}

//---------------------------------------------------------------------------------
bool CAxBitmapFont::openFromFile(const char* szFileName)
{
	assert(szFileName);

	FILE* fp = fopen(szFileName, "rb");
	if(!fp) return false;

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* pTempBuf = new char[fileSize];
	fread(pTempBuf, 1, fileSize, fp);
	fclose(fp); fp=0;

	bool ret = openFromMemory(pTempBuf, fileSize);
	delete[] pTempBuf; pTempBuf=0;

	return ret;
}

//---------------------------------------------------------------------------------
unsigned short CAxBitmapFont::fillFontMemory(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const
{
#ifdef AX_BMPFONT_EDIT
	assert(!m_bDirty);
#endif

	if(12==m_fontSize)
	{
		return fillFont_12pix(code, pTarBitMemory, pitchOfShort, withBorder);
	}
	else if(14==m_fontSize)
	{
		return fillFont_14pix(code, pTarBitMemory, pitchOfShort, withBorder);
	}
	else if(16==m_fontSize)
	{
		return fillFont_16pix(code, pTarBitMemory, pitchOfShort, withBorder);
	}

	return 0;
}

//---------------------------------------------------------------------------------
unsigned short CAxBitmapFont::fillFont_12pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const
{
	const char* pCodeMemory = m_codeTable[code].pBuf;
	if(pCodeMemory==0)
	{
		const unsigned short BoxTop[]		 = {0x00, 0x00, 0xFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};
		const unsigned short BoxMid[]		 = {0x00, 0x00, 0XFFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0XFFFF, 0x00, 0x00, 0x00};

		const unsigned short BoxTop_Border[] = {0x00, 0X00, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};
		const unsigned short BoxMid_Border[] = {0x00, 0X00, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};

		if(withBorder)
		{
			for(register int i=0; i<14; i++)
			{
				if(i==2 || i==3 || i==4 || i==9 || i==10 || i==11)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxTop_Border, sizeof(BoxTop_Border));
				}
				else if(i>4 && i<9)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxMid_Border, sizeof(BoxMid_Border));
				}
				else
				{
					memset(pTarBitMemory+i*pitchOfShort, 0, sizeof(BoxTop_Border));
				}
			}	
		}
		else
		{
			for(register int i=0; i<12; i++)
			{
				if(i==2 || i==9) 
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxTop, sizeof(BoxTop));
				}
				else if(i>2 && i<9)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxMid, sizeof(BoxMid));
				}
				else
				{
					memset(pTarBitMemory+i*pitchOfShort, 0, sizeof(BoxTop));
				}
			}		
		}

		return 12;
	}

	//clear memory
	if(withBorder)
	{
		for(register int i=0; i<14; i++)
		{
			memset(pTarBitMemory+pitchOfShort*i, 0, sizeof(unsigned short)*14);
		}
	}

	unsigned short width = *((unsigned short*)pCodeMemory);
	pCodeMemory += sizeof(unsigned short);

	for(register int i=0; i<12; i++)
	{
		if(i&1) //1,3,5...
		{
			register unsigned short* pTarBuf = pTarBitMemory+i*pitchOfShort+4;

			unsigned char byTemp = 
				*((unsigned char*)(pCodeMemory + (((unsigned int)(i-1))>>1)*3 + 2));

			for(register int j=0; j<8; j++)
			{
				bool isDot =  (byTemp & (((unsigned char)1)<<(7-j))) != 0;

				if(withBorder) //draw 3X3 block
				{
					if(isDot)
					{
						for(register int ii=0; ii<3; ii++)
						{
							register unsigned short* wbuff = pTarBuf+ii*pitchOfShort+j;
							*wbuff++ = 0xFFFF; *wbuff++ = 0xFFFF; *wbuff = 0xFFFF; 
						}
					}
				}
				else
				{
					*(pTarBuf++) = isDot?0xFFFF:0x0;
				}
			}
		}
		else //2,4,6...
		{
			register unsigned short* pTarBuf = pTarBitMemory+i*pitchOfShort;

			unsigned short wTemp = 
				*((unsigned short*)(pCodeMemory + (((unsigned int)i)>>1)*3));

			for(register int j=0; j<12; j++)
			{
				bool isDot = (wTemp & ((unsigned short)1)<<(15-j)) != 0;

				if(withBorder) //draw 3X3 block
				{
					if(isDot)
					{
						for(register int ii=0; ii<3; ii++)
						{
							register unsigned short* wbuff = pTarBuf+ii*pitchOfShort+j;
							*wbuff++ = 0xFFFF; *wbuff++ = 0xFFFF; *wbuff = 0xFFFF; 
						}
					}
				}
				else
				{
					*(pTarBuf++) = isDot?0xFFFF:0x00;
				}
			}
			pTarBuf = pTarBitMemory+(i+1)*pitchOfShort;
			for(register int j=0; j<4; j++)
			{
				bool isDot = (wTemp & ((unsigned short)1)<<(3-j)) != 0;
				if(withBorder) //draw 3X3 block
				{
					if(isDot)
					{
						for(register int ii=0; ii<3; ii++)
						{
							register unsigned short* wbuff = pTarBuf+ii*pitchOfShort+j;
							*wbuff++ = 0xFFFF; *wbuff++ = 0xFFFF; *wbuff = 0xFFFF; 
						}
					}
				}
				else
				{
					*(pTarBuf++) = isDot?0xFFFF:0x00;
				}
			}

		}
	}

	return width;
}

//---------------------------------------------------------------------------------
unsigned short CAxBitmapFont::fillFont_14pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const
{
	const char* pCodeMemory = m_codeTable[code].pBuf;
	if(pCodeMemory==0)
	{
		const unsigned short BoxTop[]		 = {0x00, 0x00, 0xFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};
		const unsigned short BoxMid[]		 = {0x00, 0x00, 0XFFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0XFFFF, 0x00, 0x00, 0x00};

		const unsigned short BoxTop_Border[] = {0x00, 0X00, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};
		const unsigned short BoxMid_Border[] = {0x00, 0X00, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};

		if(withBorder)
		{
			for(register int i=0; i<14; i++)
			{
				if(i==2 || i==3 || i==4 || i==9 || i==10 || i==11)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxTop_Border, sizeof(BoxTop_Border));
				}
				else if(i>4 && i<9)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxMid_Border, sizeof(BoxMid_Border));
				}
				else
				{
					memset(pTarBitMemory+i*pitchOfShort, 0, sizeof(BoxTop_Border));
				}
			}	
		}
		else
		{
			for(register int i=0; i<12; i++)
			{
				if(i==2 || i==9) 
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxTop, sizeof(BoxTop));
				}
				else if(i>2 && i<9)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxMid, sizeof(BoxMid));
				}
				else
				{
					memset(pTarBitMemory+i*pitchOfShort, 0, sizeof(BoxTop));
				}
			}		
		}

		return 14;
	}

	unsigned short width = *((unsigned short*)pCodeMemory);
	pCodeMemory += sizeof(unsigned short);

	//clear memory
	if(withBorder)
	{
		for(register int i=0; i<16; i++)
		{
			memset(pTarBitMemory+pitchOfShort*i, 0, sizeof(unsigned short)*16);
		}
	}

	for(int i=0; i<14; i++)
	{
		register unsigned short* pTarBuf = pTarBitMemory+i*pitchOfShort;
		unsigned short wTemp = 
			*((unsigned short*)(pCodeMemory + i*2));

		for(int j=0; j<14; j++)
		{
			bool isDot = (wTemp & ((unsigned short)1)<<(15-j)) != 0;

			if(withBorder) //draw 3X3 block
			{
				if(isDot)
				{
					for(register int ii=0; ii<3; ii++)
					{
						register unsigned short* wbuff = pTarBuf+ii*pitchOfShort+j;
						*wbuff++ = 0xFFFF; *wbuff++ = 0xFFFF; *wbuff = 0xFFFF; 
					}
				}
			}
			else
			{
				*(pTarBuf++) = isDot?0xFFFF:0x00;
			}
		}
	}

	return width;
}

//---------------------------------------------------------------------------------
unsigned short CAxBitmapFont::fillFont_16pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const
{
	const char* pCodeMemory = m_codeTable[code].pBuf;

	if(pCodeMemory==0)
	{
		const unsigned short BoxTop[]		 = {0x00, 0x00, 0xFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};
		const unsigned short BoxMid[]		 = {0x00, 0x00, 0XFFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0XFFFF, 0x00, 0x00, 0x00};

		const unsigned short BoxTop_Border[] = {0x00, 0X00, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};
		const unsigned short BoxMid_Border[] = {0x00, 0X00, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00, 0XFFFF, 0XFFFF, 0XFFFF, 0x00, 0x00, 0x00};

		if(withBorder)
		{
			for(register int i=0; i<16; i++)
			{
				if(i==2 || i==3 || i==4 || i==9 || i==10 || i==11)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxTop_Border, sizeof(BoxTop_Border));
				}
				else if(i>4 && i<9)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxMid_Border, sizeof(BoxMid_Border));
				}
				else
				{
					memset(pTarBitMemory+i*pitchOfShort, 0, sizeof(BoxTop_Border));
				}
			}	
		}
		else
		{
			for(register int i=0; i<16; i++)
			{
				if(i==2 || i==9) 
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxTop, sizeof(BoxTop));
				}
				else if(i>2 && i<9)
				{
					memcpy(pTarBitMemory+i*pitchOfShort, BoxMid, sizeof(BoxMid));
				}
				else
				{
					memset(pTarBitMemory+i*pitchOfShort, 0, sizeof(BoxTop));
				}
			}		
		}

		return 16;
	}

	unsigned short width = *((unsigned short*)pCodeMemory);
	pCodeMemory += sizeof(unsigned short);

	//clear memory
	if(withBorder)
	{
		for(register int i=0; i<18; i++)
		{
			memset(pTarBitMemory+pitchOfShort*i, 0, sizeof(unsigned short)*16);
		}
	}

	for(int i=0; i<16; i++)
	{
		register unsigned short* pTarBuf = pTarBitMemory+i*pitchOfShort;
		unsigned short wTemp = 
			*((unsigned short*)(pCodeMemory + i*2));

		for(int j=0; j<16; j++)
		{
			bool isDot = (wTemp & ((unsigned short)1)<<(15-j)) != 0;

			if(withBorder) //draw 3X3 block
			{
				if(isDot)
				{
					for(register int ii=0; ii<3; ii++)
					{
						register unsigned short* wbuff = pTarBuf+ii*pitchOfShort+j;
						*wbuff++ = 0xFFFF; *wbuff++ = 0xFFFF; *wbuff = 0xFFFF; 
					}
				}
			}
			else
			{
				*(pTarBuf++) = isDot?0xFFFF:0x00;
			}
		}
	}

	return width;
}
