// Helper.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Helper.h"
#include "HelperSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHelperApp

BEGIN_MESSAGE_MAP(CHelperApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHelperApp ����

CHelperApp::CHelperApp() :
	m_pMainDialog(0)
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CHelperApp::~CHelperApp()
{
}

// Ψһ��һ�� CHelperApp ����

CHelperApp theApp;
int g_nCodePage = 936;

// CHelperApp ��ʼ��

BOOL CHelperApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	//check codepage
	g_nCodePage = ::GetPrivateProfileIntA("Variable", "System_CodePage", 936, ".\\System.cfg");

	//---------------------------------------
	// ansys command line
	CString strCmdLine = m_lpCmdLine;
	CString strWinName;
	int nWinNameKey = strCmdLine.Find(L"-w:");
	if(nWinNameKey >= 0)
	{
		do
		{
			int nFirstDot = strCmdLine.Find(L"\"", nWinNameKey);
			if(nFirstDot < 0) break;

			int nSecondDot = strCmdLine.Find(L"\"", nFirstDot+1);
			if(nSecondDot < 0) break;

			strWinName = strCmdLine.Mid(nFirstDot+1, nSecondDot-nFirstDot-1);
		}
		while(false);
	}
	else 
	{
		// used by next key.
		nWinNameKey = 0;
	}

	int nAhchorKey = strCmdLine.Find(L"-a:", nWinNameKey);
	if(nAhchorKey >= 0)
	{
		do
		{
			int nFirstDot = strCmdLine.Find(L"\"", nAhchorKey);
			if(nFirstDot < 0) break;

			int nSecondDot = strCmdLine.Find(L"\"", nFirstDot+1);
			if(nSecondDot < 0) break;

			m_strAnchor = strCmdLine.Mid(nFirstDot+1, nSecondDot-nFirstDot-1);
		}
		while(false);
	}

	//---------------------------------------
	// Init help system
	new CHelperSystem;

	//---------------------------------------
	// create commucation dialog
	m_wndCommucation.Create(IDD_DIALOG_COM, 0);
	m_wndCommucation.SetWindowText(strWinName);

	//---------------------------------------
	// create main dialog
	m_pMainDialog = new CHelperDlg();
	
	//m_pMainDialog->Create(MAKEINTRESOURCE(IDD_HELPER_DIALOG));
	//m_pMainDialog->ShowWindow(SW_HIDE);
	m_pMainWnd = m_pMainDialog;

	//---------------------------------------
	// Go to anchor item
	static  HELPER_COMMAND cmd;
	ZeroMemory(&cmd, sizeof(HELPER_COMMAND));
	CString tmpuse=L"HelpStart";
	if(!(((CHelperApp*)AfxGetApp())->GetAnchor()==""))
	{
		tmpuse = ((CHelperApp*)AfxGetApp())->GetAnchor();
	}

	TCHAR szBuff[100];
	
	_stprintf(szBuff, _T("%s"), tmpuse);
	
	WideCharToMultiByte(CP_ACP, 0, szBuff, -1, cmd.pContents, MAX_PATH, NULL, NULL); 
	//_snprintf(cmd.pContents, MAX_PATH, "%s", (LPCTSTR)tmpuse);

	static COPYDATASTRUCT   cCopyData;
	ZeroMemory(&cCopyData, sizeof(COPYDATASTRUCT));

	cCopyData.dwData = 0;
	cCopyData.cbData = sizeof(unsigned char)*4 + strlen(cmd.pContents) + 1;
	cCopyData.lpData = &cmd;

	m_wndCommucation.SendMessage(WM_COPYDATA, 0, (LPARAM)&cCopyData);



	return TRUE;
}

int CHelperApp::ExitInstance()
{
	m_wndCommucation.DestroyWindow();

	delete m_pMainDialog; m_pMainDialog = 0;
	delete CHelperSystem::GetMe();

	return CWinApp::ExitInstance();
}

