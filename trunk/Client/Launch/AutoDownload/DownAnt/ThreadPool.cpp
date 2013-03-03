// ThreadPool.cpp: implementation of the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ThreadPool.h"
#include <process.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace DOWNANT
{


CThreadPool::CThreadPool(int threadSize, int queueSize)
{
	m_bAlive = FALSE;
	m_vThreadPool.resize(threadSize);

	m_nQueueSize = queueSize;

	m_hQueueAccessSemaphone = ::CreateSemaphore(NULL, 0, queueSize, NULL);

	if(!m_hQueueAccessSemaphone)
		throw std::exception("Can't Create QueueAccessSemaphone");
	::InitializeCriticalSection(&m_csQueue);

	m_hQueueNotFullEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if(m_hQueueNotFullEvent == NULL)
		throw std::exception("Can't Create QueueNotFullEvent");
	//��ʾ���в���
	::SetEvent(m_hQueueNotFullEvent);
}

CThreadPool::~CThreadPool()
{
	if(m_hQueueNotFullEvent)
		::CloseHandle(m_hQueueNotFullEvent);
	::DeleteCriticalSection(&m_csQueue);
	if(m_hQueueAccessSemaphone)
		::CloseHandle(m_hQueueAccessSemaphone);
}

int	CThreadPool::Begin(void)
{
	if(!m_bAlive)
	{
		for(int i=0; i<(int)m_vThreadPool.size(); i++)
		{
			try
			{
				ThreadData *pData = new ThreadData;
				assert(pData);

				//�����߳�
				pData->pThreadPool = this;
				HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, _ThreadProc, pData, CREATE_SUSPENDED, &(pData->nThreadId));
				if(hThread)
					m_vThreadPool[i] = hThread;
			}
			catch(...)
			{
				assert(false);
			}
			//��ʼ�߳�
			for_each(m_vThreadPool.begin(), m_vThreadPool.end(), ::ResumeThread);
			m_bAlive = TRUE;
		}
	}

	return (int)m_vThreadPool.size();
}

unsigned int __stdcall CThreadPool::_ThreadProc(void* lpParam)
{
	ThreadData *pData = static_cast<ThreadData*>(lpParam);
	try
	{
		pData->pThreadPool->ThreadProc(pData->nThreadId);
	}
	catch(...)
	{
	}
	delete pData;				
	_endthreadex(0);
	return 0; 
}

void CThreadPool::ThreadProc(unsigned int nThreadId) throw()
{
	//��ʼ�߳�
	while(TRUE)
	{
		//ȷ��ֻ��һ���߳̽��յ�������
		::WaitForSingleObject(m_hQueueAccessSemaphone, INFINITE);
		try
		{
			//����ؼ���
			::EnterCriticalSection(&m_csQueue);
			//������
			if(!m_dJobQueue.empty())
			{
				std::auto_ptr< CThreadRequest > pJob(m_dJobQueue.front());
				m_dJobQueue.pop();
				//ȷ�϶��в���
				::SetEvent(m_hQueueNotFullEvent);
				//�뿪�ؼ���
				::LeaveCriticalSection(&m_csQueue);
				//ִ������
				try
				{
					pJob->ThreadProc(nThreadId);
				}
				catch(CThreadPool::SuicideException&) //ʹ���쳣���߳��˳�
				{
					break;
				}
			}
			else
			{
				::LeaveCriticalSection(&m_csQueue);
			}
		}
		catch(...)
		{
			MessageBox(NULL, "������쳣�����������³���!", NULL, MB_OK|MB_ICONSTOP);
			::LeaveCriticalSection(&m_csQueue);
		}

		//����������쿴�Ƿ�����һ������
	}
}

BOOL CThreadPool::PushRequest(CThreadRequest* pRequest)
{
	assert(pRequest);
	if(!pRequest) return FALSE;
	//�Ѿ��ر�
	if(!m_bAlive) return FALSE;

	//������
	if(WAIT_OBJECT_0 != ::WaitForSingleObject(m_hQueueNotFullEvent, 0))
	{
		return FALSE;
	}

	try
	{
		//����ؼ���
		::EnterCriticalSection(&m_csQueue);
		m_dJobQueue.push(pRequest);
		if((int)m_dJobQueue.size() >= m_nQueueSize)
			::ResetEvent(m_hQueueNotFullEvent);

		//�ź�����һ
		::ReleaseSemaphore(m_hQueueAccessSemaphone, 1, NULL);
		//�뿪�ؼ���
		::LeaveCriticalSection(&m_csQueue);
		return TRUE;
	}
	catch(std::exception&)
	{
		::LeaveCriticalSection(&m_csQueue);
		//�����׳�
		throw;
	}
}

void CThreadPool::ShutDown(void)
{
	//������ɱ����
	std::vector<HANDLE>::const_iterator it;
	for(it = m_vThreadPool.begin(); it != m_vThreadPool.end(); ++it)
	{
		if(!PushRequest(new CThreadPool::SuicidePill()))
		{
			//�����������˳�
			break;
		}
	}
	m_bAlive = FALSE;

	//ǿ�ƹر��߳�
	for(it = m_vThreadPool.begin() ; it != m_vThreadPool.end() ; ++it)
	{
		switch(::WaitForSingleObject(*it, 10))
		{
		case WAIT_OBJECT_0:
			// This is what we WANT to happen.
			break;

		case WAIT_TIMEOUT:			// Something bad happened - force the termination
			::TerminateThread(*it, 1);
			break;

		case WAIT_ABANDONED:	    // The thread has been terminated already!?  maybe...
			break;
		}
		::CloseHandle(*it);
	}

	//����߳�
	m_vThreadPool.clear();
}

}