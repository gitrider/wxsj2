#include "StdAfx.h"
#include "FU_CanvasWnd.h"
#include "AxBmpFont.h"

//--------------------------------------------------------------------------------------------
CCanvasWindow::CCanvasWindow()
	: m_pBmpFont(0)
	, m_codeStart(0)
{
}

//--------------------------------------------------------------------------------------------
CCanvasWindow::~CCanvasWindow()
{

}

//--------------------------------------------------------------------------------------------
LRESULT CCanvasWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dc(m_hWnd);

	if(!m_bmpCanvas.IsNull())
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(dc);
		HBITMAP hBmpOld = dcMem.SelectBitmap(m_bmpCanvas);
		dc.BitBlt(0, 0, m_size.cx, m_size.cy, dcMem, 0, 0, SRCCOPY);
		dcMem.SelectBitmap(hBmpOld);
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
void CCanvasWindow::setBmpFont(CAxBitmapFont* pBmpFont)
{
	assert(pBmpFont);

	m_pBmpFont = pBmpFont;
}

//--------------------------------------------------------------------------------------------
void CCanvasWindow::setCodeStart(wchar_t codeStart)
{
	m_codeStart = codeStart;

	redraw();
}

//--------------------------------------------------------------------------------------------
void CCanvasWindow::_createCanvas(int width, int height)
{
	//free old canvas
	if(!m_bmpCanvas.IsNull())
	{
		m_bmpCanvas.DeleteObject();
	}

	m_size.cx = width;
	m_size.cy = height;

	//cal pitch
	m_nCanvasPitch=width*2;
	while(m_nCanvasPitch%4!=0) m_nCanvasPitch++;
	m_nCanvasPitch=m_nCanvasPitch>>1;

	// create canvas dc
	CDC dcCanvas;
	dcCanvas.CreateCompatibleDC(CClientDC(m_hWnd));

	//create DIB Section
	BITMAPINFO bmpInfo; 
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize		= sizeof (BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth		= width; 
	bmpInfo.bmiHeader.biHeight		= -height;
	bmpInfo.bmiHeader.biPlanes		= 1;
	bmpInfo.bmiHeader.biBitCount	= 16;
	bmpInfo.bmiHeader.biSizeImage	= m_nCanvasPitch * height * 2;
	m_bmpCanvas.Attach(CreateDIBSection(CClientDC(m_hWnd), &bmpInfo, DIB_RGB_COLORS, (VOID**)&(m_pCanvasBitbuf), 0, 0));
}

//--------------------------------------------------------------------------------------------
LRESULT CCanvasWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int cx = GET_X_LPARAM(lParam);
	int cy = GET_Y_LPARAM(lParam);
	bHandled = FALSE;

	if(cx==0 || cy==0) return 0;

	//reset canvas
	_createCanvas(cx, cy);

	// redraw
	redraw();

	return 1;
}

//--------------------------------------------------------------------------------------------
void CCanvasWindow::redraw(void)
{
	if(m_bmpCanvas.IsNull() || !m_pBmpFont) return;

	//fill back
	for(int i=0; i<m_size.cy; i++)
	{
		unsigned short* pCanvasBit = m_pCanvasBitbuf+i*m_nCanvasPitch;
		ZeroMemory(pCanvasBit, m_nCanvasPitch*sizeof(unsigned short)); 
	}

	int fontSize = m_pBmpFont->getFontSize();
	int charCountsWidth = m_size.cx/fontSize;
	int charCountsHeight= m_size.cy/fontSize;

	int code = (int)m_codeStart;
	for(int i=0; i<charCountsHeight; i++)
	{
		if(code>0xFFFF) break;
		for(int j=0; j<charCountsWidth; j++)
		{
			if(code>0xFFFF) break;
			unsigned short* pCanvasBit = m_pCanvasBitbuf + 
				i*fontSize*m_nCanvasPitch+j*fontSize;

			while(!(m_pBmpFont->isCodeValid((wchar_t)code)))
			{
				code++;
				if(code>0xFFFF) break;
			}
			if(code>0xFFFF) break;
			m_pBmpFont->fillFontMemory(code++, pCanvasBit, m_nCanvasPitch, false);
		}
	}

	//send redraw request
	::InvalidateRect(m_hWnd, 0, TRUE);
}