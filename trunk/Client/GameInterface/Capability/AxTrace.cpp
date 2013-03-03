
#include "StdAfx.h"
#include <windows.h>
#include "AxTrace.h"
#include <stdio.h>



//#define WATCHER_WINDOW_TITLE	("{74680574-FCAD-40bc-B9D7-3BED3B0EA4C2}")
// Window class name used by the main window of the TRACEWIN applet.
#define WATCHER_WINDOW_TITLE ("TRACEWIN")

// ID sent as COPYDATASRUCT::dwData to identify the WM_COPYDATA message
// as coming from an app using TraceWin.
#define ID_COPYDATA_TRACEMSG MAKELONG(MAKEWORD('t','w'),MAKEWORD('i','n'))


HWND	g_hWatcherWnd = NULL;


#ifdef _AXTRACE_DISABLE_

	//...
	//��ֹ��մ���
	//...

#else

	#define CONTENTS_LENGTH	(2048)
	typedef struct
	{
		unsigned char	cTraceType;
		unsigned char	cWinNum;
		unsigned char	cTypeNum;
		unsigned char	cReserve;
		char			pContents[CONTENTS_LENGTH];
	} AXIATRACEDATA, FAR *LPAXIATRACEDATA;

	//��Ϣ���ͺ���
	VOID AxTrace(unsigned char nWnd, unsigned char nType, const char *pszFmt, ...) { try
	{
		//Ѱ�Ҽ�ش���
		g_hWatcherWnd = ::FindWindowA(NULL, WATCHER_WINDOW_TITLE);
		
		//û���ҵ�������
		//if(!g_hWatcherWnd) return;

		static AXIATRACEDATA	cTraceData;
		static COPYDATASTRUCT   cCopyData;
		ZeroMemory(&cCopyData,  sizeof(COPYDATASTRUCT));
		ZeroMemory(&cTraceData, sizeof(AXIATRACEDATA));
	
		//��Ϣ����
		cTraceData.cTraceType = 1;
		cTraceData.cWinNum = (nWnd == INVALID_TRACE_WND) ? 
			(unsigned char)(:: GetCurrentProcessId()%256) : (unsigned char)nWnd;
		cTraceData.cTypeNum = (unsigned char)nType;

		va_list ptr; va_start(ptr, pszFmt);
		int nRet = _vsnprintf((char *)cTraceData.pContents, CONTENTS_LENGTH, pszFmt, ptr);
		if((nRet>=0) && (nRet<= CONTENTS_LENGTH-1))//ȷ�����������ݿɿ�
			cTraceData.pContents [ strlen(cTraceData.pContents)] ='\n';
		va_end(ptr);
				
		//����COPYDATA�ṹ
		cCopyData.dwData = ID_COPYDATA_TRACEMSG;
		cCopyData.cbData = sizeof(unsigned char)*4 + sizeof(INT) + (DWORD)strlen(cTraceData.pContents) + 1;
		cCopyData.lpData = (char *)cTraceData.pContents;//&cTraceData;

		if(!g_hWatcherWnd)
		{		
			OutputDebugString((LPCSTR)cCopyData.lpData);
		}
		else
		{	
			SendMessage(g_hWatcherWnd, WM_COPYDATA, NULL, (LPARAM)&cCopyData);//ͨ��COPYDATA���͵����ܴ���
		}
	}catch(...) {} }


	VOID AxAutoTrace(unsigned char nType, const char *pszFmt, ...)
	{
		UINT dwID = ::GetCurrentProcessId();
	}

	VOID AxVar(unsigned char nWnd, unsigned char nType, AXVALUE nVariableType, const char *szValueName, VOID* pValue) { try
	{
		if(!g_hWatcherWnd)
		{
			//Ѱ�Ҽ�ش���
			g_hWatcherWnd = ::FindWindowA(NULL, WATCHER_WINDOW_TITLE);
			//û���ҵ�������
			if(!g_hWatcherWnd) return;
		}

		static AXIATRACEDATA	cTraceData;
		static COPYDATASTRUCT   cCopyData;
		ZeroMemory(&cCopyData, sizeof(COPYDATASTRUCT));
		
		//�����������
		cTraceData.cTraceType = 2;
		cTraceData.cWinNum = (unsigned char)nWnd;
		cTraceData.cTypeNum = (unsigned char)nType;

		switch(nVariableType)
		{
		case AX_INT8:
		case AX_UINT8:
			{
				_snprintf((char *)cTraceData.pContents, 
					CONTENTS_LENGTH, 
					"%d|%s|%d", (INT)nVariableType, (char*)szValueName, *((__int8*)pValue));
			}
			break;
		case AX_INT16:
		case AX_UINT16:
			{
				_snprintf((char *)cTraceData.pContents, 
					CONTENTS_LENGTH, 
					"%d|%s|%d", (INT)nVariableType, (char*)szValueName, *((__int16*)pValue));
			}
			break;
		case AX_INT32:
		case AX_UINT32:
		case AX_FLOAT32:
			{
				_snprintf((char *)cTraceData.pContents, 
					CONTENTS_LENGTH, 
					"%d|%s|%d", (INT)nVariableType, (char*)szValueName, *((__int32*)pValue));
			}
			break;
		case AX_INT64:
		case AX_UINT64:
		case AX_DOUBLE64:
			{
				_snprintf((char *)cTraceData.pContents, 
					CONTENTS_LENGTH, 
					"%d|%s|%I64d", (INT)nVariableType, (char*)szValueName, *((__int64*)pValue));
			}
			break;
		case AX_STRING:
			{
				_snprintf((char *)cTraceData.pContents, 
					CONTENTS_LENGTH, 
					"%d|%s|%s", (INT)nVariableType, (char*)szValueName, (char*)pValue);
			}
			break;
		default:
			return;
		}

		//����COPYDATA�ṹ
		cCopyData.dwData = 0;
		cCopyData.cbData = sizeof(unsigned char)*4 + sizeof(INT) + (DWORD)strlen(cTraceData.pContents) + 1;
		cCopyData.lpData = &cTraceData;

		//ͨ��COPYDATA���͵����ܴ���
		SendMessage(g_hWatcherWnd, WM_COPYDATA, NULL, (LPARAM)&cCopyData);
	}catch(...) {} }

#endif

