#ifndef __MINIDUMP_H__
#define __MINIDUMP_H__

#if defined(__WINDOWS__)
#include <windows.h>

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo);

#endif

#endif