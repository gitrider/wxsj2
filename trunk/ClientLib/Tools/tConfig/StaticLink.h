
#pragma once

struct AX_URL_LNK
{
	const char	szLnk[MAX_PATH];
	WNDPROC		m_oldWndProc;
	BOOL		m_bCallOldProc;
	void*		pUserData;
	const wchar_t*	wszToDisplay;
};

LRESULT CALLBACK StaticLnkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

void CreateLinkFont(HWND hDlg);
void DeleteLinkFont(void);
