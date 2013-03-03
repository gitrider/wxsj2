//--------------------------------------------------------------------------------
//
// Filename   : Assert.h
// Written By : 
//
//--------------------------------------------------------------------------------

#ifndef __ASSERT__H__
#define __ASSERT__H__

// include files
#include "BaseType.h"

extern int g_Command_Assert ;//控制参数，不提示Assert的对话框，直接忽略
extern int g_Command_IgnoreMessageBox ;//控制参数，跳过MyMessageBox的中断
extern int g_FileNameFix ;//文件名称后缀

extern int g_NeedManagerDoPosInit;//控制参数，是否需要初始化管理器数据


//--------------------------------------------------------------------------------
//
// 
// 
//
//--------------------------------------------------------------------------------
VOID __assert__ (const CHAR* file, UINT line, const CHAR* func, const CHAR* expr) ;
VOID __assertex__ (const CHAR* file, UINT line, const CHAR* func, const CHAR* expr, const CHAR* msg) ;
VOID __assertspecial__ (const CHAR* file, UINT line, const CHAR* func, const CHAR* expr, const CHAR* msg) ;
VOID __messagebox__(const CHAR*msg ) ;
//--------------------------------------------------------------------------------
//
// 
// 
//
//--------------------------------------------------------------------------------
VOID __protocol_assert__ (const CHAR* file, UINT line, const CHAR* func, const CHAR* expr) ;

#if defined(NDEBUG)
	#define Assert(expr) ((VOID)0)
	#define AssertEx(expr,msg) ((VOID)0)
	#define AssertSpecial(expr,msg) ((VOID)0)
	#define MyMessageBox(msg) ((VOID)0)
#elif __LINUX__
	#define Assert(expr) {if(!(expr)){__assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr);}}
	#define ProtocolAssert(expr) ((VOID)((expr)?0:(__protocol_assert__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr),0)))
	#define AssertEx(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
	#define AssertSpecial(expr,msg) {if(!(expr)){__assertspecial__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
	#define AssertExPass(expr,msg) {if(!(expr)){__assertex__(__FILE__,__LINE__,__PRETTY_FUNCTION__,#expr,msg);}}
	#define MyMessageBox(msg) ((VOID)0)
#elif __WIN_CONSOLE__ || __WIN32__ || __WINDOWS__
	#define Assert(expr) ((VOID)((expr)?0:(__assert__(__FILE__,__LINE__,__FUNCTION__,#expr),0)))
	#define AssertEx(expr,msg) ((VOID)((expr)?0:(__assertex__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
	#define AssertSpecial(expr,msg) ((VOID)((expr)?0:(__assertspecial__(__FILE__,__LINE__,__FUNCTION__,#expr,msg),0)))
	#define MyMessageBox(msg) __messagebox__(msg)
#elif __MFC__
	#define Assert(expr) ASSERT(expr)
	#define AssertEx(expr,msg) ((VOID)0)
	#define AssertSpecial(expr,msg) ((VOID)0)
	#define MyMessageBox(msg) ((VOID)0)
#endif

//--------------------------------------------------------------------------------
// if nIndex was in [nFrom, nTo], this function will return TRUE. that means legal.
// If any illegal index found, this function will throw a except !!
// the szCodeLocation should be class_name::function_name, or something like this.
// Example: RangeCheckForIndex(i, 0, 10, "testclass::testfunction");
// Legal: 0<=i<=10;
// Illegal: i<0 or i>10;
//--------------------------------------------------------------------------------
BOOL RangeCheckForIndex_Assert(INT nIndex, INT nFrom, INT nTo, CHAR const* szCodeLocation);
#endif
