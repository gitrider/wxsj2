// GCMissionResultHandler.cpp
#include "stdafx.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\event\GMEventSystem.h"
#include "GCMissionResult.h"

using namespace Packets;
uint GCMissionResultHandler::Execute( GCMissionResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);

		if(pPacket->getIsFinished())		//�������
		{
			// �ر��ύ������Ʒ�Ľ���
			CEventSystem::GetMe()->PushEvent(GE_QUEST_AFTER_CONTINUE);
	
			// �򿪽��ս����Ľ���
			
		}
		else								//û���������
		{
			// ��Ӧһ����ʾ��Ϣ�����ر��ύ������Ʒ�Ľ��棬������������Ҽ���ǲ����Լ��Ŵ�����Ʒ
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCMissionResultHandler_Info_I_Do_Note_Need_This_Item");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ҵ���Ʒ��������Ҫ��!");
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
