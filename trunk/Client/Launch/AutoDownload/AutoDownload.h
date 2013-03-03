/************************************************\
*												*
*												*
*					AtuoDownload				*
*			----------------------------		*
*	    		0. 使用Sogou p2p功能下载		*
*				1. 使用多线程下载				*
*												*
*								by jinchao		*
*												*
\************************************************/

#pragma once

#include <windows.h>
#include <string>

class CAutoDownload
{
	/**********************************************************
		公有类型
	***********************************************************/
public:
	/*!
	\brief
		后台方法
	*/
	enum AUTODOWN_TYPE
	{
		AT_ANT,		//!< 普通多线程下载
		AT_SODA,	//!< Sogou toolbar 提供的p2p下载服务
	};

	/*!
	\brief
		工作状态，类别
	*/
	enum AUTODOWN_STATE_CATEGORY
	{
		AC_IDLE				= 0,			//!< 空闲
		AC_BUSY				= 1,			//!< 工作
		AC_FINISH_SUCCESS	= 2,			//!< 成功结束
		AC_FINISH_FAILED	= 3,			//!< 失败结束
	};

	/*!
	\brief
		工作状态
	*/
	enum AUTODOWN_STATE
	{
		AS_IDLE_NOTINIT			= 0x00000000,			//!< 空闲
		AS_IDLE_INIT_SUCCESS	= 0x00000001,			//!< 空闲_初始化成功
		AS_IDLE_INIT_FAILED		= 0x00000002,			//!< 空闲_初始化失败

		AS_BUSY_QUERY			= 0x00010000,			//!< 工作_查询文件信息
		AS_BUSY_DOWNBEGIN		= 0x00010001,			//!< 工作_开始下载
		AS_BUSY_UNIONBEGIN		= 0x00010002,			//!< 工作_开始合并

		AS_FINISH_SUCCESS		= 0x00020000,			//!< 成功结束

		AS_FINISH_FAILED_ERROR	= 0x00030000,			//!< 失败结束_发生错误
		AS_FINISH_FAILED_CANCEL	= 0x00030001,			//!< 失败结束_用户终止
		AS_FINISH_FAILED_PAUSE	= 0x00030002,			//!< 失败结束_用户暂停
		AS_FINISH_FAILED_LOCAL	= 0x00030003,			//!< 失败结束_本地文件失败

	};

	/*!
	\brief
		下载过程中消息通知，wParam为OwnerID, lParam根据消息不同有不同意义
	*/
	enum AUTODOWN_WNDMSG
	{
		AM_BEGIN = WM_USER+0X3000,		//!< 下载开始
		AM_STATUSUPDATE,				//!< 状态发生改变 lParam = 目前状态 enum AUTODOWN_STATE
		AM_SIZEUPDATE,					//!< 尺寸更新
		AM_END,							//!< 下载结束
										/*
										|  lParam 
										|		0: 成功
										|		1: 下载被Cancel
										|		2: 下载被暂停
										|		3: 普通下载时原站连接失败
										|		4: 本地写文件出错
										|		5: 下载过程中的失败
										|		6: 启动续传失败（*）
										|		10: 高速下载时getfipl失败[P2P]
										|		11: 高速下载时Probe失败[P2P]
										*/
	};

	/**********************************************************
		公有接口
	***********************************************************/
public:
	/*!
	\brief
		创建实例，当机器上没有安装Sogou toolbar时，如果指定要创建SODA类型，会失败

	\param type
		类型

	\return 
		Autodow实例指针
	*/
	static CAutoDownload* createInstance(AUTODOWN_TYPE type);

	/*!
	\brief
		销毁实例

	\return 
		None
	*/
	static void destroyInsance(CAutoDownload* pDown);

public:

	/*!
	\brief
		返回类型

	\return 
		Autodow类型
	*/
	virtual AUTODOWN_TYPE getType(void) const = 0;

	/*!
	\brief
		取得当前状态
	*/
	virtual AUTODOWN_STATE getState(void) const = 0;

	/*!
	\brief
		取得当前状态类别
	*/
	AUTODOWN_STATE_CATEGORY getStateCategory(void) const { return (AUTODOWN_STATE_CATEGORY)(getState()>>16);}

	/*!
	\brief
		取得当前正在下载的文件的文件名
	*/
	std::string getUrlFileName(void) const;

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
	virtual bool getFileSize(__int64& nFileSize, __int64& nReadSize) const = 0;

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
	virtual int getSpeedInfo(int* pTimeSpend=0, int* pTimeRemain=0) const = 0;

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
	virtual bool init(const char* szDownURL, const char* szFileName, HWND hWnd=0, int nOwnerID=0) = 0;

	/*!
	\brief	
		重置工作状态[同步]
			停止一切工作，恢复到初始化状态
	*/
	virtual void reset(void) = 0;

	/*!
	\brief	
		开始工作[异步]
			可以调用waitStop确认下载结束
	*/
	virtual void begin(void) = 0;

	/*!
	\brief	
		强制停止工作[异步]
			可以调用waitStop确认完全停止
	*/
	virtual void stop(void) = 0;

	/*!
	\brief	
		等待工作结束
	\param	bBlock
		是否同步
	\return
		是否已经结束，只有bBlock为true时有效
	*/
	virtual bool waitStop(bool bBlock) = 0;

	/**********************************************************
		工具函数
	***********************************************************/
public:
	/*!
	\brief
		工具函数，将文件大小转化为字符串描述
		(  -1024)	xxx
		[1K-1024K)	xxx.xK
		[1M-1024M)	xxx.xM
		[1G-1024G)  xxx.xG
		[1T-...)	xxx.xT

	\param nSize
		文件大小(byte)
	
	\return 
		转化后的字符串描述
	*/
	static std::string fileSize2String(__int64 nSize);

	/**********************************************************
		内部接口
	***********************************************************/
protected:
	/*!
	\brief	
		设置状态，并通知监控窗口
	\param	state
		新的状态
	\return
		None
	*/
	void setState(AUTODOWN_STATE state);

	/**********************************************************
		构造/析构
	***********************************************************/
protected:
	CAutoDownload(void);
	virtual ~CAutoDownload(void);

	/**********************************************************
		内部数据
	***********************************************************/
	/*!
	\brief	
		取得工作数据（内部调用）
	*/
	struct AUTODOWN_IMPL;
	AUTODOWN_IMPL* getImpl(void) { return m_pDownImpl; }
	const AUTODOWN_IMPL* getConstImpl(void) const{ return m_pDownImpl; }

protected:
	//下载数据
	AUTODOWN_IMPL*		m_pDownImpl;
};