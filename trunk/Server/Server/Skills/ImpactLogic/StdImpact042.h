///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact042.h
// ����Ա������
// ����˵����Ч��--���������䳤��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT042_H
#define STDIMPACT042_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact042_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_042,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ATTACK_SPEED_REFIX = 0,
							IDX_CRITICAL_RATE_REFIX,
							IDX_IMMUNO_IMPACT_COLLECTION_ONE,
							IDX_IMMUNO_IMPACT_COLLECTION_TWO,
						};
				};
				StdImpact042_T() {};
				~StdImpact042_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetAttackSpeedRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_SPEED_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetCriticalRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CRITICAL_RATE_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetImmunoImpactCollectionOne(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(
						rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_IMMUNO_IMPACT_COLLECTION_ONE,
						rImp.GetSkillLevel()
						);
				};
				INT GetImmunoImpactCollectionTwo(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(
						rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_IMMUNO_IMPACT_COLLECTION_TWO,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				INT  OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const;
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
			protected:
			private:
		} SOT_MingJiao008_T;
	};
};
#endif //STDIMPACT042_H
