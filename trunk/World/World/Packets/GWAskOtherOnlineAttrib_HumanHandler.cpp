/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "GWAskOtherOnlineAttrib_Human.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "HorseFindMateManager.h"
#include "WGOperateResult.h"
#include "WGDetailOtherOnlineAttrib_Human.h"
#include "WGDetailOtherOnlineEquipList_Human.h"
#include "WGDetailOtherOnlineAttrib_Horse.h"
#include "WGAskOtherOnlineAttrib_Human.h"

namespace Packets
{

uint	GWAskOtherOnlineAttrib_HumanHandler::Execute(GWAskOtherOnlineAttrib_Human* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourceServerPlayer = (ServerPlayer*)pPlayer;
	Assert( pSourceServerPlayer );

	GUID_t guid = pPacket->GetDestGUID();
	USER* pDestUser =  g_pOnlineUser->FindUser(guid);
	if(pDestUser){
		WGAskOtherOnlineAttrib_Human Msg;
		Msg.SetGUID(pPacket->GetSourceGUID());
		Msg.SetSourcePlayerID(pPacket->GetPlayerID());
		Msg.SetDestPlayerID(pDestUser->GetPlayerID());

		ID_t ServerID = pDestUser->GetServerID() ;
		ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
		if( pDestServerPlayer==NULL )
		{
			Assert(FALSE);
		}

		pDestServerPlayer->SendPacket(&Msg);

		/*
		FULLUSERDATA* pFullUserData = pUser->GetFullUserData();
		if(pFullUserData){
			// �������������Ϣ
			WGDetailOtherOnlineAttrib_Human Msg;
			Msg.SetPlayerID(pPacket->GetPlayerID());//����鿴�ߵ�ID
			Msg.SetLevel(pFullUserData->m_Human.m_Level);
			Msg.SetCountry(pFullUserData->m_Human.m_nCountry);
			Msg.SetMenpai(pFullUserData->m_Human.m_MenPai);
			Msg.SetDataID(pFullUserData->m_Human.m_Sex);


            pServerPlayer->SendPacket(&Msg);

			// ���������Ϣ
			INT i=0;
			INT j=0;
			for(i=0; i<HUMAN_PET_MAX_COUNT; ++i){
				if(!pFullUserData->m_PetList.m_aPetDB[i].m_GUID.IsNull()){
					WGDetailOtherOnlineAttrib_Horse Msg2;
					Msg2.SetPlayerID(pPacket->GetPlayerID());//����鿴�ߵ�ID

					Msg2.SetGUID(pFullUserData->m_PetList.m_aPetDB[i].m_GUID);
					Msg2.SetDataID(pFullUserData->m_PetList.m_aPetDB[i].m_nDataID);
					Msg2.SetLevel(pFullUserData->m_PetList.m_aPetDB[i].m_nLevel);
					Msg2.SetGenGu(pFullUserData->m_PetList.m_aPetDB[i].m_nGenGu);
					Msg2.SetRemainPoint(pFullUserData->m_PetList.m_aPetDB[i].m_nRemainPoint);
					Msg2.SetExp(pFullUserData->m_PetList.m_aPetDB[i].m_nExp);
					Msg2.SetGeneration(pFullUserData->m_PetList.m_aPetDB[i].m_byGeneration);
					Msg2.SetHappiness(pFullUserData->m_PetList.m_aPetDB[i].m_byHappiness);
					Msg2.SetSpouseGUID(pFullUserData->m_PetList.m_aPetDB[i].m_SpouseGUID);
					Msg2.SetName(pFullUserData->m_PetList.m_aPetDB[i].m_szName);
					Msg2.SetNickName(pFullUserData->m_PetList.m_aPetDB[i].m_szNick);
					Msg2.SetDexPerception(pFullUserData->m_PetList.m_aPetDB[i].m_iSmartnessPerception);
					Msg2.SetStrPerception(pFullUserData->m_PetList.m_aPetDB[i].m_iStrengthPerception);
					Msg2.SetIntPerception(pFullUserData->m_PetList.m_aPetDB[i].m_iMindPerception);
					Msg2.SetConPerception(pFullUserData->m_PetList.m_aPetDB[i].m_iConstitutionPerception);
					Msg2.SetSavvy(pFullUserData->m_PetList.m_aPetDB[i].m_iSavvy);
					Msg2.SetMountID(pFullUserData->m_PetList.m_aPetDB[i].m_nDataID);

					if(pFullUserData->m_PetList.m_aPetDB[i].m_bMating){
						Msg2.SetHorseState(1);
					}
					if(pFullUserData->m_PetList.m_aPetDB[i].m_bCheckup){
						Msg2.SetGrowRate((INT)pFullUserData->m_PetList.m_aPetDB[i].m_fGrowRate);
					}
					else{
						Msg2.SetGrowRate(INVALID_ID);
					}

					for(j=0; j<PET_MAX_SKILL_COUNT; ++j){
						Msg2.SetSkill(j, &(pFullUserData->m_PetList.m_aPetDB[i].m_SkillList[j]) );
					}

					Msg2.SetCon(pFullUserData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.Get(CATTR_LEVEL1_CON));
					Msg2.SetDex(pFullUserData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.Get(CATTR_LEVEL1_DEX));
					Msg2.SetInt(pFullUserData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.Get(CATTR_LEVEL1_INT));
					Msg2.SetStr(pFullUserData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.Get(CATTR_LEVEL1_STR));
	
					pServerPlayer->SendPacket(&Msg2);
				}
			}

			// ��������װ����Ϣ
			WGDetailOtherOnlineEquipList_Human Msg3;
			Msg3.SetPlayerID(pPacket->GetPlayerID()); //���÷���鿴�ߵ�ID
			Msg3.SetAskMode(ASK_EQUIP_ALL);

			for(i=0; i<HEQUIP_NUMBER; i++){
				//if(pFullUserData->m_Equip.IsSet((HUMAN_EQUIP)i) == TRUE){
					const _ITEM* pEquip = pFullUserData->m_Equip.GetEquipDB((HUMAN_EQUIP)i);
					if(pEquip && !pEquip->IsNullType() && (pEquip->ItemClass() == ICLASS_EQUIP) )
						Msg3.SetEquipData((HUMAN_EQUIP)i, pEquip);
				//}
			}			
			
			pServerPlayer->SendPacket(&Msg3);
		}
		*/
	}
	else{
		// ֪ͨClient����Ҳ�����
		WGOperateResult Msg;
		Msg.SetPlayerID(pPacket->GetPlayerID());
		Msg.setResult(OR_THIS_PLAYER_IS_OFFLINE);
		
		pSourceServerPlayer->SendPacket(&Msg);
	}
	
	Log::SaveLog( WORLD_LOGFILE, "GWAskOtherOnlineAttrib_HumanHandler: ok guid=%X", guid);

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}


}
