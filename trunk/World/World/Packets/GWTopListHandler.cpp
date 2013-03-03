
#include "stdafx.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "Log.h"
#include "GWTopList.h"
#include "User.h"
#include "WGTopList.h"
#include "TopListManager.h"

uint	GWTopListHandler::Execute(GWTopList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GUID_t guid = pPacket->GetGUID();

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTopListHandler...User GUID=%X not find!", 
			guid );
		return PACKET_EXE_CONTINUE;
	}

	WGTopList Msg;
	Msg.SetPlayerID(pUser->GetPlayerID());
	WORLD_TOP_LIST* pTop = Msg.GetTopList();

	switch( pPacket->GetType() )
	{
	case LEVEL_TOP_LIST:		//等级排行榜
		{
			LevelTopList* pLevelList = (LevelTopList*)g_pTopListManager->GetTopList(LEVEL_TOP_LIST);
			if (!pLevelList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pLevelList->GetListRecord() > pLevelList->GetMaxListSize())
				pTop->m_DataCount = pLevelList->GetMaxListSize();
			else
				pTop->m_DataCount = pLevelList->GetListRecord();

			for ( INT i=0;i<pTop->m_DataCount;i++ )
			{
				memcpy( pTop->m_LevelDataEx+i,pLevelList->GetData(i),sizeof(_LEVEL_TOP_DATA));
			}

		}
		break;
	case MONEY_TOP_LIST:		//游戏币排行榜
		{
			MoneyTopList* pMoneyList = (MoneyTopList*)g_pTopListManager->GetTopList(MONEY_TOP_LIST);
			if (!pMoneyList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pMoneyList->GetListRecord() > pMoneyList->GetMaxListSize())
				pTop->m_DataCount = pMoneyList->GetMaxListSize();
			else
				pTop->m_DataCount = pMoneyList->GetListRecord();

			for ( INT i=0;i<pTop->m_DataCount;i++ )
			{
				memcpy(pTop->m_MoneyDataEx+i,pMoneyList->GetData((i)),sizeof(_MONEY_TOP_DATA) );
			}

		}
		break;
	case REPUTE_TOP_LIST:		//声望排行榜
		{
			ReputeTopList* pReputeList = (ReputeTopList*)g_pTopListManager->GetTopList(REPUTE_TOP_LIST);
			if (!pReputeList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pReputeList->GetListRecord() > pReputeList->GetMaxListSize() )
				pTop->m_DataCount = pReputeList->GetMaxListSize();
			else
				pTop->m_DataCount = pReputeList->GetListRecord();
			
			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_ReputeDataEx+i,pReputeList->GetData(i), sizeof(_REPUTE_TOP_DATA) );
			}
		}
		break;
	case PET_TOP_LIST:			//宠物排行榜
		{
			PetTopList* pPetList = (PetTopList*)g_pTopListManager->GetTopList(PET_TOP_LIST);
			if (!pPetList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pPetList->GetListRecord() > pPetList->GetMaxListSize() )
				pTop->m_DataCount = pPetList->GetMaxListSize();
			else
				pTop->m_DataCount = pPetList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_PetDataEx+i,pPetList->GetData(i), sizeof(_PET_TOP_DATA) );
			}
		}
		break;
	case ECTYPE_TOP_LIST:		//副本排行榜
		{
			EctypeTopList* pEctypeList = (EctypeTopList*)g_pTopListManager->GetTopList(ECTYPE_TOP_LIST);
			if (!pEctypeList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pEctypeList->GetListRecord() > pEctypeList->GetMaxListSize() )
				pTop->m_DataCount = pEctypeList->GetMaxListSize();
			else
				pTop->m_DataCount = pEctypeList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_EctypeDataEx+i,pEctypeList->GetData(i), sizeof(_ECTYPE_TOP_DATA) );
			}
		}
		break;
	case ERRANTRY_TOP_LIST:		//侠义排行榜
		{
			ErrantryTopList* pErrantryList = (ErrantryTopList*)g_pTopListManager->GetTopList(ERRANTRY_TOP_LIST);
			if (!pErrantryList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pErrantryList->GetListRecord() > pErrantryList->GetMaxListSize() )
				pTop->m_DataCount = pErrantryList->GetMaxListSize();
			else
				pTop->m_DataCount = pErrantryList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_ErrantryDataEx+i,pErrantryList->GetData(i), sizeof(_ERRANTRY_TOP_DATA) );
			}
		}
		break;
	case CRIME_TOP_LIST:		//罪恶排行榜
		{
			CrimeTopList* pCrimeList = (CrimeTopList*)g_pTopListManager->GetTopList(CRIME_TOP_LIST);
			if (!pCrimeList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pCrimeList->GetListRecord() > pCrimeList->GetMaxListSize() )
				pTop->m_DataCount = pCrimeList->GetMaxListSize();
			else
				pTop->m_DataCount = pCrimeList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_CrimeDataEx+i,pCrimeList->GetData(i), sizeof(_CRIME_TOP_DATA) );
			}
		}
		break;
	case BINDMONEY_TOP_LIST:		//绑定游戏币排行榜
		{
			MoneyTopList* pBindMoneyList = (MoneyTopList*)g_pTopListManager->GetTopList(BINDMONEY_TOP_LIST);
			if (!pBindMoneyList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pBindMoneyList->GetListRecord() > pBindMoneyList->GetMaxListSize() )
				pTop->m_DataCount = pBindMoneyList->GetMaxListSize();
			else
				pTop->m_DataCount = pBindMoneyList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_BindMoneyDataEx+i,pBindMoneyList->GetData(i), sizeof(_MONEY_TOP_DATA) );
			}
		}
		break;
	case RMB_TOP_LIST:			//人民币排行榜
		{
			MoneyTopList* pRMBList = (MoneyTopList*)g_pTopListManager->GetTopList(RMB_TOP_LIST);
			if (!pRMBList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pRMBList->GetListRecord() > pRMBList->GetMaxListSize() )
				pTop->m_DataCount = pRMBList->GetMaxListSize();
			else
				pTop->m_DataCount = pRMBList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_RMBDataEx+i,pRMBList->GetData(i), sizeof(_MONEY_TOP_DATA) );
			}
		}
		break;
	case BINDRMB_TOP_LIST:			//绑定人民币排行榜
		{
			MoneyTopList* pBindRMBList = (MoneyTopList*)g_pTopListManager->GetTopList(BINDMONEY_TOP_LIST);
			if (!pBindRMBList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pBindRMBList->GetListRecord() > pBindRMBList->GetMaxListSize() )
				pTop->m_DataCount = pBindRMBList->GetMaxListSize();
			else
				pTop->m_DataCount = pBindRMBList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_BindRMBDataEx+i,pBindRMBList->GetData(i), sizeof(_MONEY_TOP_DATA) );
			}
		}
		break;
	case FAVOR_TOP_LIST:			//人气排行榜
		{
			FavorTopList* pFavorList = (FavorTopList*)g_pTopListManager->GetTopList(FAVOR_TOP_LIST);
			if (!pFavorList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pFavorList->GetListRecord() > pFavorList->GetMaxListSize() )
				pTop->m_DataCount = pFavorList->GetMaxListSize();
			else
				pTop->m_DataCount = pFavorList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_FavorDataEx+i,pFavorList->GetData(i), sizeof(_FAVOR_TOP_DATA) );
			}
		}
		break;
	case GUILD_TOP_LIST:			//帮派排行榜
		{
			GuildTopList* pGuildList = (GuildTopList*)g_pTopListManager->GetTopList(GUILD_TOP_LIST);
			if (!pGuildList)
			{
				return PACKET_EXE_CONTINUE;
			}

			if ( pGuildList->GetListRecord() > pGuildList->GetMaxListSize() )
				pTop->m_DataCount = pGuildList->GetMaxListSize();
			else
				pTop->m_DataCount = pGuildList->GetListRecord();

			for ( INT i=0; i<pTop->m_DataCount; i++)
			{
				memcpy(pTop->m_GuildDataEx+i,pGuildList->GetData(i), sizeof(_GUILD_TOP_DATA) );
			}
		}
		break;

	default:
		//Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	pTop->m_Type = pPacket->GetType();
	pServerPlayer->SendPacket(&Msg);

	Log::SaveLog( WORLD_LOGFILE, "GWTopListHandler...User GUID=%X", 
		guid );
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
