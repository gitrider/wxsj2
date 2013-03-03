#include "stdafx.h"
#include "LWAskCharLogin.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WLRetCharLogin.h"
#include "Log.h"
#include "ServerManager.h"
#include "WGNotifyUser.h"
#include "WLBillingStart.h"
#include "TimeManager.h"
#include "CountryManager.h"

#define MAXSTRINGLEN(s)				( sizeof(s)/sizeof(s[0]) - 1 )
#define strncat_good(a,b,c)			strncat_good_func((a),MAXSTRINGLEN(a),(b),(c))

void strncat_good_func( char* dest, size_t dest_size,const char* suffix, size_t n )
{
	if(!dest || !suffix)	return;
	strncat( dest, suffix, min( n, dest_size-strlen(dest)) );
}

UINT LWAskCharLoginHandler::Execute(LWAskCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	Assert(pPacket);
	
	ASK_LOGINSTATUS	AskStatus  = pPacket->GetAskStatus();
	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;
	Assert(pServerPlayer);

	USER* pUser;

	GUID_t	CharGuid = pPacket->GetPlayerGUID();

	if(AskStatus == ALS_ASKSTATUS)
	{
		
		WLRetCharLogin Msg;
		pUser = g_pOnlineUser->FindUser( CharGuid ) ;
		if(pUser) //用户存在，直接返回成功消息
		{
			if(pUser->UserStatus() == US_WAIT_SHUTDOWN) //服务器Crash
			{
				Msg.SetResult(ASKCHARLOGIN_SERVER_STOP);
				Msg.SetAccount(pPacket->GetAccount());
				Msg.SetPlayerID(pPacket->GetPlayerID());
				Msg.SetPlayerGUID(CharGuid);
				Msg.SetHoldStatus(TRUE);
				//Msg.SetPlayerServerID(pUser->GetServerID());
				pServerPlayer->SendPacket(&Msg);

				Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...User Hold,CharGUID = %X",CharGuid) ;
				return PACKET_EXE_CONTINUE;
			}
			else //正常游戏中
			{
				pUser->SetKey( pPacket->GetUserKey() );
				Msg.SetResult(ASKCHARLOGIN_CHARLIVING);
				Msg.SetAccount(pPacket->GetAccount());
				Msg.SetPlayerID(pPacket->GetPlayerID());
				Msg.SetPlayerGUID(CharGuid);
				Msg.SetHoldStatus(TRUE);
				//Msg.SetPlayerServerID(pUser->GetServerID());
				pServerPlayer->SendPacket(&Msg);

				//WGNotifyUser MsgNotify ;
				//MsgNotify.SetGUID( pUser->GetGUID() ) ;
				//MsgNotify.SetStatus( WGNotifyUser::NUS_NEED_SERVER_KICK ) ;
				//pServerPlayer = g_pServerManager->GetServerPlayer( pUser->GetServerID() );
				//if( pServerPlayer )
				//{
				//	pServerPlayer->SendPacket( &MsgNotify ) ;
				//}
				g_pOnlineUser->BeginKickUser(pUser);
				g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWAskCharLoginHandler...User Hold,Kick it,CharGUID = %X",CharGuid) ;
				return PACKET_EXE_CONTINUE;
			}
		}
		else //用户不存在
		{


			USER* pUser = g_pOnlineUser->FindUserByAccount(pPacket->GetAccount());
			if(pUser)
			{
				Msg.SetResult(ASKCHARLOGIN_CHARLIVING);
				Msg.SetAccount(pPacket->GetAccount());
				Msg.SetPlayerID(pPacket->GetPlayerID());
				Msg.SetPlayerGUID(pUser->GetGUID());
				Msg.SetHoldStatus(TRUE);
				//Msg.SetPlayerServerID(pUser->GetServerID());
				pServerPlayer->SendPacket(&Msg);

				//WGNotifyUser MsgNotify ;
				//MsgNotify.SetGUID( pUser->GetGUID() ) ;
				//MsgNotify.SetStatus( WGNotifyUser::NUS_NEED_SERVER_KICK ) ;
				//pServerPlayer = g_pServerManager->GetServerPlayer( pUser->GetServerID() );
				//if( pServerPlayer )
				//{
				//	pServerPlayer->SendPacket( &MsgNotify ) ;
				//}
				g_pOnlineUser->BeginKickUser(pUser);
				g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWAskCharLoginHandler...Other User Hold,Kick it,CharGUID = %X",pUser->GetGUID()) ;
				return PACKET_EXE_CONTINUE;

			}
			else
			{
				Msg.SetResult(ASKCHARLOGIN_SUCCESS);
				Msg.SetAccount(pPacket->GetAccount());
				Msg.SetPlayerID(pPacket->GetPlayerID());
				Msg.SetPlayerGUID(CharGuid);
				Msg.SetHoldStatus(FALSE);
				pServerPlayer->SendPacket(&Msg);
				Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...No User,CharGUID = %X",CharGuid) ;
			}
				return PACKET_EXE_CONTINUE;	
		}
	}
	else if(AskStatus == ALS_SENDDATA)
	{
		pUser = g_pOnlineUser->FindUser( CharGuid ) ;
		if( pUser )
		{//如果此用户存在
			Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...Char Exist! CharGUID = %X",CharGuid) ;
			return PACKET_EXE_CONTINUE;
		}
		
		if( pPacket->GetUserData()->m_Human.m_StartScene==INVALID_ID ||
			pPacket->GetUserData()->m_Human.m_StartScene>=(INT)g_Config.m_SceneInfo.m_SceneCount )
		{
			pPacket->GetUserData()->m_Human.m_StartScene = 2 ;
			pPacket->GetUserData()->m_Human.m_Position.m_fX = 160.0f ;
			pPacket->GetUserData()->m_Human.m_Position.m_fZ = 130.0f ;
		}

		ID_t	ServerID = g_Config.SceneID2ServerID(pPacket->GetUserData()->m_Human.m_StartScene);

		ServerPlayer* pFindPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
		if( pFindPlayer==NULL )
		{
			WLRetCharLogin Msg;
			Msg.SetResult(ASKCHARLOGIN_SERVER_STOP);
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetPlayerID(pPacket->GetPlayerID());
			Msg.SetPlayerGUID(CharGuid);
			pServerPlayer->SendPacket(&Msg);
			Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...Server Stop CharGUID = %X",CharGuid) ;
			return PACKET_EXE_CONTINUE;
		}	


		pUser	 = g_pOnlineUser->NewUser();

		if(!pUser) //满了
		{
			WLRetCharLogin Msg;
			Msg.SetResult(ASKCHARLIST_WORLD_FULL);
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetPlayerID(pPacket->GetPlayerID());
			Msg.SetPlayerGUID(CharGuid);
			pServerPlayer->SendPacket(&Msg);

			Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...World Full CharGUID = %X",CharGuid) ;
			return PACKET_EXE_CONTINUE;
		}

		pUser->CleanUp();

		FULLUSERDATA* pFullUserData = g_pOnlineUser->NewData();
		Assert(pFullUserData);
		memcpy(pFullUserData,pPacket->GetUserData(),sizeof(FULLUSERDATA));
		pUser->SetFullUserData( pFullUserData );
		pUser->SetKey( pPacket->GetUserKey() );
		pUser->SetUserStatus( US_HOLDUSERDATA );
		pUser->ResetKickTime() ;
		pUser->ResetSMU_KickTime();
		pUser->SetGUID( CharGuid );
		pUser->SetAccount(pPacket->GetAccount());
		//pFullUserData->SetAccName(pPacket->GetAccount());
		//pFullUserData->SetLoginHost(pPacket->GetHost());
		//pUser->SetHost(pPacket->GetHost());
		pUser->SetCountry(pPacket->GetUserData()->m_Human.m_nCountry);
		g_pOnlineUser->OnUserLogin( pUser ) ;
        g_pCountryManager->AddCountryUser((COUNTRY_ATTRIBUTE)pUser->GetCountry(), pUser->GetGUID());

		Assert(pFullUserData->m_Human.m_StartScene!=INVALID_ID);
		Assert(pFullUserData->m_Human.m_StartScene<(INT)g_Config.m_SceneInfo.m_SceneCount);
		pUser->SetSceneID( pFullUserData->m_Human.m_StartScene );

		/*
		WLRetCharLogin Msg;
		Msg.SetResult(ASKCHARLOGIN_SUCCESS);
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetPlayerID(pPacket->GetPlayerID());
		Msg.SetPlayerGUID(CharGuid);
		Msg.SetHoldStatus(TRUE);
		Msg.SetPlayerServerID(pUser->GetServerID());
		pServerPlayer->SendPacket(&Msg);
		*/

		// [ QUFEI 2007-09-10 19:24 UPDATE BugID 24699 ]
		//LoginCacheLog( pFullUserData );

		WLBillingStart	BillMsg;
		BillMsg.SetAccount(pPacket->GetAccount());
		BillMsg.SetCharName(pFullUserData->m_Human.m_Name);
		BillMsg.SetCharGUID(CharGuid);
		BillMsg.SetCharLevel(pFullUserData->m_Human.m_Level);
		UINT StartTime = (UINT)g_pTimeManager->GetANSITime();
		//pUser->SetStartTime(StartTime);
		BillMsg.SetStartTime(StartTime);
		BillMsg.SetPlayerID(pPacket->GetPlayerID());
		
		pServerPlayer->SendPacket(&BillMsg);
		
		g_pLog->FastSaveLog(LOG_FILE_4, "LWAskCharLoginHandler...ALSCENDDATA GUID=%X Name=%s AccName=%s", 
			CharGuid, pUser->GetName(), pUser->GetAccount() ) ;

		return PACKET_EXE_CONTINUE;
	}

	
	Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler::Execute()...GUID=%X OK",CharGuid) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

//VOID LWAskCharLoginHandler::LoginCacheLog(FULLUSERDATA* pUserData )
//{
//__ENTER_FUNCTION
//	Assert(pUserData);
//	
//	UINT nThisTime  = g_pTimeManager->CurrentDate();
//	
//	UINT nLogOutTime = pUserData->m_Human.m_LastLogoutTime;
//
//	//g_pLog->FastSaveLog(LOG_FILE_0, "ThisTime..... %d OK",nThisTime) ;
//	//g_pLog->FastSaveLog(LOG_FILE_0, "LogOutTime..... %d OK",nLogOutTime) ;
//	//g_pLog->FastSaveLog(LOG_FILE_0, "LogInCacheTime..... %d OK",g_Config.m_ConfigInfo.m_LogInCacheTime) ;
//	
//	INT nCurTime = g_pTimeManager->DiffTime( nLogOutTime, nThisTime )/3600000;
//	//g_pLog->FastSaveLog(LOG_FILE_0, "CurTime..... %d OK",nCurTime) ;
//
//	if ( nCurTime >= g_Config.m_ConfigInfo.m_LogInCacheTime )
//	{
//		//////////////////////////////////////////////////////////////////////
//		//玩家基本信息
//		int  idx = 1;
//		CHAR szBuffer[4096]={0};
//		CHAR szTemp[4096]={0};
//
//		memcpy( szBuffer, "", sizeof(szBuffer) );
//		memcpy( szTemp, "", sizeof(szTemp) );
//		tsnprintf(szBuffer, sizeof(szBuffer), "GUID =%X,  Name =%s,  AccName =%s,  Money =%d,  YuanBao =%d,  ZengDian =%d", pUserData->m_Human.m_GUID, pUserData->m_Human.m_Name, pUserData->m_AccName, pUserData->m_Human.m_Money, pUserData->m_Human.m_iYuanBao, pUserData->m_Human.m_nZengDian );
//		szBuffer[sizeof(szBuffer)-1] = '\0';
//
//		g_pLog->FastSaveLog(LOG_FILE_16, "PlayerBaseInfo..... %s OK",szBuffer) ;		//玩家身上的货币，元宝，赠点
//
//		//////////////////////////////////////////////////////////////////////
//		//玩家装备信息
//		memcpy( szBuffer, "", sizeof(szBuffer) );
//		memcpy( szTemp, "", sizeof(szTemp) );
//
//		for ( int i = 0; i < HEQUIP_NUMBER; i ++ )
//		{
//			if ( !pUserData->m_Equip.m_pItem[i].m_ItemGUID.isNull() )
//			{
//				if ( idx == 1 )
//				{
//					tsnprintf(szTemp, sizeof(szTemp), "GUID =%X,  Name =%s", pUserData->m_Human.m_GUID, pUserData->m_Human.m_Name );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					idx --;
//				}
//
//				tsnprintf(szTemp, sizeof(szTemp), "  EID[%d] =%d", i, pUserData->m_Equip.m_pItem[i].m_ItemGUID.m_Serial );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				tsnprintf(szTemp, sizeof(szTemp), "  EType[%d] =%d", i, pUserData->m_Equip.m_pItem[i].m_ItemIndex );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				
//				//强化等级
//				tsnprintf(szTemp, sizeof(szTemp), "  EnhanceLvl[%d] =%d", i, pUserData->m_Equip.m_pItem[i].m_Equip.m_Enhancelvl );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				//铭刻标志
//				tsnprintf(szTemp, sizeof(szTemp), "  Label[%d] =%d", i, pUserData->m_Equip.m_pItem[i].GetItemLock() );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				
//				//鉴定标志
//				tsnprintf(szTemp, sizeof(szTemp), "  Ident[%d] =%d", i, pUserData->m_Equip.m_pItem[i].GetItemIdent() );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				//星级
//				tsnprintf(szTemp, sizeof(szTemp), "  StarRank[%d] =%d", i, pUserData->m_Equip.m_pItem[i].m_Equip.GetQual() );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//				//装备 宝石数/打孔数
//				tsnprintf(szTemp, sizeof(szTemp), "  Hole[%d] =(%d / %d)", i, pUserData->m_Equip.m_pItem[i].m_Equip.m_StoneCount ,pUserData->m_Equip.m_pItem[i].m_Equip.m_GemMax );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				//镶嵌宝石
//				tsnprintf(szTemp, sizeof(szTemp), "  GemInfo[%d] =(", i  );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				for(int nGemId = 0; nGemId < pUserData->m_Equip.m_pItem[i].m_Equip.m_StoneCount; ++nGemId )
//				{
//					tsnprintf(szTemp, sizeof(szTemp), " %d ", pUserData->m_Equip.m_pItem[i].m_Equip.m_pGemInfo[nGemId].m_GemType  );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				}
//				strncat_good( szBuffer, ")", 1 );
//			}
//		}
//		szBuffer[sizeof(szBuffer)-1] = '\0';
//
//		g_pLog->FastSaveLog(LOG_FILE_16, "PlayerEquipInfo..... %s OK",szBuffer) ;		//玩家身上的所有装备ID,类型
//
//		/////////////////////////////////////////////////////////////////////
//		//玩家背包信息
//		idx = 1;
//		memcpy( szBuffer, "", sizeof(szBuffer) );
//		memcpy( szTemp, "", sizeof(szTemp) );
//
//		for ( int i = 0; i < MAX_BAG_SIZE; i ++ )
//		{
//			if ( !pUserData->m_Bag.m_pItem[i].m_ItemGUID.isNull() )
//			{
//				if ( idx == 1 )
//				{
//					tsnprintf(szTemp, sizeof(szTemp), "GUID =%X,  Name =%s", pUserData->m_Human.m_GUID, pUserData->m_Human.m_Name );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					idx --;
//				}
//
//				tsnprintf(szTemp, sizeof(szTemp), "  BGID[%d] =%d", i, pUserData->m_Bag.m_pItem[i].m_ItemGUID.m_Serial );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				tsnprintf(szTemp, sizeof(szTemp), "  BGType[%d] =%d", i, pUserData->m_Bag.m_pItem[i].m_ItemIndex );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				tsnprintf(szTemp, sizeof(szTemp), "  BGCnt[%d] =%d", i, pUserData->m_Bag.m_pItem[i].GetItemCount() );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//				if( pUserData->m_Bag.m_pItem[i].ItemClass() == ICLASS_EQUIP){
//					//强化等级
//					tsnprintf(szTemp, sizeof(szTemp), "  EnhanceLvl[%d] =%d", i, pUserData->m_Bag.m_pItem[i].m_Equip.m_Enhancelvl );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					//铭刻标志
//					tsnprintf(szTemp, sizeof(szTemp), "  Label[%d] =%d", i, pUserData->m_Bag.m_pItem[i].GetItemLock() );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//					//鉴定标志
//					tsnprintf(szTemp, sizeof(szTemp), "  Ident[%d] =%d", i, pUserData->m_Bag.m_pItem[i].GetItemIdent() );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					//星级
//					tsnprintf(szTemp, sizeof(szTemp), "  StarRank[%d] =%d", i, pUserData->m_Bag.m_pItem[i].m_Equip.GetQual() );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//					//装备 宝石/孔数
//					tsnprintf(szTemp, sizeof(szTemp), "  Hole[%d] =(%d / %d)", i, pUserData->m_Bag.m_pItem[i].m_Equip.m_StoneCount ,pUserData->m_Bag.m_pItem[i].m_Equip.m_GemMax );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					//镶嵌宝石
//					tsnprintf(szTemp, sizeof(szTemp), "  GemInfo[%d] =(", i  );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					for(int nGemId = 0; nGemId < pUserData->m_Bag.m_pItem[i].m_Equip.m_StoneCount; ++nGemId )
//					{
//						tsnprintf(szTemp, sizeof(szTemp), " %d ", pUserData->m_Bag.m_pItem[i].m_Equip.m_pGemInfo[nGemId].m_GemType  );
//						strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					}
//					strncat_good( szBuffer, ")", 1 );
//				}
//			}
//		}
//		szBuffer[sizeof(szBuffer)-1] = '\0';
//
//		g_pLog->FastSaveLog(LOG_FILE_16, "PlayerBagInfo..... %s OK",szBuffer) ;		//玩家背包的所有物品ID,类型,数量
//
//		/////////////////////////////////////////////////////////////////////
//		//玩家银行信息
//		idx = 1;
//		memcpy( szBuffer, "", sizeof(szBuffer) );
//		memcpy( szTemp, "", sizeof(szTemp) );
//
//		for ( int i = 0; i < MAX_BANK_SIZE; i ++ )
//		{
//			if ( !pUserData->m_Bank.m_pItem[i].m_ItemGUID.isNull() )
//			{
//				if ( idx == 1 )
//				{
//					tsnprintf(szTemp, sizeof(szTemp), "GUID =%X,  Name =%s", pUserData->m_Human.m_GUID, pUserData->m_Human.m_Name );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					idx --;
//				}
//
//				tsnprintf(szTemp, sizeof(szTemp), "  BKID[%d] =%d", i, pUserData->m_Bank.m_pItem[i].m_ItemGUID.m_Serial );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				tsnprintf(szTemp, sizeof(szTemp), "  BKType[%d] =%d", i, pUserData->m_Bank.m_pItem[i].m_ItemIndex );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				tsnprintf(szTemp, sizeof(szTemp), "  BKCnt[%d] =%d", i, pUserData->m_Bank.m_pItem[i].GetItemCount() );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//				if( pUserData->m_Bank.m_pItem[i].ItemClass() == ICLASS_EQUIP){
//					//强化等级
//					tsnprintf(szTemp, sizeof(szTemp), "  EnhanceLvl[%d] =%d", i, pUserData->m_Bank.m_pItem[i].m_Equip.m_Enhancelvl );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					//铭刻标志
//					tsnprintf(szTemp, sizeof(szTemp), "  Label[%d] =%d", i, pUserData->m_Bank.m_pItem[i].GetItemLock() );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//					//鉴定标志
//					tsnprintf(szTemp, sizeof(szTemp), "  Ident[%d] =%d", i, pUserData->m_Bank.m_pItem[i].GetItemIdent() );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					//星级
//					tsnprintf(szTemp, sizeof(szTemp), "  StarRank[%d] =%d", i, pUserData->m_Bank.m_pItem[i].m_Equip.GetQual() );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//					//装备 宝石/孔数
//					tsnprintf(szTemp, sizeof(szTemp), "  Hole[%d] =(%d / %d)", i, pUserData->m_Bank.m_pItem[i].m_Equip.m_StoneCount ,pUserData->m_Bank.m_pItem[i].m_Equip.m_GemMax );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					//镶嵌宝石
//					tsnprintf(szTemp, sizeof(szTemp), "  GemInfo[%d] =(", i  );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					for(int nGemId = 0; nGemId < pUserData->m_Bank.m_pItem[i].m_Equip.m_StoneCount; ++nGemId )
//					{
//						tsnprintf(szTemp, sizeof(szTemp), " %d ", pUserData->m_Bank.m_pItem[i].m_Equip.m_pGemInfo[nGemId].m_GemType  );
//						strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					}
//					strncat_good( szBuffer, ")", 1 );
//				}
//			}
//		}
//
//		tsnprintf(szTemp, sizeof(szTemp), "GUID =%X,  Name =%s", pUserData->m_Human.m_GUID, pUserData->m_Human.m_Name );
//		strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//		tsnprintf(szTemp, sizeof(szTemp), "  BKMoney =%d", pUserData->m_Bank.m_Money );
//		strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//		szBuffer[sizeof(szBuffer)-1] = '\0';
//
//		g_pLog->FastSaveLog(LOG_FILE_16, "PlayerBankInfo..... %s OK",szBuffer) ;		//玩家所拥有的银行里的物品ID,类型,数量
//
//		/////////////////////////////////////////////////////////////////////
//		//玩家的珍兽信息
//		idx = 1;
//		memcpy( szBuffer, "", sizeof(szBuffer) );
//		memcpy( szTemp, "", sizeof(szTemp) );
//
//		for ( int i = 0; i < HUMAN_PET_MAX_COUNT; i ++ )
//		{
//			if ( !pUserData->m_PetList.m_aPetDB[i].m_GUID.IsNull() )
//			{
//				if ( idx == 1 )
//				{
//					tsnprintf(szTemp, sizeof(szTemp), "GUID =%X,  Name =%s", pUserData->m_Human.m_GUID, pUserData->m_Human.m_Name );
//					strncat_good( szBuffer, szTemp, strlen(szTemp) );
//					idx --;
//				}
//
//				tsnprintf(szTemp, sizeof(szTemp), "  PHGUID[%d] =%X PLGUID[%d] =%X", 
//					i, pUserData->m_PetList.m_aPetDB[i].m_GUID.GetHighSection(),
//					i, pUserData->m_PetList.m_aPetDB[i].m_GUID.GetLowSection());
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//				tsnprintf(szTemp, sizeof(szTemp), "  PSHGUID[%d] =%X PSLGUID[%d] =%X", 
//					i, pUserData->m_PetList.m_aPetDB[i].m_SpouseGUID.GetHighSection(),
//					i, pUserData->m_PetList.m_aPetDB[i].m_SpouseGUID.GetLowSection());
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//				tsnprintf(szTemp, sizeof(szTemp), "  MID[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_nDataID );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				tsnprintf(szTemp, sizeof(szTemp), "  PName[%d] =%s", i, pUserData->m_PetList.m_aPetDB[i].m_szName );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				tsnprintf(szTemp, sizeof(szTemp), "  NName[%d] =%s", i, pUserData->m_PetList.m_aPetDB[i].m_szNick );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				tsnprintf(szTemp, sizeof(szTemp), "  PLevel[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_nLevel );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				
//				tsnprintf(szTemp, sizeof(szTemp), "  TLevel[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_nTakeLevel );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );				
//				tsnprintf(szTemp, sizeof(szTemp), "  PType[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_byPetType );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				
//				tsnprintf(szTemp, sizeof(szTemp), "  GRate[%d] =%f", i, pUserData->m_PetList.m_aPetDB[i].m_fGrowRate );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );				
//				tsnprintf(szTemp, sizeof(szTemp), "  PSavvy[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_nSavvy );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//				
//				//跟骨
//				tsnprintf(szTemp, sizeof(szTemp), "  GenGu[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_nGenGu );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );
//
//				tsnprintf(szTemp, sizeof(szTemp), "  Skill01[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_SkillList[0].m_nSkillID );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );				
//				tsnprintf(szTemp, sizeof(szTemp), "  Skill02[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_SkillList[1].m_nSkillID );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );				
//				tsnprintf(szTemp, sizeof(szTemp), "  Skill03[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_SkillList[2].m_nSkillID );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );				
//				tsnprintf(szTemp, sizeof(szTemp), "  Skill04[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_SkillList[3].m_nSkillID );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );				
//				tsnprintf(szTemp, sizeof(szTemp), "  Skill05[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_SkillList[4].m_nSkillID );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );				
//				tsnprintf(szTemp, sizeof(szTemp), "  Skill06[%d] =%d", i, pUserData->m_PetList.m_aPetDB[i].m_SkillList[5].m_nSkillID );
//				strncat_good( szBuffer, szTemp, strlen(szTemp) );								
//				
//			}
//		}
//		szBuffer[sizeof(szBuffer)-1] = '\0';
//
//		g_pLog->FastSaveLog(LOG_FILE_16, "PlayerPetInfo..... %s OK",szBuffer) ;		//玩家所拥有的珍兽信息
//
//	}
//__LEAVE_FUNCTION
//}
