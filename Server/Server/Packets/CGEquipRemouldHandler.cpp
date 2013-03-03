#include "stdafx.h"
#include "CGEquipRemould.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "GCEquipRemouldResult.h"
#include "GCCharEquipment.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
using namespace Packets;

uint CGEquipRemouldHandler::Execute(CGEquipRemould* pPacket,Player* pPlayer)
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

	Item*		pEquipItem		= NULL;
	UINT		EquipPoint		= INVALID_ITEM_POS;
	UINT		BagIndex		= INVALID_ITEM_POS;  
	EQUIPREMOULD_RESULT nResult	= EQUIPREMOULD_FAIL;
	GCEquipRemouldResult Msg;
	switch(pPacket->GetEquipPosType())
	{
	case ENUM_EQUIP_POINT:
		{
			EquipPoint = pPacket->GetPos();
			if(EquipPoint>HEQUIP_ADORN2)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler: EquipPoint error, EquipPoint=%d", EquipPoint) ;
				return PACKET_EXE_ERROR;
			}

			ItemContainer* pEquipContainer = pHuman->GetEquipContain();
			if(NULL==pEquipContainer)
			{
				AssertEx(FALSE,"[CGEquipRemouldHandler]: NULL EquipContainer pointer found!");
			}
			pEquipItem = pEquipContainer->GetItem((UINT)EquipPoint);
			Assert(pEquipItem);

			if(pEquipItem->IsEmpty())
			{
				nResult = EQUIPREMOULD_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler pEquipItem is Null at  EquipPoint= %d", EquipPoint) ;
				return PACKET_EXE_CONTINUE;
			}

			Assert (pEquipItem->GetItemClass() == ICLASS_EQUIP);

			if(!(pPacket->getItemID() == pEquipItem->GetGUID()))
			{
				nResult = EQUIPREMOULD_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler GUID is different ") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	case ENUM_BAG:
		{
			BagIndex = pPacket->GetPos();
			ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman,BagIndex);
			if( pItemContainer == NULL )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler: BagIndex error, BagIndex=%d", BagIndex) ;
				return PACKET_EXE_ERROR;
			}
			if (!pItemContainer->IsCanUse())
			{
				nResult = EQUIPREMOULD_BAG_INVALID;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler: ItemContainer is invalid, ContainerType=%d, BagIndex=%d", 
					pItemContainer->GetContainerType(), BagIndex) ;
					return PACKET_EXE_ERROR;
			}

			pEquipItem		=	pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
			if(pEquipItem->IsEmpty())
			{
				nResult = EQUIPREMOULD_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler pEquipItem is Null at  BagIndex= %d", BagIndex) ;
				return PACKET_EXE_CONTINUE;
			}

			if (pEquipItem->GetItemClass() != ICLASS_EQUIP)
			{
				nResult = EQUIPREMOULD_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler GetItemClass is not ICLASS_EQUIP ") ;
				return PACKET_EXE_CONTINUE;
			}

			if(!(pPacket->getItemID() == pEquipItem->GetGUID()))
			{
				nResult = EQUIPREMOULD_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler GUID is different ") ;
				return PACKET_EXE_CONTINUE;
			}

			if (pEquipItem->GetItemType() > EQUIP_ADORN)
			{
				nResult = EQUIPREMOULD_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler GUID is different ") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler: EquipFromType error, EquipFromType=%d", pPacket->GetEquipPosType()) ;
			return PACKET_EXE_ERROR;
		}
		break;
	}

	if (pEquipItem->GetEquipFromType() != ITEM_CREATE)
	{
		nResult = EQUIPREMOULD_TYPE_FAIL;
		Msg.SetResult(nResult);
		pGamePlayer->SendPacket( &Msg ) ;
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler Type error ") ;
		return PACKET_EXE_CONTINUE;
	}

	ITEM_PRODUCT_INFO_TB* pCreateTB = g_ItemTable.GetItemProductInfoTB(pEquipItem->GetItemTableIndex());
	if(pCreateTB == NULL)
	{
		nResult = EQUIPREMOULD_EQUIP_FAIL;
		Msg.SetResult(nResult);
		pGamePlayer->SendPacket(&Msg);
		return PACKET_EXE_CONTINUE;
	}
	if (pCreateTB->m_nRemouldInfoCount != pPacket->GetValidNum())
	{
		return PACKET_EXE_ERROR;
	}
	
	if (pCreateTB->m_RemouldMoney > pHuman->GetMoney())
	{
		nResult = EQUIPREMOULD_MONEY_FAIL;
		Msg.SetResult(nResult);
		pGamePlayer->SendPacket(&Msg);
		return PACKET_EXE_CONTINUE;
	}
	struct MATERIAL_REQ 
	{
		UINT m_nIndex;
		UINT m_nCount;
	};
	MATERIAL_REQ MaterialIndex[MAX_REMOULD_MATERIAL_NUM];
	UINT nEquipScore = 0;
	for (UINT i=0; i<pCreateTB->m_nRemouldInfoCount; ++i)
	{
		REMOULDITEM_INFO RemouldInfo = pCreateTB->m_RemouldInfo[i];

		EQUIPREMOULD_MATERIAL_INFO MaterialInfo = pPacket->GetMaterialReq(i);
		if(MaterialInfo.m_nMaterialType != RemouldInfo.m_MaterialType)
		{
			nResult = EQUIPREMOULD_MATERIAL_FAIL;
			Msg.SetResult(nResult);
			pGamePlayer->SendPacket( &Msg ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler MaterialType error") ;
			return PACKET_EXE_CONTINUE;
		}

		if(MaterialInfo.m_nMaterialLevel < RemouldInfo.m_NeedLevel)
		{
			nResult = EQUIPREMOULD_MATERIAL_LEVEL;
			Msg.SetResult(nResult);
			pGamePlayer->SendPacket( &Msg ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler MaterialLevel fail") ;
			return PACKET_EXE_CONTINUE;
		}

		UINT nMaterialIndex = GenMaterialIndex(MaterialInfo.m_nMaterialType, MaterialInfo.m_nMaterialLevel);
		//查询物品表该材料是否存在
		COMMITEM_INFO_TB* pMaterialInfoTB = g_ItemTable.GetCommItemInfoTB(nMaterialIndex);
		if(pMaterialInfoTB == NULL)
		{
			return PACKET_EXE_ERROR;
		}

		INT nCount = HumanItemLogic::CalcBagItemCount(pHuman, nMaterialIndex);
		if (nCount<RemouldInfo.m_MaterialNum)
		{
			nResult = EQUIPREMOULD_MATERIAL_COUNT;
			Msg.SetResult(nResult);
			pGamePlayer->SendPacket( &Msg ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler Material num fail") ;
			return PACKET_EXE_CONTINUE;
		}

		//获取对应材料分
		MATERIAL_LEVEL_INFO_TB* pMaterialLevelInfo = g_ItemTable.GetMaterialInfoTB(MaterialInfo.m_nMaterialLevel);
		Assert(pMaterialLevelInfo != NULL);
		if (!pMaterialLevelInfo)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[CGEquipRemouldHandler::Execute] GetMaterialInfoTB index is error [%d]",MaterialInfo.m_nMaterialLevel);
			return PACKET_EXE_ERROR;
		}
		nEquipScore = nEquipScore + pMaterialLevelInfo->m_MaterialValue;

		MaterialIndex[i].m_nIndex = nMaterialIndex;
		MaterialIndex[i].m_nCount = pCreateTB->m_RemouldInfo[i].m_MaterialNum;
	}

	nResult = (EQUIPREMOULD_RESULT)pHuman->EquipRemould(pEquipItem, nEquipScore);
	if(nResult == EQUIPREMOULD_FAIL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler: pHuman->AddHole Fail") ;
		return PACKET_EXE_ERROR;
	}

	if (nResult == EQUIPREMOULD_SUCCESS)
	{
		ITEM_LOG_PARAM LogParam;
		LogParam.OpType     =   ITEM_REMOULD_CONSUME;
		LogParam.CharGUID	=	pHuman->GetGUID();
		LogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		LogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		LogParam.SceneID	=	pHuman->getScene()->SceneID();

		for (UINT i=0; i<pCreateTB->m_nRemouldInfoCount; ++i)
		{
			if(HumanItemLogic::EraseBagItem(&LogParam, pHuman,MaterialIndex[i].m_nIndex, MaterialIndex[i].m_nCount))
			{
				SaveItemLog(&LogParam);
			}
		}
		
		pHuman->SetMoney(pHuman->GetMoney() - pCreateTB->m_RemouldMoney);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_REMOULD_REDUCE;	
		MoneyLogParam.Count		=	(INT)pCreateTB->m_RemouldMoney;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);
	}

	Msg.SetResult(nResult);
	pGamePlayer->SendPacket( &Msg ) ;
	//////////////////////////////////////////////////////////////////////////

	if((nResult == EQUIPREMOULD_SUCCESS)||(nResult == EQUIPREMOULD_MAX_FAIL))
	{
		if(pPacket->GetEquipPosType() == ENUM_EQUIP_POINT)
		{
			pHuman->ItemEffectFlush();

			pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
			INT Equip_Point = pPacket->GetPos();
			UINT RetPart = 0;
			GCDetailEquipList	SelfMsg;
			SelfMsg.setObjID( pHuman->GetID() );
			Item* pEquipItem = HumanItemLogic::GetEquip(pHuman, (HUMAN_EQUIP)Equip_Point);
			if(!pEquipItem)
			{
				Assert(FALSE);
			}
			if(pEquipItem->IsEmpty()) 
			{
				Assert(FALSE);
			}
			pEquipItem->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)Equip_Point));
			RetPart|=(1<<((UINT)Equip_Point));
			SelfMsg.SetAskPart(RetPart);
			pGamePlayer->SendPacket( &SelfMsg ) ;
		}
		else
		{
			GCItemInfo	 ItemMsg;
			ItemMsg.setIsNull(FALSE);
			ItemMsg.setID(BagIndex);
			pEquipItem->SaveValueTo(ItemMsg.getItem());
			pHuman->GetPlayer()->SendPacket(&ItemMsg);

		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRemouldHandler: success nResult=%d", nResult ) ;
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}