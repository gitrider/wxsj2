#include "stdafx.h"
#include "CGPickDropItem.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Obj_Item.h"
#include "GCPickDropItemResult.h"
#include "GCMyBagList.h"
#include "ItemRuler.h"
#include "GCItemInfo.h"
#include "GameTable.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCNotifyEquip.h"
#include "GWChat.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
using namespace Packets;




uint CGPickDropItemHandler::Execute(CGPickDropItem* pPacket, Player* pPlayer )
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	//交易状态不可操作
	if(pHuman->m_ExchangBox.m_Status > 0)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ExchangBox::m_Status>0" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//摆摊状态不可操作
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	ObjID_t	 ItemObjID	=	pPacket->getObj_ItemId();

	if( ItemObjID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	Obj* pObj = pScene->GetObjManager()->GetObj( ItemObjID ) ;
	if( pObj==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPickDropItem: not find obj=%d", ItemObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pObjScene = pObj->getScene();
	Assert(pObjScene);

	if (pObjScene != pScene)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPickDropItem: not find obj=%d", ItemObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Item*	pObj_Item = (Obj_Item*)pObj;

	if( pObj_Item->GetObjType()!= Obj::OBJ_TYPE_DROP_ITEM)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPickDropItem: Obj_Item obj=%d\r\n  is Not class of OBJ_ITEM_CLASS", ItemObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}
	
	_ITEM* pItem = pObj_Item->GetObj_Item();
	Assert(!pItem->IsNullType());

	PickResultCode	ResultCode = PICK_INVALID_ITEM;
	GCPickDropItemResult	 Msg;
	GCItemInfo		ChangeItemMsg;

	if(pHuman->IsInValidRadius(pObj->getWorldPos(),MAX_DROP_ITEM_OP_DISTANCE)==FALSE)
	{
		Msg.setResult(PICK_TOO_FAR);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE ;
	}

	if(!pHuman->IsAlive())
	{
		Msg.setResult(PICK_DEAD);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE ;
	}
	//人物死亡掉落
	if(pObj_Item->GetType()==ITYPE_PERSONDROP_OBJ_ITEM)
	{	
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_PICK_FROM_PERSON;
		//查找合适的位置
		UINT uBagIndex = INVALID_ID;
		
		if(HumanItemLogic::RecieveItemToBag(&ItemLogParam, pHuman, pItem))
		{
			SaveItemLog(&ItemLogParam);

			ResultCode = PICK_SUCCESS;
			Msg.setBagIndex(uBagIndex);
			Msg.setDropItemId(ItemObjID);		
		}
		else
			ResultCode	=	PICK_BAG_FULL;

		Msg.setResult(ResultCode);
		//发送结果
		pGamePlayer->SendPacket( &Msg ) ;
		//发送物品变化
		if(ResultCode == PICK_SUCCESS) 
		{
			pObj_Item->Recycle();
		}
		
	}
	//怪物死亡掉落
	else if(pObj_Item->GetType()==ITYPE_MONSTERDROP_OBJ_ITEM)
	{
		/*
		*	掉落包拾取流程
		*/
		if(pObj_Item->CanPick(pHuman->GetGUID(),pHuman->GetID()))
		{
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType		= ITEM_PICK_FROM_MONSTER;
			ItemLogParam.CharGUID	= pHuman->GetGUID();
			ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
			ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
			ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
					//查找合适的位置
			UINT uBagIndex = INVALID_ID;
			if(HumanItemLogic::RecieveItemToBag(&ItemLogParam, pHuman, pItem))
			{
				SaveItemLog(&ItemLogParam);
				
				ResultCode = PICK_SUCCESS;
				Msg.setBagIndex(uBagIndex);
				Msg.setDropItemId(ItemObjID);		
			}
			else
				ResultCode	=	PICK_BAG_FULL;

		}
		else
		{
			ResultCode = PICK_INVALID_OWNER;
		}

		Msg.setResult(ResultCode);
		//发送结果
		pGamePlayer->SendPacket( &Msg ) ;
		//发送物品变化
		if(ResultCode == PICK_SUCCESS) 
		{
			pObj_Item->Recycle();

			if (pHuman->GetTeamInfo()->GetTeamID() != INVALID_ID)
			{
				const CHAR* PickDropObjName = pPacket->getObj_ItemName();
				const CHAR* PickHumanName = pHuman->GetName();
				const CHAR* pick = "拾取了";
				CHAR ChatInfo[MAX_CHAT_SIZE] = "玩家";
				strcat(ChatInfo, PickHumanName);
				strcat(ChatInfo, pick);
				strcat(ChatInfo, PickDropObjName);
					
				GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;
				pChatPacket->SetSourGUID( pHuman->GetGUID() ) ;
				pChatPacket->SetChatType( CHAT_TYPE_SYSTEM ) ;

				pChatPacket->SetContexSize(strlen(ChatInfo)+1);
				pChatPacket->SetContex(ChatInfo);

				pChatPacket->SetTeamID( pHuman->GetTeamInfo()->GetTeamID() ) ;
				g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
			}
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPickObjItem: ok ObjId=", ItemObjID ) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}