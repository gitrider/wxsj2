
#include "StdAfx.h"
#include "AutoDownload_Ant.h"
#include "AutoDownload_Impl.h"
#include "DownAnt/DownManager.h"

#include <assert.h>

/**********************************************************
	����
***********************************************************/
CAutoDownload_DownAnt::CAutoDownload_DownAnt() :
	CAutoDownload()
{
	m_pDownManager = new DOWNANT::CDownManager;
}

/**********************************************************
	����
***********************************************************/
CAutoDownload_DownAnt::~CAutoDownload_DownAnt()
{
	delete m_pDownManager; m_pDownManager = 0; 
}


/**********************************************************
	ȡ�õ�ǰ״̬
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
	������������[�첽]
***********************************************************/
bool CAutoDownload_DownAnt::init(const char* szDownURL, const char* szFileName, HWND hWnd, int nOwnerID)
{
	assert(szDownURL && szFileName);

	//ȷ������״̬����Idle
	if(AC_IDLE != getStateCategory()) return false;

	//����������Ϣ
	m_pDownImpl->m_strURL = szDownURL;
	m_pDownImpl->m_strLocalFile = szFileName;
	m_pDownImpl->m_hCallbackWnd = hWnd;
	m_pDownImpl->m_nOwnerID = nOwnerID;

	m_pDownManager->Init(m_pDownImpl->m_strURL.c_str(), m_pDownImpl->m_strLocalFile.c_str(), _downManStatusCallback, this);

	setState(getState());
	return true;
}

/**********************************************************
	������������[�첽]
***********************************************************/
void CAutoDownload_DownAnt::reset(void)
{
	try 
	{
		//ֹͣ����
		stop();
		//���
		delete m_pDownManager;
	}catch(...) {}

	//���´���
	m_pDownManager = new DOWNANT::CDownManager();
	setState(getState());
}

/*******************************************************
		��ʼ����[�첽]
********************************************************/
void CAutoDownload_DownAnt::begin(void)
{
	//ȷ��״̬
	if(AS_IDLE_INIT_SUCCESS != getState()) return;

	m_pDownManager->Begin();

	setState(getState());
}

/*******************************************************
		ǿ��ֹͣ����[�첽]
********************************************************/
void CAutoDownload_DownAnt::stop(void)
{
	if(AC_BUSY != getStateCategory()) return;

	//������...
	m_pDownManager->ShutDown(FALSE);
}

/*******************************************************
		�ȴ���������(normal)
********************************************************/
bool CAutoDownload_DownAnt::waitStop(bool bBlock)
{
	return m_pDownManager->WaitOver(bBlock)==TRUE;
}

/*******************************************************
		���ع���(normal)�ص�����
********************************************************/
void WINAPI CAutoDownload_DownAnt::_downManStatusCallback(VOID* pOwner, DWORD dwCode, WPARAM wParam, LPARAM lParam)
{
	using namespace DOWNANT;

	CAutoDownload_DownAnt* pThis = (CAutoDownload_DownAnt*)pOwner;
	CAutoDownload::AUTODOWN_IMPL* pImpl = pThis->m_pDownImpl;

	switch(dwCode)
	{
		//״̬�ı�
		case DMCC_STATUS_CHANGE:
			pThis->setState(pThis->getState());
			break;

		//�ڵ�����
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
		ȡ�õ�ǰ�������ص��ļ��Ĵ�С
********************************************************/
bool CAutoDownload_DownAnt::getFileSize(__int64& nFileSize, __int64& nReadSize) const
{
	//ȷ������״̬
	if(AC_IDLE == getStateCategory()) return false;

	m_pDownManager->GetFileSize(nFileSize, nReadSize);

	return (nFileSize != 0);
}

/*******************************************************
		ȡ�õ�ǰ�����ٶ������Ϣ
********************************************************/
int CAutoDownload_DownAnt::getSpeedInfo(int* pTimeSpend, int* pTimeRemain) const
{
	return m_pDownManager->GetSpeed();
}

