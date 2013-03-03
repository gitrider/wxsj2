///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact014.h
// ����Ա������
// ����˵����Ч��--����,��Ĭ�������޵�״̬
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT014_H
#define STDIMPACT014_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact014_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_014,
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
							IDX_CAN_ACTION1_FLAG=0,
							IDX_CAN_ACTION2_FLAG,
							IDX_CAN_MOVE_FLAG,
							IDX_UNBREAKABLE_FLAG,
							IDX_MOVE_SPEED,
							IDX_STEALTH_LEVEL,
							IDX_DETECT_LEVEL,
							IDX_MODEL_ID,
							IDX_MOUNT_ID,
							IDX_ATTACK_SPEED,
						};
				};
				
				StdImpact014_T() {};
				~StdImpact014_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				
				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetCanAction1Refix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CAN_ACTION1_FLAG,
						rImp.GetSkillLevel()
						);
				};
				INT GetCanAction2Refix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CAN_ACTION2_FLAG,
						rImp.GetSkillLevel()
						);
				};
				INT GetCanMoveRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_CAN_MOVE_FLAG,
						rImp.GetSkillLevel()
						);
				};
				INT GetUnbreakableRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_UNBREAKABLE_FLAG,
						rImp.GetSkillLevel()
						);
				};
				INT GetMoveSpeedRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MOVE_SPEED,
						rImp.GetSkillLevel()
						);
				};
				INT GetStealthLevelRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_STEALTH_LEVEL,
						rImp.GetSkillLevel()
						);
				};
				INT GetDetectLevelRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_DETECT_LEVEL,
						rImp.GetSkillLevel()
						);
				};
				INT GetModelRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MODEL_ID,
						rImp.GetSkillLevel()
						);
				};
				INT GetMountRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MOUNT_ID,
						rImp.GetSkillLevel()
						);
				};
				INT GetAttackSpeedRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ATTACK_SPEED,
						rImp.GetSkillLevel()
						);
				};	

			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetBoolAttrRefix(OWN_IMPACT& rImp, Obj_Character& rMe, CharBoolAttrs_T::Index_T nIdx, BOOL& rBoolAttrRefix) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} SOT_TransformsAndCanActions_T;
	};
};
#endif //STDIMPACT014_H


