// CGShopRepairHandler.cpp

/*  旧的修理费用公式
修理费用计算：
V：物品的标准价格，查询道具表
L：最大耐久
A暂定A=6，建议开放
x：当前耐久，0<=x<=L
y：修理费用
公式：
当0<=x<1/3L时：y= -7*V^2*x*A/（4*L）+V^2
当1/3L<=x<2/3L时：y= -V^2*x*A/L+3*V^2/4
当2/3L<=x<=L时：y= -V^2*x*A/（4*L）+V^2/4
*/


/*  新的修理费用公式
修理费用计算：
V：物品的标准价格，查询道具表
L：最大耐久
x：当前耐久，0<=x<=L
y：修理费用
公式：
y=a*(1-x/L)*V，a为参数暂定a=1，建议开放
*/

#define REPAIR_SPEND 1

#include "stdafx.h"
#include "GCShopRepair.h"
#include "GCShopSell.h"
#include "GCShopBuy.h"
#include "CGShopRepair.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Log.h"
#include "ShopManager.h"
#include "HumanItemLogic.h"

uint CGShopRepairHandler::Execute( CGShopRepair* pPacket, Player* pPlayer )
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

	BYTE	Opt         =	pPacket->GetOpt();
	BOOL	RepairAll	=	pPacket->IsRepairAll();		//修理全部
	BYTE	BagIndex	=	pPacket->GetBagIndex();		//包中的位置
	UINT	UniqueID	=	pPacket->GetUniqueID();

	/*UINT	UniqueID	=	pPacket->GetUniqueID();
	SceneID_t	SceneID =	UniqueID>>16;
	ObjID_t		NpcObjID=	UniqueID&0x00ff;*/

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
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler Illegal Obj ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

	if(fDist>MAX_NPC_DISTANCE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	Item*	pCurItem = NULL;
	INT				MoneySpent = 0;
	INT				MoneyLast = 0;
	INT				MoneyHave  = pHuman->GetMoney();
	INT				RepairedIndex = 0;
	GCShopRepair	Msg;
	GCShopRepair::REPAIRED_ITEM	ItemList[MAX_REPAIRED_NUMBER];

	_SHOP*		pShop	=	pHuman->Shop( )	;
	if(!pShop)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGShopRepairHandler Npc Shop Lose");
		return PACKET_EXE_CONTINUE;
	}

	if(Opt == CGShopRepair::FromEquip)
	{
		BOOL freshAtt = FALSE;
		if(RepairAll)
		{
			INT nBeginPoint	= 0;
			INT nEndPoint	= 0;
		
			for(INT i =0; i<HEQUIP_NUMBER; i++)
			{
				pCurItem	= HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)i);
				if(!pCurItem->IsEmpty())
				{//有东西
					if(pCurItem->GetMaxDurPoint() == -1)
					{
						continue;
					}
					FLOAT fMax		= (FLOAT)pCurItem->GetMaxDurPoint();
					FLOAT fCur		= (FLOAT)pCurItem->GetDurPoints();
					FLOAT fCurMax	= (FLOAT)pCurItem->GetCurMaxDurPoint();
					FLOAT V			= (FLOAT)pCurItem->GetPrice();
					
					if(fCur != fCurMax)
					{
						//装备耐久上限-装备当前耐久）/装备耐久上限*装备价格
						MoneySpent += (INT) (( fCurMax - fCur )/fCurMax * V  + 0.5);
						if(MoneyHave<MoneySpent)
							break;

						MoneyLast	=	MoneySpent;
						UINT DamagePoint = (UINT)((fCurMax-fCur)*0.1);
						if(DamagePoint == 0)
							DamagePoint = 1;

						HumanItemLogic::SetCurEquipMaxDur(pHuman,(HUMAN_EQUIP)i, (INT)(fCurMax - DamagePoint));
						HumanItemLogic::SetEquipDur(pHuman,(HUMAN_EQUIP)i, (INT)(fCurMax - DamagePoint));
	
						ItemList[RepairedIndex].nCurMaxDur	=  pCurItem->GetCurMaxDurPoint(); 
						ItemList[RepairedIndex].nCurDur		=  pCurItem->GetCurMaxDurPoint(); 
						ItemList[RepairedIndex].IsIn		=	GCShopRepair::EQUIP;					
						ItemList[RepairedIndex++].BagIndex	=	i;
						
						if((INT)fCur == 0)
						{
							freshAtt = TRUE;
						}
					}
				}
			}
			
			ItemContainer*	pHumanContainer = pHuman->GetBaseContain();
			Assert(pHumanContainer);
			INT nContainerSize = pHumanContainer->GetContainerSize();

			for (INT i=0; i<nContainerSize; ++i)
			{
				pCurItem	= pHumanContainer->GetItem(i);
				if(!pCurItem->IsEmpty())
				{//有东西
					if (pCurItem->GetItemClass() != ICLASS_EQUIP)
					{
						continue;
					}
					if(pCurItem->GetMaxDurPoint() == -1)
					{
						continue;
					}
					
					FLOAT fMax		= (FLOAT)pCurItem->GetMaxDurPoint();
					FLOAT fCur		= (FLOAT)pCurItem->GetDurPoints();
					FLOAT fCurMax	= (FLOAT)pCurItem->GetCurMaxDurPoint();
					FLOAT V			= (FLOAT)pCurItem->GetPrice();

					if(fCur != fCurMax)
					{
						//装备耐久上限-装备当前耐久）/装备耐久上限*装备价格
						MoneySpent += (INT) (( fCurMax - fCur )/fCurMax * V  + 0.5);
						if(MoneyHave<MoneySpent)
							break;

						MoneyLast	=	MoneySpent;
						UINT DamagePoint = (UINT)((fCurMax-fCur)*0.1);
						if(DamagePoint == 0)
							DamagePoint = 1;

						HumanItemLogic::SetCurBagEquipMaxDur(pHuman,pHumanContainer->ConIndex2BagIndex(i), (INT)(fCurMax - DamagePoint));
						HumanItemLogic::SetBagEquipDur(pHuman,  pHumanContainer->ConIndex2BagIndex(i), (INT)(fCurMax - DamagePoint));

						ItemList[RepairedIndex].nCurMaxDur	=  pCurItem->GetCurMaxDurPoint(); 
						ItemList[RepairedIndex].nCurDur		=  pCurItem->GetCurMaxDurPoint(); 
						ItemList[RepairedIndex].IsIn		=	GCShopRepair::BAG;					
						ItemList[RepairedIndex++].BagIndex	=	pHumanContainer->ConIndex2BagIndex(i);
					}
				}
			}
			
			for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
			{
				pHumanContainer = pHuman->GetExtraBagContain(i);
				Assert(pHumanContainer);
				if (pHumanContainer->IsValid())
				{
					if (pHumanContainer->IsCanUse())
					{
						pCurItem	= pHumanContainer->GetItem(i);
						if(!pCurItem->IsEmpty())
						{//有东西
							if (pCurItem->GetItemClass() != ICLASS_EQUIP)
							{
								continue;
							}
							if(pCurItem->GetMaxDurPoint() == -1)
							{
								continue;
							}

							FLOAT fMax		= (FLOAT)pCurItem->GetMaxDurPoint();
							FLOAT fCur		= (FLOAT)pCurItem->GetDurPoints();
							FLOAT fCurMax	= (FLOAT)pCurItem->GetCurMaxDurPoint();
							FLOAT V			= (FLOAT)pCurItem->GetPrice();

							if(fCur != fCurMax)
							{
								//装备耐久上限-装备当前耐久）/装备耐久上限*装备价格
								MoneySpent += (INT) (( fCurMax - fCur )/fCurMax * V  + 0.5);
								if(MoneyHave<MoneySpent)
									break;

								MoneyLast	=	MoneySpent;
								UINT DamagePoint = (UINT)((fCurMax-fCur)*0.1);
								if(DamagePoint == 0)
									DamagePoint = 1;

								HumanItemLogic::SetCurBagEquipMaxDur(pHuman,pHumanContainer->ConIndex2BagIndex(i), (INT)(fCurMax - DamagePoint));
								HumanItemLogic::SetBagEquipDur(pHuman,  pHumanContainer->ConIndex2BagIndex(i), (INT)(fCurMax - DamagePoint));

								ItemList[RepairedIndex].nCurMaxDur	=  pCurItem->GetCurMaxDurPoint(); 
								ItemList[RepairedIndex].nCurDur		=  pCurItem->GetCurMaxDurPoint(); 
								ItemList[RepairedIndex].IsIn		=	GCShopRepair::BAG;					
								ItemList[RepairedIndex++].BagIndex	=	pHumanContainer->ConIndex2BagIndex(i);
							}
						}
					}
				}
			}
		
			//钱不够，不修
			if(MoneyHave<MoneySpent)
			{
				GCShopBuy Msg ;
				Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
				pHuman->GetPlayer()->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BUY_MONEY_FAIL") ;
				return PACKET_EXE_CONTINUE ;
			}
			else
			{//全修完了
				pHuman->SetMoney(MoneyHave - MoneySpent);
				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_SHOP_REPAIR;
				MoneyLogParam.Count		=	MoneySpent;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);

				Msg.SetAll(1);
				Msg.SetReListNum(RepairedIndex);
				Msg.SetReList(ItemList);
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
		}
		else
		{
			//修理单个
			pCurItem = HumanItemLogic::GetEquip(pHuman, (HUMAN_EQUIP)BagIndex);
			if(!pCurItem->IsEmpty())
			{
				FLOAT fCur = (FLOAT)pCurItem->GetDurPoints();
				FLOAT fMax = (FLOAT)pCurItem->GetMaxDurPoint();
				FLOAT fCurMax	= (FLOAT)pCurItem->GetCurMaxDurPoint();
				FLOAT V	   = (FLOAT)pCurItem->GetPrice();
				if(fCur != fMax)
				{
					MoneySpent += (INT) (( fCurMax - fCur )/fCurMax * V  + 0.5);
					if(MoneyHave>=MoneySpent)
					{//可以修
						pHuman->SetMoney(MoneyHave - MoneySpent);
						
						MONEY_LOG_PARAM	MoneyLogParam;
						MoneyLogParam.CharGUID	=	pHuman->GetGUID();
						MoneyLogParam.OPType	=	MONEY_SHOP_REPAIR;
						MoneyLogParam.Count		=	MoneySpent;
						MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
						MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
						MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
						SaveMoneyLog(&MoneyLogParam);

						UINT DamagePoint = (UINT)((fCurMax-fCur)*0.1);
						if(DamagePoint == 0)
							DamagePoint = 1;

						HumanItemLogic::SetCurEquipMaxDur(pHuman,(HUMAN_EQUIP)BagIndex, (INT)(fCurMax - DamagePoint));
						HumanItemLogic::SetEquipDur(pHuman,(HUMAN_EQUIP)BagIndex, (INT)(fCurMax - DamagePoint));

						ItemList[0].IsIn		=	GCShopRepair::EQUIP;	
						ItemList[0].nCurMaxDur	=  pCurItem->GetCurMaxDurPoint(); 
						ItemList[0].nCurDur		=  pCurItem->GetCurMaxDurPoint(); 
						ItemList[0].BagIndex	=	BagIndex;
						Msg.SetAll(0);
						Msg.SetReListNum(1);
						Msg.SetReList(ItemList);
						pHuman->GetPlayer()->SendPacket(&Msg);

						if((INT)fCur == 0)
						{
							freshAtt = TRUE;
						}
					}
					else
					{//没钱还捣乱
						GCShopBuy Msg ;
						Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
						pHuman->GetPlayer()->SendPacket( &Msg ) ;
						g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BUY_MONEY_FAIL") ;
						return PACKET_EXE_CONTINUE ;
					}
				}
			}
			else
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BagIndex = %d", BagIndex) ;
				return PACKET_EXE_CONTINUE ;
			}
		}
		if (freshAtt)
		{
			pHuman->ItemEffectFlush();
		}
	}
	else if(Opt == CGShopRepair::FromBag)
	{
		ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman, BagIndex);
		if (pItemContainer == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BagIndex is invalid, BagIndex = %d", BagIndex) ;

			return PACKET_EXE_ERROR;
		}
		if (!pItemContainer->IsCanUse())
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler extrabag is invalid time BagIndex = %d", BagIndex) ;
			return PACKET_EXE_CONTINUE ;
		}

		pCurItem = pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
		
		if(!pCurItem->IsEmpty())
		{//有东西
			FLOAT fCur = (FLOAT)pCurItem->GetDurPoints();
			FLOAT fMax = (FLOAT)pCurItem->GetMaxDurPoint();
			FLOAT fCurMax	= (FLOAT)pCurItem->GetCurMaxDurPoint();
			FLOAT V	   = (FLOAT)pCurItem->GetPrice();

			if(fCur != fMax)
			{
				MoneySpent += (INT) (( fCurMax - fCur )/fCurMax * V  + 0.5);
				if(MoneyHave>=MoneySpent)
				{//可以修
					pHuman->SetMoney(MoneyHave - MoneySpent);
					MONEY_LOG_PARAM	MoneyLogParam;
					MoneyLogParam.CharGUID	=	pHuman->GetGUID();
					MoneyLogParam.OPType	=	MONEY_SHOP_REPAIR;
					MoneyLogParam.Count		=	MoneySpent;
					MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
					MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
					MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
					SaveMoneyLog(&MoneyLogParam);

					UINT DamagePoint = (UINT)((fCurMax-fCur)*0.1);
					if(DamagePoint == 0)
						DamagePoint = 1;

					HumanItemLogic::SetBagItemDur(pHuman,(UINT)BagIndex,(INT)(fCurMax - DamagePoint));
					HumanItemLogic::SetBagItemCurMaxDur(pHuman,(UINT)BagIndex,(INT)(fCurMax - DamagePoint));

					ItemList[0].IsIn		=	GCShopRepair::BAG;					
					ItemList[0].BagIndex	=	BagIndex;
					ItemList[0].nCurMaxDur	=  pCurItem->GetCurMaxDurPoint(); 
					ItemList[0].nCurDur		=  pCurItem->GetCurMaxDurPoint(); 
					Msg.SetAll(0);
					Msg.SetReListNum(1);
					Msg.SetReList(ItemList);
					pHuman->GetPlayer()->SendPacket(&Msg);
				}
				else
				{//没钱还捣乱
					GCShopBuy Msg ;
					Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
					pHuman->GetPlayer()->SendPacket( &Msg ) ;
					g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BUY_MONEY_FAIL") ;
					return PACKET_EXE_CONTINUE ;
				}
			}
		}

	}


	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
