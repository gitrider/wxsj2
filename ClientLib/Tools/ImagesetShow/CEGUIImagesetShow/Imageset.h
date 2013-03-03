#pragma once

struct ImagesetImage
{
	std::string strName;
	int xPos;
	int yPos;
	int width;
	int height;
};

typedef std::vector< ImagesetImage > ImageVector;


class IImageFile
{
public:
	virtual void	clear(void) = 0;
	virtual bool	loadFromFile(const char* szFileName) = 0;
	virtual HBITMAP	getRGBBitmap(void) const = 0;
	virtual HBITMAP	getAlphaBitmap(void) const = 0;
};

class CImageset
{
public:
	CImageset() : m_pImageFile(0) {}
	~CImageset() { if(m_pImageFile) m_pImageFile->clear(); }
public:
	ImageVector		imageVector;
	std::string		m_strImagesetFileName;
	std::string		m_strImageFileName;
	IImageFile*		m_pImageFile;
};