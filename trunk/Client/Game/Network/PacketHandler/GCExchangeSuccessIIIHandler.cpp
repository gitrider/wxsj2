/*
�ͻ���ִ�����Ľ�������
*/
#include "stdafx.h"
#include "GCExchangeSuccessIII.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"


uint GCExchangeSuccessIIIHandler::Execute( GCExchangeSuccessIII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//1.����Ʒ
		//1.0ɾ������Ʒ
		for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++ )
		{
			tObject_Item* pItem = CDataPool::GetMe()->MyExBox_GetItem(i);
			if(pItem)
			{//�ӱ�����ɾ�������Ʒ
				INT index = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(pItem->GetGUID());
				if(index >= 0)
				{//�ɵ���
					CDataPool::GetMe()->UserBag_SetItem(index, NULL);
				}
			}
		}
		for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++ )
		{
			//SDATA_PET* pPetData = CDataPool::GetMe()->MyExBox_GetPet(i);
			//if(pPetData)
			//{//�ӱ�����ɾ�������Ʒ
			//	INT index = CDataPool::GetMe()->Pet_GetPetByGuid(pPetData->m_GUID);
			//	if(index >= 0)
			//	{//�ɵ���
			//		CDataPool::GetMe()->Pet_ClearPet(index);
			//	}
			//}
		}
		//1.1�õ�����Ʒ
		BYTE	ItemNum	=	pPacket->GetItemNum();		
		for(INT i = 0; i<ItemNum; i++)
		{
			GCExchangeSuccessIII::_SUCC_ITEM SucItem;
			SucItem = pPacket->GetItemList()[i];
			switch(SucItem.m_ToType)
			{
			case EXCHANGE_MSG::POS_BAG:
				{
					tObject_Item* pItem = CDataPool::GetMe()->OtExBox_GetItem(SucItem.m_FromIndex);
					CDataPool::GetMe()->UserBag_SetItem(SucItem.m_ToIndex, pItem,TRUE,TRUE);
					CDataPool::GetMe()->OtExBox_SetItem(SucItem.m_FromIndex,NULL,FALSE);
				}
				break;
			case EXCHANGE_MSG::POS_EQUIP:
				{
				}
				break;
//			case EXCHANGE_MSG::POS_PET:
//				{
//					//1.1��������Ʒ
//					SDATA_PET* pPetSourceData	=	CDataPool::GetMe()->OtExBox_GetPet(SucItem.m_FromIndex);
//					SDATA_PET* pPetDestData		=	CDataPool::GetMe()->Pet_GetPet(SucItem.m_ToIndex);
//
//					pPetDestData->m_nIsPresent		= pPetSourceData->m_nIsPresent;
//					pPetDestData->m_GUID			= pPetSourceData->m_GUID;
//					pPetDestData->m_idServer		= pPetSourceData->m_idServer;
//					pPetDestData->m_nDataID			= pPetSourceData->m_nDataID;
//					pPetDestData->m_nAIType			= pPetSourceData->m_nAIType;
//					pPetDestData->m_szName			= pPetSourceData->m_szName;
//					pPetDestData->m_nLevel			= pPetSourceData->m_nLevel;
//					pPetDestData->m_nExp			= pPetSourceData->m_nExp;
//					pPetDestData->m_nHP				= pPetSourceData->m_nHP;
//					pPetDestData->m_nHPMax			= pPetSourceData->m_nHPMax;
//					pPetDestData->m_nAge			= pPetSourceData->m_nAge;
//					pPetDestData->m_nEraCount		= pPetSourceData->m_nEraCount;
//					pPetDestData->m_nHappiness		= pPetSourceData->m_nHappiness;
//					pPetDestData->m_SpouseGUID		= pPetSourceData->m_SpouseGUID;
//					pPetDestData->m_nModelID		= pPetSourceData->m_nModelID;
//					pPetDestData->m_nMountID		= pPetSourceData->m_nMountID;
//
//					pPetDestData->m_nAtt_Near		= pPetSourceData->m_nAtt_Near;
//					pPetDestData->m_nDef_Near		= pPetSourceData->m_nDef_Near;
//					pPetDestData->m_nAtt_Far		= pPetSourceData->m_nAtt_Far;
//					pPetDestData->m_nDef_Far		= pPetSourceData->m_nDef_Far;
//
////					pPetDestData->m_nAttPhysics		= pPetSourceData->m_nAttPhysics;
//					pPetDestData->m_nAttMagic		= pPetSourceData->m_nAttMagic;
////					pPetDestData->m_nDefPhysics		= pPetSourceData->m_nDefPhysics;
//					pPetDestData->m_nDefMagic		= pPetSourceData->m_nDefMagic;
//					pPetDestData->m_nHit			= pPetSourceData->m_nHit;
//					pPetDestData->m_nMiss			= pPetSourceData->m_nMiss;
//					pPetDestData->m_nCritical		= pPetSourceData->m_nCritical;
//					pPetDestData->m_nAttrStrApt		= pPetSourceData->m_nAttrStrApt;
//					pPetDestData->m_nAttrConApt		= pPetSourceData->m_nAttrConApt;
//					pPetDestData->m_nAttrDexApt		= pPetSourceData->m_nAttrDexApt;
////					pPetDestData->m_nAttrSprApt		= pPetSourceData->m_nAttrSprApt;
//					pPetDestData->m_nAttrIntApt		= pPetSourceData->m_nAttrIntApt;
//					pPetDestData->m_nAttrStr		= pPetSourceData->m_nAttrStr;
//					pPetDestData->m_nAttrCon		= pPetSourceData->m_nAttrCon;
//					pPetDestData->m_nAttrDex		= pPetSourceData->m_nAttrDex;
////					pPetDestData->m_nAttrSpr		= pPetSourceData->m_nAttrSpr;
//					pPetDestData->m_nAttrInt		= pPetSourceData->m_nAttrInt;
//					pPetDestData->m_nBasic			= pPetSourceData->m_nBasic;
//					pPetDestData->m_nPot			= pPetSourceData->m_nPot;
//
//					for(INT i = 0; i<PET_MAX_SKILL_COUNT; i++ )
//					{
//						CDataPool::GetMe()->Pet_SetSkill(SucItem.m_ToIndex, i, NULL, TRUE);
//						if(pPetSourceData->m_aSkill[i])
//						{
//							PET_SKILL* pPetSkill = CDataPool::GetMe()->Pet_GetSkill(SucItem.m_ToIndex, i);
//
//							if(!pPetSkill)
//							{
//								pPetSkill = new PET_SKILL;
//								Assert(pPetSourceData->m_aSkill[i]);
//
//								pPetSkill->m_bCanUse	= pPetSourceData->m_aSkill[i]->m_bCanUse;
//								pPetSkill->m_nPetNum	= SucItem.m_ToIndex;
//								pPetSkill->m_nPosIndex	= i;
//								pPetSkill->m_pDefine	= pPetSourceData->m_aSkill[i]->m_pDefine;
//
//								CDataPool::GetMe()->Pet_SetSkill(SucItem.m_ToIndex, i, pPetSkill, TRUE);
//							}
//							else
//							{
//								Assert(0);
//								//pPetSkill->m_bCanUse	= pPetSourceData->m_aSkill[i]->m_bCanUse;
//								//pPetSkill->m_nPetNum	= SucItem.m_ToIndex;
//								//pPetSkill->m_nPosIndex	= i;
//								//pPetSkill->m_pDefine	= pPetSourceData->m_aSkill[i]->m_pDefine;
//							}
//							CDataPool::GetMe()->OtExBox_SetSkill(SucItem.m_FromIndex, i, NULL,TRUE);
//						}
//					}
//
//					CDataPool::GetMe()->OtExBox_SetPet(SucItem.m_FromIndex, NULL);
//				}
//				break;
//			default:
//				break;
			}
		}

		//2.��Ǯ
		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		UINT	OtMoney = CDataPool::GetMe()->OtExBox_GetMoney();
		UINT	MyMoney = CDataPool::GetMe()->MyExBox_GetMoney();

		if(MyMoney<=(UINT)pCharData->Get_Money()&&MyMoney>0)
		{
			pCharData->Set_Money(pCharData->Get_Money() - MyMoney);
		}

		if(OtMoney > 0)
		{
			pCharData->Set_Money(pCharData->Get_Money() + OtMoney);
		}

		//���
		CDataPool::GetMe()->OtExBox_Clear();
		CDataPool::GetMe()->MyExBox_Clear();
 
		//���±���
		CActionSystem::GetMe()->UserBag_Update();
		CActionSystem::GetMe()->UserExchangeSelf_Update();
		CActionSystem::GetMe()->UserExchangeOther_Update();

		//֪ͨ��ҽ��׳ɹ�
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GCExchangeSuccessHandler_Info_ExchangeSuccess");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���׳ɹ�");

		//���±�����Ʒ��������ܻ���Ҫ����װ������Ʒ�����Ҫ��װ�����ϵĶ����ǲ��ǿ����������ף�
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

		//�رս��׽���
		CEventSystem::GetMe()->PushEvent(GE_SUCCEED_EXCHANGE_CLOSE);
	}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
