#include "StdAfx.h"
#include ".\bitmapfile.h"

CBitmapFile::CBitmapFile(void)
	: m_nWidth(0)
	, m_nHeight(0)
	, m_nPitch(0)
	, m_pBitBuf(0)
{
}

CBitmapFile::~CBitmapFile(void)
{
	if(m_pBitBuf)
	{
		delete[] m_pBitBuf; m_pBitBuf=0;
	}
}


bool CBitmapFile::openFromFile(const char* szFileName)
{
	assert(szFileName);
	FILE* fp = fopen(szFileName, "rb");
	if(!fp) 
	{
		printf("Can't open %s error!\n", szFileName);
		return false;
	}

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	if(bmpFileHeader.bfType != 0X4D42)
	{
		printf("Must a bmp file!\n");
		fclose(fp); return false;
	}

	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	if(bmpInfoHeader.biBitCount != 24)
	{
		printf("Must a 24bitm bmp file!\n");
		fclose(fp); return false;
	}

	m_nWidth = bmpInfoHeader.biWidth;
	m_nHeight = bmpInfoHeader.biHeight;

	if((m_nWidth&0x7F) != 0 || (m_nHeight&0x7F) != 0)
	{
		printf("Must 128 multiple size file!\n");
		fclose(fp); return false;
	}

	m_nPitch = m_nWidth*3;

	//read bitbuf
	m_pBitBuf = new char[m_nPitch*m_nHeight];
	fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET);
	//flip read
	for(int i=0; i<m_nHeight; i++)
	{
		fread(m_pBitBuf+m_nPitch*(m_nHeight-i-1), 1, m_nPitch, fp);
	}
	fclose(fp); fp=0;

	return true;
}

bool CBitmapFile::splitToFile(const char* szFileName, int basesize, bool sortHorz)
{
	assert(m_pBitBuf);

	int widthCounts = m_nWidth/basesize;
	if(widthCounts*basesize!=m_nWidth) widthCounts++;

	int heightCounts = m_nHeight/basesize;
	if(heightCounts*basesize!=m_nHeight) heightCounts++;

	char* pTempBuf = new char[basesize*basesize*3];
	
	int xLeft=0, yTop=0;
	for(int i=0; i<heightCounts; i++, yTop+=basesize)
	{
		xLeft=0;
		for(int j=0; j<widthCounts; j++, xLeft+=basesize)
		{
			char szTargetFileName[MAX_PATH];
			if(sortHorz)
			{
				_snprintf(szTargetFileName, MAX_PATH, "%s_%d.bmp", szFileName, i*widthCounts+j);
			}
			else
			{
				_snprintf(szTargetFileName, MAX_PATH, "%s_%d_%d.bmp", szFileName, j, i);
			}

			int width=basesize; if(width+xLeft>m_nWidth) width=basesize/2;
			int height=basesize; if(height+yTop>m_nHeight) height=basesize/2;

			_fillBmpCrop(xLeft, yTop, width, height, pTempBuf);
			if(!_saveBmpFile(szTargetFileName, width, height, pTempBuf))
			{
				delete[] pTempBuf;
				return false;
			}

			printf("Save %s OK\n", szTargetFileName);
		}
	}

	delete[] pTempBuf; pTempBuf=0;
	return true;
}

bool CBitmapFile::_saveBmpFile(const char* szFileName, int width, int height, const char* pBuf)
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

void CBitmapFile::_fillBmpCrop(int x, int y, int width, int height, char* pBuf)
{
	for(int i=0; i<height; i++)
	{
		const char* pSourcBuf = m_pBitBuf+(y+i)*m_nPitch+x*3;
		char* pTargetBuf = pBuf+i*(width*3);

		memcpy(pTargetBuf, pSourcBuf, width*3);
	}
}
