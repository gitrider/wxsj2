/********************************************************************
	created:	2008/05/05
	created:	5:5:2008   18:42
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact064.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact064
	file ext:	h
	author:		Richard
	
	purpose:	技能触发技能
*********************************************************************/

#ifndef StdImpact064_h__
#define StdImpact064_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact064_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_064,
				};
				enum 
				{
					TargetTypeMe	= 0,
					TargetTypeOther
				};
				enum
				{
					EffectTypeOnSkillSuccess = 0,
					EffectTypeOnBeHit
				};

				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_EFFECT_RATE = 0,
							IDX_EFFECTED_SKILL_COLLECTION,
							IDX_EFFECT_TYPE,				//0 OnSkillUseSuccess, 1 OnBeHit
							IDX_TARGET_TYPE,				//0 Self, 1 other
							IDX_SKILL_ID,
						};
				};

				StdImpact064_T() {};
				~StdImpact064_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetEffectRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_EFFECT_RATE,
						rImp.GetSkillLevel()
						);
				}

				INT GetEffectedSkillCollection(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_EFFECTED_SKILL_COLLECTION,
						rImp.GetSkillLevel()
						);
				}

				INT GetEffectType(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_EFFECT_TYPE,
						rImp.GetSkillLevel()
						);
				}

				INT GetTargetType(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TARGET_TYPE,
						rImp.GetSkillLevel()
						);
				}

				INT GetSkillID(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SKILL_ID,
						rImp.GetSkillLevel()
						);
				}

			public: //message handler
				virtual VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const;


				virtual VOID OnUseSkillSuccessfully(OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill) const;
				virtual VOID OnBeHit(OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker) const;
			protected:
				BOOL IsHappend( Obj_Character &rMe, OWN_IMPACT& rImp ) const;
			private:
		} ;
	};
};

#endif // StdImpact064_h__