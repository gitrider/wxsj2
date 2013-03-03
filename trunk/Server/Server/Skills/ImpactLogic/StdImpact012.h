///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact012.h
// 程序员：高骐
// 功能说明：效果--State: refix all attack, Defence and MaxHP, MaxMP, MaxRage, MaxStrikePoint
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT012_H
#define STDIMPACT012_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact012_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_012,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_RATE = 0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ATTACK_NEAR = 0,
							IDX_DEFENCE_NEAR,
							IDX_ATTACK_FAR,
							IDX_DEFENCE_FAR,
							IDX_ATTACK_MAGIC_NEAR,
							IDX_DEFENCE_MAGIC_NEAR,
							IDX_ATTACK_MAGIC_FAR,
							IDX_DEFENCE_MAGIC_FAR,

							IDX_ATTACK_GOLD,
							IDX_RESIST_GOLD,

							IDX_ATTACK_WOOD,
							IDX_RESIST_WOOD,

							IDX_ATTACK_WATER,
							IDX_RESIST_WATER,

							IDX_ATTACK_FIRE,
							IDX_RESIST_FIRE,

							IDX_ATTACK_SOIL,
							IDX_RESIST_SOIL,
						};
				};
				StdImpact012_T() {};
				~StdImpact012_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetAttackNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackFarRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_FAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackMagicNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_MAGIC_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackMagicFarRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_MAGIC_FAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackGoldRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_GOLD,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackWoodRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_WOOD,
						rImp.GetSkillLevel()
						);
				};

				INT GetAttackWaterRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_WATER,
						rImp.GetSkillLevel()
						);
				};


				INT GetAttackFireRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_FIRE,
						rImp.GetSkillLevel()
						);
				};

				INT GetAttackSoilRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_SOIL,
						rImp.GetSkillLevel()
						);
				};

				INT GetDefenceNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetDefenceFarRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_FAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetDefenceMagicNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_MAGIC_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetDefenceMagicFarRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_MAGIC_FAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetResistGoldRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_GOLD,
						rImp.GetSkillLevel()
						);
				};
				INT GetResistWoodRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_WOOD,
						rImp.GetSkillLevel()
						);
				};
				
				INT GetResistWaterRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_WATER,
						rImp.GetSkillLevel());
				};

				INT GetResistFireRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_FIRE,
						rImp.GetSkillLevel()
						);
				};

				INT GetResistSoilRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RESIST_SOIL,
						rImp.GetSkillLevel()
						);
				};

			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
				
		} RefixAttrLevel2ByRate_AttacksDefencesAndMaxs_T;
	};
};
#endif //STDIMPACT012_H
