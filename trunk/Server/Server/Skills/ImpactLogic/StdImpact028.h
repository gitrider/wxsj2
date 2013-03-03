///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact028.h
// ����Ա��frankwu
// ����˵����
//
// Ч���������ֵĸ�ʽ��|Ч��ID|����ʱ��|����������|ľ��������|����������|����������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT028_H
#define STDIMPACT028_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact028_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_028,
				};
				class OwnImpactParamIndex_T
				{
					public:
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_GOLD_REFIX,
							IDX_WOOD_REFIX,
							IDX_WATER_REFIX,
							IDX_FIRE_REFIX,
							IDX_SOIL_REFIX,
						};
				};

				StdImpact028_T() {};
				~StdImpact028_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				
				INT GetGoldRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_GOLD_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetWoodRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_WOOD_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetWaterRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_WATER_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetFireRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_FIRE_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetSoilRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SOIL_REFIX,
						rImp.GetSkillLevel()
						);
				};

			public: //message handler
				virtual VOID OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const;
			protected:
			private:
		} DS_PetTrait_T;
	};
};
#endif //STDIMPACT028_H