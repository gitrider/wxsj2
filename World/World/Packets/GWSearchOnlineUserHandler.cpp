

#include "stdafx.h"
#include "GWSearchOnlineUser.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGReturnOnlineUser.h"
#include "GameStruct_Query.h"
#include "GuildManager.h"
#include "Array_Base.h"

uint GWSearchOnlineUserHandler::Execute( GWSearchOnlineUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );
	if(pUser){
		
		INT iReturnPos = 0;
		BOOL bContinue = FALSE;

		//////////////////////////////////////////////////////////////////////////
		// Allocates memory on the stack for returnUSERVector
		//_alloca(uSize),放弃使用_alloca，该api在linux下强烈建议不要使用，另外在windows下还要进行SEH处理
		UINT uSize = (UINT)(sizeof(USER*)*OnlineUser_SameCity_PerPage::MAX_ONLINEUSER_NUMBER);
		Assert(uSize < 1024);
		CHAR szAlloc[1024] = {0};

		OnlineUser::PUSERVector returnUSERVector(
			OnlineUser_SameCity_PerPage::MAX_ONLINEUSER_NUMBER,
			(VOID*)(szAlloc)
			);

		BOOL bRet = g_pOnlineUser->Select(pPacket->GetPostCode(), 
				returnUSERVector, bContinue, iReturnPos, pPacket->GetMaxPostion());
		if(bRet){
			OnlineUser_SameCity_PerPage results;
			results.m_iCount = returnUSERVector.GetSize();
			for(INT i=0; i<results.m_iCount; ++i){

				const USER_SIMPLE_DATA* pUserSimpleData = returnUSERVector[i]->GetUserSimpleData();
				Assert(pUserSimpleData);

				results.m_aList[i].m_PlayerGUID = pUserSimpleData->m_GUID;
				const CHAR* pGuildName = pUserSimpleData->m_szGuildName;
				tsnprintf(results.m_aList[i].m_szName, MAX_CHARACTER_NAME-1, "%s", pUserSimpleData->m_Name);
				tsnprintf(results.m_aList[i].m_szGuildName, MAX_GUILD_NAME_SIZE-1, "%s", pGuildName);
				results.m_aList[i].m_iCountry = pUserSimpleData->m_nCountry;
				results.m_aList[i].m_iLevel = pUserSimpleData->m_nLevel;
				results.m_aList[i].m_iProfession = (INT)pUserSimpleData->m_uMenPai;
				results.m_aList[i].m_PostCode = pUserSimpleData->m_iPostCode;
			}

			WGReturnOnlineUser packet;
			packet.SetContinue(bContinue);
			packet.SetPlayerID(pPacket->GetPlayerID());
			packet.SetResults(results);
			packet.SetReturnPostion(iReturnPos);
			pServerPlayer->SendPacket(&packet);
		}
	}

	Log::SaveLog( WORLD_LOGFILE, "GWSearchOnlineUserHandler...User GUID=%X", pPacket->GetGUID());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
