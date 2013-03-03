#include "StdAfx.h"

#include "Global.h"
#include "ExDll.h"
#include "DownDialog.h"
#include "Resource.h"
#include "Util.h"

// Entry point for DLL
BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	g_hInstance=(HINSTANCE)hInst;
    return TRUE;
}

extern "C"
void __declspec(dllexport) ExtractSogouDll(HWND hwndParent, int string_size, 
								char *variables, stack_t **stacktop)
{
	EXDLL_INIT();


	//检查msvcp60是否存在(sogou需要)
	{
		char szMsvcp60[MAX_PATH] = {0};
		::GetSystemDirectory(szMsvcp60, MAX_PATH);
		::PathAppend(szMsvcp60, "msvcp60.dll");
		
		if(!::PathFileExists(szMsvcp60))
		{
			char szTempFile[MAX_PATH] = {0};
			::GetTempPath(MAX_PATH, szTempFile);
			::PathAppend(szTempFile, "msvcp60.zip");
			
			//解出msvcp60
			if(!ExtractResToFile(IDR_ZIP_MSVCP60, "ZIP", szTempFile)) return;

			//解出dll
			if(!ExtractFileFromZip(szTempFile, "msvcp60.dll", szMsvcp60)) return;
		}
	}

	char szCurrentDir[MAX_PATH] = {0};
	char szSogouExe[MAX_PATH] = {0};
	::GetModuleFileName(g_hInstance, szCurrentDir, MAX_PATH);
	::PathRemoveFileSpec(szCurrentDir);
	strncpy(szSogouExe, szCurrentDir, MAX_PATH);

	::PathAppend(szSogouExe, "sogou_p4p.exe");

#if 0
	MessageBox(hwndParent, szSogouExe, "", MB_OK);
#endif 

	//解开sogou安装程序
	if(!ExtractResToFile(IDR_EXE_SOGOU, "EXE", szSogouExe))
	{
		pushstring("false");
		return;
	}

	//保存路径
	strncpy(g_szSogouExe, szSogouExe, MAX_PATH);

	pushstring("true");
}

extern "C"
void __declspec(dllexport) ShowDownloadDialog(HWND hwndParent, int string_size, 
								char *variables, stack_t **stacktop)
{
	EXDLL_INIT();

	//file to download
	popstring(g_szDownURL);
	popstring(g_szInstallPath);

	DOWN_RESULT ret = ShowDownDialog();

	if(DR_SUCCESS == ret)
	{
		char szCurrentDir[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, szCurrentDir);

		//启动安装进程
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si,sizeof(STARTUPINFO));
		ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;

		char szCmdLine[MAX_PATH] = {0};
		_snprintf(szCmdLine, MAX_PATH, "%s /D=%s", g_szLocalFile, g_szInstallPath);

		//启动sogou安装进程
		::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 
			CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szCurrentDir, &si, &pi);
	}

}
