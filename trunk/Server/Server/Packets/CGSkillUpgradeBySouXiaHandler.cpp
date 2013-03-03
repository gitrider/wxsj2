/********************************************************************
	created:	2010/5/10
	filename: 	MapServer\Server\Packets\CGSkillUpgradeBySouXiaHandler.cpp
	file path:	MapServer\Server\Packets
	file base:	CGSkillUpgradeBySouXiaHandler
	file ext:	cpp
	author:		genghao
	
	purpose:	��Ϣ�������
	
*********************************************************************/

#include "stdafx.h"
#include "CGSkillUpgradeBySouXia.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "HumanSkillUpgrad.h"
#include "GCSkillUpgradeBySouXia.h"
#include "SouXia.h"
#include "GCDiscardItemResult.h"
#include "GCDetailEquipList.h"

UINT CGSkillUpgradeBySouXiaHandler::Execute( CGSkillUpgradeBySouXia* pPacket, Player* pPlayer )
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

	// ��ȡѧϰ�ļ�������
	BYTE iStudySkillType = pPacket->GetStudySkillType();
	Assert(iStudySkillType >= ITEM_SOUXIASKILL );
	Assert(iStudySkillType <= ITEM_ZUOJI_ZHAOHUAN);

	GCAddSouXiaSkill  msg;
	// ��ȡ����������Ϣ,��ȡ����֤�ͻ��˼��������
	BYTE iBagIndex = pPacket->GetBagIndex();
	Assert(iBagIndex >= 0 );

	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iBagIndex);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillUpgradeBySouXia BagIndex is invalid, BagIndex = %d", iBagIndex) ;
		msg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_ERROR;
	}

	Item* pUseItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iBagIndex));
	if(pUseItem->GetItemClass() != ICLASS_COMITEM || COMITEM_SOUXIA != pUseItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillUpgradeBySouXia type is invalid, BagIndex = %d", iBagIndex) ;
		msg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_ERROR;
	}

	SOUXIA_INFO_TB* pGet = g_ItemTable.GetSouXiaTB(pUseItem->GetItemTableIndex());
	if (NULL != pGet && ITEM_MEMORYCLUB == pGet->m_nUseType)
	{
		if(pGet->m_nNeedLevel > pHuman->GetLevel()) // ʹ��USBû�дﵽ���Ƶȼ�
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillUpgradeBySouXia level is error, BagIndex = %d", iBagIndex) ;

			// �������Ӽ��ܳɹ�/ʧ�ܵ���Ϣ,������¼
			msg.SetResult(HUMAN_LEVEL_NOT_ENOUGH);//���ý��

			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &msg );
			}
			return PACKET_EXE_CONTINUE;
		}

		Assert(pGet->m_nMemoryType == iStudySkillType);

		if(pGet->m_nMemoryType != iStudySkillType) // �������ѧ�ļ������Ͳ��ǵ�ǰ���߰����ļ���������У��ʧ��
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillUpgradeBySouXia level is error, BagIndex = %d", iBagIndex) ;
			// �������Ӽ��ܳɹ�/ʧ�ܵ���Ϣ,������¼
			msg.SetResult(SOUXIA_SKILL_STUDY_TYPE_ERROR);//���ý��
			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &msg );
			}

			return PACKET_EXE_CONTINUE;
		}
	}
	ItemParamValue ipv = (ItemParamValue)IPV_INT;
	SkillID_t	iParamSkillID = pUseItem->GetItemParam(8,ipv); // ������������������
	if(iParamSkillID <= 0 )
	{
		// �������Ӽ��ܳɹ�/ʧ�ܵ���Ϣ,������¼		
		msg.SetResult(SOUXIA_SKILL_STUDY_TYPE_ERROR);//���ý��
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &msg );
		}
		return PACKET_EXE_CONTINUE;
	}

	// ��֤����¼��Ϣ
	Item* pEquipSouXiaItem = pHuman->GetEquipContain()->GetItem(HEQUIP_SOUXIA);
	Assert(pEquipSouXiaItem != NULL);
	if (NULL == pEquipSouXiaItem)
	{
		msg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}
	if(pEquipSouXiaItem->GetItemClass() != ICLASS_EQUIP || EQUIP_SOUXIA != pEquipSouXiaItem->GetItemType())
	{
		msg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}
	
	SOUXIA_INFO_TB* pGetEquip = g_ItemTable.GetSouXiaTB(pEquipSouXiaItem->GetItemTableIndex());
	if (NULL != pGetEquip && ITEM_SOUXIALU == pGetEquip->m_nUseType)//��������Ϊ0�ģ�������������
	{
		// ������д0��������¼Ϊ��������¼����д1��ʾΪ��������¼����д2��ʾΪ�м�����¼����д3��ʾΪ�߼�����¼����д4��ʾ������������¼����д-1��ʾ������Ч
		if(0 == pGetEquip->m_nSouXiaLevel && iStudySkillType > ITEM_SOUXIASKILL)
		{
			return PACKET_EXE_CONTINUE;
		} 
		else if (1 == pGetEquip->m_nSouXiaLevel && iStudySkillType > ITEM_PRODUCT)
		{
			return PACKET_EXE_CONTINUE;
		} 
		else if (2 == pGetEquip->m_nSouXiaLevel && iStudySkillType > ITEM_PET_ZHAOHUAN)
		{
			return PACKET_EXE_CONTINUE;
		} 
	}
	// ��ȡ�Ѿ�װ���Ē���¼�Ĳ�������,�����Ƿ�����
	
	INT	iParamSouXiaID = pEquipSouXiaItem->GetItemParam(0,ipv);
	INT	iSouXiaPos = pEquipSouXiaItem->GetItemParam(4,ipv);

	SouXia curSouXia;
	// ѧϰ����
	if (ITEM_PRODUCT != pGet->m_nMemoryType)
	{
		HumanSkillUpgrade oSkillUpgrade;
		if(!oSkillUpgrade.HumanSkillLevelUp( pHuman, iParamSkillID, 1 ))
		{
			msg.SetResult(SOUXIA_SKILL_UPGRADE_FAIL);//ѧϰʧ��
			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &msg );
			}
			return PACKET_EXE_CONTINUE;
		}
	}
	else 
	{
		//pHuman->__SetPrescrLearnedFlag(iParamSkillID, TRUE);
	}

	if(iSouXiaPos <= 0 && iParamSouXiaID == 0)
	{
		UINT iSouXiaIndexParam	= pEquipSouXiaItem->GetItemTableIndex();
		iSouXiaPos = pHuman->AddSouXia(iSouXiaIndexParam); // ���������������ϵ�����
		Assert(iSouXiaPos >= 0);
		if(iSouXiaPos < 0)
		{
			msg.SetResult(HUMAN_SOUXIA_ADD_FAIL);
			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &msg );
			}
			return PACKET_EXE_CONTINUE;
		}

		curSouXia = pHuman->GetSouXia(iSouXiaPos);//��ǰװ���Ē���¼
		_SOUXIA_DB_LOAD		SouXia_db_Load;
		SouXia_db_Load.m_SouXiaData[iSouXiaPos] = curSouXia.GetSouXiaData();
		pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iSouXiaPos);	

		BOOL bRet = g_ItemOperator.SetItemParam( pHuman->GetEquipContain(),
			pHuman->GetEquipContain()->BagIndex2ConIndex(HEQUIP_SOUXIA),0, ipv, iSouXiaIndexParam);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}

		bRet = g_ItemOperator.SetItemParam( pHuman->GetEquipContain(),
			pHuman->GetEquipContain()->BagIndex2ConIndex(HEQUIP_SOUXIA),4, ipv, iSouXiaPos);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}
		// ��������ITEM paramע�Ᵽ�浽�ļ�,����Ҫ֪ͨITEM ��param �ı�
		UINT RetPart = 0;
		GCDetailEquipList	SelfEquipMsg;
		SelfEquipMsg.setObjID( pHuman->GetID() );
		pEquipSouXiaItem->SaveValueTo(SelfEquipMsg.GetEquipData((HUMAN_EQUIP)HEQUIP_SOUXIA));
		RetPart|=(1<<((UINT)HEQUIP_SOUXIA));
		SelfEquipMsg.SetAskPart(RetPart);
		pHuman->GetPlayer()->SendPacket(&SelfEquipMsg);
	}

	Assert(iSouXiaPos >= 0 && iSouXiaPos < MAX_SOUXIA_CONTAINER);
	curSouXia = pHuman->GetSouXia(iSouXiaPos);
	// ���������������ĵڼ���λ��
	BYTE result = curSouXia.AddSkillToSouXia(iStudySkillType, iParamSkillID, pGet->m_nUseTime, pGetEquip->m_nMaxPages);

	if( result != SOUXIA_SKILL_UPGRADE_SUCCESS)//���Ӽ��ܵ���Ӧ������ 
	{
		msg.SetResult((BYTE)result);//���ý��
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &msg );
		}
		return PACKET_EXE_CONTINUE;
	}
	// �������Ӽ��ܳɹ�/ʧ�ܵ���Ϣ,������¼
	msg.SetCurSouXiaPos( (SHORT) iSouXiaPos);
	msg.SetSkillType( (BYTE)pGet->m_nMemoryType );
	msg.SetSkillID( (SkillID_t)iParamSkillID );
	msg.SetCurUseTime( (SHORT)(pGet->m_nUseTime < 0 ? 0 : pGet->m_nUseTime) );
	msg.SetResult((BYTE)result);//���ý��

	Player* pPlayer = pHuman->GetPlayer();
	if( pPlayer != NULL )
	{
		pPlayer->SendPacket( &msg );
	}
	// �����뱣��
	pHuman->SetSouXia(curSouXia.GetSouXiaData(), iSouXiaPos);
	_SOUXIA_DB_LOAD		SouXia_db_Load;
	SouXia_db_Load.m_SouXiaData[iSouXiaPos] = curSouXia.GetSouXiaData();
	pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iSouXiaPos);	

	// �ɹ���ɾ�����������,���ظ����������Ʒ��Ϣ
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType	= ITEM_STUDY_SOUXIA;
	BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iBagIndex));
	if(bRet)
	{
		SaveItemLog(&ItemLogParam);
	}
	else
	{
		msg.SetResult(ERASE_ITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		Assert(FALSE);//ɾ��ʧ��
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

