/********************************************************************
	created:	2008/05/05
	created:	5:5:2008   20:26
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact065.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact065
	file ext:	h
	author:		Richard
	
	purpose:	条件触发Impact
*********************************************************************/

#ifndef StdImpact065_h__
#define StdImpact065_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact065_T : public ImpactLogic_T
		{
		public:
			enum
			{
				ID = STD_IMPACT_065,
			};

			enum 
			{
				TargetTypeMe	= 0,
				TargetTypeOther
			};

			enum
			{
				EffectTypeOnBeCriticalHit = 0,
				EffectTypeOnBeHeal,
				EffectTypeOnCriticalHitTarget,
				EffectTypeOnDamages,
				EffectTypeOnDamage,
				EffectTypeOnDamageTarget,
				EffectTypeOnDie,
				EffectTypeOnFadeOut,
				EffectTypeOnHealTarget,
				EffectTypeOnBeHit,
				EffectTypeOnHitTarget,
				EffectTypeOnIntervalOver,
				EffectTypeOnUseSkillSuccessfully,
				EffectTypeOnBeMiss,
				EffectTypeOnMissTarget
			};
			enum
			{
				MAX_IMPACT_NUM_AT_SAME_TIME = 5,		//同时可以激发的impact数量
			};

			class ImpactDataDescriptorIndex_T
			{
			public:
				enum
				{
					IDX_EFFECT_TYPE = 0,				//0 OnSkillUseSuccess, 1 OnBeHit
					IDX_EFFECT_RATE,
					IDX_TARGET_TYPE,				//0：自己 1：对方
					IDX_IMPACT_ID,
				};
			};

			StdImpact065_T() {};
			~StdImpact065_T() {};
			BOOL IsOverTimed(VOID) const {return TRUE;};
			BOOL IsIntervaled(VOID) const {return FALSE;};

			INT GetEffectType(OWN_IMPACT const& rImp) const
			{
				return Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					ImpactDataDescriptorIndex_T::IDX_EFFECT_TYPE,
					rImp.GetSkillLevel()
					);
			}

			INT GetEffectRate(OWN_IMPACT const& rImp, INT nIndex) const
			{
				return Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					nIndex * 3 + ImpactDataDescriptorIndex_T::IDX_EFFECT_RATE,
					rImp.GetSkillLevel()
					);
			}

			INT GetTargetType(OWN_IMPACT const& rImp, INT nIndex) const
			{
				return Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					nIndex * 3 + ImpactDataDescriptorIndex_T::IDX_TARGET_TYPE,
					rImp.GetSkillLevel()
					);
			}

			INT GetImpactID(OWN_IMPACT const& rImp, INT nIndex) const
			{
				return Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					nIndex * 3 + ImpactDataDescriptorIndex_T::IDX_IMPACT_ID,
					rImp.GetSkillLevel()
					);
			}

		public: //message handler
			virtual VOID OnBeCriticalHit(OWN_IMPACT& rImp, Obj_Character & rMe) const;
			virtual VOID OnBeHeal(OWN_IMPACT& rImp, Obj_Character& rMe, INT& nHealedHP)const;
			virtual VOID OnCriticalHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe) const;
			virtual VOID OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const;
			virtual VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const;

			virtual VOID OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const;
			virtual VOID OnDie(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			virtual VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			virtual VOID OnHealTarget(OWN_IMPACT & rImp, Obj_Character & rMe, INT& rHealedHP) const;
			virtual VOID OnBeHit(OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker) const;
			virtual VOID OnHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar) const;
			virtual VOID OnBeMiss(OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker) const;
			virtual VOID OnMissTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar) const;
			virtual VOID OnUseSkillSuccessfully(OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill) const;
		protected:
			BOOL IsHappend( Obj_Character &rMe, OWN_IMPACT& rImp, INT nIndex ) const;
			BOOL IsCanHappen( OWN_IMPACT &rImp, INT iEffectType, SkillID_t nSkillID, Obj_Character& rMe ) const;
			VOID SendImpactToMe( OWN_IMPACT& rImp, Obj_Character& rMe ) const;

			//目标是对我攻击的角色
			Obj_Character* GetAttackerTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const;

			//目标是被窝攻击的角色
			Obj_Character* GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const;
		private:
		} ;
	};
};

#endif // StdImpact065_h__
