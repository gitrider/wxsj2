///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact078.h
// ����Ա���¾�
// ����˵����Ч��--��һ��ʱ�����У�ɨ�������Χ��obj������ÿ����Ҵ���impact
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT078_H
#define STDIMPACT078_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact078_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_078,
				};

				enum
				{
					ENUM_SCAN_OBJ_TYPE_ENEMY,
					ENUM_SCAN_OBJ_TYPE_TEAMMATE,
					ENUM_SCAN_OBJ_TYPE_TEAMMATE_OR_ENEMY,
				};
				
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_SCAN_RADIUS =0,
							IDX_SCAN_OBJ_TYPE, 			//0		����		1		����		2		���� + ����
							IDX_EFFECTED_OBJ_COUNT,
							IDX_SUB_IMPACT_DATA_INDEX,
						};
				};

				StdImpact078_T() {};
				~StdImpact078_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return TRUE;};

				INT GetScanRadius(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SCAN_RADIUS,
						rImp.GetSkillLevel()
						);
				};

				INT GetScanObjType(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SCAN_OBJ_TYPE,
						rImp.GetSkillLevel()
						);
				};
				
				INT GetEffectedObjCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_EFFECTED_OBJ_COUNT,
						rImp.GetSkillLevel()
						);
				};

				INT GetSubImpactDataIndex(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT_DATA_INDEX,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const;

			protected:
			private:
		};
	};
};
#endif //STDIMPACT078_H
