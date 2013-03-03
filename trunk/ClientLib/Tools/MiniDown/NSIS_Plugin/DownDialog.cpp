#include "StdAfx.h"

#include "DownDialog.h"
#include "Global.h"
#include "Resource.h"
#include "Global.h"
#include "ExDll.h"
#include "AutoDownload.h"


DOWN_RESULT g_theDownResult		= DR_BUSYING;
void *lpWndProcOld				= 0;
CAutoDownload* g_pAutoDownload	= 0;


// Callback: Handles the UserPass dialog
BOOL CALLBACK procDlgDownload(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE s_hSetupProcess = 0;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			//PBM_SETRANGE32
			::SendDlgItemMessage(hWndDlg, IDC_PROGRESS_DOWNLOAD, (WM_USER+1), 0,  MAKELPARAM(0, 100));
			::SetDlgItemText(hWndDlg, IDC_STATIC_DESC, "��������P2P����...");

			char szCurrentDir[MAX_PATH] = {0};
			::GetModuleFileName(g_hInstance, szCurrentDir, MAX_PATH);
			::PathRemoveFileSpec(szCurrentDir);

			//���ɽ���
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si,sizeof(STARTUPINFO));
			ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

			si.cb = sizeof(STARTUPINFO);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_SHOWNORMAL;

			char szCmdLine[MAX_PATH] = {0};
			_snprintf(szCmdLine, MAX_PATH, "%s /S", g_szSogouExe);

			//����sogou��װ����
			if(::CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_CONSOLE, NULL, szCurrentDir, &si, &pi))
			{
				//��ʼsetup tick
				s_hSetupProcess = pi.hProcess;
				::SetTimer(hWndDlg, 201, 100, 0);
			}
		}
		break;

	case WM_TIMER:
		{
			if(wParam == 201) //setup tick
			{
				//�ȴ����̽���
				if(WAIT_OBJECT_0 == ::WaitForSingleObject(s_hSetupProcess, 0))
				{
					//close setup tick
					::KillTimer(hWndDlg, 201);

					//���Դ���soda������
					g_pAutoDownload = CAutoDownload::createInstance(CAutoDownload::AT_SODA);
					//ʧ��,�������û�û�гɹ���װSogou
					if(0 == g_pAutoDownload)
					{
						g_pAutoDownload = CAutoDownload::createInstance(CAutoDownload::AT_ANT); 
					}

					//�������ع���
					::GetModuleFileName(::GetModuleHandle(0), g_szLocalFile, MAX_PATH);
					::PathRemoveFileSpec(g_szLocalFile);
					::PathAppend(g_szLocalFile, ::PathFindFileName(g_szDownURL));

					g_pAutoDownload->init(g_szDownURL, g_szLocalFile, g_hwndDownload);
					g_pAutoDownload->begin();

					// Enable Cancel button
					HWND hCancel = GetDlgItem(g_hwndParent, IDCANCEL);
					::EnableWindow(hCancel, TRUE);

					//start down tick
					::SetTimer(hWndDlg, 101, 100, 0);
				}
			}
			else if(wParam == 101)//down tick
			{
				//��������Ƿ����
				if(!g_pAutoDownload || !(g_pAutoDownload->waitStop(false))) break;
				//���ؽ���
				::KillTimer(hWndDlg, 101);
				//�����Ƿ�ɹ�
				g_theDownResult = (CAutoDownload::AC_FINISH_SUCCESS == g_pAutoDownload->getStateCategory()) ? 
					DR_SUCCESS : DR_ERROR;
			}
		}
		break;

	//���ؿ�ʼ
	case CAutoDownload::AM_BEGIN:
		{

		}
		break;

	case CAutoDownload::AM_STATUSUPDATE:
		{
			//��ʼ���ӷ�����...
			if(g_pAutoDownload->getState() == CAutoDownload::AS_BUSY_QUERY)
			{
				char szMsg[MAX_PATH] = {0};
				_snprintf(szMsg, MAX_PATH, "��ʼ���� %s...", g_pAutoDownload->getUrlFileName().c_str());
				::SetDlgItemText(hWndDlg, IDC_STATIC_DESC, szMsg);
			}
		}
		break;

	case CAutoDownload::AM_SIZEUPDATE:
		{
			__int64 nFileSize, nReadSize;

			//ˢ�½�����
			g_pAutoDownload->getFileSize(nFileSize, nReadSize);
			//PBM_SETPOS
			::SendDlgItemMessage(hWndDlg, IDC_PROGRESS_DOWNLOAD, (WM_USER+2), (WPARAM)(int)(nReadSize*100.0/nFileSize), 0);

			//ˢ���ٶ���ʾ
			static DWORD nLast = 0;
			DWORD nTimeNow  = ::GetTickCount();
			if(nTimeNow - nLast > 1000)
			{
				int nSpeed = g_pAutoDownload->getSpeedInfo();

				char szMsg[MAX_PATH];
				_snprintf(szMsg, MAX_PATH, "�ļ���С: %s/%s �����ٶ�: %s",
						CAutoDownload::fileSize2String(nReadSize).c_str(), 
						CAutoDownload::fileSize2String(nFileSize).c_str(), 
						CAutoDownload::fileSize2String(nSpeed).c_str());

				::SetDlgItemText(hWndDlg, IDC_STATIC_DESC, szMsg);
				nLast = nTimeNow;
			}
		}
		break;

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORLISTBOX:
		// let the NSIS window handle colors, it knows best
		return (BOOL)SendMessage(g_hwndParent, uMsg, wParam, lParam);
	}
	return FALSE;
}

// Callback: Handles the parent dialog
BOOL CALLBACK procParentWnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bRes = (BOOL)::CallWindowProc((WNDPROC)lpWndProcOld,hWnd,uMsg,wParam,lParam);

	if(uMsg == WM_NOTIFY_OUTER_NEXT && !bRes)
	{
		//pause time tick
		::KillTimer(g_hwndDownload, 101);

		//�û������Cancel
		if (wParam == NOTIFY_BYE_BYE)
		{
			if(IDYES != MessageBox(hWnd, "ȷʵҪ��ֹ��ǰ��������?", "�������˲���", 
				MB_OK|MB_YESNO|MB_ICONQUESTION))
			{
				//resume time tick
				::SetTimer(g_hwndDownload, 101, 100, 0);
				return bRes;
			}
		}
		//Stop down work
		if(g_pAutoDownload)
		{
			g_pAutoDownload->stop();
			g_pAutoDownload->waitStop(true);
		}
		g_theDownResult = DR_CANCEL;
//		PostMessage(g_hwndDownload, WM_CLOSE, 0, 0);
	}
	return bRes;
}

// Displays our dialog
DOWN_RESULT ShowDownDialog(void)
{
	g_theDownResult = DR_BUSYING;

	g_hwndDownload = ::CreateDialog(
		g_hInstance, MAKEINTRESOURCE(IDD_DIALOG_DOWNLOAD), 
		g_hwndParent, (DLGPROC)procDlgDownload);

	// Disable Next button
	HWND hNext = GetDlgItem(g_hwndParent, IDOK);
	::EnableWindow(hNext, FALSE);

	// Disable Back button
	HWND hBack = GetDlgItem(g_hwndParent, IDABORT);
	::EnableWindow(hBack, FALSE);

	// Disable Cancel button
	HWND hCancel = GetDlgItem(g_hwndParent, IDCANCEL);
	::EnableWindow(hCancel, FALSE);

	RECT dialog_r;
	int mainWndWidth, mainWndHeight;

	// Get the sizes of the UI
	GetWindowRect(GetDlgItem(g_hwndParent, DEFAULT_RECT), &dialog_r);
	MapWindowPoints(0, g_hwndParent, (LPPOINT)&dialog_r, 2);

	mainWndWidth = dialog_r.right - dialog_r.left;
	mainWndHeight = dialog_r.bottom - dialog_r.top;

	// Set our window size to fit the UI size
	SetWindowPos(
		g_hwndDownload,
		0,
		dialog_r.left,
		dialog_r.top,
		mainWndWidth,
		mainWndHeight,
		SWP_NOZORDER|SWP_NOACTIVATE
		);

	lpWndProcOld = (void *)(LONG_PTR)SetWindowLongPtr(g_hwndParent, DWL_DLGPROC, (long)(LONG_PTR)procParentWnd);

	// Sets the font of IO window to be the same as the main window
	SendMessage(g_hwndDownload, WM_SETFONT, (WPARAM)SendMessage(g_hwndParent, WM_GETFONT, 0, 0), (LPARAM)TRUE);

	// Tell NSIS to remove old inner dialog and pass handle of the new inner dialog
	SendMessage(g_hwndParent, WM_NOTIFY_CUSTOM_READY, (WPARAM)g_hwndDownload, 0);
	ShowWindow(g_hwndDownload, SW_SHOWNA);

	// Loop until the user clicks on a button
	while(DR_BUSYING == g_theDownResult) 
	{
		MSG msg;
		int nResult = GetMessage(&msg, NULL, 0, 0);
		if (!IsDialogMessage(g_hwndDownload, &msg) && !IsDialogMessage(g_hwndParent, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if(DR_ERROR == g_theDownResult)
	{
		MessageBox(g_hwndDownload, "���������İ�װ����ʧ��!", "�������˲������߰�װ", MB_OK|MB_ICONSTOP);
	}

	//ɾ�����ع���
	CAutoDownload::destroyInsance(g_pAutoDownload);
	g_pAutoDownload = 0;

	// Set window dialog procedure back to NSIS's
	SetWindowLongPtr(g_hwndParent, DWL_DLGPROC, (long)(LONG_PTR)lpWndProcOld);

	return g_theDownResult;
}
