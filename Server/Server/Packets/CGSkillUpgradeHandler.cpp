/********************************************************************
	created:	2008/01/15
	created:	15:1:2008   15:43
	filename: 	MapServer\Server\Packets\CGSkillUpgradeHandler.cpp
	file path:	MapServer\Server\Packets
	file base:	CGSkillUpgradeHandler
	file ext:	cpp
	author:		Richard
	
	purpose:	消息句柄处理
	
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
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
	
	HumanSkillUpgrade oSkillUpgrade;
	oSkillUpgrade.HumanSkillLevelUp( pHuman, pPacket->GetSkillID(), pPacket->GetLevel() );

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

