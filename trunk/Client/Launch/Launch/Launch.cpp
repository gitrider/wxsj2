// Launch.cpp : 定义应用程序的类行为。
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


// CLaunchApp 构造

CLaunchApp::CLaunchApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLaunchApp 对象

CLaunchApp theApp;


// CLaunchApp 初始化

BOOL CLaunchApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。

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
				BOOL bFlag = CreateProcess( RUN_FILENAME, 0, 0, 0, 0, 0, 0, 0, &si, &ri );//运行这个bin
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

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	return FALSE;
}
