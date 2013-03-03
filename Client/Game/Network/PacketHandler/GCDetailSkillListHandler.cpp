#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "GCDetailSkillList.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Interface\GMGameInterface.h"


uint GCDetailSkillListHandler :: Execute( GCDetailSkillList* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);
		//确认是自己的数据
		CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
		if(pMySelf->GetServerID() != pPacket->getObjID()) 
		{
			return PACKET_EXE_CONTINUE;
		}

		//清空所有技能
		pMySelf->GetCharacterData()->Skill_CleanAll();
		//刷新到用户数据
		for(INT i=0; i<(INT)pPacket->getNumSkill(); i++)
		{
			const _OWN_SKILL* pOwnerSkill = pPacket->getSkill(i);

			pMySelf->GetCharacterData()->Set_Skill(pOwnerSkill->m_nSkillID, TRUE);
			pMySelf->GetCharacterData()->Set_SkillLevel(pOwnerSkill->m_nSkillID, pOwnerSkill->m_nLevel);
		}

		// 设置默认技能
		//CGameInterface::GetMe()->Skill_SetActive( CActionSystem::GetMe()->GetMyDefaultSkillAtion() );
		if(pPacket->getNumSkill())//tempfix by yangfei_hongtu
		{
			const _OWN_SKILL* pOwnerSkill = pPacket->getSkill(0);
			pMySelf->GetCharacterData()->Set_Skill(pOwnerSkill->m_nSkillID, TRUE, TRUE);
			CGameInterface::GetMe()->Skill_SetActive( CActionSystem::GetMe()->GetMyDefaultSkillAtionBySkillId(pOwnerSkill->m_nSkillID) );
		}

		//刷新到UI
		CEventSystem::GetMe()->PushEvent(GE_SKILL_UPDATE);

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

