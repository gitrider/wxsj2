///////////////////////////////////////////////////////////////////////////////
// 文件名：Skill_Core.h
// 程序员：高骐
// 功能说明：技能内核模块
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file Skill_Core.h
*		\brief the Core logic skill module
*/

#ifndef SKILL_CORE_H
#define SKILL_CORE_H

#include "Type.h"
#include "Obj_Character.h"
#include "DataRecords.h"
#include "SkillInfo.h"
#include "Combat_Core.h"

namespace Combat_Module
{
	
	namespace Skill_Module
	{
		using namespace Combat_Module;
		
		class SkillCore_T
		{
			friend class Combat_Module::CombatCore_T;
			friend class Obj_Character;
			public:
				SkillCore_T(){};
				~SkillCore_T(){};
				BOOL InstanceSkill(SkillInfo_T& rSkillInfoOut,Obj_Character& rMe, SkillID_t nSkill) const;
				//************************************
				// Method:    ProcessSkillRequest
				// FullName:  Combat_Module::Skill_Module::SkillCore_T::ProcessSkillRequest
				// Access:    public 
				// Returns:   BOOL
				// Qualifier: const
				// Parameter: Obj_Character & rMe
				// Parameter: SkillID_t nSkillID
				// Parameter: ObjID_t nTargetID
				// Parameter: WORLD_POS const & rTargetPos
				// Parameter: FLOAT fTargetDir
				// Parameter: GUID_t guidTarget
				// 技能接口
				//************************************
				BOOL ProcessSkillRequest(Obj_Character& rMe, SkillID_t nSkillID, ObjID_t nTargetID, WORLD_POS const& rTargetPos, FLOAT fTargetDir, GUID_t guidTarget, INT iHurtDelayTime, BOOL bSubSkill=FALSE) const;
			protected:
				//************************************
				// Method:    ProcessSkillRequestImp
				// FullName:  Combat_Module::Skill_Module::SkillCore_T::ProcessSkillRequestImp
				// Access:    protected 
				// Returns:   BOOL
				// Qualifier: const
				// Parameter: Obj_Character & rMe
				// Parameter: SkillID_t nSkillID
				// Parameter: ObjID_t nTargetID
				// Parameter: WORLD_POS const & rTargetPos
				// Parameter: FLOAT fTargetDir
				// Parameter: GUID_t guidTarget
				// 技能接口的实现
				//************************************
				BOOL ProcessSkillRequestImp(Obj_Character& rMe, SkillID_t nSkillID, ObjID_t nTargetID, WORLD_POS const& rTargetPos, FLOAT fTargetDir, GUID_t guidTarget, INT iHurtDelayTime, BOOL bSubSkill) const;

				//************************************
				// Method:    CheckCanUseSkillByHorse
				// FullName:  Combat_Module::Skill_Module::SkillCore_T::CheckCanUseSkillByHorse
				// Access:    protected 
				// Returns:   BOOL
				// 是否因为马具有此技能而可以使用
				//************************************
				BOOL CheckCanUseSkillByHorse( Obj_Character& rMe, SkillID_t nSkillID ) const;

				BOOL ActiveSkillNow(Obj_Character& rMe, BOOL bSubSkill) const;
				//************************************
				// Method:    IsCanPassCheckByWeapon
				// FullName:  Combat_Module::Skill_Module::SkillCore_T::IsCanPassCheckByWeapon
				// Access:    protected 
				// Returns:   BOOL
				// Qualifier: const
				// Parameter: SkillInfo_T & rSkillInfo
				// Parameter: Obj_Character & rMe
				// Parameter: TargetingAndDepletingParams_T & rParams
				// 判断技能使用是否必须持有武器，如果是，则判断角色是否持有武器
				//************************************
				BOOL IsCanPassCheckByWeapon( SkillInfo_T &rSkillInfo, Obj_Character &rMe, TargetingAndDepletingParams_T& rParams ) const;
				VOID OnException(Obj_Character& rMe) const;
			private:
		};
		BOOL Skill_IsSkillInCollection(SkillInfo_T const& rSkillInfo, INT nCollection);
		INT Skill_GetSkillInfoDescriptorValueByIndex(SkillInfo_T const& rSkillInfo, INT nIndex);
		ConDepTerm_T const* Skill_GetConDepTermByIndex(SkillInfo_T const& rSkillInfo, INT nIndex);
		INT Skill_GetSkillHurtDelayTime( INT iSkillID );
		INT Skill_GetSkillStandFlag( SkillID_t nSkill );
	};
};
extern Combat_Module::Skill_Module::SkillCore_T g_SkillCore;
#endif //SKILL_CORE_H
