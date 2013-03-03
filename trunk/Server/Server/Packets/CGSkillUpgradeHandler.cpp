/********************************************************************
	created:	2008/01/15
	created:	15:1:2008   15:43
	filename: 	MapServer\Server\Packets\CGSkillUpgradeHandler.cpp
	file path:	MapServer\Server\Packets
	file base:	CGSkillUpgradeHandler
	file ext:	cpp
	author:		Richard
	
	purpose:	��Ϣ�������
	
*********************************************************************/

#include "stdafx.h"
#include "CGSkillUpgrade.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "HumanSkillUpgrad.h"

UINT CGSkillUpgradeHandler::Execute( CGSkillUpgrade* pPacket, Player* pPlayer )
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
	
	HumanSkillUpgrade oSkillUpgrade;
	oSkillUpgrade.HumanSkillLevelUp( pHuman, pPacket->GetSkillID(), pPacket->GetLevel() );

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

