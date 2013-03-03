/********************************************************************
	created:	2008/05/21
	created:	21:5:2008   17:30
	filename: 	MapServer\Server\Skills\SkillLogic\ImpactsToTargetAndSelfForRun.h
	file path:	MapServer\Server\Skills\SkillLogic
	file base:	ImpactsToTargetAndSelfForRun
	file ext:	h
	author:		Richard
	
	purpose:	为冲锋技能专用，同时给自己和目标各加一个impact
*********************************************************************/
#ifndef ImpactsToTargetAndSelfForRun_h__
#define ImpactsToTargetAndSelfForRun_h__

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class ImpactsToTargetAndSelfForRun_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = IMPACTS_TO_TARGET_AND_SELF_FOR_RUN,
				};
				class SkillInfoDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_ONCE_IMPACT_FOR_SELF =0,
							IDX_ACTIVATE_ONCE_IMPACT_FOR_TARGET,
							IDX_ACTIVATE_EACH_TICK_IMPACT_FOR_SELF,
							IDX_ACTIVATE_EACH_TICK_IMPACT_FOR_TARGET,
						};
				};
				// public interface
				ImpactsToTargetAndSelfForRun_T(){};
				~ImpactsToTargetAndSelfForRun_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
				// Accessor
				INT GetActivateOnceImpactForSelf(SkillInfo_T const& rSkill) const;
				INT GetActivateEachTickImpactForSelf(SkillInfo_T const& rSkill) const;
				INT GetActivateOnceImpactForTarget(SkillInfo_T const& rSkill) const;
				INT GetActivateEachTickImpactForTarget(SkillInfo_T const& rSkill) const;
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
				BOOL EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};

#endif // ImpactsToTargetAndSelfForRun_h__