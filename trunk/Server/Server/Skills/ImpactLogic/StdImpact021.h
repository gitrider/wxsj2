///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact021.h
// 程序员：高骐
// 功能说明：效果--提升下n次技能的物理攻击力
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT021_H
#define STDIMPACT021_H

#include "ImpactLogic.h"
#include "SkillInfo.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact021_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_021,
				};
				enum
				{
					COLLECTION_NUMBER = 2,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_TIMES,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_TIMES =0,
							IDX_TARGET_SKILL_COLLECTION,
							IDX_HIT_REFIX,
							IDX_CRITICAL_REFIX,
							IDX_POWER_REFIX_BY_VALUE,
							IDX_POWER_REFIX_BY_RATE,
							IDX_DEPLETE_REFIX_BY_VALUE,
							IDX_DEPLETE_REFIX_BY_RATE,
							IDX_TIME_REFIX_BY_VALUE,
							IDX_TIME_REFIX_BY_RATE,
							IDX_COOLDOWN_REFIX_BY_VALUE,
							IDX_COOLDOWN_REFIX_BY_RATE,
							IDX_CHARGE_REFIX_BY_VALUE,
							IDX_CHARGE_REFIX_BY_RATE,
							IDX_CHANNEL_REFIX_BY_VALUE,
							IDX_CHANNEL_REFIX_BY_RATE,
						};
				};
				StdImpact021_T() {};
				~StdImpact021_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetActivateTimes(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_TIMES);};
				VOID SetActivateTimes(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_TIMES, nValue);};

				INT GetActivateTimesInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ACTIVATE_TIMES,
						rImp.GetSkillLevel()
						);
				};
				INT GetTargetCollection(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TARGET_SKILL_COLLECTION,
						rImp.GetSkillLevel()
						);
				};

				INT GetHitRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_HIT_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetCriticalRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CRITICAL_REFIX,
						rImp.GetSkillLevel());
				};

				INT GetPowerRefixByValue(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_POWER_REFIX_BY_VALUE,
						rImp.GetSkillLevel()
						);
				};
				INT GetPowerRefixByRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_POWER_REFIX_BY_RATE,
						rImp.GetSkillLevel()
						);
				};

				INT GetDepleteRefixByValue(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEPLETE_REFIX_BY_VALUE,
						rImp.GetSkillLevel()
						);
				};
				INT GetDepleteRefixByRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEPLETE_REFIX_BY_RATE,
						rImp.GetSkillLevel()
						);
				};

				INT GetTimeRefixByValue(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TIME_REFIX_BY_VALUE,
						rImp.GetSkillLevel()
						);
				};
				INT GetTimeRefixByRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TIME_REFIX_BY_RATE,
						rImp.GetSkillLevel()
						);
				};

				INT GetCooldownRefixByValue(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_COOLDOWN_REFIX_BY_VALUE,
						rImp.GetSkillLevel()
						);
				};
				INT GetCooldownRefixByRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_COOLDOWN_REFIX_BY_RATE,
						rImp.GetSkillLevel()
						);
				};

				INT GetChargeRefixByValue(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CHARGE_REFIX_BY_VALUE,
						rImp.GetSkillLevel()
						);
				};
				INT GetChargeRefixByRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CHARGE_REFIX_BY_RATE,
						rImp.GetSkillLevel()
						);
				};

				INT GetChannelRefixByValue(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CHANNEL_REFIX_BY_VALUE,
						rImp.GetSkillLevel()
						);
				};
				INT GetChannelRefixByRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CHANNEL_REFIX_BY_RATE,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID RefixSkill(OWN_IMPACT & rImp, SkillInfo_T & rSkill) const;
				VOID OnUseSkillSuccessfully(OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill) const;
			protected:
			private:
		};
	};
};
#endif //STDIMPACT021_H
