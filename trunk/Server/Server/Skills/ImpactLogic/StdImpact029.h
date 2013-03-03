///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact029.h
// ����Ա������
// ����˵����Ч��--���Ҽ������������Ч��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT029_H
#define STDIMPACT029_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact029_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_029,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_SHIELD_HP,
							IDX_ABSORB_RATE,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_TARGET_SKILL_COLLECTION=0,		//gather���е�ID
							IDX_REFIX_TYPE,						//�����ٷֱȽ������� 0 = ��������
							IDX_COOLDOWN_TIME_REFIX,			//��ȴ
							IDX_CHARGE_TIME_REFIX,				//����
							IDX_CHANNEL_TIME_REFIX,				//����
							IDX_DEPLETE_REFIX,					//����1
							IDX_ATTACK_TIMES_OR_CHANNEL_TIME,	//������������������ʱ����
							IDX_ACCURACY,						//������
							IDX_CRITICALRATE,					//����
							IDX_MAX_TARGET						//���Ŀ����������
						};
				};
				StdImpact029_T() {};
				~StdImpact029_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;


				INT GetTargetSkillCollection(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TARGET_SKILL_COLLECTION,
						rImp.GetSkillLevel()
						);
				};
				INT GetRefixType(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_REFIX_TYPE,
						rImp.GetSkillLevel()
						);
				};
				INT GetCooldownTimeRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_COOLDOWN_TIME_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetChargeTimeRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CHARGE_TIME_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetChannelTimeRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CHANNEL_TIME_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetDepleteRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEPLETE_REFIX,
						rImp.GetSkillLevel()
						);
				};

				INT GetAttackTimesOrChannelTimeRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_TIMES_OR_CHANNEL_TIME,
						rImp.GetSkillLevel()
						);
				};
				INT GetAccuracyRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ACCURACY,
						rImp.GetSkillLevel()
						);
				};
				INT GetCriticalrateRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CRITICALRATE,
						rImp.GetSkillLevel()
						);
				};
				INT GetMaxTargetRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MAX_TARGET,
						rImp.GetSkillLevel()
						);
				};


			public: //message handler
				VOID RefixSkill(OWN_IMPACT& rImp, SkillInfo_T& rSkill) const;


				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				VOID  RefixAllSkillAttr( SkillInfo_T &rSkill, OWN_IMPACT& rImp ) const;
				INT   GetRifixResult( INT iBase, INT iRefix, INT iRefixType ) const;
				FLOAT GetRifixResult( FLOAT fBase, FLOAT fRefix, INT iRefixType ) const;
			private:
		} DS_EMei009_T;
	};
};
#endif //STDIMPACT029_H
