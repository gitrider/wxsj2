// BuildTools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <conio.h>
#include <ctype.h>
#include <process.h>

#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <zzip/zzip.h>

#include "Resource.h"
#include "DataBase\WXSJ_DBC.h"
#include <string>

using namespace DBC;
//--------------------------
bool g_bYesAll = false;		//自动执行所有步骤

char g_szVSS1Dir[MAX_PATH] = {0};	//外网资源目录

//--------------------------

bool choice(const char* szMessage)
{
	if(g_bYesAll) return true;

	printf("%s[Y/N]:", szMessage);
	char cInput;

	do
	{
		cInput = _getch();
		cInput = toupper(cInput);
	} while(cInput != 'Y' && cInput != 'N');
	printf("%c\n", cInput);

	if(cInput=='Y') return true;
	else return false;
}

void exit_error(const char* szMessage, ...)
{
	CHAR szTemp[1024] = {0};
	va_list ptr; va_start(ptr, szMessage);
	_vsnprintf((char *)szTemp, 1024, szMessage, ptr);
	va_end(ptr);

	printf("----------------  ERROR  -------------------\n");
	printf("%s\n", szTemp);
	system("pause");
	ExitProcess(0);
}

//-------------------------------------
// 清空本地的开发版和发行版目录
//-------------------------------------
void do_setp_0(void)
{
	system("rmdir Develop /S /Q");
	system("rmdir Develop2 /S /Q");
	system("rmdir RTM /S /Q");
}

//-------------------------------------
// 拷贝外网资源
//-------------------------------------
void do_step_1(void)
{
	char szCmd[MAX_PATH] = {0};

	//CreateDirectory("Develop", 0);

	//Copy All Bin
	//CreateDirectory("Develop", 0);
	printf("Export %s\\Client -> Client...\n", g_szVSS1Dir);
	_snprintf(szCmd, MAX_PATH, "svn export %s Develop", g_szVSS1Dir);
	system(szCmd);

	////Copy Public
	//CreateDirectory("Develop\\Public", 0);
	//printf("Export %s\\Public -> Public...\n", g_szVSS1Dir);
	//_snprintf(szCmd, MAX_PATH, "svn export %s\\Public Develop\\Public", g_szVSS1Dir);
	//system(szCmd);

	////Copy Server
	//CreateDirectory("Develop\\Server", 0);
	//printf("Copy %s\\Server -> Server...\n", g_szVSS1Dir);
	//_snprintf(szCmd, MAX_PATH, "xcopy %s\\Server Develop\\Server /S /Q", g_szVSS1Dir);
	//system(szCmd);

	////Copy SqlScript
	//CreateDirectory("Develop\\SqlScript", 0);
	//printf("Copy %s\\SqlScript -> SqlScript...\n", g_szVSS1Dir);
	//_snprintf(szCmd, MAX_PATH, "xcopy %s\\SqlScript Develop\\SqlScript /S /Y", g_szVSS1Dir);
	//system(szCmd);

	////Copy VietnamPatch
	//char szViPatch[MAX_PATH]={0};
	//_snprintf(szViPatch, MAX_PATH, "%s\\VietnamPatch", g_szVSS1Dir);
	//if(::PathFileExists(szViPatch))
	//{
	//	CreateDirectory("Develop\\VietnamPatch", 0);
	//	printf("Copy %s\\VietnamPatch -> VietnamPatch...\n", g_szVSS1Dir);
	//	_snprintf(szCmd, MAX_PATH, "xcopy %s\\VietnamPatch Develop\\VietnamPatch /S /Y", g_szVSS1Dir);
	//	system(szCmd);
	//}
}

//-------------------------------------
//拷贝内网资源
//-------------------------------------
void do_step_2(void)
{
	//DUMMY...
}

//-------------------------------------
// 删除无用的目录和文件， 以及去掉文件只读属性
//-------------------------------------
void do_step_3(void)
{
	system("rmdir Develop\\Client\\resource\\building_collision /S /Q");
	system("rmdir Develop\\Client\\ImageSetEditor /S /Q");
	system("rmdir Develop\\Client\\UIEditor /S /Q");
	system("del /F /S Develop\\Client\\Tools /S /Q");


	system("del /F /S Develop\\_desktop.ini");
	system("del /F /S Develop\\Client\\*.log");
	system("del /F /S Develop\\Client\\*.xxx");

	printf("remove file readonly attribute\n");
	system("attrib -R +A Develop\\*.* /S /D");
}

//-------------------------------------
// 生成Fairy资源整理工具
//-------------------------------------
void do_step_4(void)
{
	char szCurrent[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrent);

	char szToolsDir[MAX_PATH] = {0};
	strncpy(szToolsDir, szCurrent, MAX_PATH);
	PathAppend(szToolsDir, "Develop\\Client\\_build_tools"); 
	CreateDirectory(szToolsDir, 0);

	HRSRC hRes = ::FindResource(NULL,MAKEINTRESOURCE(IDR_ZIP1), "ZIP");
	DWORD nLen = ::SizeofResource(NULL, hRes);
	HGLOBAL hTable = ::LoadResource(0,hRes);
	LPWORD pTable = (LPWORD)::LockResource(hTable);

	if(pTable && hRes)
	{
		//从资源中解出
		CHAR szTempPath[MAX_PATH];
		CHAR szTempFileName[MAX_PATH];
		::GetTempPath(MAX_PATH, szTempPath);
		::GetTempFileName(szTempPath, "~", MAX_PATH, szTempFileName);

		FILE* fp;
		if((fp = fopen(szTempFileName, "wb")) == 0)
		{
			exit_error("Can't create temp file!");
		}
		fwrite(pTable, 1, nLen, fp);
		fclose(fp);
		UnlockResource(pTable); 
		FreeResource(hTable); 

		//解压文件
		const char* szFilesList[] = 
		{
			//"Bin/sogou_pinyin_tianlong_3025.exe",
			"Bin/InstallHelp.exe",
			"FairyUtilTool.exe",
			"FT_Clean_0.cfg",
			"FT_DDS_0.cfg",
			"FT_DDS_1.cfg",
			"FT_DDS_2.cfg",
			"FT_DDS_3.cfg",
			"FT_Merge.cfg",
			"OgreMain.dll",
			"OgrePlatform.dll",
			"d3dx9_32.dll",
			"AxpUtil.exe",
			"ResourcePak.lua",
			"PatchMake.lua",
			"SkeletonMerge.lua",
			"LuaTool.exe",
			"System.cfg",
			"LoginServer.txt",
			"FairyResources.cfg",
			"Launch.exe",
			"Launch.ini",
			"license.txt",
			//"Helper.exe",
			"Uninstall.exe",
			"UpdateExeHead.exe",
		};

		//创建可能需要的子目录
		char szBinPath[MAX_PATH] = {0};
		strncpy(szBinPath, szToolsDir, MAX_PATH);
		::PathAppend(szBinPath, "Bin");
		::CreateDirectory(szBinPath, 0);

		zzip_error_t zzipError;
		ZZIP_DIR* mZzipDir = zzip_dir_open(szTempFileName, &zzipError );
		if (zzipError != ZZIP_NO_ERROR) 
		{
			exit_error("Can't open zip file![%s]", szTempFileName);
		}

		for(int i=0; i<(int)(sizeof(szFilesList)/sizeof(const char*)); i++)
		{
			const char* szFileName = szFilesList[i];

			ZZIP_FILE* zzipFile = zzip_file_open( mZzipDir, szFileName, ZZIP_ONLYZIP | ZZIP_CASELESS );
			if( zzipFile == 0 ) 
			{
				exit_error("Can't open file in zip[%s]!", szFileName);
			}

			ZZIP_STAT zstat;
			int zip_err = zzip_dir_stat(mZzipDir, szFileName, &zstat, ZZIP_CASEINSENSITIVE);
			if( zip_err != 0 )
			{
				zzip_file_close( zzipFile );
				zzip_dir_close( mZzipDir );
				exit_error("Can't open file in zip[%s]", szFileName);
			}
			LPSTR lpFileMemory = new CHAR[zstat.st_size];
			if(!lpFileMemory) 
			{
				zzip_file_close( zzipFile );
				zzip_dir_close( mZzipDir );
				delete[] lpFileMemory; lpFileMemory = 0;
				exit_error("allocal memory error[%d]", zstat.st_size);
			}
			zip_err = zzip_file_read( zzipFile, lpFileMemory, zstat.st_size );
			if( zip_err != zstat.st_size )
			{
				zzip_file_close( zzipFile );
				zzip_dir_close( mZzipDir );
				delete [] lpFileMemory; lpFileMemory = 0;
				exit_error("Can't read file in zip:%s", szFileName);
			}

			//create new file
			char szTargetFile[MAX_PATH] = {0};
			strncpy(szTargetFile, szToolsDir, MAX_PATH);
			PathAppend(szTargetFile, szFileName);
			FILE* fp = fopen(szTargetFile, "wb");
			if(!fp)
			{
				exit_error("Can't create file: %s", szTargetFile);
			}
			fwrite(lpFileMemory, 1, zstat.st_size, fp);
			fclose(fp);

			delete[] lpFileMemory; lpFileMemory = 0;
			zzip_file_close( zzipFile );
		}
		zzip_dir_close( mZzipDir );

		//删掉ZIP文件
		DeleteFile(szTempFileName);
	}
}

//-------------------------------------
// 运行脚本整理工具
//-------------------------------------
void do_step_5(void)
{
	//生成Lua工具
	char szCurrent[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrent);

	char szToolsDir[MAX_PATH];
	strncpy(szToolsDir, szCurrent, MAX_PATH);
	PathAppend(szToolsDir, "Develop\\Client\\_build_tools");

	char szToolsFile[MAX_PATH];
	strncpy(szToolsFile, szToolsDir, MAX_PATH);
	PathAppend(szToolsFile, "LuaTool.exe");

	//open Script.tab
	DBCFile file(0);
	char szTabFile[MAX_PATH];
	strncpy(szTabFile, szCurrent, MAX_PATH); 
	PathAppend(szTabFile, "Develop\\Public\\Data\\Script.tab");

	file.OpenFromTXT(szTabFile);
	//make Script.dat
	struct ScriptTab
	{
		int nID;
		const char* szPath;
	};
	int cnt = file.GetRecordsNum();
	std::string scriptContent = "MGR_TXT\n";
	char szTemp[MAX_PATH];
	for (register int i=0; i<cnt; i++)
	{
		ScriptTab* pTab = (ScriptTab* )file.Search_Posistion(i, 0);
		if (pTab)
		{
			sprintf(szTemp, "%d=%s\n", pTab->nID, pTab->szPath);
			scriptContent += szTemp;
		}
	}

	//拷贝一份
	char szScriptSrc[MAX_PATH];
	//先删除
	sprintf(szScriptSrc, "rmdir /S/Q %s\\Data\\", szToolsDir);
	system(szScriptSrc);
	//后拷贝
	sprintf(szScriptSrc, "xcopy /S %s\\Develop\\Public\\Data\\Script %s\\Data\\", szCurrent, szToolsDir);
	system (szScriptSrc);

	//写入Script.dat
	char szDestFile[MAX_PATH];
	strncpy(szDestFile, szToolsDir, MAX_PATH); 
	PathAppend(szDestFile, "Data\\Script.dat");

	FILE* pfile = fopen(szDestFile, "w");
	if (pfile)
	{
		fwrite(scriptContent.c_str(), scriptContent.size(), 1, pfile);
		fclose(pfile);
	}

	//Delete org script.tab
	UINT dwFileAttr = GetFileAttributes(szDestFile);
	SetFileAttributes(szDestFile, dwFileAttr&(~FILE_ATTRIBUTE_READONLY));
	DeleteFile(szTabFile);
	
	//生成进程
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	strncpy(szDestFile, szCurrent, MAX_PATH); 
	PathAppend(szDestFile, "Develop\\Public\\Data");

	//删除
	sprintf(szScriptSrc, "rmdir /S/Q %s\\Script", szDestFile);
	system(szScriptSrc);
	//SHFILEOPSTRUCT shf;
	//memset(&shf,0,sizeof(SHFILEOPSTRUCT));
	//shf.hwnd = NULL;
	//shf.pFrom = szDestFile;
	//shf.wFunc = FO_DELETE;
	//shf.fFlags = FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT;
	//SHFileOperation(&shf);

	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s -input\"%s\\Data\\Script.dat\" -output\"%s\"", 
		szToolsFile, szToolsDir ,szDestFile);

	//启动进程
	if(!::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szToolsDir, &si, &pi))
	{
		exit_error("Can't create process\n%s", szCmdLine);
	}
	::WaitForSingleObject(pi.hProcess, INFINITE);


}

//-------------------------------------
// 备份未整理的开发版资源
//-------------------------------------
void do_step_6(void)
{
	system("mkdir Develop2");
	system("xcopy Develop\\*.* Develop2 /S /Q");
}

//-------------------------------------
// 运行资源整理工具
//-------------------------------------
static unsigned __stdcall _RunToolsThread_1(void* pParam)
{
	char szCurrent[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrent);

	char szToolsDir[MAX_PATH];
	strncpy(szToolsDir, szCurrent, MAX_PATH);
	PathAppend(szToolsDir, "Develop\\Client\\_build_tools");

	char szToolsFile[MAX_PATH];
	strncpy(szToolsFile, szToolsDir, MAX_PATH);
	PathAppend(szToolsFile, "FairyUtilTool.exe");

	//生成进程
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s FT_Clean_%d.cfg", szToolsFile, (int)(DWORD_PTR)pParam);

	//启动进程
	if(!::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szToolsDir, &si, &pi))
	{
		exit_error("Can't create process\n%s", szCmdLine);
	}
	::WaitForSingleObject(pi.hProcess, INFINITE);

	return 0;
}

void do_step_7(void)
{
	//生成线程
	HANDLE hProcess[1];
	for(int i=0; i<1; i++)
	{
		UINT nThreadID;
		HANDLE hMonThread = (HANDLE)::_beginthreadex(NULL, 0, _RunToolsThread_1, (void*)(DWORD_PTR)i, THREAD_QUERY_INFORMATION, &nThreadID );
		if (hMonThread == NULL)
		{
			exit_error("Can't create thread");
		}
		hProcess[i] = hMonThread;
	}

    // Wait until all child process exits.
	WaitForMultipleObjects(1, hProcess, TRUE, INFINITE );
}

//-------------------------------------
// 运行资源整理工具(纹理压缩)
//-------------------------------------
static unsigned __stdcall _RunToolsThread_2(void* pParam)
{
	char szCurrent[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrent);

	char szToolsDir[MAX_PATH];
	strncpy(szToolsDir, szCurrent, MAX_PATH);
	PathAppend(szToolsDir, "Develop\\Client\\_build_tools");

	char szToolsFile[MAX_PATH];
	strncpy(szToolsFile, szToolsDir, MAX_PATH);
	PathAppend(szToolsFile, "FairyUtilTool.exe");

	//生成进程
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s FT_DDS_%d.cfg", szToolsFile, (int)(DWORD_PTR)pParam);

	//启动进程
	if(!::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szToolsDir, &si, &pi))
	{
		exit_error("Can't create process\n%s", szCmdLine);
	}
	::WaitForSingleObject(pi.hProcess, INFINITE);

	return 0;
}

void do_step_8(void)
{
	//生成三个进程
	HANDLE hProcess[4];
	for(int i=0; i<4; i++)
	{
		UINT nThreadID;
		HANDLE hMonThread = (HANDLE)::_beginthreadex(NULL, 0, _RunToolsThread_2, (void*)(DWORD_PTR)i, THREAD_QUERY_INFORMATION, &nThreadID );
		if (hMonThread == NULL)
		{
			exit_error("Can't create thread");
		}
		hProcess[i] = hMonThread;
	}

    // Wait until all child process exits.
	WaitForMultipleObjects(4, hProcess, TRUE, INFINITE);
}

//-------------------------------------
//合并资源脚本(材质, 粒子, 特效, obj)
//-------------------------------------
void do_step_9(void)
{
	//执行资源合并进程
	char szCurrent[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrent);

	char szToolsDir[MAX_PATH];
	strncpy(szToolsDir, szCurrent, MAX_PATH);
	PathAppend(szToolsDir, "Develop\\Client\\_build_tools");

	char szToolsFile[MAX_PATH];
	strncpy(szToolsFile, szToolsDir, MAX_PATH);
	PathAppend(szToolsFile, "FairyUtilTool.exe");

	//生成进程
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s FT_Merge.cfg", szToolsFile);

	//启动进程
	if(!::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szToolsDir, &si, &pi))
	{
		exit_error("Can't create process\n%s", szCmdLine);
	}
	::WaitForSingleObject(pi.hProcess, INFINITE);
}

//-------------------------------------
//合并骨骼文件
//-------------------------------------
void do_step_10(void)
{
	char szCurrent[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrent);

	char szToolsDir[MAX_PATH];
	strncpy(szToolsDir, szCurrent, MAX_PATH);
	PathAppend(szToolsDir, "Develop\\Client\\_build_tools");

	char szToolsFile[MAX_PATH];
	strncpy(szToolsFile, szToolsDir, MAX_PATH);
	PathAppend(szToolsFile, "AxpUtil.exe");

	//生成进程
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s SkeletonMerge.lua", szToolsFile);

	//启动进程
	if(!::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szToolsDir, &si, &pi))
	{
		exit_error("Can't create process\n%s", szCmdLine);
	}
	::WaitForSingleObject(pi.hProcess, INFINITE);
}

//-------------------------------------
//拷贝可执行文件
//-------------------------------------
void do_step_11(void)
{
	//Copy client
	system("mkdir Develop\\Client\\_Bin_Game");
	system("copy %FAIRY_HOME%\\bin\\Release\\*.dll Develop\\Client\\_Bin_Game /Y");
	system("copy %OutRoot%\\Release\\*.exe Develop\\Client\\_Bin_Game /Y");
	system("copy %OutRoot%\\Release\\*.dll Develop\\Client\\_Bin_Game /Y");
	system("copy Develop\\Client\\_build_tools\\Helper.exe Develop\\Client\\_Bin_Game\\Helper.exe /Y");

	//copy server
	system("copy d:\\Prj\\Bin\\Server\\*.exe Develop\\Server /Y");
	system("mkdir Develop\\Server\\Log");
	system("mkdir Develop\\Server\\Users");

	//------copy develop2

	//Copy client
	system("mkdir Develop2\\Client\\_Bin_Game");
	system("copy %FAIRY_HOME%\\bin\\Release\\*.dll Develop2\\Client\\_Bin_Game /Y");
	system("copy %OutRoot%\\Release\\*.exe Develop2\\Client\\_Bin_Game /Y");
	system("copy %OutRoot%\\Release\\*.dll Develop2\\Client\\_Bin_Game /Y");
	system("copy Develop\\Client\\_build_tools\\Helper.exe Develop2\\Client\\_Bin_Game\\Helper.exe /Y");

	//copy server
	system("copy d:\\Prj\\Bin\\Server\\*.exe Develop2\\Server /Y");
	system("mkdir Develop2\\Server\\Log");
	system("mkdir Develop2\\Server\\Users");

	//backup pdb
	system("rmdir Develop2\\Client\\_PDB_ /S /Q");
	system("mkdir Develop2\\Client\\_PDB_");
	system("copy %FAIRY_HOME%\\bin\\Release\\*.pdb Develop2\\Client\\_PDB_ /Y");
	system("copy %OutRoot%\\Release\\*.pdb Develop2\\Client\\_PDB_ /Y");
}

//-------------------------------------
//开始创建发行版目录结构
//-------------------------------------
void do_step_12(void)
{
	system("rmdir RTM /S /Q");
	system("mkdir RTM");  
	system("mkdir RTM\\Client");
	system("mkdir RTM\\Client\\Bin");
	system("mkdir RTM\\Client\\Patch");
	system("mkdir RTM\\Client\\Data");
	system("mkdir RTM\\Client\\Helper");
	system("mkdir RTM\\Client\\DirectX9");

	system("copy Develop\\Client\\_Bin_Game\\Game.exe RTM\\Client\\Bin\\Game.exe /Y");
	system("copy Develop\\Client\\_Bin_Game\\tConfig.exe RTM\\Client\\Bin\\tConfig.exe /Y");
	system("copy Develop\\Client\\_Bin_Game\\Helper.exe RTM\\Client\\Bin\\Helper.exe /Y");
	system("copy Develop\\Client\\_Bin_Game\\CrashReport.exe RTM\\Client\\Bin\\CrashReport.exe /Y");
	system("copy Develop\\Client\\_Bin_Game\\AccountReg.exe RTM\\Client\\Bin\\AccountReg.exe /Y");
	system("copy Develop\\Client\\_Bin_Game\\*.dll RTM\\Client\\Bin /Y");
	system("copy Develop\\Client\\_Bin_Game\\*.cfg RTM\\Client\\Bin /Y");
	system("xcopy Develop\\Client\\Data RTM\\Client\\Data /S");
	system("xcopy Develop\\Client\\Helper RTM\\Client\\Helper /S");
	system("xcopy Develop\\Client\\DirectX9 RTM\\Client\\DirectX9 /S");
	system("copy Develop\\Client\\_build_tools\\LoginServer.txt RTM\\Client\\Patch\\LoginServer.txt /Y");
	system("copy Develop\\Client\\_build_tools\\license.txt RTM\\Client\\Helper\\license.txt /Y");
	system("copy Develop\\Client\\_build_tools\\System.cfg RTM\\Client\\Bin\\System.cfg /Y");
	system("copy Develop\\Client\\_build_tools\\FairyResources.cfg RTM\\Client\\Bin\\FairyResources.cfg /Y");
	system("copy Develop\\Client\\_build_tools\\Launch.exe RTM\\Client\\Launch.exe /Y");
	system("copy Develop\\Client\\_build_tools\\Launch.ini RTM\\Client\\Launch.ini /Y");
	system("copy Develop\\Client\\_build_tools\\Uninstall.exe RTM\\Client\\Uninstall.exe /Y");
	system("del /F /S RTM\\_desktop.ini");
}

//-------------------------------------
//资源打包
//-------------------------------------
void do_step_13(void)
{
	//----------
	char szCurrent[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrent);

	char szToolsDir[MAX_PATH];
	strncpy(szToolsDir, szCurrent, MAX_PATH);
	PathAppend(szToolsDir, "Develop\\Client\\_build_tools");

	char szToolsFile[MAX_PATH];
	strncpy(szToolsFile, szToolsDir, MAX_PATH);
	PathAppend(szToolsFile, "AxpUtil.exe");

	//生成进程
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s ResourcePak.lua", szToolsFile);

	//启动进程
	if(!::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szToolsDir, &si, &pi))
	{
		exit_error("Can't create process\n%s", szCmdLine);
	}
	::WaitForSingleObject(pi.hProcess, INFINITE);

	//move pak...
	system("mkdir RTM\\Client\\Data");
	system("del RTM\\Client\\Data\\*.axp /Q");
	system("move Develop\\Client\\_build_tools\\*.axp RTM\\Client\\Data");
}

//-------------------------------------
//做版本号
//-------------------------------------
void do_step_14(void)
{
	//----------
	char szCurrent[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrent);

	char szToolsDir[MAX_PATH];
	strncpy(szToolsDir, szCurrent, MAX_PATH);
	PathAppend(szToolsDir, "Develop\\Client\\_build_tools");

	char szToolsFile[MAX_PATH];
	strncpy(szToolsFile, szToolsDir, MAX_PATH);
	PathAppend(szToolsFile, "AxpUtil.exe");

	//生成进程
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s PatchMake.lua", szToolsFile);

	//启动进程
	if(!::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szToolsDir, &si, &pi))
	{
		exit_error("Can't create process\n%s", szCmdLine);
	}
	::WaitForSingleObject(pi.hProcess, INFINITE);
}

void paser_cmdline(int argc, _TCHAR* argv[])
{
	for(int i=1; i<argc; i++)
	{
		const char* szCmd = argv[i];
		
		if(stricmp(szCmd, "-all") == 0)
		{
			g_bYesAll = true;
		}

	}
}

void get_config(void)
{
	char szTemp[MAX_PATH];
	DWORD dwType, dwSize = MAX_PATH;

	//if(ERROR_SUCCESS != ::SHGetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\SOHU\\FireFox\\TLBB", "Tools_VSS1", &dwType, szTemp, &dwSize))
	//{
	//	exit_error("Param empty![VSS1]");
	//}
	strncpy(g_szVSS1Dir, "https://192.168.1.245/svn/WXSJ/trunk/Bin/", MAX_PATH);
}

int _tmain(int argc, _TCHAR* argv[])
{
	paser_cmdline(argc, argv);	//分析命令行
	get_config();				//取得配置参数

	//------------------------------------------------------------

	printf("--------------------------------\n");
	printf("---         BUILD       ---\n");
	printf("--------------------------------\n");

	printf("+\n+\n");
	printf("+  0. 清空本地的开发版和发行版目录\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_1;
	do_setp_0();

_STEP_1:
	printf("+\n+\n");
	printf("+  1. 拷贝外网资源  from %s\n", g_szVSS1Dir);
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_2;
	do_step_1();

_STEP_2:
	printf("+\n+\n");
	printf("+  2. DUMMY...\n");
	printf("+\n+\n");
//	if(!choice("Execute this step?")) goto _STEP_3;
//	do_step_2();

//_STEP_3:
	printf("+\n+\n");
	printf("+  3. 删除无用的目录和文件, 以及去掉文件只读属性\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_4;
	do_step_3();

_STEP_4:
	printf("+\n+\n");
	printf("+  4. 生成Fairy工具\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_5;
	do_step_4();

_STEP_5:
	printf("+\n+\n");
	printf("+  5. 运行脚本整理工具\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_6;
	do_step_5();

_STEP_6:
	printf("+\n+\n");
	printf("+  6. 备份未整理的开发版资源\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_7;
	do_step_6();

_STEP_7:
	printf("+\n+\n");
	printf("+  7. 运行资源整理工具(重构材质和骨骼数据)\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_8;
	do_step_7();

_STEP_8:
	printf("+\n+\n");
	printf("+  8. 运行资源整理工具(压缩纹理生成DDS)\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_9;
	do_step_8();

_STEP_9:
	printf("+\n+\n");
	printf("+  9. 合并资源脚本(材质, 粒子, 特效, obj)\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_10;
	do_step_9();

_STEP_10:
	printf("+\n+\n");
	printf("+  10. 合并骨骼文件\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_11;
	do_step_10();

_STEP_11:
	printf("+\n+\n");
	printf("+  11. 拷贝可执行文件\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_12;
	do_step_11();

_STEP_12:
	printf("+\n+\n");
	printf("+  12. 开始创建发行版目录结构\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_13;
	do_step_12();

_STEP_13:
	printf("+\n+\n");
	printf("+  13. 资源打包\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_14;
	do_step_13();

_STEP_14:
	printf("+\n+\n");
	printf("+  14. 做版本号\n");
	printf("+\n+\n");
	if(!choice("Execute this step?")) goto _STEP_15;
	do_step_14();

_STEP_15:
	printf(">>>>>>>>>>>>>>>ALL DONE<<<<<<<<<<<<<<<<\n");
	system("pause");
	return 0;
}

