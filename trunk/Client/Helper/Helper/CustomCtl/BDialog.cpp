// BDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBDialog dialog


CBDialog::CBDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CBDialog)
		// NOTE: the ClassWizard will add member initialization here
		m_style=StyleTile;
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CBDialog, CDialog)
	//{{AFX_MSG_MAP(CBDialog)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBDialog message handlers

BOOL CBDialog::OnEraseBkgnd(CDC* pDC) 
{
	CDialog::OnEraseBkgnd(pDC);	
	if(!m_bitmap.m_hObject)
		return true;
	
	CRect rect;
	GetClientRect(&rect);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = dc.SelectObject(&m_bitmap);
	int bmw, bmh ;
	BITMAP bmap;
	m_bitmap.GetBitmap(&bmap);
	bmw = bmap.bmWidth;
	bmh = bmap.bmHeight;
	int xo=0, yo=0;

	if(m_style == StyleTile)
	{
		for (yo = 0; yo < rect.Height(); yo += bmh)
		{
			for (xo = 0; xo < rect.Width(); xo += bmw)
			{
				pDC->BitBlt (xo, yo, rect.Width(),
					rect.Height(), &dc,
					0, 0, SRCCOPY);
			}
		}

	}

	if(m_style == StyleCenter)
	{
		if(bmw < rect.Width())
			xo = (rect.Width() - bmw)/2;
		else 
			xo=0;
		if(bmh < rect.Height())
			yo = (rect.Height()-bmh)/2;
		else
			yo=0;
		pDC->BitBlt (xo, yo, rect.Width(),
			rect.Height(), &dc,
			0, 0, SRCCOPY);
	}

	if(m_style == StyleStretch)
	{
		pDC->StretchBlt(xo, yo, rect.Width(),
			rect.Height(), &dc,
			0, 0,bmw,bmh, SRCCOPY);
	}

	
	dc.SelectObject(pOldBitmap);


	return true;
}

void CBDialog::SetBitmapStyle(int style)
{
	if((style==StyleTile)||
		(style==StyleCenter)||
		(style==StyleStretch))
	{	
		m_style = style;
	}

}

int CBDialog::SetBitmap(UINT nIDResource)
{
	if(m_bitmap.LoadBitmap(nIDResource))
	{
		BITMAP bmp;
		m_bitmap.GetBitmap(&bmp);

		SetWindowPos(NULL, -1, -1, bmp.bmWidth, bmp.bmHeight,
					 SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
		return 0;
	}
	else
		return 1;//error
}
