// AgentInfo_InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MrSmith.h"
#include "AgentInfo_InfoDlg.h"


// CAgentInfo_InfoDlg dialog

IMPLEMENT_DYNAMIC(CAgentInfo_InfoDlg, CDialog)
CAgentInfo_InfoDlg::CAgentInfo_InfoDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CAgentInfo_InfoDlg::IDD, pParent)
{
}

CAgentInfo_InfoDlg::~CAgentInfo_InfoDlg()
{
}

void CAgentInfo_InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_listLog1);
}


BEGIN_MESSAGE_MAP(CAgentInfo_InfoDlg, CResizableDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAgentInfo_InfoDlg message handlers

BOOL CAgentInfo_InfoDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_LIST_INFO, TOP_LEFT, BOTTOM_RIGHT);
	
	CRect rect;
	GetClientRect(&rect);

	m_listLog1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listLog1.InsertColumn(0, "Time", LVCFMT_LEFT, 80);
	m_listLog1.InsertColumn(1, "Contents", LVCFMT_LEFT, rect.Width()-50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAgentInfo_InfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_listLog1.GetSafeHwnd())
	{
		CRect rect;
		m_listLog1.GetWindowRect(&rect);

		int nWidth0 = m_listLog1.GetColumnWidth(0);
		m_listLog1.SetColumnWidth(1, cx-nWidth0-32);
	}

}

void CAgentInfo_InfoDlg::clearAllLog(void)
{
	m_listLog1.DeleteAllItems();
}

void CAgentInfo_InfoDlg::addLog(const SYSTEMTIME& tTime, const std::string& strContents)
{
	char szTime[64] = {0};
	_snprintf(szTime, 64, "%02d_%02d:%02d:%02d", 
		tTime.wDay, tTime.wHour, tTime.wMinute, tTime.wSecond);

	int newIndex = m_listLog1.InsertItem(m_listLog1.GetItemCount(), szTime);

	m_listLog1.SetItemText(newIndex, 1, strContents.c_str());
}
