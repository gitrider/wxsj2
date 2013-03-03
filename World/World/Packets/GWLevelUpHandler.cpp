/********************************************************************************
*	文件名：	GWLevelUpHandler.cpp
*	全路径：	d:\Prj\World\World\Packets\GWLevelUpHandler.cpp
*	创建人：	胡繁
*	创建时间：	2005 年 11 月 9 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/
#include "stdafx.h"
#include "GWLevelUp.h"
#include "Log.h"
#include "OnlineUser.h"
#include "TopListManager.h"

using namespace Packets;

uint GWLevelUpHandler::Execute(GWLevelUp* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );
	if( pUser==NULL )
	{//没有发现在“在线用户列表”里对应此GUID的用户
		Log::SaveLog( WORLD_LOGFILE, "GWLevelUpHandler...User GUID=%X not find!",
			pPacket->GetGUID() );

		Assert( FALSE && "找不到此人" );
		return PACKET_EXE_CONTINUE;
	}

	INT	oldLevel = pUser->GetLevel();
	pUser->SetLevel( pPacket->GetLevel() );

	//更新排行榜
	LevelTopList* pTopList = (LevelTopList*)g_pTopListManager->GetTopList(LEVEL_TOP_LIST);

	//这个函数只是测试的时候在这里调用，正常情况下应该一天只调用一次
	pTopList->HaveOldLevelRank();
	pTopList->UpDataLevelTopList(pUser);
	
	//if ( pUser->GetLevel() >= pTopList->GetEnterMinKey() )
	//{
	//	GUID_t guid = pTopList->GetMinKeyOwner();
	//	_LEVEL_TOP_DATA *pMinData = (_LEVEL_TOP_DATA *)pTopList->Get(guid);
	//	
	//	//判断自己是否在原榜中
	//	_LEVEL_TOP_DATA * pMyself = (_LEVEL_TOP_DATA*)pTopList->Get(pUser->GetGUID());
	//	if ( pMyself != NULL )
	//	{//更新原有数据
	//		pMyself->m_Exp = pPacket->GetExp();
	//		
	//		//获得玩家完整数据
	//		FULLUSERDATA* FullData = pUser->GetFullUserData();
	//		if ( FullData )
	//		{
	//			pMyself->m_OnlineTime = FullData->m_Human.m_OnlineTime;
	//		}
	//		
	//		pMyself->m_GuildNameSize = (BYTE)(strlen( pUser->GetGuildName() ));
	//		strncpy((char*)pMyself->m_GuildName,pUser->GetGuildName(),MAX_GUILD_NAME_SIZE-1);
	//		pMyself->m_UserData.m_Level	= pUser->GetLevel();
	//		pTopList->ModifyMinKeyOwner( pUser->GetGUID(),pUser->GetLevel() );
	//		//
	//		//不要在这里进行排行榜排序，排序后会使快速索引表的指针指向错误的值
	//		//
	//	}
	//	else
	//	{
	//		if ( pTopList->GetListRecord() < pTopList->GetMaxListSize() 
	//			|| pUser->GetLevel() > pMinData->m_UserData.m_Level )
	//		{
	//			_LEVEL_TOP_DATA *pData = pTopList->GetNullNode();

	//			//获得玩家完整数据
	//			FULLUSERDATA* FullData = pUser->GetFullUserData();
	//			if ( FullData )
	//			{
	//				pData->m_OnlineTime = FullData->m_Human.m_OnlineTime;
	//			}
	//			
	//			pData->m_Exp = pPacket->GetExp();
	//			pData->m_GuildNameSize = (BYTE)(strlen( pUser->GetGuildName() ));
	//			strncpy((char*)pData->m_GuildName,pUser->GetGuildName(),MAX_GUILD_NAME_SIZE-1);

	//			pData->m_UserData.m_Country = pUser->GetCountry();
	//			pData->m_UserData.m_Guid	= pUser->GetGUID();
	//			pData->m_UserData.m_Level	= pUser->GetLevel();
	//			pData->m_UserData.m_MenPai = pUser->GetMenpai();
	//			strncpy((char*)pData->m_UserData.m_Name,pUser->GetName(),MAX_CHARACTER_NAME-1);
	//			pData->m_UserData.m_NameSize	= (INT)strlen(pData->m_UserData.m_Name);
	//			if ( pTopList->Add(pUser->GetGUID(),pData) )
	//				pTopList->ModifyMinKeyOwner( pUser->GetGUID(),pUser->GetLevel() );
	//		}
	//		
	//	}

	//}

	Log::SaveLog( WORLD_LOGFILE, "GWLevelUpHandler...User GUID=%X oldLevel=%d, NewLevel=%d", 
		pPacket->GetGUID(),oldLevel,pPacket->GetLevel() ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
