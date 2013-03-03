///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact049.h
// ����Ա������
// ����˵����Ч��--��ң������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT049_H
#define STDIMPACT049_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact049_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_049,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_EFFECTED_SKILL_COLLECTION = 0,
							IDX_ACTIVATE_ODDS,
							IDX_SUB_IMPACT,
						};
				};

				StdImpact049_T() {};
				~StdImpact049_T() {};
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
				INT GetActivateOdds(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ACTIVATE_ODDS,
						rImp.GetSkillLevel()
						);
				};
				INT GetSubImpact(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const;
			protected:
			private:
		} SOT_TianShan011_T;
	};
};
#endif //STDIMPACT049_H
