// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINDOWS	(0x0410)	//Windows 98
#define _WIN32_WINNT	(0x0500)	//Windows 2000

// Windows Header Files:
#include <windows.h>
#include <windowsx.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Standard C++ Library
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <hash_map>
#include <algorithm>

// TODO: reference additional headers your program requires here
#include "GIBasicType.h"
#include "AxTrace.h"
