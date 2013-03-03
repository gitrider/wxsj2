
#include "StdAfx.h"
#include "AutoDownload_Ant.h"
#include "AutoDownload_Impl.h"
#include "DownAnt/DownManager.h"

#include <assert.h>

/**********************************************************
	构造
***********************************************************/
CAutoDownload_DownAnt::CAutoDownload_DownAnt() :
	CAutoDownload()
{
	m_pDownManager = new DOWNANT::CDownManager;
}

/**********************************************************
	析构
***********************************************************/
CAutoDownload_DownAnt::~CAutoDownload_DownAnt()
{
	delete m_pDownManager; m_pDownManager = 0; 
}


/**********************************************************
	取得当前状态
***********************************************************/
CAutoDownload::AUTODOWN_STATE CAutoDownload_DownAnt::getState(void) const
{
	using namespace DOWNANT;
	switch(m_pDownManager->GetState())
	{
	case CDownManager::IDLE_NOTINIT:		
		return AS_IDLE_NOTINIT;
		break;
	case CDownManager::IDLE_INIT_SUCCESS:	
		return AS_IDLE_INIT_SUCCESS;
		break;
	case CDownManager::IDLE_INIT_FAILED:	
		return AS_IDLE_INIT_FAILED;		
		break;

	case CDownManager::BUSY_QUERY:
	case CDownManager::BUSY_FIRST_BEGIN:
		return AS_BUSY_QUERY;
		break;
	case CDownManager::BUSY_UNION_BEGIN:	
		return AS_BUSY_UNIONBEGIN;		
		break;

	case CDownManager::FINISH_SUCCESS_ALLOVER:			
		return AS_FINISH_SUCCESS;		
		break;
	case CDownManager::FINISH_NOT_SUCCESS_UNIONFILE:	
		return AS_FINISH_FAILED_LOCAL;
		break;

	default:
		return AS_IDLE_NOTINIT;
	}
}

/**********************************************************
	设置下载任务[异步]
***********************************************************/
bool CAutoDownload_DownAnt::init(const char* szDownURL, const char* szFileName, HWND hWnd, int nOwnerID)
{
	assert(szDownURL && szFileName);

	//确定工作状态处于Idle
	if(AC_IDLE != getStateCategory()) return false;

	//保存任务信息
	m_pDownImpl->m_strURL = szDownURL;
	m_pDownImpl->m_strLocalFile = szFileName;
	m_pDownImpl->m_hCallbackWnd = hWnd;
	m_pDownImpl->m_nOwnerID = nOwnerID;

	m_pDownManager->Init(m_pDownImpl->m_strURL.c_str(), m_pDownImpl->m_strLocalFile.c_str(), _downManStatusCallback, this);

	setState(getState());
	return true;
}

/**********************************************************
	设置下载任务[异步]
***********************************************************/
void CAutoDownload_DownAnt::reset(void)
{
	try 
	{
		//停止工作
		stop();
		//清空
		delete m_pDownManager;
	}catch(...) {}

	//重新创建
	m_pDownManager = new DOWNANT::CDownManager();
	setState(getState());
}

/*******************************************************
		开始工作[异步]
********************************************************/
void CAutoDownload_DownAnt::begin(void)
{
	//确定状态
	if(AS_IDLE_INIT_SUCCESS != getState()) return;

	m_pDownManager->Begin();

	setState(getState());
}

/*******************************************************
		强制停止工作[异步]
********************************************************/
void CAutoDownload_DownAnt::stop(void)
{
	if(AC_BUSY != getStateCategory()) return;

	//下载中...
	m_pDownManager->ShutDown(FALSE);
}

/*******************************************************
		等待工作结束(normal)
********************************************************/
bool CAutoDownload_DownAnt::waitStop(bool bBlock)
{
	return m_pDownManager->WaitOver(bBlock)==TRUE;
}

/*******************************************************
		下载工作(normal)回调函数
********************************************************/
void WINAPI CAutoDownload_DownAnt::_downManStatusCallback(VOID* pOwner, DWORD dwCode, WPARAM wParam, LPARAM lParam)
{
	using namespace DOWNANT;

	CAutoDownload_DownAnt* pThis = (CAutoDownload_DownAnt*)pOwner;
	CAutoDownload::AUTODOWN_IMPL* pImpl = pThis->m_pDownImpl;

	switch(dwCode)
	{
		//状态改变
		case DMCC_STATUS_CHANGE:
			pThis->setState(pThis->getState());
			break;

		//节点下载
		case DMCC_SIZEUPDATE:
			{
				if(pImpl->m_hCallbackWnd)
				{
					::SendMessage(pImpl->m_hCallbackWnd, CAutoDownload::AM_SIZEUPDATE,pImpl->m_nOwnerID, 0);
				}
			}
			break;
	}
}


/*******************************************************
		取得当前正在下载的文件的大小
********************************************************/
bool CAutoDownload_DownAnt::getFileSize(__int64& nFileSize, __int64& nReadSize) const
{
	//确定工作状态
	if(AC_IDLE == getStateCategory()) return false;

	m_pDownManager->GetFileSize(nFileSize, nReadSize);

	return (nFileSize != 0);
}

/*******************************************************
		取得当前下载速度相关信息
********************************************************/
int CAutoDownload_DownAnt::getSpeedInfo(int* pTimeSpend, int* pTimeRemain) const
{
	return m_pDownManager->GetSpeed();
}

