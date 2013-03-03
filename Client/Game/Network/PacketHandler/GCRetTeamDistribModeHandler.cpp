#include "StdAfx.h"
#include "gcretteamdistribmode.h"
#include "DataPool/GMUIDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Event\GMEventSystem.h"


uint	GCRetTeamDistribModeHandler::Execute(GCRetTeamDistribMode *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE nMode = pPacket->getMode();
			if (nMode < ITEM_DISTRIB_NUM)
			{
				if(CUIDataPool::GetMe()->GetTeamOrGroup())
				{
					CUIDataPool::GetMe()->GetTeamOrGroup()->ChangeMode(nMode);
					
					//这个消息用于实时更改队员队伍信息框下的提示信息
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CHANGE_MODE, nMode );
					//在这添加发送NEW_DEBUGMESSAGE，以在窗口上显示改变物品分配信息
					switch(nMode)
					{
						case ITEM_FREE_DISTRIB:
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品拾取模式更改为：自由拾取模式");
							}
							break;
						case ITEM_TEAMLEADER_DISTRIB:
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品拾取模式更改为：队长拾取模式");
							}
							break;
						case ITEM_INTURNS_DISTRIB:
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品拾取模式更改为：轮流拾取模式");
							}
							break;
						case ITEM_SEPARATE_DISTRIB:
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品拾取模式更改为：分别拾取模式");
							}
							break;
					}
				}
			}
		}
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}