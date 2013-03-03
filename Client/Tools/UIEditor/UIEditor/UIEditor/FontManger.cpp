#include "StdAfx.h"
#include "FontManger.h"

CFont CFontManger:: m_font12;
CFont CFontManger:: m_font16;
CFont CFontManger::m_font14;
CFont CFontManger:: m_font15;

CFontManger::CFontManger(void)
{

}

void CFontManger::Initialize()
{
	LOGFONT lfont;
	memset(&lfont, 0, sizeof(LOGFONT));       // zero out structure
	lfont.lfCharSet = GB2312_CHARSET;
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfont.lfEscapement = 0;
	lfont.lfItalic = FALSE;
	lfont.lfOrientation = 0;
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfont.lfPitchAndFamily = FIXED_PITCH;
	lfont.lfQuality = DEFAULT_QUALITY;
	lfont.lfStrikeOut = FALSE;
	lfont.lfUnderline = FALSE;
	lfont.lfHeight = 12;                      // request a 12-pixel-height font
	_tcscpy_s(lfont.lfFaceName, _T("ËÎÌו"));        // request a face name "Arial"
	VERIFY(m_font12.CreateFontIndirect(&lfont));  // create the font
	lfont.lfHeight = 14;
	VERIFY(m_font14.CreateFontIndirect(&lfont));  // create the font
	lfont.lfHeight = 15;
	VERIFY(m_font15.CreateFontIndirect(&lfont));  // create the font
	lfont.lfHeight = 16;
	VERIFY(m_font16.CreateFontIndirect(&lfont));  // create the font
}

CFontManger::~CFontManger(void)
{
}
