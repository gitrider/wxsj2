// DumpMonitorW.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DumpMonitorW.h"
#include <winsock.h>
#include <objbase.h>

#include <string>
#include <vector>

using namespace std;

#define MAX_LOADSTRING 100

SOCKET					SendSocket;
sockaddr_in				RecvAddr;
TCHAR					g_bat_name[MAX_PATH]={0};
TCHAR					g_tsz_cur_path[MAX_PATH] = {0};

typedef vector<string>  VNOTIFY_ITEM;
vector<VNOTIFY_ITEM>	g_notify;

string UnicodeToChar(LPCWSTR lpWideCharStr, int cchWideChar)
{
	char sz_buf[1024] = {0};
	WideCharToMultiByte( CP_ACP,0,(LPWSTR)lpWideCharStr,cchWideChar,sz_buf,sizeof(sz_buf) - 1,NULL,NULL );
	return string(sz_buf);
}

wstring CharToUnicode(LPCSTR lpMultiByteStr, int cbMultiByte)
{
	WCHAR wsz_buf[1024] = {0};
	MultiByteToWideChar( CP_ACP,0,lpMultiByteStr,cbMultiByte,wsz_buf,sizeof(wsz_buf) - 1);
	return wstring(wsz_buf);
}

string read_file(const char * pcstr_file)
{
	FILE * fp = fopen(pcstr_file, "r");
	if(fp)
	{
		char sz_file_buf[1024 * 128] = {0};
		int n_read = fread(sz_file_buf, sizeof(sz_file_buf), 1, fp);
		string str_file_buf = sz_file_buf;
		fclose(fp);
		return str_file_buf;
	}

	return "";
}

void read_notify_programer()
{
	WCHAR sz_notify_file[MAX_PATH] = {0};
	swprintf_s(sz_notify_file, MAX_PATH, _T("%s\\notice_programer.txt"), g_tsz_cur_path);
	string str = UnicodeToChar(sz_notify_file, wcslen(sz_notify_file));
	string str_file_buf = read_file(str.c_str());
	
	int n_pos = str_file_buf.find("\n");
	int n_pos_old = 0;
	while(string::npos != n_pos)
	{
		string str_line = str_file_buf.substr(n_pos_old, n_pos - n_pos_old);
		
		if(str_line.length() > 5)
		{
			VNOTIFY_ITEM vi;
			int n_spos = str_line.find(";");
			while(n_spos != string::npos)
			{
				string str_sub = str_line.substr(0, n_spos);
				vi.push_back(str_sub);
				str_line = str_line.substr(n_spos + 1);
				n_spos = str_line.find(";");
			}
			g_notify.push_back(vi);
		}

		n_pos_old = n_pos + 1;
		n_pos = str_file_buf.find("\n", n_pos + 1);
	}
}

string replace_newline(string & str_content)
{
	int n_pos = str_content.find("\n");
	while(string::npos != n_pos)
	{
		str_content.insert(n_pos, "<br>", 4);
		n_pos = str_content.find("\n", n_pos + 5);
	}
	return str_content;
}

string get_resolve_file(TCHAR * ptstr_dump_file)
{
	WCHAR sz_resolve_file[MAX_PATH] = {0};
	swprintf_s(sz_resolve_file, MAX_PATH, _T("%s\\%s.sol"), g_tsz_cur_path, ptstr_dump_file);

	string str = UnicodeToChar(sz_resolve_file, wcslen(sz_resolve_file));

	string str_file_buf = read_file(str.c_str());

	if(str_file_buf.length())
	{
		return replace_newline(str_file_buf);
	}

	return "";
}

string get_dump_parse(TCHAR * ptstr_dump_file)
{
	WCHAR sz_out_file[MAX_PATH] = {0};
	swprintf_s(sz_out_file, MAX_PATH, _T("%s\\%s.txt"), g_tsz_cur_path, ptstr_dump_file);
	
	string str = UnicodeToChar(sz_out_file, wcslen(sz_out_file));

	string str_file_buf = read_file(str.c_str());

	if(str_file_buf.length())
	{
		int n_pos = str_file_buf.find(".ecxr;kb;q");
		str_file_buf = str_file_buf.substr(n_pos + 11);
		str_file_buf = str_file_buf.substr(0, str_file_buf.length() - 7);

		return replace_newline(str_file_buf);
	}

	return "";
}

void add_dump_file(FILE* f_index, TCHAR * ptstr_dump_file)
{
	if(NULL == ptstr_dump_file)
	{
		return;
	}

	if(ptstr_dump_file[0] == TEXT('.'))
	{
		return;
	}

	unsigned int u_len = wcslen(ptstr_dump_file);

	if(u_len < 5)
	{
		return;
	}

	if(ptstr_dump_file[u_len - 3] != TEXT('d') 
	|| ptstr_dump_file[u_len - 2] != TEXT('m')
	|| ptstr_dump_file[u_len - 1] != TEXT('p'))
	{
		return;
	}

	string str_file_name = UnicodeToChar(ptstr_dump_file, wcslen(ptstr_dump_file));

	string str_dump_resolve = get_resolve_file(ptstr_dump_file);

	string str_dump_parse = get_dump_parse(ptstr_dump_file);

	if(str_dump_resolve.length() <= 0)
	{
		str_file_name = "<a href=" + str_file_name +"><font color=\"#ff0000\">" + str_file_name + "</font></a>";
		str_dump_parse = "<font color=\"#ff0000\">" + str_dump_parse + "</font>";
	}
	else
	{
		str_file_name = "<a href=" + str_file_name +">" + str_file_name + "</a>";
	}
	
	string str_content = "<TR>\n<TH>\n " + str_file_name +"</TH>\n<TD>" + str_dump_resolve + "</TD><TD>" + str_dump_parse + "</TD>\n</TR>\n";

	fwrite(str_content.c_str(), str_content.length(), 1, f_index);
}

void make_html_index()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	FILE* f_index = NULL;
	TCHAR DirSpec[MAX_PATH];  // directory specification
	DWORD dwError;

	swprintf_s(DirSpec, MAX_PATH, TEXT("%s\\*"), g_tsz_cur_path);
	
	hFind = FindFirstFile(DirSpec, &FindFileData);
	
	TCHAR swz_index[MAX_PATH] = {0};
	TCHAR sz_index[MAX_PATH] = {0};
	swprintf_s(swz_index, MAX_PATH, TEXT("%s\\index.htm"), g_tsz_cur_path);

	string str = UnicodeToChar(swz_index, wcslen(swz_index));

	f_index = fopen(str.c_str(), "w");
	if (f_index == NULL){
 		goto end;
	}

	{
		const char * pstr_head = "<html>\n<title>bug分析系统</title>\n<body>\n<TABLE border=1>\n";
		fwrite(pstr_head, strlen(pstr_head), 1, f_index);
	}
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		goto end;
	} 
	else 
	{
		//WriteToLogw(FindFileData.cFileName, wcslen(FindFileData.cFileName));
		add_dump_file(f_index, FindFileData.cFileName);
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			//WriteToLogw(FindFileData.cFileName, wcslen(FindFileData.cFileName));
			add_dump_file(f_index, FindFileData.cFileName);
		}

		dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			goto end;
		}
	}

end:
	if(f_index)
	{
		const char * pstr_foot = "</TABLE>\n</body>\n</html>\n";
		fwrite(pstr_foot, strlen(pstr_foot), 1, f_index);

		fclose(f_index);
	}

	if(hFind == INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
	}
	
	return;
}

class CFileWatcher 
{
public:
	bool StartWatch( wstring path); //开始监视
	CFileWatcher()  // 初始化
	{ 
		hThread=NULL;
		hDir=INVALID_HANDLE_VALUE  ;
	}
	~CFileWatcher() 
	{
		if(hThread !=NULL)
		{
			::TerminateThread(hThread, 0 );
			hThread = NULL;
		}
		if(hDir !=INVALID_HANDLE_VALUE)
		{
			CloseHandle( hDir );
			hDir = INVALID_HANDLE_VALUE;
		}
	}

private:
	wstring strWatchedDir;
	HANDLE hThread;
	HANDLE hDir; 
private:
	static DWORD WINAPI ThreadProc( LPVOID lParam ) ; //线程函数，用来监视
};


bool CFileWatcher::StartWatch( wstring path)
{
	strWatchedDir = path;

	DWORD ThreadId;  //创建一个新线程用于监视
	hThread=::CreateThread(NULL,0,ThreadProc,this,0,&ThreadId );

	return NULL!=hThread;
}

void notice_programer(TCHAR * ptstr_dump_file)
{
	for(vector<VNOTIFY_ITEM>::iterator Iter = g_notify.begin(); 
		Iter != g_notify.end();
		Iter++)
	{
		string str = UnicodeToChar(ptstr_dump_file, wcslen(ptstr_dump_file));

		if(strstr(str.c_str(), Iter->at(0).c_str()))
		{
			for(int i = 1; i < Iter->size(); i++)
			{
				string str_reciver = Iter->at(i) + string(";");
				GUID guid;
				::CoCreateGuid(&guid);
				char sz_guid[64] = {0};
				sprintf_s(sz_guid, sizeof(sz_guid), "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}"
				, guid.Data1, guid.Data2, guid.Data3
				, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3]
				, guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

				string SendBuf = "debug;666666;" + str_reciver  + "bug是程序员的爱人，你的爱人找你来了。 http://192.168.1.181/dumphome/index.htm ;" + sz_guid +";";
				sendto(SendSocket, SendBuf.c_str(), SendBuf.length(), 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
			}

			break;
		}
	}
}

DWORD WINAPI CFileWatcher::ThreadProc( LPVOID lParam )  //线程函数
{
	CFileWatcher* obj = (CFileWatcher*)lParam;  //参数转化

	obj->hDir = CreateFile(            //打开目录，得到目录的句柄
		obj->strWatchedDir.c_str(),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL
		);
	if(obj->hDir ==INVALID_HANDLE_VALUE  )
		return false;

	char buf[(sizeof(FILE_NOTIFY_INFORMATION)+MAX_PATH)*2];
	FILE_NOTIFY_INFORMATION* pNotify=(FILE_NOTIFY_INFORMATION*)buf;
	DWORD dwBytesReturned;       
	while(true)
	{
		if( ::ReadDirectoryChangesW( obj->hDir,
			pNotify,
			sizeof(buf),
			true,
			FILE_NOTIFY_CHANGE_FILE_NAME|
			FILE_NOTIFY_CHANGE_DIR_NAME|
			FILE_NOTIFY_CHANGE_LAST_WRITE|
			FILE_NOTIFY_CHANGE_CREATION|
			FILE_NOTIFY_CHANGE_SECURITY|
			FILE_NOTIFY_CHANGE_SIZE,
			&dwBytesReturned,
			NULL,
			NULL ) )
		{
			int nLen = pNotify->FileNameLength/2;
			WCHAR sz_notify_name[MAX_PATH] = {0};
			wcsncat_s(sz_notify_name, MAX_PATH, pNotify->FileName, nLen);

			if(NULL != wcsstr(sz_notify_name, _T(".htm")))
			{
				continue;
			}

			BOOL b_add_dump_file = FALSE;
			
			switch(pNotify->Action)
			{
			case FILE_ACTION_ADDED:
				{	
					if(sz_notify_name[nLen - 3] == _T('d') 
					&& sz_notify_name[nLen - 2] == _T('m')
					&& sz_notify_name[nLen - 1] == _T('p'))
					{
						WCHAR sz_dump_file[MAX_PATH] = {0};
						swprintf_s(sz_dump_file, MAX_PATH, _T("%s\\%s"), g_tsz_cur_path, sz_notify_name);
						WCHAR sz_out_file[MAX_PATH] = {0};
						swprintf_s(sz_out_file, MAX_PATH, _T("%s.txt"), sz_dump_file);
						WCHAR sz_command[MAX_PATH * 3] = {0};
						swprintf_s(sz_command, TEXT("%s %s %s"), g_bat_name, sz_dump_file, sz_out_file);
						string str_command = UnicodeToChar(sz_command, wcslen(sz_command));
						system(str_command.c_str());
						Sleep(1000);
						if(-1 == ::GetFileAttributes(sz_dump_file))
						{
							//::MessageBox(::GetActiveWindow(), _T("OK"), _T("OK"), MB_OK);
							Sleep(10000);
						}

						b_add_dump_file = TRUE;
					}
				}
			case FILE_ACTION_REMOVED:
			case FILE_ACTION_RENAMED_NEW_NAME:
			//case FILE_ACTION_RENAMED_OLD_NAME:
			case FILE_ACTION_MODIFIED:
				{
					make_html_index();
					if(b_add_dump_file)
					{
						notice_programer(sz_notify_name);
					}
				}
				break;
			}  
		}
		else          
			break;          
	}
	return 0;
}

CFileWatcher g_file_watcher;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	::CoInitialize(NULL);

	::GetCurrentDirectory(MAX_PATH, g_tsz_cur_path);

	read_notify_programer();

	swprintf_s(g_bat_name, MAX_PATH, _T("%s\\ParseDump.bat"), g_tsz_cur_path);
	
	WSADATA wsaData;
	//---------------------------------------------
	// Initialize Winsock
	WSAStartup(MAKEWORD(2,2), &wsaData);

	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(5555);
	//RecvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	RecvAddr.sin_addr.s_addr = inet_addr("192.168.1.186");

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DUMPMONITORW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DUMPMONITORW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	::CoUninitialize();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DUMPMONITORW));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DUMPMONITORW);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	g_file_watcher.StartWatch(g_tsz_cur_path);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			//case IDM_ABOUT:
			//	system("E:\\Project\\trunk\\tools\\DumpMonitor\\release\\ParseDump.bat E:\\Project\\trunk\\tools\\DumpMonitor\\release\\Server_crash_USER-4E349Q8389_06241945_0.dmp E:\\Project\\trunk\\tools\\DumpMonitor\\release\\out.txt");
			//	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			//	break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		RECT rect;
		::GetClientRect(hWnd, &rect);
		::DrawText(hdc, _T("dump监控系统，请不要关闭"), wcslen(_T("dump监控系统，请不要关闭")), &rect, DT_CENTER);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		::MoveWindow(hWnd, 500,300,200,80, TRUE);
		break;
	case WM_DESTROY:
		closesocket(SendSocket);
		WSACleanup();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
