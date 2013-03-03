// UpdateExeHead.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "UpdateExeHead.h"
#include "Util.h"
#include "Resource.h"

#include "AXP.h"


// Global Variables:
HINSTANCE	g_hInst=0;								// current instance
HWND		g_hMainWnd=0;							// MainWindow
HANDLE		g_hWorkThread=0;						// work thread
BOOL		g_bFaultMode=FALSE;						// ����ģʽ
BOOL		g_bSuccess=FALSE;						// �����ɹ�
CHAR		g_szGamePath[MAX_PATH] = {0};			// ��ϷĿ¼
CHAR		g_szGameVersion[32] = {0};				// ���ذ汾
CHAR		g_szGameVersion_nor[64] = {0};
HANDLE		g_hBeginHandle=0;						// ��ʼ�����ź�

//�汾����
CHAR		g_szVersionFrom[32] = {0};				// ֧�ִ�?�汾����
CHAR		g_szVersionFrom_nor[32] = {0};			// ֧�ִ�?�汾����
CHAR		g_szVersionTo[32] = {0};				// ������?�汾
CHAR		g_szVersionTo_nor[32] = {0};			// ������?�汾
CHAR		g_szPatchZip[MAX_PATH] = {0};			// ZIP������
UINT		g_nUpdateFileCount = 0;					// �����ļ���

// exe�����С
#ifdef _DEBUG
UINT		g_nExeSize = 716800;				
#else
UINT		g_nExeSize = 212992;
#endif

//�������ģʽ
#define WM_MSG_ENTERFAULT		(WM_USER+1001)
//������
#define WM_MSG_SETPROGRESS		(WM_USER+1002)
//����
#define WM_MSG_SUCCESS			(WM_USER+1003)

//Ѱ�Ұ汾��(call by work thead)
bool _findGameVersion(const char* szGamePath, bool bCheckVersionValid)
{
	CHAR szVersionFile[MAX_PATH] = {0};
	strncpy(szVersionFile, szGamePath, MAX_PATH);
	::PathAppend(szVersionFile, "(version)");

	FILE* fp = fopen(szVersionFile, "r");
	if(!fp) return false;
		
	//��ȡ��һ��
	char szTemp[MAX_PATH] = {0};
	if(!fgets(szTemp, MAX_PATH, fp)) 
	{
		fclose(fp);
		return false;
	}
	fclose(fp);

	char* szDot = strchr(szTemp, '|');
	if(!szDot) return false;
	*szDot = 0;
	
	strncpy(g_szGameVersion, szTemp, 32);
	normalizeVersion(g_szGameVersion, g_szGameVersion_nor, 64);

	if(bCheckVersionValid)
	{
		//�жϰ汾�Ƿ�֧��
		if( stricmp(g_szGameVersion_nor, g_szVersionFrom_nor) < 0 || 
			stricmp(g_szGameVersion_nor, g_szVersionTo_nor) > 0)
		{
			return false;
		}
	}

	return true;
}

//Ѱ����ϷĿ¼(call by work thead)
bool _searchGamePath(void)
{
#if 0
	//1. Ѱ��ע���
	do
	{
		DWORD dwType, dwSize=MAX_PATH;

		if(ERROR_SUCCESS != ::SHGetValue(HKEY_LOCAL_MACHINE, 
			"SOFTWARE\\SOHU\\FireFox\\TLBB", "InstallPath", &dwType, g_szGamePath, &dwSize))
		{
			break;
		}
		
		if(_findGameVersion(g_szGamePath, true)) return true;

	}while(0);
#endif

	//2. Ѱ�ҵ�ǰĿ¼
	do
	{
		::GetModuleFileName(g_hInst, g_szGamePath, MAX_PATH);
		::PathRemoveFileSpec(g_szGamePath);

		if(_findGameVersion(g_szGamePath, true)) return true;
	}while(0);

	//3. ���ѡ��
	do
	{
		BROWSEINFO bi; 
		bi.hwndOwner		= g_hMainWnd; 
		bi.pidlRoot			= NULL; 
		bi.pszDisplayName	= g_szGamePath; 
		bi.lpszTitle		= "��ѡ�������˲������ڵ�·��"; 
		bi.ulFlags			= BIF_RETURNONLYFSDIRS; 
		bi.lpfn				= NULL; 
		bi.lParam			= 0; 
		bi.iImage			= 0; 

		ITEMIDLIST* pidl = ::SHBrowseForFolder(&bi);
		if(pidl==NULL || !::SHGetPathFromIDList(pidl, g_szGamePath)) 
		{
			break;
		}

		if(_findGameVersion(g_szGamePath, false)) return true;
		
	}while(0);


	return false;
}

//��ȡ��������(call by work thead)
bool _readPatchContents(void)
{
	char szExeSelf[MAX_PATH] = {0};
	::GetModuleFileName(g_hInst, szExeSelf, MAX_PATH);
	FILE* fp = fopen(szExeSelf, "rb");
	if(!fp) return false;

	do
	{
		if(0 != fseek(fp, g_nExeSize, SEEK_SET)) break;

		int nLen=0;

		//read version from
		if(1 != fread(&nLen, sizeof(int), 1, fp) || nLen>=32 ) break;
		if(nLen != fread(g_szVersionFrom, 1, nLen, fp)) break;
		normalizeVersion(g_szVersionFrom, g_szVersionFrom_nor, 64);

		//read version to
		if(1 != fread(&nLen, sizeof(int), 1, fp) || nLen>=32 ) break;
		if(nLen != fread(g_szVersionTo, 1, nLen, fp)) break;
		normalizeVersion(g_szVersionTo, g_szVersionTo_nor, 64);

		//create temp patch file
		char szTempPath[MAX_PATH];
		GetTempPath(MAX_PATH, szTempPath);
		PathAppend(szTempPath, "TLBBPatch");
		CreateDirectory(szTempPath, 0);
		_snprintf(g_szPatchZip, MAX_PATH, "%s\\tlbb-%s-%s.xzip", 
			szTempPath, g_szVersionFrom, g_szVersionTo);

		FILE* fpWrite = fopen(g_szPatchZip, "wb");
		if(!fpWrite) break;

		char szTempBuf[1024] = {0};
		do
		{
			size_t nReadSize = fread(szTempBuf, 1, 1024, fp);
			if(nReadSize==0) break;

			fwrite(szTempBuf, 1, nReadSize, fpWrite);
			if(feof(fp)) break;
		}while(true);

		fclose(fpWrite);
		fclose(fp);

		//��ȡ���е�PatchInfo�ļ�����
		char szPatchInfoFile[MAX_PATH];
		_snprintf(szPatchInfoFile, MAX_PATH, "%stlbb_patchinfo_%s_%s.txt", 
			szTempPath, g_szVersionFrom, g_szVersionTo);
		AXP::IUpdater* pUpdater = AXP::createUpdater();
		if(ExtractFileFromZip(pUpdater, g_szPatchZip, "Update.txt", szPatchInfoFile))
		{
			FILE* fpPatch = fopen(szPatchInfoFile, "rb");
			if(fpPatch)
			{
				fseek(fpPatch, 0, SEEK_END);
				int nSize = ftell(fpPatch);
				fseek(fpPatch, 0, SEEK_SET);

				char* pTxtBuf = new char[nSize+1];
				memset(pTxtBuf, 0, nSize+1);
				fread(pTxtBuf, 1, nSize, fpPatch);
				fclose(fpPatch); fpPatch=0;

				//send to ui
				::SendDlgItemMessage(g_hMainWnd, IDC_EDIT_PATCHINFO, WM_SETTEXT, 
					0, (LPARAM)pTxtBuf);

				delete[] pTxtBuf; pTxtBuf=0;
				::DeleteFile(szPatchInfoFile);
			}
		}
		AXP::destroyUpdater(pUpdater); pUpdater=0;
		return true;
	}while(0);

	//error!
	fclose(fp);
	return false;
}

void __stdcall _updateCallBack(const char *szPakName, const char* szFileName, AXP::UPDATE_PROCESS process)
{
	static int s_nIndex = 0;

	int nProgress = (int)((s_nIndex++)*100.f/g_nUpdateFileCount + .5f);

	char strMsg[MAX_PATH] = {0};
	switch(process)
	{
	case AXP::UP_NEW_FILE:
		_snprintf(strMsg, MAX_PATH, "�����ļ�%s", szFileName);
		break;

	case AXP::UP_DEL_FILE:
		_snprintf(strMsg, MAX_PATH, "ɾ���ļ�%s", szFileName);
		break;

	default:
		_snprintf(strMsg, MAX_PATH, "%s", szFileName);
		break;
	}
	
	::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
			0, (LPARAM)strMsg);
	::SendMessage(g_hMainWnd, WM_MSG_SETPROGRESS, (WPARAM)nProgress, 0);
}

//������Ϸ�汾
bool _updateGame(void)
{
	AXP::IUpdater* pUpdater = AXP::createUpdater();

	//����������
	if(!pUpdater->addPatchFile(g_szPatchZip))
	{
		::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
			0, (LPARAM)"��������Դ����");
		AXP::destroyUpdater(pUpdater);
		return false;
	}

	//�õ���Ҫ���µ��ļ���
	g_nUpdateFileCount = pUpdater->getUpdateFile();
	::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
		0, (LPARAM)"׼����ʼ����");

	//���ñ���
	char szTemp[MAX_PATH] = {0};
	_snprintf(szTemp, MAX_PATH, "�������˲�����������[%s-%s]", 
		g_szVersionFrom, g_szVersionTo);
	::SendMessage(g_hMainWnd, WM_SETTEXT, 0, (LPARAM)szTemp);

	//׼��������
	::SendMessage(g_hMainWnd, WM_MSG_SETPROGRESS, 0, 0);

	//�ȴ��û�ȷ��
	::EnableWindow(::GetDlgItem(g_hMainWnd, IDOK), TRUE);
	::WaitForSingleObject(g_hBeginHandle, INFINITE);

	//�����Ϸ����������
	if(checkIfGameRun(g_hMainWnd, true))
	{
		::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
			0, (LPARAM)"��Ϸ�ͻ�����������,�ڽ�������Ĳ���ǰ��رտͻ���!");
		AXP::destroyUpdater(pUpdater);
		return false;
	}

	//������������
	if(!(pUpdater->updateVersion(g_szGamePath, _updateCallBack)))
	{
		char szTemp[MAX_PATH] = {0};
		_snprintf(szTemp, MAX_PATH, "���´���(code=%d,%s)", 
			AXP::getLastError(), AXP::getLastErrorDesc());
		::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 0, (LPARAM)szTemp);
		AXP::destroyUpdater(pUpdater);
		return false;
	}

	AXP::destroyUpdater(pUpdater);
	pUpdater=0;

	//���������ļ������Ժ��޸�����ʹ��
	char szGamePatchFile[MAX_PATH];
	strncpy(szGamePatchFile, g_szGamePath, MAX_PATH);
	::PathAppend(szGamePatchFile, "Patch");
	::PathAppend(szGamePatchFile, ::PathFindFileName(g_szPatchZip));
	::DeleteFile(szGamePatchFile);
	::MoveFile(g_szPatchZip, szGamePatchFile);

	return true;
}

//�������ģʽ(call by main thread)
void _enterFaultMode(void)
{
	g_bFaultMode=true;
	//����ok��ť
	::EnableWindow(::GetDlgItem(g_hMainWnd, IDOK), FALSE);
	//��ʾȡ����ť
	::EnableWindow(::GetDlgItem(g_hMainWnd, IDCANCEL), TRUE);
	//�ػ�
	::InvalidateRect(g_hMainWnd, 0, TRUE);

	//��ʾ�رհ�ť
	HMENU hMenu = ::GetSystemMenu(g_hMainWnd, FALSE);
	::EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND|MF_ENABLED);
}

UINT __stdcall _WorkThread(LPVOID pParam)
{
	//��ȡ��������
	if(!_readPatchContents())
	{
		::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
			0, (LPARAM)"��ȡ������Դʧ�ܣ�����������!");
		::SendMessage(g_hMainWnd, WM_MSG_ENTERFAULT, 0, 0);
		return 0;
	}

	//Ѱ����ϷĿ¼�Ͱ汾
	if(!_searchGamePath())
	{
		::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
			0, (LPARAM)"�޷��ҵ��������˲�����װĿ¼!");
		::SendMessage(g_hMainWnd, WM_MSG_ENTERFAULT, 0, 0);
		return 0;
	}
	
	//�жϰ汾�Ƿ�֧��
	if(stricmp(g_szGameVersion_nor, g_szVersionFrom_nor) < 0)
	{
		char szError[MAX_PATH] = {0};
		_snprintf(szError, MAX_PATH, "�ò���֧��%s���ϵİ汾,���ذ汾Ϊ%s", 
			g_szVersionFrom, g_szGameVersion);
		::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
			0, (LPARAM)szError);
		::SendMessage(g_hMainWnd, WM_MSG_ENTERFAULT, 0, 0);
		return 0;
	}
	if(stricmp(g_szGameVersion_nor, g_szVersionTo_nor) > 0)
	{
		char szError[MAX_PATH] = {0};
		_snprintf(szError, MAX_PATH, "�ò���֧��%s���µİ汾,���ذ汾Ϊ%s", 
			g_szVersionTo, g_szGameVersion);
		::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
			0, (LPARAM)szError);
		::SendMessage(g_hMainWnd, WM_MSG_ENTERFAULT, 0, 0);
		return 0;
	}

	//��ʼ����
	if(!_updateGame())
	{
		::SendMessage(g_hMainWnd, WM_MSG_ENTERFAULT, 0, 0);
		return 0;
	}

	//����
	::SendDlgItemMessage(g_hMainWnd, IDC_STATIC_STATUS, WM_SETTEXT, 
			0, (LPARAM)"���½���");
	::SendMessage(g_hMainWnd, WM_MSG_SUCCESS, 0, 0);

	return 0;
}

//������Ϸ
void RunLaunch(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	
	char szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s", g_szGamePath);
	::PathAppend(szCmdLine, "Launch.exe");

	//��������
	::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, g_szGamePath, &si, &pi);
}

INT_PTR CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDCANCEL:
				if(g_bSuccess) RunLaunch();
				PostQuitMessage(0);
				break;
				
			case IDOK:
				HMENU hMenu = ::GetSystemMenu(hwndDlg, FALSE);
				::EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
				::EnableWindow(::GetDlgItem(hwndDlg, IDOK), FALSE);
				::EnableWindow(::GetDlgItem(hwndDlg, IDCANCEL), FALSE);
				if(g_hBeginHandle) SetEvent(g_hBeginHandle);
				break;
			}
		}
		break;

	case WM_MSG_ENTERFAULT:
		_enterFaultMode();
		break;

	case WM_MSG_SETPROGRESS:
		{
			UINT nPosOld = (UINT)::SendDlgItemMessage(hwndDlg, IDC_PROGRESS_MAIN, PBM_GETPOS, 
				0, 0);
			UINT nPosNow = (UINT)wParam;
			if(nPosNow != nPosOld)
			{
				::SendDlgItemMessage(hwndDlg, IDC_PROGRESS_MAIN, PBM_SETPOS, nPosNow, 0);
			}
		}
		break;

	case WM_MSG_SUCCESS:
		{
			HMENU hMenu = ::GetSystemMenu(hwndDlg, FALSE);
			::EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND|MF_ENABLED);
			::EnableWindow(::GetDlgItem(hwndDlg, IDOK), FALSE);
			::EnableWindow(::GetDlgItem(hwndDlg, IDCANCEL), TRUE);
			::SetDlgItemText(hwndDlg, IDCANCEL, "ȷ��");
			::CloseHandle(g_hBeginHandle); g_hBeginHandle=0;
			::SendDlgItemMessage(hwndDlg, IDC_PROGRESS_MAIN, PBM_SETPOS, 100, 0);

			char szTemp[MAX_PATH] = {0};
			_snprintf(szTemp, MAX_PATH, "�������˲����ɹ����µ�%s!", g_szVersionTo);
			MessageBox(g_hMainWnd, szTemp, "�������˲���", MB_OK|MB_ICONINFORMATION);

			g_bSuccess = TRUE;
		}
		break;

	case WM_INITDIALOG:
		{
			g_hMainWnd = hwndDlg;

			//Center main window
			CenterDlg(hwndDlg);

			//����ͼ��
			HICON hMainIcon = LoadIcon(g_hInst, (LPCTSTR)IDD_DIALOG_MAIN);
			::SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hMainIcon);
			::SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hMainIcon);

			//set status
			::SetDlgItemText(hwndDlg, IDC_STATIC_STATUS, "��ȡ�汾��Ϣ...");
			::SendDlgItemMessage(hwndDlg, IDC_PROGRESS_MAIN, PBM_SETRANGE, 
				0, MAKELPARAM(0, 100));

			g_hBeginHandle = ::CreateEvent(0, TRUE, FALSE, 0);
			::ResetEvent(g_hBeginHandle);

			//create work thread
			UINT nThreadID;
			g_hWorkThread = (HANDLE)::_beginthreadex(NULL, 0, _WorkThread, 0, CREATE_SUSPENDED|THREAD_QUERY_INFORMATION, &nThreadID );
			if(g_hWorkThread == NULL)
			{
				::SetDlgItemText(hwndDlg, IDC_STATIC_STATUS, "�����߳�ʧ��");
				_enterFaultMode();
				break;
			}

			//go!
			::ResumeThread(g_hWorkThread);
			//not care
			::CloseHandle(g_hWorkThread); g_hWorkThread=0;
		}
		break;

	case WM_CTLCOLORSTATIC:
		{
			HWND hStatic = (HWND)lParam;

			if(g_bFaultMode && hStatic == GetDlgItem(hwndDlg, IDC_STATIC_STATUS))
			{
				HBRUSH hBrush = (HBRUSH)(INT_PTR)::DefWindowProc(hwndDlg, WM_CTLCOLORSTATIC, wParam, lParam);

				SetTextColor((HDC)wParam, RGB(255, 0, 0));
				return (BOOL)(INT)(INT_PTR)hBrush;
			}
			else return FALSE;
		}
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return FALSE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_hInst = hInstance;

	::DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, MainDlgProc);

	return 0;
}