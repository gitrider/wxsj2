
#include "stdafx.h"
#include "CGMapInfo.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

//ָ�������л��Ľű�ID
#define CHANGE_SCENE_SCRIPT_ID	800021		//��һ�汾Ϊ211022
#define EVENT_CHANGE_SCENE_FUNC_NAME "OnChangeSceneEvent"

uint	CGMapInfoHandler::Execute(CGMapInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	pScene->GetLuaInterface()->ExeScript_DDD(
				CHANGE_SCENE_SCRIPT_ID, 
				EVENT_CHANGE_SCENE_FUNC_NAME, 
				(INT)pScene->SceneID(), 
				(INT)pHuman->GetID(), 
				(INT)pPacket->GetIndex()
				) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGMapInfoHandler: GUID=%X", pHuman->GetGUID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}