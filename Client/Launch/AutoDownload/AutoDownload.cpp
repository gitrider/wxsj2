
#include "StdAfx.h"
#include "AutoDownload.h"
#include "AutoDownload_Impl.h"

#include "AutoDownload_Ant.h"
#include "DownAnt/DownAnt.h"

CAutoDownload* CAutoDownload::createInstance(AUTODOWN_TYPE type)
{
	if(AT_ANT == type) 
	{
		return new CAutoDownload_DownAnt;
	}

	return 0;
}

/**********************************************************
	销毁实例
***********************************************************/
void CAutoDownload::destroyInsance(CAutoDownload* pDown)
{
	if(!pDown) return;

	delete pDown;
}

/**********************************************************
	构造
***********************************************************/
CAutoDownload::CAutoDownload()
{
	m_pDownImpl = new AUTODOWN_IMPL;
	
	m_pDownImpl->m_hCallbackWnd = 0;
	m_pDownImpl->m_state = AS_IDLE_NOTINIT;
	m_pDownImpl->m_nOwnerID = 0;
}

/**********************************************************
	析构
***********************************************************/
CAutoDownload::~CAutoDownload()
{
	delete m_pDownImpl; m_pDownImpl = 0;
}

/*******************************************************
		设置当前状态
********************************************************/
void CAutoDownload::setState(AUTODOWN_STATE state)
{
	CAutoDownload::AUTODOWN_IMPL* pImpl = getImpl();

	if(pImpl->m_hCallbackWnd && pImpl->m_state != state)
	{
		pImpl->m_state = state;
		::SendMessage(pImpl->m_hCallbackWnd, CAutoDownload::AM_STATUSUPDATE, pImpl->m_nOwnerID, getState());
	}
	else
	{
		pImpl->m_state = state;
	}
}

/*******************************************************
		取得当前正在下载的文件的文件名
********************************************************/
std::string CAutoDownload::getUrlFileName(void) const
{
	//确定工作状态处于Idle
	if(AS_IDLE_NOTINIT == getState()) return "";

	//分析URl
	CHAR strServer[MAX_PATH];
	CHAR strObject[INTERNET_MAX_URL_LENGTH];
	DWORD dwService;
	INTERNET_PORT nPort;
	
	DOWNANT::AfxParseURL(getConstImpl()->m_strURL.c_str(), dwService, strServer, strObject, nPort);
	
	std::string strTemp = strObject;
	return strTemp.substr(strTemp.find_last_of("\\/")+1);
}

/*******************************************************
		工具函数，将文件大小转化为字符串描述
********************************************************/
std::string CAutoDownload::fileSize2String(__int64 nSize)
{
	char szTemp[64] = {0};
	if(nSize < 1024)
	{
		_snprintf(szTemp, 64, "%d", nSize);
	}
	else if(nSize < 1024*1024)
	{
		_snprintf(szTemp, 64, "%.1fK", (double)nSize/1024.0);
	}
	else if(nSize < 1024L*1024L*1024L)
	{
		_snprintf(szTemp, 64, "%.1fM", (double)nSize/(1024.0*1024.0));
	}
	else if((double)nSize < 1024.0*1024.0*1024.0*1024.0)
	{
		_snprintf(szTemp, 64, "%.1fG", (double)nSize/(1024.0*1024.0*1024.0));
	}
	else
	{
		_snprintf(szTemp, 64, "%.1fT", (double)nSize/(1024.0*1024.0*1024.0*1024.0));
	}

	return std::string(szTemp);
}
