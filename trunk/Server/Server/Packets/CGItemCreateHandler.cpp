#include "stdafx.h"
#include "CGItemCreate.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "GCItemCreateResult.h"
#include "GCNotifyEquip.h"
using namespace Packets;

uint CGItemCreateHandler::Execute(CGItemCreate* pPacket,Player* pPlayer)
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

	GCItemCreateResult Msg;
	UINT uBagIndex = INVALID_ITEM_POS;
	ITEMCREATE_RESULT nResult;
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

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

	// add by gh 2010/05/19
	BOOL bNeedNpc = TRUE;
	ITEM_PRODUCT_INFO_TB* pCreateTB = g_ItemTable.GetItemProductInfoTB(pPacket->GetItemIndex());
	if(pCreateTB == NULL)
	{
		nResult = ITEMCREATE_ITEM_FAIL;
		Msg.SetResult(nResult);
		pGamePlayer->SendPacket(&Msg);
		return PACKET_EXE_CONTINUE;
	}
	if (0 == pCreateTB->m_NeedNPC)
	{
		bNeedNpc = FALSE;
	}
		 
	if (bNeedNpc)
	{
		ObjID_t		NpcObjID=	pPacket->GetUniqueID();
		/*if(SceneID != pScene->SceneID())
		{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler Illegal scene ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
		}*/

		//距离判定
		Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcObjID));
		if(pNpcObj == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGItemCreateHandler Illegal Obj ObjName = %s", pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE ; 
		}

		FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

		if(fDist>MAX_NPC_DISTANCE)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGItemCreateHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE ; 
		}
	}
	else // 判定捜侠录的已经学习过的神器配方有无该配方 
	{
		if (!pHuman->HasLearnedProduct(pPacket->GetUniqueID()))//NPC Id 成了配方ID
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGItemCreateHandler Need Study by SouXia ProductIndex = %d", pPacket->GetItemIndex()) ;
			return PACKET_EXE_CONTINUE ; 
		}
	}
	

	if (pPacket->GetValidNum()>=MAX_CREATE_MATERIAL_NUM)
	{
		return PACKET_EXE_ERROR;
	}
	UINT nEquipScore = 0;
	switch(GetSerialClass(pPacket->GetItemIndex()))
	{
	case ICLASS_EQUIP:
		{
			EQUIP_TB* pEquipTB = g_ItemTable.GetEquipTB(pPacket->GetItemIndex());
			if (pEquipTB == NULL)
			{
				nResult = ITEMCREATE_ITEM_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket(&Msg);
				return PACKET_EXE_CONTINUE;
			}

			INT nSpace = HumanItemLogic::CalcBagSpace(pHuman);

			if (nSpace<=0)
			{
				nResult = ITEMCREATE_BAG_FULL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket(&Msg);
				return PACKET_EXE_CONTINUE;
			}

			ITEM_PRODUCT_INFO_TB* pCreateTB = g_ItemTable.GetItemProductInfoTB(pPacket->GetItemIndex());
			if(pCreateTB == NULL)
			{
				nResult = ITEMCREATE_ITEM_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket(&Msg);
				return PACKET_EXE_CONTINUE;
			}
			
			if (pCreateTB->m_nCreateInfoCount != pPacket->GetValidNum())
			{
				return PACKET_EXE_ERROR;
			}

			if (pCreateTB->m_CreateMoney > pHuman->GetMoney())
			{
				nResult = ITEMCREATE_MONEY_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket(&Msg);
				return PACKET_EXE_CONTINUE;
			}

			struct MATERIAL_REQ 
			{
				UINT m_nIndex;
				UINT m_nCount;
			};
			MATERIAL_REQ MaterialIndex[MAX_CREATE_MATERIAL_NUM];
			memset(MaterialIndex, 0, sizeof(MATERIAL_REQ)*pCreateTB->m_nCreateInfoCount);
			MATERIAL_LEVEL_INFO_TB* pMaxMaterialLevelInfo = g_ItemTable.GetMaterialInfoTB(MAX_MATERIAL_LEVEL);
			Assert(pMaxMaterialLevelInfo);
			if (!pMaxMaterialLevelInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[CGItemCreateHandler::Execute] GetMaterialInfoTB index is error [%d]",MAX_MATERIAL_LEVEL);
				return PACKET_EXE_ERROR;
			}
			for(UINT i=0; i<pCreateTB->m_nCreateInfoCount; ++i)
			{
				CREATEITEM_MATERIAL_INFO MaterialInfo = pPacket->GetMaterialReq(i);
				
				//无效数据
				if ((MaterialInfo.m_nMaterialType == -1)||(MaterialInfo.m_nMaterialLevel == 0)
					||(MaterialInfo.m_nMaterialLevel > MAX_VALIDMATERIAL_LEVEL))
				{
					return PACKET_EXE_ERROR;
				}
				//无效材料类型
				if (pCreateTB->m_CreateInfo[i].m_MaterialType != MaterialInfo.m_nMaterialType)
				{
					nResult = ITEMCREATE_MATERIAL_FAIL;
					Msg.SetResult(nResult);
					pGamePlayer->SendPacket(&Msg);
					return PACKET_EXE_CONTINUE;
				}
				
				//材料等级不够
				if(pCreateTB->m_CreateInfo[i].m_NeedLevel>MaterialInfo.m_nMaterialLevel)
				{
					nResult = ITEMCREATE_MATERIAL_LEVEL;
					Msg.SetResult(nResult);
					pGamePlayer->SendPacket(&Msg);
					return PACKET_EXE_CONTINUE;
				}
				
				UINT nMaterialIndex = GenMaterialIndex(MaterialInfo.m_nMaterialType, MaterialInfo.m_nMaterialLevel);

				//查询物品表该材料是否存在
				COMMITEM_INFO_TB* pMaterialInfoTB = g_ItemTable.GetCommItemInfoTB(nMaterialIndex);
				if(pMaterialInfoTB == NULL)
				{
					return PACKET_EXE_ERROR;
				}

				//查询背包中该材料数量
				INT nCount = HumanItemLogic::CalcBagItemCount(pHuman, nMaterialIndex);
				//材料数量不够
				if(pCreateTB->m_CreateInfo[i].m_MaterialNum>nCount)
				{
					nResult = ITEMCREATE_MATERIAL_COUNT;
					Msg.SetResult(nResult);
					pGamePlayer->SendPacket(&Msg);
					return PACKET_EXE_CONTINUE;
				}
				
				//获取对应材料分
				MATERIAL_LEVEL_INFO_TB* pMaterialLevelInfo = g_ItemTable.GetMaterialInfoTB(MaterialInfo.m_nMaterialLevel);
				Assert(pMaterialLevelInfo != NULL);
				if (!pMaterialLevelInfo)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "[CGItemCreateHandler::Execute] GetMaterialInfoTB index is error [%d]",MaterialInfo.m_nMaterialLevel);
					return PACKET_EXE_ERROR;
				}
				nEquipScore = nEquipScore + pMaterialLevelInfo->m_MaterialValue;
				MaterialIndex[i].m_nIndex = nMaterialIndex;
				MaterialIndex[i].m_nCount = pCreateTB->m_CreateInfo[i].m_MaterialNum;
			}

			ITEM_LOG_PARAM LogParam;
			LogParam.OpType     =   ITEM_CREATE_FROMMATERIAL;
			LogParam.CharGUID	=	pHuman->GetGUID();
			LogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			LogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			LogParam.SceneID	=	pHuman->getScene()->SceneID();

			if (nEquipScore>=pMaxMaterialLevelInfo->m_MaterialValue)
			{
				nEquipScore = pMaxMaterialLevelInfo->m_MaterialValue;
			}
			_HUMAN_EQUIPCREATE_INIT EquipInit;
			EquipInit.m_nItemIndex = pPacket->GetItemIndex();
			EquipInit.m_nEquipFromType = ITEM_CREATE;
			EquipInit.m_nEquipScore = nEquipScore;
			EquipInit.m_nPlayerJob	= pHuman->GetMenPai();

			BOOL Ret = HumanItemLogic::CreateItemToBag(&LogParam, pHuman,&EquipInit, uBagIndex);

			if (Ret)
			{
				SaveItemLog(&LogParam);

				LogParam.CleanUp();
				LogParam.OpType     =   ITEM_CREATE_MATERIALCONSUME;
				LogParam.CharGUID	=	pHuman->GetGUID();
				LogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				LogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				LogParam.SceneID	=	pHuman->getScene()->SceneID();
				for (UINT i=0; i<pCreateTB->m_nCreateInfoCount; ++i)
				{
					if(HumanItemLogic::EraseBagItem(&LogParam, pHuman,MaterialIndex[i].m_nIndex, MaterialIndex[i].m_nCount))
					{
							SaveItemLog(&LogParam);
					}
				}

				pHuman->SetMoney(pHuman->GetMoney() - pCreateTB->m_CreateMoney);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_CREATE_REDUCE;	
				MoneyLogParam.Count		=	(INT)pCreateTB->m_CreateMoney;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
		}
		break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGItemCreateHandler: item type error, type=%d", GetSerialType(pPacket->GetItemIndex()) ) ;
			return PACKET_EXE_ERROR;
		}
	    break;
	}
	
	nResult = ITEMCREATE_SUCCESS;
	Msg.SetResult(nResult);
	pGamePlayer->SendPacket(&Msg);

	Item* pCreateItem  = HumanItemLogic::GetBagItem(pHuman, uBagIndex);
	Assert(pCreateItem);

	GCNotifyEquip ItemMsg;
	ItemMsg.SetBagIndex(uBagIndex);
	pCreateItem->SaveValueTo(ItemMsg.GetItem());
	pGamePlayer->SendPacket(&ItemMsg);

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}