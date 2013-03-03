
/** 异常相关
 */

#pragma once


#include "GIBasicType.h"


// ref from http://www.codeproject.com/tools/minidump.asp 
//generate stack frame pointers for all functions - same as /Oy- in the project
#pragma optimize("y", off)


//---------------------------------------------------------
// 分析SEH异常
VOID KYLIN_API tProcessException(PEXCEPTION_POINTERS pException, bool bRunCrashReport);


//---------------------------------------------------------
// 异常应用

// 抛出字符串形式异常
VOID KYLIN_API tThrowStringException(LPCTSTR szFmt, ...);
// 抛出断言形式异常
VOID KYLIN_API tThrowAssertException(LPCTSTR szFileName, INT nFileLine, LPCTSTR pExpr);
// 显示一个内部异常
INT KYLIN_API tProcessInnerException(PEXCEPTION_POINTERS pException, HWND hParentWnd, LPCTSTR szTitle);

#if 1
	// 抛出一个字符串类型异常
	#define KLThrow	tThrowStringException

	// 断言判断
	#define KLAssert( bCondition) \
		do { \
				if( (bCondition) == 0 ) \
				{ \
					tThrowAssertException(_T(__FILE__), __LINE__, _T(#bCondition)); \
				} \
			} while(0);

#else
	// 抛出一个字符串类型异常
	#define KLThrow	tThrowStringException

	// 断言判断 Release版不做任何事情
	#define KLAssert(bCondition) (VOID)(0)
#endif

				
