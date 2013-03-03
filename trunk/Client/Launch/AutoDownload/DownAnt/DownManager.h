///////////////////////////////////////
//	���ع����������������������ϵĹ�����
//	Ҳ���ṩ���û��Ľӿڵ���

#pragma once

#include "ThreadPool.h"
#include "DownAnt.h"

namespace DOWNANT
{

typedef VOID (WINAPI *DOWNMANAGER_STATUS_CALLBACK)(VOID* pOwner, DWORD dwCode, WPARAM wParam, LPARAM lParam);

#define DMCC_STATUS_CHANGE	(0)
#define DMCC_NEW_ANT		(1)
#define DMCC_ANT_LOG		(2)
#define DMCC_FILEINFO		(3)
#define DMCC_SIZEUPDATE		(4)

#define MAX_ANT_NUM			(5)

struct AntSection
{
	//����
	CDownAnt*	pAnt;
	//��ʼ��ַ�������õ�ַ
	__int64		nStart;		
	//������ַ���������õ�ַ
	__int64		nEnd;		
	//���ڴ���ĵ�ַ��[nStart, nNow)���Ѿ�������Ĳ���,��nNow<0��ʾ��û�п�ʼ����
	__int64		nNow;
	//��ʱ�ļ�
	char		szFileName[MAX_PATH];
};

class CDownManager
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
		IDLE_NOTINIT,							//0	 ����	- ��δ��ʼ��	
		IDLE_INIT_SUCCESS,						//1  ����	- ��ʼ���ɹ�
		IDLE_INIT_FAILED,						//2  ����	- ��ʼ��ʧ��

		BUSY_QUERY,								//æµ��	- ��ʼ��ѯ
		BUSY_FIRST_BEGIN,						//æµ��	- ��һ�����Ͽ�ʼ����
		BUSY_UNION_BEGIN,						//æµ��	- ��ʼ�ϲ��ļ�

		FINISH_SUCCESS_ALLOVER,					//�ɹ��������

		FINISH_NOT_SUCCESS_UNIONFILE,			//����ʧ�� - �ϲ��ļ�����
		FINISH_NOT_SUCCESS_ABORT,				//����ʧ�� - ����

	};

public:
	static DWORD	WINAPI	_AntDownStatusCallBack(VOID* pOwner, VOID* pDownAnt);
	DWORD AntDownStatusCallBack(VOID* pDownAnt);

	BOOL				Init(
							LPCTSTR strURLToDownload, 
							LPCTSTR strFileToDownloadInto,
							DOWNMANAGER_STATUS_CALLBACK funcStatusFunc = NULL, 
							VOID* pOwner = NULL);

	void				Begin(BOOL bBlock=FALSE);
	void				ShutDown(BOOL bRightNow=FALSE);
	BOOL				WaitOver(BOOL bBlock);

	DOWN_STATE			GetState(void) const { return m_theState; }
	DOWN_STATE_CATEGORY GetStateCategory(void)  const { return m_theStateCategory; }
	LPCSTR				GetDownUrl(void)  const { return m_strURLToDownload; }
	LPCSTR				GetLocalFile(void)  const { return m_strFileToDownloadInto; }
	LPCSTR				GetStatusInfo(void)  const { return m_strStatusInfor; }
	__int64				GetBlockNum(void)  const { return m_nBlockNum; }
	AntSection*			GetSection(void)  { return m_vDownAnt; }
	INT					GetAntNum(void)  const { return m_nAntNum; }
	void				GetFileSize(__int64& nFileSize, __int64& nFileSizeRead)   const 
						{ nFileSize=m_nFileSize;  nFileSizeRead=m_nFileRead; }
	INT					GetSpeed(void)  const  { return m_nSpeed; }

private:
	void				SetStatus(DOWN_STATE newState);
	void				OnGetFileInformation(DWORD dwAntNum);
	void				OnAntWorkOver(DWORD dwAntNum);
	void				OnAntWorkFailed(DWORD dwAntNum);
private:
	//��״̬
	DOWN_STATE_CATEGORY	m_theStateCategory;
	//��ϸ״̬
	DOWN_STATE			m_theState;
	//�����ļ��ĵ�ַ
	CHAR				m_strURLToDownload[INTERNET_MAX_URL_LENGTH];
	//���ش�����ַ
	CHAR				m_strFileToDownloadInto[MAX_PATH];
	//��������,���ֵ�ᴫ���ص�����
	VOID*				m_pOwner;
	//�̳߳�
	CThreadPool			m_ThreadPool;
	//��������
	AntSection 			m_vDownAnt[MAX_ANT_NUM];
	//������Ŀ
	INT					m_nAntNum;
	//�ļ���С
	__int64				m_nFileSize;
	//�Ѿ����ص��ļ�
	__int64				m_nFileRead;
	//��ʼ�����ļ�ʱ��
	DWORD				m_dwBeginTickCount;
	//�����ٶ�
	INT					m_nSpeed;
	//�ڵ���
	__int64				m_nBlockNum;
	//�ص�����
	DOWNMANAGER_STATUS_CALLBACK		UserCallBack;
	//�������
	CHAR				m_strStatusInfor[MAX_PATH];
	//��ʱ�ļ���
	CHAR				m_szTempFileName[MAX_PATH];
	//ȫ������Event
	HANDLE				m_hAllOverEvent;
	//���ںϲ��ļ�
	BOOL				m_bUnioning;
	//�߳���
	CRITICAL_SECTION	m_threadSection;

public:
	CDownManager();
	virtual ~CDownManager();
};

}