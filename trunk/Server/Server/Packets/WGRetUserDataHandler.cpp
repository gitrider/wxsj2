

#include "stdafx.h"
#include "WGRetUserData.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "IncomingPlayerManager.h"
#include "GCConnect.h"
#include "GUIDManager.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "HumanDB.h"

uint WGRetUserDataHandler::Execute( WGRetUserData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID ;
	GamePlayer* pGamePlayer = NULL ;

	Assert( pPlayer ) ;
	
	if( pPlayer->IsServerPlayer() )
	{
		PlayerID = pPacket->GetPlayerID() ;
		pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
		if( pGamePlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else if( pPlayer->IsGamePlayer() )
	{
		PlayerID = pPlayer->PlayerID() ;
		pGamePlayer = (GamePlayer*)pPlayer ;
	}
	else
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_SERVER_DATA )
	{//��ServerManagerģ����ִ�в���
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_SERVER_DATA_INCOMING ) ;

		g_pIncomingPlayerManager->SendPacket( pPacket, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(ret=%d) GUID=%X ServerPlayer...OK",
			pPacket->GetReturn(), pGamePlayer->m_HumanGUID ) ;

		return PACKET_EXE_NOTREMOVE ;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_SERVER_DATA_INCOMING )
	{//��IncomingPlayerManagerģ����ִ�е�
		Assert( MyGetCurrentThreadID()==g_pIncomingPlayerManager->m_ThreadID ) ;
	
		switch( pPacket->GetReturn() )
		{
		case UDR_NOTFINDUSERDATA:
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute UDR_NOTFINDUSERDATA(PID=%d,GUID=%X)",
					pPacket->GetPlayerID(),pGamePlayer->m_HumanGUID ) ;

				return PACKET_EXE_ERROR ;
			}
			break ;
		case UDR_USERDATA:
			{
				GCConnect Msg ;
				Msg.SetServerID( g_pServerManager->GetServerID() ) ;
				//��������
				SceneID_t sceneid = pPacket->GetUserData()->m_Human.m_StartScene ;
				WORLD_POS enterpos = pPacket->GetUserData()->m_Human.m_Position ;
				Msg.SetSceneID( sceneid ) ;
				Msg.SetWorldPos( &enterpos ) ;
				//��������

				pGamePlayer->InitHuman( pPacket->GetUserData(),UDR_USERDATA ) ;

				pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_ENTER ) ;

				pGamePlayer->SendPacket( &Msg ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(ret=%d) GUID=%X SceneID=%d Pos=(%.2f,%.2f)...OK ",
					pPacket->GetReturn(), pGamePlayer->m_HumanGUID, sceneid, enterpos.m_fX, enterpos.m_fZ ) ;
			
				return PACKET_EXE_CONTINUE ;
			}
			break ;
		
		case UDR_USERDATALIVING:
			{
				//ȡ���Ѿ��û���GUID���ݵ�Obj_Human��Ϣ
				Obj_Human* pOldHuman = (Obj_Human*)(g_pGUIDManager->Get(pGamePlayer->m_HumanGUID)) ;
				if( pOldHuman==NULL )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute Cannot find HumanData (GUID=%X)",
						pGamePlayer->m_HumanGUID ) ;

					return PACKET_EXE_ERROR ;
				}
				//ȡ��ӵ�д�GUID��Obj_Human���ڵĳ���ָ��
				Scene* pScene = pOldHuman->getScene() ;
				if( pScene==NULL )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute Scene==NULL (GUID=%X)",
						pGamePlayer->m_HumanGUID ) ;

					return PACKET_EXE_ERROR ;
				}

				((GamePlayer*)(pOldHuman->GetPlayer()))->SetPlayerStatus(PS_SERVER_ANOTHER_GUID_ENTER) ;
			
				//��˳������͵�ǰ��Ϣ
				pScene->SendPacket( pPacket, pOldHuman->GetPlayerID() ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(ret=%d) UDR_USERDATALIVING GUID=%X SceneID=%d ...OK ",
					pPacket->GetReturn(), pGamePlayer->m_HumanGUID, pScene->SceneID() ) ;

				return PACKET_EXE_NOTREMOVE ;
			}
			break ;
		case UDR_USERSERVERCRASH: //ȡ����Server Crash �������
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute UDR_USERSERVERCRASH(PID=%d, GUID=%X)",
					pPacket->GetPlayerID(), pGamePlayer->m_HumanGUID ) ;

				return PACKET_EXE_ERROR ;
			}
			break;
		case UDR_KEYERROR:
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute UDR_KEYERROR(PID=%d, GUID=%X)",
					pPacket->GetPlayerID(), pGamePlayer->m_HumanGUID ) ;

				return PACKET_EXE_ERROR ;
			}
			break ;
		default :
			{
				Assert(FALSE) ;
			}
			break ;
		}
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_ANOTHER_GUID_ENTER )
	{//�ɵ�������ڵĳ�����ִ�� Scene
		Obj_Human* pHuman = pGamePlayer->GetHuman() ;
		Assert( pHuman ) ;

		Scene* pScene = pHuman->getScene() ;
		if( pScene==NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR WGRetUserDataHandler::Scene==NULL(ret=%d) GUID=%X",
				pPacket->GetReturn(), pGamePlayer->m_HumanGUID) ;
			return PACKET_EXE_CONTINUE ;
		}

		//���ִ���̵߳������Ƿ���ȷ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
		
		GamePlayer* pNewPlayer = g_pPlayerPool->GetPlayer( pPacket->GetPlayerID() ) ;
		Assert( pNewPlayer ) ;

		Obj_Human* pNewHuman = pNewPlayer->GetHuman() ;
		Assert( pNewHuman ) ;
		{//������Ϣ����
			pPacket->SetReturn( UDR_USERDATA ) ;
			FULLUSERDATA* pData = pPacket->GetUserData() ;

			memcpy( &(pData->m_Human), pHuman->GetDB()->GetHumanDB(), sizeof(_HUMAN_DB_LOAD) ) ;
			memcpy( &(pData->m_Equip), pHuman->GetDB()->GetEquipDB(), sizeof(_EQUIP_DB_LOAD) ) ;
			memcpy( &(pData->m_Bag), pHuman->GetDB()->GetBag(), sizeof(_BAG_DB_LOAD) ) ;
			memcpy( &(pData->m_Bank), pHuman->GetDB()->GetBankDB(), sizeof(_BANK_DB_LOAD) ) ;
			memcpy( &(pData->m_Skill), pHuman->GetDB()->GetSkillDB(), sizeof(_SKILL_DB_LOAD) ) ;
			memcpy( &(pData->m_Cooldown), pHuman->GetDB()->GetCooldownDB(), sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN) ) ;
			memcpy( &(pData->m_XinFa), pHuman->GetDB()->GetXinFaDB(), sizeof(_XINFA_DB_LOAD) ) ;
			memcpy( &(pData->m_Impact), pHuman->GetDB()->GetImpactDB(), sizeof(_IMPACT_DB_LOAD) ) ;
			memcpy( &(pData->m_Ability), pHuman->GetDB()->GetAbilityDB(), sizeof(_ABILITY_DB_LOAD) ) ;
			memcpy( &(pData->m_Mission), pHuman->GetDB()->GetMissionDB(), sizeof(_MISSION_DB_LOAD) ) ;
			memcpy( &(pData->m_Setting), pHuman->GetDB()->GetSettingDB(), sizeof(_SETTING_DB_LOAD) ) ;
			memcpy( &(pData->m_PetList), pHuman->GetDB()->m_dbPetList, sizeof(_PET_DB_LIST_LOAD) ) ;
			memcpy( &(pData->m_Relation), pHuman->GetDB()->GetRelationDB(), sizeof(_RELATION_DB_LOAD) ) ;
			memcpy( &(pData->m_PrivateInfo), pHuman->GetDB()->GetPrivateInfoDB(), sizeof(_PRIVATE_INFO_DB_LOAD) ) ;
			memcpy( &(pData->m_SouXia), pHuman->GetDB()->GetSouXiaDB(), sizeof(_SOUXIA_DB_LOAD) ) ; // add by gh 2010/05/10
			memcpy( &(pData->m_Mount), pHuman->GetDB()->GetMountDB(), sizeof(_MOUNT_DB_LOAD) ) ;    // [7/8/2010 �¾���]
		 // pData->m_bIsPasswdUnlock = pHuman->__IsPasswordUnlock(); // ��ʱ��������
		}

		pGamePlayer->SetDirty( TRUE ) ;

		g_pIncomingPlayerManager->SendPacket( pPacket, pNewPlayer->PlayerID() ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(ret=%d) GUID=%X SceneID=%d IncomingPlayerManager...OK ",
			pPacket->GetReturn(), pGamePlayer->m_HumanGUID, pScene->SceneID() ) ;

		return PACKET_EXE_NOTREMOVE_ERROR ;
	}

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

