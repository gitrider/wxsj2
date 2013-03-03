///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact022.h
// ����Ա��frankwu
// ����˵�������︴����ͳһЧ��
//
// Ч���������ֵĸ�ʽ��|Ч��ID|��Ч����|�˺�����|�����˺��ٷ���|��ȡ�˺��ٷ���|
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT022_H
#define STDIMPACT022_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact022_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_022,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_ODDS,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_ODDS = 0,
							IDX_COUNTER_PUNCH_COUNT,
							IDX_REFLECT_RATE,
							IDX_ABSORB_RATE,
						};
				};
				StdImpact022_T() {};
				~StdImpact022_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetActivateOdds(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_ODDS);};
				VOID SetActivateOdds(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_ODDS, nValue);};

				INT GetActivateOddsInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ACTIVATE_ODDS,
						rImp.GetSkillLevel()
						);
				};
				INT GetDamageCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_COUNTER_PUNCH_COUNT,
						rImp.GetSkillLevel()
						);
				};
				INT GetReflectRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_REFLECT_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetAbsorbRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ABSORB_RATE,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				virtual VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& nDamage, SkillID_t nSkillID) const;
				virtual VOID OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const;
			protected:
			private:
		} DS_PetRevenge_T;
	};
};
#endif //STDIMPACT022_H