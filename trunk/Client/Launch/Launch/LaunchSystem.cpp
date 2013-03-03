#include "StdAfx.h"
#include "LaunchSystem.h"
#include "LaunchUtil.h"
#include "AutoDownload.h"
#include "Resource.h"
#include "UpdateSystem.h"
#include  <io.h>

/***************************************************
	ȫ������
****************************************************/
LaunchSystem* LaunchSystem::s_pMe = 0;

/***************************************************
	����
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

    //�������߳�������
	m_pAutodownload = CAutoDownload::createInstance(CAutoDownload::AT_ANT);
	if (m_pAutodownload)
	{
		CString strErrInfo = "�������߳�������ʧ��";
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);	
	}

}

/***************************************************
	����
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
	ϵͳ��ʼ��
****************************************************/
void LaunchSystem::init(HWND hWnd, const char* szIniFile)
{
	CHAR szTemp[MAX_PATH] = {0};

	m_hNotifyWnd = hWnd;

	//��ǰĿ¼
	CHAR szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	//������ʱĿ¼
	strncpy(szTemp, szCurrentDir, MAX_PATH);
	PathAppend(szTemp, "Patch");
	PathAddBackslash(szTemp);
	m_strPatchDir = szTemp;
	CreateDirectory(m_strPatchDir.c_str(), 0);

	//��������
	CHAR szFullIni[MAX_PATH] = {0};
	strncpy(szFullIni, szCurrentDir, MAX_PATH);
	PathAppend(szFullIni, szIniFile);
	m_strIniFile = szFullIni;

	//��Ϣ�ļ�URL
	::GetPrivateProfileString("Launch", "Help_URL", "", szTemp, MAX_PATH, szFullIni);
	m_strHelpURL = szTemp;

	//��Ϸexe
	::GetPrivateProfileString("Launch", "GameExe", "", szTemp, MAX_PATH, szFullIni);
	m_strGameExe = szTemp;

	//��Ϸ�����ļ�
	::GetPrivateProfileString("Launch", "GameConfig", "", szTemp, MAX_PATH, szFullIni);
	m_strGameConfig = szTemp;

	//Patch�ļ�
	::GetPrivateProfileString("Launch", "PatchInfo", "", szTemp, MAX_PATH, szFullIni);
	m_strPatchFile.first = szTemp;
	m_strPatchFile.second = m_strPatchDir + "PatchInfo.txt";

	//LoginServer�ļ�
	::GetPrivateProfileString("Launch", "LoginServer", "", szTemp, MAX_PATH, szFullIni);
	m_strServerInfo.first = szTemp;
	m_strServerInfo.second = m_strPatchDir + "LoginServer.txt";

	//Resources�����ļ�,���ڶ�����Ϣ
	::GetPrivateProfileString("Launch", "Resources", "", szTemp, MAX_PATH, szFullIni);
	m_strFairyConfig = szTemp;

	//���汾��Ϣ��0:���а棨���¶�����MPQ������ɣ���������ɢ��Դ��	1���ڲ�����԰棨����ɢ�ļ���ʽ���£�     
	m_bTestVersion= ::GetPrivateProfileInt("Launch","Version",0,szFullIni);

	//���������߳�
	UINT nThreadID;
	m_hWorkThread = (HANDLE)_beginthreadex(0, 0, _workThread, this, CREATE_SUSPENDED, &nThreadID);
	::ResumeThread(m_hWorkThread);
}


/***************************************************
	�����߳�
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
	//�������ؿ��ƴ���
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


	//��������ϵͳ
	CString strErrInfo;
	m_pUpdateSystem = new UpdateSystem;

	//----------------------------------------------
	//�������� "�������"
	if( enterProcedure_CheckInternetState() == false )
	{
		CString strErrInfo;
		strErrInfo.LoadString( IDS_LOCAL_NET_ERR );
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);
		m_nUpdateExit = -2;
		return;
	}

	//----------------------------------------------
	//�������� "�����̿ռ�"
	if( enterProcedure_CheckDiskState() == false ) 
	{
		CString strErrInfo;
		strErrInfo.LoadString( IDS_NOT_ENOUGH_SPACE );
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);
		m_nUpdateExit = -5;
		return;
	}

	//----------------------------------------------
	//�������� "�������°汾��Ϣ",PatchInfo.txt
	if(!enterProcedure_DownloadPatchInfo()) 
	{
		m_nUpdateExit = -3;
		return;
	}
	else 
	{
		//��������ϵͳ
		CString strErrInfo;
		string strText = "PatchInfo.txt���سɹ�\n";
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

		strText = "����ϵͳ�����ɹ���\n";
		m_pUpdateSystem->Log(strText.c_str(),strText.size());
	}

	//----------------------------------------------
	//�������� "�������·�����������Ϣ",LoginServer.txt
	if(!enterProcedure_DownloadServerInfo()) 
	{
		string strText = "LoginServer.txt����ʧ��\n";
		m_pUpdateSystem->Log(strText.c_str(),strText.size());
		m_nUpdateExit = -3;
		return;
	}
	
	CString strText = "LoginServer.txt���سɹ�\n";
	m_pUpdateSystem->Log(strText,strText.GetLength());

	//----------------------------------------------
	//�������� "���ذ�װ���°汾" cjsh-1.4.1.2.zip
	if(!enterProcedure_DownloadPatch())
	{
		string strText = "���°����ػ�װʧ��\n";
		m_pUpdateSystem->Log(strText.c_str(),strText.size());
		m_nUpdateExit = -3;
		return;
	}

	strText = "���°����ء���װ���ɹ���\n";
	m_pUpdateSystem->Log(strText,strText.GetLength());

	::SendMessage(m_hNotifyWnd, NM_OK, 0, 0);

	//----------------------------------------------
	//�رմ���
	::DestroyWindow(m_hDownloadWnd);
	m_hDownloadWnd = 0;

	m_nUpdateExit = 0;
}


/*******************************************************
		���ع���������մ���
********************************************************/
LRESULT CALLBACK LaunchSystem::downloadWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LaunchSystem* pThis = (LaunchSystem*)(DWORD_PTR)::GetWindowLong(hWnd, GWL_USERDATA);
	if(pThis)
	{
		CAutoDownload* pDownload = pThis->m_pAutodownload;
		UpdateSystem* pUpdateSystem = pThis->m_pUpdateSystem;

		// ���ؿ�ʼ
		if(message == CAutoDownload::AM_BEGIN)
		{
			//��ʼ����...

			return TRUE;
		}
		// ״̬�����ı�
		else if(message == CAutoDownload::AM_STATUSUPDATE)
		{
			//��ʼ���ӷ�����...
			if(pDownload->getState() == CAutoDownload::AS_BUSY_QUERY)
			{
				CString strTemp;
				strTemp.Format(IDS_DOWNLOADING, pDownload->getUrlFileName().c_str());
				::SendMessage(pThis->m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strTemp, 0);

				strTemp.LoadString(IDS_DOWNLOAD_QUER);
				::SendMessage(pThis->m_hNotifyWnd, NM_FILE, (WPARAM)(LPCTSTR)(LPCTSTR)strTemp, 0);
			}
			//�ɹ�����!
			else if(pDownload->getStateCategory() == CAutoDownload::AC_FINISH_SUCCESS)
			{
				//...
				//::SendMessage(pThis->m_hNotifyWnd, NM_FILE, (WPARAM)(LPCTSTR)"���ؽ���", 0);
			}
			//ʧ�ܽ���!
			else if(pDownload->getStateCategory() == CAutoDownload::AC_FINISH_FAILED)
			{
				CString strTemp = "����ʧ��.";
				::SendMessage(pThis->m_hNotifyWnd, NM_FILE, (WPARAM)(LPCTSTR)"����ʧ��", 0);
			}

			return TRUE;
		}
		// �ߴ����
		else if(message == CAutoDownload::AM_SIZEUPDATE)
		{
			__int64 nFileSize, nReadSize;

			//ˢ�½�����
			pDownload->getFileSize(nFileSize, nReadSize);
			::SendMessage(pThis->m_hNotifyWnd, NM_PROCESS1, (WPARAM)(int)(nReadSize*100.0/nFileSize), 0);

			//ˢ���ٶ���ʾ
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
	����ĳһ��ָ������(�������״��)
********************************************************/
bool LaunchSystem::enterProcedure_CheckInternetState(void)
{
	if( InternetGetConnectedState( &m_dwInternetState, 0 ) == TRUE ) {
		return true;
	}
	return false;
}

/*******************************************************
	����ĳһ��ָ������(�����̿ռ�)
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
	����ĳһ��ָ������(�������°汾��Ϣ)
****************************************************/
bool LaunchSystem::enterProcedure_DownloadPatchInfo(void)
{
	//�趨����״̬
	m_theProcedure = PRO_DOWNLOAD_PATCH_INFO;	// �������°汾��Ϣ

	CString strDownloadingPatchInfo;
	strDownloadingPatchInfo.LoadString(IDS_DOWNLOADING_PATCH_INFO);
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);

	//�������°汾��Ϣ
	m_pAutodownload->init(m_strPatchFile.first.c_str(), m_strPatchFile.second.c_str(), m_hDownloadWnd, PRO_DOWNLOAD_PATCH_INFO);
	m_pAutodownload->begin();

	//��������
	m_pAutodownload->waitStop(true);

	//�Ƿ�ɹ�����ʱĬ���ǳɹ���
	if(CAutoDownload::AC_FINISH_SUCCESS != m_pAutodownload->getStateCategory())
	{
		CString strDownloadPatchFault;
		strDownloadPatchFault.LoadString(IDS_DOWNLOAD_PATCH_INFO_FAULT);
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strDownloadPatchFault, 0);
		m_pUpdateSystem->Log(strDownloadPatchFault,strDownloadPatchFault.GetLength());
		return false;
	}

	//����
	m_pAutodownload->reset();

	strDownloadingPatchInfo.LoadString(IDS_DOWNLOADING_PATCH_INFO_OK);
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);

	return true;
}

/***************************************************
	����ĳһ��ָ������(�������°汾)
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

	//ѭ���õ�Ҫ���ص�Patch����Ϣ��Ȼ������
	char szPath[1024], szFile[1024], szTemp[1024];
	_splitpath( m_strPatchFile.second.c_str(), szPath, szTemp, NULL, NULL );
	PathAppend( szPath, szTemp );
	for( int n = 0; n < m_pUpdateSystem->GetUpdateStep(); n ++ ) 
	{
		const PatchInfo* ppi = m_pUpdateSystem->GetUpdateInfo( n );
		strcpy( szTemp, ppi->urlInfo[0].strURL.c_str() );

		//���URL
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

		//����ļ���
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
		strDownloadingPatchInfo.Format("�����ļ�%s", strFileName );
		::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);
		strDownloadingPatchInfo += "\n";
		m_pUpdateSystem->Log(strDownloadingPatchInfo,strDownloadingPatchInfo.GetLength());

		if( PathFileExists( szFile ) == TRUE )	//�������иø��°�
		{
			CString strErrInfo ;
			strErrInfo.Format("\n\n�������и��°�%s,���ڼ����Ϸ��ԡ���\n",szFile);
			m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());

			CString strInfo;

			//��鱾���ļ��Ϸ��ԣ���Ҫ���version��command�ļ�
			if( m_pUpdateSystem->PreUpdate( n, szFile, strInfo ) == TRUE ) 
			{
				strErrInfo.Format("����飬����%s�ļ��������°棬��������\n",szFile);
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());

				strErrInfo.Format("��ʼ��װ�ð汾����\n");
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());

				//�������� "��װ���°汾"
				if(!enterProcedure_Install()) 
				{
					strErrInfo.Format("�ð汾��װʧ�ܡ���\n\n");
					m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
					m_nUpdateExit = -4;
					return( false );
				}

				strErrInfo.Format("�ð汾��װ�ɹ�����\n\n",n+1);
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
				continue;
			}
			else
			{	
				strErrInfo.Format("����飬����%s�ļ����𻵣�ɾ������������\n\n",szFile);
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
				strErrInfo.Format("��%s�ļ�����ļ�����", strFileName);
				::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strErrInfo, 0);

				strErrInfo += "\n";
				m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
				return false;
			}

			m_pAutodownload->reset();

			CString strErrInfo;
			strErrInfo.Format("����%s�ɹ�!", strFileName);
			::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strErrInfo, 0);
			m_pUpdateSystem->Log(strErrInfo,strErrInfo.GetLength());
		}

	
		CString strInfo;
		//��������ļ��Ϸ��ԣ���Ҫ���version��command�ļ�
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
		strText.Format("\n\n��%d�����°����سɹ�\n\n",n+1);
		m_pUpdateSystem->Log(strText,strText.GetLength());

		strText.Format("\n\n��ʼ��װ��%d���°汾����\n\n",n+1);
		m_pUpdateSystem->Log(strText,strText.GetLength());

		//�������� "��װ���°汾"
		if(!enterProcedure_Install()) 
		{
			strText.Format("\n\n��%d���°汾��װʧ�ܡ���\n\n",n+1);
			m_pUpdateSystem->Log(strText,strText.GetLength());
			m_nUpdateExit = -4;
			return( false );
		}

		strText.Format("\n\n��%d���°汾��װ�ɹ�����\n\n",n+1);
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
	����ĳһ��ָ������(�������·�����������Ϣ)
****************************************************/
bool LaunchSystem::enterProcedure_DownloadServerInfo(void)
{
	//�趨����״̬
	m_theProcedure = PRO_DOWNLOAD_SERVER_INFO;

	CString strDownloadingPatchInfo;
	strDownloadingPatchInfo.LoadString(IDS_DOWNLOAD_SRV_INFO);
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);

	//�������·�����������Ϣ
	m_pAutodownload->init(m_strServerInfo.first.c_str(), m_strServerInfo.second.c_str(), m_hDownloadWnd, PRO_DOWNLOAD_PATCH_INFO);
	m_pAutodownload->begin();

	//��������
	m_pAutodownload->waitStop(true);

	//�Ƿ�ɹ�����ʱĬ���ǳɹ���
	if(CAutoDownload::AC_FINISH_SUCCESS != m_pAutodownload->getStateCategory())
	{
		CString strDownloadPatchFault;
		strDownloadPatchFault.LoadString(IDS_DOWNLOAD_SRV_ERR);
		::SendMessage(m_hNotifyWnd, NM_FAULT, (WPARAM)(LPCTSTR)strDownloadPatchFault, 0);
		m_pUpdateSystem->Log(strDownloadPatchFault,strDownloadPatchFault.GetLength());
		return false;
	}

	//����
	m_pAutodownload->reset();

	strDownloadingPatchInfo.LoadString(IDS_DOWNLOAD_SRV_OK);
	::SendMessage(m_hNotifyWnd, NM_STATUS, (WPARAM)(LPCTSTR)strDownloadingPatchInfo, 0);
	::SendMessage(m_hNotifyWnd, NM_FILE, (WPARAM)"", 0);

	return true;
}

/***************************************************
	����ĳһ��ָ������(��װ���°汾)
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

		strUpdateInfo = "����ò���ѳɹ����㻹���¸�ɶ��ѽ��\n";
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

	//���һ�£��Ա��´θ���
	m_pUpdateSystem->ClearMyself();
	return true;
}

/***************************************************
	������Ϸ
****************************************************/
void LaunchSystem::runGame(void)
{
	//��ǰ·��
	CHAR szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	//��Ϸexe
	CHAR szGameExe[MAX_PATH];
	strncpy(szGameExe, szCurrentDir, MAX_PATH);
	PathAppend(szGameExe, m_strGameExe.c_str());
	//PathAppend(szGameExe, "Bin/game/Game.exe");

	//��Ϸ����Ŀ¼
	CHAR szGamePath[MAX_PATH];
	strncpy(szGamePath, szGameExe, MAX_PATH);
	::PathRemoveFileSpec(szGamePath);

	//������
	CHAR szGameCommand[MAX_PATH*2];

	if( IsTestVersion() )
	{
		_snprintf(szGameCommand, MAX_PATH*2, "%s -test", szGameExe);
	}
	else
	{
		_snprintf(szGameCommand, MAX_PATH*2, "%s -fl", szGameExe);
	}


	//��������
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
		CString strText = "����Gameʧ��!";
		//����ʧ��
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
