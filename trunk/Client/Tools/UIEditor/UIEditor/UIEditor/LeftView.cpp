// LeftView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UIEditor.h"
#include "LeftView.h"
#include "FontManger.h"

// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CView)

CLeftView::CLeftView()
{
	m_initFlag = FALSE;
}

CLeftView::~CLeftView()
{
}

BEGIN_MESSAGE_MAP(CLeftView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLeftView ��ͼ

void CLeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CLeftView ���

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeftView ��Ϣ�������

void CLeftView::OnInitialUpdate( )
{
	CRect rect;
	GetClientRect(&rect);
	m_leftTab.Create(TCS_TABS | TCS_FIXEDWIDTH | WS_CHILD | 
		WS_VISIBLE | TCS_SINGLELINE | TCS_BOTTOM , rect, this, AFX_IDW_PANE_FIRST);
	DWORD dwExStyle= m_leftTab.GetExtendedStyle();
	m_leftTab.SetExtendedStyle(dwExStyle | TCS_EX_FLATSEPARATORS);
	m_leftTab.SetFont(&CFontManger::m_font12);
	m_leftTab.InsertItem(0, _T("�����б�"));
	m_leftTab.InsertItem(1, _T("ͼ�񼯺�"));
	m_leftTab.CreateChild();
	m_initFlag = TRUE;
}
BOOL CLeftView::OnEraseBkgnd(CDC* pDC)
{
	return CView::OnEraseBkgnd(pDC);
}



void CLeftView::OnSize(UINT nType, int cx, int cy)
{
	if (m_initFlag)
	{
		m_leftTab.OnSize(nType, cx, cy);
	}
	CView::OnSize(nType, cx, cy);
	
}
