///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact045.h
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ﱬ�������ӣ�ͬʱ�ܵ��˺�������ŭ��
//			���ּ�����������
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT045_H
#define STDIMPACT045_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact045_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_045,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_RAGE_REGENERATION_ADDITIONAL =0,
						};
				};
				StdImpact045_T() {};
				~StdImpact045_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetRageRegenerationAdditional(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RAGE_REGENERATION_ADDITIONAL,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const;
			protected:
			private:
		}SOT_GaiBang012_T;
	};
};
#endif //STDIMPACT045_H
