// LegendWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MrSmith.h"
#include "LegendWnd.h"
#include ".\legendwnd.h"
#include "MemDC.h"
#include "SMSystem.h"
#include "MainWnd.h"
// CLegendWnd

// singleton instance pointer
CLegendWnd* CLegendWnd::s_pMe = 0;
const int nLegendWidth=9, nLegendHeight=9;

IMPLEMENT_DYNAMIC(CLegendWnd, CStatic)
CLegendWnd::CLegendWnd(CMainWnd* pMainWnd) :
	m_pMainWnd(pMainWnd),
	m_idHover(-1),
	m_idSelected(-1)
{
	s_pMe = this;
	m_bmpLegend.LoadBitmap(IDB_BITMAP_LEGEND);
}

CLegendWnd::~CLegendWnd()
{
	s_pMe = 0;
}


BEGIN_MESSAGE_MAP(CLegendWnd, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CLegendWnd message handlers


void CLegendWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	CMemDC MemDC(&dc, rect);
	MemDC.FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE));
	//----------------------------------------------------
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(&dc);
	CBitmap* pOldBmp = dcBmp.SelectObject(&m_bmpLegend);
	
	//draw all legend
	int nX=0, nY=0;
	LegendMap::iterator it;
	for(it=m_legendMap.begin(); it!=m_legendMap.end(); it++)
	{
		int nXOffset = 0;
		switch(it->second.sel_state)
		{
		case SS_NORMAL:
			break;
		case SS_HORVER:
			MemDC.FillSolidRect(nX, nY, nLegendWidth+2, nLegendHeight+2, ::GetSysColor(COLOR_ACTIVECAPTION));
			break;
		case SS_SELECT:
			MemDC.Draw3dRect(nX, nY, nLegendWidth+2, nLegendHeight+2, 
				GetSysColor(COLOR_BTNSHADOW ), GetSysColor(COLOR_BTNHIGHLIGHT));
			break;
		}

		switch(it->second.state)
		{
		case LS_DIE:
			nXOffset = 0;
			break;

		case LS_LIVE_0:
			nXOffset = 9;
			break;

		case LS_LIVE_1:
			nXOffset = 18;
			break;

		case LS_FAULT:
			nXOffset = 27;
			break;
		}
		MemDC.BitBlt(nX+1, nY+1, nLegendWidth, nLegendHeight, &dcBmp, nXOffset, 0, SRCCOPY);

		nX += nLegendWidth+2;
		if(nX + nLegendWidth+2 >= rect.Width())
		{
			nY += nLegendHeight+2;
			nX = 0;
		}
	}

	dcBmp.SelectObject(pOldBmp);
	//----------------------------------------------------
}

BOOL CLegendWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE; //CStatic::OnEraseBkgnd(pDC);
}

int CLegendWnd::AllocaLegend(void)
{
	int id = generateUniqueId();

	Legend newLegend;
	newLegend.id = id;
	newLegend.state = LS_DIE;
	newLegend.sel_state = SS_NORMAL;

	m_legendMap.insert(std::make_pair(id, newLegend));

	InvalidateLegend();
	return id;
}

/*************************************************************************
	Implementation method to generate a unique id to use for a Agent.
*************************************************************************/
int CLegendWnd::generateUniqueId(void)
{
	static int s_uid_counter = 0;
    int ret = ++s_uid_counter;

	if(ret < 0) ret = 0;
	return ret;
}

void CLegendWnd::InvalidateLegend(void)
{
	this->Invalidate(FALSE);
}

void CLegendWnd::SetLegendState(int id, LegendState state)
{
	LegendMap::iterator itFind;
	itFind = m_legendMap.find(id);

	if(itFind == m_legendMap.end()) return;

	itFind->second.state = state;

	InvalidateLegend();
}

void CLegendWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rectClient;
	GetClientRect(&rectClient);

	int nX=0, nY=0;
	LegendMap::iterator it;
	int idHover = -1;
	for(it=m_legendMap.begin(); it!=m_legendMap.end(); it++)
	{
		CRect rect(nX, nY, nX+nLegendWidth+2, nY+nLegendHeight+2);

		if(it->second.sel_state != SS_SELECT)
		{
			if(rect.PtInRect(point))
			{
				it->second.sel_state = SS_HORVER;
				idHover = it->second.id;
			}
			else
			{
				it->second.sel_state = SS_NORMAL;
			}
		}

		nX += nLegendWidth+2;
		if(nX + nLegendWidth+2 >= rectClient.Width())
		{
			nY += nLegendHeight+2;
			nX = 0;
		}
	}

	if(m_idHover != idHover)
	{
		m_idHover = idHover;
		InvalidateLegend();
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CLegendWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);

	if(m_idHover == -1) return;

	LegendMap::iterator itThis = m_legendMap.find(m_idHover);
	LegendMap::iterator itSelect = m_legendMap.find(m_idSelected);
	if(itThis == itSelect) return;
	if(itThis->second.state == LS_DIE) return;
	
	// deselcted old 
	if(itSelect != m_legendMap.end())
	{
		itSelect->second.sel_state = SS_NORMAL;
	}

	//selected new
	if(itThis != m_legendMap.end())
	{
		itThis->second.sel_state = SS_SELECT;
	}

	m_idSelected = m_idHover;

	//------------------------------------------------
	//Log 
	SMITH::System::GetMe()->onLegendSelectChanged(m_idSelected, 
		m_pMainWnd->getLogDlg(), m_pMainWnd->getInfoDlg());

	InvalidateLegend();
}
