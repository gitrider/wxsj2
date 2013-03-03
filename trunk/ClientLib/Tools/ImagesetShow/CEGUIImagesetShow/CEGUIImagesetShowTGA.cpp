#include "StdAfx.h"
#include "CEGUIImagesetShowTGA.h"

CTGAFile::CTGAFile()
	: m_nWidth(0)
	, m_nHeight(0)
	, m_hRGBBitmap(0)
	, m_hAlphaBitmap(0)
{
}

CTGAFile::~CTGAFile()
{
	clear();
}

void CTGAFile::clear(void)
{
	if(m_hRGBBitmap)
	{
		::DeleteObject(m_hRGBBitmap);
		m_hRGBBitmap=0;
	}

	if(m_hAlphaBitmap)
	{
		::DeleteObject(m_hAlphaBitmap);
		m_hAlphaBitmap=0;
	}

	m_nWidth = m_nHeight = 0;
}

bool _saveBmpFile(const char* szFileName, int width, int height, const char* pBuf)
{
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));

	FILE* fp = fopen(szFileName, "wb");
	if(!fp) 
	{
		printf("Create file %s ERROR!\n", szFileName);
		return false;
	}

	bmpFileHeader.bfType = 0x4D42;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits+width*height*3;

	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = width;
	bmpInfoHeader.biHeight = height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biSizeImage = width*height*3;
	bmpInfoHeader.biXPelsPerMeter = 0x0b12;
	bmpInfoHeader.biYPelsPerMeter = 0x0b12;
	bmpInfoHeader.biClrImportant= 0;

	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	for(int i=0; i<height; i++)
	{
		fwrite(pBuf+width*3*(height-i-1), 1, width*3, fp);
	}
	fclose(fp);

	return true;
}

bool CTGAFile::loadFromFile(const char* szFileName)
{
	assert(szFileName && szFileName[0]);

	FILE* fp = fopen(szFileName, "rb");
	if(!fp) return false;

	#pragma pack(push, 1)
	struct TGAHEADER
	{  
		BYTE   IdLength;  
		BYTE   ColorMapType;  
		BYTE   ImageType;  
		WORD   CMapStart;  
		WORD   CMapLenght;  
		BYTE   CMapDepth;  
		WORD   XOffSet;  
		WORD   YOffSet;  
		WORD   Width;  
		WORD   Height;  
		BYTE   PixelDepth;  
		BYTE   ImageDescriptor;  
	} tgaFileHeader;
	#pragma pack(pop)

	//read file head
	fread(&tgaFileHeader, sizeof(TGAHEADER), 1, fp);
	if(tgaFileHeader.ImageType != 2 || tgaFileHeader.PixelDepth != 32) 
	{
		fclose(fp);
		return false;
	}
	m_nWidth = tgaFileHeader.Width;
	m_nHeight = tgaFileHeader.Height;
	m_nPixelDepth = tgaFileHeader.PixelDepth;

	char* pTempMemory = new char[m_nWidth*m_nHeight*(m_nPixelDepth/8)];
	fread(pTempMemory, 1, m_nWidth*m_nHeight*(m_nPixelDepth/8), fp);
	fclose(fp); fp=0;

	int canvasPitch = m_nWidth*3;
	while(canvasPitch%4) canvasPitch++;
	char* pCanvasBitbuf = 0;

	HDC hScreenDC = GetDC(GetDesktopWindow());

	//create RGB bitmap
	BITMAPINFO bmpInfo; 
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize		= sizeof (BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth		= m_nWidth; 
	bmpInfo.bmiHeader.biHeight		= m_nHeight;
	bmpInfo.bmiHeader.biPlanes		= 1;
	bmpInfo.bmiHeader.biBitCount	= 24;
	bmpInfo.bmiHeader.biSizeImage	= canvasPitch * m_nHeight;

	m_hRGBBitmap = CreateDIBSection(hScreenDC, &bmpInfo, DIB_RGB_COLORS, (VOID**)&(pCanvasBitbuf), 0, 0);
	
	const unsigned char* pSrcBuf = (const unsigned char*)pTempMemory;
	unsigned char* pTargetBuf = (unsigned char*)pCanvasBitbuf;
	if(m_nPixelDepth == 32)
	{
		for(int i=0; i<m_nHeight; i++)
		{
			for(int j=0; j<m_nWidth; j++)
			{
				*pTargetBuf++ = *pSrcBuf++;
				*pTargetBuf++ = *pSrcBuf++;
				*pTargetBuf++ = *pSrcBuf++;
				pSrcBuf ++;
			}
		}
	}

	//create Alpha bitmap
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize		= sizeof (BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth		= m_nWidth; 
	bmpInfo.bmiHeader.biHeight		= m_nHeight;
	bmpInfo.bmiHeader.biPlanes		= 1;
	bmpInfo.bmiHeader.biBitCount	= 24;
	bmpInfo.bmiHeader.biSizeImage	= canvasPitch * m_nHeight;

	m_hAlphaBitmap = CreateDIBSection(hScreenDC, &bmpInfo, DIB_RGB_COLORS, (VOID**)&(pCanvasBitbuf), 0, 0);
	
	pSrcBuf = (const unsigned char*)pTempMemory;
	pTargetBuf = (unsigned char*)pCanvasBitbuf;
	if(m_nPixelDepth == 32)
	{
		for(int i=0; i<m_nHeight; i++)
		{
			for(int j=0; j<m_nWidth; j++)
			{
				pSrcBuf += 3;
				*pTargetBuf++ = *pSrcBuf;
				*pTargetBuf++ = *pSrcBuf;
				*pTargetBuf++ = *pSrcBuf;
				pSrcBuf++;
			}
		}
	}

	::ReleaseDC(GetDesktopWindow(), hScreenDC);

	_saveBmpFile("D:\\TEMP.BMP", m_nWidth, m_nHeight, pCanvasBitbuf);

	return true;
}
