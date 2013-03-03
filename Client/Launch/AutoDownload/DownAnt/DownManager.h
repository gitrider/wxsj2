///////////////////////////////////////
//	下载管理器，用来管理下载蚂蚁的工作，
//	也是提供给用户的接口的类

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
	//蚂蚁
	CDownAnt*	pAnt;
	//开始地址，包括该地址
	__int64		nStart;		
	//结束地址，不包括该地址
	__int64		nEnd;		
	//正在处理的地址，[nStart, nNow)是已经处理完的部分,当nNow<0表示还没有开始处理
	__int64		nNow;
	//临时文件
	char		szFileName[MAX_PATH];
};

class CDownManager
{
public:
	//主状态
	enum DOWN_STATE_CATEGORY
	{
		IDLE,					//空闲
		BUSY,					//忙碌中
		FINISH_SUCCESS,			//成功下载完毕
		FINISH_NOT_SUCCESS,		//下载失败
	};

	enum DOWN_STATE
	{
		IDLE_NOTINIT,							//0	 空闲	- 尚未初始化	
		IDLE_INIT_SUCCESS,						//1  空闲	- 初始化成功
		IDLE_INIT_FAILED,						//2  空闲	- 初始化失败

		BUSY_QUERY,								//忙碌中	- 开始查询
		BUSY_FIRST_BEGIN,						//忙碌中	- 第一个蚂蚁开始工作
		BUSY_UNION_BEGIN,						//忙碌中	- 开始合并文件

		FINISH_SUCCESS_ALLOVER,					//成功下载完毕

		FINISH_NOT_SUCCESS_UNIONFILE,			//下载失败 - 合并文件错误
		FINISH_NOT_SUCCESS_ABORT,				//下载失败 - 放弃

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
	//主状态
	DOWN_STATE_CATEGORY	m_theStateCategory;
	//详细状态
	DOWN_STATE			m_theState;
	//下载文件的地址
	CHAR				m_strURLToDownload[INTERNET_MAX_URL_LENGTH];
	//本地存贮地址
	CHAR				m_strFileToDownloadInto[MAX_PATH];
	//附加属性,这个值会传给回调函数
	VOID*				m_pOwner;
	//线程池
	CThreadPool			m_ThreadPool;
	//下载蚂蚁
	AntSection 			m_vDownAnt[MAX_ANT_NUM];
	//蚂蚁数目
	INT					m_nAntNum;
	//文件大小
	__int64				m_nFileSize;
	//已经下载的文件
	__int64				m_nFileRead;
	//开始下载文件时间
	DWORD				m_dwBeginTickCount;
	//下载速度
	INT					m_nSpeed;
	//节点数
	__int64				m_nBlockNum;
	//回调函数
	DOWNMANAGER_STATUS_CALLBACK		UserCallBack;
	//错误解释
	CHAR				m_strStatusInfor[MAX_PATH];
	//临时文件名
	CHAR				m_szTempFileName[MAX_PATH];
	//全部结束Event
	HANDLE				m_hAllOverEvent;
	//正在合并文件
	BOOL				m_bUnioning;
	//线程锁
	CRITICAL_SECTION	m_threadSection;

public:
	CDownManager();
	virtual ~CDownManager();
};

}