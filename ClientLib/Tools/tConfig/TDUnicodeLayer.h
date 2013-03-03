/****************************
	部分在98下必须通过
Microsoft Layer for Unicode 调用的函数接口
*****************************/

#pragma once

//SetWindowTextW
BOOL TD_SetWindowTextW(HWND hWnd, LPCWSTR lpString);

//MessageBoxW
int  TD_MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);

//TextOutW
BOOL TD_TextOutW(HDC hdc, int nXStart, int nYStart, LPCWSTR lpString, int cbString);

