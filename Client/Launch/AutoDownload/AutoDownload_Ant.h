/************************************************\
*												*
*												*
*				使用多线程下载					*
*												*
*								by jinchao		*
*												*
\************************************************/
#pragma once

#include "AutoDownload.h"

namespace DOWNANT { class CDownManager; }

class CAutoDownload_DownAnt : public CAutoDownload
{
	/**********************************************************
		公有接口
	***********************************************************/
public:
	/*!
	\brief
		返回类型

	\return 
		Autodow类型
	*/
	virtual AUTODOWN_TYPE getType(void) const { return AT_ANT; }

	/*!
	\brief
		取得当前状态
	*/
	virtual AUTODOWN_STATE getState(void) const;

	/*!
	\brief	
		设置下载任务[异步]

	\prama szDownURL
		下载地址(http，ftp协议)

	\param szFileName
		下载后本地存储位置
	
	\param hWnd
		下载过程中消息通知窗口，通知消息部分参照AUTODOWN_WNDMSG

	\param hWnd
		发送通知消息时的OwnerID, 通过wParam返回给调用者

	\return
		初始化是否成功
	*/
	virtual bool init(const char* szDownURL, const char* szFileName, HWND hWnd=0, int nOwnerID=0);

	/*!
	\brief	
		重置工作状态[同步]
			停止一切工作，恢复到初始化状态
	*/
	virtual void reset(void);

	/*!
	\brief	
		开始工作[异步]
			可以调用waitStop确认下载结束
	*/
	virtual void begin(void);

	/*!
	\brief	
		强制停止工作[异步]
			可以调用waitStop确认完全停止
	*/
	virtual void stop(void);

	/*!
	\brief	
		等待工作结束
	\param	bBlock
		是否同步
	\return
		是否已经结束，只有bBlock为true时有效
	*/
	virtual bool waitStop(bool bBlock);

	/*!
	\brief
		取得当前正在下载的文件的大小

	\param nFileSize[out]
		文件大小

	\param nReadSize[out]
		已经下载的文件大小

	\return
		是否取得文件大小
	*/
	virtual bool getFileSize(__int64& nFileSize, __int64& nReadSize) const;

	/*!
	\brief
		取得当前下载速度相关信息
	
	\param pTimeSpend
		已经花费的时间(s)

	\param pTimeRemain
		估计剩余时间

	\return
		下载平均速度(byte/s)
	*/
	virtual int getSpeedInfo(int* pTimeSpend=0, int* pTimeRemain=0) const;

	/**********************************************************
		内部数据
	***********************************************************/
protected:
	DOWNANT::CDownManager* m_pDownManager;

	/*!
	\brief	
		Downant 回调函数
	*/
	static void WINAPI _downManStatusCallback(VOID* pOwner, DWORD dwCode, WPARAM wParam, LPARAM lParam);

	/**********************************************************
		构造/析构
	***********************************************************/
	friend CAutoDownload* CAutoDownload::createInstance(AUTODOWN_TYPE type);
	friend void CAutoDownload::destroyInsance(CAutoDownload* pDown);

protected:
	CAutoDownload_DownAnt(void);
	virtual ~CAutoDownload_DownAnt(void);
};