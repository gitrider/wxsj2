#pragma once

class CAutoDownload;
class UpdateSystem;
//class FixSystem;

const unsigned long MAX_NEED_SPACE = 200*1024*1024;		// 升级程序需要的磁盘空间，
														// 这部分空间包括Patch文件，与临时包文件
														// 目前估计要600M

class LaunchSystem
{
	/***************************************************
		共有类型
	****************************************************/
public:
	//工作流程
	enum PROCEDURE	
	{
		PRO_NONE,						//!< 无

		PRO_DOWNLOAD_PATCH_INFO,		//!< 0. 下载最新版本信息
		PRO_DOWNLOAD_PATCH,				//!< 1. 下载最新版
		PRO_DOWNLOAD_SERVER_INFO,		//!< 2. 下载服务器负载信息
		PRO_INSTALL_PATCH,				//!< 3. 安装最新版
	};

	//通知消息
	enum NOTIFY_MSG
	{
		NM_STATUS = (WM_USER+0X5000),	//!< 当前状态描述文字	wParam-文字内容
		NM_FILE,						//!< 当前正在处理的文件 wParam-文字内容
		NM_PROCESS1,					//!< 进度条1进度		wParam-(0-100)
		NM_PROCESS2,					//!< 进度条2进度		wParam-(0-100)
		NM_FAULT,						//!< 致命错误			wParam-文字内容
		NM_ENABLE_CANCEL,				//!< 允许取消
		NM_DISABLE_CANCEL,				//!< 禁止取消
		NM_OK,							//!< Launch运行完毕
	};

	/***************************************************
		共有接口
	****************************************************/
public:
	/*!
	\brief
		系统初始化

	\param hNotifyWnd
		通知窗口

	\param szLaunchIniFile
		配置文件

	\return
		None
	*/
	void init(HWND hWnd, const char* szLaunchIniFile = "Launch.ini");

	/*!
	\brief
		客户端fix函数，必须在init后执行
	\return
		None
	*/
	//void fix();

	/*!
	\brief
		得到信息文件地址

	\return
		信息文件地址
	*/
	const std::string&	getHelpURL(void) const { return m_strHelpURL; }

	/*!
	\brief
		得到游戏系统配置文件

	\return
		信息文件地址
	*/
	const std::string&	getGameConfig(void) const { return m_strGameConfig; }

	/*!
	\brief
		取得当前正在工作的异步流程

	\return
		流程名
	*/
	PROCEDURE getCurrentProcedure(void) const { return m_theProcedure; }

	/*!
	\brief
		运行游戏

	\return
		None
	*/
	void runGame(void);

	BOOL IsTestVersion()
	{
		return m_bTestVersion;
	}

	/***************************************************
		内部数据
	****************************************************/
private:
	/*!
	\brief
		进入某一个指定流程

	\param procedure
		流程名

	\return
		None
	*/
	//系列函数
	bool enterProcedure_CheckInternetState(void);	//!< 检查网络状况
	bool enterProcedure_CheckDiskState(void);		//!< 检查磁盘空间
	bool enterProcedure_DownloadPatchInfo(void);	//!< 下载最新版本信息
	bool enterProcedure_DownloadPatch(void);		//!< 下载最新版本
	bool enterProcedure_DownloadServerInfo(void);	//!< 下载最新服务器负载信息
	bool enterProcedure_Install(void);				//!< 安装最新版本

	//bool enterProcedure_ParseFiles(void);			//!< 分析当前版本
	/*!
	\brief 
		工作线程函数
	*/
	static unsigned int __stdcall _workThread(void* lpParam);
	void workThread(void);

	/*!
	\brief 
		工作线程函数
	*/
	//static unsigned int __stdcall _fixThread(void* lpParam);
	//void fixThread(void);

	/*!
	\brief 
		下载窗口消息接受窗口
	*/
	static LRESULT CALLBACK downloadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void FormatErrorMessage( LPSTR lpMessage );
	/***************************************************
		内部数据
	****************************************************/
private:
	BOOL			m_bTestVersion;		//是否为内部测试版,默认不是
	std::string		m_strPatchDir;		//!< 工作目录
	HWND			m_hNotifyWnd;		//!< 通知窗口
	std::string		m_strIniFile;		//!< Launch主配置文件

	std::string		m_strHelpURL;		//!< 信息显示URL
	std::string		m_strGameExe;		//!< 游戏exe
	std::string		m_strGameConfig;	//!< 游戏配置文件
	std::string		m_strFairyConfig;	//!< FairyResources配置文件

	HANDLE			m_hWorkThread;		//!< 工作线程句柄
	//HANDLE			m_hFixThread;		//!< 工作线程句柄
	PROCEDURE		m_theProcedure;		//!< 取得当前正在工作的异步流程

	CAutoDownload*	m_pAutodownload;	//!< 下载工具
	HWND			m_hDownloadWnd;		//!< 下载控制窗口

	typedef std::pair< std::string, std::string> FilePair;

	FilePair		m_strPatchFile;		//!< 最新版本信息文件
	FilePair		m_strServerInfo;


	UpdateSystem*	m_pUpdateSystem;	//!< 更新系统

	//FixSystem*		m_pFixSystem;		//!< FIX系统

	DWORD			m_dwInternetState;
	int				m_nUpdateExit;		//!< 升级线程退出码
										//!< -1. 线程没有启动
										//!< -2. 线程正常启动，由于未知错误没有正常结束
										//!< -3. 线程正常启动，由于下载错误而结束
										//!< -4. 线程正常启动，由于安装错误而结束
										//!< -5. 线程正常启动，由于磁盘空间不足错误而结束
	
	/***************************************************
		构造/析构
	****************************************************/
public:
	LaunchSystem();
	~LaunchSystem();
	
	/*!
	\brief
		得到自身

	\return
		自身指针
	*/
	static LaunchSystem*	GetMe(void) { return s_pMe; }

private:
	static LaunchSystem* s_pMe;	//!< 用于保存自身的指针
};