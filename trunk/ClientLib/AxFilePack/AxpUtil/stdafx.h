// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINDOWS	(0x0410)	//Windows 98
#define _WIN32_WINNT	(0x0500)	//Windows 2000

#include <tchar.h>
#include <conio.h>
#include <io.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include <windows.h>
#include <windowsx.h>

#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <AXP.h>