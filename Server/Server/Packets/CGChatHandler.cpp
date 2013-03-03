

#include "stdafx.h"
#include "CGChat.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ChatPipe.h"
#include "GCChat.h"
#include "GWChat.h"
#include "GameTable.h"
#include "DoubleMoodAction.h"


uint CGChatHandler::Execute( CGChat* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGChatHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGChatHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGChatHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	INT i ;
	BOOL bIsCanSay = TRUE ;
	BYTE bChatType = pPacket->GetChatType() ;
	//消耗判断
	for( i=0; i<g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeCount; i++ )
	{
		if( bIsCanSay==FALSE )
			break ;

		switch( g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeType[i] )
		{
		case CHAT_NEED_MP:
			{
				INT CurMP = pHuman->GetMP() ;
				INT NeedMP = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
				if( CurMP<NeedMP )
					bIsCanSay = FALSE ;
			}
			break ;
		case CHAT_NEED_VIGOR:
			{
				INT CurVigor = pHuman->GetVigor() ;
				INT NeedVigor = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
				if( CurVigor<NeedVigor )
					bIsCanSay = FALSE ;
			}
			break ;
		case CHAT_NEED_ENERGY:
			{
				INT CurEnergy = pHuman->GetEnergy() ;
				INT NeedEnergy = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
				if( CurEnergy<NeedEnergy )
					bIsCanSay = FALSE ;
			}
			break ;
		default :
			break ;
		};
	}

	if( bIsCanSay == TRUE )//等级判断
	{
		if( pHuman->GetLevel() < g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_NeedLevel )
		{
			bIsCanSay = FALSE ;
		}
	}

	if( bIsCanSay == FALSE )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGChatHandler: Consume Error! GUID=%X ChatType=%d Contex=%s TarName=%s TeamID=%d ChannelID=%d GuildID=%d", 
			pGamePlayer->m_HumanGUID, 
			pPacket->GetChatType(), 
			pPacket->GetContex(), 
			pPacket->GetTargetName(), 
			pPacket->GetTeamID(), 
			/*****pPacket->GetChannelID(),*****/
			pPacket->GetGuildID() ) ;

		return PACKET_EXE_CONTINUE ;
	}

	for( i=0; i<g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeCount; i++ )
	{
		switch( g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeType[i] )
		{
		case CHAT_NEED_MP:
			{
				INT CurMP = pHuman->GetMP() ;
				INT NeedMP = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
				pHuman->SetMP( CurMP-NeedMP ) ;
			}
			break ;
		case CHAT_NEED_VIGOR:
			{
				INT CurVigor = pHuman->GetVigor() ;
				INT NeedVigor = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
				pHuman->SetVigor( CurVigor-NeedVigor ) ;
			}
			break ;
		case CHAT_NEED_ENERGY:
			{
				INT CurEnergy = pHuman->GetEnergy() ;
				INT NeedEnergy = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
				pHuman->SetEnergy( CurEnergy-NeedEnergy ) ;
			}
			break ;
		default :
			break ;
		};
	}

	switch( bChatType )
	{
	case CHAT_TYPE_NEAR:
		{
			ObjID_t SourID = pHuman->GetID() ;
			ObjID_t DestID = INVALID_ID ;
			GCChat* pChatPacket = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;

			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( pPacket->GetContexSize() ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetSourNameSize( (BYTE)(strlen(pHuman->GetName())) ) ;
			pChatPacket->SetSourName( pHuman->GetName() ) ;
			pChatPacket->SetSourID(pHuman->GetID()) ;

			pScene->GetChatPipe()->SendPacket( pChatPacket, SourID, DestID ) ;
		}
		break ;
	case CHAT_TYPE_TEAM:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( pPacket->GetContexSize() ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetTeamID( pPacket->GetTeamID() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_USER:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( pPacket->GetContexSize() ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetTargetName( pPacket->GetTargetName() ) ;
			pChatPacket->SetTargetSize( pPacket->GetTargetSize() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_WORLD:
	case CHAT_TYPE_SYSTEM:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( pPacket->GetContexSize() ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_GUILD:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( pPacket->GetContexSize() ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetGuildID( pPacket->GetGuildID() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_JIAZU:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( pPacket->GetContexSize() ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetGuildID( pPacket->GetGuildID() );
			pChatPacket->SetJiaZuID( pPacket->GetJiaZuID() );

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_COUNTRY:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( pPacket->GetContexSize() ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetCountryID( pPacket->GetCountryID() );

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_DMA_FORCE:		//双人强制
	case CHAT_TYPE_DMA_REQUEST:		//双人邀请
	case CHAT_TYPE_DMA_AGREE:		//双人同意
	case CHAT_TYPE_DMA_REFUSE:		//双人拒绝
	case CHAT_TYPE_DMA_CANCEL:		//双人停止
		{
			Obj_Human* pDest = pScene->GetHumanManager()->GetHuman(pPacket->GetDestID());
			if(pDest){// 目标玩家又可能掉线 [6/5/2008 wuwenbin]
				if(pDest->GetObjType() == Obj::OBJ_TYPE_HUMAN){
					DoubleMoodAction::GetInstance().ProcessDoubleMoodAction(
						pPacket->GetChatType(),
						pPacket->GetContex(), 
						pHuman, 
						pDest);
				}
			}
		}
		break;
	default :
		{
			Assert( FALSE ) ;
		}
		break ;
	};

	g_pLog->FastSaveLog( LOG_FILE_12, "CGChatHandler: GUID=%X ChatType=%d Contex=%s TarName=%s TeamID=%d ChannelID=%d GuildID=%d", 
		pGamePlayer->m_HumanGUID, 
		pPacket->GetChatType(), 
		pPacket->GetContex(), 
		pPacket->GetTargetName(), 
		pPacket->GetTeamID(), 
		pPacket->GetJiaZuID(),
		pPacket->GetGuildID() ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
