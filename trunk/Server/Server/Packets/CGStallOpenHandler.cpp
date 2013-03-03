/*
�ͻ�������򿪽���̯λ
*/

#include "stdafx.h"
#include "CGStallOpen.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCStallOpen.h"
#include "ItemContainer.h"
#include "GCDetailAttrib_Pet.h"
#include "GameTable.h"

uint CGStallOpenHandler::Execute( CGStallOpen* pPacket, Player* pPlayer )
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
	ObjID_t ObjId = pPacket->GetObjID();


	//��ʱ����
	GCStallOpen::_STALL_ITEM	StallItem[STALL_BOX_SIZE+STALL_PET_BOX_SIZE];

	if(ObjId == pHuman->GetID())
	{//�Լ����Լ����϶���
		//̯��container
		if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
		{
			GCStallError	Msg;
			Msg.SetID(STALL_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallOpenHandler::ObjID=%d, ERR_ILLEGAL: != ServerStallBox::STALL_OPEN"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE;
		}

		ItemContainer*	pStallContainer		= pHuman->m_StallBox.GetContainer(); 
		ItemContainer*	pStallPetContainer	= pHuman->m_StallBox.GetPetContainer(); 
		
		//ѭ��д��
		UINT	k = 0;	
		for(INT i = 0; i<pStallContainer->GetContainerSize(); i++)
		{
			if( pHuman->m_StallBox.GetSerialByIndex(i) != 0 )
			{//���������Ķ������Ĺ�����Ҫ֪ͨ�ͻ����������к�
				StallItem[k].nIndex		= i;
				StallItem[k].nPrice		= pHuman->m_StallBox.GetPriceByIndex(i);
				StallItem[k].nSerial	= pHuman->m_StallBox.GetSerialByIndex(i);
				if(pStallContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					pStallContainer->GetItem(i)->SaveValueTo(&(StallItem[k].item));
				}
				k++;
			}
		}

		for(INT i = 0; i<pStallPetContainer->GetContainerSize(); i++)
		{
			if( pHuman->m_StallBox.GetPetSerialByIndex(i) != 0 )
			{//�ж���
				StallItem[k].bIsPet		=	TRUE;
				StallItem[k].nIndex		=	i;
				StallItem[k].nPrice		=	pHuman->m_StallBox.GetPetPriceByIndex(i);
				StallItem[k].nSerial	=	pHuman->m_StallBox.GetPetSerialByIndex(i);
				if(pStallPetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					StallItem[k].PetGuid	=	pStallPetContainer->GetItem(i)->GetPetGUID();
				}
				k++;
			}
		}

		pHuman->StallNameChanged();

		//��Ϣ���
		GCStallOpen Msg;
		Msg.SetFirstPage( pHuman->m_StallBox.GetFirstPage());
		Msg.SetMerchadiseNum(k);
		Msg.SetMerchadiseList(StallItem);
		Msg.SetShopName(pHuman->m_StallBox.GetStallName(), (UINT)strlen(pHuman->m_StallBox.GetStallName()));
		Msg.SetObjID(ObjId);
		Msg.SetGUID(pHuman->GetGUID());

		//��������Q
		pGamePlayer->SendPacket(&Msg);

	}
	else
	{//����
		//�жϾ����Ƿ���Դ�
		Obj_Human* pTargetHuman = pScene->GetHumanManager()->GetHuman( ObjId );
		if( pTargetHuman == NULL )
		{
			GCStallError Msg;
			Msg.SetID(STALL_MSG::ERR_OWNER_INVALID);
			pHuman->GetPlayer()->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallOpenHandler::ObjName=%s, ERR_OWNER_INVALID: ObjId = %d"
				,pHuman->GetName(), ObjId) ;
			return PACKET_EXE_CONTINUE;
		}
		
		if(pTargetHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
		{
			GCStallError	Msg;
			Msg.SetID(STALL_MSG::ERR_CLOSE);
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallOpenHandler::ObjName=%s, ERR_CLOSE"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//̯��container
		ItemContainer*	pStallContainer = pTargetHuman->m_StallBox.GetContainer(); 

		//ѭ��д��
		UINT	k = 0;	
		for(INT i = 0; i<pStallContainer->GetContainerSize(); i++)
		{
			if( pStallContainer->GetItem(i)->IsEmpty() == FALSE )
			{//�ж���
				StallItem[k].nIndex		= i;
				StallItem[k].nPrice		= pTargetHuman->m_StallBox.GetPriceByIndex(i);
				StallItem[k].nSerial	= pTargetHuman->m_StallBox.GetSerialByIndex(i);
				pStallContainer->GetItem(i)->SaveValueTo(&(StallItem[k].item));
				k++;
			}
		}

		//��Ϣ���
		GCStallOpen Msg;
		Msg.SetFirstPage( pTargetHuman->m_StallBox.GetFirstPage());
		Msg.SetMerchadiseNum(k);
		Msg.SetMerchadiseList(StallItem);
		Msg.SetShopName(pTargetHuman->m_StallBox.GetStallName(), MAX_STALL_NAME);
		Msg.SetObjID(ObjId);
		Msg.SetGUID(pTargetHuman->GetGUID());

		//��������
		pGamePlayer->SendPacket(&Msg);

		//�����б�
		ItemContainer*	pStallPetContainer = pTargetHuman->m_StallBox.GetPetContainer(); 

		//ѭ��д��
		for(INT i = 0; i<pStallPetContainer->GetContainerSize(); i++)
		{
			if( pStallPetContainer->GetItem(i)->IsEmpty() == FALSE )
			{//�ж���
				INT	nPrice	= pTargetHuman->m_StallBox.GetPetPriceByIndex(i);
				INT	nSerial	= pTargetHuman->m_StallBox.GetPetSerialByIndex(i);

				Item* pIt = pStallPetContainer->GetItem(i);
				GCDetailAttrib_Pet PetMsgDetail;

				//��װGCDetailAttrib_Pet�ṹ
				Obj_Human::CalculatePetDetailAttrib(PetMsgDetail, pIt);
				PetMsgDetail.SetTradeIndex( i );
				
				PET_EXTRA_STRUCT::GCStallPetView_t	ExtraPetInfo;
				ExtraPetInfo.m_bFlag	= TYPE_STALL;
				ExtraPetInfo.m_nPrice	= nPrice;
				ExtraPetInfo.m_nSerial	= nSerial;

				PetMsgDetail.SetExtraInfoLength(ExtraPetInfo.GetSize());
				PetMsgDetail.SetExtraInfoData((BYTE*)&ExtraPetInfo);
				pGamePlayer->SendPacket( &PetMsgDetail );		
			}
		}
	}
	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallOpenHandler::ObjID=%d"
		,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
