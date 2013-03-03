#pragma once
#include "Imageset.h"

class CTGAFile : public IImageFile
{
public:
	CTGAFile();
	virtual ~CTGAFile();

	virtual void	clear(void);
	virtual bool	loadFromFile(const char* szFileName);
	virtual HBITMAP	getRGBBitmap(void) const { return m_hRGBBitmap; }
	virtual HBITMAP	getAlphaBitmap(void) const { return m_hAlphaBitmap; }

private:
	int		m_nWidth;
	int		m_nHeight;
	int		m_nPixelDepth;

	HBITMAP	m_hRGBBitmap;
	HBITMAP	m_hAlphaBitmap;
};