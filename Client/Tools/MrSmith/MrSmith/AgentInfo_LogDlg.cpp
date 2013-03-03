// AgentInfo_LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MrSmith.h"
#include "AgentInfo_LogDlg.h"
#include ".\agentinfo_logdlg.h"


// CAgentInfo_LogDlg dialog

IMPLEMENT_DYNAMIC(CAgentInfo_LogDlg, CDialog)
CAgentInfo_LogDlg::CAgentInfo_LogDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CAgentInfo_LogDlg::IDD, pParent)
{
}

CAgentInfo_LogDlg::~CAgentInfo_LogDlg()
{
}

void CAgentInfo_LogDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
}


BEGIN_MESSAGE_MAP(CAgentInfo_LogDlg, CResizableDialog)
	ON_WM_SIZE()
	ON_COMMAND(32773, On32773)
END_MESSAGE_MAP()


// CAgentInfo_LogDlg message handlers

BOOL CAgentInfo_LogDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_LIST_LOG, TOP_LEFT, BOTTOM_RIGHT);
	
	CRect rect;
	GetClientRect(&rect);

	m_listLog.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listLog.InsertColumn(0, "Time", LVCFMT_LEFT, 80);
	m_listLog.InsertColumn(1, "Contents", LVCFMT_LEFT, rect.Width()-50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAgentInfo_LogDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_listLog.GetSafeHwnd())
	{
		CRect rect;
		m_listLog.GetWindowRect(&rect);

		int nWidth0 = m_listLog.GetColumnWidth(0);
		m_listLog.SetColumnWidth(1, cx-nWidth0-32);
	}

}

void CAgentInfo_LogDlg::clearAllLog(void)
{
	m_listLog.DeleteAllItems();
}

void CAgentInfo_LogDlg::addLog(const SYSTEMTIME& tTime, const std::string& strContents)
{
	char szTime[64] = {0};
	_snprintf(szTime, 64, "%02d_%02d:%02d:%02d", 
		tTime.wDay, tTime.wHour, tTime.wMinute, tTime.wSecond);

	int newIndex = m_listLog.InsertItem(m_listLog.GetItemCount(), szTime);

	m_listLog.SetItemText(newIndex, 1, strContents.c_str());
}

void CAgentInfo_LogDlg::On32773()
{
	// TODO: Add your command handler code here
}
