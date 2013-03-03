///////////////////////////////////////////////////////////////////////////////
// �ļ�����SkillLogic.h
// ����Ա������
// ����˵�������ܵĻ����࣬�������ܵĹ����ӿںͳ��ù���
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef SCRIPTLOGIC_H
#define SCRIPTLOGIC_H
#include "Type.h"
#include "Obj_Human.h"
namespace Script_Module
{
	class ScriptLogic_T
	{
		public:
			// public interface
			ScriptLogic_T(){};
			~ScriptLogic_T(){};
			//OnRefixItemPower();
			BOOL StartCharging(Obj_Character& rMe) const;
			BOOL StartChanneling(Obj_Character& rMe) const;
			BOOL StartLaunching(Obj_Character& rMe) const;
			BOOL Action_ActivateOnceHandler(Obj_Character& rMe) const;
			BOOL Action_ActivateEachTickHandler(Obj_Character& rMe) const;
			BOOL Item_IsSkillLikeScript(Obj_Character& rMe, ScriptID_t nScript) const;
			BOOL Item_CallDefaultEvent(Obj_Character& rMe, ScriptID_t nScript, INT nBagIndex) const;
			BOOL RegisterImpactEvent(Obj_Character& rReceiver, Obj_Character& rSender, OWN_IMPACT& rImp, Time_t nDelayTime, BOOL bCriticalFlag) const;
			BOOL RegisterSkillMissEvent(Obj_Character& rReceiver, Obj_Character& rSender, SkillID_t nSkill, Time_t nDelayTime) const;
			BOOL RegisterBeSkillEvent(Obj_Character& rReceiver, Obj_Character& rSender, INT nBehaviorType, Time_t nDelayTime) const;
			BOOL Script_CancelImpacts(Obj_Character& rMe, ScriptID_t nScriptID) const;
		protected: //interface for interval using
			BOOL Script_OnConditionCheck(Obj_Character& rMe) const;
			BOOL Script_OnDeplete(Obj_Character& rMe) const;
			BOOL Script_OnActivateOnce(Obj_Character& rMe) const;
			BOOL Script_OnActivateEachTick(Obj_Character& rMe) const;
			// standard methods
			VOID SetCooldown(Obj_Character& rMe, CooldownID_t nID, INT nCooldownTime) const;
			//VOID StopSkill(Obj_Character& rMe) const;
		private:
	};
	extern ScriptLogic_T g_ScriptLogic;
}
#endif //SCRIPTLOGIC_H
