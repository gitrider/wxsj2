///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact080.h
// ����Ա���¾�
// ����˵����Ч��--͵impact���Լ�
// �޸ļ�¼��
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT080_H
#define STDIMPACT080_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact080_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_080,
				};

				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_RATE = 0,
						};
				};

				enum
				{
					IDX_STEAL_FROM_FRIEND = 0,					//�����Ѵ�͵
					IDX_STEAL_FROM_ENEMY,						//�ӵ��˴�͵
				};
				
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_STEAL_FROM_TYPE = 0,			//	0 ���� 1 ����
							IDX_RATE,							// 	����
							IDX_COLLECTION,						//  ָ����Ч��ID����(-1��Ч) ����ָ�������ֵ�󣬾Ͳ��ٿ��������IDX_STAND_FLAG��Ӱ��
							IDX_STAND_FLAG,						//	Ӱ�����ʱ�־
							IDX_COUNT,							//	����
						};
				};

				StdImpact080_T() {};
				~StdImpact080_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				
				INT GetStealFromType(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_STEAL_FROM_TYPE,
						rImp.GetSkillLevel()
						);
				};
				INT GetRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetCollection(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_COLLECTION,
						rImp.GetSkillLevel()
						);
				};
				INT GetStandFlag(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_STAND_FLAG,
						rImp.GetSkillLevel()
						);
				};
				INT GetCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_COUNT,
						rImp.GetSkillLevel()
						);
				};

			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				Obj_Character* GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const;
			private:
				
		};
	};
};
#endif //STDIMPACT079_H
