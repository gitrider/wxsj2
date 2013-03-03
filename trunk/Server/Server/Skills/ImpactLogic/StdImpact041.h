///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact041.h
// 程序员：高骐
// 功能说明：效果--明教的五星连珠的狂暴效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT041_H
#define STDIMPACT041_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact041_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_041,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							//IDX_ATTACK_PHY_REFIX = 0,
							IDX_ATTACK_NEAR_REFIX = 0,
							//IDX_ATTACK_FAR_REFIX,

							//IDX_DEFENCE_PHY_REFIX,
							IDX_DEFENCE_NEAR_REFIX,
							//IDX_DEFENCE_FAR_REFIX,

							IDX_ATTACK_MAGIC_NEAR_REFIX_NEAR,
							IDX_DEFENCE_MAGIC_NEAR_REFIX_NEAR,
							IDX_RAGE_REGENERATE_REFIX,
						};
				};
				StdImpact041_T() {};
				~StdImpact041_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetAttackNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_NEAR_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetDefenceNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_NEAR_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackMagicNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(
						rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_MAGIC_NEAR_REFIX_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetDefenceMagicNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_MAGIC_NEAR_REFIX_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetRageRegenerateRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(
						rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RAGE_REGENERATE_REFIX,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				VOID RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const;
			protected:
			private:

		} SOT_MingJiao006_T;
	};
};
#endif //STDIMPACT041_H
