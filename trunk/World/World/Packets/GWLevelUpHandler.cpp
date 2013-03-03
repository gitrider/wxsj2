/********************************************************************************
*	�ļ�����	GWLevelUpHandler.cpp
*	ȫ·����	d:\Prj\World\World\Packets\GWLevelUpHandler.cpp
*	�����ˣ�	����
*	����ʱ�䣺	2005 �� 11 �� 9 ��	15:09
*
*	����˵����	
*	�޸ļ�¼��
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
	{//û�з����ڡ������û��б����Ӧ��GUID���û�
		Log::SaveLog( WORLD_LOGFILE, "GWLevelUpHandler...User GUID=%X not find!",
			pPacket->GetGUID() );

		Assert( FALSE && "�Ҳ�������" );
		return PACKET_EXE_CONTINUE;
	}

	INT	oldLevel = pUser->GetLevel();
	pUser->SetLevel( pPacket->GetLevel() );

	//�������а�
	LevelTopList* pTopList = (LevelTopList*)g_pTopListManager->GetTopList(LEVEL_TOP_LIST);

	//�������ֻ�ǲ��Ե�ʱ����������ã����������Ӧ��һ��ֻ����һ��
	pTopList->HaveOldLevelRank();
	pTopList->UpDataLevelTopList(pUser);
	
	//if ( pUser->GetLevel() >= pTopList->GetEnterMinKey() )
	//{
	//	GUID_t guid = pTopList->GetMinKeyOwner();
	//	_LEVEL_TOP_DATA *pMinData = (_LEVEL_TOP_DATA *)pTopList->Get(guid);
	//	
	//	//�ж��Լ��Ƿ���ԭ����
	//	_LEVEL_TOP_DATA * pMyself = (_LEVEL_TOP_DATA*)pTopList->Get(pUser->GetGUID());
	//	if ( pMyself != NULL )
	//	{//����ԭ������
	//		pMyself->m_Exp = pPacket->GetExp();
	//		
	//		//��������������
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
	//		//��Ҫ������������а�����������ʹ�����������ָ��ָ������ֵ
	//		//
	//	}
	//	else
	//	{
	//		if ( pTopList->GetListRecord() < pTopList->GetMaxListSize() 
	//			|| pUser->GetLevel() > pMinData->m_UserData.m_Level )
	//		{
	//			_LEVEL_TOP_DATA *pData = pTopList->GetNullNode();

	//			//��������������
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
