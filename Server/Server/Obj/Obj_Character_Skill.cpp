// Character_Skill.cpp

#include "stdafx.h"
#include "Obj_Character.h"
#include "SkillInfo.h"
#include "Skill_Core.h"
#include "AI_Character.h"
#include "GCCharSkill_Missed.h"
#include "GCDetailHealsAndDamages.h"
#include "Scene.h"
#include "GameTable.h"
#include "Obj_Pet.h"
#include "Obj_Monster.h"
#include "AI_Monster.h"
#include "ActionDelegator.h"


using namespace Action_Module;
using namespace Combat_Module::Skill_Module;
using namespace Combat_Module;
using namespace Combat_Module;
using namespace Combat_Module::Camp_Stand;
using namespace Packets;

static VOID BroadCast_HealAndDamageMsg(Obj_Character* pThis, Obj_Character* pTarget,GCDetailHealsAndDamages& rMsg);

// 计算对方对我来说是否为隐身的
BOOL CalcIsTargetStealth(INT nThisLevel, INT nThisDetectLevel, const WORLD_POS *pThisPos, INT nTargetLevel, INT nTargetStealthLevel, const WORLD_POS *pTargetPos)
{
	if(nTargetStealthLevel <= 0)
		return FALSE;

	INT nDeltaLevel = nThisDetectLevel + nThisLevel;
	INT nStealthLevel = nTargetStealthLevel + nTargetLevel;
	if(nStealthLevel > nDeltaLevel)
	{
		return TRUE;
	}
	return FALSE;
}



VOID Obj_Character::GM_killObj(VOID)
{
__ENTER_FUNCTION
	INT nHP = GetHP();
	
	HealthIncrement(-nHP, INVALID_ID, this);		

__LEAVE_FUNCTION
}

VOID Obj_Character::GM_ForceDie(Obj_Character* pKiller)
{
	__ENTER_FUNCTION
	if(TRUE==IsAliveInDeed()&&TRUE==IsActiveObj())
	{
		SetHP(0);
		OnDie(pKiller->GetID());
	}
	__LEAVE_FUNCTION
}

const float KLU_PI = 3.1415926f;
#define KLU_MINFLOAT	(1.0E-9)

FLOAT Obj_Character::KLU_GetYAngle(const WORLD_POS& fvPos1, const WORLD_POS& fvPos2)	
{
	float fDistance = WordPosLength(fvPos1.m_fX-fvPos2.m_fX, fvPos1.m_fZ-fvPos2.m_fZ );
	if(fDistance <= 0.0f) return 0.0f;

	double fACos = (fvPos2.m_fZ - fvPos1.m_fZ ) / fDistance;
	if( fACos >  1.0f) fACos = 0.0f;
	if( fACos < -1.0f) fACos = KLU_PI;

	if( fACos >-KLU_MINFLOAT && fACos <KLU_MINFLOAT)
	{
		if(fvPos2.m_fX > fvPos1.m_fX)
			return  (FLOAT)KLU_PI/2.0f;
		else						
			return -(FLOAT)KLU_PI/2.0f;
	}

	fACos = ::acos(fACos);

	// [0~180]
	if(fvPos2.m_fX >= fvPos1.m_fX)
		return (FLOAT)fACos;
	//(180, 360)
	else
		return (FLOAT)(2 * KLU_PI - fACos);
}


VOID Obj_Character::BeatBack( 
							 Obj_Character* pAttacker, 
							 INT iMyHPValue, 
							 VOID* pMsg,
							 const SkillTemplateData_T* pSkillTemplate 
							 )
{
	__ENTER_FUNCTION

	GCDetailHealsAndDamages& msg = *(GCDetailHealsAndDamages*)pMsg;

	const WORLD_POS* pTmpPos = pAttacker->getWorldPos();
	//被伤害者的位置
	const WORLD_POS* pPosMe	 = getWorldPos();
	//按概率计算是否击退
	msg.SetBeatBack( IsBeatBack( *pAttacker, pSkillTemplate ) );
	WORLD_POS sPos;
	//做碰撞检测, 计算击退距离
	CalculateBeatBackPos( *pAttacker, this, pSkillTemplate, sPos );
	msg.SetBeatBackPos( sPos );
	//设置击退位置
	msg.SetCloseToEnable( FALSE );

	//只有选择目标的技能才做跟进
	if( pSkillTemplate->GetTargetingLogic()==TARGET_SPECIFIC_UNIT && Obj::OBJ_TYPE_HUMAN==pAttacker->GetObjType() )
	{
		////计算跟进的位置

		FLOAT fAngle = KLU_GetYAngle( *pTmpPos, *pPosMe );
		pAttacker->setDir( fAngle );

		msg.SetAttackerAngle( fAngle );

		WORLD_POS sPosAttacker = *pTmpPos;
		sPosAttacker.m_fX += sPos.m_fX - pPosMe->m_fX;
		sPosAttacker.m_fZ += sPos.m_fZ - pPosMe->m_fZ;
		//设置跟进位置
		pAttacker->setWorldPos( &sPosAttacker );
		msg.SetCloseToPos( sPosAttacker );
		msg.SetCloseToEnable( TRUE );
	}
	setWorldPos( &sPos );

	__LEAVE_FUNCTION
}

VOID Obj_Character::BeatFly( 
							Obj_Character* pAttacker, 
							INT iMyHPValue, 
							VOID* pMsg,
							const SkillTemplateData_T* pSkillTemplate
							)
{
	__ENTER_FUNCTION

	GCDetailHealsAndDamages& msg = *(GCDetailHealsAndDamages*)pMsg;
	//已经死亡应击飞, 攻击者必须是玩家
	if( iMyHPValue <= 0 && Obj::OBJ_TYPE_HUMAN==pAttacker->GetObjType() )
	{
		//按概率计算是否击飞
		msg.SetBeatFly( IsBeatFly( *pAttacker, pSkillTemplate ) );
		msg.SetBeatFlyDistance( pSkillTemplate->GetBeatFlyDistance() );
	}
	__LEAVE_FUNCTION
}

VOID Obj_Character::SendHealAndDamageMSG( INT nIncrement, Obj_Character* pCharacter, BOOL bCritical, INT iLogicCount )
{
	__ENTER_FUNCTION

	GCDetailHealsAndDamages msg;
	INT nValue = GetHP() + nIncrement;

	if( NULL != pCharacter )
	{
		//获得攻击者角色身上的技能信息对象
		SkillInfo_T& rSkillInfo = pCharacter->GetSkillInfo();
		INT iID = rSkillInfo.GetSkillID();
		if( iID != INVALID_ID )
		{
			//根据技能ID从全局技能模板管理器中获得技能模板实例
			const SkillTemplateData_T* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID( iID );
			//已经死亡应击飞
			if( nValue <= 0 )
			{
				//按概率计算是否击飞
				BeatFly( pCharacter, nValue, (VOID*)&msg, pSkillTemplate );
			}
			//如果HP是减少，应击退, 击打者跟进
			//else if( nIncrement < 0 )
			//{
			//	////按概率计算是否击退
			//	BeatBack( pCharacter, nValue, (VOID*)&msg, pSkillTemplate );
			//}
		}
	}

	msg.SetReceiverID(GetID());
	if(pCharacter)
	{
		msg.SetSenderID(pCharacter->GetID());
		msg.SetSenderLogicCount(iLogicCount);
	}
	else
	{
		msg.SetSenderID(INVALID_ID);
		msg.SetSenderLogicCount(0);
	}
	msg.SetHpModification(nIncrement);
	msg.SetCriticalHitFlag(bCritical);
	BroadCast_HealAndDamageMsg(this, pCharacter,msg);
	__LEAVE_FUNCTION
}


VOID Obj_Character::OnIncementHp( INT nIncrement, Obj_Character* pCharacter )
{
	__ENTER_FUNCTION

	INT nValue = GetHP() + nIncrement;
	if(0<=nIncrement)
	{
		//OnHeal
		if(GetMaxHP()<nValue)
		{
			nValue=GetMaxHP();
		}
		INT nOldHP = GetHP();
		SetHP(nValue);
		if(nOldHP <= 0 && GetHP() > 0)
		{
			if ( this->GetObjType() != OBJ_TYPE_HUMAN  )
				OnRelive((pCharacter)?(pCharacter->GetID()):INVALID_ID);
		}
	}	
	__LEAVE_FUNCTION
}

VOID Obj_Character::OnDecementHP( INT nIncrement, BOOL bDOT, Obj_Character* pCharacter )
{
	__ENTER_FUNCTION

	if( nIncrement < 0 )
	{
		INT nValue = GetHP() + nIncrement;

		//消耗装备耐久
		UpdataEquipDamagePoint( DURSPOIL_DEFENCE );

		// set hp now
		//打断和干扰动作进行,如果是DOT,HOT则不打断动作
		if( !bDOT )
		{
			GetGlobalActionDelegator().DisturbCurrentAction(*this);
		}
		AI_Character *pAI = GetAIObj();
		INT nOldHP = GetHP();
		if(0>nValue)
		{
			nValue=0;
		}
		SetHP(nValue);

		if(NULL!=pAI)
		{
			pAI->OnDamage(-nIncrement, pCharacter);
		}
		if(nOldHP > 0 && GetHP() <= 0)
		{
			SetMP(nValue); // add by gh 2010/04/09 for refresh die attri
			if(Obj::OBJ_TYPE_HUMAN==GetObjType())
			{
				SetPneuma( nValue );
			}

			INT iTmpID = INVALID_ID;
			if( NULL != pCharacter )
			{
				OnDie(pCharacter->GetID());
			}
		}
	}
	__LEAVE_FUNCTION
}

VOID Obj_Character::HealthIncrement(INT nIncrement, INT iLogicCount, Obj_Character* pCharacter, BOOL bCritical, BOOL bDOT )
{
	__ENTER_FUNCTION
	if(TRUE==IsActiveObj())
	{
		SendHealAndDamageMSG(nIncrement, pCharacter, bCritical, iLogicCount);
		//处理血量增加
		OnIncementHp(nIncrement, pCharacter);
		//处理血量减少
		OnDecementHP(nIncrement, bDOT, pCharacter);
		//为技能同步，实时发送CharBase消息，并清除HP脏标记，组织自动刷属性时重复发送
		SendCharBaseDirectForHP();
	}
	__LEAVE_FUNCTION
}


VOID Obj_Character::ManaIncrement(INT nIncrement, INT iLogicCount, Obj_Character* pCharacter)
{
	__ENTER_FUNCTION
	if(TRUE==IsActiveObj())
	{
		INT nValue = GetMP() + nIncrement;
		GCDetailHealsAndDamages msg;
		msg.SetReceiverID(GetID());
		if(pCharacter)
		{
			msg.SetSenderID(pCharacter->GetID());
			msg.SetSenderLogicCount(iLogicCount);
		}
		else
		{
			msg.SetSenderID(INVALID_ID);
			msg.SetSenderLogicCount(0);
		}
		msg.SetMpModification(nIncrement);
		msg.SetCriticalHitFlag(FALSE);
		BroadCast_HealAndDamageMsg(this, pCharacter,msg);
		if(0<=nIncrement)
		{
			//OnHeal
			if(GetMaxMP()<nValue)
			{
				nValue = GetMaxMP();
			}
			SetMP(nValue);
		}
		else
		{   //OnDamage
			if(0>nValue)
			{
				nValue=0;
			}
			SetMP(nValue);
		}
	}
	__LEAVE_FUNCTION
}

VOID Obj_Character::PneumaIncrement(INT nIncrement, INT iLogicCount, Obj_Character* pCharacter)
{
	__ENTER_FUNCTION
	if(TRUE==IsActiveObj())
	{
		INT nValue = GetPneuma() + nIncrement;
		GCDetailHealsAndDamages msg;
		msg.SetReceiverID(GetID());
		if(pCharacter)
		{
			msg.SetSenderID(pCharacter->GetID());
			msg.SetSenderLogicCount(iLogicCount);
		}
		else
		{
			msg.SetSenderID(INVALID_ID);
			msg.SetSenderLogicCount(0);
		}
		msg.SetPneumaModification(nIncrement);
		msg.SetCriticalHitFlag(FALSE);
		BroadCast_HealAndDamageMsg(this, pCharacter,msg);
		if(0<=nIncrement)
		{
			if(GetMaxPneuma()<nValue)
			{
				nValue = GetMaxPneuma();
			}
			SetPneuma(nValue);
		}
		else
		{   
			if(0>nValue)
			{
				nValue=0;
			}
			SetPneuma(nValue);
		}
	}
	__LEAVE_FUNCTION
}

VOID Obj_Character::RageIncrement(INT nIncrement, INT iLogicCount, Obj_Character* pCharacter)
{
	__ENTER_FUNCTION
	if(TRUE==IsActiveObj()&&Obj::OBJ_TYPE_HUMAN == GetObjType())
	{
		INT nValue = GetRage() + nIncrement;
		GCDetailHealsAndDamages msg;
		msg.SetReceiverID(GetID());
		if(pCharacter)
		{
			msg.SetSenderID(pCharacter->GetID());
			msg.SetSenderLogicCount(iLogicCount);
		}
		else
		{
			msg.SetSenderID(INVALID_ID);
			msg.SetSenderLogicCount(0);
		}
		msg.SetRageModification(nIncrement);
		msg.SetCriticalHitFlag(FALSE);
		BroadCast_HealAndDamageMsg(this, pCharacter,msg);
		if(0<=nIncrement)
		{
			//OnHeal
			if(GetMaxRage()<nValue)
			{
				nValue = GetMaxRage();
			}
			SetRage(nValue);
		}
		else
		{   //OnDamage
			if(0>nValue)
			{
				nValue=0;
			}
			SetRage(nValue);
		}
	}
	__LEAVE_FUNCTION
}
VOID Obj_Character::StrikePointIncrement(INT nIncrement, Obj_Character* pCharacter)
{
	__ENTER_FUNCTION
	if(TRUE==IsActiveObj()&&Obj::OBJ_TYPE_HUMAN == GetObjType())
	{
		
		INT nValue = GetStrikePoint() + nIncrement;
		GCDetailHealsAndDamages msg;
		msg.SetReceiverID(GetID());
		if(pCharacter)
		{
			msg.SetSenderID(pCharacter->GetID());
			msg.SetSenderLogicCount(pCharacter->GetLogicCount());
		}
		else
		{
			msg.SetSenderID(INVALID_ID);
			msg.SetSenderLogicCount(0);
		}
		msg.SetStrikePointModification(nIncrement);
		msg.SetCriticalHitFlag(FALSE);
		BroadCast_HealAndDamageMsg(this, pCharacter,msg);
		if(0<=nIncrement)
		{
			//OnHeal
			if(GetMaxStrikePoint()<nValue)
			{
				nValue = GetMaxStrikePoint();
			}
			SetStrikePoint(nValue);
		}
		else
		{   //OnDamage
			if(0>nValue)
			{
				nValue=0;
			}
			SetStrikePoint(nValue);
		}
	}
	__LEAVE_FUNCTION
}
VOID  Obj_Character::RefixSkill(SkillInfo_T& rSkill)
{
	__ENTER_FUNCTION
	Skill_RefixSkill(rSkill);
	Impact_RefixSkill(rSkill);
	__LEAVE_FUNCTION
}
VOID Obj_Character::RefixImpact(OWN_IMPACT& rImpactNeedRedix)
{
	__ENTER_FUNCTION
	return Impact_RefixImpact(rImpactNeedRedix);
	__LEAVE_FUNCTION
}
VOID Obj_Character::RefixRageRegeneration(INT& rRageRegeneration)
{
	__ENTER_FUNCTION
	return Impact_RefixRageRegeneration(rRageRegeneration);
	__LEAVE_FUNCTION
}

BOOL Obj_Character::IsEnemyCamp(Obj_Character& rTar)
{
__ENTER_FUNCTION

	Obj_Character *pOwner_A = GetOwner();
	Obj_Character *pOwner_B = rTar.GetOwner();
	if ( pOwner_A != NULL || pOwner_B != NULL )
	{
		if ( pOwner_A == &rTar
			|| pOwner_B == this
			|| pOwner_A == pOwner_B )
		{
			return FALSE;
		}
	}
	const _CAMP_DATA *pCamp_A, *pCamp_B;
	BOOL bHuman_A, bHuman_B;
	const CampAndStandDataMgr_T *pCampMgr;

	pCamp_A		= GetCampData();
	pCamp_B		= rTar.GetCampData();
	bHuman_A	= (GetObjType() == OBJ_TYPE_HUMAN);
	bHuman_B	= (rTar.GetObjType() == OBJ_TYPE_HUMAN);
	pCampMgr	= &g_CampAndStandDataMgr;

	if( pCamp_A==NULL || pCamp_B==NULL )
	{
		return FALSE;
	}
    //特殊阵营 :帮会模式 队伍模式 家族模式
    ENUM_RELATION eRelation;
    if (pCamp_A->m_nCampID > CAMP_NPC_END || pCamp_B->m_nCampID > CAMP_NPC_END)
    {
        eRelation = (ENUM_RELATION)CalRelationPlus(bHuman_A, bHuman_B, (Obj_Human*)pOwner_A, (Obj_Human*)pOwner_B, pCamp_A, pCamp_B);
    }
    else
    {
        eRelation = CalcRelationType( pCamp_A, bHuman_A, pCamp_B, bHuman_B, pCampMgr );
    }

	if ( eRelation == RELATION_FRIEND )
		return FALSE;
	else
		return TRUE;

	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}
BOOL Obj_Character::IsFriendCamp(Obj_Character& rTar)
{
__ENTER_FUNCTION
	return FALSE==IsEnemyCamp(rTar);
__LEAVE_FUNCTION
	return FALSE;
}
BOOL Obj_Character::CanUseSkillNow(SkillID_t nID)
{
	__ENTER_FUNCTION

	BOOL bCanUse = FALSE;
	// 角色身上是否拥有未完成的动作
	if(0>=GetActionTime() && GetGlobalActionDelegator().CanDoNextAction(*this))
	{
		bCanUse = TRUE;
	}

	if (FALSE==bCanUse && INVALID_ID!=nID)
	{
		Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nID);
		// 是瞬发技能
		if (pSkillTemplate && TRUE==pSkillTemplate->IsInterruptMove())
		{
			// 上一个技能可以被瞬发技能打断
			if (TRUE == TRUE/*rSkillInfo.GetStandFlag()*/)
			{
				// 中断上一个技能动作
				GetGlobalActionDelegator().InterruptCurrentAction(*this);
				bCanUse = TRUE;
			}
		}
	}
	return bCanUse;

	__LEAVE_FUNCTION
	return TRUE;
}
VOID	Obj_Character::Skill_ExecFromItemScript(SkillID_t const nID)
{
	__ENTER_FUNCTION
	SkillInfo_T& rSkillInfo = GetSkillInfo();
	TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();
	if(TRUE!=CanUseSkillNow(nID))
	{
		return;
		//InterruptSkillNow();
	}
	AI_Character* pAI = GetAIObj();
	if(NULL!=pAI)
	{
		pAI->Stop();
		rParams.SetIgnoreConditionCheckFlag(FALSE);
		ObjID_t nTargetID = rParams.GetTargetObj();
		WORLD_POS pos = rParams.GetTargetPosition();
		INT HurtDelayTime = Skill_GetSkillHurtDelayTime( nID );
		pAI->UseSkill(nID,nTargetID,pos.m_fX,pos.m_fZ,HurtDelayTime,-1.0f,INVALID_ID);	
	}
	__LEAVE_FUNCTION
	return;
}

VOID	Obj_Character::Skill_ExecFromScript(SkillID_t const nID, ObjID_t nTargetID, WORLD_POS& pos, FLOAT fDir, BOOL bPassSkillCheck)
{
	__ENTER_FUNCTION
	SkillInfo_T& rSkillInfo = GetSkillInfo();
	TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();
	if(TRUE!=CanUseSkillNow(nID))
	{
		//InterruptSkillNow();
	}
	AI_Character* pAI = GetAIObj();
	if(NULL!=pAI)
	{
		pAI->Stop();
		rParams.SetIgnoreConditionCheckFlag(bPassSkillCheck);
		INT HurtDelayTime = Skill_GetSkillHurtDelayTime( nID );
		pAI->UseSkill(nID,nTargetID,pos.m_fX,pos.m_fZ,HurtDelayTime,fDir, INVALID_ID);	
	}
	__LEAVE_FUNCTION
	return;
}

BOOL 	Obj_Character::Skill_IsSkillCooldowned(SkillID_t nID)
{
	//如果此技能需要冷却状态则判断是否已冷却，否则忽略冷却影响

__ENTER_FUNCTION
	SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	if(NULL==pSkillTemplate)
	{
		return FALSE;
	}
	//获取技能冷却ID
	CooldownID_t nCooldownID = (CooldownID_t)(pSkillTemplate->GetCooldownID());
	//如果是无效ID或者是自动连续释放的技能
	if(INVALID_ID==nCooldownID || pSkillTemplate->IsAutoShotSkill())
	{
		//如果冷却时间大于0证明还在冷却中
		if(0<GetAutoRepeatCooldown())
		{
			return FALSE;
		}
		//不存在冷却或已经冷却
		return TRUE; 
	}
	else if(FALSE==IsCooldowned(nCooldownID))
	{
		//冷却中
		return FALSE;
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL	Obj_Character::IsPrevCanViewMe( const Obj *pObj )
{
__ENTER_FUNCTION
	if(NULL==pObj)
	{
		return FALSE;
	}
	if(FALSE==IsCharacterObj(pObj->GetObjType()))
	{
		return FALSE;
	}
	if(TRUE==IsPartner((Obj_Character*)pObj))
	{
		return TRUE;
	}
	if(m_nView_PrevStealthLevel != 0)
	{
		Obj_Character * pCharacter = (Obj_Character*)pObj;
		BOOL bResult = CalcIsTargetStealth(pCharacter->GetPrevLevel(), pCharacter->GetPrevDetectLevel(), pCharacter->getWorldPos(),
			GetLevel(), GetStealthLevel(), getWorldPos());
		if(bResult)
		{
			return FALSE;
		}
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL	Obj_Character::IsCanViewMe( const Obj *pObj )
{
__ENTER_FUNCTION
	if(NULL==pObj)
	{
		return FALSE;
	}
	if(FALSE==IsCharacterObj(pObj->GetObjType()))
	{
		return FALSE;
	}
	if(TRUE==IsPartner((Obj_Character*)pObj))
	{
		return TRUE;
	}
	if(InStealthMode())
	{
		Obj_Character * pCharacter = (Obj_Character*)pObj;
		BOOL bResult = CalcIsTargetStealth(pCharacter->GetLevel(), pCharacter->GetDetectLevel(), pCharacter->getWorldPos(),
			GetLevel(), GetStealthLevel(), getWorldPos());
		if(bResult)
		{
			return FALSE;
		}
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}
BOOL	Obj_Character::InStealthMode(VOID)
{
__ENTER_FUNCTION
	return 0<GetStealthLevel();
__LEAVE_FUNCTION
	return FALSE;
}

BOOL Obj_Character::Skill_CanUseThisSkillInThisStatus(SkillID_t nSkillID)
{
	__ENTER_FUNCTION
	if(INVALID_ID==nSkillID)
	{
		return FALSE;
	}
	Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
	if(NULL==pSkillTemplate)
	{
		return FALSE;
	}
	// 技能是否受到标记1限制
	if(TRUE==pSkillTemplate->GetDisableByFlag1())
	{
		if(FALSE==CanActionFlag1())
		{
			return FALSE;
		}
	}
	// 技能是否受到标记2限制
	if(TRUE==pSkillTemplate->GetDisableByFlag2())
	{
		if(FALSE==CanActionFlag2())
		{
			return FALSE;
		}
	}
	// 技能是否受到标记3限制
	if(TRUE==pSkillTemplate->GetDisableByFlag3())
	{
		if(INVALID_ID != GetMountID())
		{
			return FALSE;
		}
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

Obj* Obj_Character::Skill_CreateObjSpecial(WORLD_POS const& rPosition, INT nDataID)
{
	__ENTER_FUNCTION
	TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();
	Obj_Special* pObjSpecial = NULL;
	Scene* pScene=getScene();
	if(NULL!=pScene)
	{
		pObjSpecial = (Obj_Special*)(pScene->NewObject(Obj::OBJ_TYPE_SPECIAL));
		if(NULL!=pObjSpecial)
		{
			_OBJ_SPECIAL_INIT initer;
			initer.m_nOwnerID = GetID();
			initer.m_nDataID = nDataID;
			initer.m_Pos = rPosition;
			pObjSpecial->Init(&initer);
		}
	}
	return pObjSpecial;
	__LEAVE_FUNCTION
	return NULL;
}
BOOL Obj_Character::Skill_RefixSkill(SkillInfo_T& rSkillInfo)
{
	__ENTER_FUNCTION
	if(Obj::OBJ_TYPE_MONSTER==GetObjType())
	{
		return TRUE;
	}
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		//获得角色身上的技能列表
		_SKILL_LIST const& rList = ((Obj_Human*)this)->Skill_GetSkillList();
		//技能数量
		BYTE const& rCount=rList.m_Count;
		SkillInfo_T PassiveSkill;
		for(INT i=0; rCount>i; ++i)
		{
			_OWN_SKILL const& rSkill = rList.m_aSkill[i];
			PassiveSkill.Init();
			if(TRUE==g_SkillCore.InstanceSkill(PassiveSkill, *this, rSkill.m_nSkillID))
			{
				SkillLogic_T const* pLogic = Skill_GetLogic(PassiveSkill);
				if(NULL!=pLogic)
				{
					pLogic->Refix_SkillInstance(PassiveSkill, rSkillInfo);
				}
			}
		}
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

_SkillMissMSGDelayTime* Obj_Character::GetDelayTimeSkillMissList( INT iIndex )
{
	__ENTER_FUNCTION

	if( iIndex<0 || iIndex>=MAX_DELAY_TIME_SKILL_MISS_NUM )
	{
		return NULL;
	}
	return &m_DelayTimeSkillMissList[iIndex];
	__LEAVE_FUNCTION
	return NULL;
}

VOID Obj_Character::AddDelayTimeSkillMissList( _SkillMissMSGDelayTime& val )
{
	__ENTER_FUNCTION

	for( INT i=0; i<MAX_DELAY_TIME_SKILL_MISS_NUM; ++i )
	{
		if( INVALID_ID == m_DelayTimeSkillMissList[i].m_nSkill )
		{
			m_DelayTimeSkillMissList[i] = val;
			m_DelayTimeSkillMissList[i].m_nDelayTime += (Time_t)g_pTimeManager->CurrentTime();
			break;
		}
	}
	__LEAVE_FUNCTION
}

VOID Obj_Character::DelayTimeSkillMissHeartBeat()
{
	__ENTER_FUNCTION

	for( INT i=0; i<MAX_DELAY_TIME_SKILL_MISS_NUM; ++i )
	{
		if( INVALID_ID == m_DelayTimeSkillMissList[i].m_nSkill )
		{
			continue;
		}

		if( (Time_t)g_pTimeManager->CurrentTime() >= m_DelayTimeSkillMissList[i].m_nDelayTime )
		{
			getScene()->GetEventCore().SendDelayTimeMissMSG
				( 
				GetID(), 
				m_DelayTimeSkillMissList[i].m_nSender,
				m_DelayTimeSkillMissList[i].m_nSkill,
				m_DelayTimeSkillMissList[i].m_nLogicCount 
				);

			Obj* pObj = getScene()->GetObjManager()->GetObj( m_DelayTimeSkillMissList[i].m_nSender );
			if( NULL == pObj )
			{
				m_DelayTimeSkillMissList[i].Cleanup();
				return;
			}
			if( !pObj->IsActiveObj() )
			{
				m_DelayTimeSkillMissList[i].Cleanup();
				return;
			}
			Obj_Character* pSender = NULL;
			if( pObj->GetObjType() == OBJ_TYPE_PET ||
				pObj->GetObjType() == OBJ_TYPE_HUMAN ||
				pObj->GetObjType() == OBJ_TYPE_MONSTER )
			{
				pSender = static_cast<Obj_Character*>(pObj);
				AI_Character *pAI = GetAIObj();

				if(NULL!=pAI)
				{
					pAI->OnDamage(0, pSender);
				}
			}
			m_DelayTimeSkillMissList[i].Cleanup();
		}
	}

	__LEAVE_FUNCTION
}

Obj_Character* Obj_Character::CreateGuard(UINT uDataID, INT nBaseAI /* = 0*/, INT nExtAI/* = 0*/)
{
	__ENTER_FUNCTION
	_OBJ_MONSTER_INIT	init;

	init.m_uDataID			=	uDataID;
	init.m_Pos.m_fX			=	getWorldPos()->m_fX - 3.f;
	init.m_Pos.m_fZ			=	getWorldPos()->m_fZ;
	init.m_RespawnTime		=	-1;
	init.m_BaseAI			=	nBaseAI;
	init.m_ExtAIScript		=   nExtAI;
	init.m_idScript			=	-1;

    Scene* pScene = getScene();
	if (!pScene) {
		return NULL;
	}

	pScene->GetMap()->VerifyPos(&(init.m_Pos) );
	if (FALSE == pScene->GetMap()->IsCanGo(init.m_Pos) )
	{
		init.m_Pos = *getWorldPos();
	}

	ObjID_t MonsterID = pScene->CreateTempMonster((const _OBJ_MONSTER_INIT*)&init);

	Obj* pObj = pScene->GetObjManager()->GetObj(MonsterID);
	if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		((Obj_Monster*)pObj)->SetBaseCampID(GetBaseCampID() );
		AI_Monster* pAI = (AI_Monster*)((Obj_Monster*)pObj)->GetAIObj();
		if (pAI)
		{
			pAI->SetSpeedyOfRecover(0.f);
		}
		return (Obj_Character*)pObj;
	}

	__LEAVE_FUNCTION
	return NULL;
}

static VOID BroadCast_HealAndDamageMsg(Obj_Character* pThis, Obj_Character* pTarget,GCDetailHealsAndDamages& rMsg)
{
	Obj_Human* pHuman = NULL;
	if(NULL!=pThis)
	{
		if(Obj::OBJ_TYPE_HUMAN==pThis->GetObjType())
		{
			//给玩家自己发消息	
			pHuman = (Obj_Human*)pThis;
			Player* pGamePlayer = pHuman->GetPlayer();
			if(NULL!=pGamePlayer)
			{
				pGamePlayer->SendPacket(&rMsg) ;
			}				
		}
		if(Obj::OBJ_TYPE_PET==pThis->GetObjType())
		{
			Obj_Human* pCreator = NULL;
			Obj_Pet* pPet = (Obj_Pet*)pThis;
			pCreator = pPet->GetCreator();
			if(pCreator != NULL)
			{
				Player* pGamePlayer = pCreator->GetPlayer();
				if(NULL!=pGamePlayer)
				{
					pGamePlayer->SendPacket(&rMsg) ;
				}
			}
		}
	}
						//不能重复给自己发送消息
	if(NULL!=pTarget && pThis != pTarget)
	{
		if(Obj::OBJ_TYPE_HUMAN==pTarget->GetObjType())
		{
			pHuman = (Obj_Human*)pTarget;
			Player* pGamePlayer = pHuman->GetPlayer();
			if(NULL!=pGamePlayer)
			{
				pGamePlayer->SendPacket(&rMsg) ;
			}				
		}
		if(Obj::OBJ_TYPE_PET==pTarget->GetObjType())
		{
			Obj_Human* pCreator = NULL;
			Obj_Pet* pPet = (Obj_Pet*)pTarget;
			pCreator = pPet->GetCreator();
			Player* pGamePlayer = pCreator->GetPlayer();
			if(NULL!=pGamePlayer)
			{
				pGamePlayer->SendPacket(&rMsg) ;
			}				
		}
	}
}

BOOL Obj_Character::IsBeatFly( Obj_Character& rMe, Skill_Module::SkillTemplateData_T const* pTemplate ) const
{
	__ENTER_FUNCTION
		if( pTemplate == NULL )
			return FALSE;

	FLOAT fBeatFlyRate = pTemplate->GetBeatFlyRate();
	return IsHappenByRate( fBeatFlyRate, rMe );

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL Obj_Character::IsBeatBack( Obj_Character& rMe, Skill_Module::SkillTemplateData_T const * pTemplate ) const
{
	__ENTER_FUNCTION
	if( pTemplate == NULL )
		return FALSE;

	//玩家不会被击退
	if( Obj::OBJ_TYPE_HUMAN == GetObjType() )
	{
		return FALSE;
	}

	FLOAT fBeatFlyRate = pTemplate->GetBeatBackRate();
	return IsHappenByRate( fBeatFlyRate, rMe );

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL Obj_Character::IsHappenByRate( FLOAT fRate, Obj_Character& rMe ) const
{
	__ENTER_FUNCTION

	Scene * pScene = rMe.getScene();

	if( fRate < 0.0000001f )
		return FALSE;

	if(NULL==pScene)
		return FALSE;

	FLOAT fRand = (FLOAT)(pScene->GetRand100());

	if( fRand <= fRate  )
		return TRUE;

	return FALSE;

	__LEAVE_FUNCTION
		return FALSE;

}

VOID Obj_Character::CalculateBeatBackPos( Obj_Character& rMe, Obj* rTag, SkillTemplateData_T const* pTemplate, WORLD_POS &sPos ) const
{
	__ENTER_FUNCTION

	FLOAT fDistance			= pTemplate->GetBeatBackDistance();
	WORLD_POS const* pPos	= rMe.getWorldPos();
	WORLD_POS sPosMe		= *pPos;
	pPos					= rTag->getWorldPos();
	WORLD_POS sPosTag		= *pPos;
	WORLD_POS sDir;
	sDir.m_fX = sPosTag.m_fX - sPosMe.m_fX;
	sDir.m_fZ = sPosTag.m_fZ - sPosMe.m_fZ;

	//获取方向
	NormalizeWorldPos( sDir );
	//最终希望到达的目标点
	WORLD_POS sFinalPos;
	sFinalPos.m_fX = sPosTag.m_fX + fDistance*sDir.m_fX;
	sFinalPos.m_fZ = sPosTag.m_fZ + fDistance*sDir.m_fZ;

	FLOAT fLengthOfStep = 0.5f;
	FLOAT fLengthTmp	= 0;
	WORLD_POS sPosTmp;
	//按照前进方向以格子边长(0.5)为步长向目标方向查询,遇障碍或到达目标点为止
	while( TRUE )
	{
		//比较当前位置和目标点的距离,如果小于步长则直接到达并结束
		fLengthTmp = WordPosLength( sFinalPos.m_fX-sPosTag.m_fX, sFinalPos.m_fZ-sPosTag.m_fZ );
		if( fLengthTmp < fLengthOfStep )
		{
			if ( rMe.getScene()->GetMap()->IsCanGo(sFinalPos) )
				sPosTag = sFinalPos;
			break;
		}
		//如果大于步长则增加一个步长的距离
		sPosTmp.m_fX = sPosTag.m_fX + sDir.m_fX*fLengthOfStep;
		sPosTmp.m_fZ = sPosTag.m_fZ + sDir.m_fZ*fLengthOfStep;
		//判断新位置是否可以通过
		if ( rMe.getScene()->GetMap()->IsCanGo(sPosTmp) )
			sPosTag = sPosTmp;
		//不可通过则结束
		else break;
	}
	sPos = sPosTag;
	__LEAVE_FUNCTION

}

VOID Obj_Character::NormalizeWorldPos( WORLD_POS& sPos ) const
{
	__ENTER_FUNCTION
		FLOAT fLength = WordPosLength( sPos.m_fX, sPos.m_fZ );
	sPos.m_fX /= fLength;
	sPos.m_fZ /= fLength;
	__LEAVE_FUNCTION
}

FLOAT Obj_Character::WordPosLength( FLOAT fX, FLOAT fY ) const
{
	__ENTER_FUNCTION
		FLOAT fLength = sqrt( pow(fX,2.0f) + pow(fY,2.0f) );
	return fLength;

	__LEAVE_FUNCTION
		return 0;
}

VOID Obj_Character::SubSkillAutoUseHeartBeat()
{
	//处理待发放的技能列表
	//获得角色身上的目标和消耗参数实例
	TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();
	for( INT i=0; i<SkillTemplateData_T::MAX_SUB_SKILL; ++i )
	{
		if( m_aSubSkill[i] == INVALID_ID )
		{
			break;
		}
		g_SkillCore.ProcessSkillRequest(	*this, 
			m_aSubSkill[i],
			rParams.GetTargetObj(), 
			rParams.GetTargetPosition(), 
			rParams.GetTargetDirection(), 
			rParams.GetTargetGuid(),
			TRUE
			);
		m_aSubSkill[i] = INVALID_ID;
	}
}

INT Obj_Character::CalRelationPlus(BOOL bHuman_A, BOOL bHuman_B, Obj_Human *pHumanA, Obj_Human *pHumanB, const _CAMP_DATA *pCamp_A, const _CAMP_DATA *pCamp_B)
{
    __ENTER_FUNCTION
    //特殊阵营 :帮会模式 队伍模式 家族模式， 只针对 Obj_Human
    if (bHuman_A)
    {
        if (pHumanA)
        {
            //A为玩家   B为非玩家    A为特殊阵营肯定是对立关系
            if (!bHuman_B)
                return RELATION_ENEMY;

            //B也为玩家
            if (pHumanB)
            {
                if (pCamp_A->m_nCampID  >=  CAMP_NPC_END)
                {
                    switch (pCamp_A->m_nCampID)
                    {
                    case CAMP_GUILD://帮会模式
                        {
                            GuildID_t   GuildId_A   =   pHumanA->GetGuildID();
                            GuildID_t   GuildId_B   =   pHumanB->GetGuildID();

                            //A有帮会
                            if (GuildId_A != INVALID_GUILD_ID)
                            {
                                if (GuildId_B != INVALID_GUILD_ID)
                                {//B也有帮会
                                    return (GuildId_A==GuildId_B)?RELATION_FRIEND:RELATION_ENEMY;
                                }
                                return RELATION_ENEMY;
                            }
                            return  RELATION_ENEMY;
                        }
                        break;
                    case CAMP_TEAM://队伍模式
                        {
                            TeamInfo *pInfoA    =   pHumanA->GetTeamInfo();
                            TeamInfo *pInfoB    =   pHumanB->GetTeamInfo();

                            if (pInfoA)
                            {
                                if (pInfoB)
                                {
                                    if (pInfoB->GetTeamID() && pInfoA->GetTeamID())
                                    {
                                        return RELATION_FRIEND;
                                    }
                                    else
                                        return RELATION_ENEMY;
                                }
                                return RELATION_ENEMY;
                            }
                            //自己不在队伍  所有人都是敌人
                            return RELATION_ENEMY;
                        }
                        break;
                    case CAMP_FAMILY://家族模式
                        {
                            GuildID_t   GuildId_A   =   pHumanA->GetGuildID();
                            GuildID_t   GuildId_B   =   pHumanB->GetGuildID();

                            //A有帮会
                            if (GuildId_A != INVALID_GUILD_ID)
                            {
                                if (GuildId_B != INVALID_GUILD_ID)
                                {//B也有帮会
                                    if (GuildId_A == GuildId_B)//同帮会才比较家族ID
                                    {
                                        INT ID_A    =   pHumanA->GetFamilyID();
                                        INT ID_B    =   pHumanB->GetFamilyID();
                                        return (ID_A==ID_B)?RELATION_FRIEND:RELATION_ENEMY;
                                    }
                                    return RELATION_ENEMY;
                                }
                                return RELATION_ENEMY;
                            }
                            return  RELATION_ENEMY;
                        }
                        break;
                    }
                }
            }
            return RELATION_ENEMY;
        }
    }

    if (bHuman_B)
    {
        if (pHumanB)
        {
            //B为玩家   A为非玩家    B为特殊阵营肯定是对立关系
            if (!bHuman_A)
                return RELATION_ENEMY;

            //A也为玩家
            if (pHumanA)
            {
                if (pCamp_B->m_nCampID  >=  CAMP_NPC_END)
                {
                    switch (pCamp_B->m_nCampID)
                    {
                    case CAMP_GUILD://帮会模式
                        {
                            GuildID_t   GuildId_B   =   pHumanB->GetGuildID();
                            GuildID_t   GuildId_A   =   pHumanA->GetGuildID();

                            //B有帮会
                            if (GuildId_B != INVALID_GUILD_ID)
                            {
                                if (GuildId_A != INVALID_GUILD_ID)
                                {//A也有帮会
                                    return (GuildId_A==GuildId_B)?RELATION_FRIEND:RELATION_ENEMY;
                                }
                                return RELATION_ENEMY;
                            }
                            return  RELATION_ENEMY;
                        }
                        break;
                    case CAMP_TEAM://队伍模式
                        {
                            TeamInfo *pInfoB    =   pHumanB->GetTeamInfo();
                            TeamInfo *pInfoA    =   pHumanA->GetTeamInfo();

                            if (pInfoB)
                            {
                                if (pInfoA)
                                {
                                    if (pInfoB->GetTeamID() && pInfoA->GetTeamID())
                                    {
                                        return RELATION_FRIEND;
                                    }
                                    else
                                        return RELATION_ENEMY;
                                }
                                return RELATION_ENEMY;
                            }
                            //自己不在队伍  所有人都是敌人
                            return RELATION_ENEMY;
                        }
                        break;
                    case CAMP_FAMILY://家族模式
                        {
                            GuildID_t   GuildId_B   =   pHumanB->GetGuildID();
                            GuildID_t   GuildId_A   =   pHumanA->GetGuildID();

                            //B有帮会
                            if (GuildId_B != INVALID_GUILD_ID)
                            {
                                if (GuildId_A != INVALID_GUILD_ID)
                                {//B也有帮会
                                    if (GuildId_A == GuildId_B)//同帮会才比较家族ID
                                    {
                                        INT ID_A    =   pHumanA->GetFamilyID();
                                        INT ID_B    =   pHumanB->GetFamilyID();
                                        return (ID_A==ID_B)?RELATION_FRIEND:RELATION_ENEMY;
                                    }
                                    return RELATION_ENEMY;
                                }
                                return RELATION_ENEMY;
                            }
                            return  RELATION_ENEMY;
                        }
                        break;
                    }
                }
            }
            return RELATION_ENEMY;
        }
    }
    __LEAVE_FUNCTION
    return RELATION_ENEMY;
}
/*// add by gh for souxia 技能属性(次数)改变
VOID Obj_Character::PetZhaoHuanTimesIncrement(INT nIncrement, INT iLogicCount, Obj_Character* pCharacter, SkillID_t skillId)
{
	__ENTER_FUNCTION
		if(TRUE==IsActiveObj())
		{
			INT nValue = GetPetZhaoHuanSkillTimes(skillId) + nIncrement;
			SetPetZhaoHuanSkillTimes(skillId, nValue);
		}
		__LEAVE_FUNCTION
}*/


