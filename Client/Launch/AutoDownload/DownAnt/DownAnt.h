

////////////////////////////////////////////
// 下载蚂蚁，表示一个下载线程
//
#pragma once

#include <windows.h>
#include <wininet.h>

#include "ThreadPool.h"

namespace DOWNANT
{

typedef DWORD (WINAPI *DOWNANT_STATUS_CALLBACK)(VOID* pOwner, VOID* pDownAnt);
#define MANAGER_CMD_CONTINUE	(0)		//继续下载
#define	MANAGER_CMD_FORCEABORT	(1)		//强制中止
#define MANAGER_CMD_STOPHERE	(2)		//下载的目前为止

//下载单位4K
#define	DOWNANT_MINSIZE		(1024*4)
#define SAVE_MINSIZE		(1024*1024*2)	//每1024*1024*2K存一次盘，以备断点续传

class CDownAnt : public CThreadRequest
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
		IDLE_NOTINIT,							//空闲	- 尚未初始化	
		IDLE_INIT_SUCCESS,						//空闲	- 初始化成功
		IDLE_INIT_FAILED,						//空闲	- 初始化失败

		BUSY_OPEN_REQUEST,						//忙碌中	- 生成Request
		BUSY_RESOLVING_NAME,					//忙碌中	- 查找服务器中
		BUSY_RESOLVED_NAME,						//忙碌中	- 查找完服务器
		BUSY_CONNECTING,						//忙碌中	- 连接服务器中
		BUSY_CONNECTED,							//忙碌中	- 连接完服务器
		BUSY_REDIRECTING,						//忙碌中	- 重新定位中
		BUSY_GET_FILE_INFORMATION,				//忙碌中	- 取得文件信息
		BUSY_BEGIN_DOWNFILE,					//忙碌中	- 开始下载文件中

		FINISH_SUCCESS_OVER,					//成功下载完毕
		FINISH_SUCCESS_STOPHERE,				//用户定义下载完毕

		FINISH_FORCEABORT,						//下载失败 - 用于取消
		FINISH_ERROR_TIMEOUT,					//下载失败 - 超时
		FINISH_ERROR_TRY_OPEN_LOCALFILE,		//下载失败 - 生成本地文件失败
		FINISH_ERROR_INTERNET_OPEN,				//下载失败 - 打开Internet句柄失败
		FINISH_ERROR_SETSTATUSCALLBACK,			//下载失败 - 设置状态回调函数失败
		FINISH_ERROR_INTERNETCONNECT,			//下载失败 - 连接Internet失败
		FINISH_ERROR_OPENREQUEST,				//下载失败 - 生成Request失败
		FINISH_ERROR_SENDREQUEST,				//下载失败 - 发送Request失败
		FINISH_ERROR_QUERYINFO,					//下载失败 - 查询信息
		FINISH_ERROR_NEED_PROXY_AUTH_REQ,		//下载失败 - 需要代理服务器登录
		FINISH_ERROR_DENIED,					//下载失败 - 被服务器拒绝
		FINISH_ERROR_INVALID_HTTP_RESPONSE,		//下载失败 - 非法的HTTP请求
		FINISH_ERROR_READFILE,					//下载失败 - 读取Internet文件失败
		FINISH_ERROR_EXCEPTION,					//下载失败 - 有异常抛出
	};

public:
	//初始化
	// strURLToDownload			- 网络文件地址
	// strFileToDownloadInto	- 本地文件地址
	// pOwner					- 参数，会被传入回调函数
	// funcUserCallBack			- 回调函数
	// nOverTime				- 超时时间
	BOOL				Init(
								LPCTSTR strURLToDownload, 
								LPCTSTR strFileToDownloadInto,
								DOWNANT_STATUS_CALLBACK funcUserCallBack = NULL, 
								VOID* pOwner = NULL,
								DWORD nOverTime = INFINITE);

	void				ForceBreak(void);
	//设置下载节点范围
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

	//输出Debug信息
	BOOL				m_bTrace;
	//是否使用断点续传功能
	BOOL			m_bUseBreakpoint ;

	//文件起始位置
	__int64			GetFileStartPos()
	{
		return m_nFileStartPos;
	}

protected:
	//ID号
	DWORD				m_dwID;
	//主状态
	DOWN_STATE_CATEGORY	m_theStateCategory;
	//详细状态
	DOWN_STATE			m_theState;
	//下载文件的地址
	TCHAR			m_strURLToDownload[INTERNET_MAX_URL_LENGTH];
	//本地存贮地址
	TCHAR			m_strFileToDownloadInto[MAX_PATH];
	//附加属性,这个值会传给回调函数
	VOID*			m_pOwner;
	//服务段类型http,ftp
	DWORD			m_dwServiceType;
	//服务器地址
	CHAR			m_strServer[MAX_PATH];
	//文件在服务器上的位置
	CHAR			m_strObject[INTERNET_MAX_URL_LENGTH];
	//服务器端口
	WORD			m_nPort;
	//WinInet所用的句柄
	HINTERNET		m_hInternetSession;
	HINTERNET		m_hHttpConnection;
	HINTERNET		m_hHttpFile;
	//用户命令
	DWORD			m_dwUserCmd;
	//强制中止命令
	BOOL			m_bForceBreak;
	//超时时限(秒)
	DWORD			m_nOverTime;
	//开始时间
	DWORD			m_dwBeginTime;
	//是否得到了文件大小
	BOOL			m_bGotFileSize;
	//文件总大小
	__int64			m_dwFileSizeTotal;
	//文件起始位置
	__int64			m_nFileStartPos;
	//已经下载的文件大小
	__int64			m_dwFileSizeRead;
	//文件存储句柄
	FILE*			m_fpFileToWrite;
	//回调函数
	DOWNANT_STATUS_CALLBACK	UserCallBack;
	//状态描述
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

	//检查是否需要强制退出（超时或者用户强制终止)
	BOOL					CheckAbort(void);
};

BOOL __stdcall AfxParseURL(LPCTSTR pstrURL, DWORD& dwServiceType,
	LPTSTR strServer, LPTSTR strObject, INTERNET_PORT& nPort);

}