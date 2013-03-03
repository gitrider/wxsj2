/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "GWDetailOtherOnlineAttrib_Horse.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGDetailOtherOnlineAttrib_Horse.h"
#include "User.h"
#include "OnlineUser.h"

namespace Packets
{

uint	GWDetailOtherOnlineAttrib_HorseHandler::Execute(GWDetailOtherOnlineAttrib_Horse* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	Assert( pServerPlayer );

	GUID_t guid = pPacket->GetPlayerGUID();
	USER* pUser = g_pOnlineUser->FindUser(guid);
	if(pUser){
		ID_t ServerID = pUser->GetServerID() ;
		ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
		if( pDestServerPlayer==NULL )
		{
			Assert(FALSE);
		}
		WGDetailOtherOnlineAttrib_Horse Msg;
		Msg.SetPlayerID(pPacket->GetPlayerID());//发起查看者的ID

		Msg.SetGUID(pPacket->GetGUID());
		Msg.SetDataID(pPacket->GetDataID());
		Msg.SetLevel(pPacket->GetLevel());
		Msg.SetGenGu(pPacket->GetGenGu());
		Msg.SetRemainPoint(pPacket->GetRemainPoint());
		Msg.SetExp(pPacket->GetExp());
		Msg.SetGeneration(pPacket->GetGeneration());
		Msg.SetHappiness(pPacket->GetHappiness());
		Msg.SetSpouseGUID(pPacket->GetSpouseGUID());
		Msg.SetName(pPacket->GetName());
		Msg.SetNickName(pPacket->GetNickName());
		Msg.SetDexPerception(pPacket->GetDexPerception());
		Msg.SetStrPerception(pPacket->GetStrPerception());
		Msg.SetIntPerception(pPacket->GetIntPerception());
		Msg.SetConPerception(pPacket->GetConPerception());
		Msg.SetSavvy(pPacket->GetSavvy());
		Msg.SetMountID(pPacket->GetMountID());

		Msg.SetHorseState(pPacket->GetHorseState());
		Msg.SetGrowRate(pPacket->GetGrowRate());

		for(UINT i=0; i<PET_MAX_SKILL_COUNT; ++i){
			Msg.SetSkill(i, pPacket->GetSkill(i));
		}

		Msg.SetCon(pPacket->GetCon());
		Msg.SetDex(pPacket->GetDex());
		Msg.SetInt(pPacket->GetInt());
		Msg.SetStr(pPacket->GetStr());

		pDestServerPlayer->SendPacket(&Msg);

		Log::SaveLog( WORLD_LOGFILE, "GWDetailOtherOnlineAttrib_HorseHandler: ok ");
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

}
