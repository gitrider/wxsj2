///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact046.h
// ����Ա������
// ����˵����Ч��--������007��Ч��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT046_H
#define STDIMPACT046_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact046_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_046,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_EFFECTED_SKILL_COLLECTION=0,
							IDX_POWER_REFIX_RATE,
							IDX_RAGE_CONVERT_RATE,
						};
				};

				StdImpact046_T() {};
				~StdImpact046_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetEffectedSkillCollection(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_EFFECTED_SKILL_COLLECTION,
						rImp.GetSkillLevel()
						);
				};
				INT GetPowerRefixRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_POWER_REFIX_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetRageConvertRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RAGE_CONVERT_RATE,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const;
			protected:
			private:
		} SOT_Dali007_T;
	};
};
#endif //DS_REFLECT_H
