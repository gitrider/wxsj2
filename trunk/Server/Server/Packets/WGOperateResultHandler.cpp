/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "WGOperateResult.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "PlayerPool.h"

namespace Packets
{

uint	WGOperateResultHandler::Execute(WGOperateResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID() ;
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
	if( pGamePlayer==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	// 对返回码做特殊的条件处理 [6/16/2008 wuwenbin]
	switch(pPacket->getResult()){
		case OR_CANCELHORSEFINDMATE_SUCC:
			{
				// 重置当前征友的骑乘GUID [6/16/2008 wuwenbin]
				PET_GUID_t petGuid;
				pHuman->SetFindingMateHorseGUID(petGuid);
			}
			break;
		case OR_ISSUEHORSE_FAIL:
			{
				// 重置当前征友的骑乘GUID [6/16/2008 wuwenbin]
				PET_GUID_t petGuid;
				pHuman->SetFindingMateHorseGUID(petGuid);
				// 骑乘征友发布失败则返回玩家的金钱 [6/16/2008 wuwenbin]
				UINT uMoney = pHuman->GetMoney() ;
				uMoney += g_TableInit.m_PetConfig.m_Money_FindingHorseMate;
				pHuman->SetMoney(uMoney);
				pHuman->SaveMoneyLogByHuman(MONEY_HORSE_FINDINGMATING, g_TableInit.m_PetConfig.m_Money_FindingHorseMate);
			}

			break;
		default:
			break;
	}

	pHuman->SendOperateResultMsg(pPacket->getResult());

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
}
