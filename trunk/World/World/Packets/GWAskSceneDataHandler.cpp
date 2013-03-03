

#include "stdafx.h"
#include "GWAskSceneData.h"
#include "Log.h"
#include "WGRetSceneData.h"
#include "ServerPlayer.h"
#include "CityManager.h"
#include "SceneInfo.h"

uint GWAskSceneDataHandler::Execute( GWAskSceneData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//world�յ��������Գ�����Ϣ������
	//���ݲ�ͬ���ͣ���Ӧ��ͬ����
	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	SceneID_t	SceneID		=	pPacket->GetSceneID();
	ID_t		ClientRes	=	pPacket->GetClientRes() ;
	BYTE		bType		=	pPacket->GetType();
	BYTE		ExtraDataL	=	pPacket->GetExtraDataLength();
	CHAR*		ExtraData	=	pPacket->GetExtraData();

	WGRetSceneData Msg ;

	switch(bType)
	{
		case GWAskSceneData::TYPE_COPY:
			{
				g_pSceneInfo->SetSceneClientRes( SceneID, ClientRes ) ;

				Msg.SetSceneID( SceneID ) ;
				Msg.SetClientRes( ClientRes ) ;
				pServerPlayer->SendPacket( &Msg ) ;
			}
			break;

		case GWAskSceneData::TYPE_CITY:
			{
				//�յ��������˳��г���load�ɹ�����Ϣ
				//world���Է����Լ���ʵ�ĳ��нṹ��
				GWAskSceneData::CITY_DATA CityData;
				memcpy(&CityData, ExtraData, ExtraDataL);
				SceneID_t	PortSceneID = CityData.m_PortSceneID;
				GuildID_t	nGuildID	= CityData.m_GuildID;

				_SERVER_DATA* pServerData = pServerPlayer->GetServerData();
				City* pCity = g_pCityManager->NewCity();
				pCity->SetSceneID(SceneID);						//�����Լ���ID
				pCity->SetPortSceneID(PortSceneID);				//���е���ڳ���ID
				pCity->SetGuildID(nGuildID);					//���еİ��ID
				pCity->SetServerID(pServerData->m_ServerID);	//�������ڷ�����
				pCity->LoadBuildingInfo(SceneID);				//���볡��������Ϣ

				SCENE_INIT_DATA CityInitData;
				
				CityInitData.m_nDataType	= SCENE_TYPE_CIT;
				CityInitData.m_CityData		= *pCity->GetCityDB();

				Msg.SetSceneID(SceneID);
				Msg.SetSceneInitData(&CityInitData);
				pServerPlayer->SendPacket( &Msg ) ;
			}
			break;

		case GWAskSceneData::TYPE_BATTLE:
			{

			}
			break;
		default:
			break;
	}

	Log::SaveLog( WORLD_LOGFILE, "GWAskSceneDataHandler SceneID=%d", 
		pPacket->GetSceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
