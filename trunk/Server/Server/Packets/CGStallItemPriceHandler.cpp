/*
�ͻ��˸ı���Ʒ�۸�
*/

#include "stdafx.h"
#include "CGStallItemPrice.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "GCStallItemPrice.h"

uint CGStallItemPriceHandler::Execute( CGStallItemPrice* pPacket, Player* pPlayer )
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

	//����
	_ITEM_GUID	ItemGuid	=	pPacket->GetObjGUID();
	PET_GUID_t	PetGuid		=	pPacket->GetPetGUID();
	UINT		ItemPrice	=	pPacket->GetPrice();
	UINT		ItemSerial	=	pPacket->GetSerial();

	if (ItemPrice == 0)
	{
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_PRICE_ZERO);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallItemPriceHandler::ObjName=%s, STALL_MSG::ERR_PRICE_ZERO"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}
	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
	{
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_ILLEGAL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallItemPriceHandler::ObjName=%s, ERR_ILLEGAL:!= ServerStallBox::STALL_OPEN"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}
	
	if (ItemPrice == 0)
	{
		return PACKET_EXE_CONTINUE;
	}
	ItemContainer*		pStallContainer		= pHuman->m_StallBox.GetContainer();
	ItemContainer*		pStallPetContainer	= pHuman->m_StallBox.GetPetContainer();
	GCStallError		MsgError;

	if(PetGuid.IsNull())
	{//����Ʒ�ļ۸����
		INT	IndexInStall	=	pStallContainer->GetIndexByGUID(&ItemGuid);
		if(IndexInStall<0)
		{
			MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallItemPriceHandler::ObjID=%d, ERR_NEED_NEW_COPY: IndexInStall = %d"
				,pHuman->GetID(), IndexInStall) ;
			return PACKET_EXE_CONTINUE ;
		}

		if(pHuman->m_StallBox.GetSerialByIndex(IndexInStall) > ItemSerial)
		{
			MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallItemPriceHandler::ObjID=%d, ERR_NEED_NEW_COPY: ItemSerial = %d, BoxSerial = %d"
				,pHuman->GetID(), ItemSerial, pHuman->m_StallBox.GetSerialByIndex(IndexInStall)) ;
			return PACKET_EXE_CONTINUE ;
		}

		pHuman->m_StallBox.SetPriceByIndex(IndexInStall, ItemPrice);
		pHuman->m_StallBox.IncSerialByIndex(IndexInStall);

		//��Client����һ����Ϣ������Client�汾��
		GCStallItemPrice msg;
		msg.SetObjGUID(pPacket->GetObjGUID());
		msg.SetPrice(pPacket->GetPrice());
		msg.SetSerial(pHuman->m_StallBox.GetSerialByIndex(IndexInStall));

		pGamePlayer->SendPacket(&msg);

		g_pLog->FastSaveLog( LOG_FILE_1, "CGStallItemPriceHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d, ItemPrice = %d"
			,pHuman->GetName(), IndexInStall, ItemPrice) ;
	}
	else
	{//�ı����۸�
		INT	PetIndexInStall	= pStallPetContainer->GetIndexByGUID(&PetGuid);

		if(PetIndexInStall<0)
		{
			MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallItemPriceHandler::ObjName=%s, ERR_NEED_NEW_COPY: PetIndexInStall = %d"
				,pHuman->GetName(), PetIndexInStall) ;
			return PACKET_EXE_CONTINUE ;
		}

		if(pHuman->m_StallBox.GetPetSerialByIndex(PetIndexInStall) != ItemSerial)
		{
			MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallItemPriceHandler::ObjName=%s, ERR_NEED_NEW_COPY: ItemSerial = %d, BoxSerial = %d"
				,pHuman->GetName(), ItemSerial, pHuman->m_StallBox.GetPetSerialByIndex(PetIndexInStall)) ;
			return PACKET_EXE_CONTINUE ;
		}

		pHuman->m_StallBox.SetPetPriceByIndex(PetIndexInStall, ItemPrice);
		pHuman->m_StallBox.IncPetSerialByIndex(PetIndexInStall);

		//��Client����һ����Ϣ������Client�汾��
		GCStallItemPrice msg;
		msg.SetPetGUID(PetGuid);
		msg.SetPrice(ItemPrice);
		msg.SetSerial(pHuman->m_StallBox.GetPetSerialByIndex(PetIndexInStall));

		pGamePlayer->SendPacket(&msg);

		g_pLog->FastSaveLog( LOG_FILE_1, "CGStallItemPriceHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d, ItemPrice = %d"
			,pHuman->GetName(), PetIndexInStall, ItemPrice) ;
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
