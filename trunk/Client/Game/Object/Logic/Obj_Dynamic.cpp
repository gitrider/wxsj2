
#include "StdAfx.h"
#include <Math.h>
#include "Obj_Dynamic.h"
#include "..\Manager\ObjectManager.h"
#include "Character\Obj_PlayerMySelf.h"
#include "GIUtil.h"
#include "Global.h"
#include "World\Path.h"



KL_IMPLEMENT_DYNAMIC(CObject_Dynamic, GETCLASS(CObject_Map));



CObject_Dynamic::CObject_Dynamic()
{
	m_bServerPath	= FALSE;
	m_dwPathID		= UINT_MAX;

	m_posPathNodePrev		= fVector3( 0.f, 0.f, 0.f );
}

CObject_Dynamic::~CObject_Dynamic()
{
}


const fVector3 *CObject_Dynamic::GetBeginPathNode( VOID )const
{
	if ( m_listPathNode.empty() )
		return NULL;

	return &(*m_listPathNode.begin());
}

VOID CObject_Dynamic::AddPathNode( BOOL bServerPath, UINT dwPathID, const fVector3 *pfvNode )
{
	if ( m_listPathNode.empty() )
	{
		m_posPathNodePrev = GetPosition();
		m_bServerPath	= bServerPath;
	}
	else if ( !m_bServerPath && bServerPath )
	{
		return ;
	}
	else if ( m_bServerPath && !bServerPath )
	{
		m_bServerPath	= bServerPath;
		CleanupPathNode();
		m_posPathNodePrev = GetPosition();
	}
	else
	{
		if ( dwPathID != m_dwPathID )
		{
			CleanupPathNode();
			m_posPathNodePrev = GetPosition();
		}
	}
	m_dwPathID = dwPathID;
	m_listPathNode.push_back( *pfvNode );

}

VOID CObject_Dynamic::CleanupPathNode( VOID )
{
	m_listPathNode.erase( m_listPathNode.begin(), m_listPathNode.end() );
}

BOOL CObject_Dynamic::BeginPathNode( VOID )
{
	if ( m_listPathNode.empty() )
		return FALSE;

	CalcMoveDir();
	return TRUE;
}

BOOL CObject_Dynamic::NextPathNode( VOID )
{
	if ( m_listPathNode.empty() )
		return FALSE;

	m_posPathNodePrev = *(m_listPathNode.begin());
	m_listPathNode.pop_front();

	if ( m_listPathNode.empty() )
		return FALSE;

	CalcMoveDir();
	OnNextPathNode();
	return TRUE;
}

BOOL CObject_Dynamic::CalcMoveDir( VOID )
{
	if ( m_listPathNode.empty() )
		return FALSE;

	fVector3 fvNextPos;
	fVector2 fvNextPos2D;
	fvNextPos		= (*m_listPathNode.begin());
	fvNextPos2D.x	= fvNextPos.x;
	fvNextPos2D.y	= fvNextPos.z;

	FLOAT fYAngle = KLU_GetYAngle(
		fVector2(GetPosition().x, GetPosition().z), 
		fvNextPos2D);

	SetFaceDir( fYAngle );
	return TRUE;
}

BOOL CObject_Dynamic::DoMove( FLOAT fElapseTime, FLOAT fSpeed )
{
	const fVector3* pRrePos = GetPrevPathNode();
	if ( m_listPathNode.empty() )
		return FALSE;

	// ��ǰλ��
	fVector3 fvCurPos = GetPosition();
	// ��ǰ��Ŀ��·����
	fVector3 fvBeginPathNode = (*m_listPathNode.begin());

	// ��ǰλ���뵱ǰ��Ŀ��·����ľ���
	fVector3 fvDistToBeginPathNode = fvBeginPathNode - fvCurPos;
	FLOAT fDir = GetFaceDir();

	// ��һ֡���ƶ���·������
	FLOAT fMoveDist = fSpeed * fElapseTime;
	FLOAT fMoveDistSq = fMoveDist * fMoveDist;

	// ��ǰλ���뵱ǰ��Ŀ��·����·�����ȵ�ƽ��
	FLOAT fDistToBeginPathNodeSq = fvDistToBeginPathNode.x * fvDistToBeginPathNode.x + fvDistToBeginPathNode.z * fvDistToBeginPathNode.z;

	// �����һ֡���ƶ���·������С�ڵ�ǰλ�õ���ǰ��Ŀ��·����·������
	if ( fDistToBeginPathNodeSq > fMoveDistSq )
	{
		FLOAT fTarDist = sqrt(fDistToBeginPathNodeSq) ;
		FLOAT fDistX = (fMoveDist*(fvBeginPathNode.x-fvCurPos.x))/fTarDist ;
		FLOAT fDistZ = (fMoveDist*(fvBeginPathNode.z-fvCurPos.z))/fTarDist ;
		
		
		fVector2 fvPos;
		fvPos.x = fvCurPos.x + fDistX;
		fvPos.y = fvCurPos.z + fDistZ;
		
		//��Ҫλ����������ֹ��Ϊ��������赲���ڲ�
		if( CObjectManager::GetMe()->GetMySelf() == this && CPath::IsPointInUnreachRegion(fvPos))
		{
			fvPos = KLU_GetReflect(
						fVector2(pRrePos->x, pRrePos->z),
						fVector2(fvBeginPathNode.x, fvBeginPathNode.z),
						fvPos);//

		}

		SetMapPosition( fvPos );
	}
	else
	{
		fVector2 fvPos;
		// Ŀ��·�������һ��
		if ( !NextPathNode() )
		{
			fvPos.x = fvBeginPathNode.x;
			fvPos.y = fvBeginPathNode.z;
			//��Ҫλ����������ֹ��Ϊ��������赲���ڲ�
			if( CObjectManager::GetMe()->GetMySelf() == this && CPath::IsPointInUnreachRegion(fvPos))
			{
				//����·�ߵľ���㣬 ��pRrePos �� fvBeginPathNode������ͬһ����
				fvPos = KLU_GetReflect(
							fVector2(pRrePos->x, pRrePos->z),
							fVector2(fvBeginPathNode.x, fvBeginPathNode.z),
							fvPos);//
			}

			SetMapPosition( fvPos );
			OnMoveStop( );
		}
		else
		{
			FLOAT fDistToBeginPathNode = sqrt( fDistToBeginPathNodeSq );
			fMoveDist -= fDistToBeginPathNode;

			fvPos.x = fvBeginPathNode.x;
			fvPos.y = fvBeginPathNode.z;
			SetMapPosition( fvPos );
			CalcMoveDir();
			fDir = GetFaceDir();

			FLOAT fDistX = sin( fDir ) * fMoveDist;
			FLOAT fDistZ = cos( fDir ) * fMoveDist;
			fvCurPos = fvBeginPathNode;
			fvPos.x = fvCurPos.x + fDistX;
			fvPos.y = fvCurPos.z + fDistZ;
			
			//��Ҫλ����������ֹ��Ϊ��������赲���ڲ�
			if( CObjectManager::GetMe()->GetMySelf() == this && CPath::IsPointInUnreachRegion(fvPos))
			{
				// �õ��µ�ӳ������
				pRrePos = GetPrevPathNode();
				fvBeginPathNode = (*m_listPathNode.begin());

				//����·�ߵľ���㣬 ��pRrePos �� fvBeginPathNode������ͬһ����
				fvPos = KLU_GetReflect(
							fVector2(pRrePos->x, pRrePos->z),
							fVector2(fvBeginPathNode.x, fvBeginPathNode.z),
							fvPos);//

			}//
			
			SetMapPosition( fvPos );
		}

//		RegisterToZone();
	}
	return TRUE;
}

VOID CObject_Dynamic::OnNextPathNode( VOID )
{
}

VOID CObject_Dynamic::OnMoveStop( VOID )
{
}
