///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact055.h
// ����Ա������
// ����˵����Ч��--������Ч��ת�Ƶ�����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT055_H
#define STDIMPACT055_H

 
#include "ImpactLogic.h"
 
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact055_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_055,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_TRANSFER_IMPACT_COUNT=0,
							IDX_TRANSFER_COLLECTION_0,
							IDX_TRANSFER_COLLECTION_1
						};
				};
				StdImpact055_T() {};
				~StdImpact055_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};

				INT GetTransferImpactCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TRANSFER_IMPACT_COUNT,
						rImp.GetSkillLevel()
						);
				};
				INT GetTransferCollection0(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TRANSFER_COLLECTION_0,
						rImp.GetSkillLevel()
						);
				};
				INT GetTransferCollection1(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TRANSFER_COLLECTION_1,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
			private:
		} DI_TransferImpactToGuard_T;
	};
};
#endif //STDIMPACT055_H
