/////////////////////////////////////////////////////////////////////////////////
//文件名称：BaseType.h
//开发者：Jack
//功能描述：系统底层类型基础文件，无授权人员不得修改此文件内容
//版本说明：Windows操作系统需要定义宏：__WINDOWS__
//			Linux操作系统需要定义宏：__LINUX__
//修改情况：2005-11-1 创建
//			2007-0323 增加FD_SETSIZE的尺寸
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
	#include <stdarg.h>
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>

#ifndef FLT_MAX
#define FLT_MAX         3.402823466e+38F
#endif

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
typedef float           FLOAT;			//标准浮点
typedef double			DOUBLE;			//标准双精度浮点
typedef UCHAR			uchar;
typedef USHORT			ushort;
typedef UINT			uint;
typedef ULONG			ulong;
typedef ULONG			IP_t;
typedef USHORT			PacketID_t;
typedef UCHAR			BYTE;
typedef	BYTE			BPacketID_t;
typedef INT				BOOL;
typedef long long		i64;



///////////////////////////////////////////////////////////////////////
//基本数据宏定义
///////////////////////////////////////////////////////////////////////

//IP地址的字符最大长度
#define IP_SIZE			24
//无效的句柄
#define INVALID_HANDLE	-1
//无效的ID值
#define INVALID_ID		-1
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
//SHORT
#ifndef SHORT_MAX
	#define SHORT_MAX ((short)(((unsigned short)-1)>>1))
	#define SHORT_MIN (~SHORT_MAX)
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
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
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
