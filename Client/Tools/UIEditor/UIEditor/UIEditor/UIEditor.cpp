// UIEditor.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CUIEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CUIEditorApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_NEW_PRJ, &CUIEditorApp::OnFileNewPrj)
	ON_COMMAND(ID_FILE_NEW_DESIGN_PRJ, &CUIEditorApp::OnFileNewDesignPrj)
	ON_COMMAND(ID_OPEN_DESIGN_PRJ, &CUIEditorApp::OnOpenDesignPrj)
END_MESSAGE_MAP()


// CUIEditorApp 构造

CUIEditorApp::CUIEditorApp()
{
	m_needCallSkinExit = FALSE;
}


// 唯一的一个 CUIEditorApp 对象

CUIEditorApp theApp;


// CUIEditorApp 初始化

BOOL CUIEditorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
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
	
	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	//加载插件在主窗口创建之前
	//if (LoadPlugin() == FALSE)
	//{
	//	return FALSE;
	//}

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(16);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUIEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CUIEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	// 启用拖/放
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

	//显示标题
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
	//	MessageBox(NULL, _T("没有可用的插件！"), _T("提示"), MB_OK);
	//	return FALSE;
	//}
	//PFUNEXCHANGEINTERFACE pfnExchangInterface;
	//HMODULE dll = LoadLibraryA(path);
	//if (dll == NULL)
	//{
	//	MessageBox(NULL, _T("无法加载插件！"), _T("提示"), MB_OK);
	//	return FALSE;
	//}
	//pfnExchangInterface = (PFUNEXCHANGEINTERFACE)GetProcAddress(dll, "ExchangInterface");
	//if (pfnExchangInterface == NULL)
	//{
	//	MessageBox(NULL, _T("无法找到插件导出函数！"), _T("提示"), MB_OK);
	//	return FALSE;
	//}
	//UINT *nWho = (UINT*) GetProcAddress(dll, "nWho");
	//if (!nWho || (nWho && *nWho != UIEDITOR_ID))
	//{
	//	MessageBox(NULL, _T("当前插件不适合编辑器！"), _T("提示"), MB_OK);
	//	return FALSE;
	//}
	//int ver = pfnExchangInterface(&m_pRender, &m_pSystem, &m_pAction, &m_pImage, &m_uiEiditor);
	//if (ver != UIEDITOR_VERSION)
	//{
	//	MessageBox(NULL, _T("插件接口版本不正确！"), _T("提示"), MB_OK);
	//	return FALSE;
	//}

	////调用初始化函数
	//if (m_pAction == NULL || m_pRender == NULL || m_pImage == NULL || m_pSystem == NULL)
	//{
	//	MessageBox(NULL, _T("无法得到必须的接口函数！"), _T("提示"), MB_OK);
	//	return FALSE;
	//}
	//
	//if( false == m_pSystem->Initialize(AfxGetInstanceHandle()) )
	//{
	//	MessageBox(NULL, _T("初始化插件失败！"), _T("提示"), MB_OK);
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

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CUIEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CUIEditorApp 消息处理程序
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
		//在状态栏显示
		CHAR szMouseText[MAX_PATH] = {0};
		sprintf(szMouseText, "程序出现错误%s",e.getMessage().c_str());
		((CMainFrame*)AfxGetMainWnd())->SetStatusText(1,szMouseText);
	}
	return TRUE;
}
