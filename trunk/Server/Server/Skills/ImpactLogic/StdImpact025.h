///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact025.h
// ����Ա��frankwu
// ����˵����Ч��--��������һ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT025_H
#define STDIMPACT025_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact025_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_025,
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
						};
				};
				StdImpact025_T() {};
				~StdImpact025_T() {};
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
			public: //message handler
				INT OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const;
			protected:
			private:
		} DS_CounteractCriticalHit_T;
	};
};
#endif //StdImpact025_H


