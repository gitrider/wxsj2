
/** CrashReport.cpp : Defines the entry point for the application.
 */

#include "stdafx.h"
#include "resource.h"
#include "Util.h"
#include "Global.h"
//#include "StaticLink.h"
#include "XZip.h"
#include <shlobj.h>
#include "DumpFileAnsys.h"



INT_PTR CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(	HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPTSTR    lpCmdLine,
						int       nCmdShow)
{
	g_hInstance = hInstance;

	CHAR szFileName[MAX_PATH] = {0};
	GetModuleFileName(GetModuleHandle(NULL), szFileName, MAX_PATH);
	PathRemoveFileSpec(szFileName);
	PathAddBackslash(szFileName);
	strcpy(g_szCurrentDir, szFileName);

	// 分析命令行
	char* pMark1 = strchr(lpCmdLine, '"'); if(!pMark1) return 0;
	char* pMark2 = strchr(pMark1+1, '"');  if(!pMark2) return 0;

	char* pMark3 = strchr(pMark2+1, '"');  if(!pMark3) return 0;
	char* pMark4 = strchr(pMark3+1, '"');  if(!pMark4) return 0;

	char* pMark5 = strchr(pMark4+1, '"');  if(!pMark5) return 0;
	char* pMark6 = strchr(pMark5+1, '"');  if(!pMark6) return 0;

	*pMark2 = 0; _snprintf(g_szSmallInfoFile, MAX_PATH, "%s", pMark1+1);
	*pMark4 = 0; _snprintf(g_szBigInfoFile, MAX_PATH, "%s", pMark3+1);
	*pMark6 = 0; _snprintf(g_szDumpFile, MAX_PATH, "%s", pMark5+1);

//	MessageBox(NULL, g_szSmallInfoFile, "1", MB_OK);
//	MessageBox(NULL, g_szBigInfoFile, "2", MB_OK);
//	MessageBox(NULL, g_szDumpFile, "3", MB_OK);

	::CoInitialize(NULL);

//	AnsysDumpFile(g_szDumpFile);

	::DialogBox(hInstance, MAKEINTRESOURCE(IDD_ERROR_REPORT), NULL, MainDlgProc);
	::CoUninitialize();

	return 0;
}

// 显示错误信息
void SetSmallInfo(HWND hwndDlg, LPCTSTR szSmallInfoFile)
{
	HWND hSmallEdit = ::GetDlgItem(hwndDlg, IDC_SMALL_INFO);
	if(!hSmallEdit) 
		return;

	FILE* fp = fopen(szSmallInfoFile, "rb");
	if(!fp) 
		return;

	CHAR szTemp[1024] = {0};
	fread(szTemp, sizeof(CHAR), 1024,fp);
	fclose(fp); fp = NULL;

	if((BYTE)szTemp[0] == (BYTE)0XFF && (BYTE)szTemp[1] == (BYTE)0XFE)
	{
//----------------------
		CHAR szNormal[1024];
		WideCharToMultiByte(CP_ACP, 0, (WCHAR*)(szTemp+sizeof(WORD)), (int)wcslen((WCHAR*)szTemp), szNormal, 1024, NULL, FALSE);
		::SetWindowText(hSmallEdit, szNormal);
//----------------------
	}
	else
	{
		::SetWindowText(hSmallEdit, szTemp);
	}

}

// 发送报告
void SendReport(void)
{
	//-------------------------------------------------------
	CHAR szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	CHAR szCrashFile[MAX_PATH] = {0};
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	_snprintf(szCrashFile, MAX_PATH, "Error%02d_%02d_%02d_%02d.zip", timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute,timeNow.wSecond);

	//-------------------------------------------------------
	CHAR szLocalFile[MAX_PATH] = {0};
	strncpy(szLocalFile, szCurrentDir, MAX_PATH);
	PathAppend(szLocalFile, szCrashFile);
	HZIP hz = CreateZip((void *)szLocalFile, 0, ZIP_FILENAME);
	if(!hz) 
	{
		MessageBox(g_hMainWnd, "Create Error.zip error!", "ErrorReport", MB_OK|MB_ICONSTOP);
		return;
	}

	ZipAdd(hz, "dump.dmp", (void *)g_szDumpFile, 0, ZIP_FILENAME);
	ZipAdd(hz, "info.txt", (void *)g_szBigInfoFile, 0, ZIP_FILENAME);

	CHAR szLogFile[MAX_PATH];

	strncpy(szLogFile, szCurrentDir, MAX_PATH);
	PathAppend(szLogFile, "Engine.log");
	ZipAdd(hz, "Engine.log", (void *)szLogFile, 0, ZIP_FILENAME);

	strncpy(szLogFile, szCurrentDir, MAX_PATH);
	PathAppend(szLogFile, "CEGUI.log");
	ZipAdd(hz, "CEGUI.log", (void *)szLogFile, 0, ZIP_FILENAME);

	CloseZip(hz);

	//-------------------------------------------------------
	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = g_hMainWnd;
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "选择保存数据文件的目录";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR szReturn[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szReturn);

	//-------------------------------------------------------
	CHAR szSaveAs[MAX_PATH] = {0};
	strncpy(szSaveAs, szReturn, MAX_PATH);
	PathAppend(szSaveAs, szCrashFile);
	CopyFile(szLocalFile, szSaveAs, FALSE);
	
}


// 用记事本打开详细数据文件
void DetailInfo()
{
	CHAR szNotePad[MAX_PATH];

	GetWindowsDirectory(szNotePad, MAX_PATH);
	PathAppend(szNotePad, "notepad.exe");

	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s %s", szNotePad, g_szBigInfoFile);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	if(CreateProcess( NULL, szCmdLine, NULL, NULL, FALSE,CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) == FALSE)
	{
		MessageBox(NULL, "无法启动记事本!", NULL, MB_OK|MB_ICONSTOP);
	}
}


//用WinDbg调试Dmp文件
void DebugDump()
{
	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s -z %s -c\".ecxr\"", g_szWinDbg, g_szDumpFile);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	if(CreateProcess( NULL, szCmdLine, NULL, NULL, FALSE,CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) == FALSE)
	{
		MessageBox(NULL, "无法启动WinDbg!", NULL, MB_OK|MB_ICONSTOP);
	}
}


INT_PTR CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				// 直接关闭窗口
			case ID_CANCEL:
				PostQuitMessage(0);
				break;

				// 发送错误报告
			case IDC_SEND:
				SendReport();
				break;

				// 查看更多的信息
			case IDC_MORE_INFO:
				DetailInfo();
				break;
			}
			break;
		}
		break;

	case WM_INITDIALOG:
		{
			g_hMainWnd = hwndDlg;
			HICON hIcon = LoadIcon( g_hInstance, MAKEINTRESOURCE(IDI_ICON_ERROR) ) ;
			SendMessage( hwndDlg, WM_SETICON, ICON_BIG, ( LPARAM )hIcon ) ;
			SendMessage( hwndDlg, WM_SETICON, ICON_SMALL, ( LPARAM )hIcon ) ;

//			CreateLinkFont(hwndDlg);
			CenterDlg(hwndDlg);

			SetSmallInfo(hwndDlg, g_szSmallInfoFile);
/*
			static AX_URL_LNK g_BigInfoLnk = { 
					"查看详细数据", 
					"1", 
					NULL, TRUE};
			SetWindowLong(GetDlgItem(hwndDlg, IDC_STATIC_MORE), GWL_USERDATA, (LONG)(LONG_PTR)(&g_BigInfoLnk));
			g_BigInfoLnk.m_oldWndProc = (WNDPROC)(LONG_PTR)SetWindowLong(GetDlgItem(hwndDlg, IDC_STATIC_MORE), GWL_WNDPROC, (LONG)(LONG_PTR)StaticLnkWndProc);

			// 寻找WinDbg
			CHAR szWinDbg[MAX_PATH];
			DWORD dwType, dwSize = MAX_PATH;
			if(ERROR_SUCCESS != ::SHGetValue( HKEY_CURRENT_USER, "Software\\Microsoft\\DebuggingTools", "WinDbg", &dwType, szWinDbg, &dwSize) )
			{
				// 没有WinDbg
				EnableWindow(GetDlgItem(hwndDlg, IDC_STATIC_WINDBG), FALSE);
				break;
			}

			PathAppend(szWinDbg, "windbg.exe");
			_snprintf(g_szWinDbg, MAX_PATH, "%s", szWinDbg);

			static AX_URL_LNK g_WinDbgLnk = { 
					"调试(WinDbg)", 
					"2", 
					NULL, TRUE};
			SetWindowLong(GetDlgItem(hwndDlg, IDC_STATIC_WINDBG), GWL_USERDATA, (LONG)(LONG_PTR)(&g_WinDbgLnk));
			g_WinDbgLnk.m_oldWndProc = (WNDPROC)(LONG_PTR)SetWindowLong(GetDlgItem(hwndDlg, IDC_STATIC_WINDBG), GWL_WNDPROC, (LONG)(LONG_PTR)StaticLnkWndProc);
*/
		}
		break;

	case WM_CLOSE:
//		DeleteLinkFont();
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return FALSE;
}