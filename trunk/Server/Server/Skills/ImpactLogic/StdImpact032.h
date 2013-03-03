///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact032.h
// 程序员：高骐
// 功能说明：效果--武当技能相濡以沫的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT032_H
#define STDIMPACT032_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact032_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_032,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_SNARE_ACTIVATE_COOLDOWN_ELAPSED =0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_SNARE_ACTIVATE_ODDS = 0,
							IDX_SNARE_IMPACT_DATA_INDEX,
							IDX_SNARE_ACTIVATE_COOLDOWN,
							IDX_SCAN_RADIUS,
							IDX_EFFECTED_OBJ_COUNT,
							IDX_FREEZE_IMPACT_DATA_INDEX,
							//IDX_DEFENCE_PHYSICS_REFIX,
							IDX_DEFENCE_NEAR_REFIX,

							IDX_DEFENCE_MAGIC_NEAR_REFIX,
							IDX_RESIST_GOLD_REFIX,
							IDX_RESIST_WOOD_REFIX,
							IDX_RESIST_WATER_REFIX,
							IDX_RESIST_FIRE_REFIX,
							IDX_RESIST_SOIL_REFIX,
						};
				};
				StdImpact032_T() {};
				~StdImpact032_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetSnareActivateCooldownElapsed(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_SNARE_ACTIVATE_COOLDOWN_ELAPSED);};
				VOID SetSnareActivateCooldownElapsed(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_SNARE_ACTIVATE_COOLDOWN_ELAPSED, nValue);};

				INT GetSnareActivateOdds(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SNARE_ACTIVATE_ODDS,
						rImp.GetSkillLevel()
						);
				};
				INT GetSnareImpactDataIndex(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SNARE_IMPACT_DATA_INDEX,
						rImp.GetSkillLevel()
						);
				};
				INT GetSnareActivateCooldown(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SNARE_ACTIVATE_COOLDOWN,
						rImp.GetSkillLevel()
						);
				};
				INT GetScanRadius(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SCAN_RADIUS,
						rImp.GetSkillLevel()
						);
				};
				INT GetEffectedObjCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_EFFECTED_OBJ_COUNT,
						rImp.GetSkillLevel()
						);
				};
				INT GetFreezeImpactDataIndex(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_FREEZE_IMPACT_DATA_INDEX,
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
				INT GetDefenceMagicNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_MAGIC_NEAR_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetResistGoldRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_GOLD_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetResistWoodRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_WOOD_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetResistWaterRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_WATER_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetResistFireRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_FIRE_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetResistSoilRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_SOIL_REFIX,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				VOID MarkModifiedAttrDirty(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const;
			protected:
			private:
		} DS_WuDang010_T;
	};
};
#endif //STDIMPACT032_H
