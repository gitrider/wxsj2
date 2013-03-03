// UIEditor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UIEditor.h"
#include "MainFrm.h"


#include "UIEditorDoc.h"
#include "UIEditorView.h"
#include "DataPool.h"
#include "Version.h"
#include "CEGUIExceptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIEditorApp

BEGIN_MESSAGE_MAP(CUIEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CUIEditorApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CUIEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CUIEditorApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_NEW_PRJ, &CUIEditorApp::OnFileNewPrj)
	ON_COMMAND(ID_FILE_NEW_DESIGN_PRJ, &CUIEditorApp::OnFileNewDesignPrj)
	ON_COMMAND(ID_OPEN_DESIGN_PRJ, &CUIEditorApp::OnOpenDesignPrj)
END_MESSAGE_MAP()


// CUIEditorApp ����

CUIEditorApp::CUIEditorApp()
{
	m_needCallSkinExit = FALSE;
}


// Ψһ��һ�� CUIEditorApp ����

CUIEditorApp theApp;


// CUIEditorApp ��ʼ��

BOOL CUIEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	AfxInitRichEdit2();

	CWinApp::InitInstance();

	m_ShowConsole = GetPrivateProfileIntA("System", "ShowConsole", 0, "./UIEditor.inf");
	if (m_ShowConsole)
	{
		AllocConsole();
		FILE* file;
		freopen_s(&file, "CONOUT$", "w", stdout); 
		SetConsoleTitle(_T("UIEditor Console"));
	}
	
	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	//���ز���������ڴ���֮ǰ
	//if (LoadPlugin() == FALSE)
	//{
	//	return FALSE;
	//}

	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(16);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUIEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CUIEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();

	//char kin[100];
	//GetPrivateProfileStringA("Skin", "LastSelected", "./kin/kin00.skin", kin, 100, "./UIEditor.inf");
	//////////////////////////////////////////////////////////
	//InitSkinMagicLib( AfxGetInstanceHandle(),"DMJCI" ,NULL,NULL );
	////	LoadSkinFile( (char*)(LPCSTR)("kin/kin01.data" ));
	//LoadSkinFile( kin);
	////MessageBoxA(NULL, kin, "Test", MB_OK);
	//SetDialogSkin( "Dialog" );

	//SetWindowSkin(AfxGetMainWnd()->GetSafeHwnd(), "MainFrame");
	//EnableWindowScrollbarSkin(AfxGetMainWnd()->GetSafeHwnd(), SB_BOTH);
	//INT index ;
	//sscanf_s(kin, "./kin/kin%02d.skin", &index);
	//((CMainFrame*)m_pMainWnd)->SetSkinIndex(index);
	m_needCallSkinExit = TRUE;

	//��ʾ����
	CString szTitle = "UIEditor  ";
	szTitle += UI_EDITOR_VERSION;
	m_pMainWnd->SetWindowText(szTitle);

	g_DataPool.Initialize();
	g_luaDlg->Initailzie();
	g_xmlViewDlg->Initailzie();
	g_toolBarDlg->InsertToolWindowToList();
	g_leftTreeImage->Initialize();

	return TRUE;
}

BOOL CUIEditorApp::LoadPlugin()
{
	//char path[MAX_PATH];
	//memset(path, 0, sizeof(path));
	//GetPrivateProfileStringA("Plugin", "Name", "", path, MAX_PATH, "./UIEditor.inf");
	//if (strlen(path) <= 0)
	//{
	//	MessageBox(NULL, _T("û�п��õĲ����"), _T("��ʾ"), MB_OK);
	//	return FALSE;
	//}
	//PFUNEXCHANGEINTERFACE pfnExchangInterface;
	//HMODULE dll = LoadLibraryA(path);
	//if (dll == NULL)
	//{
	//	MessageBox(NULL, _T("�޷����ز����"), _T("��ʾ"), MB_OK);
	//	return FALSE;
	//}
	//pfnExchangInterface = (PFUNEXCHANGEINTERFACE)GetProcAddress(dll, "ExchangInterface");
	//if (pfnExchangInterface == NULL)
	//{
	//	MessageBox(NULL, _T("�޷��ҵ��������������"), _T("��ʾ"), MB_OK);
	//	return FALSE;
	//}
	//UINT *nWho = (UINT*) GetProcAddress(dll, "nWho");
	//if (!nWho || (nWho && *nWho != UIEDITOR_ID))
	//{
	//	MessageBox(NULL, _T("��ǰ������ʺϱ༭����"), _T("��ʾ"), MB_OK);
	//	return FALSE;
	//}
	//int ver = pfnExchangInterface(&m_pRender, &m_pSystem, &m_pAction, &m_pImage, &m_uiEiditor);
	//if (ver != UIEDITOR_VERSION)
	//{
	//	MessageBox(NULL, _T("����ӿڰ汾����ȷ��"), _T("��ʾ"), MB_OK);
	//	return FALSE;
	//}

	////���ó�ʼ������
	//if (m_pAction == NULL || m_pRender == NULL || m_pImage == NULL || m_pSystem == NULL)
	//{
	//	MessageBox(NULL, _T("�޷��õ�����Ľӿں�����"), _T("��ʾ"), MB_OK);
	//	return FALSE;
	//}
	//
	//if( false == m_pSystem->Initialize(AfxGetInstanceHandle()) )
	//{
	//	MessageBox(NULL, _T("��ʼ�����ʧ�ܣ�"), _T("��ʾ"), MB_OK);
	//	return FALSE;
	//}
	return TRUE;
}

int CUIEditorApp::ExitInstance( )
{
	//if (m_needCallSkinExit)
	//{
	//	ExitSkinMagicLib();
	//}
	
	if (m_ShowConsole)
	{
		FreeConsole();
	}
	return 0;
}

void CUIEditorApp::OnFileNew()
{
		CWinApp::OnFileNew();
}
void CUIEditorApp::OnFileOpen()
{
	CWinApp::OnFileOpen();
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CUIEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CUIEditorApp ��Ϣ�������
void CUIEditorApp::OnFileNewPrj()
{
	//theApp.m_pAction->CreateProject(EPT_LAYOUT);
}

void CUIEditorApp::OnFileNewDesignPrj()
{
	//theApp.m_pAction->CreateProject(EPT_DESIGN);
}

void CUIEditorApp::OnOpenDesignPrj()
{
	
}

void CUIEditorApp::OnOpenPrj()
{
	
}

BOOL CUIEditorApp::OnIdle(LONG lCount)
{
	if (CWinApp::OnIdle(lCount))
		return TRUE;   

	try
	{
		if (g_pEditorView)
		{
			CUIEditorView::TimerProc(NULL, 0, 0, 0);
		}
	}
	catch (CEGUI::Exception& e)
	{
		//��״̬����ʾ
		CHAR szMouseText[MAX_PATH] = {0};
		sprintf(szMouseText, "������ִ���%s",e.getMessage().c_str());
		((CMainFrame*)AfxGetMainWnd())->SetStatusText(1,szMouseText);
	}
	return TRUE;
}
