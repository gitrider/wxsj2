/*
 *	���ݿ��̹߳�����
 */

#include "stdafx.h" 
#include "DBThreadManager.h"
#include "ODBCInterface.h"
#include "LoginMain.h"

DBThreadManager*	g_pDBThreadManager = NULL;




DBThreadManager::DBThreadManager()
{
	__ENTER_FUNCTION
	
		mDBThreadCount	= DB_CONNECTION_COUNT;						//��ǰ���õ����ݿ�������
		memset(mpDBThread,0,sizeof(DBThread*)*MAX_DB_THREAD_POOL);	//�������ݿ�������
		for(UINT i=0;i<mDBThreadCount;i++)
		{
			mpDBThread[i]		= new DBThread;
		}
		

	__LEAVE_FUNCTION
}

DBThreadManager::~DBThreadManager()
{	
	__ENTER_FUNCTION
	
		for(UINT i=0;i<mDBThreadCount;i++)
		{
			SAFE_DELETE(mpDBThread[i]);
		}

	__LEAVE_FUNCTION
}

BOOL	DBThreadManager::Init()
{
	__ENTER_FUNCTION	
		if(g_SingleDBConnection == 1)	mDBThreadCount = 3;
		for(UINT i =0;i<mDBThreadCount;i++)
		{
			if(mpDBThread[i])	
			{
				BOOL bRet = mpDBThread[i]->Init();
				
			}
		}
		return TRUE;

		__LEAVE_FUNCTION	
		return FALSE;
}


BOOL	DBThreadManager::Start()
{
	__ENTER_FUNCTION
		
		for(UINT i =0;i<mDBThreadCount;i++)
		{
			if(mpDBThread[i])
				mpDBThread[i]->start();
			
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	DBThreadManager::Stop()
{
	__ENTER_FUNCTION
		
		for(UINT i =0;i<mDBThreadCount;i++)
		{
			if(mpDBThread[i])
			{
				mpDBThread[i]->stop();
			}
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}
BOOL	DBThreadManager::IsPoolTID(TID tid)
{
	__ENTER_FUNCTION
		
		for(UINT i =0;i<mDBThreadCount;i++)
		{
			if(mpDBThread[i])
			{
				if(tid == mpDBThread[i]->getTID())
					return TRUE;
			}
		}

		return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	DBThreadManager::SendPacket(Packet* pPacket,PlayerID_t pID)
{
	__ENTER_FUNCTION

		
		for(UINT i =0;i<mDBThreadCount;i++)
		{
				if(!mpDBThread[i])	
					continue;	
				if(!mpDBThread[i]->IsActive())
					continue;
				if(mpDBThread[i]->SendPacket(pPacket,pID))
				{
					return TRUE;
				}
			
		}

		return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}

ODBCInterface*	DBThreadManager::GetInterface(TID tid)
{
	__ENTER_FUNCTION


		for(UINT i =0;i<mDBThreadCount;i++)
		{
			if(!mpDBThread[i])	
				continue;	
			if(!mpDBThread[i]->IsActive())
				continue;
			if(tid == mpDBThread[i]->getTID())
				return mpDBThread[i]->GetInterface();

		}

		return NULL;

		__LEAVE_FUNCTION

		return NULL;
}