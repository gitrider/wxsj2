/********************************************************************
	created:	2008/05/13
	created:	13:5:2008   19:58
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact075.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact075
	file ext:	h
	author:		Richard
	
	purpose:	与impact29功能完全一样,只是不用gather而是直接使用技能ID

				参考impact29
*********************************************************************/

#ifndef StdImpact075_h__
#define StdImpact075_h__


#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact075_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_075,
				};

				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_TARGET_SKILL_ID=0,				//Skill ID
							IDX_REFIX_TYPE,						//按点或百分比进行修正 0 = 点数修正
							IDX_COOLDOWN_TIME_REFIX,			//冷却
							IDX_CHARGE_TIME_REFIX,				//聚气
							IDX_CHANNEL_TIME_REFIX,				//引导
							IDX_DEPLETE_REFIX,					//消耗1
							IDX_ATTACK_TIMES_OR_CHANNEL_TIME,	//连续攻击次数或引导时间间隔
							IDX_ACCURACY,						//命中率
							IDX_CRITICALRATE,					//会心
							IDX_MAX_TARGET						//最大目标搜索个数
						};
				};
				StdImpact075_T() {};
				~StdImpact075_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;


				INT GetTargetSkillID(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_TARGET_SKILL_ID,
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
		} ;
	};
};

#endif // StdImpact075_h__
