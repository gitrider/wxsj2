#include "StdAfx.h"
#include "StaticLink.h"
#include <process.h>
#include "TDUnicodeLayer.h"

HFONT g_hLinkFont = NULL;

UINT WINAPI ShellExecute_Proc( void* pUrl )
{
	char* pszUrl = (char*)pUrl;

	if(pszUrl[0] == '1')
	{
		//Open Url
		ShellExecuteA( GetDesktopWindow(), "open", ( LPCSTR )pUrl, NULL, NULL, SW_SHOW ) ;
	}
	else if(pszUrl[0] == '2')
	{
		//give tips

	}

	return 0 ;
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
			UINT threadID;
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ShellExecute_Proc, ( LPVOID )pData->szLnk, 0, &threadID ) ;
			CloseHandle( hThread ) ;

			pData->m_bCallOldProc = TRUE;
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

			const wchar_t* wszText = pData->wszToDisplay;
			TD_TextOutW( hDC, 0, 0, wszText, (INT)wcslen(wszText) ) ;

			EndPaint( hWnd, &ps ) ;

//			HWND hCWnd = GetCapture();
//			if ( hWnd == hCWnd )
//			{
//				ReleaseCapture() ;
//			}
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
				//logFont.lfUnderline = TRUE ;
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