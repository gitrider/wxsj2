/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   14:44
	filename: 	MapServer\Server\Ai\AI_Horse.cpp
	file path:	MapServer\Server\Ai
	file base:	AI_Horse
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "Obj_Character.h"
#include "TimeManager.h"
#include "Obj_Horse.h"
#include "Scene.h"
#include "GameTable.h"
#include "AI_Horse.h"
#include "GCPlayerRelive.h"
#include "GCManipulatePetRet.h"
#include "GCOperateResult.h"
#include "SinCos.h"

#define HORSE_DELAY_TIME (5000)
#define HORSE_MOVE_RANGE (10)
#define HORSE_MAX_TRY_TIMES (5)

AI_Horse::AI_Horse( VOID )
{
	m_RandMoveTimer.CleanUp();
	m_ScanImpactOfOwnerTimer.CleanUp();
}

AI_Horse::~AI_Horse( VOID )
{
}

BOOL AI_Horse::Init( Obj_Character *pCharacter )
{
__ENTER_FUNCTION

	BOOL bResult = AI_Character::Init( pCharacter );
	if ( !bResult )
	{
		return FALSE;
	}

	m_RandMoveTimer.CleanUp();
	m_ScanImpactOfOwnerTimer.CleanUp();

	Obj_Pet* pPet = (Obj_Pet*)m_pCharacter;
	if(NULL==m_pCharacter)
	{
		AssertEx(FALSE,"[AI_Pet::Init]: NULL m_pCharacter Found!! check it now.");
		return FALSE;
	}

	m_nDelayTime = HORSE_DELAY_TIME;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID AI_Horse::Term( VOID )
{
	AI_Character::Term( );
}

VOID AI_Horse::AI_Logic_Idle( UINT uTime )
{
__ENTER_FUNCTION

	if( !m_RandMoveTimer.IsSetTimer() )
	{
		m_RandMoveTimer.BeginTimer(HORSE_DELAY_TIME+rand()%HORSE_DELAY_TIME, uTime ) ;
	}
	if( m_RandMoveTimer.CountingTimer( uTime ) )
	{
		ToRandMove( ) ;
	}

__LEAVE_FUNCTION	
}

VOID AI_Horse::AI_Logic_Combat( UINT uTime )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID AI_Horse::AI_Logic_Dead( UINT uTime )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID	AI_Horse::ChangeMoveMode()
{
__ENTER_FUNCTION

	Obj_Horse* pHorse = GetCharacter();
	if (!pHorse)
	{
		return ;
	}
	Obj_Character* pOwner = pHorse->GetOwner();
	if (!pOwner)
	{
		return ;
	}
	FLOAT fDistSqr = CalcDistSqrOfToOwner();
	if ( fDistSqr > HORSE_REFOLLOW_DISTSQR_C ) 
	{// 距离大于15m则瞬移
		if (pHorse->CanMove()) 
		{// 非强控制下才能瞬移:如定身, 眩晕...
			FLOAT fDir = pOwner->getDir()-__PI/2;

			FLOAT fX = pOwner->getWorldPos()->m_fX - (-(2*sin(fDir)) );
			FLOAT fZ = pOwner->getWorldPos()->m_fZ - (2*cos(fDir));
			WORLD_POS pos;
			pos.m_fX = fX;
			pos.m_fZ = fZ;

			pHorse->getScene()->GetMap()->VerifyPos(&pos);
			pHorse->Teleport( &pos );
			pHorse->SetMoveMode(Obj_Horse::MOVE_MODE_WALK);
			return ;
		}
	}
	else if (fDistSqr > HORSE_REFOLLOW_DISTSQR_B) 
	{// 距离大于8m则快速跑
		pHorse->SetMoveMode(Obj_Horse::MOVE_MODE_SPRINT);
	}
	else if (fDistSqr > HORSE_REFOLLOW_DISTSQR_A)
	{// 距离大于3m则小跑
		if (pHorse->GetMoveMode() != Obj_Horse::MOVE_MODE_SPRINT)
			pHorse->SetMoveMode(Obj_Horse::MOVE_MODE_RUN);
	}
	else
	{// 距离在3m以内，正常跟随
		if (pHorse->GetMoveMode() != Obj_Horse::MOVE_MODE_WALK)
			pHorse->SetMoveMode(Obj_Horse::MOVE_MODE_HOBBLE);
	}

	if (!m_pCharacter->IsMoving() && fDistSqr > 4.1f)
		Baby_Go(pOwner->getWorldPos());

__LEAVE_FUNCTION
}

FLOAT	AI_Horse::CalcDistSqrOfToOwner(VOID)
{
__ENTER_FUNCTION
	Obj_Horse* pHorse = GetCharacter();
	if (pHorse)
	{
		Obj_Character* pOwner = GetCharacter()->GetOwner();
		if (pOwner)
		{
			FLOAT fDeltaX = pOwner->getWorldPos()->m_fX - m_pCharacter->getWorldPos()->m_fX;
			FLOAT fDeltaZ = pOwner->getWorldPos()->m_fZ - m_pCharacter->getWorldPos()->m_fZ;

			return (fDeltaX*fDeltaX + fDeltaZ*fDeltaZ);
		}
	}

	return 0.f;
__LEAVE_FUNCTION
	return 0.f;
}

VOID AI_Horse::Event_OnDie( Obj *pKiller/* = NULL*/ )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION    
}

VOID AI_Horse::Baby_Go(const WORLD_POS *paTargetPos)
{
__ENTER_FUNCTION
	if (!paTargetPos)
	{
		Assert(NULL && "AI_Pet::Baby_Go...paTargetPos=NULL...");
		return ;
	}
	if (!m_pCharacter)
	{
		return ;
	}

	Obj_Character* pOwner = GetCharacter()->GetOwner();
	if (!pOwner)
	{
		return ;
	}
	
	FLOAT fDir = pOwner->getDir()-__HALF_PI;

	FLOAT fX = paTargetPos->m_fX + (2*SinCos::Sin(fDir));
	FLOAT fZ = paTargetPos->m_fZ + (2*SinCos::Cos(fDir));
	WORLD_POS pos;
	pos.m_fX = fX;
	pos.m_fZ = fZ;
	// 确保位置的合法性
	m_pCharacter->getScene()->GetMap()->VerifyPos(&pos);
	if (m_pCharacter->getScene()->GetMap()->IsCanGo(pos)) {
		Move( &pos );
	}
	else {
		Move(paTargetPos);
	}

__LEAVE_FUNCTION
}

VOID AI_Horse::BabyToAttack(VOID)
{
__ENTER_FUNCTION
	
	//ChangeState(ESTATE_COMBAT);

__LEAVE_FUNCTION
}

VOID AI_Horse::Relive( BOOL bSkillRelive )
{
__ENTER_FUNCTION

	ChangeState(ESTATE_IDLE);

__LEAVE_FUNCTION
}

VOID AI_Horse::After_Die()
{
__ENTER_FUNCTION

__LEAVE_FUNCTION    
}

ORESULT AI_Horse::PushCommand_UseSkill(ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget)
{
__ENTER_FUNCTION

	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

BOOL AI_Horse::ProcessSkillInCache(BOOL IsAttackState/*= TRUE*/)
{
__ENTER_FUNCTION
	
	return TRUE;	

__LEAVE_FUNCTION
	return FALSE;
}

VOID AI_Horse::ToRandMove()
{
__ENTER_FUNCTION
	
	WORLD_POS Tar;
	BOOL FindPos=FALSE;
	for (INT i=0; i<HORSE_MAX_TRY_TIMES; i++)
	{
		Tar.m_fX = GetCharacter()->GetRespawnPos()->m_fX+rand()%HORSE_MOVE_RANGE - rand()%HORSE_MOVE_RANGE;
		Tar.m_fZ = GetCharacter()->GetRespawnPos()->m_fZ+rand()%HORSE_MOVE_RANGE - rand()%HORSE_MOVE_RANGE;
		GetCharacter()->getScene()->GetMap()->VerifyPos( &Tar );
		if (GetCharacter()->getScene()->GetMap()->IsCanGo(Tar) )
		{
			FindPos = TRUE ;
			break ;
		}
	}

	if( FindPos==FALSE )
		return ;
	if(Move(&Tar) == FALSE)
		return ;

__LEAVE_FUNCTION
}

BOOL	AI_Horse::IsCombatBeOver()
{
__ENTER_FUNCTION
	//Obj_Horse* pHorse = (Obj_Horse*)m_pCharacter;
	//if (!pHorse) 
	//{
	//	Assert(NULL && "AI_Pet::IsCombatBeOver...pPet=NULL...");
	//	return TRUE;
	//}
	//Obj_Human* pCreator = pHorse->GetCreator();
	//if (!pCreator)
	//{
	//	return TRUE;
	//}
	//if (pCreator->GetStealthLevel() > 0)
	//{
	//	return TRUE;
	//}
	//if (INVALID_ID != pHorse->GetTargetID())
	//{
	//	Obj* pObj = pHorse->getScene()->GetObjManager()->GetObj(pHorse->GetTargetID());
	//	if (pObj && IsCharacterObj(pObj->GetObjType())==TRUE)
	//	{
	//		if ( ((Obj_Character*)pObj)->IsCanViewMe(pCreator) == FALSE )
	//		{// 如果宠物的主人看不到目标则战斗结束
	//			return TRUE;
	//		}
	//		FLOAT fDeltaX = pObj->getWorldPos()->m_fX - pHorse->getWorldPos()->m_fX;
	//		FLOAT fDeltaZ = pObj->getWorldPos()->m_fZ - pHorse->getWorldPos()->m_fZ;
	//		FLOAT fDistSqr = fDeltaX*fDeltaX + fDeltaZ*fDeltaZ;

	//		if ( fDistSqr > HORSE_REFOLLOW_DISTSQR_C ) 
	//		{// 距离大于15m则自动结束战斗
	//			return TRUE;			
	//		}
	//	}
	//}
	
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}
