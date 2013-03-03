// PerfectControl\Tree\OptionDialogPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "OptionTreeDialogPanel.h"


// COptionTreeDialogPanel

IMPLEMENT_DYNAMIC(COptionTreeDialogPanel, CWnd)

COptionTreeDialogPanel::COptionTreeDialogPanel(INT dlgResId, CWnd* pParent, DLGPROC dlgProc)
{
	Create(dlgResId, pParent, dlgProc);
}

COptionTreeDialogPanel::~COptionTreeDialogPanel()
{
}

BOOL COptionTreeDialogPanel::Create(INT dlgResId, CWnd* pParent, DLGPROC dlgProc)
{
	if (dlgProc)
	{
		m_hWnd = ::CreateDialog(AfxGetInstanceHandle(), MAKEINTRESOURCE(dlgResId), 
			pParent ? pParent->GetSafeHwnd() : NULL , dlgProc);
		return m_hWnd != NULL;
	}
	return FALSE;
}


BEGIN_MESSAGE_MAP(COptionTreeDialogPanel, CWnd)
END_MESSAGE_MAP()



// COptionTreeDialogPanel 消息处理程序


