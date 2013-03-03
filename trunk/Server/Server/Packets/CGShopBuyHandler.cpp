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
	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	BYTE	m_nndex		=	pPacket->GetIndex();	//��Դλ������
	UINT	UniqueID	=	pPacket->GetUniqueID();
	
	SceneID_t	SceneID =	UniqueID>>16;
	ObjID_t		NpcObjID=	UniqueID&0x00ff;

    //�����ж�
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
	{//û�д򿪽��״��ھ���
		Assert(0);
		return PACKET_EXE_CONTINUE ;
	}

	//���̵�ʱ������̵��ָ���Ѿ�������human�����ˣ��ر��̵�ʱ���ᱻ���
	_SHOP*		pShop	=	pHuman->Shop( )	;
	INT		itemNumber	=	0;

	//��ʱ�涨200����Ϊ�ع�����Ʒ��
	if(m_nndex >= 200)
	{//�ڻع������в���
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
		{//��Ǯ����
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
		{//�ӻع��б����湺��ɹ�	
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

			//���»ع���ť
            //ˢ�������ع��б���Ʒ������
            GCShopSoldList::_MERCHANDISE_ITEM	SoldItem[MAX_BOOTH_SOLD_NUMBER];

			// 20100413 AddCodeBegin
			pHuman->ReorderSoldList();	// ��������ع���Ʒ�б�ȥ���м�Ŀ���Ʒ��
			// 20100413 AddCodeBegin

            pHuman->GetSlodListInfo(&SoldItem[0]);
            GCShopSoldList	MsgSold;
            MsgSold.SetMerchadiseNum(MAX_BOOTH_SOLD_NUMBER);
            MsgSold.SetMerchadiseList(&SoldItem[0]);
            pHuman->GetPlayer()->SendPacket( &MsgSold );
		}
	}
	else
	{//�����˹ҵ��̵������

		//һ�ε��������ĸ���  0ΪԼ��ֵ��ΪĬ�Ϲ����������̵������ļ�Ϊ׼
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
            //У���̵�汾����ƥ����ʾ�ͻ���ˢ�·����б�
            if (pShop->m_nVersion != pPacket->GetVersion())
            {
                GCShopBuy Msg ;
                Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MUST_FRESH);
                pPlayer->SendPacket( &Msg ) ;

                //�����Ϣ
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
                listMsg.SetMerchadiseNum(k);//һ��Ҫ����������������List
                listMsg.SetMerchadiseList(MerchandiseList);
                listMsg.SetVersion(pShop->m_nVersion);
                pHuman->GetPlayer()->SendPacket( &listMsg ) ;

                return PACKET_EXE_CONTINUE ;
            }

            //������Ʒ��������
            if(pShop->m_ItemList->m_TypeMaxNum[m_nndex] < itemNumber)
            {//�����ˣ�ֱ�ӷ��أ�
                //Ӧ�ò����ߵ����
                //���ȱ��ﲻӦ�ó���0,
                //��������ʱ���������֪ͨ����������֮�ϵĿͻ���
                //�ڽ�����ɾ�������Ʒ���������Ҳ�������ٵ������Ʒ��

                GCShopBuy Msg ;
                Msg.SetBuyOk((BYTE)GCShopBuy::BUY_NO_MERCH);
                pPlayer->SendPacket( &Msg ) ;
                return PACKET_EXE_CONTINUE ;
            }
		}

        uint BaseMoney= 0;
        //��̬�̵ֻ꣬����Ϸ��
        if (!pShop->m_IsDyShop)
        {
            //����۸�
            BaseMoney = ShopMgr::ConvertItemType2Money(pShop->m_ItemList->m_ListType[m_nndex]);
            if(BaseMoney<0) BaseMoney = 0;
            BaseMoney *= static_cast<INT>(pShop->m_scale);
            BaseMoney *= itemNumber;
            if( pHuman->GetMoney() < BaseMoney )
            {//��Ǯ����
                GCShopBuy Msg ;
                Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
                pHuman->GetPlayer()->SendPacket( &Msg ) ;
                return PACKET_EXE_CONTINUE ;
            }
        }
        else//��̬�̵�
        {
            INT nPriceType  =   pShop->m_ItemList->m_PriceType[m_nndex];
            INT nPrice      =   pShop->m_ItemList->m_Price[m_nndex];
            switch (nPriceType)
            {
            case PRICE_GOLD:
                {
                    INT nGold = pHuman->GetMoney();
                    nGold    -= nPrice*itemNumber;
                    if (nGold < 0)//����ֻ�ܷ�ֹ��������Ϊʲô�Ͳ�����
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
            case PRICE_GP://�ﹱ
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

			//Ҫ���´����˵Ķ�̬��Ʒ��
			if(pShop->m_IsDyShop)
			{
                // �۳�������Ʒ
                pShop->m_ItemList->m_TypeMaxNum[m_nndex] -= itemNumber;


                //�����Ϣ
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
                Msg.SetMerchadiseNum(k);//һ��Ҫ����������������List
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
