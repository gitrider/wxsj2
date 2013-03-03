//////////////////////////////////////////////////////
//
//	AIXA|Trace2 (C) Copyright �_Ԫ
//
//	ʹ�÷���:
//		1. ���� AxTrace.h AxTrace.cpp��������,���齫AxTrace.h������Ԥ����ͷStdAfx.h��
//
//		2. ʹ�� AxTrace ���������Ϣ
//
//			VOID AxTrace(unsigned char nWnd, unsigned char nType, const char *pszFmt, ...);
//					nWnd		-- ������Ϣ���Ӵ��ڱ��
//					nType		-- ��Ϣ��ʾ���ͣ����ڽ��մ���������ʾ����Ϣ��������
//					pszFmt,...	-- ��Ϣ����
//			����:
//					AxTrace(0, 1, "nIndex=%d", nIndex);
//
//		3. ʹ�� AxVar ������������Ϣ
//			VOID AxVar(unsigned char nWnd, unsigned char nType, AXVALUE nVariableType, const char *szValueName, VOID* pValue);
//					nWnd			-- ���ձ�����ص��Ӵ��ڱ��
//					nType			-- ��Ϣ��ʾ���ͣ����ڽ��մ���������ʾ����Ϣ��������
//					nVariableType	-- ��������
//					szValueName		-- ��������
//					pValue			-- ָ�����ָ��
//			����:
//					AxVar(0, 1, AX_INT32, "nVar", &nVar);
//
//		4. ���� _AXTRACE_DISABLE_ �����������е���Ϣ�������
//		
//////////////////////////////////////////////////////



#ifndef _AXTRACE_INCLUDE_
#define _AXTRACE_INCLUDE_


#include "GIBasicType.h"



#define INVALID_TRACE_WND	((unsigned char)-1)


#ifdef _AXTRACE_DISABLE_
	#define AxTrace		__noop
	#define AxVar		__noop
#else
	enum AXVALUE
	{
		AX_INT8,
		AX_UINT8,
		AX_INT16,
		AX_UINT16,
		AX_INT32,
		AX_UINT32,
		AX_INT64,
		AX_UINT64,
		AX_FLOAT32,
		AX_DOUBLE64,
		AX_STRING,
	};

	VOID KYLIN_API AxTrace(unsigned char nWnd, unsigned char nType, const char *pszFmt, ...);
	VOID KYLIN_API AxVar(unsigned char nWnd, unsigned char nType, AXVALUE nVariableType, const char *szValueName, VOID* pValue);

#endif


#endif	// _AXTRACE_INCLUDE_