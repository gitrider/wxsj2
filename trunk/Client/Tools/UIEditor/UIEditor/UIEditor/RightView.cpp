// RightView.cpp : 实现文件
//

#include "stdafx.h"
#include "UIEditor.h"
#include "RightView.h"
#include "FontManger.h"


// CRightView

IMPLEMENT_DYNCREATE(CRightView, CView)

CRightView::CRightView()
{
	m_createFlag = FALSE;
}

CRightView::~CRightView()
{
}

BEGIN_MESSAGE_MAP(CRightView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRightView 绘图

void CRightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	//m_RightTab->OnPaint(pDC);
}

afx_msg int CRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CView::OnCreate(lpCreateStruct);
}

void CRightView::OnInitialUpdate( )
{
	CRect rect;
	GetClientRect(&rect);
	m_pRightTab = new CRightTab();
	m_pRightTab->Create(TCS_TABS | TCS_FIXEDWIDTH | WS_CHILD | 
		WS_VISIBLE | TCS_SINGLELINE | TCS_BOTTOM, rect, this, 0);
	DWORD dwExStyle= m_pRightTab->GetExtendedStyle();
	m_pRightTab->SetExtendedStyle(dwExStyle | TCS_EX_FLATSEPARATORS);

	m_pRightTab->InsertItem(0, _T("属性"));
	m_pRightTab->InsertItem(1, _T("事件"));
	//m_pRightTab->CreateChild();

	m_pRightTab->SetFont(&CFontManger::m_font12);
	m_createFlag = TRUE;
}

void CRightView::OnSize(UINT nType, int cx, int cy)
{
	if ( m_createFlag )
	{
		m_pRightTab->OnSize(nType, cx, cy);
	}
	CView::OnSize(nType, cx, cy);
}


// CRightView 诊断

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRightView 消息处理程序

BOOL CRightView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return FALSE;//CView::OnEraseBkgnd(pDC);
}

void CRightView::OnDestroy()
{
	CView::OnDestroy();
}
