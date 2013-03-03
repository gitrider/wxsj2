// AI_Pet.cpp

#include "stdafx.h"
#include "Obj_Character.h"
#include "TimeManager.h"
#include "Obj_Pet.h"
#include "Scene.h"
#include "GameTable.h"
#include "AI_Pet.h"
#include "GCPlayerRelive.h"
#include "GCManipulatePetRet.h"
#include "GCOperateResult.h"
#include "SinCos.h"

#define DELAY_TIME (5000)
#define PET_MOVE_RANGE (10)
#define MAX_TRY_TIMES (5)

const FLOAT PET_REFOLLOW_DISTSQR_B = 100;
const FLOAT PET_REFOLLOW_DISTSQR_C = 225;
const FLOAT PET_STAND_DISTANCE_TO_ONWER_MIN = 4.0f;

AI_Pet::AI_Pet( VOID )
{
	m_RandMoveTimer.CleanUp();
	m_ScanImpactOfOwnerTimer.CleanUp();
}

AI_Pet::~AI_Pet( VOID )
{
}

BOOL AI_Pet::Init( Obj_Character *pCharacter )
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

	m_nDelayTime = DELAY_TIME;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID AI_Pet::Term( VOID )
{
	AI_Character::Term( );
}

VOID AI_Pet::AI_Logic_Idle( UINT uTime )
{
__ENTER_FUNCTION

	Obj_Pet* pPet = (Obj_Pet*)GetCharacter();
	//if (!pPet->GetCreator()) 
	//������ﵱǰû��ӵ����
	if( pPet->GetOwnerID() == INVALID_ID )
	{
		if( !m_RandMoveTimer.IsSetTimer() )
		{
			m_RandMoveTimer.BeginTimer(DELAY_TIME+rand()%DELAY_TIME, uTime ) ;
		}
		if( m_RandMoveTimer.CountingTimer( uTime ) )
		{
			ToRandMove( ) ;
		}
	}
	else 
	{
		if (m_nDelayTime > 0)
		{
			m_nDelayTime -= m_pCharacter->GetLogicTime();
		}
		else
		{
			if (m_pCharacter && !m_pCharacter->IsMoving() )
			{// �鿴��������û��Ҫ�����ļ��ܣ������ʱ����
				m_pCharacter->SetMoveMode(Obj_Pet::MOVE_MODE_WALK);
				ProcessSkillInCache(FALSE);
			}
		}
		ChangeMoveMode();
	}

__LEAVE_FUNCTION	
}

VOID AI_Pet::AI_Logic_Combat( UINT uTime )
{
__ENTER_FUNCTION

	Obj_Pet* pPet = (Obj_Pet*)m_pCharacter;
	if(NULL==m_pCharacter)
	{
		AssertEx(FALSE,"[AI_Pet::Logic_Attack]: NULL m_pCharacter Found!! check it now.");
		return ;
	}
	// �Ƿ����ս��
	if (IsCombatBeOver())
	{
		ChangeState(ESTATE_IDLE);
		pPet->SetTargetID(INVALID_ID);
		return ;
	}
	// �����ǰ������û�����򲻽����κβ���
	if ( FALSE == m_pCharacter->CanUseSkillNow())
	{
		return ;
	}

	if ( m_pCharacter->IsMoving() ) 
	{// �������Moving�򲻽��й���
		return ;
	}


	// ִ�л����е��﹦�ͻ򷨹��͵ļ���
	if (FALSE == ProcessSkillInCache() )
	{// Ĭ��ʹ��0����ͨ����
		// �ж�Ŀ�����Ч��
		Obj* pTarObj = m_pCharacter->getScene()->GetObjManager()->GetObj( pPet->GetTargetID() );
		if ( pTarObj == NULL || !((Obj_Character*)pTarObj)->IsAlive() )
		{// Ŀ����Ч
			pPet->SetTargetID( INVALID_ID );

			ChangeState(ESTATE_IDLE);
			return ;
		}
		INT HurtDelayTime = Skill_GetSkillHurtDelayTime( 0 );

		UseSkill(0, pPet->GetTargetID(), pTarObj->getWorldPos()->m_fX, pTarObj->getWorldPos()->m_fZ, HurtDelayTime); 
	}

__LEAVE_FUNCTION

}

VOID AI_Pet::AI_Logic_Dead( UINT uTime )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID	AI_Pet::ChangeMoveMode()
{
__ENTER_FUNCTION

	Obj_Pet* pPet = GetCharacter();
	if (!pPet)
	{
		return ;
	}
	Obj_Character* pOwner = pPet->GetOwner();
	if (!pOwner)
	{
		return ;
	}
	FLOAT fDistSqr = CalcDistSqrOfToOwner();
	if ( fDistSqr > PET_REFOLLOW_DISTSQR_C ) 
	{// �������15m��˲��
		if (pPet->CanMove()) 
		{// ��ǿ�����²���˲��:�綨��, ѣ��...
			FLOAT fDir = pOwner->getDir()-__PI/2;

			FLOAT fX = pOwner->getWorldPos()->m_fX - (-(2*sin(fDir)) );
			FLOAT fZ = pOwner->getWorldPos()->m_fZ - (2*cos(fDir));
			WORLD_POS pos;
			pos.m_fX = fX;
			pos.m_fZ = fZ;

			pPet->getScene()->GetMap()->VerifyPos(&pos);
			pPet->Teleport( &pos );
			pPet->SetMoveMode(Obj_Pet::MOVE_MODE_WALK);
			return ;
		}
	}
	else if (fDistSqr > PET_REFOLLOW_DISTSQR_B) 
	{// �������PET_REFOLLOW_DISTSQR_B�������
		pPet->SetMoveMode(Obj_Pet::MOVE_MODE_RUN);
	}
	else
	{
		if (pPet->GetMoveMode() != Obj_Pet::MOVE_MODE_WALK)
			pPet->SetMoveMode(Obj_Pet::MOVE_MODE_HOBBLE);
	}
	if (!m_pCharacter->IsMoving() && 
		(fDistSqr>REFOLLOW_DISTSQR_A||fDistSqr<PET_STAND_DISTANCE_TO_ONWER_MIN) )
		Baby_Go(pOwner->getWorldPos());

__LEAVE_FUNCTION
}

FLOAT	AI_Pet::CalcDistSqrOfToOwner(VOID)
{
__ENTER_FUNCTION
	Obj_Pet* pPet = GetCharacter();
	if (pPet)
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

VOID AI_Pet::Event_OnDie( Obj *pKiller/* = NULL*/ )
{
__ENTER_FUNCTION

	//ObjID_t idKiller = INVALID_ID;
	//if ( pKiller != NULL )
	//{
	//	idKiller = pKiller->GetID() ;
	//}
	//
	//After_Die();

	////////////////////////////////////////////////////////////////////
	//// ������︴�����
	//UINT nTermTime = 0;
	//Obj_Pet* pPet = GetCharacter();
	//if (pPet)
	//{
	//	const Obj_Character::_RELIVE_INFO *pReliveInfo = pPet->GetReliveInfo(TRUE);
	//	if (pReliveInfo != NULL)
	//	{
	//		Obj_Human* pCreator = pPet->GetCreator();
	//		if (pCreator)
	//		{
	//			nTermTime = BODYSTAYTIME / 3;
	//		}
	//	}
	//}
	//if (nTermTime)
	//	m_BodyTimer.BeginTimer(nTermTime, g_pTimeManager->CurrentTime());
	//else
	//	m_BodyTimer.BeginTimer(BODYSTAYTIME, g_pTimeManager->CurrentTime());

 //   ChangeState(ESTATE_DEAD);	


__LEAVE_FUNCTION    
}

VOID AI_Pet::Baby_Go(const WORLD_POS *paTargetPos)
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

	//�������ս��������������ս������������ս��״̬��ط����������а��������˵ľ����ж�
	State* pState = GetAIState();
	if( pState != NULL )
	{
		FLOAT fDistSqr = CalcDistSqrOfToOwner();

		if( pState->GetStateID()==ESTATE_COMBAT && fDistSqr<PET_REFOLLOW_DISTSQR_B )
		{
			return;
		}
	}
	
	WORLD_POS pos;
	Obj_Pet* pPet = static_cast<Obj_Pet*>(GetCharacter());
	INT iIndex = pPet->GetIndexInOnwer();
	FLOAT fDistanceRefix = pOwner->getScene()->GetRand100() * 0.02f;
	GetStandPos(pOwner, iIndex, paTargetPos, fDistanceRefix, pos);

	// ȷ��λ�õĺϷ���
	m_pCharacter->getScene()->GetMap()->VerifyPos(&pos);
	if (m_pCharacter->getScene()->GetMap()->IsCanGo(pos)) 
	{
		Move( &pos );
	}
	else 
	{
		Move(paTargetPos);
	}

__LEAVE_FUNCTION
}

VOID AI_Pet::BabyToAttack(VOID)
{
__ENTER_FUNCTION
	
	ChangeState(ESTATE_COMBAT);

__LEAVE_FUNCTION
}

VOID AI_Pet::Relive( BOOL bSkillRelive )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID AI_Pet::After_Die()
{
__ENTER_FUNCTION


__LEAVE_FUNCTION    
}

ORESULT AI_Pet::PushCommand_UseSkill(ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget)
{
__ENTER_FUNCTION
	Obj_Pet* pPet = GetCharacter();
	if (!pPet)
	{
		Assert(NULL && "AI_Pet::PushCommand_UseSkill...pPet=NULL...");
		return OR_ERROR;
	}
	
	SLogicSkill_Param SkillParam;
	SkillParam.idSkill = idSkill;
	SkillParam.idTarget = idTarget;
	SkillParam.TargetPos = WORLD_POS(fTargetX, fTargetZ);
	SkillParam.fDir = fDir;
	SkillParam.guidTarget = guidTarget;

	SetSkillParamCache(SkillParam);

	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

BOOL AI_Pet::ProcessSkillInCache(BOOL IsAttackState/*= TRUE*/)
{
__ENTER_FUNCTION
	
	Obj_Pet* pPet = GetCharacter();
	if (!pPet)
	{
		Assert(NULL && "AI_Pet::ProcessSkillInCache...pPet=NULL...");
		return FALSE;
	}
	if (FALSE == pPet->IsActiveObj() )
	{
		return FALSE;
	}
    Obj_Human* pCreator = pPet->GetCreator();
    if (!pCreator)
	{// ������Ч��ֱ�ӷ���
		return FALSE;
	}
	if (FALSE == pPet->CanUseSkillNow() )
	{
		return FALSE;
	}
	// ��ִ�л����еļ���--m_SkillID
	const SLogicSkill_Param& SkillParam = GetSkillParamCache();
	if (SkillParam.IsValid() == TRUE)
	{
		INT HurtDelayTime = Skill_GetSkillHurtDelayTime( SkillParam.idSkill );

		INT nRet = UseSkill(SkillParam.idSkill, SkillParam.idTarget, 
			SkillParam.TargetPos.m_fX, SkillParam.TargetPos.m_fZ, HurtDelayTime, SkillParam.fDir, SkillParam.guidTarget);
		const_cast<SLogicSkill_Param&>(SkillParam).Cleanup();
		if(OR_FAILED(nRet))
		{
			pCreator->SendOperateResultMsg(nRet);
		}
		return TRUE;
	}

	INT i;
	for ( i = 0; i < HUMAN_PET_MAX_COUNT; ++i)
	{
		if (INVALID_ID != pPet->m_aCacheOfSkill[i])
		{
			BOOL bCooldowned = pPet->Skill_IsSkillCooldowned(pPet->m_aCacheOfSkill[i]);
			if (!bCooldowned)
			{// �ü��ܵ�CD��û�е�
				continue;
			}
			const SkillTemplateData_T* pSkillData_T = g_SkillTemplateDataMgr.GetInstanceByID(pPet->m_aCacheOfSkill[i]);
			if (!pSkillData_T)
			{
				Assert(NULL && "AI_Pet::ProcessSkillInCache...pSkillData_T=NULL...");
				return FALSE;
			}


			ID_t OperateMode = pSkillData_T->GetOperateModeForPetSkill();
			ID_t TypeOfSkill = pSkillData_T->GetTypeOfPetSkill();

			// ����ǽ���״̬��ֻ�ͷ��﹦�ͻ򷨹��͵ļ���
			if (TRUE == IsAttackState)
			{
				ID_t RateOfSkill = pSkillData_T->GetPetRateOfSkill();
				RateOfSkill == -1 ? RateOfSkill = 100 : NULL;
				//FLOAT fLaunchRate = g_PetAIStrategyTble[TypeOfSkill][pPet->GetAIType()];
				//INT nUseSkillRate = (INT)(RateOfSkill * fLaunchRate);
				INT nUseSkillRate = RateOfSkill;
				if (rand() % 100 > nUseSkillRate)
				{// ��������ü���ʧ�����ж���һ��
					continue;
				}
				// �ж�Ŀ�����Ч��
				Obj* pTarObj = m_pCharacter->getScene()->GetObjManager()->GetObj( pPet->GetTargetID() );
				if ( pTarObj == NULL || !((Obj_Character*)pTarObj)->IsAlive() )
				{// Ŀ����Ч���л��ɸ���״̬
					pPet->SetTargetID( INVALID_ID );
					//						SetAIState( AI_PET_FOLLOW );
					return FALSE;
				}
				INT iHurtDelayTime = Skill_GetSkillHurtDelayTime( pPet->m_aCacheOfSkill[i] );
				ORESULT oResult = UseSkill(pPet->m_aCacheOfSkill[i], pTarObj->GetID(), 
					pTarObj->getWorldPos()->m_fX, pTarObj->getWorldPos()->m_fZ, iHurtDelayTime);
				break;
			}
		}
	}
	if (HUMAN_PET_MAX_COUNT == i)
	{
		return FALSE;
	}

	return TRUE;	

__LEAVE_FUNCTION
	return FALSE;
}

VOID AI_Pet::ToRandMove()
{
__ENTER_FUNCTION
	
	WORLD_POS Tar;
	BOOL FindPos=FALSE;
	for (INT i=0; i<MAX_TRY_TIMES; i++)
	{
		Tar.m_fX = GetCharacter()->GetRespawnPos()->m_fX+rand()%PET_MOVE_RANGE - rand()%PET_MOVE_RANGE;
		Tar.m_fZ = GetCharacter()->GetRespawnPos()->m_fZ+rand()%PET_MOVE_RANGE - rand()%PET_MOVE_RANGE;
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

BOOL	AI_Pet::IsCombatBeOver()
{
__ENTER_FUNCTION
	Obj_Pet* pPet = (Obj_Pet*)m_pCharacter;
	if (!pPet) 
	{
		Assert(NULL && "AI_Pet::IsCombatBeOver...pPet=NULL...");
		return TRUE;
	}
	Obj_Human* pCreator = pPet->GetCreator();
	if (!pCreator)
	{
		return TRUE;
	}
	if (pCreator->GetStealthLevel() > 0)
	{
		return TRUE;
	}
	if (INVALID_ID != pPet->GetTargetID())
	{
		Obj* pObj = pPet->getScene()->GetObjManager()->GetObj(pPet->GetTargetID());
		if (pObj && IsCharacterObj(pObj->GetObjType())==TRUE)
		{
			if ( ((Obj_Character*)pObj)->IsCanViewMe(pCreator) == FALSE )
			{// �����������˿�����Ŀ����ս������
				return TRUE;
			}
			FLOAT fDeltaX = pObj->getWorldPos()->m_fX - pPet->getWorldPos()->m_fX;
			FLOAT fDeltaZ = pObj->getWorldPos()->m_fZ - pPet->getWorldPos()->m_fZ;
			FLOAT fDistSqr = fDeltaX*fDeltaX + fDeltaZ*fDeltaZ;

			if ( fDistSqr > PET_REFOLLOW_DISTSQR_B ) 
			{// �������15m���Զ�����ս��
				return TRUE;			
			}
		}
	}
	
	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}

VOID AI_Pet::GetStandPos( Obj_Character* pChar, INT iIndex, const WORLD_POS* paTargetPos, FLOAT fDistanceRefix, WORLD_POS &oOutPos )
{
	__ENTER_FUNCTION

	FLOAT fCharDir = pChar->__GetDir();
	FLOAT fTmp = (FLOAT)MAX_MONSTER_PET_TAKE; 
	FLOAT fDir = fCharDir - __PI*iIndex/fTmp;

	FLOAT fX = (2*SinCos::Sin(fDir));
	FLOAT fZ = (2*SinCos::Cos(fDir));
	
	FLOAT fRefixX = GetGoodRefix( fX, fDistanceRefix );
	FLOAT fRefixZ = GetGoodRefix( fZ, fDistanceRefix );

	fX += paTargetPos->m_fX + fRefixX;
	fZ += paTargetPos->m_fZ + fRefixZ;
	oOutPos.m_fX = fX;
	oOutPos.m_fZ = fZ;
	__LEAVE_FUNCTION

}

FLOAT AI_Pet::GetGoodRefix( FLOAT iBase, FLOAT iRefix )
{
	if( iBase >= 0 )
	{
		return iRefix;
	}
	return -iRefix;
}