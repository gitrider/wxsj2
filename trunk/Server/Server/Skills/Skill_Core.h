///////////////////////////////////////////////////////////////////////////////
// �ļ�����Skill_Core.h
// ����Ա������
// ����˵���������ں�ģ��
//
// �޸ļ�¼��
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
				// ���ܽӿ�
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
				// ���ܽӿڵ�ʵ��
				//************************************
				BOOL ProcessSkillRequestImp(Obj_Character& rMe, SkillID_t nSkillID, ObjID_t nTargetID, WORLD_POS const& rTargetPos, FLOAT fTargetDir, GUID_t guidTarget, INT iHurtDelayTime, BOOL bSubSkill) const;

				//************************************
				// Method:    CheckCanUseSkillByHorse
				// FullName:  Combat_Module::Skill_Module::SkillCore_T::CheckCanUseSkillByHorse
				// Access:    protected 
				// Returns:   BOOL
				// �Ƿ���Ϊ����д˼��ܶ�����ʹ��
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
				// �жϼ���ʹ���Ƿ�����������������ǣ����жϽ�ɫ�Ƿ��������
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
