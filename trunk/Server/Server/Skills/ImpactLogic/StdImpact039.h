///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact039.h
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ﱬ�������ӣ�ͬʱ�ܵ��˺�������ŭ��
//			���ּ�����������
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT039_H
#define STDIMPACT039_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact039_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_039,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_CRITICAL_REFIX = 0,
							IDX_RAGE_REGENERATE_REFIX,
						};
				};
				StdImpact039_T() {};
				~StdImpact039_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetCriticalRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CRITICAL_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetRageRegenerateRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RAGE_REGENERATE_REFIX,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const;
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
			protected:
			private:
		} SOT_ShaoLin007_T;
	};
};
#endif //STDIMPACT039_H
