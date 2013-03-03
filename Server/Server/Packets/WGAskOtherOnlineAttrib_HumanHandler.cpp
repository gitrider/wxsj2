/********************************************************************
	created:	2008/06/04
	created:	4:6:2008   13:46
	filename: 	d:\prj\src\MapServer\Server\Packets\WGAskOtherOnlineAttrib_HumanHandler.cpp
	file path:	d:\prj\src\MapServer\Server\Packets
	file base:	WGAskOtherOnlineAttrib_HumanHandler
	file ext:	cpp
	author:		wuwenbin
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "WGAskOtherOnlineAttrib_Human.h"
#include "GWDetailOtherOnlineAttrib_Human.h"
#include "GWDetailOtherOnlineAttrib_Horse.h"
#include "GWDetailOtherOnlineEquipList_Human.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

uint WGAskOtherOnlineAttrib_HumanHandler::Execute( WGAskOtherOnlineAttrib_Human* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	Assert(pServerPlayer);

	PlayerID_t PlayerID = pPacket->GetDestPlayerID();	//目标玩家的PlayerID
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if ( pScene==NULL )
	{ // 主要是玩家断线
		// Assert( pScene );
		return PACKET_EXE_CONTINUE;
	}

	// 发送人物基本信息
	GWDetailOtherOnlineAttrib_Human Msg;
	Msg.SetPlayerID(pPacket->GetSourcePlayerID());//发起查看者的ID
	Msg.SetLevel(pHuman->GetLevel());
	Msg.SetCountry(pHuman->GetCountry());
	Msg.SetMenpai(pHuman->GetMenPai());
	Msg.SetDataID(pHuman->GetDataID());
	Msg.SetPlayerGUID(pPacket->GetGUID());	//返回GUID，其实就是最初发起者的GUID；）


	pServerPlayer->SendPacket(&Msg);

	// 发送骑乘信息
	INT i=0;
	INT j=0;
	for(i=0; i<HUMAN_PET_MAX_COUNT; ++i){
		const _PET_DB_LOAD* pPetDB = pHuman->GetDB()->GetPet(i);
		if(pPetDB && !pPetDB->m_GUID.IsNull()){
			GWDetailOtherOnlineAttrib_Horse Msg2;
			Msg2.SetPlayerGUID(pPacket->GetGUID());	//返回GUID，其实就是最初发起者的GUID；）
			Msg2.SetPlayerID(pPacket->GetSourcePlayerID());//发起查看者的ID

			Msg2.SetGUID(pPetDB->m_GUID);
			Msg2.SetDataID(pPetDB->m_nDataID);
			Msg2.SetLevel(pPetDB->m_nLevel);
			Msg2.SetGenGu(pPetDB->m_nGenGu);
			Msg2.SetRemainPoint(pPetDB->m_nRemainPoint);
			Msg2.SetExp(pPetDB->m_nExp);
			Msg2.SetGeneration(pPetDB->m_byGeneration);
			Msg2.SetHappiness(pPetDB->m_byHappiness);
			Msg2.SetSpouseGUID(pPetDB->m_SpouseGUID);
			Msg2.SetName(pPetDB->m_szName);
			Msg2.SetNickName(pPetDB->m_szNick);
			Msg2.SetDexPerception(pPetDB->m_iSmartnessPerception);
			Msg2.SetStrPerception(pPetDB->m_iStrengthPerception);
			Msg2.SetIntPerception(pPetDB->m_iMindPerception);
			Msg2.SetConPerception(pPetDB->m_iConstitutionPerception);
			Msg2.SetSavvy(pPetDB->m_iSavvy);
			Msg2.SetMountID(pPetDB->m_nDataID);

			if(pPetDB->m_bMating){
				Msg2.SetHorseState(1/*HorseState_Mating*/);
			}
			if(pPetDB->m_bCheckup){
				Msg2.SetGrowRate((INT)pPetDB->m_fGrowRate);
			}
			else{
				Msg2.SetGrowRate(INVALID_ID);
			}

			for(j=0; j<PET_MAX_SKILL_COUNT; ++j){
				Msg2.SetSkill(j, &(pPetDB->m_SkillList[j]) );
			}

			Msg2.SetCon(pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_CON));
			Msg2.SetDex(pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_DEX));
			Msg2.SetInt(pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_INT));
			Msg2.SetStr(pPetDB->m_BaseAttrLevel1.Get(CATTR_LEVEL1_STR));

			pServerPlayer->SendPacket(&Msg2);
		}
	}

	// 发送人物装备信息
	GWDetailOtherOnlineEquipList_Human Msg3;

	Msg3.SetPlayerGUID(pPacket->GetGUID());	//返回GUID，其实就是最初发起者的GUID；）

	Msg3.SetPlayerID(pPacket->GetSourcePlayerID()); //设置发起查看者的ID
	Msg3.SetAskMode(ASK_EQUIP_ALL);

	for(i=0; i<HEQUIP_NUMBER; i++){
		//if(pFullUserData->m_Equip.IsSet((HUMAN_EQUIP)i) == TRUE){
		const _ITEM* pEquip = const_cast<_EQUIP_DB_LOAD*>(pHuman->GetDB()->GetEquipDB())->GetEquipDB((HUMAN_EQUIP)i);
		if(pEquip && !pEquip->IsNullType() && (pEquip->ItemClass() == ICLASS_EQUIP) )
			Msg3.SetEquipData((HUMAN_EQUIP)i, pEquip);
		//}
	}			

	pServerPlayer->SendPacket(&Msg3);



	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
