
#include "StdAfx.h"
#include "../NetWork/NetManager.h"
#include "GMObjectLoadQueue.h"
#include "SocketOutputStream.h"
#include "SocketInputStream.h"
#include "Manager/ObjectManager.h"
#include "../Procedure/GameProcedure.h"
#include "GITimeSystem.h"
#include "GIUtil.h"
#include "GIException.h"
#include "CGCharAskBaseAttrib.h"
#include "Logic/Character/Obj_PlayerMySelf.h"



CObjectLoadQueue::CObjectLoadQueue()
{
	m_hLoadThread = NULL;
}

CObjectLoadQueue::~CObjectLoadQueue()
{
/*
	if( m_hLoadThread )
	{
		TerminateThread( m_hLoadThread, 0 );

		CloseHandle( m_hLoadThread ); 
		m_hLoadThread = NULL;
	}
*/
}


BOOL CObjectLoadQueue::TryAddLoadTask(INT idObj)
{

	// ����Լ����������
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject( idObj );
	if( !pObj || ( pObj == CObjectManager::GetMe()->GetMySelf() ) )
		return FALSE;

/*
	if( !m_hLoadThread )
	{
		UINT nThreadID;
		m_hLoadThread = (HANDLE)::_beginthreadex( NULL, 0, NULL, this, CREATE_SUSPENDED | THREAD_QUERY_INFORMATION, &nThreadID );
		if( m_hLoadThread == NULL )
		{
			KLThrow( _T("Can't create load thread!") );
		}

		ResumeThread( m_hLoadThread );
	}
*/

	// ����Լ�λ��
	fVector3 fvPosMySelf = CObjectManager::GetMe()->GetMySelf()->GetPosition();

	// ���ݾ����������
	float fDistanceSq = KLU_GetDistSq( pObj->GetPosition(), fvPosMySelf );
	m_mapLoadTask.insert( std::make_pair( fDistanceSq, idObj ) );

	return TRUE;
}

void CObjectLoadQueue::Tick(void)
{
	// ÿ�崦��ĸ���
	const INT WORK_SPEED = 2;
	if( !m_mapLoadTask.empty() )
	{
		int nDoRealWork = 0;
		do
		{
			// ������Ҫ���ص�OBJ
			INT idObj = m_mapLoadTask.begin()->second;

			CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(idObj);
			if( pObj )
			{
				// ������Ϣ
				CGCharAskBaseAttrib msgAskBaseAttrib;
				msgAskBaseAttrib.setTargetID(pObj->GetServerID());
				CNetManager::GetMe()->SendPacket(&msgAskBaseAttrib);

				nDoRealWork++;
			}
			else
			{
				// ��Object�Ѿ���ɾ��
				int a = 0;
			}

			// �Ӷ�����ɾ��
			m_mapLoadTask.erase(m_mapLoadTask.begin());

			// ������ʵ�������˳�
			if( nDoRealWork>=WORK_SPEED || m_mapLoadTask.empty() ) 
				break;

		} while(true);
	}

	char szTemp[32];
	_snprintf(szTemp, 32, "%d", m_mapLoadTask.size());
	CGameProcedure::s_pGfxSystem->SetDebugString("ObjQueue", szTemp);
}

//���ĳһ��ָ��Object�Ķ���
void CObjectLoadQueue::ClearTask(INT idObj)
{
	//NOT SUPPORT ANYMORE!!
	//
}

void CObjectLoadQueue::ClearAllTask(void)
{
	m_mapLoadTask.clear();
}