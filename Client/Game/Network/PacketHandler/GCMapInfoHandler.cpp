#include "StdAfx.h"
#include "GCMapInfo.h"
#include "Procedure\GameProcedure.h"
#include "World\WorldManager.h"
#include "Event/GMEventSystem.h"
#include "DBC\GMDataBase.h"

#include "Object\Manager\ObjectManager.h"


uint GCMapInfoHandler::Execute( GCMapInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	switch( pPacket->GetType() )
	{
	case ASK_MAP_INFO:
		{
			_MAP_INFO* pMap = pPacket->GetMapInfo();
			for( int i = 0; i < pPacket->GetDataCount(); i++ )
			{
				std::vector< STRING > vParam;
				char szTemp[100];
				sprintf(szTemp, "0,%d", i);
				vParam.push_back( szTemp );

				sprintf( szTemp, "%s", pMap[i].m_SceneName);
				szTemp[ pMap[i].m_NameSize ] = '\0';
				vParam.push_back( szTemp );

				sprintf(szTemp, "%d,%d", (INT)pMap[i].m_DestPos.m_fX, (INT)pMap[i].m_DestPos.m_fZ);
				vParam.push_back( szTemp );

				sprintf(szTemp, "%d,%d", pMap[i].m_SceneType, pMap[i].m_MinMapLevel);
				vParam.push_back( szTemp );

				sprintf( szTemp, "%s", &pMap[i].m_LetterIndex);
				szTemp[1] = '\0';
				vParam.push_back( szTemp );

				CEventSystem::GetMe()->PushEvent( GE_UPDATE_MAP_LIST, vParam );
			}
		}
		break;

	case ASK_NPC_INF0:
		{
			_NPC_INFO* pNpc = pPacket->GetNPCInfo();
			const tDataBase* pDBC = CDataBaseSystem::GetMe()->GetDataBase( DBC_CREATURE_ATT );
	
			for( int i = 0; i < pPacket->GetDataCount(); i++ )
			{
				std::vector< STRING > vParam;
				char szTemp[100];
				sprintf(szTemp, "1,%d", i);
				vParam.push_back( szTemp );

				if( pDBC != NULL )
				{
					const _DBC_CREATURE_ATT* pMissionNpc = (const _DBC_CREATURE_ATT*)pDBC->Search_Index_EQU(pNpc[i].m_NPCType);
					sprintf(szTemp, "%s %s", pMissionNpc->pName, pMissionNpc->pNPCTypeName);
					vParam.push_back( szTemp );
					
					sprintf(szTemp, "%d", pMissionNpc->nNPCType);
					vParam.push_back( szTemp );
					vParam.push_back( STRING(pMissionNpc->pFilterName) );


					// Ìî³äNPCÈÎÎñ
					CObject_PlayerNPC* pNpcObj = (CObject_PlayerNPC*)( CObjectManager::GetMe()->FindCharacterByName( pMissionNpc->pName ) );
					if( pNpcObj )
					{
					}
				}
				else
				{
					vParam.push_back( "" );
					vParam.push_back( "0" );
					vParam.push_back( "" );
				}
				
				sprintf(szTemp, "%d,%d", (INT)pNpc[i].m_Pos.m_fX, (INT)pNpc[i].m_Pos.m_fZ);
				vParam.push_back( szTemp );

				CEventSystem::GetMe()->PushEvent( GE_UPDATE_MAP_LIST, vParam );
			}
		}
		break;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
