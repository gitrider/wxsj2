/********************************************************************
	created:	2008/02/28
	created:	28:2:2008   18:45
	filename: 	f:\Tester\kpmLib\include\kpmPlatform.h
	file path:	f:\Tester\kpmLib\include
	file base:	kpmPlatform
	file ext:	h
	author:		zpxiang
	
	purpose:	
*********************************************************************/
#ifndef _KPM_PLATFORM_H_
#define _KPM_PLATFORM_H_

#include <string>

namespace KPM
{
using namespace std;
#if (defined(WIN32) || defined(WIN64))

	// In MSVC 8.0, there are some functions declared as deprecated.
#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#include <assert.h>      
#include <windows.h>      
#define  PLATFORM_LITTLE_ENDIAN  1

#ifdef WIN64
#define PLATFORM_64BIT
#else
#define PLATFORM_32BIT
#endif

#endif


#if !defined(WIN32) && !defined(WIN64)

	// Typedefs for ANSI C
	typedef unsigned char  BYTE;
	typedef short          SHORT;
	typedef unsigned short WORD;
	typedef unsigned short USHORT;
	typedef long           LONG;
	typedef unsigned long  DWORD;
	typedef unsigned long  DWORD_PTR;
	typedef long           LONG_PTR;
	typedef long           INT_PTR;
	typedef long long      LONGLONG;
#ifndef __OBJC__
#define BOOL           bool
#endif
	typedef void         * HANDLE;
	typedef void         * LPOVERLAPPED; // Unsupported on Linux and Mac
	typedef char           TCHAR;
	typedef unsigned long  LCID;

	typedef struct _FILETIME
	{ 
		DWORD dwLowDateTime; 
		DWORD dwHighDateTime; 
	}
	FILETIME, *PFILETIME;

	typedef union _LARGE_INTEGER
	{
#if PLATFORM_LITTLE_ENDIAN
		struct
		{
			DWORD LowPart;
			LONG HighPart;
		};
#else
		struct
		{
			LONG HighPart;
			DWORD LowPart;
		};
#endif
		LONGLONG QuadPart;
	}
	LARGE_INTEGER, *PLARGE_INTEGER;

	// Some Windows-specific defines
#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#define VOID     void
#define WINAPI 

#define FILE_BEGIN    SEEK_SET
#define FILE_CURRENT  SEEK_CUR
#define FILE_END      SEEK_END

#define CREATE_NEW    1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS   4

#define FILE_SHARE_READ 0x00000001L
#define GENERIC_WRITE   0x40000000
#define GENERIC_READ    0x80000000

#define FILE_FLAG_DELETE_ON_CLOSE         1   // Sam: Added these two defines so it would compile.
#define FILE_FLAG_SEQUENTIAL_SCAN         2

#define ERROR_SUCCESS                     0
#define ERROR_INVALID_FUNCTION            1
#define ERROR_FILE_NOT_FOUND              2
#define ERROR_ACCESS_DENIED               5
#define ERROR_NOT_ENOUGH_MEMORY           8
#define ERROR_BAD_FORMAT                 11
#define ERROR_NO_MORE_FILES              18
#define ERROR_GEN_FAILURE                31
#define ERROR_HANDLE_EOF                 38
#define ERROR_HANDLE_DISK_FULL           39
#define ERROR_NOT_SUPPORTED              50
#define ERROR_INVALID_PARAMETER          87
#define ERROR_DISK_FULL                 112
#define ERROR_CALL_NOT_IMPLEMENTED      120
#define ERROR_ALREADY_EXISTS            183
#define ERROR_CAN_NOT_COMPLETE         1003
#define ERROR_PARAMETER_QUOTA_EXCEEDED 1283
#define ERROR_FILE_CORRUPT             1392
#define ERROR_INSUFFICIENT_BUFFER      4999

#define INVALID_HANDLE_VALUE ((HANDLE) -1)

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

#define _stricmp strcasecmp
#define _strnicmp strncasecmp

	extern int globalerr;

	void  SetLastError(int err);
	int   GetLastError();
	char *ErrString(int err);

	// Emulation of functions for file I/O available in Win32
	HANDLE CreateFile(const char * lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, void * lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	BOOL   CloseHandle(HANDLE hObject);

	DWORD  GetFileSize(HANDLE hFile, DWORD * lpFileSizeHigh);
	DWORD  SetFilePointer(HANDLE, LONG lDistanceToMove, LONG * lpDistanceToMoveHigh, DWORD dwMoveMethod);
	BOOL   SetEndOfFile(HANDLE hFile);

	BOOL   ReadFile(HANDLE hFile, void * lpBuffer, DWORD nNumberOfBytesToRead, DWORD * lpNumberOfBytesRead, void * lpOverLapped);
	BOOL   WriteFile(HANDLE hFile, const void * lpBuffer, DWORD nNumberOfBytesToWrite, DWORD * lpNumberOfBytesWritten, void * lpOverLapped);

	BOOL   IsBadReadPtr(const void * ptr, int size);
	DWORD  GetFileAttributes(const char * szileName);

	BOOL   DeleteFile(const char * lpFileName);
	BOOL   MoveFile(const char * lpFromFileName, const char * lpToFileName);
	void   GetTempPath(DWORD szTempLength, char * szTemp);
	void   GetTempFileName(const char * lpTempFolderPath, const char * lpFileName, DWORD something, char * szLFName);

#define strnicmp strncasecmp

#endif // !WIN32




};



#endif