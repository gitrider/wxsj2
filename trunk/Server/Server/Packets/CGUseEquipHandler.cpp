#include "stdafx.h"
#include "CGUseEquip.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCCharEquipment.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCUseEquipResult.h"
#include "ItemRuler.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCDetailEquipList.h"


using namespace Packets;

extern UINT GetEquipmentMaxLevelGemID(Item *pEquipment);

uint CGUseEquipHandler::Execute(CGUseEquip* pPacket, Player* pPlayer )
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

	//切场景时丢弃换装消息
	if(pGamePlayer->GetPlayerStatus()!=PS_SERVER_NORMAL ||
		!pHuman->IsActiveObj() )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler: change scene") ;
		return PACKET_EXE_CONTINUE;
	}
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

	BYTE  BagIndex = pPacket->getBagIndex();
	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, BagIndex);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler BagIndex is invalid, BagIndex = %d", BagIndex) ;

		return PACKET_EXE_ERROR;
	}

	Item* pUseItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(BagIndex));
	
	GCUseEquipResult Msg;

	//////////////////////////////////////////////////////////////////////////
	//装配条件判断
	UCHAR	itemClass = pUseItem->GetItemClass();

	if(itemClass == ICLASS_EQUIP)
	{
		if (pUseItem->GetDurPoints() == 0)
		{
			Msg.setResult(USEEQUIP_CURDUR_ZERO);
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler: cur Dur is Zero") ;
			return PACKET_EXE_CONTINUE;
		}
		if(!pUseItem->IsWuMenPai())
		{
			//需求等级判断,职业判断
			if(pUseItem->GetRequireLevel()>pHuman->GetLevel())
			{
				Msg.setResult(USEEQUIP_LEVEL_FAIL);
				pGamePlayer->SendPacket( &Msg ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler: can't equip") ;
				return PACKET_EXE_CONTINUE;
			}
			else if (!pUseItem->InReqJob(pHuman->GetMenPai()))
			{
				Msg.setResult(USEEQUIP_JOB_FAIL);
				pGamePlayer->SendPacket( &Msg ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler: can't equip") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		
		HUMAN_EQUIP		Equip_Point =(HUMAN_EQUIP)pUseItem->GetEquipPoint();
		ItemContainer*	pEquipContainer = pHuman->GetEquipContain();
		
		if(!pEquipContainer)
		{
			Assert(pEquipContainer);
			return PACKET_EXE_CONTINUE;
		}

		Item*	pEquipItem = pEquipContainer->GetItem((UINT)Equip_Point);

		if(!pEquipItem)
		{
			Assert(pEquipItem);
			return PACKET_EXE_CONTINUE;

		}
		
		BOOL			bEquipStatus = (!pEquipItem->IsEmpty());

		if(bEquipStatus)
		{
			if(Equip_Point == HEQUIP_RING1)
			{
				pEquipItem = pEquipContainer->GetItem((UINT)HEQUIP_RING2);

				if(!pEquipItem)
				{
					Assert(pEquipItem);
					return PACKET_EXE_CONTINUE;

				}

				BOOL b2EquipStatus = (pEquipItem->IsEmpty());
				if(b2EquipStatus)
				{
					g_ItemOperator.MoveItem(pBagContainer,
						pBagContainer->BagIndex2ConIndex(BagIndex),
						pEquipContainer,
						(UINT)HEQUIP_RING2);

					Equip_Point = HEQUIP_RING2;
				}

				else
				{
					if (!pBagContainer->IsCanUse())
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler extrabag is invalid time BagIndex = %d", BagIndex) ;
						return PACKET_EXE_CONTINUE ;
					}
					g_ItemOperator.ExchangeItem(pBagContainer,
						pBagContainer->BagIndex2ConIndex(BagIndex),
						pEquipContainer,
						(UINT)Equip_Point);
				}
			}
			else if(Equip_Point == HEQUIP_ADORN1)
			{
				pEquipItem = pEquipContainer->GetItem((UINT)HEQUIP_ADORN2);

				if(!pEquipItem)
				{
					Assert(pEquipItem);
					return PACKET_EXE_CONTINUE;

				}

				BOOL b2EquipStatus = (pEquipItem->IsEmpty());
				if(b2EquipStatus)
				{
					g_ItemOperator.MoveItem(pBagContainer,
						pBagContainer->BagIndex2ConIndex(BagIndex),
						pEquipContainer,
						(UINT)HEQUIP_ADORN2);

					Equip_Point = HEQUIP_ADORN2;
				}
				else
				{
					if (!pBagContainer->IsCanUse())
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler extrabag is invalid time BagIndex = %d", BagIndex) ;
						return PACKET_EXE_CONTINUE ;
					}
					g_ItemOperator.ExchangeItem(pBagContainer,
						pBagContainer->BagIndex2ConIndex(BagIndex),
						pEquipContainer,
						(UINT)Equip_Point);
				}
			}
			else
			{
				if (!pBagContainer->IsCanUse())
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler extrabag is invalid time BagIndex = %d", BagIndex) ;
					return PACKET_EXE_CONTINUE ;
				}
				g_ItemOperator.ExchangeItem(pBagContainer,
					pBagContainer->BagIndex2ConIndex(BagIndex),
					pEquipContainer,
					(UINT)Equip_Point);
			}
		}
		else
		{ 
			g_ItemOperator.MoveItem(pBagContainer,
									pBagContainer->BagIndex2ConIndex(BagIndex),
									pEquipContainer,
									(UINT)Equip_Point);
		}
		
		Item* pEquip = pEquipContainer->GetItem((UINT)Equip_Point);
		
		if(pEquip&&pEquip->IsCanEquipBind())
		{
			g_ItemOperator.SetItemBind(pEquipContainer,(UINT)Equip_Point);	
		}

		Msg.setBagIndex(BagIndex);
		Msg.setEquipPoint(Equip_Point);
		Msg.setEquipID(pEquipItem->GetGUID());
		Msg.setEquipResID(pEquipItem->GetItemTableIndex());
		Msg.setItemResID(pUseItem->GetItemTableIndex());
		Msg.setBagItemId(pUseItem->GetGUID());
		Msg.setResult(USEEQUIP_SUCCESS);
		
		pGamePlayer->SendPacket( &Msg ) ;
		pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
		//刷新装备对人物的属性影响

		pHuman->ItemEffectFlush();

		//如果可见
		if(pHuman->IsVisualPart(Equip_Point))
		{	
			GCCharEquipment OtherMsg;
			OtherMsg.setObjID(pHuman->GetID());
			OtherMsg.setID(Equip_Point,pEquip->GetItemTableIndex());

			pScene->BroadCast(&OtherMsg,pHuman,TRUE);

		}


		UINT RetPart = 0; // modify by gh 增加了捜侠录以后变成了16 就会越界故将WORD改成了UINT
		GCDetailEquipList	SelfMsg;
		SelfMsg.setObjID( pHuman->GetID() );
		pEquipItem = HumanItemLogic::GetEquip(pHuman,Equip_Point);
		if(!pEquipItem)
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		if(pEquipItem->IsEmpty()) 
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler: Equip_Point =%d Fail", Equip_Point ) ;
			return PACKET_EXE_CONTINUE;
		}
			
		pEquipItem->SaveValueTo(SelfMsg.GetEquipData(Equip_Point));
		RetPart|=(1<<((UINT)Equip_Point));
		SelfMsg.SetAskPart(RetPart);
		pGamePlayer->SendPacket( &SelfMsg ) ;
	}
	else
	{
		Msg.setResult(USEEQUIP_TYPE_FAIL);
		Msg.setBagIndex(0);
		Msg.setEquipPoint(0);
		pGamePlayer->SendPacket( &Msg ) ;
		
	}
	

	g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipHandler: BagIndex=%d", BagIndex ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}