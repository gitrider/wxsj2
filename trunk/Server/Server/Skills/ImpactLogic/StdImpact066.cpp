/********************************************************************
	created:	2008/05/05
	created:	5:5:2008   20:52
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact065.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact065
	file ext:	cpp
	author:		Richard
	
	purpose:	条件触发技能
*********************************************************************/

#include "stdafx.h"
#include "StdImpact066.h"
#include "GameTable.h"
#include "Scene.h"
#include "AI_Character.h"
#include "AI_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact066_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if( NULL == pAttacker)
			{
				return;
			}
			if( !IsCanHappen(rImp, EffectTypeOnDamage, nSkillID, rMe) )
			{
				return;
			}
			
			SendSkillToTarget(rImp, rMe, *pAttacker );

			__LEAVE_FUNCTION
		}

		VOID StdImpact066_T::OnUseSkillSuccessfully( OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill ) const
		{
			__ENTER_FUNCTION
			if( !IsCanHappen(rImp, EffectTypeOnUseSkillSuccessfully, rSkill.GetSkillID(), rMe) )
			{
				return;
			}
			
			SendSkillToTarget(rImp, rMe, *GetBeHitTarget( rMe, rImp ) );
			__LEAVE_FUNCTION
		}

		VOID StdImpact066_T::OnBeHit( OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker ) const
		{
			__ENTER_FUNCTION
			if( !IsCanHappen(rImp, EffectTypeOnBeHit, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, rAttacker );
			__LEAVE_FUNCTION
		}

		VOID StdImpact066_T::OnBeMiss( OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker ) const
		{
			__ENTER_FUNCTION
			if( !IsCanHappen(rImp, EffectTypeOnBeHit, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, rAttacker );
			__LEAVE_FUNCTION
		}


		VOID StdImpact066_T::OnBeCriticalHit( OWN_IMPACT& rImp, Obj_Character & rMe ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnBeCriticalHit, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, *GetAttackerTarget( rMe, rImp ) );
		}

		VOID StdImpact066_T::OnBeHeal( OWN_IMPACT& rImp, Obj_Character& rMe, INT& nHealedHP ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnBeHeal, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, *GetAttackerTarget( rMe, rImp ) );
		}

		VOID StdImpact066_T::OnCriticalHitTarget( OWN_IMPACT & rImp, Obj_Character & rMe ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnCriticalHitTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, *GetBeHitTarget( rMe, rImp ) );
		}

		VOID StdImpact066_T::OnDamages( OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID ) const
		{
			if( NULL == pAttacker)
			{
				return;
			}
			if( !IsCanHappen(rImp, EffectTypeOnDamages, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, *pAttacker );
		}

		VOID StdImpact066_T::OnDamageTarget( OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnDamageTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, rTar );
		}

		VOID StdImpact066_T::OnDie( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnDie, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, *GetAttackerTarget( rMe, rImp ) );
		}

		VOID StdImpact066_T::OnFadeOut( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnFadeOut, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, *GetAttackerTarget( rMe, rImp ) );
		}

		VOID StdImpact066_T::OnHealTarget( OWN_IMPACT & rImp, Obj_Character & rMe, INT& rHealedHP ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnHealTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, *GetBeHitTarget( rMe, rImp ) );
		}



		VOID StdImpact066_T::OnHitTarget( OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnHitTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, rTar );
		}

		VOID StdImpact066_T::OnMissTarget( OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnHitTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendSkillToTarget(rImp, rMe, rTar );
		}


		BOOL StdImpact066_T::IsHappend( Obj_Character &rMe, OWN_IMPACT& rImp ) const
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
		BOOL StdImpact066_T::IsCanHappen( OWN_IMPACT &rImp, INT iEffectType, SkillID_t nSkillID, Obj_Character& rMe ) const
		{
			__ENTER_FUNCTION

			if(TRUE==rImp.IsFadeOut())
			{
				return FALSE;
			}
			
			if( iEffectType != GetEffectType( rImp ) )
			{
				return FALSE;
			}


			if( !IsHappend(rMe, rImp) )
			{
				return FALSE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact066_T::SendSkillToTarget( OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rAttacker ) const
		{
			__ENTER_FUNCTION

			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
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
				iTargetID	= rAttacker.GetID();
				pPos		= rAttacker.getWorldPos();
				//iGuid		= pChar->GetGUID();
			}
			INT HurtDelayTime = Skill_GetSkillHurtDelayTime( GetSkillID(rImp) );
			if( rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN )
			{
				AI_Human* pAIHuman = (AI_Human*)pAI;
				const SkillTemplateData_T* pSkillTemplate =  g_SkillTemplateDataMgr.GetInstanceByID( GetSkillID(rImp) );
				pAIHuman->PushCommand_UseSkill( GetSkillID(rImp), iTargetID, pPos->m_fX, pPos->m_fZ, rMe.getDir(), iGuid, HurtDelayTime );
			}
			else
			{
				pAI->UseSkill( GetSkillID(rImp), iTargetID, pPos->m_fX, pPos->m_fZ, HurtDelayTime, rMe.getDir(), iGuid);
			}
			__LEAVE_FUNCTION

		}
		Obj_Character* StdImpact066_T::GetAttackerTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION

			Scene* pScene = rMe.getScene();
			if( pScene == NULL )
			{
				return NULL;
			}
			INT iTargetID = rImp.GetCasterObjID();
			Obj_Character* pChar = (Obj_Character*)pScene->GetObjManager()->GetObj( iTargetID );
			return pChar;
			__LEAVE_FUNCTION
			return NULL;

		}

		Obj_Character* StdImpact066_T::GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION

			Scene* pScene = rMe.getScene();
			if( pScene == NULL )
			{
				return NULL;
			}
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Obj_Character* pChar = (Obj_Character*)pScene->GetObjManager()->GetObj( rParams.GetTargetObj() );
			return pChar;
			__LEAVE_FUNCTION
			return NULL;
		}	
	};
};




