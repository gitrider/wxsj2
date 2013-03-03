///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact023.h
// ����Ա��frankwu
// ����˵�������������DS
//
// Ч���������ֵĸ�ʽ��|Ч��ID|��Ч����|�ܻ�ʱ��Ч|����ʱ��Ч|�ܻ�ʱ�˺�����
//					   |����ʱ��ȡ����MP�ٷ���|����ʱ��ȡ����ŭ���ٷ���|
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT023_H
#define STDIMPACT023_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact023_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_023,
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
							IDX_DAMAGE_IGNORE_RATE,
							IDX_MP_DAMAGE_RATE,
							IDX_RAGE_DAMAGE_RATE,
						};
				};
				StdImpact023_T() {};
				~StdImpact023_T() {};
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
				INT GetDamageIgnoreRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DAMAGE_IGNORE_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetMpDamageRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MP_DAMAGE_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetRageDamageRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RAGE_DAMAGE_RATE,
						rImp.GetSkillLevel()
						);
				};

			public: //message handler
				virtual VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& nDamage, SkillID_t nSkillID) const;
				virtual VOID OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const;
			protected:
			private:
		} DS_PetDefence_T;
	};
};
#endif //STDIMPACT023_H