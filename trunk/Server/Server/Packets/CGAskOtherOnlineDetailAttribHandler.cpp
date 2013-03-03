
#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "ServerManager.h"
#include "CGAskOtherOnlineDetailAttrib.h"
#include "GWAskOtherOnlineAttrib_Human.h"
#include "PacketFactoryManager.h"
#include "GCDetailAttrib.h"
#include "GCDetailEquipList.h"
#include "HumanItemLogic.h"

using namespace Packets;

uint CGAskOtherOnlineDetailAttribHandler::Execute( CGAskOtherOnlineDetailAttrib* pPacket, Player* pPlayer )
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

	ObjID_t ObjID = pHuman->GetID();
	if( ObjID == INVALID_ID )
	{
		Assert( FALSE ) ;
		return PACKET_EXE_ERROR ;
	}

	Obj_Human *pTarget = pScene->GetHumanManager()->GetHuman(pPacket->getTargetID());
	if( pTarget==NULL )
	{
		// �鿴����Ҳ��ڵ�ǰServer������GLServer��������
		if(pPacket->GetGUID() != INVALID_GUID){
			GWAskOtherOnlineAttrib_Human* pMsg = (GWAskOtherOnlineAttrib_Human*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKOTHERONLINEATTRIB_HUMAN));

			pMsg->SetSourceGUID(pHuman->GetGUID());
			pMsg->SetDestGUID(pPacket->GetGUID());

			pMsg->SetPlayerID(pHuman->GetPlayerID());
			g_pServerManager->SendPacket(pMsg, INVALID_ID);

			g_pLog->FastSaveLog( LOG_FILE_1, "CGAskOtherOnlineDetailAttribHandler: ok obj=%d scene=%d", 
				ObjID, pScene->SceneID() ) ;

			return PACKET_EXE_CONTINUE ;
		}
		else{
			Assert(FALSE);
			return PACKET_EXE_ERROR;
		}
	}

	if(pTarget->GetObjType() == Obj::OBJ_TYPE_HUMAN)
	{
		// ������һ�����Ϣ
		GCDetailAttrib	Msg;

		Msg.SetObjID( pTarget->GetID() );
		Msg.SetLevel(pTarget->GetLevel());
		Msg.SetMenpai(pTarget->GetMenPai() ) ;
		Msg.SetModelID(pTarget->GetModelID());
		Msg.SetMountID( pTarget->GetMountID() );
		Msg.SetMountIndex( pTarget->GetMountIndex() ); //  [7/23/2010 �¾���]

		// ������������Ϣ
		INT i = 0;
		for (i=0; i < HUMAN_PET_MAX_COUNT; ++i )
		{
			Item* pPetFind = pTarget->GetPetContain()->GetItem(i);
			PET_GUID_t guidFind = pPetFind->GetPetGUID();
			if ( !guidFind.IsNull() )
			{
				pTarget->RefeshHorseDetailAttrib(guidFind);
			}
		}

		// �������װ����Ϣ
		UINT RetPart = 0;
		GCDetailEquipList EquipMsg;
		for(i=0; i<HEQUIP_NUMBER; i++)
		{
			Item* pEquipItem = HumanItemLogic::GetEquip((Obj_Human*)(pTarget), (HUMAN_EQUIP)i);

			if(!pEquipItem)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGAskOtherOnlineDetailAttribHandler: faile 2") ;
				return PACKET_EXE_CONTINUE;
			}
			if(pEquipItem->IsEmpty())
				continue;

			pEquipItem->SaveValueTo(EquipMsg.GetEquipData((HUMAN_EQUIP)i));
			RetPart|=(1<<((UINT)i));
		}	
		EquipMsg.SetAskPart(RetPart);
		pGamePlayer->SendPacket(&EquipMsg);

	}
	else if(pTarget->GetObjType() == Obj::OBJ_TYPE_PET)
	{
		// ���ﲻͨ���˷�������ϸ����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskOtherOnlineDetailAttribHandler: Not Implement Obj_Pet Target obj=%d", ObjID ) ;
		return	PACKET_EXE_CONTINUE;
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskOtherOnlineDetailAttribHandler: Warring No Rights to ask other obj's detailattribute obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskOtherOnlineDetailAttribHandler: ok obj=%d scene=%d", 
		ObjID, pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
