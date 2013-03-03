#include "StdAfx.h"
#include "LaunchSystem.h"
#include "LaunchUtil.h"
#include "AutoDownload.h"
#include "Resource.h"
#include "UpdateSystem.h"
#include  <io.h>

/***************************************************
	全局数据
****************************************************/
LaunchSystem* LaunchSystem::s_pMe = 0;

/***************************************************
	构造
****************************************************/
LaunchSystem::LaunchSystem() :
	m_theProcedure(PRO_NONE), 
	m_hWorkThread(0),
	m_hNotifyWnd(0),
	m_pUpdateSystem(0),
	m_dwInternetState(-1),
	m_nUpdateExit(-1)
{
	s_pMe = this;
	m_bTestVersion = FALSE;

    //创建多线程下载器
	m_pAutodownload = CAutoDownload::createInstance(CAutoDownload::AT_ANT);
	if (m_pAutodownload)
	{
		CString strErrInfo = "创建多线程下载器失败";
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);	
	}

}

/***************************************************
	析构
****************************************************/
LaunchSystem::~LaunchSystem()
{
	CAutoDownload::destroyInsance(m_pAutodownload);
	m_pAutodownload = 0;
	s_pMe = 0;
	if( m_hWorkThread )
	{ 
		::CloseHandle( m_hWorkThread ); 
		m_hWorkThread = 0;
	}

	if (m_pUpdateSystem)
		delete m_pUpdateSystem;
	m_pUpdateSystem = 0;
}

/***************************************************
	系统初始化
****************************************************/
void LaunchSystem::init(HWND hWnd, const char* szIniFile)
{
	CHAR szTemp[MAX_PATH] = {0};

	m_hNotifyWnd = hWnd;

	//当前目录
	CHAR szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	//创建临时目录
	strncpy(szTemp, szCurrentDir, MAX_PATH);
	PathAppend(szTemp, "Patch");
	PathAddBackslash(szTemp);
	m_strPatchDir = szTemp;
	CreateDirectory(m_strPatchDir.c_str(), 0);

	//读入设置
	CHAR szFullIni[MAX_PATH] = {0};
	strncpy(szFullIni, szCurrentDir, MAX_PATH);
	PathAppend(szFullIni, szIniFile);
	m_strIniFile = szFullIni;

	//信息文件URL
	::GetPrivateProfileString("Launch", "Help_URL", "", szTemp, MAX_PATH, szFullIni);
	m_strHelpURL = szTemp;

	//游戏exe
	::GetPrivateProfileString("Launch", "GameExe", "", szTemp, MAX_PATH, szFullIni);
	m_strGameExe = szTemp;

	//游戏配置文件
	::GetPrivateProfileString("Launch", "GameConfig", "", szTemp, MAX_PATH, szFullIni);
	m_strGameConfig = szTemp;

	//Patch文件
	::GetPrivateProfileString("Launch", "PatchInfo", "", szTemp, MAX_PATH, szFullIni);
	m_strPatchFile.first = szTemp;
	m_strPatchFile.second = m_strPatchDir + "PatchInfo.txt";

	//LoginServer文件
	::GetPrivateProfileString("Launch", "LoginServer", "", szTemp, MAX_PATH, szFullIni);
	m_strServerInfo.first = szTemp;
	m_strServerInfo.second = m_strPatchDir + "LoginServer.txt";

	//Resources配置文件,用于读包信息
	::GetPrivateProfileString("Launch", "Resources", "", szTemp, MAX_PATH, szFullIni);
	m_strFairyConfig = szTemp;

	//读版本信息，0:发行版（更新动作在MPQ包中完成，看不到零散资源）	1：内部版调试版（以零散文件形式更新）     
	m_bTestVersion= ::GetPrivateProfileInt("Launch","Version",0,szFullIni);

	//启动工作线程
	UINT nThreadID;
	m_hWorkThread = (HANDLE)_beginthreadex(0, 0, _workThread, this, CREATE_SUSPENDED, &nThreadID);
	::ResumeThread(m_hWorkThread);
}


/***************************************************
	工作线程
****************************************************/
unsigned int LaunchSystem::_workThread(void* lpParam)
{
	((LaunchSystem*)lpParam)->workThread();
	return 0;
}


void LaunchSystem::workThread(void)
{
	m_nUpdateExit = -2;

	//----------------------------------------------
	//创建下载控制窗口
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	HINSTANCE hInstance =  ::GetModuleHandle(0);
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.lpfnWndProc	= (WNDPROC)downloadWndProc;
	wcex.hInstance		= hInstance;
	wcex.lpszClassName	= "LAUNCH_CMD_WINDOW_CLASS";
	RegisterClassEx(&wcex);
	
	m_hDownloadWnd = CreateWindow("LAUNCH_CMD_WINDOW_CLASS", "", 0,
		0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	UpdateWindow(m_hDownloadWnd);

	::SetWindowLong(m_hDownloadWnd, GWL_USERDATA, (LONG)(DWORD_PTR)this);


	//创建更新系统
	CString strErrInfo;
	m_pUpdateSystem = new UpdateSystem;

	//----------------------------------------------
	//进入流程 "检查网络"
	if( enterProcedure_CheckInternetState() == false )
	{
		CString strErrInfo;
		strErrInfo.LoadString( IDS_LOCAL_NET_ERR );
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);
		m_nUpdateExit = -2;
		return;
	}

	//----------------------------------------------
	//进入流程 "检查磁盘空间"
	if( enterProcedure_CheckDiskState() == false ) 
	{
		CString strErrInfo;
		strErrInfo.LoadString( IDS_NOT_ENOUGH_SPACE );
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);
		m_nUpdateExit = -5;
		return;
	}

	//----------------------------------------------
	//进入流程 "下载最新版本信息",PatchInfo.txt
	if(!enterProcedure_DownloadPatchInfo()) 
	{
		m_nUpdateExit = -3;
		return;
	}
	else 
	{
		//创建更新系统
		CString strErrInfo;
		string strText = "PatchInfo.txt下载成功\n";
		m_pUpdateSystem->Log(strText.c_str(),strText.size());
	
		if( !(m_pUpdateSystem->Init( m_hNotifyWnd, PRO_INSTALL_PATCH, m_strPatchFile.second.c_str(), m_strFairyConfig.c_str(), strErrInfo )) )
		{
			m_nUpdateExit = -3;
			CString strAnsysPatchFault;
			strErrInfo += "\n";
			strAnsysPatchFault.Format( IDS_ANSYS_PATCH_INFO, strErrInfo );
			::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strAnsysPatchFault, 0);

			m_pUpdateSystem->Log(strAnsysPatchFault,strAnsysPatchFault.GetLength());
			return;
		}

		strText = "更新系统创建成功！\n";
		m_pUpdateSystem->Log(strText.c_str(),strText.size());
	}

	//----------------------------------------------
	//进入流程 "下载最新服务器负载信息",LoginServer.txt
	if(!enterProcedure_DownloadServerInfo()) 
	{
		string strText = "LoginServer.txt下载失败\n";
		m_pUpdateSystem->Log(strText.c_str(),strText.size());
		m_nUpdateExit = -3;
		return;
	}
	
	CString strText = "LoginServer.txt下载成功\n";
	m_pUpdateSystem->Log(strText,strText.GetLength());

	//----------------------------------------------
	//进入流程 "下载安装最新版本" cjsh-1.4.1.2.zip
	if(!enterProcedure_DownloadPatch())
	{
		string strText = "更新包下载或安装失败\n";
		m_pUpdateSystem->Log(strText.c_str(),strText.size());
		m_nUpdateExit = -3;
		return;
	}

	strText = "更新包下载、安装都成功！\n";
	m_pUpdateSystem->Log(strText,strText.GetLength());

	::SendMessage(m_hNotifyWnd, NM_OK, 0, 0);

	//----------------------------------------------
	//关闭窗口
	::DestroyWindow(m_hDownloadWnd);
	m_hDownloadWnd = 0;

	m_nUpdateExit = 0;
}


/*******************************************************
		下载工作命令接收窗口
********************************************************/
LRESULT CALLBACK LaunchSystem::downloadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LaunchSystem* pThis = (LaunchSystem*)(DWORD_PTR)::GetWindowLong(hWnd, GWL_USERDATA);
	if(pThis)
	{
		CAutoDownload* pDownload = pThis->m_pAutodownload;
		UpdateSystem* pUpdateSystem = pThis->m_pUpdateSystem;

		// 下载开始
		if(message == CAutoDownload::AM_BEGIN)
		{
			//开始工作...

			return TRUE;
		}
		// 状态发生改变
		else if(message == CAutoDownload::AM_STATUSUPDATE)
		{
			//开始连接服务器...
			if(pDownload->getState() == CAutoDownload::AS_BUSY_QUERY)
			{
				CString strTemp;
				strTemp.Format(IDS_DOWNLOADING, pDownload->getUrlFileName().c_str());
				::SendMessage(pThis->m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strTemp, 0);

				strTemp.LoadString(IDS_DOWNLOAD_QUER);
				::SendMessage(pThis->m_hNotifyWnd, NM_FILE, (WPARAM)(LPCTSTR)(LPCTSTR)strTemp, 0);
			}
			//成功结束!
			else if(pDownload->getStateCategory() == CAutoDownload::AC_FINISH_SUCCESS)
			{
				//...
				//::SendMessage(pThis->m_hNotifyWnd, NM_FILE, (WPARAM)(LPCTSTR)"下载结束", 0);
			}
			//失败结束!
			else if(pDownload->getStateCategory() == CAutoDownload::AC_FINISH_FAILED)
			{
				CString strTemp = "下载失败.";
				::SendMessage(pThis->m_hNotifyWnd, NM_FILE, (WPARAM)(LPCTSTR)"下载失败", 0);
			}

			return TRUE;
		}
		// 尺寸更新
		else if(message == CAutoDownload::AM_SIZEUPDATE)
		{
			__int64 nFileSize, nReadSize;

			//刷新进度条
			pDownload->getFileSize(nFileSize, nReadSize);
			::SendMessage(pThis->m_hNotifyWnd, NM_PROCESS1, (WPARAM)(int)(nReadSize*100.0/nFileSize), 0);

			//刷新速度显示
			static DWORD nLast = 0;
			DWORD nTimeNow  = ::GetTickCount();
			if(nTimeNow - nLast > 1000)
			{
				int nSpeed = pDownload->getSpeedInfo();

				CString strTemp;
				strTemp.Format(IDS_DOWNLOAD_SPEED, 
						CAutoDownload::fileSize2String(nReadSize).c_str(), 
						CAutoDownload::fileSize2String(nFileSize).c_str(), 
						CAutoDownload::fileSize2String(nSpeed).c_str());

				::SendMessage(pThis->m_hNotifyWnd, NM_FILE, (WPARAM)(LPCTSTR)strTemp, 0);
				nLast = nTimeNow;
			}

			return TRUE;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*******************************************************
	进入某一个指定流程(检查网络状况)
********************************************************/
bool LaunchSystem::enterProcedure_CheckInternetState(void)
{
	if( InternetGetConnectedState( &m_dwInternetState, 0 ) == TRUE ) {
		return true;
	}
	return false;
}

/*******************************************************
	进入某一个指定流程(检查磁盘空间)
********************************************************/
bool LaunchSystem::enterProcedure_CheckDiskState(void)
{
	char szTemp[1024] = {0};
	if( ::GetModuleFileName( NULL, szTemp, 1000 ) ) {
		char szDriver[10] = {0};
		_splitpath( szTemp, szDriver, 0, 0, 0 );
		ULARGE_INTEGER uSpaceSize;
		if( GetDiskFreeSpaceEx( szDriver, &uSpaceSize, 0, 0) )
		{
			if( uSpaceSize.HighPart>0 || uSpaceSize.LowPart > MAX_NEED_SPACE ) {
				return true;
			}
			else 
			{
				return false;
			}
		}
	}
	return true;
}

/***************************************************
	进入某一个指定流程(下载最新版本信息)
****************************************************/
bool LaunchSystem::enterProcedure_DownloadPatchInfo(void)
{
	//设定工作状态
	m_theProcedure = PRO_DOWNLOAD_PATCH_INFO;	// 下载最新版本信息

	CString strDownloadingPatchInfo;
	strDownloadingPatchInfo.LoadString(IDS_DOWNLOADING_PATCH_INFO);
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);

	//下载最新版本信息
	m_pAutodownload->init(m_strPatchFile.first.c_str(), m_strPatchFile.second.c_str(), m_hDownloadWnd, PRO_DOWNLOAD_PATCH_INFO);
	m_pAutodownload->begin();

	//阻塞下载
	m_pAutodownload->waitStop(true);

	//是否成功，暂时默认是成功的
	if(CAutoDownload::AC_FINISH_SUCCESS != m_pAutodownload->getStateCategory())
	{
		CString strDownloadPatchFault;
		strDownloadPatchFault.LoadString(IDS_DOWNLOAD_PATCH_INFO_FAULT);
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strDownloadPatchFault, 0);
		m_pUpdateSystem->Log(strDownloadPatchFault,strDownloadPatchFault.GetLength());
		return false;
	}

	//重置
	m_pAutodownload->reset();

	strDownloadingPatchInfo.LoadString(IDS_DOWNLOADING_PATCH_INFO_OK);
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);

	return true;
}

/***************************************************
	进入某一个指定流程(下载最新版本)
****************************************************/
bool LaunchSystem::enterProcedure_DownloadPatch(void)
{
	m_theProcedure = PRO_DOWNLOAD_PATCH;

	if( m_pUpdateSystem->IfUpdateOK() )
	{
		::SendMessage(m_hNotifyWnd, NM_PROCESS1, (WPARAM)100, 0);
		CString strErrInfo;
		strErrInfo.LoadString( IDS_DOWNLOAD_END );
		::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strErrInfo, 0);
		::SendMessage(m_hNotifyWnd, NM_FILE, (WPARAM)"", 0);
		
		strErrInfo += "\n";
		m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
		return true;
	}

	if( m_pUpdateSystem->IfInit() == FALSE ) 
	{
		CString strErrInfo;
		strErrInfo.LoadString( IDS_ERR_US_INIT );
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);

		strErrInfo += "\n";
		m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
		return( false );
	}

	//循环得到要下载的Patch包信息，然后下载
	char szPath[1024], szFile[1024], szTemp[1024];
	_splitpath( m_strPatchFile.second.c_str(), szPath, szTemp, NULL, NULL );
	PathAppend( szPath, szTemp );
	for( int n = 0; n < m_pUpdateSystem->GetUpdateStep(); n ++ ) 
	{
		const PatchInfo* ppi = m_pUpdateSystem->GetUpdateInfo( n );
		strcpy( szTemp, ppi->urlInfo[0].strURL.c_str() );

		//检查URL
		CString strServer, strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType = AFX_INET_SERVICE_HTTP;
		if( AfxParseURL( szTemp, dwServiceType, strServer, strObject, nPort ) == FALSE ) 
		{
			CString strErrInfo;
			strErrInfo.LoadString( IDS_ERR_US_URL );
			::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);
			strErrInfo += "\n";
			m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
			return( false );
		}

		//检查文件名
		CString strFileName = PathFindFileName( strObject );
		if( strFileName == strObject )
		{
			CString strErrInfo;
			strErrInfo.LoadString( IDS_ERR_US_URL );
			::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);

			strErrInfo += "\n";
			m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
			return( false );
		}

		szFile[0] = 0;
		PathAppend( szFile, szPath );
		PathAppend( szFile, strFileName );

		CString strDownloadingPatchInfo;
		strDownloadingPatchInfo.Format("下载文件%s", strFileName );
		::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);
		strDownloadingPatchInfo += "\n";
		m_pUpdateSystem->Log(strDownloadingPatchInfo,strDownloadingPatchInfo.GetLength());

		if( PathFileExists( szFile ) == TRUE )	//本地已有该更新包
		{
			CString strErrInfo ;
			strErrInfo.Format("\n\n本地已有更新包%s,现在检查其合法性……\n",szFile);
			m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());

			CString strInfo;

			//检查本地文件合法性，主要检查version和command文件
			if( m_pUpdateSystem->PreUpdate( n, szFile, strInfo ) == TRUE ) 
			{
				strErrInfo.Format("经检查，本地%s文件已是最新版，无需下载\n",szFile);
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());

				strErrInfo.Format("开始安装该版本……\n");
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());

				//进入流程 "安装最新版本"
				if(!enterProcedure_Install()) 
				{
					strErrInfo.Format("该版本安装失败……\n\n");
					m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
					m_nUpdateExit = -4;
					return( false );
				}

				strErrInfo.Format("该版本安装成功……\n\n",n+1);
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
				continue;
			}
			else
			{	
				strErrInfo.Format("经检查，本地%s文件有损坏，删除后将重新下载\n\n",szFile);
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
				::DeleteFile(szFile);
				n--;
				continue;
			}

		}
		else
		{
			string url = ppi->urlInfo[0].strURL.c_str();
			m_pAutodownload->init(ppi->urlInfo[0].strURL.c_str(), szFile, 
				m_hDownloadWnd, PRO_DOWNLOAD_PATCH_INFO);
			m_pAutodownload->begin();

			m_pAutodownload->waitStop(true);

			if(CAutoDownload::AC_FINISH_SUCCESS != m_pAutodownload->getStateCategory())
			{
				CString strErrInfo;
				strErrInfo.Format("无%s文件或该文件错误", strFileName);
				::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);

				strErrInfo += "\n";
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
				return false;
			}

			m_pAutodownload->reset();

			CString strErrInfo;
			strErrInfo.Format("下载%s成功!", strFileName);
			::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strErrInfo, 0);
			m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
		}

	
		CString strInfo;
		//检查下载文件合法性，主要检查version和command文件
		if( m_pUpdateSystem->PreUpdate( n, szFile, strInfo ) == FALSE ) 
		{
			strInfo += "\n";
			m_pUpdateSystem->Log(strInfo,strInfo.GetLength());

			CString strErrInfo;
			strErrInfo.Format(IDS_DOWNLOAD_FILE_ERR, strFileName, strInfo );
			::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);

			strErrInfo += "\n";
			m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
			return( false );
		}

		CString strText = "";
		strText.Format("\n\n第%d个更新包下载成功\n\n",n+1);
		m_pUpdateSystem->Log(strText,strText.GetLength());

		strText.Format("\n\n开始安装第%d个新版本……\n\n",n+1);
		m_pUpdateSystem->Log(strText,strText.GetLength());

		//进入流程 "安装最新版本"
		if(!enterProcedure_Install()) 
		{
			strText.Format("\n\n第%d个新版本安装失败……\n\n",n+1);
			m_pUpdateSystem->Log(strText,strText.GetLength());
			m_nUpdateExit = -4;
			return( false );
		}

		strText.Format("\n\n第%d个新版本安装成功……\n\n",n+1);
		m_pUpdateSystem->Log(strText,strText.GetLength());

	}//for end

	::SendMessage(m_hNotifyWnd, NM_PROCESS1, (WPARAM)100, 0);
	CString strErrInfo;
	strErrInfo.LoadString( IDS_DOWNLOAD_END );
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strErrInfo, 0);
	::SendMessage(m_hNotifyWnd, NM_FILE, (WPARAM)"", 0);
	Sleep( 100 );
	return true;
}

/***************************************************
	进入某一个指定流程(下载最新服务器负载信息)
****************************************************/
bool LaunchSystem::enterProcedure_DownloadServerInfo(void)
{
	//设定工作状态
	m_theProcedure = PRO_DOWNLOAD_SERVER_INFO;

	CString strDownloadingPatchInfo;
	strDownloadingPatchInfo.LoadString(IDS_DOWNLOAD_SRV_INFO);
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);

	//下载最新服务器负载信息
	m_pAutodownload->init(m_strServerInfo.first.c_str(), m_strServerInfo.second.c_str(), m_hDownloadWnd, PRO_DOWNLOAD_PATCH_INFO);
	m_pAutodownload->begin();

	//阻塞下载
	m_pAutodownload->waitStop(true);

	//是否成功，暂时默认是成功的
	if(CAutoDownload::AC_FINISH_SUCCESS != m_pAutodownload->getStateCategory())
	{
		CString strDownloadPatchFault;
		strDownloadPatchFault.LoadString(IDS_DOWNLOAD_SRV_ERR);
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strDownloadPatchFault, 0);
		m_pUpdateSystem->Log(strDownloadPatchFault,strDownloadPatchFault.GetLength());
		return false;
	}

	//重置
	m_pAutodownload->reset();

	strDownloadingPatchInfo.LoadString(IDS_DOWNLOAD_SRV_OK);
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);
	::SendMessage(m_hNotifyWnd, NM_FILE, (WPARAM)"", 0);

	return true;
}

/***************************************************
	进入某一个指定流程(安装最新版本)
****************************************************/
bool LaunchSystem::enterProcedure_Install(void)
{
	m_theProcedure = PRO_INSTALL_PATCH;

	CString strUpdateInfo;
	if( m_pUpdateSystem->IfUpdateOK() ) 
	{
		strUpdateInfo.LoadString( IDS_START_UPDATE_OK );
		::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strUpdateInfo, 0);
		::SendMessage(m_hNotifyWnd, NM_PROCESS2, (WPARAM)100, 0);
		::SendMessage(m_hNotifyWnd, NM_OK, 0, 0);

		strUpdateInfo = "更新貌似已成功，你还更新个啥劲呀！\n";
		m_pUpdateSystem->Log(strUpdateInfo,strUpdateInfo.GetLength());
		return true;
	}

	if( m_pUpdateSystem->IfInit() == FALSE ) 
	{
		CString strErrInfo;
		strErrInfo.LoadString( IDS_ERR_US_INIT );
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);
		m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
		return( false );
	}

	strUpdateInfo.LoadString( IDS_START_UPDATE );
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strUpdateInfo, 0);
	::SendMessage(m_hNotifyWnd, NM_DISABLE_CANCEL, 0, 0);

	strUpdateInfo += "\n";
	m_pUpdateSystem->Log(strUpdateInfo,strUpdateInfo.GetLength());

	Sleep( 100 );

	CString strInfo;
	if( m_pUpdateSystem->DoUpdate( strInfo ) == FALSE ) 
	{
		strInfo += "\n";
		m_pUpdateSystem->Log(strInfo,strInfo.GetLength());

		CString strErrInfo;
		strErrInfo.Format( IDS_UPDATE_FAULT, strInfo );
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);
		m_pUpdateSystem->UndoUpdate();
		::SendMessage(m_hNotifyWnd, NM_PROCESS2, (WPARAM)0, 0);
		::SendMessage(m_hNotifyWnd, NM_ENABLE_CANCEL, 0, 0);
		strErrInfo += "\n";
		m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());

		return false;
	}

	strUpdateInfo.LoadString( IDS_START_UPDATE_OK );
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strUpdateInfo, 0);
	::SendMessage(m_hNotifyWnd, NM_PROCESS2, (WPARAM)100, 0);
	::SendMessage(m_hNotifyWnd, NM_ENABLE_CANCEL, 0, 0);
	::SendMessage(m_hNotifyWnd, NM_OK, 0, 0);

	strUpdateInfo += "\n";
	m_pUpdateSystem->Log(strUpdateInfo,strUpdateInfo.GetLength());

	//清除一下，以备下次更新
	m_pUpdateSystem->ClearMyself();
	return true;
}

/***************************************************
	运行游戏
****************************************************/
void LaunchSystem::runGame(void)
{
	//当前路径
	CHAR szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	//游戏exe
	CHAR szGameExe[MAX_PATH];
	strncpy(szGameExe, szCurrentDir, MAX_PATH);
	PathAppend(szGameExe, m_strGameExe.c_str());
	//PathAppend(szGameExe, "Bin/game/Game.exe");

	//游戏运行目录
	CHAR szGamePath[MAX_PATH];
	strncpy(szGamePath, szGameExe, MAX_PATH);
	::PathRemoveFileSpec(szGamePath);

	//命令行
	CHAR szGameCommand[MAX_PATH*2];

	if( IsTestVersion() )
	{
		_snprintf(szGameCommand, MAX_PATH*2, "%s -test", szGameExe);
	}
	else
	{
		_snprintf(szGameCommand, MAX_PATH*2, "%s -fl", szGameExe);
	}


	//启动进程
	STARTUPINFO			si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
	
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	
	if (CreateProcess(NULL, szGameCommand, NULL, NULL, 
		FALSE,CREATE_DEFAULT_ERROR_MODE, NULL,
		szGamePath, &si, &pi) == FALSE)
	{
		CString strText = "启动Game失败!";
		//启动失败
		m_pUpdateSystem->Log(strText,strText.GetLength());
		
		return;
	}
}


void LaunchSystem::FormatErrorMessage( LPSTR lpMessage )
{
	if (!FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMessage,
		0,
		NULL ))
	{
		// Handle the error.
		return;
	}

}
