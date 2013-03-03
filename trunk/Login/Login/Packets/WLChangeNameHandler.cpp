#include "stdafx.h" 
#include "WLChangeName.h"
#include "LWChangeName.h"
#include "DBThreadManager.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
//#include "DBCheckCharName.h"
#include "DBImmigration.h"
#include "Log.h"


UINT WLChangeNameHandler::Execute(WLChangeName* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
		
	TID CurrentThreadID = MyGetCurrentThreadID();

	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		Assert(pPacket);
		ServerPlayer* pServerPlayer  = (ServerPlayer*)pPlayer;
		Assert(pServerPlayer);

		WLChangeName *pNew = (WLChangeName*)g_pPacketFactoryManager->CreatePacket(PACKET_WL_CHANGENAME);
		if(pNew)
		{
			// 必须重新创建包，Login以前没有包传递的机制[2007/08/13 YangJun]
			pNew->m_sPacketUnit			= pPacket->m_sPacketUnit;

			if(g_pDBThreadManager->SendPacket(pNew,INVALID_ID))
			{//加入成功，将消息发送到DB处理
				return PACKET_EXE_CONTINUE;	
			}
			else
			{
				g_pPacketFactoryManager->RemovePacket(pNew);
				//DB 压力过大，让用户重新尝试
				LWChangeName Msg;
				Msg.m_sPacketUnit		= pPacket->m_sPacketUnit;
				Msg.m_sPacketUnit.SetResultType(Packets::CHGNAME_RE_DBBUSY);

				pServerPlayer->SendPacket(&Msg);
				
				g_pLog->FastSaveLog(LOG_FILE_0,"WLChangeNameHandler::Execute()....Server Busy  GUID = %X",
					pPacket->m_sPacketUnit.GetUserGuid());
				return PACKET_EXE_CONTINUE;
			}
		}
		return PACKET_EXE_CONTINUE;
	}
	else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
	{
		Assert(NULL == pPlayer);

		ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
		if(!pInterface)
		{
			return PACKET_EXE_CONTINUE;
		}

		LWChangeName* pMsg = (LWChangeName*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_CHANGENAME);
		if(pMsg)
		{
			pMsg->m_sPacketUnit			= pPacket->m_sPacketUnit;

			// t_char表里检查[2007/08/13 YangJun]
			DBImmigration ImmigrationInfo( pInterface );
			ImmigrationInfo.SetCharGuid(pPacket->m_sPacketUnit.GetUserGuid());
			ImmigrationInfo.SetCharName(pPacket->m_sPacketUnit.GetUserNewName());
			
			if(ImmigrationInfo.IsCharNameOK() == FALSE)
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"WLChangeNameHandler::Execute...ImmigrationInfo.IsCharNameOK()...GUID = %X Name=%s CHGNAME_RE_REPEATED",
					pPacket->m_sPacketUnit.GetUserGuid(),pPacket->m_sPacketUnit.GetUserNewName());

				pMsg->m_sPacketUnit.SetResultType(Packets::CHGNAME_RE_REPEATED);
				g_pServerManager->SendPacket(pMsg,WORLD_PLAYER_ID);	
				return PACKET_EXE_CONTINUE;
			}

			if ( ImmigrationInfo.UpdateNewCharName() == FALSE )
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"WLChangeNameHandler::Execute...ImmigrationInfo.UpdateNewCharName()...GUID = %X Get Errors: %s",
					pPacket->m_sPacketUnit.GetUserGuid(),ImmigrationInfo.GetErrorMessage());

				pMsg->m_sPacketUnit.SetResultType(Packets::CHGNAME_RE_NAMEERROR);
				g_pServerManager->SendPacket(pMsg,WORLD_PLAYER_ID);	
				return PACKET_EXE_CONTINUE;
			}

			// t_charname表里检查[2007/08/13 YangJun]
			//DBCheckCharName CheckCharName(pInterface);
			//CheckCharName.SetCharName(pPacket->m_sPacketUnit.GetUserNewName());
			//BOOL bRetCheckName = CheckCharName.Load();
			//if(!bRetCheckName)
			//{
			//	g_pLog->FastSaveLog(LOG_FILE_0,"WLChangeNameHandler::Execute...CheckCharName.Load()...GUID = %X Get Errors: %s",
			//		pPacket->m_sPacketUnit.GetUserGuid(),CheckCharName.GetErrorMessage());
			//	//检查名称失败
			//	pMsg->m_sPacketUnit.SetResultType(Packets::CHGNAME_RE_NAMEERROR);
			//	g_pServerManager->SendPacket(pMsg,WORLD_PLAYER_ID);
			//	return PACKET_EXE_CONTINUE;
			//}
			//bRetCheckName = CheckCharName.ParseResult(&Result);
			//if(!bRetCheckName)
			//{
			//	g_pLog->FastSaveLog(LOG_FILE_0,"WLChangeNameHandler::Execute...CheckCharName.ParseResult()...GUID = %X Get Errors: %s",
			//		pPacket->m_sPacketUnit.GetUserGuid(),CheckCharName.GetErrorMessage());

			//	pMsg->m_sPacketUnit.SetResultType(Packets::CHGNAME_RE_NAMEERROR);
			//	g_pServerManager->SendPacket(pMsg,WORLD_PLAYER_ID);	
			//	return PACKET_EXE_CONTINUE;
			//}

			pMsg->m_sPacketUnit.SetResultType(Packets::CHGNAME_RE_OK);
			g_pServerManager->SendPacket(pMsg,WORLD_PLAYER_ID);

			g_pLog->FastSaveLog(LOG_FILE_0,"WLChangeNameHandler::Execute...GUID=%X OldName=%s NewName=%s OK!",
				pPacket->m_sPacketUnit.GetUserGuid(),pPacket->m_sPacketUnit.GetUserOldName(),pPacket->m_sPacketUnit.GetUserNewName());
		}
		return PACKET_EXE_CONTINUE;
	}

	Assert(FALSE);
__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}