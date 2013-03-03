#include "StdAfx.h"
#include "TDUnicodeLayer.h"

BOOL TD_IsWindowNT(void)
{
	OSVERSIONINFO Ver;
	ZeroMemory(&Ver, sizeof(OSVERSIONINFO));
	Ver.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);

	GetVersionEx(&Ver);
	return (VER_PLATFORM_WIN32_NT==Ver.dwPlatformId);
}

//--------------------------------------------------------------------------
BOOL TD_SetWindowTextW(HWND hWnd, LPCWSTR lpString)
{
	typedef BOOL (WINAPI* LPSetWindowTextW)(HWND hWnd, LPCWSTR lpString);

	LPSetWindowTextW lpFn = (LPSetWindowTextW)::GetProcAddress(
		::LoadLibraryA(TD_IsWindowNT() ? "user32.dll" : "unicows.dll"),
		"SetWindowTextW");

	if(lpFn)return lpFn(hWnd, lpString);

	return 0;	
}

//--------------------------------------------------------------------------
int TD_MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	typedef int (WINAPI* LPMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);

	LPMessageBoxW lpFn = (LPMessageBoxW)::GetProcAddress(
		::LoadLibraryA(TD_IsWindowNT() ? "user32.dll" : "unicows.dll"),
		"MessageBoxW");

	if(lpFn)return lpFn(hWnd, lpText, lpCaption, uType);

	return 0;	
}

//--------------------------------------------------------------------------
BOOL TD_TextOutW(HDC hdc, int nXStart, int nYStart, LPCWSTR lpString, int cbString)
{
	typedef BOOL (WINAPI* LPTextOutW)(HDC hdc, int nXStart, int nYStart, LPCWSTR lpString, int cbString);

	LPTextOutW lpFn = (LPTextOutW)::GetProcAddress(
		::LoadLibraryA(TD_IsWindowNT() ? "gdi32.dll" : "unicows.dll"),
		"TextOutW");

	if(lpFn)return lpFn(hdc, nXStart, nYStart, lpString, cbString);

	return 0;	
}

