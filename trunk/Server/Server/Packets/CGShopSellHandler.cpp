// CGShopSellHandler.cpp

#include "stdafx.h"
#include "CGShopSell.h"
#include "GCShopSell.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Log.h"
#include "ShopManager.h"
#include "GCShopSoldList.h"
#include "HumanItemLogic.h"

uint CGShopSellHandler::Execute( CGShopSell* pPacket, Player* pPlayer )
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

	BYTE	m_nBagIndex	=	pPacket->GetBagIndex();		//���е�λ��
	UINT	UniqueID	=	pPacket->GetUniqueID();

	SceneID_t	SceneID =	UniqueID>>16;
	ObjID_t		NpcObjID=	UniqueID&0x00ff;

    //ȡ���������ж�
	//if(SceneID != pScene->SceneID())
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSellHandler Illegal scene ObjName = %s", pHuman->GetName()) ;
	//	return PACKET_EXE_CONTINUE ; 
	//}

	//�����ж�
	Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcObjID));
	if(pNpcObj == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSellHandler Illegal Obj ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

	if(fDist>MAX_NPC_DISTANCE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSellHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	Item*		 pCurrentItem = HumanItemLogic::GetBagItem(pHuman,m_nBagIndex);

	if(!pCurrentItem||ConvertSerial2ItemType(pCurrentItem->GetItemTableIndex()).isNull())
	{
		//��������������
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGShopSellHandler m_nBagIndex=%d PlayerName=s%",
			m_nBagIndex,  pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE;
	}

	//�ж���������ǲ����չ������Ʒ
	_SHOP*		pShop	=	pHuman->Shop( )	;

	//�̵겻����
	if(!pShop)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGShopSellHandler Npc Shop Lose");
		return PACKET_EXE_CONTINUE;
	}

    if(pShop->m_IsDyShop)
    {
        g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGShopSellHandler DynamicShop can't Sell");
        return PACKET_EXE_CONTINUE;
    }

	//��ѯ�չ��ȼ�����
	if( pCurrentItem->GetItemClass() == ICLASS_EQUIP )
	{
		if( pCurrentItem->GetRequireLevel() > pShop->m_nBuyLevel )
		{
			return PACKET_EXE_CONTINUE;
		}
	}
	else
	{
		if( pCurrentItem->GetItemLevel() > pShop->m_nBuyLevel )
		{
			return PACKET_EXE_CONTINUE;
		}
	}

	BOOL bCanBuy = TRUE;
	if(pShop->m_nBuyType != SHOP_All)
	{
		switch(pShop->m_nBuyType)
		{
		case SHOP_All:
			{
			}
			break;
		case SHOP_DEFENCE:		//����
			{
				if( ICLASS_EQUIP == pCurrentItem->GetItemClass())
				{
					/*if( pCurrentItem->GetEquipPoint() != HEQUIP_CAP &&
						pCurrentItem->GetEquipPoint() != HEQUIP_ARMOR &&
						pCurrentItem->GetEquipPoint() != HEQUIP_CUFF &&
						pCurrentItem->GetEquipPoint() != HEQUIP_BOOT )
					{
						bCanBuy = FALSE;
					}*/
				}
				else
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_ADORN:		//����
			{
				if( ICLASS_EQUIP == pCurrentItem->GetItemClass())
				{
					/*if( pCurrentItem->GetEquipPoint() != HEQUIP_SASH &&
						pCurrentItem->GetEquipPoint() != HEQUIP_RING &&
						pCurrentItem->GetEquipPoint() != HEQUIP_NECKLACE &&
						pCurrentItem->GetEquipPoint() != HEQUIP_RIDER)
					{
						bCanBuy = FALSE;
					}*/
				}
				else
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_WEAPON:		//����
			{
				if( ICLASS_EQUIP == pCurrentItem->GetItemClass())
				{
					/*if( pCurrentItem->GetEquipPoint() != HEQUIP_WEAPON )
					{
						bCanBuy = FALSE;
					}*/
				}
				else
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_FOOD:			//ʳ��
			{
				if( ICLASS_COMITEM != pCurrentItem->GetItemClass())
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_MATERIAL:		//����
			{
				if( ICLASS_MATERIAL != pCurrentItem->GetItemClass())
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_COMITEM:		//ҩƷ
			{
				if( ICLASS_COMITEM != pCurrentItem->GetItemClass())
				{
					bCanBuy = FALSE;
				}
			}
			break;
		default:
			{
				bCanBuy = FALSE;
				break;
			}
		}
	}
	
	//�Ƿ񷵻�ʧ����Ϣ������
	if(bCanBuy == FALSE)
	{
		return PACKET_EXE_CONTINUE;
	}

	INT   iPrice = pCurrentItem->GetSellPrice();

	//2006-4-21 
	//������̵�����Ѿ�����������Ʒ���������������Ʒ���ջ����۸�*ϵ��A(=1/3) ���㣻
	//������δ������Ʒ���۸�Ӧ�ø��ͣ���˼۸�ϵ��ΪB(=1/10)������д��ڵ���������config�п��ŵ���A,B�Ľӿڡ�
	//�;�˥��ϵ��D=��ǰ�;�/����;�
	//����ʧ��˥������F=1/������ʧ�ܴ���+1��
	//������յĳ��ۼ۸�ΪV=�����۸�B*A*D*F
	//�ٳ����̵�ü۸����

	FLOAT fCur			= 1;
	FLOAT fMax			= 1;

	if(pCurrentItem->GetItemClass() == ICLASS_EQUIP)
	{
		fCur		= (FLOAT)pCurrentItem->GetDurPoints();
		fMax		= (FLOAT)pCurrentItem->GetMaxDurPoint();
	}

	//����һ�£�һ����ŵ������б���
	_ITEM tempitem;
	pCurrentItem->SaveValueTo(&tempitem);
    INT nNum  = pCurrentItem->GetLayedNum();

	//���ݵ��ع��б���,ֻ��¼�����ĵ���
    if (0 == pPacket->GetCount())//�Ҽ�������������������
    {
        pHuman->AddToSoldList(m_nBagIndex, iPrice, pCurrentItem->GetLayedNum());
		iPrice *= nNum; // 20100409 AddCode 
    }
    else
    {
        pHuman->AddToSoldList(m_nBagIndex, iPrice, pPacket->GetCount());
		iPrice *= pPacket->GetCount(); // 20100409 AddCode 
    }

	//��Ǯ    ����*����
	//iPrice *= nNum; // 20100409 DeleteCode ���ѡ��������Ʒ�ĸ����󣬷������ǰ��ո���Ʒ���ܸ��������ģ�Ӧ��Ϊ�������ѡ���ĸ�������
	pHuman->SetMoney( pHuman->GetMoney() + iPrice);

	MONEY_LOG_PARAM	MoneyLogParam;
	MoneyLogParam.CharGUID	=	pHuman->GetGUID();
	MoneyLogParam.OPType	=	MONEY_SHOP_SELL;
	MoneyLogParam.Count		=	iPrice;
	MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
	MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	SaveMoneyLog(&MoneyLogParam);
		
	//ˢ�������ع��б���Ʒ������
	GCShopSoldList::_MERCHANDISE_ITEM	SoldItem[MAX_BOOTH_SOLD_NUMBER];
    pHuman->GetSlodListInfo(&SoldItem[0]);
	GCShopSoldList	MsgSold;
	MsgSold.SetMerchadiseNum(MAX_BOOTH_SOLD_NUMBER);
	MsgSold.SetMerchadiseList(&SoldItem[0]);
	pHuman->GetPlayer()->SendPacket( &MsgSold );

	GCShopSell	MsgSell;
	pHuman->GetPlayer()->SendPacket( &MsgSell );
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSellHandler m_nBagIndex=%d ",
		m_nBagIndex  ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
