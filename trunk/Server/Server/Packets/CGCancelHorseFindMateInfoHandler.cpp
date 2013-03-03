/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "CGCancelHorseFindMateInfo.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "HorseFindMateModule.h"

namespace Packets
{

uint	CGCancelHorseFindMateInfoHandler::Execute(CGCancelHorseFindMateInfo* pPacket, Player* pPlayer )
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
	
	INT iIndex = pHuman->GetHorseIndexByGUID( pPacket->GetGUID() );
	if( iIndex == INVALID_ID )
	{
		return PACKET_EXE_ERROR;
	}
	_PET_DB* pPetDB = pHuman->GetHorseDB( iIndex );

	HorseFindMateModule::GetInstance().CancelHorseFindMateInfo(pHuman, pPetDB);

	g_pLog->FastSaveLog(LOG_FILE_1, "CGQueryHorseFindMateInfoHandler: ok obj=%d,scene=%d",
		pHuman->GetID(), pScene->SceneID());

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}


}
