/////////////////////////////////////////////////////////////////////////////////
//�ļ����ƣ�BaseType.h
//����������ϵͳ�ײ����ͻ����ļ�������Ȩ��Ա�����޸Ĵ��ļ�����
//�汾˵����Windows����ϵͳ��Ҫ����꣺__WINDOWS__
//			Linux����ϵͳ��Ҫ����꣺__LINUX__
//�޸������2005-11-1 ����
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __BASETYPE_H__
#define __BASETYPE_H__



/////////////////////////////////////////////////////////////////////////////////
//������ϢԤ�����
/////////////////////////////////////////////////////////////////////////////////
#ifndef FD_SETSIZE
#define FD_SETSIZE      4096
#endif /* FD_SETSIZE */


/////////////////////////////////////////////////////////////////////////////////
//��ǰ������ϵͳͷ�ļ�����
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
//��׼�������Ͷ���
///////////////////////////////////////////////////////////////////////
#define VOID			void			//��׼��
typedef unsigned char	UCHAR;			//��׼�޷���CHAR
typedef char			CHAR;			//��׼CHAR
typedef unsigned int	UINT;			//��׼�޷���INT
typedef int				INT;			//��׼INT
typedef unsigned short	USHORT;			//��׼�޷���short
typedef short			SHORT;			//��׼short
typedef unsigned long	ULONG;			//��׼�޷���LONG(���Ƽ�ʹ��)
typedef long			LONG;			//��׼LONG(���Ƽ�ʹ��)
typedef float			FLOAT;			//��׼float

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
//�������ݺ궨��
///////////////////////////////////////////////////////////////////////

//IP��ַ���ַ���󳤶�
#define IP_SIZE			24
//��Ч�ľ��
#define INVALID_HANDLE	-1
//��Ч��IDֵ
#define INVALID_ID		-1
//TAB������Ч������ֵ(�·���Ӧ�ò�ȡ���ֵ.����ֵ(-1)�ںܶ༼�ܵ��л�ʹ��)
#define INVALID_PARAM_ID (-9999)

//��
#ifndef TRUE
	#define TRUE 1
#endif
//��
#ifndef FALSE
	#define FALSE 0
#endif
//�ļ�·�����ַ���󳤶�
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
//����Ԥ����궨��
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



//����ָ��ֵɾ���ڴ�
#ifndef SAFE_DELETE
#if defined(__WINDOWS__)
#define SAFE_DELETE(x)	if( (x)!=NULL ) { Assert(_CrtIsValidHeapPointer(x));delete (x); (x)=NULL; }
#elif defined(__LINUX__)
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
#endif
//����ָ��ֵɾ�����������ڴ�
#ifndef SAFE_DELETE_ARRAY
#if defined(__WINDOWS__)
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { Assert(_CrtIsValidHeapPointer(x));delete[] (x); (x)=NULL; }
#elif defined(__LINUX__)
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
#endif

//ɾ��ָ��������(Ӧ����ʹ��SAFE_DELETE_ARRAY)
#ifndef DELETE_ARRAY
#if defined(__WINDOWS__)
#define DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#elif defined(__LINUX__)
#define DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
#endif
//����ָ�����free�ӿ�
#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif
//����ָ�����Release�ӿ�
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif


#endif
