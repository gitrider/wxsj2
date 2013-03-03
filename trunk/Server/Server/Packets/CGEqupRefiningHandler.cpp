/********************************************************************
created:	2010/6/25
filename: 	MapServer\Server\Packets\CGEquipRefiningHandler.cpp
file path:	MapServer\Server\Packets
file base:	CGSouXiaSewHandler
file ext:	cpp
author:		genghao

purpose:	��Ϣ�������

*********************************************************************/

#include "stdafx.h"
#include "GamePlayer.h"
#include "GCEquipRefining.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCEquipRefining.h"
#include "CGEquipRefining.h"
#include "GCNotifyEquip.h"

UINT CGEquipRefiningHandler::Execute( CGEquipRefining* pPacket, Player* pPlayer )
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

	//�г���ʱ������װ��Ϣ
	//����״̬���ɲ���
	//��̯״̬���ɲ���

	// ��ȡ������װ��
	BYTE iEquipItemPos = pPacket->GetEquipItemPos();
	Assert(iEquipItemPos >= 0 );
	Assert(iEquipItemPos <= MAX_BAG_SIZE);
	
	GCEquipRefining	msg;
	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iEquipItemPos);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRefiningHandler equipment BagIndex is invalid, BagIndex = %d", iEquipItemPos) ;
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}

	Item* pEquipment = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iEquipItemPos));
	Assert(NULL != pEquipment);
	//һЩ��֤���Ǽ����ȼ�...
	if( NULL == pEquipment || ICLASS_EQUIP != pEquipment->GetItemClass() || !(pEquipment->IsRuler(IRL_DISCARD) )
		|| pEquipment->IsAdsorbMagic()	|| pEquipment->GetRequireLevel() < EQUIP_REFINING_MIN_LEVEL	//�Ѿ���ħ��������ȼ�С��40
		|| 0 == pEquipment->GetEquipGemCount())	//û����Ƕ��ʯ
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRefiningHandler equipment type is invalid, BagIndex = %d", iEquipItemPos) ;
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);

		return PACKET_EXE_CONTINUE;
	}
	//��ȡ��������
	BYTE iRefiningItemPos = pPacket->GetRefiningItemPos();
	Assert(iRefiningItemPos >= 0 );
	Assert(iRefiningItemPos <= MAX_BAG_SIZE);

	pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRefiningItemPos);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRefiningHandler refining BagIndex is invalid, BagIndex = %d", iRefiningItemPos) ;
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}

	Item* pRefiningItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRefiningItemPos));
	Assert(NULL != pRefiningItem);
	if(NULL == pRefiningItem || pRefiningItem->GetItemClass() != ICLASS_COMITEM || pRefiningItem->GetItemType() != COMITEM_EQUIP_REFINING)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRefiningHandler refining type is invalid, BagIndex = %d", iRefiningItemPos) ;
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);

		return PACKET_EXE_CONTINUE;
	}
	//�߼����� 
	COMMITEM_INFO_TB* pGet = g_ItemTable.GetCommItemInfoTB(pRefiningItem->GetItemTableIndex());
	Assert(NULL != pGet);
	if ((pEquipment->GetRequireLevel()-1)/10+1 != pGet->m_nLevel)//��֤�ȼ�
	{
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);

		return PACKET_EXE_CONTINUE;
	}

	if ( 0 == pEquipment->GetAttrCount())// && pEquipment->GetLevel() 
	{
		msg.SetResult(EQUIPREFINING_NOT_HAVE_ATTR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}

	//EQUIP_TB* pGet = g_ItemTable.GetEquipTB(pEquipment->GetItemTableIndex());
	//Assert(NULL != pGet);
	//ɾ��װ���������������
	UINT iPrice = pEquipment->GetSellPrice();//������Ҫɾ����װ�����Ը�������
	BYTE iRequireLevel = pEquipment->GetRequireLevel();
	BYTE curEquipPoint = pEquipment->GetEquipPoint();
	BYTE attrCount = 0;

	_ITEM_ATTR tmpItemAttr[MAX_ITEM_ATTR];
	for (BYTE index=0; index<pEquipment->GetAttrCount(); ++index)
	{
		tmpItemAttr[index] = pEquipment->GetEquipAttr(index);
		if (tmpItemAttr[index].m_AttrType > 0)
		{
			attrCount++;
		}
	}
	Assert(attrCount == pEquipment->GetAttrCount());
	//��ʼɾ��װ���Ϳ۳���Ǯ
	BOOL bRet = FALSE;
	ITEM_LOG_PARAM		ItemLogParam;
	ItemLogParam.OpType	= ITEM_EQUIP_REFINING;

	bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iEquipItemPos));
	if(bRet)
	{
		SaveItemLog(&ItemLogParam);
	}
	else
	{
		msg.SetResult(EQUIPREFINING_ERASE_ITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}
	//ɾ����������
	bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRefiningItemPos));
	if(bRet)
	{
		SaveItemLog(&ItemLogParam);
	}
	else
	{
		msg.SetResult(EQUIPREFINING_ERASE_ITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}

	UINT iSoulBeadBagIndex = 0;
	Item*	pSoulBeadItem = NULL;
	//�����
	if (HEQUIP_MAINHAND == curEquipPoint)
	{
		bRet = HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,ITEM_WU_SOUL_BEAD, iSoulBeadBagIndex);
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		} 
		else 
		{
			msg.SetResult(EQUIPREFINING_ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&msg);
			return PACKET_EXE_CONTINUE;
		}
		
		pSoulBeadItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iSoulBeadBagIndex));
		Assert(NULL != pSoulBeadItem);
		Assert(ICLASS_SOUL_BEAD == pSoulBeadItem->GetItemClass());	
		
		pSoulBeadItem->SetSoulBeadPrice(iPrice);//�ɻ����۸����ó�
		pSoulBeadItem->SetSoulBeadNeedLevel(iRequireLevel);
		pSoulBeadItem->SetWuSoulBeadAttr(attrCount, tmpItemAttr);
		
	}//������
	else if (EQUIP_CAP == pEquipment->GetItemType()//EQUIP_PIFENG == pEquipment->GetEquipPoint() || 
			|| EQUIP_SHOULDER == pEquipment->GetItemType() || EQUIP_HAND == pEquipment->GetItemType()
			|| EQUIP_ARMOR == pEquipment->GetItemType() || EQUIP_BOOT == pEquipment->GetItemType())
	{
		bRet = HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,ITEM_WU_SOUL_BEAD, iSoulBeadBagIndex);
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		} 
		else 
		{
			msg.SetResult(EQUIPREFINING_ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&msg);
			return PACKET_EXE_CONTINUE;
		}
		pSoulBeadItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iSoulBeadBagIndex));
		Assert(NULL != pSoulBeadItem);
		Assert(ICLASS_SOUL_BEAD == pSoulBeadItem->GetItemClass());	
		
		pSoulBeadItem->SetSoulBeadPrice(iPrice);//�ɻ����۸����ó�
		pSoulBeadItem->SetSoulBeadNeedLevel(iRequireLevel);
		pSoulBeadItem->SetQiSoulBeadAttr(attrCount, tmpItemAttr);
	}
	// ֪ͨ���ӵ��ߵ�����
	GCNotifyEquip ItemMsg;
	Assert(pSoulBeadItem);
	ItemMsg.SetBagIndex(iSoulBeadBagIndex);
	pSoulBeadItem->SaveValueTo(ItemMsg.GetItem());
	pGamePlayer->SendPacket(&ItemMsg);
	
	msg.SetResult(EQUIPREFINING_SUCCESS);
	//pGamePlayer->SendPacket(&msg);
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}

