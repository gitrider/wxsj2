
#include "StdAfx.h"
#include "GCSkillUpgrade.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Interface\GMInterface_Script_Tooltip.h"
#include "..\..\..\GameInterface\Include\Game\GIActionSystem.h"
#include "..\..\..\Game\Action\GMActionSystem.h"


uint GCSkillUpgradeHandler::Execute( GCSkillUpgrade* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if (CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		// ѧϰ�ɹ�
		if (pPacket->IsSuccessful())
		{
			CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
			if (pMySelf)
			{
				const SCLIENT_SKILL* skill = pMySelf->GetCharacterData()->Get_Skill(pPacket->GetSkillID());
				// û��������������
				if (NULL == skill)				
				{
					pMySelf->GetCharacterData()->Set_Skill(pPacket->GetSkillID(), TRUE);					
				}

				pMySelf->GetCharacterData()->Set_SkillLevel(pPacket->GetSkillID(), pPacket->GetLevel());

				// ���supertooltip�򿪣�������ʾ���Ƕ�Ӧ���ܵ�button�������������̸��¼��ܵ�tooltip 20100602 BLL
				if ( CGameProcedure::s_pUISystem->IsWindowShow("SuperTooltip") && (SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip.GetActionItem()) != NULL &&
					(SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip.GetActionItem())->GetType() == AOT_SKILL &&
					(SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip.GetActionItem())->GetDefineID() == pPacket->GetSkillID() )
				{
					// ֪ͨsupertooltip��ˢ��
					CEventSystem::GetMe()->PushEvent( GE_UPDATE_SUPERTOOLTIP );
				}

				// ֪ͨ���ܿ�ˢ��
				CEventSystem::GetMe()->PushEvent( GE_SKILL_UPDATE );
			}			
		}
		
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
