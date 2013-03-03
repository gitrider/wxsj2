// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINDOWS	(0x0410)	//Windows 98
#define _WIN32_WINNT	(0x0500)	//Windows 2000
#define _WIN32_IE		(0x0400)	//为 IE 5.0 及更新版本改变为适当的值。


// Windows Header Files:
#include <windows.h>
#include <windowsx.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <io.h>

// Standard C++ Library
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <hash_map>
#include <algorithm>

