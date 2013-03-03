#include "StdAfx.h"

#include "Global.h"

HINSTANCE	g_hInstance = 0;
HWND		g_hwndDownload = 0;
char		g_szSogouExe[MAX_PATH] = {0};
char		g_szDownURL[MAX_PATH] = {0};
char		g_szLocalFile[MAX_PATH] = {0};
char		g_szInstallPath[MAX_PATH] = {0};