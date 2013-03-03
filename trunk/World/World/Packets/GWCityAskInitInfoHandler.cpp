

#include "stdafx.h"
#include "GWCityAskInitInfo.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCityError.h"
#include "CityManager.h"
#include "GuildManager.h"
#include "Config.h"
#include "WGCityInitInfo.h"


uint GWCityAskInitInfoHandler::Execute( GWCityAskInitInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
		ID_t	ServerID = pServerPlayer->GetServerData()->m_ServerID;
		for(UINT i = 0; i<g_Config.m_SceneInfo.m_SceneCount; i++)
		{
			if(g_Config.m_SceneInfo.m_pScene[i].m_ServerID == ServerID)
			{//���������ĳ���
				for(INT j =0; j<MAX_CITY_PER_WORLD; j++ )
				{
					City* pCity = g_pCityManager->GetCityByIndex(j);
					if(pCity)
					{
						if(pCity->GetPortSceneID() == g_Config.m_SceneInfo.m_pScene[i].m_SceneID)
						{//��ǰ�����ǹ�����������ϵ�
							/*WGCityInitInfo*/

						}
						
					}
				}
					

			}


		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
