

////////////////////////////////////////////
// �������ϣ���ʾһ�������߳�
//
#pragma once

#include <windows.h>
#include <wininet.h>

#include "ThreadPool.h"

namespace DOWNANT
{

typedef DWORD (WINAPI *DOWNANT_STATUS_CALLBACK)(VOID* pOwner, VOID* pDownAnt);
#define MANAGER_CMD_CONTINUE	(0)		//��������
#define	MANAGER_CMD_FORCEABORT	(1)		//ǿ����ֹ
#define MANAGER_CMD_STOPHERE	(2)		//���ص�ĿǰΪֹ

//���ص�λ4K
#define	DOWNANT_MINSIZE		(1024*4)
#define SAVE_MINSIZE		(1024*1024*2)	//ÿ1024*1024*2K��һ���̣��Ա��ϵ�����

class CDownAnt : public CThreadRequest
{
public:
	//��״̬
	enum DOWN_STATE_CATEGORY
	{
		IDLE,					//����
		BUSY,					//æµ��
		FINISH_SUCCESS,			//�ɹ��������
		FINISH_NOT_SUCCESS,		//����ʧ��
	};

	enum DOWN_STATE
	{
		IDLE_NOTINIT,							//����	- ��δ��ʼ��	
		IDLE_INIT_SUCCESS,						//����	- ��ʼ���ɹ�
		IDLE_INIT_FAILED,						//����	- ��ʼ��ʧ��

		BUSY_OPEN_REQUEST,						//æµ��	- ����Request
		BUSY_RESOLVING_NAME,					//æµ��	- ���ҷ�������
		BUSY_RESOLVED_NAME,						//æµ��	- �����������
		BUSY_CONNECTING,						//æµ��	- ���ӷ�������
		BUSY_CONNECTED,							//æµ��	- �����������
		BUSY_REDIRECTING,						//æµ��	- ���¶�λ��
		BUSY_GET_FILE_INFORMATION,				//æµ��	- ȡ���ļ���Ϣ
		BUSY_BEGIN_DOWNFILE,					//æµ��	- ��ʼ�����ļ���

		FINISH_SUCCESS_OVER,					//�ɹ��������
		FINISH_SUCCESS_STOPHERE,				//�û������������

		FINISH_FORCEABORT,						//����ʧ�� - ����ȡ��
		FINISH_ERROR_TIMEOUT,					//����ʧ�� - ��ʱ
		FINISH_ERROR_TRY_OPEN_LOCALFILE,		//����ʧ�� - ���ɱ����ļ�ʧ��
		FINISH_ERROR_INTERNET_OPEN,				//����ʧ�� - ��Internet���ʧ��
		FINISH_ERROR_SETSTATUSCALLBACK,			//����ʧ�� - ����״̬�ص�����ʧ��
		FINISH_ERROR_INTERNETCONNECT,			//����ʧ�� - ����Internetʧ��
		FINISH_ERROR_OPENREQUEST,				//����ʧ�� - ����Requestʧ��
		FINISH_ERROR_SENDREQUEST,				//����ʧ�� - ����Requestʧ��
		FINISH_ERROR_QUERYINFO,					//����ʧ�� - ��ѯ��Ϣ
		FINISH_ERROR_NEED_PROXY_AUTH_REQ,		//����ʧ�� - ��Ҫ�����������¼
		FINISH_ERROR_DENIED,					//����ʧ�� - ���������ܾ�
		FINISH_ERROR_INVALID_HTTP_RESPONSE,		//����ʧ�� - �Ƿ���HTTP����
		FINISH_ERROR_READFILE,					//����ʧ�� - ��ȡInternet�ļ�ʧ��
		FINISH_ERROR_EXCEPTION,					//����ʧ�� - ���쳣�׳�
	};

public:
	//��ʼ��
	// strURLToDownload			- �����ļ���ַ
	// strFileToDownloadInto	- �����ļ���ַ
	// pOwner					- �������ᱻ����ص�����
	// funcUserCallBack			- �ص�����
	// nOverTime				- ��ʱʱ��
	BOOL				Init(
								LPCTSTR strURLToDownload, 
								LPCTSTR strFileToDownloadInto,
								DOWNANT_STATUS_CALLBACK funcUserCallBack = NULL, 
								VOID* pOwner = NULL,
								DWORD nOverTime = INFINITE);

	void				ForceBreak(void);
	//�������ؽڵ㷶Χ
	BOOL				SetFilePoint(__int64 nPosition);

	DWORD				GetID(void) { return m_dwID; }
	DOWN_STATE			GetState(void) { return m_theState; }
	DOWN_STATE_CATEGORY GetStateCategory(void) { return m_theStateCategory; }
	LPCTSTR				GetDownUrl(void) { return m_strURLToDownload; }
	LPCTSTR				GetLocalFile(void) { return m_strFileToDownloadInto; }
	LPCTSTR				GetServer(void) { return m_strServer; }
	void				GetFileSize(BOOL& bGotFileSize, __int64& nFileSizeTotal, __int64& nFileSizeRead)
	{
		bGotFileSize = m_bGotFileSize;
		nFileSizeTotal = m_dwFileSizeTotal;
		nFileSizeRead = m_dwFileSizeRead;
	}

	//���Debug��Ϣ
	BOOL				m_bTrace;
	//�Ƿ�ʹ�öϵ���������
	BOOL			m_bUseBreakpoint ;

	//�ļ���ʼλ��
	__int64			GetFileStartPos()
	{
		return m_nFileStartPos;
	}

protected:
	//ID��
	DWORD				m_dwID;
	//��״̬
	DOWN_STATE_CATEGORY	m_theStateCategory;
	//��ϸ״̬
	DOWN_STATE			m_theState;
	//�����ļ��ĵ�ַ
	TCHAR			m_strURLToDownload[INTERNET_MAX_URL_LENGTH];
	//���ش�����ַ
	TCHAR			m_strFileToDownloadInto[MAX_PATH];
	//��������,���ֵ�ᴫ���ص�����
	VOID*			m_pOwner;
	//���������http,ftp
	DWORD			m_dwServiceType;
	//��������ַ
	CHAR			m_strServer[MAX_PATH];
	//�ļ��ڷ������ϵ�λ��
	CHAR			m_strObject[INTERNET_MAX_URL_LENGTH];
	//�������˿�
	WORD			m_nPort;
	//WinInet���õľ��
	HINTERNET		m_hInternetSession;
	HINTERNET		m_hHttpConnection;
	HINTERNET		m_hHttpFile;
	//�û�����
	DWORD			m_dwUserCmd;
	//ǿ����ֹ����
	BOOL			m_bForceBreak;
	//��ʱʱ��(��)
	DWORD			m_nOverTime;
	//��ʼʱ��
	DWORD			m_dwBeginTime;
	//�Ƿ�õ����ļ���С
	BOOL			m_bGotFileSize;
	//�ļ��ܴ�С
	__int64			m_dwFileSizeTotal;
	//�ļ���ʼλ��
	__int64			m_nFileStartPos;
	//�Ѿ����ص��ļ���С
	__int64			m_dwFileSizeRead;
	//�ļ��洢���
	FILE*			m_fpFileToWrite;
	//�ص�����
	DOWNANT_STATUS_CALLBACK	UserCallBack;
	//״̬����
	TCHAR			m_strStatusInfor[MAX_PATH];
public:
	CDownAnt(DWORD dwID);
	~CDownAnt();
	virtual void ThreadProc(int nThreadId)  throw();	

protected:
	static void CALLBACK	_OnWininetStatusCallBack(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, 
								LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	virtual void			OnWininetStatusCallBack(HINTERNET hInternet, DWORD dwInternetStatus, 
								LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	
	void					SetStatus(DOWN_STATE newState);

	virtual	 void			OnStatusCallback(void);
	void					TraceStatus(void);

	//����Ƿ���Ҫǿ���˳�����ʱ�����û�ǿ����ֹ)
	BOOL					CheckAbort(void);
};

BOOL __stdcall AfxParseURL(LPCTSTR pstrURL, DWORD& dwServiceType,
	LPTSTR strServer, LPTSTR strObject, INTERNET_PORT& nPort);

}