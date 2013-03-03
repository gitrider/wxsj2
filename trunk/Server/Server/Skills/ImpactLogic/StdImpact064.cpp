/********************************************************************
	created:	2008/05/06
	created:	6:5:2008   17:02
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact064.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact064
	file ext:	cpp
	author:		Richard
	
	purpose:	技能触发技能
*********************************************************************/

#include "stdafx.h"
#include "StdImpact064.h"
#include "GameTable.h"
#include "Scene.h"
#include "AI_Character.h"
#include "AI_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact064_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			return;
			__LEAVE_FUNCTION
		}

		VOID StdImpact064_T::OnUseSkillSuccessfully( OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill ) const
		{
			__ENTER_FUNCTION
			//0代表响应此消息
			if( EffectTypeOnSkillSuccess != GetEffectType( rImp ) )
			{
				return;
			}
			if( !IsHappend(rMe, rImp) )
			{
				return;
			}
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Obj_Character* pChar = (Obj_Character*)pScene->GetObjManager()->GetObj( rParams.GetTargetObj() );
			if(NULL==pChar)
			{
				return;
			}
			if(INVALID_ID==rSkill.GetSkillID())
			{
				return;
			}
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetEffectedSkillCollection(rImp));
			if(NULL==pCollection)
			{
				return;
			}
			if(FALSE==pCollection->IsThisIDInCollection(rSkill.GetSkillID()))
			{
				return;
			}
			INT iTargetType = GetTargetType( rImp );
			AI_Character* pAI = rMe.GetAIObj();
			if( pAI == NULL )
			{
				return;
			}
			ObjID_t iTargetID		= INVALID_ID;
			const WORLD_POS* pPos	= NULL;
			GUID_t iGuid			= INVALID_ID;
			if( iTargetType == TargetTypeMe )
			{
				iTargetID	= rMe.GetID();
				pPos		= rMe.getWorldPos();
				//iGuid		= rMe.GetGUID();
			}
			else
			{
				iTargetID	= rParams.GetTargetObj();
				pPos		= pChar->getWorldPos();
				//iGuid		= pChar->GetGUID();
			}
			INT HurtDelayTime = Skill_GetSkillHurtDelayTime( GetSkillID(rImp) );
			if( rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN )
			{
				AI_Human* pAIHuman = (AI_Human*)pAI;
				pAIHuman->PushCommand_UseSkill( GetSkillID(rImp), iTargetID, pPos->m_fX, pPos->m_fZ, rMe.getDir(), iGuid, HurtDelayTime );
			}
			else
			{
				pAI->UseSkill( GetSkillID(rImp), iTargetID, pPos->m_fX, pPos->m_fZ, HurtDelayTime, rMe.getDir(), iGuid);
			}
			__LEAVE_FUNCTION
		}

		VOID StdImpact064_T::OnBeHit( OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker ) const
		{
			__ENTER_FUNCTION
			//1代表响应此消息
			if( EffectTypeOnBeHit != GetEffectType( rImp ) )
			{
				return;
			}
			if( !IsHappend(rMe, rImp) )
			{
				return;
			}
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			SkillInfo_T& rSkill = rAttacker.GetSkillInfo();
			if(INVALID_ID==rSkill.GetSkillID())
			{
				return;
			}
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetEffectedSkillCollection(rImp));
			if(NULL==pCollection)
			{
				return;
			}
			if(FALSE==pCollection->IsThisIDInCollection(rSkill.GetSkillID()))
			{
				return;
			}


			INT iTargetType = GetTargetType( rImp );
			AI_Character* pAI = rMe.GetAIObj();
			if( pAI == NULL )
			{
				return;
			}
			ObjID_t iTargetID		= INVALID_ID;
			const WORLD_POS* pPos	= NULL;
			GUID_t iGuid			= 0;
			if( iTargetType == TargetTypeMe )
			{
				iTargetID	= rMe.GetID();
				pPos		= rMe.getWorldPos();
				//iGuid		= rMe.GetGUID();
			}
			else
			{
				iTargetID	= rAttacker.GetID();
				pPos		= rAttacker.getWorldPos();
				//iGuid		= rAttacker.GetGUID();
			}
			INT HurtDelayTime = Skill_GetSkillHurtDelayTime( GetSkillID(rImp) );
			if( rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN )
			{
				AI_Human* pAIHuman = (AI_Human*)pAI;
				pAIHuman->PushCommand_UseSkill( GetSkillID(rImp), iTargetID, pPos->m_fX, pPos->m_fZ, rMe.getDir(), iGuid, HurtDelayTime );
			}
			else
			{
				pAI->UseSkill( GetSkillID(rImp), iTargetID, pPos->m_fX, pPos->m_fZ, HurtDelayTime, rMe.getDir(), iGuid);
			}

			__LEAVE_FUNCTION
		}

		BOOL StdImpact064_T::IsHappend( Obj_Character &rMe, OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION

			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return FALSE;
			}
			INT iRate = GetEffectRate( rImp );
			INT nRand = pScene->GetRand100();
			if( nRand <= iRate )
			{
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION

			return FALSE;
		}
	};
};

