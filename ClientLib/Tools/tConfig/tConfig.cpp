// tConfig.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "tConfig.h"
#include "tConfigDlg.h"
#include "DirectXErrorDlg.h"
#include "Util.h"
#include "DefConfig.h"
#include "InnerString.h"
#include "TDUnicodeLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtConfigApp

BEGIN_MESSAGE_MAP(CtConfigApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CtConfigApp 构造

CtConfigApp::CtConfigApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
    mExitCode = 0;

	//加载代码环境
	g_nCodePage = ::GetPrivateProfileIntA("Variable", "System_CodePage", 936, ".\\System.cfg");
}


// 唯一的一个 CtConfigApp 对象
CtConfigApp theApp;
//系统配置
//CSystemInfo	g_theSystemInfo;
HardwareInfoSystem gHardwareInfoSystem;

//客户端配置文件
CGameConfigManager g_theGameConfigs;
//是否是通过游戏运行
BOOL g_bRunFromGame = FALSE;
//是否第一次运行
BOOL g_bFirstRun = FALSE;

BOOL CtConfigApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();
    
	//首先检测DirectX版本号
	const wchar_t *szDxNeed = L"4.09.00.0904";
	std::string strDxNeed;

    // 获取dx版本
    const wchar_t* szCurrentDXVersion = gHardwareInfoSystem.getDXVersion();
	if(szCurrentDXVersion[0]==0 || wcscmp(szDxNeed, szCurrentDXVersion) > 0)
	{
		CDirectXErrorDlg dlg;
		dlg.DoModal();
        mExitCode = 0xC0DE;
        return FALSE;
	}

	const char* szCmdLine = GetCommandLineA();

	//从游戏运行
	if(strstr(szCmdLine, "-fg") != 0) g_bRunFromGame = TRUE;

    // 获取其他硬件信息
    gHardwareInfoSystem.initSystem();

    // 判断当前显存是否足够
    const float RequiredVideoMemory = 32.0f;
    const float currentFreeVideoMemory = gHardwareInfoSystem.getHardwareInfo().mFreeLocalVideoMemory;
    if ( currentFreeVideoMemory < RequiredVideoMemory )
    {
		wchar_t errorInfoStr[MAX_PATH]={0};

        _snwprintf(errorInfoStr, MAX_PATH, 
			getInnerString(INNER_LOW_VIDEO_MEMORY),
            currentFreeVideoMemory, RequiredVideoMemory );

        if ( IDNO == TD_MessageBoxW( NULL, errorInfoStr, getInnerString(INNER_TITLE), MB_YESNO|MB_ICONQUESTION ) )
        {
            mExitCode = 0xC0DE;
            return FALSE;
        }        
    }

    // 检测显卡驱动版本号
    //const std::string& validateInfoString = gHardwareInfoSystem.validateVideoDriver();

    //if (false == validateInfoString.empty())
    //{
    //    MessageBox(NULL, validateInfoString.c_str(), "《天龙八部》配置程序", MB_OK | MB_ICONERROR);
    //}

	//第一次运行
	DWORD dwType, dwSize=sizeof(DWORD), dwValue=0;
	SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\SOHU\\FireFox\\TLBB", "RunConfig", 
		&dwType, &dwValue, &dwSize);

	g_bFirstRun = (dwValue != 1);

	//写运行标志
	dwValue = 1;
	SHSetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\SOHU\\FireFox\\TLBB", 
		"RunConfig", REG_DWORD, &dwValue, sizeof(DWORD));

	//从游戏运行而且也不是第一次运行，退出
	if(g_bRunFromGame && !g_bFirstRun) return FALSE;

	//填充推荐配置
	FillCommendCfg();

	//如果是第一次运行, 删除个人配置文件
	if(g_bFirstRun)
	{
		g_theGameConfigs.DeletePrivateFile();
	}

	//加载所有配置文件
	g_theGameConfigs.LoadAllConfigFile();
	if(!g_theGameConfigs.GetLastestConfig())
	{
		TD_MessageBoxW(NULL, getInnerString(INNER_CANT_OPEN), getInnerString(INNER_TITLE), MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	CtConfigDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	return FALSE;

}

int CtConfigApp::ExitInstance()
{
    int retCode = CWinApp::ExitInstance();

    if (mExitCode != 0xC0DE)
        return retCode;
    else
        return mExitCode;
}
