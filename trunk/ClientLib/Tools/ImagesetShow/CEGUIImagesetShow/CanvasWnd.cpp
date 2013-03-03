// CanvasWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CEGUIImagesetShow.h"
#include "CanvasWnd.h"
#include ".\canvaswnd.h"
#include "Imageset.h"
#include "CEGUIImagesetShowTGA.h"

// CCanvasWnd

IMPLEMENT_DYNAMIC(CCanvasWnd, CStatic)
CCanvasWnd::CCanvasWnd()
	: m_pImageset(0)
	, m_freshType(FT_RGB)
	, m_nFocusIndex(-1)
{
}

CCanvasWnd::~CCanvasWnd()
{
}


BEGIN_MESSAGE_MAP(CCanvasWnd, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CCanvasWnd message handlers
void CCanvasWnd::freshType(FRESH_TYPE type)
{
	m_freshType = type;
	InvalidateRect(0);
}

void CCanvasWnd::setImageset(CImageset*	pImageset) 
{ 
	m_pImageset = pImageset; 
	m_nFocusIndex=-1;
	InvalidateRect(0);
}

void CCanvasWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	dc.FillRect(&rect, CBrush::FromHandle((HBRUSH)::GetStockObject(DKGRAY_BRUSH)));

	if(!m_pImageset)
	{
		return;
	}

	HBITMAP hSrc = 0;
	switch(m_freshType)
	{
	case FT_RGB:	hSrc = m_pImageset->m_pImageFile->getRGBBitmap(); break;
	case FT_ALPHA:	hSrc = m_pImageset->m_pImageFile->getAlphaBitmap(); break;
	default: return;
	}

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(&dc);
	CBitmap* pOldBmp = dcSrc.SelectObject(CBitmap::FromHandle(hSrc));

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcSrc, 0, 0, SRCCOPY);

	//draw focus
	if(m_nFocusIndex>=0 && m_nFocusIndex<(int)m_pImageset->imageVector.size())
	{
		ImagesetImage& image = m_pImageset->imageVector[m_nFocusIndex];

		CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
		CPen* pOldPen = dc.SelectObject(&pen);

		int oldStyle = dc.SetROP2(R2_XORPEN);

		dc.MoveTo(image.xPos, image.yPos);
		dc.LineTo(image.xPos + image.width, image.yPos);
		dc.LineTo(image.xPos + image.width, image.yPos+image.height);
		dc.LineTo(image.xPos, image.yPos + image.height);
		dc.LineTo(image.xPos, image.yPos);

		dc.SetROP2(oldStyle);
		dc.SelectObject(pOldPen);
	}

	dcSrc.SelectObject(pOldBmp);
}

void CCanvasWnd::setFocus(int index)
{
	if(index<0 || index>=(int)m_pImageset->imageVector.size()) return;

	m_nFocusIndex = index;
	InvalidateRect(0);
}
