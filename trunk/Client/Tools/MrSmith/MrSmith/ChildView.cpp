// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "MrSmith.h"
#include "ChildView.h"
#include ".\childview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView ��Ϣ�������

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	// TODO: �ڴ˴������Ϣ����������
	
	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
    CRect rectClient;
    GetClientRect(&rectClient);
    
    if (!m_mainWnd.Create(IDD_DIALOG_MAIN, this))
		return -1;
	m_mainWnd.ShowWindow(SW_SHOW);
	m_mainWnd.MoveWindow(&rectClient);
	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
    CRect rectClient;
    GetClientRect(&rectClient);
	if(m_mainWnd.GetSafeHwnd())
	{
//		rectClient.DeflateRect(1, 1, 1, 1);
		m_mainWnd.MoveWindow(&rectClient);
	}
}
