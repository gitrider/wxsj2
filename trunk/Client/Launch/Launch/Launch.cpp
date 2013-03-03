// Launch.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Launch.h"
#include "LaunchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CONST CString LOG_FILENAME = "LaunchBin.log";
CONST CString EXE_FILENAME = "Launch.exe";
CONST CString RUN_FILENAME = "Launch.bin";
// CLaunchApp

BEGIN_MESSAGE_MAP(CLaunchApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLaunchApp ����

CLaunchApp::CLaunchApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLaunchApp ����

CLaunchApp theApp;


// CLaunchApp ��ʼ��

BOOL CLaunchApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�

#ifndef _DEBUG
	FILE* fp;
	fp = fopen( LOG_FILENAME, "w" );
	char szModuleName[1024];
	GetModuleFileName( NULL, szModuleName, 1024 );
	CString strModule = PathFindFileName( szModuleName );
	DWORD dwTimes = 0;
	if( strModule.CompareNoCase( EXE_FILENAME ) == 0 )
	{
		if( PathFileExists( RUN_FILENAME ) )
		{
			while( true ) 
			{
				SetFileAttributes( RUN_FILENAME, FILE_ATTRIBUTE_NORMAL );
				if( DeleteFile( RUN_FILENAME ) == FALSE ) 
				{
					Sleep( 10 );
					if( fp ) fprintf( fp, "DeleteFile %s Failed!%d\n", RUN_FILENAME, GetLastError() );
					if( dwTimes ++ == 10 ) 
					{
						if( fp ) fclose( fp );
						return FALSE;
					}
					continue;
				}
				break;
			}
		}

		dwTimes = 0;
		while( true ) 
		{
			if( CopyFile( EXE_FILENAME, RUN_FILENAME, FALSE ) ) 
			{
				STARTUPINFO si = {0};
				si.cb = sizeof( STARTUPINFO );
				si.dwFlags = STARTF_FORCEONFEEDBACK;
				PROCESS_INFORMATION ri = {0};
				BOOL bFlag = CreateProcess( RUN_FILENAME, 0, 0, 0, 0, 0, 0, 0, &si, &ri );//�������bin
				if( bFlag == FALSE ) 
				{
					if( fp ) fprintf( fp, "CreateProcess %s Failed!%d\n", RUN_FILENAME, GetLastError() );
				}
				if( fp ) 
					fclose( fp );
				return FALSE;
			}
			Sleep( 10 );
			if( fp ) fprintf( fp, "CopyFile %s Failed!%d\n", RUN_FILENAME, GetLastError() );
			if( dwTimes ++ == 10 ) 
			{
				if( fp )
					fclose( fp );
				return FALSE;
			}
		}
	}
	if( fp ) 
		fclose( fp );
#endif

	Sleep( 100 );
	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	//Display Main UI
	CLaunchDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
