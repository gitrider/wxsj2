#include "StdAfx.h"
#include "GCNotifyChangeScene.h"
#include "CGAskChangeScene.h"
#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMNetManager.h"
#include "../SMProcedureManager.h"

using namespace Packets;
UINT GCNotifyChangeSceneHandler::Execute(GCNotifyChangeScene* pPacket, Player* pPlayer)
{
	SMITH::Agent*		pAgent	= (SMITH::Agent*)pPlayer;

	// 发送请求消息
	CGAskChangeScene	msg;
	msg.SetSourSceneID(pPacket->getCurrentSceneID());
	msg.SetDestSceneID(pPacket->getTargetSceneID());
	pAgent->getNetManager()->sendPacket(&msg);

	// 设置目标场景
	WORLD_POS*			pos		= pPacket->getTargetPos();
	pAgent->getVariable()->setAs_Int( "DestScene_ID", pPacket->getTargetSceneID() );
	pAgent->getVariable()->setAs_Vector2( "DestScene_EnterPos", pos->m_fX, pos->m_fZ );

	return PACKET_EXE_CONTINUE;
}
