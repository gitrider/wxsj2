
#include "stdafx.h"
#include "ScanOperator_Enemy.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "AI_Monster.h"


ScanOperator_Enemy::ScanOperator_Enemy( )
{
}

ScanOperator_Enemy::~ScanOperator_Enemy( )
{
}

BOOL ScanOperator_Enemy::Init( SCANOPERATOR_ENEMY_INIT* pInit )
{
__ENTER_FUNCTION
	
	if( pInit==NULL )
	{
		Assert( FALSE ) ;
		return FALSE ;
	}

	m_pMonster = pInit->m_pMonster ;
	m_fRadius = pInit->m_fRadius ;

	return 	ScanOperator::Init( pInit ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

UINT ScanOperator_Enemy::On_FindObj( Obj* pObj )
{
__ENTER_FUNCTION

	if( pObj==NULL )
	{
		Assert( FALSE ) ;
		return SCANRETURN_CONTINUE ;
	}

	if( !IsCharacterObj(pObj->GetObjType()) )
	{
		return SCANRETURN_CONTINUE ;
	}

	Obj_Character* pCharacter = (Obj_Character*)pObj ;
	if( !m_pMonster->IsEnemy( pCharacter ) )
	{
		return SCANRETURN_CONTINUE ;
	}

	BOOL bAlive = pCharacter->IsAlive();
	BOOL bEnemy = m_pMonster->IsEnemy(pCharacter);
	BOOL bCanViewMe = pCharacter->IsCanViewMe(m_pMonster);
	if(bAlive && bEnemy && bCanViewMe)
	{
		//�����λ��֮��ľ���
		FLOAT fDist = MySqrt(m_pMonster->getWorldPos(), pCharacter->getWorldPos() ) ;
		if( fDist<m_fRadius )
		{	
			//�����λ��֮��ľ���С���ض�ֵʱ�ż�����˶���
			m_pMonster->GetMonsterAI()->AddPrimaryEnemy( pCharacter->GetID() ) ;
			return SCANRETURN_RETURN;
		}
	}

__LEAVE_FUNCTION

	return SCANRETURN_CONTINUE ;
}




