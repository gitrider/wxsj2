// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MrSmith.h"

#include "MainFrm.h"
#include "SMSystem.h"
#include "DlgAddAgent.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SYSTEM_START, OnSystemStart)
	ON_COMMAND(ID_SYSTEM_PAUSE, OnSystemPause)
	ON_COMMAND(ID_SYSTEM_ADD, OnSystemAdd)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_START, OnUpdateSystemStart)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_PAUSE, OnUpdateSystemPause)
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_SEPARATOR,           // Agent system state
	ID_SEPARATOR,           // Agent num
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL, 100);

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	srand( (unsigned)time( NULL ) );

	return 0;
}

void CMainFrame::setStatusText_AgentNum(int nNumber)
{
	CString strTemp;
	strTemp.Format("Agent=%d", nNumber);

	m_wndStatusBar.SetPaneText(1, (LPCTSTR)strTemp);
}

void CMainFrame::setStatusText_AgentStatus(const char* szStatus)
{
	m_wndStatusBar.SetPaneText(2, (LPCTSTR)szStatus);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnSystemStart()
{
	// TODO: Add your command handler code here
	SMITH::System* pSystem = SMITH::System::GetMe();

	if(pSystem->getState() == SMITH::System::Ready)
	{
		SMITH::System::GetMe()->start();
	}
	else if(pSystem->getState() == SMITH::System::Working || pSystem->getState() == SMITH::System::Pause)
	{
		SMITH::System::GetMe()->stop();
	}
}

void CMainFrame::OnSystemPause()
{
	SMITH::System* pSystem = SMITH::System::GetMe();

	if(pSystem->getState() == SMITH::System::Pause)
	{
		pSystem->resume();
	}
	else if(pSystem->getState() == SMITH::System::Working)
	{
		pSystem->pause();
	}
}

void CMainFrame::OnUpdateSystemStart(CCmdUI *pCmdUI)
{
	SMITH::System* pSystem = SMITH::System::GetMe();

	pCmdUI->SetCheck(pSystem->getState() != SMITH::System::Ready);
	pCmdUI->Enable(pSystem->getState() != SMITH::System::StopPedding);
}


void CMainFrame::OnUpdateSystemPause(CCmdUI *pCmdUI)
{
	SMITH::System* pSystem = SMITH::System::GetMe();

	pCmdUI->SetCheck(pSystem->getState() == SMITH::System::Pause);
	pCmdUI->Enable(pSystem->getState() == SMITH::System::Working || pSystem->getState() == SMITH::System::Pause);
}

void CMainFrame::OnSystemAdd()
{
	// TODO: Add your command handler code here
	static int s_nAddNumber = 1;

	CDlgAddAgent dlg(s_nAddNumber);
	if(IDOK != dlg.DoModal()) return;

	s_nAddNumber = dlg.getAgentNum();

	//delete at smith thread!
	SMITH::AgentCreateParam *params = new SMITH::AgentCreateParam;
	*params = *dlg.getAgentCreateParam();

	SMITH::System::GetMe()->addAgent(s_nAddNumber, params);
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	using namespace SMITH;

	switch(message)
	{
	case System::WMA_AGENT_NUM:
		{
			setStatusText_AgentNum((int)wParam);
		}
		return TRUE;

	case System::WMA_STATE_CHANGED:
		{
			switch(System::GetMe()->getState())
			{
			case System::Ready:
				setStatusText_AgentStatus("Ready");
				break;
			case System::Working:
				setStatusText_AgentStatus("Working...");
				break;
			case System::Pause:
				setStatusText_AgentStatus("Pause");
				break;
			case System::StopPedding:
				setStatusText_AgentStatus("Stoping...");
				break;
			default:
				setStatusText_AgentStatus("");
				break;
			}
		}
		return TRUE;
	default: break;
	}
	return CFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	SMITH::System::GetMe()->stop();

	//start timer
	SetTimer(101, 100, 0);
}

void CMainFrame::OnFileOpen()
{
	CFileDialog dlg(TRUE, "ini", 0, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Create params file(*.ini)|*.ini|All File(*.*)|*.*||");
	if(IDOK != dlg.DoModal()) return;

	if(!SMITH::System::GetMe()->loadAgentsCreateParam((LPCTSTR)dlg.GetPathName()))
	{
		MessageBox("�����ļ���������!", 0, MB_OK|MB_ICONSTOP);
	}
}

void CMainFrame::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE, "ini", 0, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Create params file(*.ini)|*.ini|All File(*.*)|*.*||");
	if(IDOK != dlg.DoModal()) return;

	if(!SMITH::System::GetMe()->saveAgentsCreateParam((LPCTSTR)dlg.GetPathName()))
	{
		MessageBox("�����ļ���������!", 0, MB_OK|MB_ICONSTOP);
	}
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 101)
	{
		if(SMITH::System::GetMe()->getState() == SMITH::System::Ready)
		{
			KillTimer(101);

			CFrameWnd::OnClose();
			return;
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(SMITH::AgentManager::GetMe() && SMITH::AgentManager::GetMe()->getAgentNum() != 0);
}
