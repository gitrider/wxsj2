/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "WGDetailOtherOnlineAttrib_Horse.h"
#include "GCDetailAttrib_Pet.h"
#include "DB_Struct.h"
#include "PlayerPool.h"
#include "Obj_Human.h"
#include "Scene.h"

namespace Packets
{

UINT WGDetailOtherOnlineAttrib_HorseHandler::Execute( WGDetailOtherOnlineAttrib_Horse* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();//发起查看者的ID
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	GCDetailAttrib_Pet Msg;
	Msg.SetExtraInfoLength(1);
	BYTE bFlag;
	bFlag = TYPE_CONTEX_MENU_OTHER_PET;
	Msg.SetExtraInfoData(&bFlag);


	Msg.SetGUID( pPacket->GetGUID() );
	Msg.SetDataID( pPacket->GetDataID() );
	Msg.SetName( pPacket->GetName() );
	Msg.SetNickName( pPacket->GetNickName() );
	Msg.SetHP( pPacket->GetHP() );
	Msg.SetAIType(pPacket->GetAIType());
	Msg.SetSpouseGUID(pPacket->GetSpouseGUID());
	Msg.SetLevel(pPacket->GetLevel());
	Msg.SetLife(pPacket->GetLife());
	Msg.SetGeneration(pPacket->GetGeneration());
	Msg.SetHappiness(pPacket->GetHappiness());
	Msg.SetExp(pPacket->GetExp());
	Msg.SetStrPerception(pPacket->GetStrPerception());
	Msg.SetConPerception(pPacket->GetConPerception());
	Msg.SetDexPerception(pPacket->GetDexPerception());
	Msg.SetIntPerception(pPacket->GetIntPerception());
	Msg.SetSex( pPacket->GetSex() );
	Msg.SetHorseState( pPacket->GetHorseState());
	Msg.SetStr( pPacket->GetStr() );
	Msg.SetCon( pPacket->GetCon() );
	Msg.SetDex( pPacket->GetDex() );
	Msg.SetInt( pPacket->GetInt() );
	Msg.SetGenGu( pPacket->GetGenGu() );
	Msg.SetHPMax( pPacket->GetHPMax() );
	Msg.SetLevel( pPacket->GetLevel() );
	Msg.SetSavvy( pPacket->GetSavvy() );

	Msg.SetModelID( pPacket->GetModelID() );
	Msg.SetMountID( pPacket->GetMountID() );
	Msg.SetRemainPoint( pPacket->GetRemainPoint() );
	Msg.SetGrowRate(pPacket->GetGrowRate() );

	for(INT i = 0; i < PET_MAX_SKILL_COUNT; i++){
		Msg.SetSkill(i, pPacket->GetSkill(i));
	}

	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "WGDetailOtherOnlineAttrib_HorseHandler: ok obj=%d,scene=%d",
		pGamePlayer->GetHuman()->GetID(), pGamePlayer->GetHuman()->getScene()->SceneID());


	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

}
