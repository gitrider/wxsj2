

#include "stdafx.h"
#include "CGMove.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Log.h"
#include "GCMove.h"
//#include "GCNewPlayer_Move.h"


uint CGMoveHandler::Execute( CGMove* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//Assert(FALSE) ;
	//return PACKET_EXE_ERROR ;//�����˵���Ϣ��








	//GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	//Assert( pGamePlayer ) ;

	//Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	//Assert( pHuman ) ;

	//Scene* pScene = pHuman->getScene() ;
	//Assert( pScene ) ;

	////����߳�ִ����Դ�Ƿ���ȷ
	//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	//ObjID_t ObjID = pHuman->GetID() ;

	//WORLD_POS CurPos = pPacket->getCurrentPos() ;
	//WORLD_POS TarPos = pPacket->getCurTargetPos() ;
	//pScene->GetMap()->VerifyPos( &CurPos ) ;
	//pScene->GetMap()->VerifyPos( &TarPos ) ;

	//WORLD_POS OwnFinalPos = *pHuman->GetFinalTarPos() ;
	//BYTE bOwnMoveIndex = pHuman->getMoveIndex() ;

	////�����ҵ��ƶ������Ƿ�Ƿ�
	//if( !pScene->CheckMove( pHuman, &CurPos, &TarPos ) )
	//{//�Ƿ�
	//}

	//pHuman->MoveTo( &CurPos, &TarPos ) ;
	//pHuman->setFinalTarPos( &pPacket->getFinalTargetPos() ) ;
	//pHuman->setMoveIndex( pPacket->getRequestIndex() ) ;

	//GCMove Msg ;
	//Msg.setRequestIndex( pPacket->getRequestIndex() ) ;
	//pGamePlayer->SendPacket( &Msg ) ;


	//if( !MyPosEqual( &OwnFinalPos, &(pPacket->getFinalTargetPos()) ) )
	//{
	//	GCNewPlayer_Move Msg ;
	//	Msg.setObjID( pHuman->GetID() ) ;
	//	Msg.setWorldPos( *pHuman->getWorldPos() ) ;
	//	Msg.setTargetPos( *pHuman->GetCurrentTargetPos() ) ;
	//	Msg.setEquipVer( pHuman->GetEquipVer() ) ;

	//	pScene->BroadCast( &Msg, pHuman ) ;
	//}

	//Log::SaveLog( ".\\Log\\debug.log", "CGMoveHandler::ObjID=%d X=%f Y=%f", 
	//	ObjID, TarPos.m_fX, TarPos.m_fZ ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
