#include "stdafx.h"
#include "CGEquipDecompound.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "GCEquipDecompoundResult.h"
#include "GCCharEquipment.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
using namespace Packets;

uint CGEquipDecompoundHandler::Execute(CGEquipDecompound* pPacket,Player* pPlayer)
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
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: ObjID=%d, ExchangBox::m_Status>0" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//摆摊状态不可操作
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	ObjID_t		NpcObjID=	pPacket->GetUniqueID();
	Obj* pNpcObj = pScene->GetObjManager()->GetObj(NpcObjID);
	if (pNpcObj == NULL)
	{
		return PACKET_EXE_ERROR;
	}
	else
	{
		if( pNpcObj->GetObjType() != Obj::OBJ_TYPE_MONSTER )
		{
			return PACKET_EXE_ERROR;
		}
	}

	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

	if(fDist>MAX_NPC_DISTANCE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	Item*		pEquipItem		= NULL;
	UINT		EquipPoint		= INVALID_ITEM_POS;
	UINT		BagIndex		= INVALID_ITEM_POS;  
	EQUIPDECOMPOUND_RESULT nResult	= EQUIPDECOMPOUND_FAIL;
	GCEquipDecompoundResult Msg;
	switch(pPacket->GetEquipPosType())
	{
	case ENUM_EQUIP_POINT:
		{
			EquipPoint = pPacket->GetPos();
			if(EquipPoint>HEQUIP_ADORN2)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: EquipPoint error, EquipPoint=%d", EquipPoint) ;
				return PACKET_EXE_ERROR;
			}

			ItemContainer* pEquipContainer = pHuman->GetEquipContain();
			if(NULL==pEquipContainer)
			{
				AssertEx(FALSE,"[CGEquipDecompoundHandler]: NULL EquipContainer pointer found!");
			}
			pEquipItem = pEquipContainer->GetItem((UINT)EquipPoint);
			Assert(pEquipItem);

			if(pEquipItem->IsEmpty())
			{
				nResult = EQUIPDECOMPOUND_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler pEquipItem is Null at  EquipPoint= %d", EquipPoint) ;
				return PACKET_EXE_CONTINUE;
			}

			Assert (pEquipItem->GetItemClass() == ICLASS_EQUIP);

			if(!(pPacket->getItemID() == pEquipItem->GetGUID()))
			{
				nResult = EQUIPDECOMPOUND_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler GUID is different ") ;
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
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: BagIndex error, BagIndex=%d", BagIndex) ;
				return PACKET_EXE_ERROR;
			}
			if (!pItemContainer->IsCanUse())
			{
				nResult = EQUIPDECOMPOUND_BAG_INVALID;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: ItemContainer is invalid, ContainerType=%d, BagIndex=%d",
					pItemContainer->GetContainerType(), BagIndex) ;
				return PACKET_EXE_ERROR;
			}

			pEquipItem		=	pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
			if(pEquipItem->IsEmpty())
			{
				nResult = EQUIPDECOMPOUND_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler pEquipItem is Null at  BagIndex= %d", BagIndex) ;
				return PACKET_EXE_CONTINUE;
			}

			if (pEquipItem->GetItemClass() != ICLASS_EQUIP)
			{
				nResult = EQUIPDECOMPOUND_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler GetItemClass is not ICLASS_EQUIP ") ;
				return PACKET_EXE_CONTINUE;
			}

			if(!(pPacket->getItemID() == pEquipItem->GetGUID()))
			{
				nResult = EQUIPDECOMPOUND_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler GUID is different ") ;
				return PACKET_EXE_CONTINUE;
			}

			if (pEquipItem->GetItemType() > EQUIP_ADORN)
			{
				nResult = EQUIPDECOMPOUND_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler GUID is different ") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: EquipFromType error, EquipFromType=%d", pPacket->GetEquipPosType()) ;
			return PACKET_EXE_ERROR;
		}
		break;
	}

	if (pEquipItem->GetEquipFromType() != ITEM_CREATE)
	{
		nResult = EQUIPDECOMPOUND_TYPE_FAIL;
		Msg.SetResult(nResult);
		pGamePlayer->SendPacket( &Msg ) ;
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler Type error ") ;
		return PACKET_EXE_CONTINUE;
	}

	ITEM_PRODUCT_INFO_TB* pCreateTB = g_ItemTable.GetItemProductInfoTB(pEquipItem->GetItemTableIndex());
	if(pCreateTB == NULL)
	{
		nResult = EQUIPDECOMPOUND_EQUIP_FAIL;
		Msg.SetResult(nResult);
		pGamePlayer->SendPacket(&Msg);
		return PACKET_EXE_CONTINUE;
	}

	UINT nSpace = HumanItemLogic::CalcBagSpace(pHuman);
	if (nSpace<pCreateTB->m_nCreateInfoCount)
	{
		nResult = EQUIPDECOMPOUND_BAG_FAIL;
		Msg.SetResult(nResult);
		pGamePlayer->SendPacket(&Msg);
		return PACKET_EXE_CONTINUE;
	}

	UINT nScore = pEquipItem->GetEquipScore();
	UINT nLevel = 0;
	MATERIAL_LEVEL_INFO_TB* pMaterialLevelInfo = NULL;
	for (UINT i=1; i<=MAX_VALIDMATERIAL_LEVEL; ++i)
	{
		pMaterialLevelInfo = g_ItemTable.GetMaterialInfoTB(i);
		if (!pMaterialLevelInfo)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[CGEquipDecompoundHandler::Execute] GetMaterialInfoTB index is error [%d]",i);			
			return PACKET_EXE_ERROR;
		}
		if (nScore<pMaterialLevelInfo->m_MaterialValue)
		{
			nLevel = i-1;
			break;
		}
	}

	if (nLevel>0)
	{
		ITEM_LOG_PARAM LogParam;
		for(UINT i=0; i<pCreateTB->m_nCreateInfoCount; ++i)
		{
			//查询材料拆解等级
			nScore = pEquipItem->GetEquipScore()*pCreateTB->m_CreateInfo[i].m_SplitRate/(100*pCreateTB->m_nCreateInfoCount*pCreateTB->m_CreateInfo[i].m_MaterialNum);
			for (UINT j=1; j<=MAX_VALIDMATERIAL_LEVEL; ++j)
			{
				pMaterialLevelInfo = g_ItemTable.GetMaterialInfoTB(j);
				if (!pMaterialLevelInfo)
				{
     				g_pLog->FastSaveLog( LOG_FILE_1, "[CGEquipDecompoundHandler::Execute] GetMaterialInfoTB index is error [%d]",j);
					return PACKET_EXE_ERROR;
				}
				if (nScore<pMaterialLevelInfo->m_MaterialValue)
				{
					nLevel = j-1;
					break;
				}
			}
			if (nLevel <= 0)
			{
				nLevel = 1;
			}
			if (nLevel>0)
			{
				UINT nMaterialIndex = GenMaterialIndex(pCreateTB->m_CreateInfo[i].m_MaterialType, nLevel);
				//查询物品表该材料是否存在
				COMMITEM_INFO_TB* pMaterialInfoTB = g_ItemTable.GetCommItemInfoTB(nMaterialIndex);
				Assert(pMaterialInfoTB != NULL);
				if (!pMaterialInfoTB)
				{
     				g_pLog->FastSaveLog( LOG_FILE_1, "[CGEquipDecompoundHandler::Execute] GetCommItemInfoTB index is error [%d]",nMaterialIndex);
					return PACKET_EXE_ERROR;
				}

				UINT nCount = (UINT)(pCreateTB->m_CreateInfo[i].m_MaterialNum*((FLOAT)pCreateTB->m_CreateInfo[i].m_SplitRate/100.0));
				Assert(nCount<pCreateTB->m_CreateInfo[i].m_MaterialNum);
				if (nCount ==0 )
				{
					nCount = 1;
				}
				LogParam.OpType	  = ITEM_DECOMPOUND_MATERIALGET;
				LogParam.ItemType = nMaterialIndex;
				BOOL ret = HumanItemLogic::CreateMultiItemToBag(&LogParam, pHuman, nMaterialIndex, nCount);
				Assert(ret);
				SaveItemLog(&LogParam);
			}
		}
	}

	nResult = EQUIPDECOMPOUND_SUCCESS;
	//////////////////////////////////////////////////////////////////////////

	if((nResult == EQUIPDECOMPOUND_SUCCESS))
	{
		if(pPacket->GetEquipPosType() == ENUM_EQUIP_POINT)
		{
			HumanItemLogic::EraseEquip(pHuman,(HUMAN_EQUIP)EquipPoint);
			pHuman->ItemEffectFlush();

			pHuman->SetEquipVer(pHuman->GetEquipVer()+1);

			Msg.SetResult(nResult);
			Msg.SetEquipPoint(EquipPoint);
			pGamePlayer->SendPacket(&Msg);

			GCCharEquipment OtherMsg;
			//如果可见
			if(pHuman->IsVisualPart((HUMAN_EQUIP)EquipPoint))
			{	
				OtherMsg.setObjID(pHuman->GetID());
				OtherMsg.setID((HUMAN_EQUIP)EquipPoint,-1);
				pScene->BroadCast(&OtherMsg,pHuman,TRUE);
			}
		}
		else
		{
			ITEM_LOG_PARAM logParam;
			logParam.OpType	  = ITEM_DECOMPOUND_REMOVE;
			HumanItemLogic::EraseBagItem(&logParam, pHuman, BagIndex);

			SaveItemLog(&logParam);
			GCItemInfo	 ItemMsg;
			ItemMsg.setIsNull(TRUE);
			ItemMsg.setID(BagIndex);
			pHuman->GetPlayer()->SendPacket(&ItemMsg);
			Msg.SetResult(nResult);
			pHuman->GetPlayer()->SendPacket(&Msg);
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: success nResult=%d", nResult ) ;
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}