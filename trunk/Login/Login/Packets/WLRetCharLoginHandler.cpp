#include "stdafx.h" 
#include "WLRetCharLogin.h"
#include "ProcessManager.h"
#include "ProcessPlayerManager.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "LCRetCharLogin.h"
#include "PlayerPool.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "DBCharFullData.h"
#include "DBCharExtradbVersion.h"
#include "DBCrcInfo.h"
#include "DB_Struct.h"
#include "LWAskCharLogin.h"
#include "LWNotifyUser.h"
#include "PacketFactoryManager.h"
#include "Log.h"
#include "DBThreadManager.h"
#include "CharConfig.h"
#include "IpTable.h"

//#define MAX_CRC_CHECK_TIMES 5
#define NO_MAGINT_TIME	513802240	//513802240<->2007-11-16 0:0:0

UINT WLRetCharLoginHandler::Execute(WLRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		TID CurrentThreadID = MyGetCurrentThreadID();
		Assert(pPacket);

		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			g_pProcessManager->SendPacket(pPacket,pPacket->GetPlayerID());

			return PACKET_EXE_NOTREMOVE;

		}
		else if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID) 
		{
			
			PlayerID_t		PlayerID   = pPacket->GetPlayerID();

			LoginPlayer* pLoginPlayer = g_pPlayerPool->GetPlayer(PlayerID);
			Assert(pLoginPlayer);

			if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....acc error,acc=%s,packacc=%s",
				pPacket->GetAccount(),pPacket->GetAccount());
				return PACKET_EXE_CONTINUE;
			}
			if(pPacket->GetHoldStatus() == TRUE) //�û�����
			{
				LCRetCharLogin Msg;
				Msg.SetResult(pPacket->GetResult());


				if(pPacket->GetResult() == ASKCHARLOGIN_SERVER_STOP)
				{
					pLoginPlayer->SendPacket(&Msg);
					g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....User is Online ,Server is CrashDown\
				   Account = %s,GUID = %d",pPacket->GetAccount(),pPacket->GetPlayerGUID());
					return PACKET_EXE_CONTINUE;
				}

				if(pPacket->GetResult() == ASKCHARLOGIN_NO_POINT)
				{
					pLoginPlayer->SendPacket(&Msg);
					g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....User Online ,No Point\
											   Account = %s,GUID = %d",pPacket->GetAccount(),pPacket->GetPlayerGUID());
					return PACKET_EXE_CONTINUE;

				}
				
				if(pPacket->GetResult() == ASKCHARLOGIN_END_TIME)
				{
					pLoginPlayer->SendPacket(&Msg);
					g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....User Online ,End Time\
											   Account = %s,GUID = %d",pPacket->GetAccount(),pPacket->GetPlayerGUID());
					return PACKET_EXE_CONTINUE;

				}
				if(pPacket->GetResult() == ASKCHARLOGIN_CHARLIVING)
				{
					pLoginPlayer->SendPacket(&Msg);
					g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....User Online ,Kicking \
											   Account = %s,GUID = %d",pPacket->GetAccount(),pPacket->GetPlayerGUID());
					return PACKET_EXE_CONTINUE;

				}
				if(pPacket->GetResult() == ASKCHARLOGIN_NOT_LOGON)
				{
					pLoginPlayer->SendPacket(&Msg);
					g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....User Online ,Not Login Before \
										  Account = %s,GUID = %d",pPacket->GetAccount(),pPacket->GetPlayerGUID());
					return PACKET_EXE_CONTINUE;
				}
	
				//�����ɫ���ڷ�������Ϣ
				ID_t	ServerID = pPacket->GetPlayerServerID();
				/*
				INT index = g_Config.m_ServerInfo.m_HashServer[ServerID] ;
				_SERVER_DATA* pServerData = 	&(g_Config.m_ServerInfo.m_pServer[index]);
				Assert(pServerData);
				Msg.SetServerIP(pServerData->m_IP0);
				Msg.SetServerPort(pServerData->m_Port0);
				*/

				INT nEnable = g_Config.m_LoginInfo.ProxyConnect;
				if(nEnable == PERMIT_EDU_PROXY_CONNECT || nEnable == FORBIT_PROXY_CONNECT)
				{
					Msg.SetServerIP(ProxyTool_GetIPByNetProvider(pLoginPlayer, ServerID));
					Msg.SetServerPort(ProxyTool_GetPortByNetProvider(pLoginPlayer, ServerID));
				}
				else/* if(nEnable == PERMIT_PROXY_CONNECT)*/
				{
					Msg.SetServerIP(ProxyTool_GetCorrespondingIP(pLoginPlayer, ServerID));
					Msg.SetServerPort(ProxyTool_GetCorrespondingPort(pLoginPlayer, ServerID));
				}
				//ȡ���û���½Keyֵ
				Msg.SetUserKey(pLoginPlayer->GetUserKey());
				// ���������ڵ�WorldID[12/13/2006]
				Msg.SetWorldId((BYTE)(g_Config.m_WorldInfo.m_WorldID));
				Msg.SetZoneID((UINT)(g_Config.m_WorldInfo.m_ZoneID));
				pLoginPlayer->SendPacket(&Msg);
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_SERVER_READY);
				g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()...Online ,Acc=%s,ProxyType=%d,GUID=%X,IP=%s,Port=%d,SID=%d",
					pPacket->GetAccount(),pLoginPlayer->GetProxyType(),pPacket->GetPlayerGUID(),
					Msg.GetServerIP(), Msg.GetServerPort(),
					//pServerData->m_IP0,pServerData->m_Port0,
					ServerID);

				return PACKET_EXE_CONTINUE;

			}
			else //�û������� ,��ҪLoad ����
			{
				//�����ݿ����Ƶ�����ж�
				UINT uTime = g_pTimeManager->CurrentTime();
				if(uTime<pLoginPlayer->m_LastDBOpTime+DB_OPERATION_TIME)
				{
					//�û���������Ƶ��
					LCRetCharLogin Msg;
					Msg.SetResult(ASKCHARLOGIN_OP_TIMES);
					pLoginPlayer->SendPacket(&Msg);
					
					g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....User Need Load From DB  Account = %s,GUID = %X",
						pPacket->GetAccount(),pPacket->GetPlayerGUID());
					return PACKET_EXE_CONTINUE;
				}
				//
				if(pPacket->GetResult() == ASKCHARLIST_WORLD_FULL 
					|| pPacket->GetResult() == ASKCHARLOGIN_SERVER_STOP)
				{
					LCRetCharLogin Msg;
					Msg.SetResult(pPacket->GetResult());
					pLoginPlayer->SendPacket(&Msg);
					g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....User Can't Login  Account = %s,GUID = %X ,Result = %d",
						pPacket->GetAccount(),pPacket->GetPlayerGUID(),pPacket->GetResult());
					return PACKET_EXE_CONTINUE;

				}
				
				if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
				{//����ɹ�������Ϣ���͵�DB����
					pLoginPlayer->m_LastDBOpTime = uTime;
					pLoginPlayer->ResetKick();
					return PACKET_EXE_NOTREMOVE;	
				}
				else
				{
					//DB ѹ���������û����³���
					LCRetCharLogin Msg;
					Msg.SetResult(ASKCHARLOGIN_SERVER_BUSY);
					pLoginPlayer->SendPacket(&Msg);
					pLoginPlayer->m_LastDBOpTime = uTime;
					
					g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....Server Busy  Account = %s,GUID = %X",
						pPacket->GetAccount(),pPacket->GetPlayerGUID());
					return PACKET_EXE_CONTINUE;
				}
			}

		}
		else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
		{

			PlayerID_t	PlayerID			= pPacket->GetPlayerID();
			GUID_t	    PlayerCharGUID		= pPacket->GetPlayerGUID();

			LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
			Assert(pLoginPlayer);

			ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
			if(!pInterface)
			{// ��Ϊ��ȫ���ʶ���ӵ�[4/16/2007]
				return PACKET_EXE_CONTINUE;
			}

			if(pLoginPlayer->GetDBOperating() == TRUE||!pInterface->IsConnected())
			{
				LCRetCharLogin* pRetCharLoginMsg = 
					(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);
				if(!pRetCharLoginMsg)
				{
					AssertEx(FALSE,"���� LCRetCharLogin ��Ϣʧ��");
				}
				pRetCharLoginMsg->SetResult(ASKCHARLOGIN_SERVER_BUSY);
				g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
				g_pLog->FastSaveLog(LOG_FILE_0, "WLRetCharLoginHandler::Execute()....���ݿ������ͻ! acc=%s",
					pLoginPlayer->GetAccount()) ;
				return PACKET_EXE_CONTINUE;
			}

			pLoginPlayer->SetDBOperating(TRUE);

			Assert(pInterface);

			DBCharFullData	CharFullDataObject(pInterface);
			CharFullDataObject.SetCharGuid(PlayerCharGUID);
			
			BOOL bRetLoad	 = CharFullDataObject.Load();
			INT	 ResultCount = CharFullDataObject.GetResultCount();
			if(!bRetLoad)
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"CharFullDataObject.Load()....acc = %s Get Errors: %s",
					pLoginPlayer->GetAccount(),
					CharFullDataObject.GetErrorMessage());
				
				LCRetCharLogin* pRetCharLoginMsg = 
					(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);

				if(!pRetCharLoginMsg)
				{
					AssertEx(FALSE,"���� LCRetCharLogin ��Ϣʧ��");
				}

				pRetCharLoginMsg->SetResult(ASKCHARLOGIN_LOADDB_ERROR);
				g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
				pLoginPlayer->SetDBOperating(FALSE);
				return PACKET_EXE_CONTINUE;
			}

			if(ResultCount == 0)
			{
				Assert(FALSE);
				LCRetCharLogin* pRetCharLoginMsg = 
					(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);

				pRetCharLoginMsg->SetResult(ASKCHARLOGIN_LOADDB_ERROR);
				g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
				pLoginPlayer->SetDBOperating(FALSE);
				return PACKET_EXE_CONTINUE;
			}

			LWAskCharLogin*		pMsg	=	
				(LWAskCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_ASKCHARLOGIN);
			if(!pMsg)
			{
				AssertEx(FALSE,"���� LWAskCharLogin ��Ϣʧ��");
			}

			CharFullDataObject.ParseResult(pMsg->GetUserData());
		
			//// yangjun[12/5/2006] !!!!!!!!!!! begin
			//// ������ڵ�dbVersion���ǲ���pMsg->GetUserData()->m_Human.m_DBVersion[12/5/2006]
			//// ��ΪLogin�����������ݵ�ʱ�򣬿���ShareMemory���ڴ洢���ݣ��Ӷ��޸���dbVersion
			//// ��¼�����У�һ��Ҫ����DBCharFullData::ParseResult������������壡
			//DBCharExtradbVersion  dbVersionObject(pInterface);
			//dbVersionObject.SetCharGuid(PlayerCharGUID);
			//BOOL bVersionCheck = dbVersionObject.Load();
			//if(bVersionCheck)
			//{
			//	dbVersionObject.ParseResult(pMsg->GetUserData());
			//	Assert(pMsg->GetUserData());
			//	if(pMsg->GetUserData()->m_Human.m_DBVersion != dbVersionObject.GetdbVersion())
			//	{
			//		g_pLog->FastSaveLog(LOG_FILE_0,"ERROR:DBCharExtradbVersion acc=%s,charguid=%X ,odbv=%d,cdbv=%d load fulluser from db ",
			//			pLoginPlayer->GetAccount(),PlayerCharGUID,pMsg->GetUserData()->m_Human.m_DBVersion,dbVersionObject.GetdbVersion());
			//		bVersionCheck = FALSE;
			//	}
			//}
			//
			//if(!bVersionCheck)
			//{
			//	Assert(FALSE);
			//	LCRetCharLogin* pRetCharLoginMsg = 
			//		(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);

			//	pRetCharLoginMsg->SetResult(ASKCHARLOGIN_LOADDB_ERROR);
			//	g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
			//	g_pPacketFactoryManager->RemovePacket(pMsg);	// fix memory leak[4/16/2007]
			//	pLoginPlayer->SetDBOperating(FALSE);
			//	return PACKET_EXE_CONTINUE;
			//}
			//// yangjun[12/5/2006] !!!!!!!!!!! end

			//���CharFullDataObject ����Ч��(CRC)
			UINT Crc32 = FullDataCrc(pMsg->GetUserData());
			BOOL bPass = (Crc32==CharFullDataObject.GetCRC());
			
			if(!bPass)
			{
				if( 0 != CharFullDataObject.GetCRC())
				{
					g_pLog->SaveLog(CRC32LOGIN_LOG,"ERROR:acc=%s,charguid=%X ,DbCrc=%d,BinCrc=%d load fulluser from db ",
						pLoginPlayer->GetAccount(),PlayerCharGUID,CharFullDataObject.GetCRC()+PlayerCharGUID,Crc32+PlayerCharGUID);

					LWNotifyUser *pCrcMsg = (LWNotifyUser*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_NOTIFYUSER);
					if(pCrcMsg)
					{//֪ͨWorld��������Mother����
						pCrcMsg->SetAccount(pLoginPlayer->GetAccount());
						pCrcMsg->SetNotifyStatus(LNOTIFY_CRC_ERR);

						pCrcMsg->SetCharGUID(PlayerCharGUID);
						pCrcMsg->SetSaveCrc(CharFullDataObject.GetCRC());
						pCrcMsg->SetLoadCrc(Crc32);
						pCrcMsg->SetUserKey(pMsg->GetUserData()->m_Human.m_LastLoginTime) ;//������key���������͵�½ʱ��

						g_pServerManager->SendPacket(pCrcMsg, WORLD_PLAYER_ID);
					}
					
					// ������ʱֻ��¼��ͬ��Crc,�����з��[1/12/2007]
					//LCRetCharLogin* pRetCharLoginMsg = 
					//	(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);

					//pRetCharLoginMsg->SetResult(ASKCHARLOGIN_CRC_ERROR);
					//g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
					//pLoginPlayer->SetDBOperating(FALSE);
					//return PACKET_EXE_CONTINUE;

					//��������¼���ļ�
					{
						CHAR filename[_MAX_PATH]={0} ;
						sprintf( filename, "./errUsr/%Xl_%u.%.4d-%.2d-%.2d-%.2d-%.2d-%.2d.usr", 
							PlayerCharGUID, Crc32,
							g_pTimeManager->GetYear(), g_pTimeManager->GetMonth()+1, g_pTimeManager->GetDay(),
							g_pTimeManager->GetHour(), g_pTimeManager->GetMinute(), g_pTimeManager->GetSecond() ) ;
						pMsg->GetUserData()->OutputToDisk( filename ) ;
#ifdef CRC32_ERROR_LOGTO_DB
						//���浽���ݿ�
						DBCrcInfo SaveCrcInfoObject(pInterface);
						SaveCrcInfoObject.SetCharGuid(PlayerCharGUID);
						SaveCrcInfoObject.SetCrc32(Crc32);
						SaveCrcInfoObject.SetServerId(100);
						if(SaveCrcInfoObject.Save(pMsg->GetUserData()))
						{
							SaveCrcInfoObject.ParseResult(pMsg->GetUserData());
						}
						
						//����crc��ͬ������
						DBCrcInfo ChangeCrcInfoObject(pInterface);
						ChangeCrcInfoObject.SetCharGuid(PlayerCharGUID);
						ChangeCrcInfoObject.SetChange(TRUE);
						if(ChangeCrcInfoObject.Delete())
						{
							ChangeCrcInfoObject.ParseResult(pMsg->GetUserData());
						}
#endif
					}
				}
				else
				{
					g_pLog->SaveLog(CRC32LOGIN_LOG,"INIT:acc=%s,charguid=%X Should init crc by ShareMemory",
						pLoginPlayer->GetAccount(),PlayerCharGUID);
				}
			}
			else
			{
#ifdef CRC32_ERROR_LOGTO_DB
				//ɾ��crc��ͬ������
				DBCrcInfo ChangeCrcInfoObject(pInterface);
				ChangeCrcInfoObject.SetCharGuid(PlayerCharGUID);
				ChangeCrcInfoObject.SetChange(FALSE);
				if(ChangeCrcInfoObject.Delete())
				{
					ChangeCrcInfoObject.ParseResult(pMsg->GetUserData());
				}
#endif
				g_pLog->SaveLog(CRC32LOGIN_LOG,"WLRetCharLoginHandler:acc=%s,charguid=%X ,DbCrc=%d,BinCrc=%d load fulluser from db ",
					pLoginPlayer->GetAccount(),PlayerCharGUID,CharFullDataObject.GetCRC(),Crc32);
			}
			
			//���MagicInt����
			INT nMagicIntNow	= 0;
			BYTE nMagicIntType	= pMsg->GetUserData()->GetMagicIntType();
			switch(nMagicIntType)
			{
			case MAGIC_INT_TYPE2:	//MagicInt�㷨2
				nMagicIntNow	= FullDataMagicInt2(pMsg->GetUserData());
				break;
			case MAGIC_INT_TYPE1:	//MagicInt�㷨1
			default:
				nMagicIntNow	= FullDataMagicInt1(pMsg->GetUserData());
				break;
			}

			INT nMagicIntSave	= pMsg->GetUserData()->GetMagicIntSetting();
			UINT nLogouttime	= pMsg->GetUserData()->m_Human.m_LastLogoutTime;

			if(nMagicIntNow == nMagicIntSave || (0 == nMagicIntSave /*&& NO_MAGINT_TIME > nLogouttime*/))
			{//MagicInt��������ǳ�ʱ������ͨ�����
				g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler:acc=%s,charguid=%X ,nMagicIntSave=%d,nMagicIntNow=%d,nLogouttime=%u MagicInt Check OK",
					pLoginPlayer->GetAccount(),PlayerCharGUID,nMagicIntSave,nMagicIntNow,nLogouttime);
			}
			else
			{
				//��¼��������־
				g_pLog->SaveLog(CRC32LOGIN_LOG,"MAGICINT:acc=%s,charguid=%X ,nMagicIntSave=%d,nMagicIntNow=%d,nLogouttime=%u MagicInt Check Error!!!",
					pLoginPlayer->GetAccount(),PlayerCharGUID,nMagicIntSave,nMagicIntNow,nLogouttime);
				//֪ͨWorld
				LWNotifyUser *pCrcMsg = (LWNotifyUser*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_NOTIFYUSER);
				if(pCrcMsg)
				{//֪ͨWorld��������Mother����
					pCrcMsg->SetAccount(pLoginPlayer->GetAccount());
					pCrcMsg->SetNotifyStatus(LNOTIFY_MAGICINT_ERR);

					pCrcMsg->SetCharGUID(PlayerCharGUID);
					pCrcMsg->SetSaveCrc(nMagicIntSave);
					pCrcMsg->SetLoadCrc(nMagicIntNow);
					pCrcMsg->SetUserKey(pMsg->GetUserData()->m_Human.m_LastLoginTime) ;//������key���������͵�½ʱ��

					g_pServerManager->SendPacket(pCrcMsg, WORLD_PLAYER_ID);
				}
				//֪ͨ�����ϵ�ͷ�
				LCRetCharLogin* pRetCharLoginMsg = 
					(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);
				if(pRetCharLoginMsg)
				{
					pRetCharLoginMsg->SetResult(ASKCHARLOGIN_MAGICINT_ERROR);
					g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
				}
				g_pPacketFactoryManager->RemovePacket(pMsg);	// fix memory leak[4/16/2007]
				pLoginPlayer->SetDBOperating(FALSE);
				return PACKET_EXE_CONTINUE;
			}
			
			//����ѡ�񳡾�
			if(pMsg->GetUserData()->m_Human.m_LastLoginTime==0)
			{
				if(g_DBSceneTable.isValidSceneID(pLoginPlayer->GetChooseSceneID(),CT_NORMAL))
				{
					pMsg->GetUserData()->m_Human.m_StartScene = pLoginPlayer->GetChooseSceneID();	
					g_DBSceneTable.GetPosBySceneID(pLoginPlayer->GetChooseSceneID(),
						CT_NORMAL,pMsg->GetUserData()->m_Human.m_Position);
				}
			}
		
			//// �����ϴεĵ�¼ʱ�䣬Server����ʾ��[12/12/2006]
			//pMsg->GetUserData()->m_Human.m_LastConnectServerTime = pMsg->GetUserData()->m_Human.m_LastLoginTime;
			////���÷������������
			//if(LANGUAGE_GBK == g_Config.m_ConfigInfo.m_nLanguage)
			//{
			//	pMsg->GetUserData()->m_Human.m_ActualAge = pLoginPlayer->GetAccFatigueSign()?1:20;
			//	pMsg->GetUserData()->m_Human.m_FatigueInfo.SetTotalOnlineTime(pLoginPlayer->GetAccOnlineTime());
			//}
			//pMsg->GetUserData()->m_Human.m_LastLoginTime	= g_pTimeManager->CurrentDate();
			//pMsg->GetUserData()->SetAccountSafeSign(pLoginPlayer->m_AccountSafeSign);
			////δ����ͬ��Ƶ�������ݣ����ݵ�ǰ��ҵ�¼��IP����ͬ��Ƶ��
			//if(INVALID_ID == pMsg->GetUserData()->m_Human.m_IPRegion)
			//{
			//	INT nIPRegion = g_pIPRegionTable->FindIPRegion(pLoginPlayer->GetSocket()->m_Host);
			//	pMsg->GetUserData()->m_Human.m_IPRegion = nIPRegion;

			//	g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....acc=%s,GUID=%X,Set IPRegion=%d",
			//		pLoginPlayer->GetAccount(),PlayerCharGUID,nIPRegion);
			//}
			pMsg->SetAskStatus(ALS_SENDDATA);
			pMsg->SetAccount(pLoginPlayer->GetAccount());
			//pMsg->SetHost(pLoginPlayer->GetSocket()->m_Host);
			pMsg->SetPlayerID(PlayerID);
			pMsg->SetPlayerGUID(PlayerCharGUID);
			pMsg->SetUserKey(pLoginPlayer->GetUserKey());

			g_pServerManager->SendPacket(pMsg,WORLD_PLAYER_ID);
			pLoginPlayer->SetDBOperating(FALSE);
			
			g_pLog->FastSaveLog(LOG_FILE_0,"WLRetCharLoginHandler::Execute()....OK!,acc=%s",
				pLoginPlayer->GetAccount());
		}
		else 
		{
			AssertEx(FALSE,"WLRetCharLoginHandler �߳���Դִ�д���!");
		}

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}