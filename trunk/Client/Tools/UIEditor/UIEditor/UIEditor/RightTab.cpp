// RightTab.cpp : 实现文件
//

#include "stdafx.h"
#include "UIEditor.h"
#include "RightTab.h"
#include "FontManger.h"

// CRightTab

IMPLEMENT_DYNAMIC(CRightTab, CTabCtrl)

CRightTab::CRightTab()
{
	m_pPropList = NULL;
	m_pEventList = NULL;
}

CRightTab::~CRightTab()
{
}


BEGIN_MESSAGE_MAP(CRightTab, CTabCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CRightTab::OnTcnSelchange)
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(NM_CLICK, &CRightTab::OnNMClick)
END_MESSAGE_MAP()



// CRightTab 消息处理程序
 

BOOL CRightTab::CreateChild()
{
	m_pPropList = new CRightPropList();
	CRect rect;
	this->GetClientRect(&rect);
	rect.bottom -= 20;
	
	m_pPropList->Create(WS_CHILD|WS_VISIBLE, rect, this ,0);
	m_pPropList->SetFont(&CFontManger::m_font12);

	m_pEventList = new CRightEventList();
	m_pEventList->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT| LVS_SINGLESEL , rect, this ,1);
	m_pEventList->SetFont(&CFontManger::m_font12);

	m_curSel = RTAB_SEL_PROPLIST;
	m_pPropList->ShowWindow(SW_SHOW);
	m_pEventList->ShowWindow(SW_HIDE);
	m_pEventList->OnTabLoseFocus();		
	return TRUE;
}

void CRightTab::OnSize(UINT nType, int cx, int cy)
{
	MoveWindow(0,0, cx, cy, TRUE);
	if(m_pPropList)
	{
		m_pPropList->MoveWindow(0,0, cx, cy - 25);
	}
	if (m_pEventList)
	{
		m_pEventList->MoveWindow(0,0,cx,cy-25);
	}
	
}

int CRightTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateChild();

	return 0;
}

void CRightTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_curSel = GetCurSel();
	if (m_curSel == RTAB_SEL_PROPLIST)
	{
		m_pPropList->ShowWindow(SW_SHOW);
		m_pEventList->ShowWindow(SW_HIDE);
		m_pEventList->OnTabLoseFocus();		
	}
	else if (m_curSel == RTAB_SEL_EVENT)
	{
		m_pPropList->ShowWindow(SW_HIDE);
		m_pEventList->ShowWindow(SW_SHOW);
		m_pPropList->OnTabLoseFocus();
	}
	*pResult = 0;
}

BOOL CRightTab::OnEraseBkgnd(CDC* pDC)
{
	return  CTabCtrl::OnEraseBkgnd(pDC);
}

void CRightTab::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CTabCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CRightTab::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}
