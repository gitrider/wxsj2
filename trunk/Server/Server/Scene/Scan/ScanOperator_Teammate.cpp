
#include "stdafx.h"
#include "ScanOperator_Teammate.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "AI_Monster.h"


ScanOperator_Teammate::ScanOperator_Teammate( )
{
}

ScanOperator_Teammate::~ScanOperator_Teammate( )
{
}

BOOL ScanOperator_Teammate::Init( SCANOPERATOR_TEAMMATE_INIT* pInit )
{
__ENTER_FUNCTION

	if( pInit==NULL )
	{
		Assert( FALSE ) ;
		return FALSE ;
	}

	m_pMonster = pInit->m_pMonster ;
	m_fRadius = pInit->m_fRadius ;
	m_bOnlyNoEnemy = pInit->m_bOnlyNoEnemy;
	m_bScanAllMonster = pInit->m_bScanAllMonster;

	return 	ScanOperator::Init( pInit ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

UINT ScanOperator_Teammate::On_FindObj( Obj* pObj )
{
__ENTER_FUNCTION

	if( pObj==NULL )
	{
		Assert( FALSE ) ;
		return SCANRETURN_RETURN ;
	}

	if( !IsCharacterObj( pObj->GetObjType() ) )
	{
		return SCANRETURN_CONTINUE;
	}
	Obj_Character* pCharacter = (Obj_Character*)pObj ;
	// ���ŵģ����ǵжԵ�
	if ( pCharacter->IsAlive()						
		&& !m_pMonster->IsEnemy( pCharacter ) 
		&& ((Obj_Monster*)pCharacter)->GetMonsterAI()->HasEnemy() != m_bOnlyNoEnemy )
	{
		if (!m_bScanAllMonster)
		{// �������ɨ���������͵Ĺ�����ֻɨ�����������Ĺ֣�
			if (((Obj_Monster*)pCharacter)->GetMonsterAI()->AIParam(AIPARAM_SCANTIME) < 0)
			{
				return SCANRETURN_CONTINUE;
			}
		}

		if ( pCharacter->GetObjType() != Obj::OBJ_TYPE_MONSTER )
			return SCANRETURN_CONTINUE;
		if ( m_pMonster->GetID() == ((Obj_Monster*)pCharacter)->GetID() )
			return SCANRETURN_CONTINUE;

		//�����λ��֮��ľ���
		FLOAT fDist = MySqrt(m_pMonster->getWorldPos(), pCharacter->getWorldPos() ) ;
		if( fDist<m_fRadius )
		{	
			INT nCount = m_pMonster->GetMonsterAI()->GetTeammateCount();
			if ( nCount >= m_nCount )
				return SCANRETURN_RETURN ;
			//�����λ��֮��ľ���С���ض�ֵʱ�ż�����Ѷ���
			m_pMonster->GetMonsterAI()->AddTeammate( pCharacter->GetID() ) ;
		}
	}

__LEAVE_FUNCTION

	return SCANRETURN_CONTINUE ;
}
