
#include "windows.h"
#include <windowsx.h>
#include "resource.h"
#include <process.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


struct AX_URL_LNK
{
	TCHAR		szText[64];
	TCHAR		szLnk[MAX_PATH];

	WNDPROC		m_oldWndProc ;
	BOOL		m_bCallOldProc ;

};

BOOL CALLBACK dlgproc(HWND,UINT, WPARAM,LPARAM);
LRESULT CALLBACK StaticLnkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
void CreateLinkFont(HWND hDlg);
void DeleteLinkFont(void);


HFONT g_hLinkFont = NULL;

static AX_URL_LNK g_BigInfoLnk = {
					"",
					"1", 
					NULL, TRUE};

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpszCmdLine,int nCmdShow)
{
	DialogBox(hInstance,(LPCSTR)IDD_DIALOG1,::GetDesktopWindow(),dlgproc);//对话框的调用
	return 0;
}

void runSogouPinyin(void)
{
	char szCommand[MAX_PATH];
	char szPath[MAX_PATH];

	::GetModuleFileName(0, szCommand, MAX_PATH);
	::PathRemoveFileSpec(szCommand);

	strncpy(szPath, szCommand, MAX_PATH);

	::PathAppend(szCommand, "sogou_pinyin_tianlong_3025.exe");

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
		szPath, &si, &pi) == FALSE)
	{
		//启动失败
		//TODO...
		return;
	}

}

BOOL CALLBACK dlgproc(HWND hdlg,UINT msg ,WPARAM wp,LPARAM lp)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			//HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,_T("..\\1.bmp"),IMAGE_BITMAP,640,480,LR_LOADFROMFILE);


			//m_PictureAA.SetBitmap (hBitmap1);


			CreateLinkFont(hdlg);

			::GetWindowText(GetDlgItem(hdlg, IDC_STATIC_LINK), g_BigInfoLnk.szText, 64);
			SetWindowLong(GetDlgItem(hdlg, IDC_STATIC_LINK), GWL_USERDATA, (LONG)(LONG_PTR)(&g_BigInfoLnk));
			g_BigInfoLnk.m_oldWndProc = (WNDPROC)(LONG_PTR)SetWindowLong(GetDlgItem(hdlg, IDC_STATIC_LINK), GWL_WNDPROC, (LONG)(LONG_PTR)StaticLnkWndProc);
		}
		return 1;
	case WM_COMMAND:
		switch(LOWORD(wp))
		{
		case IDOK:
			runSogouPinyin();
			DeleteLinkFont();
			EndDialog(hdlg,0);
			PostQuitMessage(0);
			return 1;
		}
		break;
	case WM_CLOSE:
		DeleteLinkFont();
		EndDialog(hdlg,0);
		PostQuitMessage(0);
		return 1;

	}
	return 0;
}

LRESULT CALLBACK StaticLnkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	AX_URL_LNK *pData = (AX_URL_LNK *)(LONG_PTR)GetWindowLong(hWnd, GWL_USERDATA);

	switch ( uMsg )
	{
	case WM_SETCURSOR :
		{
			SetCursor( LoadCursor( NULL, IDC_HAND ) ) ;
			pData->m_bCallOldProc = FALSE;
		}
		break ;

	case WM_LBUTTONUP :
		{
			ShellExecute(0, NULL, "http://tl.sohu.com/events/defence/", NULL,NULL, SW_NORMAL);
		}
		break ;

	case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			HDC hDC = BeginPaint( hWnd, &ps ) ;

			SelectObject( hDC, g_hLinkFont ) ;
			
			RECT rect ;
			GetWindowRect( hWnd, &rect ) ;

			DWORD dwPos = GetMessagePos() ;
			POINT pt ;
			pt.x = GET_X_LPARAM( dwPos ) ;
			pt.y = GET_Y_LPARAM( dwPos ) ;
			
			SetBkMode( hDC, TRANSPARENT );
//			if ( PtInRect( &rect, pt ) )
//			{
//				SetTextColor( hDC, RGB( 128, 0, 128 ) ) ;
//			}
//			else 
			{
				SetTextColor( hDC, RGB( 0, 0, 255 ) ) ;
			}
			
			SetBkColor( hDC, GetSysColor( COLOR_3DFACE ) ) ;
			
			AX_URL_LNK *pData = (AX_URL_LNK *)(LONG_PTR)GetWindowLong(hWnd, GWL_USERDATA);

			TextOut( hDC, 0, 0, pData->szText, (INT)strlen(pData->szText) ) ;

			EndPaint( hWnd, &ps ) ;

			pData->m_bCallOldProc = TRUE;
		}
		break;

	case WM_DESTROY:
		break ;
	}

	if(pData->m_bCallOldProc && pData->m_oldWndProc )
	{
		pData->m_oldWndProc(hWnd, uMsg, wParam, lParam);
	}
	pData->m_bCallOldProc = TRUE;

	return TRUE;
}

void CreateLinkFont(HWND hDlg)
{
    HFONT hFont = NULL ;
    LOGFONT logFont ;
	memset( &logFont, 0, sizeof( LOGFONT ) ) ;

	if( NULL == g_hLinkFont )
	{
		hFont = ( HFONT )SendMessage( hDlg, WM_GETFONT, 0, 0 ) ;
		if ( NULL != hFont )
		{
			if ( 0 != GetObject( hFont, sizeof( LOGFONT ), &logFont ) )
			{
				logFont.lfUnderline = TRUE ;
				g_hLinkFont = CreateFontIndirect( &logFont ) ;
			}
		}
	}
}

void DeleteLinkFont(void)
{
	if(g_hLinkFont)
	{
		DeleteObject(g_hLinkFont);
		g_hLinkFont=NULL;
	}
}
