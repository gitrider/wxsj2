// tConfig.cpp : ����Ӧ�ó��������Ϊ��
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


// CtConfigApp ����

CtConfigApp::CtConfigApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
    mExitCode = 0;

	//���ش��뻷��
	g_nCodePage = ::GetPrivateProfileIntA("Variable", "System_CodePage", 936, ".\\System.cfg");
}


// Ψһ��һ�� CtConfigApp ����
CtConfigApp theApp;
//ϵͳ����
//CSystemInfo	g_theSystemInfo;
HardwareInfoSystem gHardwareInfoSystem;

//�ͻ��������ļ�
CGameConfigManager g_theGameConfigs;
//�Ƿ���ͨ����Ϸ����
BOOL g_bRunFromGame = FALSE;
//�Ƿ��һ������
BOOL g_bFirstRun = FALSE;

BOOL CtConfigApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();
    
	//���ȼ��DirectX�汾��
	const wchar_t *szDxNeed = L"4.09.00.0904";
	std::string strDxNeed;

    // ��ȡdx�汾
    const wchar_t* szCurrentDXVersion = gHardwareInfoSystem.getDXVersion();
	if(szCurrentDXVersion[0]==0 || wcscmp(szDxNeed, szCurrentDXVersion) > 0)
	{
		CDirectXErrorDlg dlg;
		dlg.DoModal();
        mExitCode = 0xC0DE;
        return FALSE;
	}

	const char* szCmdLine = GetCommandLineA();

	//����Ϸ����
	if(strstr(szCmdLine, "-fg") != 0) g_bRunFromGame = TRUE;

    // ��ȡ����Ӳ����Ϣ
    gHardwareInfoSystem.initSystem();

    // �жϵ�ǰ�Դ��Ƿ��㹻
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

    // ����Կ������汾��
    //const std::string& validateInfoString = gHardwareInfoSystem.validateVideoDriver();

    //if (false == validateInfoString.empty())
    //{
    //    MessageBox(NULL, validateInfoString.c_str(), "�������˲������ó���", MB_OK | MB_ICONERROR);
    //}

	//��һ������
	DWORD dwType, dwSize=sizeof(DWORD), dwValue=0;
	SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\SOHU\\FireFox\\TLBB", "RunConfig", 
		&dwType, &dwValue, &dwSize);

	g_bFirstRun = (dwValue != 1);

	//д���б�־
	dwValue = 1;
	SHSetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\SOHU\\FireFox\\TLBB", 
		"RunConfig", REG_DWORD, &dwValue, sizeof(DWORD));

	//����Ϸ���ж���Ҳ���ǵ�һ�����У��˳�
	if(g_bRunFromGame && !g_bFirstRun) return FALSE;

	//����Ƽ�����
	FillCommendCfg();

	//����ǵ�һ������, ɾ�����������ļ�
	if(g_bFirstRun)
	{
		g_theGameConfigs.DeletePrivateFile();
	}

	//�������������ļ�
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
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
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
