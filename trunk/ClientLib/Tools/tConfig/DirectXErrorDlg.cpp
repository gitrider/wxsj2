// DirectXErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tConfig.h"
#include "DirectXErrorDlg.h"
#include ".\directxerrordlg.h"
#include "StaticLink.h"
#include "TDUnicodeLayer.h"
#include "InnerString.h"

#include <dsetup.h>

typedef INT (WINAPI * LPDIRECTXSETUPSETCALLBACK)( DSETUP_CALLBACK Callback );
typedef INT (WINAPI * LPDIRECTXSETUPGETVERSION)(DWORD *lpdwVersion, DWORD *lpdwMinorVersion);
#ifdef UNICODE
typedef INT (WINAPI * LPDIRECTXSETUP)( HWND hWnd, LPWSTR lpszRootPath, DWORD dwFlags );
#else
typedef INT (WINAPI * LPDIRECTXSETUP)( HWND hWnd, LPSTR lpszRootPath, DWORD dwFlags );
#endif 

/// dxsetup����Ľӿ�
static HMODULE                   s_hModDSetup = NULL;
static LPDIRECTXSETUPSETCALLBACK s_DirectXSetupSetCallback = NULL;
static LPDIRECTXSETUP            s_DirectXSetup = NULL;

// CDirectXErrorDlg dialog

IMPLEMENT_DYNAMIC(CDirectXErrorDlg, CDialog)
CDirectXErrorDlg::CDirectXErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirectXErrorDlg::IDD, pParent)
{
}

CDirectXErrorDlg::~CDirectXErrorDlg()
{
}

void CDirectXErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDirectXErrorDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_SETUP, OnBnClickedButtonSetup)
END_MESSAGE_MAP()


// CDirectXErrorDlg message handlers

BOOL CDirectXErrorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CDirectXErrorDlg::InitDirectSetupAPIs()
{
    if( s_hModDSetup != NULL )
        return true;

    s_hModDSetup = LoadLibrary( TEXT("..\\Directx9\\dsetup.dll") );
    if( s_hModDSetup == NULL ) 
        return false;

    s_DirectXSetupSetCallback = (LPDIRECTXSETUPSETCALLBACK)GetProcAddress( s_hModDSetup, "DirectXSetupSetCallback" );

#ifdef UNICODE
    s_DirectXSetup = (LPDIRECTXSETUP)GetProcAddress( s_hModDSetup, "DirectXSetupW" );
#else
    s_DirectXSetup = (LPDIRECTXSETUP)GetProcAddress( s_hModDSetup, "DirectXSetupA" );
#endif 

    return true;
}

DWORD WINAPI DirectXSetupCallbackFunction( DWORD dwReason, DWORD dwMsgType, 
                                          LPSTR strMessage, LPSTR strName, 
                                          VOID* pInfo )
{   
    return IDOK;
}

void CDirectXErrorDlg::OnBnClickedButtonSetup()
{
    // TODO: Add your control notification handler code here

    /** ��һ�ַ������ֶ�����dxsetup�ӿ�
    if (!InitDirectSetupAPIs())
    {
        ::MessageBox(NULL, "�޷��ҵ�dsetup.dll��", "�������˲������ó���", MB_OK);
        return;
    }
  
    s_DirectXSetupSetCallback( (DSETUP_CALLBACK)DirectXSetupCallbackFunction );

    //dwFlags = DSETUP_DIRECTX;
   
    int retCode = s_DirectXSetup( GetSafeHwnd(), "..\\Directx9", DSETUP_DIRECTX );

    switch (retCode)
    {
    case DSETUPERR_SUCCESS:
        {
            ::MessageBox(NULL, "��װ�ɹ���", "�������˲������ó���", MB_OK);
            break;
        }

    case DSETUPERR_SUCCESS_RESTART:
        {
            ::MessageBox(NULL, "�޷��ҵ�dsetup.dll!", "�������˲������ó���", MB_OK);
            break;
        }

    default:
        {
            char errorStr[64];
            _snprintf(errorStr, 64, "��װ���󣬴������ ��%d", retCode);

            ::MessageBox(NULL, errorStr, "�������˲������ó���", MB_OK);

            break;
        }
    }
    */

    // �ڶ��ַ���������dxsetup.exe
    //���ɽ���
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si,sizeof(STARTUPINFOA));
    ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOWNORMAL;

    //��������
    if(!::CreateProcessA(NULL, "..\\Directx9\\DXSETUP.exe", NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi))
    {
        TD_MessageBoxW(NULL, L"Can't find dxsetup.exe!", getInnerString(INNER_TITLE), MB_OK);
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    //��ⷵ��ֵ
 //   DWORD dwExitCode;
 //   GetExitCodeProcess(pi.hProcess, &dwExitCode);

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}
