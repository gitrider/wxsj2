
#include "stdafx.h"
#include "GWRefreshDataToWorld.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "GuildManager.h"
#include "TopListManager.h"

VOID UpdateMoneyTopList(USER* pUser);

uint GWRefreshDataToWorldHandler::Execute( GWRefreshDataToWorld* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );
	if(pUser){
		USER_SIMPLE_DATA* pUserSimpleData = const_cast<USER_SIMPLE_DATA*>(pUser->GetUserSimpleData());
		Assert(pUserSimpleData);
		if(pPacket->IsUpdateAttrib(REFRESH_COUNTRY)){
			pUserSimpleData->m_nCountry = pPacket->GetCountry();
		}
		if(pPacket->IsUpdateAttrib(REFRESH_MENPAI)){
			pUserSimpleData->m_uMenPai = (UINT)pPacket->GetMenPai();
		}
		if(pPacket->IsUpdateAttrib(REFRESH_LEVEL)){
			pUserSimpleData->m_nLevel = pPacket->GetLevel();
		}
		if(pPacket->IsUpdateAttrib(REFRESH_GUILD)){
			pUserSimpleData->m_GuildID = pPacket->GetGuildID();
			const CHAR* szGuildName = g_pGuildManager->GetGuildName(pPacket->GetGuildID());
			tsnprintf_s(pUserSimpleData->m_szGuildName, MAX_GUILD_NAME_SIZE-1, szGuildName);
		}
		if(pPacket->IsUpdateAttrib(REFRESH_POSTCODE)){
			pUserSimpleData->m_iPostCode = pPacket->GetPostCode();
		}
		if (pPacket->IsUpdateAttrib(REFRESH_MONEY)){
			pUserSimpleData->m_uMoney = pPacket->GetMoney();
			UpdateMoneyTopList(pUser);
		}
		if (pPacket->IsUpdateAttrib(REFRESH_BANK_MONEY)){
			pUserSimpleData->m_uBankMoney = pPacket->GetMoney();
			UpdateMoneyTopList(pUser);
		}
	}

	Log::SaveLog( WORLD_LOGFILE, "GWRefreshDataToWorldHandler...User GUID=%X", pPacket->GetGUID());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

VOID UpdateMoneyTopList(USER* pUser)
{
	USER_SIMPLE_DATA* pUserSimpleData = const_cast<USER_SIMPLE_DATA*>(pUser->GetUserSimpleData());
	Assert(pUserSimpleData);
	UINT newMoney = pUserSimpleData->m_uMoney + pUserSimpleData->m_uBankMoney;

	MoneyTopList* pTopList = (MoneyTopList*)g_pTopListManager->GetTopList(MONEY_TOP_LIST);
	pTopList->HaveOldMoneyRank();
	pTopList->UpDataMoneyTopList(pUser,newMoney);
	//if ( pUser->GetLevel() >= pTopList->GetEnterMinKey() )
	//{
	//	//获得玩家的总在线时间
	//	FULLUSERDATA* FullUserData = pUser->GetFullUserData();

	//	GUID_t guid = pTopList->GetMinKeyOwner();
	//	_MONEY_TOP_DATA *pMinData = (_MONEY_TOP_DATA *)pTopList->Get(guid);

	//	//判断自己是否在原榜中
	//	_MONEY_TOP_DATA * pMyself = (_MONEY_TOP_DATA*)pTopList->Get(pUser->GetGUID());
	//	if ( pMyself != NULL )
	//	{//更新原有数据
	//		pMyself->m_Money = newMoney;
	//		if (FullUserData)
	//		{
	//			pMyself->m_OnLineTime = FullUserData->m_Human.m_OnlineTime;
	//		}

	//		pMyself->m_GuildNameSize = (BYTE)(strlen( pUser->GetGuildName() ));
	//		strncpy((char*)pMyself->m_GuildName,pUser->GetGuildName(),MAX_GUILD_NAME_SIZE-1);
	//		pMyself->m_UserData.m_Level	= pUser->GetLevel();
	//		pTopList->ModifyMinKeyOwner( pUser->GetGUID(),newMoney );
	//	}
	//	else
	//	{//自己不在原榜中
	//		if ( pTopList->GetListRecord() < pTopList->GetMaxListSize() 
	//			|| newMoney > pMinData->m_Money )
	//		{//插入新数据
	//			_MONEY_TOP_DATA *pData = pTopList->GetNullNode();
	//			pData->m_Money = newMoney;
	//			if (FullUserData)
	//			{
	//				pData->m_OnLineTime = FullUserData->m_Human.m_OnlineTime;
	//			}

	//			pData->m_GuildNameSize = (BYTE)(strlen( pUser->GetGuildName() ));
	//			strncpy((char*)pData->m_GuildName,pUser->GetGuildName(),MAX_GUILD_NAME_SIZE-1);

	//			pData->m_UserData.m_Country = pUser->GetCountry();
	//			pData->m_UserData.m_Guid	= pUser->GetGUID();
	//			pData->m_UserData.m_Level	= pUser->GetLevel();
	//			pData->m_UserData.m_MenPai = pUser->GetMenpai();
	//			strncpy((char*)pData->m_UserData.m_Name,pUser->GetName(),MAX_CHARACTER_NAME-1);
	//			pData->m_UserData.m_NameSize	= (INT)strlen(pData->m_UserData.m_Name);
	//			if ( pTopList->Add(pUser->GetGUID(),pData) )
	//				pTopList->ModifyMinKeyOwner( pUser->GetGUID(),newMoney );
	//		}
	//	}
	//}

}
