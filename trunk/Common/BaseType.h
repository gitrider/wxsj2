/////////////////////////////////////////////////////////////////////////////////
//文件名称：BaseType.h
//功能描述：系统底层类型基础文件，无授权人员不得修改此文件内容
//版本说明：Windows操作系统需要定义宏：__WINDOWS__
//			Linux操作系统需要定义宏：__LINUX__
//修改情况：2005-11-1 创建
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __BASETYPE_H__
#define __BASETYPE_H__



/////////////////////////////////////////////////////////////////////////////////
//网络信息预定义宏
/////////////////////////////////////////////////////////////////////////////////
#ifndef FD_SETSIZE
#define FD_SETSIZE      4096
#endif /* FD_SETSIZE */


/////////////////////////////////////////////////////////////////////////////////
//当前包含的系统头文件引用
/////////////////////////////////////////////////////////////////////////////////
#if defined(__WINDOWS__)
	#pragma warning ( disable : 4786 )
	#include <Windows.h>
	#include "crtdbg.h"
#elif defined(__LINUX__)
	#include <sys/types.h>
	#include <pthread.h>
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <stdarg.h>

using namespace std;

///////////////////////////////////////////////////////////////////////
//标准数据类型定义
///////////////////////////////////////////////////////////////////////
#define VOID			void			//标准空
typedef unsigned char	UCHAR;			//标准无符号CHAR
typedef char			CHAR;			//标准CHAR
typedef unsigned int	UINT;			//标准无符号INT
typedef int				INT;			//标准INT
typedef unsigned short	USHORT;			//标准无符号short
typedef short			SHORT;			//标准short
typedef unsigned long	ULONG;			//标准无符号LONG(不推荐使用)
typedef long			LONG;			//标准LONG(不推荐使用)
typedef float			FLOAT;			//标准float

typedef UCHAR			uchar;
typedef USHORT			ushort;
typedef UINT			uint;
typedef ULONG			ulong;
typedef ULONG			IP_t;
typedef USHORT			PacketID_t;
typedef INT				BOOL;
typedef UCHAR			BYTE;
typedef	BYTE			BPacketID_t;




///////////////////////////////////////////////////////////////////////
//基本数据宏定义
///////////////////////////////////////////////////////////////////////

//IP地址的字符最大长度
#define IP_SIZE			24
//无效的句柄
#define INVALID_HANDLE	-1
//无效的ID值
#define INVALID_ID		-1
//TAB表当中无效的配置值(新方案应该采取这个值.旧有值(-1)在很多技能当中会使用)
#define INVALID_PARAM_ID (-9999)

//真
#ifndef TRUE
	#define TRUE 1
#endif
//假
#ifndef FALSE
	#define FALSE 0
#endif
//文件路径的字符最大长度
#ifndef _MAX_PATH
	#define _MAX_PATH 260
#endif

#ifndef UINT_MAX
#define UINT_MIN	(0)
#define UINT_MAX	(0xFFFFFFFF)
#endif

#ifndef INT_MAX
#define INT_MIN		(-2147483648)
#define INT_MAX		(2147483647)
#endif

#ifndef DWORD_MAX
#define DWORD_MIN	UINT_MIN
#define DWORD_MAX	UINT_MAX
#endif

#ifndef USHORT_MAX
#define USHORT_MIN	(0)
#define USHORT_MAX	(0xFFFF)
#endif

#ifndef SHORT_MAX
#define SHORT_MIN	(-32768)
#define SHORT_MAX	(32767)
#endif

#ifndef WORD_MAX
#define WORD_MIN	USHORT_MIN
#define WORD_MAX	USHORT_MAX
#endif

#ifndef UCHAR_MAX
#define UCHAR_MIN	(0)
#define UCHAR_MAX	(0xFF)
#endif

#ifndef CHAR_MAX
#define CHAR_MIN	(-128)
#define CHAR_MAX	(127)
#endif

#ifndef BYTE_MAX
#define BYTE_MIN	UCHAR_MIN
#define BYTE_MAX	UCHAR_MAX
#endif

///////////////////////////////////////////////////////////////////////
//调试预定义宏定义
///////////////////////////////////////////////////////////////////////
#if defined(NDEBUG)
	#define __ENTER_FUNCTION_FOXNET if(1){
	#define __LEAVE_FUNCTION_FOXNET }
#else
	#define __ENTER_FUNCTION_FOXNET if(1){
	#define __LEAVE_FUNCTION_FOXNET }
#endif


#if defined(NDEBUG)
	#define _MY_TRY try
	#define _MY_CATCH catch(...)
#else
	#define _MY_TRY try
	#define _MY_CATCH catch(...)
#endif



//根据指针值删除内存
#ifndef SAFE_DELETE
#if defined(__WINDOWS__)
#define SAFE_DELETE(x)	if( (x)!=NULL ) { Assert(_CrtIsValidHeapPointer(x));delete (x); (x)=NULL; }
#elif defined(__LINUX__)
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
#endif
//根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#if defined(__WINDOWS__)
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { Assert(_CrtIsValidHeapPointer(x));delete[] (x); (x)=NULL; }
#elif defined(__LINUX__)
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
#endif

//删除指针型数据(应尽量使用SAFE_DELETE_ARRAY)
#ifndef DELETE_ARRAY
#if defined(__WINDOWS__)
#define DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#elif defined(__LINUX__)
#define DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
#endif
//根据指针调用free接口
#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif
//根据指针调用Release接口
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif


#endif
