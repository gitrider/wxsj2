// mylink.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Resource.h"

char g_szSprFile[MAX_PATH]={0};
const char* g_szFileNeedBackup[] = 
{
	"Game.exe", "Game.pdb"
};
const int g_nFileNeedBackupCounts = sizeof(g_szFileNeedBackup)/sizeof(const char* );
char g_szWorkPath[MAX_PATH];
const char* g_szTargetPath = "V:\\_Jinch\\未加密Game备份\\%s";
bool g_bDebugMode=false;

std::string trimstring(const std::string& strIn)
{
	std::string strOut = strIn;
	static const std::string EMPTY = "";

	std::string::size_type start = strOut.find_first_not_of("\t ");
	if(start == std::string::npos) return EMPTY;
	strOut = strOut.substr(start, std::string::npos);

	std::string::size_type end = strOut.find_last_not_of("\t \r\n");
	if(end == std::string::npos) return EMPTY;
	strOut = strOut.substr(0, end+1);

	return strOut;
}

void CenterDlg(HWND hDlg)
{
	int x,y;
	int w,h;
	
	RECT rect;

	GetWindowRect(hDlg,&rect);

	w = rect.right - rect.left;
	h = rect.bottom - rect.top;

	x = ( GetSystemMetrics(SM_CXMAXIMIZED) - w ) /2;
	y = ( GetSystemMetrics(SM_CYMAXIMIZED) - h ) /2;

//	MoveWindow(hDlg,x,y,w,h,TRUE);

	::SetWindowPos(hDlg, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
}

DWORD getFileSize(const char* szFileName)
{
	assert(szFileName);
	if(!szFileName || szFileName[0] == 0) return 0;

	//Open file Handle
	HANDLE hFile = ::CreateFile(szFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	//Get File Size
	DWORD nFileSize = ::GetFileSize(hFile, 0);
	
	//Close handle
	::CloseHandle(hFile); hFile=0;

	return nFileSize;
}

bool initBackupFiles(HWND hwndDlg)
{
	strncpy(g_szWorkPath, g_szSprFile, MAX_PATH);
	::PathRemoveFileSpec(g_szWorkPath);
	::PathRemoveFileSpec(g_szWorkPath);

	std::string strInputDesc;
	for(int i=0; i<g_nFileNeedBackupCounts; i++)
	{
		char szFile[MAX_PATH];
		strncpy(szFile, g_szWorkPath, MAX_PATH);
		::PathAppend(szFile, g_szFileNeedBackup[i]);

		DWORD fileSize = getFileSize(szFile);
		if(fileSize==0)
		{
			char msg[MAX_PATH]={0};
			_snprintf(msg, MAX_PATH, "Can't open source file[%s]", szFile);
			MessageBox(hwndDlg, msg, "mylink", MB_OK|MB_ICONSTOP);
			return false;
		}

		char temp[MAX_PATH];
		_snprintf(temp, MAX_PATH, "%s (FileSize:%d)\n", szFile, fileSize);

		strInputDesc += temp;
	}

	::SetDlgItemText(hwndDlg, IDC_STATIC_FILE_SOURCE, strInputDesc.c_str());

	return true;
}

//递归创建目录
bool forceCreatePath(const char* szPath)
{
	//目录已经存在
	if(::PathFileExists(szPath)) return true;
	//能够直接创建
	if(::CreateDirectory(szPath, 0)) return true;

	//取得上一级目录
	char szParentPath[MAX_PATH] = {0};
	strncpy(szParentPath, szPath, MAX_PATH);
	if(!::PathRemoveFileSpec(szParentPath)) return false;

	//创建上一级目录
	if(!forceCreatePath(szParentPath)) return false;

	return TRUE==::CreateDirectory(szPath, 0);
}

bool backupFiles(HWND hwndDlg)
{
	char szTargetDir[MAX_PATH]={0};
	::GetDlgItemText(hwndDlg, IDC_STATIC_FILE_TARGET, szTargetDir, MAX_PATH);
	if(szTargetDir[0]==0) return false;
	if(forceCreatePath(szTargetDir) == false)
	{
		char szMsg[MAX_PATH]={0};
		_snprintf(szMsg, MAX_PATH, "创建目标目录失败!\n请尝试手动创建该目录!\n%s", szTargetDir);
		MessageBox(hwndDlg, szMsg, "mylink", MB_OK|MB_ICONSTOP);
		return false;
	}

	for(int i=0; i<g_nFileNeedBackupCounts; i++)
	{
		char szSource[MAX_PATH] = {0};
		char szTarget[MAX_PATH] = {0};
		strncpy(szSource, g_szWorkPath, MAX_PATH);
		::PathAppend(szSource, g_szFileNeedBackup[i]);

		strncpy(szTarget, szTargetDir, MAX_PATH);
		::PathAppend(szTarget, g_szFileNeedBackup[i]);

		if(::PathFileExists(szTarget))
		{
			char szMsg[MAX_PATH]={0};
			_snprintf(szMsg, MAX_PATH, "%s 已经存在， 覆盖?", szTarget);
			if(IDYES != MessageBox(hwndDlg, szMsg, "mylink", MB_YESNO|MB_ICONQUESTION))
			{
				return false;
			}
		}

		if(!CopyFile(szSource, szTarget, FALSE))
		{
			DWORD dwErr = ::GetLastError();
			char szMsg[MAX_PATH]={0};
			_snprintf(szMsg, MAX_PATH, "拷贝文件失败, LastError=%08X\n%s\n->\n%s", 
				dwErr, szSource, szTarget);
			MessageBox(hwndDlg, szMsg, "mylink", MB_OK|MB_ICONSTOP);
			return false;
		}
	}

	MessageBox(hwndDlg, "备份成功!", "mylink", MB_OK|MB_ICONINFORMATION);

	return true;
}


INT_PTR CALLBACK _BackupDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					if(backupFiles(hwndDlg))
					{
						EndDialog(hwndDlg, 0);
					}
				}
				break;

			case IDCANCEL:
				if(IDYES == MessageBox(hwndDlg, "取消备份?", "mylink", MB_YESNO|MB_ICONQUESTION))
				{
					EndDialog(hwndDlg, 0);
				}
                break;

			case IDC_EDIT_VERSION:
				if(HIWORD(wParam) == EN_CHANGE)
				{
					char szVersion[MAX_PATH];
					::GetDlgItemText(hwndDlg, IDC_EDIT_VERSION, szVersion, MAX_PATH);
					if(strlen(szVersion)==0)
					{
						::EnableWindow(::GetDlgItem(hwndDlg, IDOK), FALSE);
						::SetDlgItemText(hwndDlg, IDC_STATIC_FILE_TARGET, "");
						break;
					}
					char szOutput[MAX_PATH];
					_snprintf(szOutput, MAX_PATH, g_szTargetPath, szVersion);
					::SetDlgItemText(hwndDlg, IDC_STATIC_FILE_TARGET, szOutput);
					::EnableWindow(::GetDlgItem(hwndDlg, IDOK), TRUE);
				}
				break;
			}
			break;
		}
		break;

	case WM_INITDIALOG:
		{
			CenterDlg(hwndDlg);
			if(!initBackupFiles(hwndDlg))
			{
				EndDialog(hwndDlg, 1);
			}

			::SetFocus(::GetDlgItem(hwndDlg, IDC_EDIT_VERSION));
			::EnableWindow(::GetDlgItem(hwndDlg, IDOK), FALSE);
		}
		break;

	case WM_CLOSE:
		//PostQuitMessage(0);
		break;

	default:
		break;
	}
	return FALSE;
}

int runOriginalLink(void)
{
	//find Original link.exe ( as _link.exe)
	char szOriginalLink[MAX_PATH];
	::GetModuleFileName(0, szOriginalLink, MAX_PATH);
	::PathRemoveFileSpec(szOriginalLink);
	::PathAppend(szOriginalLink, "_link.exe");
	if(!::PathFileExists(szOriginalLink))
	{
		printf("ERROR: Can't open original link.exe\n");
		return 1;
	}

	//当前路径
	char szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	const int MaxCmdLineSize = 1024*4;
	char szCommand[MaxCmdLineSize]={0};
	const char* pCmd = ::GetCommandLine();
	if(pCmd[0] == '"')
	{
		pCmd = strchr(pCmd+1, '"')+1; 
	}
	else
	{
		pCmd = strchr(::GetCommandLine(), ' ');
	}

	if(_snprintf(szCommand, MaxCmdLineSize, "\"%s\" %s", szOriginalLink, pCmd?pCmd:"") > MaxCmdLineSize-8)
	{
		printf("ERROR: Toooooo long commandline\n%s!", ::GetCommandLine());
		return 1;
	}

	printf("Run original link.exe\n%s\n", szCommand);

	//启动进程
	STARTUPINFO			si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
	
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	
	if (CreateProcess(NULL, szCommand, NULL, NULL, 
		FALSE,CREATE_DEFAULT_ERROR_MODE, NULL,
		szCurrentDir, &si, &pi) == FALSE)
	{
		//启动失败
		DWORD err = ::GetLastError();
		printf("ERROR: Can't create process(%08X)\n", err);
		return 1;
	}

	// Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

	//get exit code
	DWORD ret;
	::GetExitCodeProcess(pi.hProcess, &ret);

	//Not success!
	return ret;
}

bool isRspFile(const char* szInputFile)
{
	if(!szInputFile || szInputFile[0]==0) return false;
	int len = (int)strlen(szInputFile);

	if(szInputFile[0]=='@' && stricmp(::PathFindExtension(szInputFile), ".rsp")==0)
	{
		strncpy(g_szSprFile, szInputFile+1, MAX_PATH);
		return true;
	}

	if(szInputFile[0]=='"' && len>4 && szInputFile[1]=='@' && szInputFile[len-1]=='"')
	{
		strncpy(g_szSprFile, szInputFile+2, len-3);
		if(stricmp(::PathFindExtension(g_szSprFile), ".rsp")==0)
		{
			return true;
		}

		memset(g_szSprFile, 0, MAX_PATH);
	}

	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//init rand seed
   	srand(::GetTickCount());

	int mode=0;	// 1 - exe,		2 - lib,	other...

	//must be "@xxx.rsp" mode
	if(argc == 2 && isRspFile(argv[1]))
	{
		mode=1;
	}
	else if(argc==3 && (stricmp(argv[1], "-lib")==0 || stricmp(argv[1], "/lib")==0) && 
		isRspFile(argv[2]))
	{
		mode=2;
	}

	if(mode==0)
	{
		return runOriginalLink();
	}
	
	// check rsp file 
	if(!::PathFileExists(g_szSprFile))
	{
		return runOriginalLink();
	}
	
	// is build client
	bool buildClient = false;
	char szOutRoot[MAX_PATH];
	if(::GetEnvironmentVariable("OutRoot", szOutRoot, MAX_PATH) && szOutRoot[0]!=0)
	{
		//is build client ?
		if(_strnicmp(szOutRoot, g_szSprFile, strlen(szOutRoot)) == 0)
		{
			buildClient = true;
		}
	}

	if(!buildClient)
	{
		return runOriginalLink();
	}

	//random options rsp file 
	{
		char debugMylink[MAX_PATH]={0};
		if(::GetEnvironmentVariable("DebugMyLink", debugMylink, MAX_PATH) && debugMylink[0]=='1')
		{
			g_bDebugMode = true;
		}

		if(g_bDebugMode)
		{
			char szMsg[MAX_PATH]={0};
			_snprintf(szMsg, MAX_PATH, "Before random\n%s", g_szSprFile);
			MessageBox(::GetDesktopWindow(), szMsg, "mylink", MB_OK|MB_ICONINFORMATION);
		}

		//读入输入文件
		typedef std::vector< std::string > StringVector;
		StringVector fileVector;

		FILE* fpIn = fopen(g_szSprFile, "r");
		while(true)
		{
			if(feof(fpIn)) break;
			char tempBuf[1024*4] = {0};
			fgets(tempBuf, 1024*4, fpIn);
			std::string strTemp = trimstring(tempBuf);
			if(strTemp.empty()) continue;
		
			fileVector.push_back(strTemp);
		};
		fclose(fpIn);

		//打乱顺序
		const int SWAP_COUNTS = 1000000;

		int lineCounts = (int)fileVector.size();
		int* idBuf = new int[lineCounts];
		for(int i=0; i<lineCounts; i++) idBuf[i]=i;
		for(int i=0; i<SWAP_COUNTS; i++)
		{
			int index1 = rand()%(lineCounts-1) + 1; // [1, lineCounts-1]
			int index2 = rand()%(lineCounts-1) + 1; 

			register int temp = idBuf[index1];
			idBuf[index1] = idBuf[index2];
			idBuf[index2] = temp;
		}


		// out put
		FILE* fpOut = fopen(g_szSprFile, "w");
		for(int i=0; i<lineCounts; i++)
		{
			fprintf(fpOut, "%s\n", fileVector[idBuf[i]].c_str());
		}
		fclose(fpOut);
		delete[] idBuf; idBuf=0;


		if(g_bDebugMode)
		{
			MessageBox(::GetDesktopWindow(), "After random", "mylink", MB_OK|MB_ICONINFORMATION);
		}

		printf("mylink: random all obj file link order!");
	}

	//find Original link.exe ( as _link.exe)
	char szOriginalLink[MAX_PATH];
	::GetModuleFileName(0, szOriginalLink, MAX_PATH);
	::PathRemoveFileSpec(szOriginalLink);
	::PathAppend(szOriginalLink, "_link.exe");
	if(!::PathFileExists(szOriginalLink))
	{
		printf("ERROR: Can't open original link.exe\n");
		return 1;
	}

	//当前路径
	char szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	//命令行
	char szCommand[MAX_PATH*2];
	_snprintf(szCommand, MAX_PATH*2, "\"%s\"", szOriginalLink);
	for(int i=1; i<argc; i++)
	{
		strcat(szCommand, " ");
		strcat(szCommand, argv[i]);
	}

	//启动进程
	STARTUPINFO			si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
	
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	
	if (CreateProcess(NULL, szCommand, NULL, NULL, 
		FALSE,CREATE_DEFAULT_ERROR_MODE, NULL,
		szCurrentDir, &si, &pi) == FALSE)
	{
		//启动失败
		DWORD err = ::GetLastError();
		printf("ERROR: Can't create process(%08X)\n", err);
		return 1;
	}

	// Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

	//get exit code
	DWORD ret;
	::GetExitCodeProcess(pi.hProcess, &ret);

	//Not success!
	if(ret!=0 || !buildClient) return ret;

	strlwr(g_szSprFile);
	if(strstr(g_szSprFile, "game") == 0) return ret;
	
	//backup exe and pdb
	return (int)::DialogBox(::GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG_BACKUP), ::GetDesktopWindow(), _BackupDlgProc);
}

