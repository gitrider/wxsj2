

#include "stdafx.h"
#include "CGOtherEquip.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCOtherEquip.h"
#include "Log.h"



uint CGOtherEquipHandler::Execute( CGOtherEquip* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t ObjID = pPacket->getObjID() ;
    if( ObjID==INVALID_ID )
	{
		return PACKET_EXE_ERROR ;
	}

	//��ʼ���Ҫ��װ����Ϣ��Obj�ǲ���һ����ҵ�Obj_Human������
	Obj* pDestObj = pScene->GetObjManager()->GetObj( ObjID ) ;
	if( pDestObj==NULL )
	{//���ObjID�Ѿ��뿪������
//		Assert( FALSE ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if( !IsCharacterObj( pDestObj->GetObjType() ) )
	{
		return PACKET_EXE_ERROR ;
	}

	Obj_Character* pDestCharacter = (Obj_Character*)pDestObj ;
	if( pDestCharacter->GetObjType()!=Obj::OBJ_TYPE_HUMAN )
	{
		return PACKET_EXE_ERROR ;
	}
	//������

	Obj_Human* pDestHuman = (Obj_Human*)pDestCharacter ;

	GCOtherEquip Msg ;
	Msg.setObjID( ObjID ) ;
	Msg.setName( pDestHuman->GetName() ) ;
	Msg.setDirectory( pDestHuman->getDir() ) ;
//��������

	pGamePlayer->SendPacket( &Msg ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGOtherEquipHandler::ObjID=%d", ObjID ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
