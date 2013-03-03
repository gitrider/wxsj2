
#include "StdAfx.h"
#include "UpdateSystem.h"
#include "DownManager.h"
#include "../axtrace.h"

namespace DOWNANT
{


//================================================================
CDownManager::CDownManager()
{
	m_theStateCategory = IDLE;
	m_theState = IDLE_NOTINIT;

	m_strURLToDownload[0] = 0;
	m_strFileToDownloadInto[0] = 0;

	m_pOwner = NULL;

	m_strStatusInfor[0] = 0;

	UserCallBack = NULL;

	ZeroMemory(m_vDownAnt, sizeof(AntSection)*MAX_ANT_NUM);
	for(int i=0; i<MAX_ANT_NUM; i++)m_vDownAnt[i].nNow=-1;

	m_nAntNum = 0;
	m_nFileSize = 0;
	m_nFileRead = 0;

	m_nBlockNum = 0;

	m_nSpeed = 0;

	m_hAllOverEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bUnioning = FALSE;
}

CDownManager::~CDownManager()
{
	::CloseHandle(m_hAllOverEvent);
	m_ThreadPool.ShutDown();
}

BOOL CDownManager::Init(
						LPCTSTR strURLToDownload, 
						LPCTSTR strFileToDownloadInto,
						DOWNMANAGER_STATUS_CALLBACK funcStatusFunc, 
						VOID* pOwner)

{
	assert(m_theStateCategory != BUSY);
	assert(strURLToDownload && strFileToDownloadInto);

	_snprintf(m_strURLToDownload, INTERNET_MAX_URL_LENGTH, "%s", strURLToDownload);
	_snprintf(m_strFileToDownloadInto, MAX_PATH, "%s", strFileToDownloadInto);

	m_pOwner = pOwner;
	UserCallBack = funcStatusFunc;

	if(strlen(m_strURLToDownload) > 0 && strlen(m_strFileToDownloadInto) > 0)
	{
		//监测URL合法性
		DWORD dwServiceType;
		CHAR  strServer[MAX_PATH];
		CHAR  strObject[INTERNET_MAX_URL_LENGTH];
		WORD  nPort;

		if (!AfxParseURL( m_strURLToDownload, dwServiceType, strServer, strObject, nPort) || dwServiceType==INTERNET_SCHEME_UNKNOWN)
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "非法的下载地址: %s", m_strURLToDownload);
			UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
			SetStatus(IDLE_INIT_FAILED);
			return FALSE;
		}
		
		SetStatus(IDLE_INIT_SUCCESS);
	}
	else
	{
		_snprintf(m_strStatusInfor, MAX_PATH, "下载地址和本地文件地址不能为空" );
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		SetStatus(IDLE_INIT_FAILED);
		return FALSE;
	}
	//取得临时文件名
	_snprintf(m_szTempFileName, MAX_PATH, "%s%s",strFileToDownloadInto,".tmp" );

	//创建线程锁
	InitializeCriticalSection(&m_threadSection);

	//设置对一个服务器可以同时进行的Request数
	// 参见MS:Q183110(http://support.microsoft.com/kb/183110)
	//如果不进行这项设置，会出现只有两个线程能同时工作
	DWORD dwNumLimit = MAX_ANT_NUM+1;
	::InternetSetOption(NULL, INTERNET_OPTION_MAX_CONNS_PER_SERVER, &dwNumLimit, sizeof(DWORD));
	::InternetSetOption(NULL, INTERNET_OPTION_MAX_CONNS_PER_1_0_SERVER, &dwNumLimit, sizeof(DWORD));
	//开始运行线程池
	m_ThreadPool.Begin();
	return TRUE;
}

void CDownManager::Begin(BOOL bBlock)
{
	CDownAnt* pDownMan = new CDownAnt(0);
	strcpy(m_vDownAnt[0].szFileName, m_szTempFileName);
	strcat( m_vDownAnt[0].szFileName, ".0");

	pDownMan->m_bTrace = TRUE;
	pDownMan->Init(
		m_strURLToDownload,
		m_vDownAnt[0].szFileName, 
		_AntDownStatusCallBack, 
		this);

	//开始第一个蚂蚁下载
	m_vDownAnt[0].pAnt = pDownMan;
	if(UserCallBack)
	{
		UserCallBack(m_pOwner, DMCC_NEW_ANT, (WPARAM)0, (LPARAM)pDownMan);
	}
	m_ThreadPool.PushRequest(pDownMan);

	SetStatus(BUSY_QUERY);
	if(bBlock)
	{
		::ResetEvent(m_hAllOverEvent);
		::WaitForSingleObject(m_hAllOverEvent, INFINITE);
	}
}

void CDownManager::ShutDown(BOOL bRightNow)
{
	if(bRightNow)
	{
		m_nAntNum = 0;
		m_ThreadPool.ShutDown();
	}
	else
	{
		for(int i=0; i<m_nAntNum; i++)
		{
			if(m_vDownAnt[i].pAnt)
			{
				m_vDownAnt[i].pAnt->ForceBreak();
			}
		}
	}
}

DWORD CDownManager::_AntDownStatusCallBack(VOID* pOwner, VOID* pDownAnt)
{
	if(pOwner)
	{
		return ((CDownManager*)pOwner)->AntDownStatusCallBack(pDownAnt);
	}
	return MANAGER_CMD_FORCEABORT;
}

DWORD CDownManager::AntDownStatusCallBack(VOID* pDownAnt)
{
	//! 可能会被多个线程调用
	EnterCriticalSection(&m_threadSection);
	
	__try
	{
	CDownAnt* pAnt = (CDownAnt*)pDownAnt;
	CDownAnt::DOWN_STATE_CATEGORY staCat = pAnt->GetStateCategory();
	CHAR szMsg[MAX_PATH] = {0};
	
	DWORD dwAntNum = pAnt->GetID();
	if(dwAntNum < 0 || dwAntNum>=MAX_ANT_NUM) 
		return MANAGER_CMD_FORCEABORT;

	switch(staCat)
	{
	case CDownAnt::IDLE:
		{
			if(CDownAnt::IDLE_INIT_SUCCESS == pAnt->GetState())
			{
				if(UserCallBack)
				{
					UserCallBack(m_pOwner, DMCC_ANT_LOG, (WPARAM)dwAntNum, (LPARAM)"初始化成功");
				}
			}
		}
		break;

	case CDownAnt::BUSY:
		{
			switch(pAnt->GetState())
			{
			case CDownAnt::BUSY_OPEN_REQUEST:
				{
					if(dwAntNum >0 && (INT)dwAntNum <m_nAntNum && m_vDownAnt[dwAntNum].pAnt)
					{
						//设置蚂蚁的下载范围
						if ( m_vDownAnt[dwAntNum].pAnt->m_bUseBreakpoint)
						{
							m_vDownAnt[dwAntNum].nStart += m_vDownAnt[dwAntNum].pAnt->GetFileStartPos()/DOWNANT_MINSIZE;
						}
						m_vDownAnt[dwAntNum].pAnt->SetFilePoint(m_vDownAnt[dwAntNum].nStart*DOWNANT_MINSIZE);
				
					}
				}
				break;

			case CDownAnt::BUSY_RESOLVING_NAME:
				{
					SetStatus(BUSY_FIRST_BEGIN);
					if(UserCallBack)
					{
						_snprintf(szMsg, MAX_PATH, "查找服务器: %s...", pAnt->GetServer());
						UserCallBack(m_pOwner, DMCC_ANT_LOG, (WPARAM)dwAntNum, (LPARAM)szMsg);
					}
				}
				break;

			case CDownAnt::BUSY_CONNECTING:
				{
					if(UserCallBack)
					{
						_snprintf(szMsg, MAX_PATH, "连接服务器: %s...", pAnt->GetServer());
						UserCallBack(m_pOwner, DMCC_ANT_LOG, (WPARAM)dwAntNum, (LPARAM)szMsg);
					}
				}
				break;

			case CDownAnt::BUSY_CONNECTED:
				{
					if(UserCallBack)
					{
						_snprintf(szMsg, MAX_PATH, "连接服务器成功!");
						UserCallBack(m_pOwner, DMCC_ANT_LOG, (WPARAM)dwAntNum, (LPARAM)szMsg);
					}
				}
				break;

			case CDownAnt::BUSY_REDIRECTING:
				{
					if(UserCallBack)
					{
						_snprintf(szMsg, MAX_PATH, "文件重新定位");
						UserCallBack(m_pOwner, DMCC_ANT_LOG, (WPARAM)dwAntNum, (LPARAM)szMsg);
					}
				}
				break;

			case CDownAnt::BUSY_GET_FILE_INFORMATION:
				{
					if(UserCallBack)
					{
						BOOL bGotFileSize;
						__int64 nFileSizeTotal, nFileSizeRead;
						pAnt->GetFileSize(bGotFileSize, nFileSizeTotal, nFileSizeRead);
						if(!bGotFileSize)
							return MANAGER_CMD_FORCEABORT;
					}

					//开始其他蚂蚁的工作
					OnGetFileInformation(dwAntNum);
				}
				break;

			case CDownAnt::BUSY_BEGIN_DOWNFILE:
				{
					BOOL bGotFileSize;
					__int64 nFileSizeTotal, nFileSizeRead;
					pAnt->GetFileSize( bGotFileSize, nFileSizeTotal, nFileSizeRead);
					if(nFileSizeRead==0)
					{
						UserCallBack(m_pOwner, DMCC_ANT_LOG, (WPARAM)dwAntNum, (LPARAM)"开始下载文件");
					}

					__int64 nBlockRead = nFileSizeRead/DOWNANT_MINSIZE;
					if(nBlockRead*DOWNANT_MINSIZE < nFileSizeRead) 
						nBlockRead++;

					__int64 nBlockPosNow = m_vDownAnt[dwAntNum].nStart + nBlockRead;
					m_vDownAnt[dwAntNum].nNow = nBlockPosNow;

					m_nFileRead += DOWNANT_MINSIZE;
					if(m_nFileRead > m_nFileSize) m_nFileRead = m_nFileSize;

					DWORD dwStep = ::GetTickCount()-m_dwBeginTickCount;
					if(dwStep==0) 
						m_nSpeed=0;
					else
						m_nSpeed = (int)((float)m_nFileRead/((float)dwStep/1000.0f));
					if(UserCallBack)
					{
						UserCallBack(m_pOwner, DMCC_SIZEUPDATE, 
							(WPARAM)dwAntNum, 
							(LPARAM)(m_vDownAnt[dwAntNum].nStart*DOWNANT_MINSIZE + nFileSizeRead));
					}
					//该蚂蚁下载结束
					if(  m_vDownAnt[dwAntNum].nEnd > 0 && m_vDownAnt[dwAntNum].nNow >= m_vDownAnt[dwAntNum].nEnd)
					{
						//结束该蚂蚁的工作
						return MANAGER_CMD_STOPHERE;
					}
				}
				break;
			}
		}
		break;

	case CDownAnt::FINISH_SUCCESS:
		{
			AxTrace(0, 1, "Ant Over:%d", dwAntNum);
			OnAntWorkOver(dwAntNum);
			if(UserCallBack)
			{
				UserCallBack(m_pOwner, DMCC_ANT_LOG, (WPARAM)dwAntNum, (LPARAM)"下载结束");
			}

		}
		break;

	case CDownAnt::FINISH_NOT_SUCCESS:
		{
			OnAntWorkFailed(dwAntNum);
			if(UserCallBack)
			{
				UserCallBack(m_pOwner, DMCC_ANT_LOG, (WPARAM)dwAntNum, (LPARAM)"下载失败");
			}
		}
		break;

	};
	
	return MANAGER_CMD_CONTINUE;

	}
	__finally {
		LeaveCriticalSection(&m_threadSection);
	}


}

void CDownManager::SetStatus(DOWN_STATE newState)
{
	m_theState = newState;

	switch(m_theState)
	{
	case IDLE_NOTINIT:
	case IDLE_INIT_SUCCESS:
	case IDLE_INIT_FAILED:
		{
			m_theStateCategory = IDLE;
		}
		break;

	case BUSY_QUERY:
	case BUSY_FIRST_BEGIN:
	case BUSY_UNION_BEGIN:
		{
			m_theStateCategory = BUSY;
		}
		break;

	case FINISH_SUCCESS_ALLOVER:
		{
			m_theStateCategory = FINISH_SUCCESS;
		}
		break;

	case FINISH_NOT_SUCCESS_UNIONFILE:
	case FINISH_NOT_SUCCESS_ABORT:
		{
			m_theStateCategory = FINISH_NOT_SUCCESS;
		}
		break;

	default:
		assert(false);
		break;
	}

	switch(m_theStateCategory)
	{
	case FINISH_SUCCESS:
		{
			//删除临时文件
			for(int i=0; i<m_nAntNum; i++)
			{
				::DeleteFile(m_vDownAnt[i].szFileName);
			
				char szBuf[1024];
				sprintf( szBuf, "删除临时文件%s.", m_vDownAnt[i].szFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			}
		}
		break;

	case FINISH_NOT_SUCCESS:
		{
			//删除临时文件
			for(int i=1; i<m_nAntNum; i++)
			{
				::DeleteFile(m_vDownAnt[i].szFileName);
				char szBuf[1024];
				sprintf( szBuf, "删除临时文件%s.", m_vDownAnt[i].szFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			}

			DeleteFile(m_strFileToDownloadInto);
			char szBuf[1024];
			sprintf( szBuf, "删除临时文件%s.",m_strFileToDownloadInto );
			UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
		}
	}

	if(UserCallBack)
	{
		UserCallBack(m_pOwner, DMCC_STATUS_CHANGE, (WPARAM)m_theStateCategory, (LPARAM)m_theState);
	}
}

void CDownManager::OnGetFileInformation(DWORD dwAntNum)
{
	//第一个蚂蚁得到尺寸
	if(dwAntNum == 0)
	{
		BOOL bGotFileSize;
		__int64 nFileSizeRead;
		m_vDownAnt[0].pAnt->GetFileSize(bGotFileSize, m_nFileSize, nFileSizeRead);
		if(bGotFileSize)
		{
			//每250K使用1个蚂蚁
			__int64 nAntNum = m_nFileSize/(250*1024);
			if(nAntNum*(250*1024) < m_nFileSize)
				nAntNum++;
			if(nAntNum > MAX_ANT_NUM) 
				nAntNum=MAX_ANT_NUM;
			m_nAntNum = (int)nAntNum;

			//计算出节点数(4K一个节点)
			m_nBlockNum = m_nFileSize/DOWNANT_MINSIZE;
			if(m_nBlockNum * DOWNANT_MINSIZE < m_nFileSize) 
				m_nBlockNum++;

			//平均一个蚂蚁的节点数
			__int64 nAntBlock = m_nBlockNum/m_nAntNum;
			if(nAntBlock*m_nAntNum < m_nBlockNum) 
				nAntBlock++;

			__int64 nNow = 0;
			for(int i=0; i<m_nAntNum; i++)
			{
				//设置文件范围
				m_vDownAnt[i].nStart = nNow;
				if ( i == 0 && m_vDownAnt[i].pAnt->m_bUseBreakpoint)
				{
					m_vDownAnt[i].nStart = m_vDownAnt[i].pAnt->GetFileStartPos()/DOWNANT_MINSIZE;
				}

				m_vDownAnt[i].nEnd = nNow + nAntBlock;

				if(i==m_nAntNum-1)
					m_vDownAnt[i].nEnd=m_nBlockNum;

				m_vDownAnt[i].nNow = -1;

				AxTrace(0, 1, "SetRange(%d)[%d,%d)", i, 
					nNow*DOWNANT_MINSIZE, m_vDownAnt[i].nEnd*DOWNANT_MINSIZE);

				//开始其他蚂蚁的下载
				if(i != 0)
				{
					strcpy(m_vDownAnt[i].szFileName, m_szTempFileName);
					CHAR szTemp[32];
					_snprintf(szTemp, 32, ".%d", i);
					strcat( m_vDownAnt[i].szFileName, szTemp);

					CDownAnt* pDownMan = new CDownAnt(i);
					pDownMan->m_bTrace = TRUE;
					pDownMan->Init(
						m_strURLToDownload,
						m_vDownAnt[i].szFileName, 
						_AntDownStatusCallBack, 
						this);

					//开始第一个蚂蚁下载

					m_vDownAnt[i].pAnt = pDownMan;
					if ( m_vDownAnt[i].pAnt->m_bUseBreakpoint)
					{
						m_vDownAnt[i].nStart = m_vDownAnt[i].pAnt->GetFileStartPos()/DOWNANT_MINSIZE;
					}
					if(UserCallBack)
					{
						UserCallBack(m_pOwner, DMCC_NEW_ANT, (WPARAM)i, (LPARAM)pDownMan);
					}
					m_ThreadPool.PushRequest(pDownMan);
				}
				nNow+=nAntBlock;
			}
			//开始计时
			m_dwBeginTickCount = ::GetTickCount();
			AxTrace(0, 0, "BeginTickCount: %d", m_dwBeginTickCount);
		}

		if(UserCallBack)
		{
			UserCallBack(m_pOwner, DMCC_FILEINFO, (WPARAM)m_nFileSize, (LPARAM)m_nAntNum);
		}
	}
}

void CDownManager::OnAntWorkFailed(DWORD dwAntNum)
{
	AxTrace(0, 1, "OnAntWorkFailed[%d] [%d,%d)", dwAntNum,
		m_vDownAnt[dwAntNum].nStart,  m_vDownAnt[dwAntNum].nEnd);

	//单线程
	if(m_nAntNum==1)
	{
		SetStatus(FINISH_NOT_SUCCESS_ABORT);
		::SetEvent(m_hAllOverEvent);
		return;
	}

	//看是否所有的蚂蚁都工作完毕
	int nOverNum = 0;
	for(int i=0; i<m_nAntNum; i++)
	{
		if(m_vDownAnt[i].pAnt->GetStateCategory() != BUSY)
		{
			nOverNum++;
		}
	}

	//全部结束
	if(nOverNum==m_nAntNum)
	{
		SetStatus(FINISH_NOT_SUCCESS_ABORT);
		::SetEvent(m_hAllOverEvent);
	}
}

void CDownManager::OnAntWorkOver(DWORD dwAntNum)
{
	AxTrace(0, 1, "OnAntWorkOver[%d] [%d,%d)", dwAntNum,
		m_vDownAnt[dwAntNum].nStart,  m_vDownAnt[dwAntNum].nEnd);

	//单线程
	if(m_nAntNum==1 && !m_bUnioning)
	{
		m_bUnioning = TRUE;
		CopyFile( m_vDownAnt[0].szFileName, m_strFileToDownloadInto, FALSE);
		m_bUnioning = FALSE;
		SetStatus(FINISH_SUCCESS_ALLOVER);
		::SetEvent(m_hAllOverEvent);
		return;
	}
	
	//看是否所有的蚂蚁都工作完毕
	int nOverNum = 0;
	for(int i=0; i<m_nAntNum; i++)
	{
		if(m_vDownAnt[i].nNow == m_vDownAnt[i].nEnd)
		{
			nOverNum++;
		}
	}

	//全部下载完毕
	if( nOverNum==m_nAntNum && !m_bUnioning)
	{
//		AxTrace(0, 2, "AllOver!!!", dwAntNum);
		m_bUnioning = TRUE;
		//合并文件
		FILE* fpBig = fopen(m_strFileToDownloadInto, "wb");
		if(fpBig == NULL)
		{
			char szBuf[1024];
			sprintf( szBuf, "打开文件%s失败.", m_strFileToDownloadInto );
			UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			SetStatus(FINISH_NOT_SUCCESS_UNIONFILE);
			return;
		}
		SetStatus(BUSY_UNION_BEGIN);

		__int64 nFileSize = m_vDownAnt[0].nEnd * DOWNANT_MINSIZE;

		BYTE* pTemp = new BYTE[(size_t)nFileSize+32];
		if(pTemp == NULL) 
		{
			char szBuf[1024];
			sprintf( szBuf, "内存分配失败." );
			UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			fclose(fpBig);
			SetStatus(FINISH_NOT_SUCCESS_UNIONFILE);
			return;
		}

		char szBuf[1024];
		sprintf( szBuf, "为合并数据分配%u + 32 空间.",nFileSize );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		for(int i=0; i<m_nAntNum; i++)
		{
			FILE* fpSmall = fopen(m_vDownAnt[i].szFileName, "rb");
			if(fpSmall==NULL)
			{
				char szBuf[1024];
				sprintf( szBuf, "打开文件%s失败.", m_vDownAnt[i].szFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
				fclose(fpBig);
				SetStatus(FINISH_NOT_SUCCESS_UNIONFILE);
				return;
			}

			int nReadSize = (int)fread(pTemp, 1, (size_t)nFileSize, fpSmall);
			if(nReadSize > 0)
			{
				int Write = fwrite( pTemp, 1, nReadSize, fpBig);
				if ( Write!=nFileSize )
				{
					char szBuf[1024];
					sprintf( szBuf, "写文件%s失败,欲写入%u实际只写入%u字节!\n",m_strFileToDownloadInto,nReadSize,Write );
					UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );	
				}
	
				char szBuf[1024];
				sprintf( szBuf, "第%d次向%s文件写入%d字节数据.",i+1,m_strFileToDownloadInto,nReadSize );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			}
			fclose(fpSmall);
		}

		delete[] pTemp;
		fclose(fpBig);

		m_bUnioning = FALSE;
		SetStatus(FINISH_SUCCESS_ALLOVER);
		::SetEvent(m_hAllOverEvent);
		m_nAntNum = 0;
	}
}

BOOL CDownManager::WaitOver(BOOL bBlock)
{
	if(bBlock)
	{
		while(true)
		{
			if( WAIT_OBJECT_0  == ::MsgWaitForMultipleObjects(1, &m_hAllOverEvent, 
										FALSE, INFINITE, QS_ALLINPUT))
			{
				break;
			}

			//继续处理消息循环
			MSG msg;
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{ 
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			};
		};

		return TRUE;
	}
	else
	{
		return WAIT_OBJECT_0 == WaitForSingleObject(m_hAllOverEvent, 0);
	}
}


}