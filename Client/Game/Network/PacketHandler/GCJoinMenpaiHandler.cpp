// GCJoinMenpaiHandler.cpp

#include "StdAfx.h"

#include "..\..\Procedure\Gameprocedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Sound\GMSoundSystem.h"
#include "GCJoinMenpai.h"

uint GCJoinMenpaiHandler::Execute( GCJoinMenpai* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		short shortSucceedJoin = pPacket->GetIsSucceedJoin();
		if( JOINMENPAI_SUCCEED == shortSucceedJoin )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCJoinMenpaiHandler_Info_Join_Menpai_Ok");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"加入门派成功!");
			CSoundSystemFMod::_PlayUISoundFunc(62);
		}
		else
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCJoinMenpaiHandler_Info_Join_Menpai_Fail");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"加入门派失败!");
		}
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
