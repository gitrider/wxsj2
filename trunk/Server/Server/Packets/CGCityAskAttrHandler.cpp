

#include "stdafx.h"
#include "CGCityAskAttr.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "GCCityAttr.h"

uint CGCityAskAttrHandler::Execute( CGCityAskAttr* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	//��Ϣ�ĺϷ���
	if(pScene->m_SceneInitData.m_nDataType != SCENE_TYPE_CIT)
	{
		return PACKET_EXE_CONTINUE;
	}
	//��ǰ�����еĽ�����ˢ������������������Ϣ
	for(INT i = 0; i<MAX_CITY_BUILDING_NUM; i++)
	{
		if(pScene->m_SceneInitData.m_CityData.m_Builds[i].m_BuildingID != INVALID_ID)
		{
			GCCityAttr MsgToClient;
			MsgToClient.SetUpdateAttrib(CITY_UPDATE_BUILDING, TRUE);
			MsgToClient.SetBuildingIndex(i);
			MsgToClient.SetBuildingData(pScene->m_SceneInitData.m_CityData.m_Builds[i]);
			pGamePlayer->SendPacket(&MsgToClient);
		}
	}
	
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
