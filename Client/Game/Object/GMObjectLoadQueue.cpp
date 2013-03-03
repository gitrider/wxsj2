
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

	// 玩家自己不放入队列
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

	// 玩家自己位置
	fVector3 fvPosMySelf = CObjectManager::GetMe()->GetMySelf()->GetPosition();

	// 根据距离排序加载
	float fDistanceSq = KLU_GetDistSq( pObj->GetPosition(), fvPosMySelf );
	m_mapLoadTask.insert( std::make_pair( fDistanceSq, idObj ) );

	return TRUE;
}

void CObjectLoadQueue::Tick(void)
{
	// 每桢处理的个数
	const INT WORK_SPEED = 2;
	if( !m_mapLoadTask.empty() )
	{
		int nDoRealWork = 0;
		do
		{
			// 存在需要加载的OBJ
			INT idObj = m_mapLoadTask.begin()->second;

			CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(idObj);
			if( pObj )
			{
				// 处理消息
				CGCharAskBaseAttrib msgAskBaseAttrib;
				msgAskBaseAttrib.setTargetID(pObj->GetServerID());
				CNetManager::GetMe()->SendPacket(&msgAskBaseAttrib);

				nDoRealWork++;
			}
			else
			{
				// 该Object已经被删除
				int a = 0;
			}

			// 从队列中删除
			m_mapLoadTask.erase(m_mapLoadTask.begin());

			// 做过真实工作，退出
			if( nDoRealWork>=WORK_SPEED || m_mapLoadTask.empty() ) 
				break;

		} while(true);
	}

	char szTemp[32];
	_snprintf(szTemp, 32, "%d", m_mapLoadTask.size());
	CGameProcedure::s_pGfxSystem->SetDebugString("ObjQueue", szTemp);
}

//清空某一个指定Object的队列
void CObjectLoadQueue::ClearTask(INT idObj)
{
	//NOT SUPPORT ANYMORE!!
	//
}

void CObjectLoadQueue::ClearAllTask(void)
{
	m_mapLoadTask.clear();
}