/********************************************************************
	created:	2008/05/05
	created:	5:5:2008   20:52
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact065.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact065
	file ext:	cpp
	author:		Richard
	
	purpose:	条件触发Impact
*********************************************************************/

#include "stdafx.h"
#include "StdImpact065.h"
#include "GameTable.h"
#include "Scene.h"
#include "AI_Character.h"
#include "AI_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact065_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if( !IsCanHappen(rImp, EffectTypeOnDamage, nSkillID, rMe) )
			{
				return;
			}
			
			SendImpactToMe(rImp, rMe);

			__LEAVE_FUNCTION
		}

		VOID StdImpact065_T::OnUseSkillSuccessfully( OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill ) const
		{
			__ENTER_FUNCTION
			if( !IsCanHappen(rImp, EffectTypeOnUseSkillSuccessfully, rSkill.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
			__LEAVE_FUNCTION
		}

		VOID StdImpact065_T::OnBeHit( OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker ) const
		{
			__ENTER_FUNCTION
			if( !IsCanHappen(rImp, EffectTypeOnBeHit, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
			__LEAVE_FUNCTION
		}

		VOID StdImpact065_T::OnBeMiss( OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker ) const
		{
			__ENTER_FUNCTION
			if( !IsCanHappen(rImp, EffectTypeOnBeMiss, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
			__LEAVE_FUNCTION
		}

		VOID StdImpact065_T::OnBeCriticalHit( OWN_IMPACT& rImp, Obj_Character & rMe ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnBeCriticalHit, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		VOID StdImpact065_T::OnBeHeal( OWN_IMPACT& rImp, Obj_Character& rMe, INT& nHealedHP ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnBeHeal, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		VOID StdImpact065_T::OnCriticalHitTarget( OWN_IMPACT & rImp, Obj_Character & rMe ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnCriticalHitTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		VOID StdImpact065_T::OnDamages( OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnDamages, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		VOID StdImpact065_T::OnDamageTarget( OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnDamageTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		VOID StdImpact065_T::OnDie( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnDie, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		VOID StdImpact065_T::OnFadeOut( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnFadeOut, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		VOID StdImpact065_T::OnHealTarget( OWN_IMPACT & rImp, Obj_Character & rMe, INT& rHealedHP ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnHealTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}



		VOID StdImpact065_T::OnHitTarget( OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnHitTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		VOID StdImpact065_T::OnMissTarget( OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar ) const
		{
			if( !IsCanHappen(rImp, EffectTypeOnMissTarget, rImp.GetSkillID(), rMe) )
			{
				return;
			}

			SendImpactToMe(rImp, rMe);
		}

		BOOL StdImpact065_T::IsHappend( Obj_Character &rMe, OWN_IMPACT& rImp , INT nIndex ) const
		{
			__ENTER_FUNCTION

			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return FALSE;
			}

			INT iRate = GetEffectRate( rImp , nIndex);
			INT nRand = pScene->GetRand100();
			if( nRand <= iRate )
			{
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION

				return FALSE;
		}
		BOOL StdImpact065_T::IsCanHappen( OWN_IMPACT &rImp, INT iEffectType, SkillID_t nSkillID, Obj_Character& rMe ) const
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

			//IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetEffectedSkillCollection(rImp));
			//if(NULL==pCollection)
			//{
			//	return FALSE;
			//}
			//if(FALSE==pCollection->IsThisIDInCollection(nSkillID))
			//{
			//	return FALSE;
			//}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact065_T::SendImpactToMe( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			//第一个技能是否可以被触发
			BOOL bFirstHappend = IsHappend(rMe, rImp, 0);

			for(INT nID = 0; nID < MAX_IMPACT_NUM_AT_SAME_TIME; nID++)
			{
				INT iImpactID = GetImpactID(rImp, nID);
				if(INVALID_ID == iImpactID)
				{
					break;
				}
				if(0 == nID)
				{
					//如果第一个技能没有概率触发
					if(!bFirstHappend)
					{
						continue;
					}
				}
				else
				{
					if(INVALID_ID == GetEffectRate( rImp , nID))
					{
						//其它的impact的概率值写-1，认为应该取第一项计算出的概率值
						if(!bFirstHappend)
						{
							continue;
						}
					}
					else
					{
						//单独计算其概率值
						if(!IsHappend(rMe, rImp, nID))
						{
							continue;
						}
					}
				}
				
				INT iTargetType = GetTargetType( rImp, nID );
				if( iTargetType == TargetTypeMe )
				{
					g_ImpactCore.SendImpactToUnit(rMe, iImpactID, rMe.GetID(), 0);
				}
				else
				{
					Obj_Character* pAttacker = GetBeHitTarget( rMe, rImp );
					if(pAttacker)
					{
						g_ImpactCore.SendImpactToUnit(*pAttacker, iImpactID, rMe.GetID(), 0);
					}
				}
			}
		}

		Obj_Character* StdImpact065_T::GetAttackerTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const
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

		Obj_Character* StdImpact065_T::GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const
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




