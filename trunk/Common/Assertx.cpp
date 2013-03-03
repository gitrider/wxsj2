//--------------------------------------------------------------------------------
// 
// Filename   : Assert.cpp 
// Written By : 
//
//--------------------------------------------------------------------------------

// include files
#include "stdafx.h"
#include "Assertx.h"
#include "Type.h"
#include <time.h>
#ifdef __LINUX__
#include <execinfo.h>
#endif

int	g_Command_Assert=0 ;//���Ʋ�����0:��ͨ�������Ի������û�ѡ��(ȱʡֵ)
						//			1:����
						//			2:�����׳��쳣���ڻ�ȡ���ж�ջ
int g_Command_IgnoreMessageBox=FALSE ;//���Ʋ���������MyMessageBox���ж�

int g_NeedManagerDoPosInit=1;//���Ʋ������Ƿ���Ҫ��ʼ������������






VOID __show__( const CHAR* szTemp )
{

#ifdef __LINUX__
	printf("Assert:%s",szTemp);
#endif

	//������־
#ifndef GAME_CLIENT
	FILE* f = fopen( "./Log/assert.log", "a" ) ;
	fwrite( szTemp, 1, strlen(szTemp), f ) ;
	fwrite( "\r\n", 1, 2, f ) ;
	fclose(f) ;
#endif

#if defined(__WINDOWS__)
	static MyLock lock ;
	if( g_Command_Assert!=1 )
	{
		lock.Lock() ;
		INT iRet = ::MessageBoxA( NULL, szTemp, "�쳣", MB_OK ) ;
		lock.Unlock() ;
	}
#elif defined(__LINUX__)
	
#endif

#ifdef GAME_CLIENT
	throw(std::string(szTemp));
#else
	throw(1);
#endif
}

VOID __messagebox__(const CHAR*msg )
{
	if( g_Command_IgnoreMessageBox )
		return ;
#if defined(__WINDOWS__)
	::MessageBoxA( NULL, msg, "��Ϣ", MB_OK ) ;
#elif defined(__LINUX__)
#endif
}
//--------------------------------------------------------------------------------
//
// __assert__
//
//
//--------------------------------------------------------------------------------
VOID __assert__ ( const CHAR * file , UINT line , const CHAR * func , const CHAR * expr )
{
	CHAR szTemp[1024] = {0};
	
#ifdef __LINUX__ //������ʽ
	sprintf( szTemp, "[%s][%d][%s][%s]\n", file, line, func, expr ) ;
#else
	sprintf( szTemp, "[%s][%d][%s][%s]", file, line, func, expr ) ;
#endif
	__show__(szTemp) ;
}

VOID __assertex__ ( const CHAR * file , UINT line , const CHAR * func , const CHAR * expr ,const CHAR* msg)
{
	CHAR szTemp[1024] = {0};
	
#ifdef __LINUX__
	sprintf( szTemp, "[%s][%d][%s][%s]\n[%s]\n", file, line, func, expr ,msg ) ;
#else
	sprintf( szTemp, "[%s][%d][%s][%s]\n[%s]", file, line, func, expr ,msg ) ;
#endif
	__show__(szTemp) ;
}

VOID __assertspecial__ ( const CHAR * file , UINT line , const CHAR * func , const CHAR * expr ,const CHAR* msg)
{
	CHAR szTemp[1024] = {0};
	
#ifdef __LINUX__
	sprintf( szTemp, "S[%s][%d][%s][%s]\n[%s]\n", file, line, func, expr ,msg ) ;
#else
	sprintf( szTemp, "S[%s][%d][%s][%s]\n[%s]", file, line, func, expr ,msg ) ;
#endif
	__show__(szTemp) ;
}
//--------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------
VOID __protocol_assert__ ( const CHAR * file , UINT line , const CHAR * func , const CHAR * expr )
{
	printf( "[%s][%d][%s][%s]", file, line, func, expr ) ;
}

//--------------------------------------------------------------------------------
// if nIndex was in [nFrom, nTo], this function will return TRUE. that means legal.
// If any illegal index found, this function will throw a except !!
// the szCodeLocation should be class_name::function_name, or something like this.
// Example: RangeCheckForIndex(i, 0, 10, "testclass::testfunction");
// Legal: 0<=i<=10;
// Illegal: i<0 or i>10;
//--------------------------------------------------------------------------------
BOOL RangeCheckForIndex_Assert(INT nIndex, INT nFrom, INT nTo, CHAR const* szCodeLocation)
{
	INT nUpBorder=0;
	INT nLowBorder=0;
	CHAR szBuff[1024];
	if(nFrom > nTo)
	{
		nUpBorder = nFrom;
		nLowBorder = nTo;
	}
	else
	{
		nUpBorder = nTo;
		nLowBorder = nFrom;
	}
	Assert(0<=nUpBorder);
	Assert(0<=nLowBorder);
	Assert(0<=nIndex);
	if(nIndex>=nLowBorder && nIndex<=nUpBorder)
	{
		return TRUE;
	}
	tsnprintf(szBuff,
		sizeof(szBuff),
		"[%s] Index out of range!! nIndex=%d, but legal range is [%d, %d]. check it now, please!!",
		szCodeLocation,
		nIndex,
		nLowBorder,
		nUpBorder
		);
	szBuff[sizeof(szBuff)-1] = '\0';
	AssertEx(FALSE, szBuff);
	return FALSE;
}