
#include "StdAfx.h"
#include "DownAnt.h"
#include "UpdateSystem.h"
#include "DownManager.h"
//#include <assert.h>
#include "../axtrace.h"

namespace DOWNANT
{


CDownAnt::CDownAnt(DWORD dwID)
{
	m_dwID = dwID;

	m_theStateCategory = IDLE;
	m_theState = IDLE_NOTINIT;
	
	m_strURLToDownload[0] = 0;
	m_strFileToDownloadInto[0] = 0;

	m_pOwner = NULL;

	m_dwServiceType = INTERNET_SCHEME_UNKNOWN;
	m_strServer[0] = 0;
	m_strObject[0] = 0;
	m_nPort = 80;

	m_hInternetSession = NULL;
	m_hHttpConnection = NULL;
	m_hHttpFile = NULL;

	m_dwUserCmd = MANAGER_CMD_CONTINUE;
	m_bForceBreak = FALSE;
	m_nOverTime = INFINITE;

	m_bGotFileSize = FALSE;
	m_dwFileSizeTotal = 0;
	m_dwFileSizeRead = 0;
	m_fpFileToWrite = NULL;
	m_bUseBreakpoint = FALSE;
	m_nFileStartPos = 0;

	UserCallBack = NULL;

	m_bTrace = FALSE;
	m_strStatusInfor[0] = 0;
}

CDownAnt::~CDownAnt()
{
	//及时关闭与外界的联系,如果下载线程还在运行中,这步很重要!
	AxTrace((BYTE)m_dwID, 0, "~CDownAnt");
	UserCallBack = NULL;
	if (m_hHttpFile)
	{
		::InternetCloseHandle(m_hHttpFile);
		m_hHttpFile = NULL;
	}
	if (m_hHttpConnection)
	{
		::InternetCloseHandle(m_hHttpConnection);
		m_hHttpConnection = NULL;
	}
	if (m_hInternetSession)
	{
		::InternetCloseHandle(m_hInternetSession);
		m_hInternetSession = NULL;
	}
}

BOOL CDownAnt::Init(
					LPCTSTR strURLToDownload, 
					LPCTSTR strFileToDownloadInto,
					DOWNANT_STATUS_CALLBACK funcUserCallBack, 
					VOID* pOwner,
					DWORD nOverTime)
{
	assert(m_theStateCategory != BUSY);
	assert(strURLToDownload && strFileToDownloadInto);

	_snprintf(m_strURLToDownload, INTERNET_MAX_URL_LENGTH, "%s", strURLToDownload);
	_snprintf(m_strFileToDownloadInto, MAX_PATH, "%s", strFileToDownloadInto);
	
	m_pOwner = pOwner;

	UserCallBack = funcUserCallBack;

	if(strlen(m_strURLToDownload) > 0 && strlen(m_strFileToDownloadInto) > 0)
	{
		if (!AfxParseURL(m_strURLToDownload, m_dwServiceType, m_strServer, m_strObject, m_nPort) || m_dwServiceType==INTERNET_SCHEME_UNKNOWN)
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "地址解析错误: %s", m_strURLToDownload);
			SetStatus(IDLE_INIT_FAILED);
			UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
			return FALSE;
		}
		SetStatus(IDLE_INIT_SUCCESS);
		return TRUE;
	}
	else
	{
		_snprintf(m_strStatusInfor, MAX_PATH, "下载地址和本地文件地址不能为空" );
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		SetStatus(IDLE_INIT_FAILED);
		return FALSE;
	}

	m_nOverTime = nOverTime;
}	

void CDownAnt::ForceBreak(void)
{
	assert(this);
	m_bForceBreak = TRUE;
}

BOOL CDownAnt::SetFilePoint(__int64 nPosition)
{
	assert(this);
	assert(m_hHttpFile);

	//定位文件
	//if (!m_bUseBreakpoint)
	m_nFileStartPos = nPosition;
	AxTrace((BYTE)m_dwID, 0, "SetFilePoint :%d", m_nFileStartPos);
	return TRUE;
}

void CDownAnt::ThreadProc(int nThreadId)
{
	assert(m_theState == IDLE_INIT_SUCCESS);
	assert(m_hInternetSession == NULL);
	assert(m_fpFileToWrite == NULL);
	assert(m_hHttpConnection == NULL);
	assert(m_hHttpFile == NULL);


	if((m_fpFileToWrite = ::fopen(m_strFileToDownloadInto, "ab")) != NULL)
	{
		char szBuf[1024];
		sprintf( szBuf, "%s文件已存在，使用断点续传",m_strFileToDownloadInto );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		//该文件已存在，使用断点续传
		m_bUseBreakpoint = TRUE;
		
		//得到当前已下载文件大小
		fseek(m_fpFileToWrite, 0, SEEK_END);
		m_nFileStartPos = ftell(m_fpFileToWrite);
		if (m_nFileStartPos==0)
			m_bUseBreakpoint = FALSE;		
		fseek(m_fpFileToWrite, 0, SEEK_SET);


	}
	else
	{
		//不存在该文件，重新打开，
		if((m_fpFileToWrite = ::fopen(m_strFileToDownloadInto, "wb")) == NULL)
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "本地文件创建失败: %s, Error:%d", m_strFileToDownloadInto, ::GetLastError());
			UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
			SetStatus(FINISH_ERROR_TRY_OPEN_LOCALFILE);
			return;
		}	

		m_bUseBreakpoint = FALSE;
	}

	//打开Internet句柄
	m_hInternetSession = ::InternetOpen("DOWN_ANT_BY_JEAN", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (m_hInternetSession == NULL)
	{
		_snprintf(m_strStatusInfor, MAX_PATH, "(::InternetOpen) Error:%d\n", ::GetLastError());
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		SetStatus(FINISH_ERROR_INTERNET_OPEN);
		return;
	}
	if(CheckAbort()) return;
	AxTrace((BYTE)m_dwID, 0, "After ::InternetOpen");

	DWORD dwTemp=0, dwSize = sizeof(DWORD);
	BOOL bRet = InternetQueryOption( m_hInternetSession, 
		INTERNET_OPTION_MAX_CONNS_PER_SERVER, &dwTemp, &dwSize);
	DWORD dwErr = GetLastError();

	//设置状态回调函数
	if (::InternetSetStatusCallback(m_hInternetSession, _OnWininetStatusCallBack) == INTERNET_INVALID_STATUS_CALLBACK)
	{
		_snprintf(m_strStatusInfor, MAX_PATH, "(::InternetSetStatusCallback), Error:%d\n", ::GetLastError());
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		SetStatus(FINISH_ERROR_SETSTATUSCALLBACK);
		return;
	}
	if(CheckAbort()) return;

	//打开Internet连接
    m_hHttpConnection = ::InternetConnect(m_hInternetSession, m_strServer, m_nPort, NULL, 
		NULL, m_dwServiceType, 0, (DWORD)(DWORD_PTR)this);

	if (m_hHttpConnection == NULL)
	{
		_snprintf(m_strStatusInfor, MAX_PATH, "(::InternetConnect), Error:%d\n", ::GetLastError());
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		SetStatus(FINISH_ERROR_INTERNETCONNECT);
		return;
	}
	if(CheckAbort()) return;
	AxTrace((BYTE)m_dwID, 0, "After ::InternetConnect");

	//生成Request
	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = "*/*";
	ppszAcceptTypes[1] = NULL;
	m_hHttpFile = ::HttpOpenRequest(m_hHttpConnection, 
		NULL, m_strObject, NULL, NULL, ppszAcceptTypes, 
		INTERNET_FLAG_RELOAD, 
		(DWORD)(DWORD_PTR) this);
	if (m_hHttpFile == NULL)
	{
		_snprintf(m_strStatusInfor, MAX_PATH, "(::HttpOpenRequest), Error:%d\n", ::GetLastError());
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		SetStatus(FINISH_ERROR_OPENREQUEST);
		return;
	}
	if(CheckAbort()) return;
	AxTrace((BYTE)m_dwID, 0, "After ::HttpOpenRequest");
	SetStatus(BUSY_OPEN_REQUEST);

	//定位文件
	CHAR szAddHeads[MAX_PATH] = {0};
	if(m_nFileStartPos > 0)
	{
		_snprintf(szAddHeads, MAX_PATH, "Range: bytes=%d-", m_nFileStartPos);
		UpdateSystem::GetMe()->Log( szAddHeads, (DWORD)strlen( szAddHeads ) );
		::HttpAddRequestHeaders(m_hHttpFile, szAddHeads, -1, HTTP_ADDREQ_FLAG_ADD);
	}

	//发送Request
	BOOL bSend = ::HttpSendRequest( m_hHttpFile, NULL, 0, NULL, 0);
	if (!bSend)
	{
		_snprintf(m_strStatusInfor, MAX_PATH, "(::HttpSendRequest), Error:%d\n", ::GetLastError());
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		SetStatus(FINISH_ERROR_SENDREQUEST);
		return;
	}
	if(CheckAbort()) return;
	//查询状态
	DWORD dwInfoSize = 32;
	TCHAR szStatusCode[32] = {0};
	if (!::HttpQueryInfo(m_hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		_snprintf(m_strStatusInfor, MAX_PATH, "Failed in call to HttpQueryInfo for HTTP query status code, Error:%d\n", ::GetLastError());
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		SetStatus(FINISH_ERROR_QUERYINFO);
		return;
	}
	else
	{
		long nStatusCode = atoi(szStatusCode);
		
		if(nStatusCode != HTTP_STATUS_OK)
		{
			m_theState = FINISH_ERROR_INVALID_HTTP_RESPONSE;

			if (nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ)
			{
				m_theState = FINISH_ERROR_NEED_PROXY_AUTH_REQ;
				_snprintf(m_strStatusInfor, MAX_PATH, "Authentication errors, Status Code:%d\n", nStatusCode);
				UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
				OnStatusCallback();
				return;
			}
			else if (nStatusCode == HTTP_STATUS_DENIED)
			{
				m_theState = FINISH_ERROR_DENIED;
				_snprintf(m_strStatusInfor, MAX_PATH, "Authentication errors, Status Code:%d\n", nStatusCode);
				UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
				OnStatusCallback();
				return;
			}
		}
	}
	if(CheckAbort()) return;
	AxTrace((BYTE)m_dwID, 0, "After ::HttpQueryInfo(HTTP_QUERY_STATUS_CODE)");

	// 取得文件大小   
	CHAR szContentLength[32] = {0};
	dwInfoSize = 32;
	if (::HttpQueryInfo(m_hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwInfoSize, NULL))
	{
		m_bGotFileSize = TRUE;
		m_dwFileSizeTotal = _atoi64(szContentLength);
		_snprintf(m_strStatusInfor, MAX_PATH, "FileSize: %d", m_dwFileSizeTotal);
		UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
		if ( GetID() == 0 )
			m_dwFileSizeTotal += m_nFileStartPos;
	}
	//得到文件大小，让Manager处理位置
	SetStatus(BUSY_GET_FILE_INFORMATION);
	if(CheckAbort())
		return;

	//开始下载
	DWORD dwStartTicks = ::GetTickCount();
	DWORD dwBytesRead = 0;
	char szReadBuf[DOWNANT_MINSIZE] = {0};
	DWORD dwBytesToRead = DOWNANT_MINSIZE;
	m_dwFileSizeRead = 0;

	SetStatus(BUSY_BEGIN_DOWNFILE);
	do
	{
		if (!::InternetReadFile(m_hHttpFile, szReadBuf, dwBytesToRead, &dwBytesRead))
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "(::InternetReadFile), Error:%d\n", ::GetLastError());
			UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
			SetStatus(FINISH_ERROR_READFILE);
			return;
		}
		else if (dwBytesRead && m_dwUserCmd == MANAGER_CMD_CONTINUE)
		{
			try
			{
				::fwrite(szReadBuf, dwBytesRead, 1, m_fpFileToWrite);
				char szBuf[1024];
				sprintf( szBuf, "向下载的分片文件%s中写入%d个字节数据.", m_strFileToDownloadInto,dwBytesRead );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			}
			catch(...) 
			{
				_snprintf(m_strStatusInfor, MAX_PATH, "写本地文件是发生异常, Error:%d", ::GetLastError());
				UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
				SetStatus(FINISH_ERROR_EXCEPTION);
				return;
			}
				
			m_dwFileSizeRead += dwBytesRead;
			if ( m_dwFileSizeRead % SAVE_MINSIZE == 0 )
			{
				//每SAVE_MINSIZE字节存一次盘
				if(m_fpFileToWrite)
					::fclose(m_fpFileToWrite);
				m_fpFileToWrite = NULL;
				
				char szBuf[1024];
				sprintf( szBuf, "关闭%s文件存一次盘,下载达到%d字节.",m_strFileToDownloadInto,m_dwFileSizeRead );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

				if((m_fpFileToWrite = ::fopen(m_strFileToDownloadInto, "ab")) == NULL)
				{
					_snprintf(m_strStatusInfor, MAX_PATH, "本地文件追加信息失败: %s, Error:%d", m_strFileToDownloadInto, ::GetLastError());
					UpdateSystem::GetMe()->Log( m_strStatusInfor, (DWORD)strlen( m_strStatusInfor ) );
					SetStatus(FINISH_ERROR_TRY_OPEN_LOCALFILE);
					return;
				}

			}

			OnStatusCallback();
		}

		if(CheckAbort()) 
		{
			if(m_fpFileToWrite)
				::fclose(m_fpFileToWrite);
			m_fpFileToWrite = NULL;
			return;
		}
	} 
	while (dwBytesRead && m_dwUserCmd == MANAGER_CMD_CONTINUE);

	if(m_fpFileToWrite)
		::fclose(m_fpFileToWrite);
	m_fpFileToWrite = NULL;

	SetStatus(FINISH_SUCCESS_OVER);
	return;
}

void CDownAnt::OnWininetStatusCallBack(HINTERNET /*hInternet*/, DWORD dwInternetStatus, 
                                         LPVOID lpvStatusInformation, DWORD /*dwStatusInformationLength*/)
{
	switch (dwInternetStatus)
	{
    case INTERNET_STATUS_RESOLVING_NAME:
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "%s", (LPCTSTR) lpvStatusInformation);
			SetStatus(BUSY_RESOLVING_NAME);
			break;
		}
    case INTERNET_STATUS_NAME_RESOLVED:
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "%s", (LPCTSTR) lpvStatusInformation);
			SetStatus(BUSY_RESOLVED_NAME);
			break;
		}
    case INTERNET_STATUS_CONNECTING_TO_SERVER:
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "%s", (LPCTSTR) lpvStatusInformation);
			SetStatus(BUSY_CONNECTING);
			break;
		}
    case INTERNET_STATUS_CONNECTED_TO_SERVER:
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "%s", (LPCTSTR) lpvStatusInformation);
			SetStatus(BUSY_CONNECTED);
			break;
		}
    case INTERNET_STATUS_REDIRECT:
		{
			_snprintf(m_strStatusInfor, MAX_PATH, "%s", (LPCTSTR) lpvStatusInformation);
			SetStatus(BUSY_REDIRECTING);
			break;
		}
    default:
		{
//			AxTrace((BYTE)m_dwID, 1, "UNKNOWN INET STATUS 0X%X!", dwInternetStatus);
			break;
		}
	}
}

void CDownAnt::SetStatus(CDownAnt::DOWN_STATE newState)
{
	m_theState = newState;
	OnStatusCallback();
}

void CALLBACK CDownAnt::_OnWininetStatusCallBack(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, 
                                                  LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	CDownAnt* pDownMan = (CDownAnt*)(DWORD_PTR)dwContext;
	assert(pDownMan);
	
	pDownMan->OnWininetStatusCallBack(hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}

void CDownAnt::OnStatusCallback(void)
{
	if(m_bTrace)TraceStatus();

	switch(m_theState)
	{
	case IDLE_NOTINIT:
	case IDLE_INIT_SUCCESS:
	case IDLE_INIT_FAILED:
		{
			m_theStateCategory = IDLE;
		}
		break;

	case BUSY_OPEN_REQUEST:
	case BUSY_RESOLVING_NAME:
	case BUSY_RESOLVED_NAME:
	case BUSY_CONNECTING:
	case BUSY_CONNECTED:
	case BUSY_REDIRECTING:
	case BUSY_GET_FILE_INFORMATION:
	case BUSY_BEGIN_DOWNFILE:
		{
			m_theStateCategory = BUSY;
		}
		break;

	case FINISH_SUCCESS_OVER:
	case FINISH_SUCCESS_STOPHERE:
		{
			m_theStateCategory = FINISH_SUCCESS;
		}
		break;

	case FINISH_FORCEABORT:
	case FINISH_ERROR_TIMEOUT:
	case FINISH_ERROR_TRY_OPEN_LOCALFILE:
	case FINISH_ERROR_INTERNET_OPEN:
	case FINISH_ERROR_SETSTATUSCALLBACK:
	case FINISH_ERROR_INTERNETCONNECT:
	case FINISH_ERROR_OPENREQUEST:
	case FINISH_ERROR_SENDREQUEST:
	case FINISH_ERROR_QUERYINFO:
	case FINISH_ERROR_NEED_PROXY_AUTH_REQ:
	case FINISH_ERROR_DENIED:
	case FINISH_ERROR_INVALID_HTTP_RESPONSE:
	case FINISH_ERROR_READFILE:
	case FINISH_ERROR_EXCEPTION:
		{
			m_theStateCategory = FINISH_NOT_SUCCESS;
			AxTrace((BYTE)m_dwID, 0, m_strStatusInfor);
		}
		break;

	default:
		assert(false);
		break;
	}

	if(m_theStateCategory == FINISH_SUCCESS || m_theStateCategory == FINISH_NOT_SUCCESS)
	{
		if(m_fpFileToWrite)
		{
			::fclose(m_fpFileToWrite);
			m_fpFileToWrite = NULL;
		}

		if (m_hHttpFile)
		{
			::InternetCloseHandle(m_hHttpFile);
			m_hHttpFile = NULL;
		}
		if (m_hHttpConnection)
		{
			::InternetCloseHandle(m_hHttpConnection);
			m_hHttpConnection = NULL;
		}
		if (m_hInternetSession)
		{
			::InternetCloseHandle(m_hInternetSession);
			m_hInternetSession = NULL;
		}
	}

	if(UserCallBack)
	{
		m_dwUserCmd = UserCallBack(m_pOwner, this);
	}
}

BOOL CDownAnt::CheckAbort(void)
{
	if(m_bForceBreak || MANAGER_CMD_FORCEABORT == m_dwUserCmd)
	{
		SetStatus(FINISH_FORCEABORT);
		return TRUE;
	}
	if(MANAGER_CMD_STOPHERE == m_dwUserCmd)
	{
		SetStatus(FINISH_SUCCESS_STOPHERE);
		return TRUE;
	}

	if(m_nOverTime != INFINITE && m_theStateCategory == BUSY)
	{
		DWORD dwTimeNow = ::GetTickCount();
		DWORD dwTimeRun;
		if(dwTimeNow >= m_dwBeginTime) dwTimeRun = dwTimeNow-m_dwBeginTime;
		else dwTimeRun = dwTimeNow + (0XFFFFFFFF-m_dwBeginTime);

		if(dwTimeRun > m_nOverTime*1000)
		{
			SetStatus(FINISH_ERROR_TIMEOUT);
			return TRUE;
		}
	}
	
	return FALSE;
}

void CDownAnt::TraceStatus(void)
{
#ifdef _DEBUG
	CHAR szTemp[MAX_PATH] = {0};

	switch(m_theState)
	{
	case IDLE_NOTINIT:
		AxTrace((BYTE)m_dwID, 0, "IDLE_NOTINIT");
		break;
		
	case IDLE_INIT_SUCCESS:
		AxTrace((BYTE)m_dwID, 0, "IDLE_INIT_SUCCESS");
		break;
		
	case IDLE_INIT_FAILED:
		AxTrace((BYTE)m_dwID, 0, "IDLE_INIT_FAILED");
		break;
		
	case BUSY_OPEN_REQUEST:
		AxTrace((BYTE)m_dwID, 0, "BUSY_OPEN_REQUEST");
		break;

	case BUSY_RESOLVING_NAME:
		AxTrace((BYTE)m_dwID, 0, "BUSY_RESOLVING_NAME");
		AxTrace((BYTE)m_dwID, 0, m_strStatusInfor);
		break;
		
	case BUSY_RESOLVED_NAME:
		AxTrace((BYTE)m_dwID, 0, "BUSY_RESOLVED_NAME");
		AxTrace((BYTE)m_dwID, 0, m_strStatusInfor);
		break;
		
	case BUSY_CONNECTING:
		AxTrace((BYTE)m_dwID, 0, "BUSY_CONNECTING");
		AxTrace((BYTE)m_dwID, 0, m_strStatusInfor);
		break;
		
	case BUSY_CONNECTED:
		AxTrace((BYTE)m_dwID, 0, "BUSY_CONNECTED");
		AxTrace((BYTE)m_dwID, 0, m_strStatusInfor);
		break;
		
	case BUSY_REDIRECTING:
		AxTrace((BYTE)m_dwID, 0, "BUSY_REDIRECTING");
		AxTrace((BYTE)m_dwID, 0, m_strStatusInfor);
		break;
		
	case BUSY_GET_FILE_INFORMATION:
		_snprintf(szTemp, MAX_PATH, "BUSY_GET_FILE_INFORMATION:%d", m_dwFileSizeTotal);
		AxTrace((BYTE)m_dwID, 0, szTemp);
		break;
		
	case BUSY_BEGIN_DOWNFILE:
		_snprintf(szTemp, MAX_PATH, "BUSY_BEGIN_DOWNFILE:%d[%d]", m_dwFileSizeRead+m_nFileStartPos, m_dwFileSizeTotal);
		AxTrace((BYTE)m_dwID, 0, szTemp);
//		Sleep(20);
		break;
		
	case FINISH_FORCEABORT:
		AxTrace((BYTE)m_dwID, 0, "FINISH_FORCEABORT");
		break;
		
	case FINISH_ERROR_TIMEOUT:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_TIMEOUT");
		break;
		
	case FINISH_SUCCESS_OVER:
		_snprintf(szTemp, MAX_PATH, "FINISH_SUCCESS_OVER:%s", m_strFileToDownloadInto);
		AxTrace((BYTE)m_dwID, 0, szTemp);
		break;

	case FINISH_SUCCESS_STOPHERE:
		AxTrace((BYTE)m_dwID, 0, "FINISH_SUCCESS_STOPHERE");
		break;
		
	case FINISH_ERROR_TRY_OPEN_LOCALFILE:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_TRY_OPEN_LOCALFILE");
		break;
		
	case FINISH_ERROR_INTERNET_OPEN:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_INTERNET_OPEN");
		break;
		
	case FINISH_ERROR_SETSTATUSCALLBACK:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_SETSTATUSCALLBACK");
		break;
		
	case FINISH_ERROR_INTERNETCONNECT:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_INTERNETCONNECT");
		break;
		
	case FINISH_ERROR_OPENREQUEST:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_OPENREQUEST");
		break;
		
	case FINISH_ERROR_SENDREQUEST:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_SENDREQUEST");
		break;
		
	case FINISH_ERROR_QUERYINFO:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_QUERYINFO");
		break;
		
	case FINISH_ERROR_NEED_PROXY_AUTH_REQ:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_NEED_PROXY_AUTH_REQ");
		break;
		
	case FINISH_ERROR_DENIED:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_DENIED");
		break;
		
	case FINISH_ERROR_INVALID_HTTP_RESPONSE:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_INVALID_HTTP_RESPONSE");
		break;
		
	case FINISH_ERROR_READFILE:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_READFILE");
		break;
		
	case FINISH_ERROR_EXCEPTION:
		AxTrace((BYTE)m_dwID, 0, "FINISH_ERROR_EXCEPTION");
		break;

	default:
		AxTrace((BYTE)m_dwID, 0, "UNKNOWN! %d", m_theState);
		break;
	}
#endif
}
//--------------------------------------------------
// 地址分析
// Code base on MFC (C)Microsoft

BOOL __stdcall MyAfxIsValidAddress(const void* lp, UINT nBytes,
	BOOL bReadWrite /* = TRUE */)
{
	// simple version using Win-32 APIs for pointer validation.
	return (lp != NULL && !IsBadReadPtr(lp, nBytes) &&
		(!bReadWrite || !IsBadWritePtr((LPVOID)lp, nBytes)));
}

static BOOL __stdcall _AfxParseURLWorker(LPCTSTR pstrURL,
	LPURL_COMPONENTS lpComponents, DWORD& dwServiceType,
	INTERNET_PORT& nPort, DWORD dwFlags)
{
	// this function will return bogus stuff if lpComponents
	// isn't set up to copy the components

	assert(lpComponents != NULL && pstrURL != NULL);
	if (lpComponents == NULL || pstrURL == NULL)
		return FALSE;
	assert(lpComponents->dwHostNameLength == 0 ||
			lpComponents->lpszHostName != NULL);
	assert(lpComponents->dwUrlPathLength == 0 ||
			lpComponents->lpszUrlPath != NULL);
	assert(lpComponents->dwUserNameLength == 0 ||
			lpComponents->lpszUserName != NULL);
	assert(lpComponents->dwPasswordLength == 0 ||
			lpComponents->lpszPassword != NULL);

	assert(MyAfxIsValidAddress(lpComponents, sizeof(URL_COMPONENTS), TRUE));

	LPTSTR pstrCanonicalizedURL;
	TCHAR szCanonicalizedURL[INTERNET_MAX_URL_LENGTH];
	DWORD dwNeededLength = INTERNET_MAX_URL_LENGTH;
	BOOL bRetVal;
	BOOL bMustFree = FALSE;
	DWORD dwCanonicalizeFlags = dwFlags &
		(ICU_NO_ENCODE | ICU_DECODE | ICU_NO_META |
		ICU_ENCODE_SPACES_ONLY | ICU_BROWSER_MODE);
	DWORD dwCrackFlags = dwFlags & (ICU_ESCAPE | ICU_USERNAME);

	bRetVal = InternetCanonicalizeUrl(pstrURL, szCanonicalizedURL,
		&dwNeededLength, dwCanonicalizeFlags);

	if (!bRetVal)
	{
		if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			return FALSE;

		pstrCanonicalizedURL = new TCHAR[dwNeededLength];
		bMustFree = TRUE;
		bRetVal = InternetCanonicalizeUrl(pstrURL, pstrCanonicalizedURL,
			&dwNeededLength, dwCanonicalizeFlags);
		if (!bRetVal)
		{
			delete [] pstrCanonicalizedURL;
			return FALSE;
		}
	}
	else
		pstrCanonicalizedURL = szCanonicalizedURL;

	// now that it's safely canonicalized, crack it

	bRetVal = InternetCrackUrl(pstrCanonicalizedURL, 0,
						dwCrackFlags, lpComponents);
	if (bMustFree)
		delete [] pstrCanonicalizedURL;

	if (!bRetVal)
		dwServiceType = INTERNET_SCHEME_UNKNOWN;
	else
	{
		nPort = lpComponents->nPort;
		switch (lpComponents->nScheme)
		{
		case INTERNET_SCHEME_FTP:
		case INTERNET_SCHEME_GOPHER:
		case INTERNET_SCHEME_HTTP:
		case INTERNET_SCHEME_HTTPS:
		case INTERNET_SCHEME_FILE:
		case INTERNET_SCHEME_NEWS:
		case INTERNET_SCHEME_MAILTO:
			dwServiceType = lpComponents->nScheme;
			break;

		default:
			dwServiceType = INTERNET_SCHEME_UNKNOWN;
		}
	}

	return bRetVal;
}

BOOL __stdcall AfxParseURL(LPCTSTR pstrURL, DWORD& dwServiceType,
	LPTSTR strServer, LPTSTR strObject, INTERNET_PORT& nPort)
{
	dwServiceType = INTERNET_SCHEME_UNKNOWN;

	assert(pstrURL != NULL);
	if (pstrURL == NULL)
		return FALSE;

	URL_COMPONENTS urlComponents;
	memset(&urlComponents, 0, sizeof(URL_COMPONENTS));
	urlComponents.dwStructSize = sizeof(URL_COMPONENTS);

	urlComponents.dwHostNameLength = MAX_PATH;
	urlComponents.lpszHostName = strServer;
	urlComponents.dwUrlPathLength = INTERNET_MAX_URL_LENGTH;
	urlComponents.lpszUrlPath = strObject;

	BOOL bRetVal = _AfxParseURLWorker(pstrURL, &urlComponents,
					dwServiceType, nPort, ICU_BROWSER_MODE);

	return bRetVal;
}

}
