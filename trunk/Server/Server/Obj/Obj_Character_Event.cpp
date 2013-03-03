// Character_Skill.cpp

#include "stdafx.h"
#include "Scene.h"
#include "GCOperateResult.h"
#include "Skill_Core.h"
#include "Combat_Core.h"
#include "GameTable.h"
#include "Obj_Character.h"
#include "AI_Character.h"
#include "ActionDelegator.h"
#include "StdImpactSubSkill60.h"

using namespace Combat_Module::Skill_Module;
using namespace Combat_Module;
using namespace Action_Module;

VOID OnBeAttack(Obj_Character& rAttacker)
{
}

VOID OnBeAssisted(Obj_Character& rAssistor)
{
}

VOID Obj_Character::OnBeSkill(Obj_Character& rAttacker, INT nBehaviorType)
{
__ENTER_FUNCTION
	// �ڽ�ɫ�ܵ�����ʱ�ص���ҵ�BabyToAttack-->������ҵĳ���ȥ����
	if (BEHAVIOR_TYPE_HOSTILITY== nBehaviorType)
	{
		if (rAttacker.GetObjType() == Obj::OBJ_TYPE_HUMAN)
		{
			((Obj_Human*)&rAttacker)->SetCurTarget(GetID());
			((Obj_Human*)&rAttacker)->BabyToAttack();
		}
	}
	m_pAI_Character->OnBeSkill(&rAttacker, nBehaviorType);
__LEAVE_FUNCTION
}

//����ͬһ�����ܾ��ж����˺�����������þ���ImpactLogic_T����ÿ�������˺������ض�����
//Ȼ��������˺����͵��˺�ֵ����ٵ���Impact_OnDamage����ʵ�ʵ��˺�
VOID Obj_Character::OnDamages(INT const* const pDamage, ObjID_t nAttackerID, BOOL bCritical, SkillID_t nSkillID, INT iLogicCount)
{
	__ENTER_FUNCTION
	//��ʼ���˺��б�������Ϊ�±�
	INT DamageList[DAMAGE_TYPE_NUMBER];

	DamageList[DAMAGE_TYPE_NEAR]    = pDamage[DAMAGE_TYPE_NEAR];
	DamageList[DAMAGE_TYPE_FAR]     = pDamage[DAMAGE_TYPE_FAR];
	DamageList[DAMAGE_TYPE_MAGIC_NEAR]= pDamage[DAMAGE_TYPE_MAGIC_NEAR];
	DamageList[DAMAGE_TYPE_MAGIC_FAR]= pDamage[DAMAGE_TYPE_MAGIC_FAR];

	DamageList[DAMAGE_TYPE_GOLD]	= pDamage[DAMAGE_TYPE_GOLD];
	DamageList[DAMAGE_TYPE_WOOD]	= pDamage[DAMAGE_TYPE_WOOD];
	DamageList[DAMAGE_TYPE_WATER]	= pDamage[DAMAGE_TYPE_WATER];
	DamageList[DAMAGE_TYPE_FIRE]	= pDamage[DAMAGE_TYPE_FIRE];
	DamageList[DAMAGE_TYPE_SOIL]	= pDamage[DAMAGE_TYPE_SOIL];
	
	Scene* pScene = getScene();
	Obj_Character* pAttacker = NULL;
	//��ù����߶���
	if(NULL!=pScene && INVALID_ID!=nAttackerID)
	{
		ObjManager* pObjManager = pScene->GetObjManager();
		if(NULL !=pObjManager)
		{
			pAttacker = (Obj_Character*)(pObjManager->GetObj(nAttackerID));
		}
	}
	//DamageShield process by types
	//ִ������Ч���б��е�����Ч��
	Impact_OnDamages(DamageList, pAttacker, nSkillID);

	//Total damage Calculation
	INT nDamage = DamageList[DAMAGE_TYPE_NEAR]
				+ DamageList[DAMAGE_TYPE_FAR]
				+ DamageList[DAMAGE_TYPE_MAGIC_NEAR]
				+ DamageList[DAMAGE_TYPE_MAGIC_FAR]

				+ DamageList[DAMAGE_TYPE_GOLD]
				+ DamageList[DAMAGE_TYPE_WOOD]
				+ DamageList[DAMAGE_TYPE_WATER]
				+ DamageList[DAMAGE_TYPE_FIRE]
				+ DamageList[DAMAGE_TYPE_SOIL];

	// �˺���СΪ1
	0>=nDamage?nDamage=1:NULL;
	//non-type damage shield here
	Impact_OnDamage(nDamage, pAttacker, nSkillID);
	if(NULL!=pAttacker)
	{
		pAttacker->OnDamageTarget(nDamage, *this, nSkillID);
	}
	
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		MenPai_OnDamage(nDamage);
	}

	HealthIncrement(-nDamage, iLogicCount, pAttacker, bCritical);

	__LEAVE_FUNCTION
}

//��һ�˺����͵Ĵ���
VOID Obj_Character::OnDamage(INT nDamage, ObjID_t nAttackerID, BOOL bCritical, SkillID_t nSkillID, INT iLogicCount)
{
	__ENTER_FUNCTION
	Scene* pScene = getScene();
	Obj_Character* pAttacker = NULL;
	if(NULL!=pScene && INVALID_ID!=nAttackerID)
	{
		ObjManager* pObjManager = pScene->GetObjManager();
		if(NULL !=pObjManager)
		{
			pAttacker = (Obj_Character*)(pObjManager->GetObj(nAttackerID));
		}
	}
	// �˺���СΪ1
	0>=nDamage?nDamage=1:NULL;
	//non-type damage shield here
	Impact_OnDamage(nDamage, pAttacker, nSkillID);
	if(NULL!=pAttacker)
	{
		pAttacker->OnDamageTarget(nDamage, *this, nSkillID);
	}

	// �����������ԵĴ���
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		MenPai_OnDamage(nDamage);
	}

	HealthIncrement(-nDamage, iLogicCount, pAttacker, bCritical);

	__LEAVE_FUNCTION
}

VOID Obj_Character::OnMyTrapActivated(Obj_Special& rTrap)
{
	__ENTER_FUNCTION
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		MenPai_OnMyTrapActivated(rTrap);
	}
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnDamageTarget(INT& nDamage, Obj_Character& rTar, SkillID_t nSkillID)
{
	__ENTER_FUNCTION
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		MenPai_OnDamageTarget(rTar, nDamage);
	}
	Impact_OnDamageTarget(nDamage, rTar, nSkillID);
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnBeHit(Obj_Character& rAttacker)
{
	__ENTER_FUNCTION
	Impact_OnBeHit(rAttacker);

	//if( GetObjType() == Obj::OBJ_TYPE_HUMAN )
	//{
	//	static_cast<Obj_Human*>(this)->BabyToProtectMe( rAttacker.GetID() );
	//}
	__LEAVE_FUNCTION
}
VOID Obj_Character::OnHitTarget(Obj_Character& rTarget)
{
	__ENTER_FUNCTION
	Impact_OnHitTarget(rTarget);

	__LEAVE_FUNCTION
}

VOID Obj_Character::OnBeMiss(Obj_Character& rAttacker)
{
	__ENTER_FUNCTION
	Impact_OnMissHit(rAttacker);
	
	__LEAVE_FUNCTION
}
VOID Obj_Character::OnMissTarget(Obj_Character& rTarget)
{
	__ENTER_FUNCTION
	Impact_OnMissTarget(rTarget);

	__LEAVE_FUNCTION
}

VOID Obj_Character::OnActionStarted(VOID)
{
	__ENTER_FUNCTION
	Impact_OnActionStarted();
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnCriticalHitTarget(SkillID_t const nSkill, Obj_Character& rTarget)
{
	__ENTER_FUNCTION
	Impact_OnCriticalHitTarget(nSkill, rTarget);
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnBeCriticalHit(SkillID_t const nSkill, Obj_Character& rAttacker)
{
	__ENTER_FUNCTION
	Impact_OnBeCriticalHit(nSkill, rAttacker);
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnDepleteStrikePoints(INT nStrikePoints)
{
	__ENTER_FUNCTION
	// �����������ԵĴ���
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		MenPai_OnDepleteStrikePoints(nStrikePoints);
	}
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnHealTarget(SkillID_t const nSkill, Obj_Character& rTarget, INT nHealedHP)
{
	__ENTER_FUNCTION
	// �����������ԵĴ���
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		MenPai_OnHealTarget(nHealedHP);
	}
	Impact_OnHealTarget(nSkill, rTarget, nHealedHP);
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnBeHeal(SkillID_t const nSkill, Obj_Character& rCaster, INT nHealedHP)
{
	__ENTER_FUNCTION
	Impact_OnBeHeal(nSkill, rCaster, nHealedHP);
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnUseSkillSuccessfully(SkillInfo_T& rSkill)
{
	__ENTER_FUNCTION
	// pet get more action point here
	
	if( !Impact_HaveImpactOfSpecificLogicID( StdImpact060_T::ID ) )
	{
		//�����Ӽ���
		OnSubSkill( rSkill );
	}
	Impact_OnUseSkillSuccessfully(rSkill);
	
	UpdataEquipDamagePoint( DURSPOIL_ATTACT );
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnSubSkill( SkillInfo_T& rSkillInf )
{
	__ENTER_FUNCTION
	INT iSkillID = rSkillInf.GetSkillID();
	Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(iSkillID);
	INT iIndex = 0;
	for( INT i=0; i<Skill_Module::SkillTemplateData_T::MAX_SUB_SKILL; ++i )
	{
		INT iSubSkillID = pSkillTemplate->GetSubSkill( i );
		if( INVALID_ID ==  iSubSkillID )
		{
			break;
		}
		INT iRateForSubSkill = pSkillTemplate->GetSubSkillSuccessRate( i );
		if( !IsSubSkillCanSuccess(iRateForSubSkill) )
		{
			continue;
		}
		//����Ҵ��������б���Ӵ˼���
		SetSubSkillID( iIndex, iSubSkillID );
		++iIndex;
	}

	__LEAVE_FUNCTION
}

BOOL Obj_Character::IsSubSkillCanSuccess( INT iRate )
{
	__ENTER_FUNCTION
	Scene* pScene = getScene();
	if(NULL==pScene)
	{
		return FALSE;
	}
	INT nRet = pScene->GetRand100();
	if( nRet <= iRate )
	{
		return TRUE;
	}
	__LEAVE_FUNCTION

	return FALSE;
}

VOID Obj_Character::OnDie( ObjID_t idKiller )
{
__ENTER_FUNCTION

	//Ч�������
	Impact_OnDie();
	////���������
	//Obj *pObj = getScene()->GetObjManager()->GetObj( idKiller );
	//if ( pObj != NULL )
	//{
	//	if ( IsCharacterObj( pObj->GetObjType() ) )
	//	{
	//		pObj->OnKillObject( GetID() );
	//	}
	//}

	if ( GetScriptID() != INVALID_ID )
	{
		getScene()->GetLuaInterface()->ExeScript_DDD(
			GetScriptID(), 
			"OnDie",
			getScene()->SceneID(),
			(INT)GetID(), 
			(INT)idKiller );
	}

	if(FALSE == GetGlobalActionDelegator().CanDoNextAction(*this))
	{
		GetGlobalActionDelegator().InterruptCurrentAction(*this);
	}

	AI_Character *pAI = GetAIObj();
	if(NULL!=pAI)
	{
		Obj *pKiller = getScene()->GetObjManager()->GetObj(idKiller);
		pAI->OnDie(pKiller);
	}
__LEAVE_FUNCTION
}

VOID Obj_Character::OnRelive( ObjID_t idKiller )
{
__ENTER_FUNCTION

	AI_Character *pAI = GetAIObj();
	if(NULL!=pAI)
	{
		Obj *pKiller = getScene()->GetObjManager()->GetObj(idKiller);
		pAI->OnRelive(pKiller);

		pAI->Relive(TRUE);
	}

__LEAVE_FUNCTION
}

INT Obj_Character::OnFiltrateImpact(OWN_IMPACT& rImp)
{
	__ENTER_FUNCTION
	return Impact_OnFiltrateImpact(rImp);
	__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Character::OnStealthLevelChanged()
{
__ENTER_FUNCTION
	UpdateViewCharacter();
__LEAVE_FUNCTION
}

VOID Obj_Character::OnDetectLevelChanged()
{
__ENTER_FUNCTION
	UpdateViewCharacter();
__LEAVE_FUNCTION
}

VOID Obj_Character::OnLevelChanged()
{
__ENTER_FUNCTION
	UpdateViewCharacter();
__LEAVE_FUNCTION
}
