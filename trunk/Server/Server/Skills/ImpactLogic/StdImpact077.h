///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact077.h
// ����Ա��
// ����˵����Ч��--��һ��ʱ���ڰ�����ֵ���������С����ܡ��������ޡ��������ޡ������ħ������
//
// �޸ļ�¼����56���ܣ������������ƣ�
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT077_H
#define STDIMPACT077_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact077_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_077,
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
							IDX_HIT=0,
							IDX_MISS,
							IDX_CRITICAL,
							IDX_MAX_HP,
							IDX_MAX_MP,							
							//IDX_ATTACK_PHYSICS,
							IDX_ATTACK_NEAR,

							//IDX_DEFENCE_PHYSICS,
							IDX_DEFENCE_NEAR,

							IDX_ATTACK_MAGIC_NEAR,
							IDX_DEFENCE_MAGIC_NEAR,
						};
				};
				StdImpact077_T() {};
				~StdImpact077_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetHitRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_HIT,
						rImp.GetSkillLevel()
						);
				};
				INT GetMissRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MISS,
						rImp.GetSkillLevel()
						);
				};
				INT GetCriticalRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CRITICAL,
						rImp.GetSkillLevel()
						);
				};
				INT GetMaxHpRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MAX_HP,
						rImp.GetSkillLevel()
						);
				};
				INT GetMaxMpRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MAX_MP,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetDefenceNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackMagicNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_MAGIC_NEAR,
						rImp.GetSkillLevel()
						);
				};
				INT GetDefenceMagicNearRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DEFENCE_MAGIC_NEAR,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		};
	};
};
#endif //STDIMPACT056_H
