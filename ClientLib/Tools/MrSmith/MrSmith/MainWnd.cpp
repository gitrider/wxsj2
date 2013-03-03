// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MrSmith.h"
#include "MainWnd.h"
#include ".\mainwnd.h"

#include "SMSystem.h"
#include "SMNetManager.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "CGCommand.h"
#include "SMLog.h"
#include "SMNetSend.h"

// CMainWnd dialog

IMPLEMENT_DYNAMIC(CMainWnd, CDialog)
CMainWnd* CMainWnd::s_pMe = 0;
CMainWnd::CMainWnd(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CMainWnd::IDD, pParent), 
#pragma warning(disable : 4355)
	m_legendWnd(this)
#pragma warning(default : 4355)
{
	s_pMe = this;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
	DDX_Control(pDX, IDC_EDT_CMD, m_edtCmd);
}


BEGIN_MESSAGE_MAP(CMainWnd, CResizableDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnTcnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SENDALL, OnBnClickedBtnSendall)
END_MESSAGE_MAP()


// CMainWnd message handlers

BOOL CMainWnd::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_STATIC_TOP, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_BOTTOM, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC_PARENT, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TAB_MAIN, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_EDT_CMD, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_SEND, BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_SENDALL, BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC_AGENTINFO_PARENT, BOTTOM_LEFT, BOTTOM_RIGHT);

	m_legendWnd.SubclassWindow(this->GetDlgItem(IDC_STATIC_PARENT)->GetSafeHwnd());

	//create sub window
	createSubWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMainWnd::createSubWindow(void)
{
	m_tabMain.InsertItem(0, "Log");
	m_tabMain.InsertItem(1, "Info");

	CRect rect;
	CWnd* pParent = GetDlgItem(IDC_STATIC_AGENTINFO_PARENT);
	pParent->GetClientRect(&rect);
	pParent->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

	//Log window
	m_infodlg_Log.Create(CAgentInfo_LogDlg::IDD, pParent);
	m_infodlg_Log.MoveWindow(&rect);
	m_infodlg_Log.ShowWindow(SW_SHOW);

	//Info window
	m_infodlg_Info.Create(CAgentInfo_InfoDlg::IDD, pParent);
	m_infodlg_Info.MoveWindow(&rect);
	m_infodlg_Info.ShowWindow(SW_HIDE);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rect;
	CWnd* pParent = GetDlgItem(IDC_STATIC_AGENTINFO_PARENT);
	if(!pParent) return;

	pParent->GetClientRect(&rect);

	if(m_infodlg_Log.GetSafeHwnd())
	{
		m_infodlg_Log.MoveWindow(&rect);
	}

	if(m_infodlg_Info.GetSafeHwnd())
	{
		m_infodlg_Info.MoveWindow(&rect);
	}
}

void CMainWnd::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSel = m_tabMain.GetCurSel();
	switch(nSel)
	{
	case 0:
		m_infodlg_Log.ShowWindow(SW_SHOW);
		m_infodlg_Info.ShowWindow(SW_HIDE);
		break;

	case 1:
		m_infodlg_Log.ShowWindow(SW_HIDE);
		m_infodlg_Info.ShowWindow(SW_SHOW);
		break;
	}
}

BOOL CMainWnd::PreTranslateMessage(MSG* pMsg)
{
	switch( pMsg->message )
	{
	case WM_KEYDOWN :
		// 截获回车键消息
		if( pMsg->wParam == VK_RETURN )
		{
			OnBnClickedBtnSend();
			m_edtCmd.SetWindowText( "" );
			return TRUE;
		}
		break;
	}
	return CResizableDialog::PreTranslateMessage( pMsg );
}

// 指令分析
void ParseCommand( SMITH::Agent* pAgent, char* str, int siz )
{
	SMITH::Log*		pLog	= NULL;
	if( !pAgent || pAgent->getState() != SMITH::Agent::Working )
		return;
	pLog			= pAgent->getLog();
	if( !pLog )
		return;

	char			cmd[512]= { "" };
	if( sscanf( str, "cmd::%s", cmd ) )
	{
		if( strcmp( cmd, "ent" ) == 0 )
		{
			pLog->addLog( "发送Ask CharLogin..." );
			SMITH::MsgSend_CLAskCharLogin( pAgent );
		}
	}
	else
	{
		CGCommand	msg;
		msg.SetCommand( str );
		msg.SetCommandSize( (BYTE)siz );
		pAgent->getNetManager()->sendPacket( &msg );
	}

	pLog->addLogFmt( "Send Command : %s", str );
}

void CMainWnd::OnBnClickedBtnSend()
{
	// TODO: Add your control notification handler code here
	CString			str;
	m_edtCmd.GetWindowText( str );
	size_t			siz		= strlen( str.GetString() );
	if( siz <= 0 )
		return;

	const int		id		= SMITH::System::GetMe()->getSelectedAgent();
	SMITH::Agent*	pAgent	= SMITH::AgentManager::GetMe()->findAgent( id );

	ParseCommand( pAgent, str.GetBuffer( int(siz) ), int(siz) );
}

void CMainWnd::OnBnClickedBtnSendall()
{
	// TODO: Add your control notification handler code here
	CString			str;
	m_edtCmd.GetWindowText( str );
	size_t			siz		= strlen( str.GetString() );
	if( siz <= 0 )
		return;

	SMITH::Agent*	pAgent	= NULL;
	for( int i = 1; i <= SMITH::AgentManager::GetMe()->getAgentNum(); i ++ )
	{
		Sleep( 100 );
        pAgent	= SMITH::AgentManager::GetMe()->findAgent( i );

		ParseCommand( pAgent, str.GetBuffer( int(siz) ), int(siz) );
	}	
}
