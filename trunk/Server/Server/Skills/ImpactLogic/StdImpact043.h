///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact043.h
// ����Ա������
// ����˵����Ч��--����Ŀ��ʱ�����Լ���ŭ����������һ��ʱ���ڷ���������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT043_H
#define STDIMPACT043_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact043_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_043,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_RAGE_REGENERATE_REFIX = 0,
							IDX_RESIST_GOLD_REFIX,
							IDX_RESIST_WOOD_REFIX,
							IDX_RESIST_WATER_REFIX,
							IDX_RESIST_FIRE_REFIX,
							IDX_RESIST_SOIL_REFIX,
						};
				};
				StdImpact043_T() {};
				~StdImpact043_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetRageRegenerateRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RAGE_REGENERATE_REFIX,
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
				VOID RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const;
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
			protected:
			private:
		} SOT_MingJiao013_T;
	};
};
#endif //STDIMPACT043_H
