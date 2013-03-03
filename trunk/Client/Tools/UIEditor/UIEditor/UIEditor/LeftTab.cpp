// LeftTab.cpp : 实现文件
//

#include "stdafx.h"
#include "UIEditor.h"
#include "LeftTab.h"


// CLeftTab

IMPLEMENT_DYNAMIC(CLeftTab, CTabCtrl)

CLeftTab::CLeftTab()
{

}

CLeftTab::~CLeftTab()
{
}


BEGIN_MESSAGE_MAP(CLeftTab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CLeftTab::OnTcnSelchange)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CLeftTab::CreateChild()
{
	m_treeImage = new CLeftTreeImage();
	m_treeImage->Create(WS_CHILD  | TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|
						TVS_SHOWSELALWAYS|TVS_INFOTIP, CRect(0,0,600,200), this, 11);
	m_treeWindow = new CLeftTreeWindow();
	m_treeWindow->Create(WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|
		TVS_SHOWSELALWAYS|TVS_INFOTIP, CRect(0,0,600,200), this, 12);
	SetCurSel(0);
	return TRUE;
}

void CLeftTab::OnSize(UINT nType, int cx, int cy)
{
	MoveWindow(0,0, cx, cy, TRUE);
	m_treeImage->MoveWindow(0,0, cx, cy - 25);
	m_treeWindow->MoveWindow(0,0, cx, cy - 25);
	//CWnd::OnSize(nType, cx, cy);
}

// CLeftTab 消息处理程序



void CLeftTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_curSel = GetCurSel();
	if (m_curSel == 1)
	{
		m_treeImage->ShowWindow(SW_SHOW);
		m_treeWindow->ShowWindow(SW_HIDE);

		m_treeWindow->OnTabFoucsEvent(true);
		m_treeImage->OnTabFoucsEvent(false);
	}
	else if (m_curSel == 0)
	{
		m_treeImage->ShowWindow(SW_HIDE);
		m_treeWindow->ShowWindow(SW_SHOW);

		m_treeWindow->OnTabFoucsEvent(false);
		m_treeImage->OnTabFoucsEvent(true);
	}
	*pResult = 0;
}

BOOL CLeftTab::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;//CTabCtrl::OnEraseBkgnd(pDC);
}
