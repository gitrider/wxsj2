#include "stdafx.h"
#include "CGUseGem.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "GCUseGemResult.h"
#include "GCCharEquipment.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
using namespace Packets;

uint CGUseGemHandler::Execute(CGUseGem* pPacket,Player* pPlayer)
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
	//����״̬���ɲ���
	if(pHuman->m_ExchangBox.m_Status > 0)
	{//����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGemHandler: ObjID=%d, ExchangBox::m_Status>0" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//��̯״̬���ɲ���
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGemHandler: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	USEGEM_RESULT	Result = USEGEM_SUCCESS;
	GCUseGemResult pResultPacket;

	UINT nValidCount = pPacket->GetValidInfoCount();
	if (nValidCount>MAX_ITEM_GEM)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGemHandler: pPacket->GetValidInfoCount()=%d, error" ,nValidCount) ;

		return PACKET_EXE_ERROR;
	}
	Item*		pEquipItem		= NULL;
	INT			EquipPoint		= INVALID_ITEM_POS;
	if(pPacket->GetEquipPosType()>=INVALID_ITEM_POS_TYPE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem Equip posType error, posType=%d", pPacket->GetEquipPosType()) ;
		return PACKET_EXE_ERROR;
	}

	//�ڱ�������
	if(pPacket->GetEquipPosType() == ENUM_BAG)
	{
		if(pPacket->GetPos()>MAX_BAG_SIZE)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem Item pos error") ;
			return PACKET_EXE_ERROR;
		}
		ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, pPacket->GetPos());
		if (pBagContainer == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem BagIndex is invalid, BagIndex = %d", pPacket->GetPos()) ;

			return PACKET_EXE_ERROR;
		}

		pEquipItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(pPacket->GetPos()));
		Assert(pEquipItem != NULL);
		//��������ôû��ѽ
		if(pEquipItem->IsEmpty())
		{
			Result = USEGEM_EQUIP_FAIL;
			pGamePlayer->SendPacket( &pResultPacket ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem Equip empyty, bagpos=%d", pPacket->GetPos()) ;
			return PACKET_EXE_CONTINUE;
		}
	}

	//������
	if(pPacket->GetEquipPosType() == ENUM_EQUIP_POINT)
	{
		if(pPacket->GetPos()>MAX_BASE_EQUIP_COUNT)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem Item pos error") ;
			return PACKET_EXE_ERROR;
		}
		EquipPoint = pPacket->GetPos();
		ItemContainer* pEquipContainer = pHuman->GetEquipContain();
		if(NULL==pEquipContainer)
		{
			AssertEx(FALSE,"[CGUseGem]: NULL EquipContainer pointer found!");
		}
		pEquipItem = pEquipContainer->GetItem((UINT)EquipPoint);
		Assert(pEquipItem != NULL);

		//������ô���Ҳ���ѽ
		if(pEquipItem->IsEmpty())
		{
			Result = USEGEM_EQUIP_FAIL;
			pGamePlayer->SendPacket( &pResultPacket ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem Equip empyty, bagpos=%d", pPacket->GetPos()) ;
			return PACKET_EXE_CONTINUE;
		}
	}

	//��Ҫ��Ƕ�ĸ�������װ��
	if(pEquipItem->GetItemClass()!=ICLASS_EQUIP)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem GemItem is not ICLASS_EQUIP") ;
		return PACKET_EXE_ERROR;
	}

	//�㷢��װ�����ұ���Ĳ�һ��ѽ
	if(!(pEquipItem->GetGUID() == pPacket->getItemID()))
	{
		Result = USEGEM_EQUIP_FAIL;
		pGamePlayer->SendPacket( &pResultPacket ) ;
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem EQUIP GUID ERROR") ;
		return PACKET_EXE_CONTINUE;
	}


	for(UINT i=0; i<nValidCount; ++i)
	{
		HOLE_GEM_INFO* pInfo = pPacket->GetHoleGemInfo(i);
		if (!pInfo)
		{
			return PACKET_EXE_ERROR;
		}

		for(UINT j=i+1; j<nValidCount; ++j)
		{
			HOLE_GEM_INFO* pFindInfo = pPacket->GetHoleGemInfo(j);
			if (!pInfo)
			{
				return PACKET_EXE_ERROR;
			}
			
			if (pFindInfo->m_GemBagIndex == pInfo->m_GemBagIndex)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem Gembagindex is same") ;
				return PACKET_EXE_ERROR;
			}
		}
		if((pInfo->m_nGemHolePos<pEquipItem->GetCurGemHole())&&(pInfo->m_GemBagIndex<MAX_BAG_SIZE))
		{
			Item*		 pGemItem		=	HumanItemLogic::GetBagItem(pHuman, pInfo->m_GemBagIndex);
			if(pGemItem == NULL)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem Gembagindex error") ;
				return PACKET_EXE_ERROR;
			}
			
			//��ʯ���ƶ�
			if(pGemItem->IsEmpty())
			{
				Result = USEGEM_GEM_FAIL;
				pGamePlayer->SendPacket( &pResultPacket ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem GemItem is empty, gembagindex=%d", pInfo->m_GemBagIndex) ;

				return PACKET_EXE_CONTINUE;
			}
			
			//�Ǳ�ʯ�ǲ��ɷ��õ�
			if(pGemItem->GetItemClass()!=ICLASS_GEM)
			{
				Result = USEGEM_GEM_FAIL;
				pGamePlayer->SendPacket( &pResultPacket ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem GemItem is not ICLASS_GEM") ;

				return PACKET_EXE_CONTINUE;
			}


			if(pEquipItem->GetGemSlotMax()>0)
			{
				//��Ƕ�ڲ����ڵĿ�λ��
				if(pInfo->m_nGemHolePos>pEquipItem->GetCurGemHole())
				{	
					g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem GemHole is not enough") ;
					return PACKET_EXE_ERROR;
				}
			}
			
			//������Ƕװ������
			if (!EquipCanAddHole((EQUIP_TYPE)pEquipItem->GetItemType()))
			{
				Result = USEGEM_EQUIP_FAIL;
				pGamePlayer->SendPacket( &pResultPacket ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem EquipType error") ;
				return PACKET_EXE_CONTINUE;
			}
			
			//������Ƕ����
			if( !pGemItem->GetReqEquip(pEquipItem->GetItemType()))
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGem GemItem Equip type error, require equippoint=%d", pEquipItem->GetEquipPoint()) ;
				Result = USEGEM_CANNT_USE;
				pResultPacket.SetResult(Result);
				pHuman->GetPlayer()->SendPacket( &pResultPacket );
				return PACKET_EXE_CONTINUE;
			}
			//������Ч��Ϣ
		}
		else
		{
			return PACKET_EXE_ERROR;
		}
	}
	//
	

	//����������֤��ȷ
	for(UINT i=0; i<nValidCount; ++i)
	{
		HOLE_GEM_INFO* pInfo = pPacket->GetHoleGemInfo(i);
		pHuman->UseGem(pEquipItem, pInfo->m_GemBagIndex, pInfo->m_nGemHolePos);
	}


	//���װ��������
	if((pPacket->GetEquipPosType() == ENUM_EQUIP_POINT)&&(EquipPoint != INVALID_ITEM_POS))
	{
		//����װ���汾
		pHuman->SetEquipVer(pHuman->GetEquipVer()+1);

		//ˢ��װ�������������Ӱ��
		pHuman->ItemEffectFlush();

		//�������Ч��
		if(pHuman->IsVisualPart((HUMAN_EQUIP)EquipPoint))
		{	
			GCCharEquipment OtherMsg;
			OtherMsg.setObjID(pHuman->GetID());
			OtherMsg.setID((HUMAN_EQUIP)EquipPoint, pEquipItem->GetItemTableIndex());

			pScene->BroadCast(&OtherMsg,pHuman,TRUE);

		}

		//����װ��
		UINT RetPart = 0; // modify by gh for new equip point 2010/05/10
		GCDetailEquipList	SelfMsg;
		SelfMsg.setObjID( pHuman->GetID() );
		pEquipItem = HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)EquipPoint);
		if(!pEquipItem)
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		if(pEquipItem->IsEmpty()) 
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGemHandler: Equip_Point =%d Fail", EquipPoint ) ;
			return PACKET_EXE_CONTINUE;
		}

		pEquipItem->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)EquipPoint));
		RetPart|=(1<<((UINT)EquipPoint));
		SelfMsg.SetAskPart(RetPart);
		pGamePlayer->SendPacket( &SelfMsg ) ;
	}
	else	//����֪ͨ��������Ʒ�ı�
	{
		GCItemInfo  EquipMsg;
		EquipMsg.setIsNull(FALSE);
		EquipMsg.setID(pPacket->GetPos());
		pEquipItem->SaveValueTo(EquipMsg.getItem());
		pHuman->GetPlayer()->SendPacket(&EquipMsg);
	}

	Result = USEGEM_SUCCESS;
	pResultPacket.SetResult(Result);
	pGamePlayer->SendPacket(&pResultPacket);
	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}