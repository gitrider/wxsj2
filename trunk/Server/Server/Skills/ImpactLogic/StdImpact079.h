///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact079.h
// ����Ա���¾�
// ����˵����Ч��--������ͬӰ�����ʵ�Ч��
// �޸ļ�¼��
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT079_H
#define STDIMPACT079_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact079_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_079,
				};

				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_RATE = 0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_FRIEND_RATE = 0,
							IDX_FRIEND_STAND_FLAG,				//�ѷ�Ӱ�����ʱ�־
							IDX_FRIEND_DISPEL_COUNT,			//�ѷ���������
							IDX_ENEMY_RATE,
							IDX_ENEMY_STAND_FLAG,				//�з�Ӱ�����ʱ�־
							IDX_ENEMY_DISPEL_COUNT,				//�з���������
						};
				};

				StdImpact079_T() {};
				~StdImpact079_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				
				INT GetFriendRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_FRIEND_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetFriendStandFlag(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_FRIEND_STAND_FLAG,
						rImp.GetSkillLevel()
						);
				};
				INT GetFriendDispelCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_FRIEND_DISPEL_COUNT,
						rImp.GetSkillLevel()
						);
				};
				INT GetEnemyRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ENEMY_RATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetEnemyStandFlag(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ENEMY_STAND_FLAG,
						rImp.GetSkillLevel()
						);
				};
				INT GetEnemyDispelCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ENEMY_DISPEL_COUNT,
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
