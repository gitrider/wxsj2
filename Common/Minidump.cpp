#include "stdafx.h"
#include "Minidump.h"

#if defined(__WINDOWS__)
#include <stdio.h>
#include <stdlib.h>
#include <Dbghelp.h>
#pragma comment( lib, "DbgHelp" )
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

extern char * g_dump_exe_name;

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	CHAR szhostname[MAX_PATH] = {0};
	gethostname(szhostname, MAX_PATH - 1);
	CHAR szCrashFile[MAX_PATH] = {0};
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	_snprintf(szCrashFile, MAX_PATH, "%s_crash_%s_%02d%02d%02d%02d.dmp", 
		g_dump_exe_name, szhostname, timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute,timeNow.wSecond);

	HANDLE lhDumpFile = CreateFile(szCrashFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
	if(lhDumpFile)
	{
		MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
		loExceptionInfo.ExceptionPointers = ExceptionInfo;
		loExceptionInfo.ThreadId = GetCurrentThreadId();
		loExceptionInfo.ClientPointers = TRUE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
		CloseHandle(lhDumpFile);

		char szCommand[MAX_PATH] = {0};
		PathAppend(szCommand, "upload_dump.bat ");
		strcat(szCommand, szCrashFile);
		system(szCommand);	
	}
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif