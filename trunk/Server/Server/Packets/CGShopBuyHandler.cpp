// CGBuyHandler.cpp

#include "stdafx.h"
#include "CGShopBuy.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCNotifyEquip.h"
#include "Log.h"
#include "GCShopBuy.h"
#include "ShopManager.h"
#include "GCShopUpdateMerchandiseList.h"
#include "GCShopSoldList.h"
#include "HumanItemLogic.h"
#include "GCShopMerchandiseList.h"

uint CGShopBuyHandler::Execute( CGShopBuy* pPacket, Player* pPlayer )
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

	BYTE	m_nndex		=	pPacket->GetIndex();	//资源位置索引
	UINT	UniqueID	=	pPacket->GetUniqueID();
	
	SceneID_t	SceneID =	UniqueID>>16;
	ObjID_t		NpcObjID=	UniqueID&0x00ff;

    //距离判定
	Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcObjID));
	if(pNpcObj == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler Illegal Obj") ;
		return PACKET_EXE_CONTINUE ; 
	}

	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

	if(fDist>MAX_NPC_DISTANCE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler Out Of Range") ;
		return PACKET_EXE_CONTINUE ; 
	}

	if(pHuman->Shop( )	== NULL)
	{//没有打开交易窗口就买
		Assert(0);
		return PACKET_EXE_CONTINUE ;
	}

	//打开商店时，这个商店的指针已经被存在human身上了，关闭商店时它会被清空
	_SHOP*		pShop	=	pHuman->Shop( )	;
	INT		itemNumber	=	0;

	//暂时规定200以上为回购的商品栏
	if(m_nndex >= 200)
	{//在回购队列中查找
        m_nndex -= 200;
		_ITEM tempItem;
		UINT  uPrice = 0;
		if(!pHuman->GetFromSoldListByIndex(m_nndex, tempItem, uPrice))
		{
			//Assert(0);
			return PACKET_EXE_CONTINUE ;
		}

		itemNumber = tempItem.GetItemCount();
		INT iPrice = (INT)uPrice;

		if( ((INT)pHuman->GetMoney()) < iPrice )
		{//金钱不够
			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
			pHuman->GetPlayer()->SendPacket( &Msg ) ;
			return PACKET_EXE_CONTINUE ;
		}
		
		if(!pHuman->RecieveOneFromSoldList(m_nndex))
		{
			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_BAG_FULL);
			pHuman->GetPlayer()->SendPacket( &Msg ) ;
			return PACKET_EXE_CONTINUE ;
		}
		else
		{//从回购列表里面购买成功	
			pHuman->SetMoney( pHuman->GetMoney() - iPrice);
			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_SHOP_COST;
			MoneyLogParam.Count		=	iPrice;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);


			GCShopBuy Msg ;
			Msg.SetBuyOk( (BYTE)GCShopBuy::BUY_OK );
			Msg.SetIndex( tempItem.m_ItemIndex );
			Msg.SetNum( tempItem.GetItemCount() );
			pHuman->GetPlayer()->SendPacket( &Msg ) ;

			//更新回购按钮
            //刷新整个回购列表商品到界面
            GCShopSoldList::_MERCHANDISE_ITEM	SoldItem[MAX_BOOTH_SOLD_NUMBER];

			// 20100413 AddCodeBegin
			pHuman->ReorderSoldList();	// 重新整理回购物品列表，去除中间的空物品格
			// 20100413 AddCodeBegin

            pHuman->GetSlodListInfo(&SoldItem[0]);
            GCShopSoldList	MsgSold;
            MsgSold.SetMerchadiseNum(MAX_BOOTH_SOLD_NUMBER);
            MsgSold.SetMerchadiseList(&SoldItem[0]);
            pHuman->GetPlayer()->SendPacket( &MsgSold );
		}
	}
	else
	{//在商人挂的商店里查找

		//一次点击可以买的个数  0为约定值，为默认购买数量以商店配置文件为准
        INT itemNumber;
        if (pPacket->GetCount() == 0)
        {
            itemNumber = 1; 
        }
        else
        {
            itemNumber = pPacket->GetCount();
        }

		if(	pShop->m_IsDyShop )
		{
            //校验商店版本，不匹配提示客户端刷新贩卖列表
            if (pShop->m_nVersion != pPacket->GetVersion())
            {
                GCShopBuy Msg ;
                Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MUST_FRESH);
                pPlayer->SendPacket( &Msg ) ;

                //填充消息
                GCShopUpdateMerchandiseList::_MERCHANDISE_ITEM	MerchandiseList[MAX_BOOTH_NUMBER];
                INT k = 0;
                for(INT i = 0;i<pShop->m_ItemList->m_ListCount;i++)
                {
                    MerchandiseList[k].idTable		=	pShop->m_ItemList->m_ListTypeIndex[i];
                    MerchandiseList[k].MaxNumber	=	pShop->m_ItemList->m_TypeMaxNum[i];

                    MerchandiseList[k].nPrice		=	pShop->m_ItemList->m_Price[i];
                    MerchandiseList[k].nPriceType	=	pShop->m_ItemList->m_PriceType[i];
                    MerchandiseList[k++].byNumber	=	pShop->m_ItemList->m_TypeMaxNum[i];
                }

                GCShopUpdateMerchandiseList	listMsg;
                listMsg.SetMerchadiseNum(k);//一定要先设置数量再设置List
                listMsg.SetMerchadiseList(MerchandiseList);
                listMsg.SetVersion(pShop->m_nVersion);
                pHuman->GetPlayer()->SendPacket( &listMsg ) ;

                return PACKET_EXE_CONTINUE ;
            }

            //有限商品不够卖了
            if(pShop->m_ItemList->m_TypeMaxNum[m_nndex] < itemNumber)
            {//卖完了，直接返回，
                //应该不会走到这里，
                //首先表里不应该出现0,
                //其次卖完的时候服务器会通知所有连在他之上的客户端
                //在界面上删掉这个商品，这样玩家也不可能再点这个商品了

                GCShopBuy Msg ;
                Msg.SetBuyOk((BYTE)GCShopBuy::BUY_NO_MERCH);
                pPlayer->SendPacket( &Msg ) ;
                return PACKET_EXE_CONTINUE ;
            }
		}

        uint BaseMoney= 0;
        //静态商店，只扣游戏币
        if (!pShop->m_IsDyShop)
        {
            //计算价格
            BaseMoney = ShopMgr::ConvertItemType2Money(pShop->m_ItemList->m_ListType[m_nndex]);
            if(BaseMoney<0) BaseMoney = 0;
            BaseMoney *= static_cast<INT>(pShop->m_scale);
            BaseMoney *= itemNumber;
            if( pHuman->GetMoney() < BaseMoney )
            {//金钱不够
                GCShopBuy Msg ;
                Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
                pHuman->GetPlayer()->SendPacket( &Msg ) ;
                return PACKET_EXE_CONTINUE ;
            }
        }
        else//动态商店
        {
            INT nPriceType  =   pShop->m_ItemList->m_PriceType[m_nndex];
            INT nPrice      =   pShop->m_ItemList->m_Price[m_nndex];
            switch (nPriceType)
            {
            case PRICE_GOLD:
                {
                    INT nGold = pHuman->GetMoney();
                    nGold    -= nPrice*itemNumber;
                    if (nGold < 0)//这里只管防止发生错误，为什么就不管了
                    {
                        nGold = 0;
                        GCShopBuy Msg ;
                        Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
                        pHuman->GetPlayer()->SendPacket( &Msg ) ;
                        return PACKET_EXE_CONTINUE ;
                    }
                    pHuman->SetMoney(nGold);
                }
                break;
            case PRICE_RMB:
                break;
            case PRICE_GP://帮贡
                {
                    INT nTotal  = pHuman->GetHonor() - nPrice*itemNumber;
                    if (nTotal < 0)
                    {
                        nTotal = 0;
                        GCShopBuy Msg ;
                        Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
                        pHuman->GetPlayer()->SendPacket( &Msg ) ;
                        return PACKET_EXE_CONTINUE ;
                    }

                    pHuman->SetHonor(nTotal);
                }
                break;
            }
        }
        
		
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_SHOP;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
		ItemLogParam.ShopGUID	= pShop->m_ShopId;


		if(!HumanItemLogic::CreateMultiItemToBag(&ItemLogParam,
            pHuman,
            pShop->m_ItemList->m_ListTypeIndex[m_nndex],
            itemNumber))
        {
			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_BAG_FULL);
			pHuman->GetPlayer()->SendPacket( &Msg ) ;
			return PACKET_EXE_CONTINUE ;
		}
		else
		{	
			SaveItemLog(&ItemLogParam);

            if (!pShop->m_IsDyShop)
            {
                pHuman->SetMoney( pHuman->GetMoney() - BaseMoney);

                MONEY_LOG_PARAM	MoneyLogParam;
                MoneyLogParam.CharGUID	=	pHuman->GetGUID();
                MoneyLogParam.OPType	=	MONEY_SHOP_COST;
                MoneyLogParam.Count		=	BaseMoney;
                MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
                MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
                MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
                SaveMoneyLog(&MoneyLogParam);
            }

			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_OK);
			Msg.SetIndex(pShop->m_ItemList->m_ListTypeIndex[m_nndex]);
			Msg.SetNum(itemNumber);
			pHuman->GetPlayer()->SendPacket( &Msg ) ;

			//要更新此商人的动态商品表
			if(pShop->m_IsDyShop)
			{
                // 扣除有限物品
                pShop->m_ItemList->m_TypeMaxNum[m_nndex] -= itemNumber;


                //填充消息
                GCShopUpdateMerchandiseList::_MERCHANDISE_ITEM	MerchandiseList[MAX_BOOTH_NUMBER];
                INT k = 0;
                for(INT i = 0;i<pShop->m_ItemList->m_ListCount;i++)
                {
                    MerchandiseList[k].idTable		=	pShop->m_ItemList->m_ListTypeIndex[i];
                    MerchandiseList[k].MaxNumber	=	pShop->m_ItemList->m_TypeMaxNum[i];

                    MerchandiseList[k].nPrice		=	pShop->m_ItemList->m_Price[i];
                    MerchandiseList[k].nPriceType	=	pShop->m_ItemList->m_PriceType[i];
                    MerchandiseList[k++].byNumber	=	pShop->m_ItemList->m_TypeMaxNum[i];
                }

                GCShopUpdateMerchandiseList	Msg;
                Msg.SetMerchadiseNum(k);//一定要先设置数量再设置List
                Msg.SetMerchadiseList(MerchandiseList);
                Msg.SetVersion(pShop->m_nVersion);
                pHuman->GetPlayer()->SendPacket( &Msg ) ;
			}
			return PACKET_EXE_CONTINUE ;
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler m_nndex=%d",
		m_nndex) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
