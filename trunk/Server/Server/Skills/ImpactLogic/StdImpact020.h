///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact020.h
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ڣ�Ч�������������������ԭ�ظ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT020_H
#define STDIMPACT020_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact020_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_020,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_RATE,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_HP_RECOVER_RATE=0,
							IDX_MP_RECOVER_RATE,
							IDX_RAGE_RECOVER_RATE,
						};
				};
				StdImpact020_T() {};
				~StdImpact020_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetHpRecoverRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_HP_RECOVER_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetMpRecoverRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MP_RECOVER_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetRageRecoverRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RAGE_RECOVER_RATE,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID OnDie(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} SOT_SelfResurrection_T;
	};
};
#endif //STDIMPACT020_H
