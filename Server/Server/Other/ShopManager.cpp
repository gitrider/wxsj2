#include "stdafx.h"
#include "ShopManager.h"

//public
#include "Obj_Human.h"
#include "Player.h"
#include "Obj_Monster.h"

#include "WXSJ_DBC.h"//DBC
#include "ItemHelper.h"//TSerialHelper
#include "ItemTable.h"//_ITEM_TYPE
#include "TimeManager.h"//g_pTimeManager
#include "GCShopUpdateMerchandiseList.h"//GCShopUpdateMerchandiseList
#include "FileDef.h"
#include "Scene.h"

//globle
using namespace DBC;
StaticShopManager* g_pStaticShopManager =	NULL;

ShopMgr::ShopMgr()
{
	__ENTER_FUNCTION
	m_Count = 0;
	m_Shops = NULL;
	__LEAVE_FUNCTION
}
ShopMgr::~ShopMgr()
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}

VOID	ShopMgr::CleanUp( )
{
	__ENTER_FUNCTION
	SAFE_DELETE_ARRAY(m_Shops)
	__LEAVE_FUNCTION
}

//直接从ItemBoxManager::ConvertItemType2Index抄过来
INT		ShopMgr::ConvertItemType2Money(_ITEM_TYPE it)
{
	__ENTER_FUNCTION

	Assert(it.isNull() == FALSE);

	switch(it.m_Class)
	{
	case ICLASS_EQUIP:
		{
			EQUIP_TB* pGET =	g_ItemTable.GetEquipTB(it.ToSerial());
			Assert(pGET);
			if (!pGET)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[ShopMgr::ConvertItemType2Money] GetEquipTB index is error [%d]",it.ToSerial());
				return -1;
			}
			return pGET->m_BasePrice;
		}
		break;
	case ICLASS_MATERIAL: 
	case ICLASS_COMITEM:
	case ICLASS_IDENT:
		{
			COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(it.ToSerial());
			Assert(pGET);
			if (pGET)
			{
			return pGET->m_nBasePrice;
		}
			else
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[ShopMgr::ConvertItemType2Money] GetCommItemInfoTB index is error [%d]",it.ToSerial());
				return -1;
			}
		}
		break;
	case ICLASS_TASKITEM: 
		{
			TASKITEM_INFO_TB*	pGET		=		g_ItemTable.GetTaskItemInfoTB(it.ToSerial());
			Assert(pGET);
			if (!pGET)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[ShopMgr::ConvertItemType2Money] GetTaskItemInfoTB index is error [%d]",it.ToSerial());
				return -1;
			}
			return pGET->m_nBasePrice;
			return 1;
		}
		break;
	case ICLASS_GEM:
		{
			GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB(it.ToSerial());
			Assert(pGET);
			if (!pGET)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[ShopMgr::ConvertItemType2Money] GetGemInfoTB index is error [%d]",it.ToSerial());
				return -1;
			}
			return pGET->m_nPrice;
		}
		break;
	case ICLASS_STOREMAP:
		{
			STORE_MAP_INFO_TB*	pGET		=		g_ItemTable.GetStoreMapTB(it.ToSerial());
			Assert(pGET);
			if (!pGET)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[ShopMgr::ConvertItemType2Money] GetStoreMapTB index is error [%d]",it.ToSerial());			
				return -1;
			}
			return	pGET->m_nBasePrice;
			break;
		}
	case ICLASS_TALISMAN: 
		Assert(FALSE);
		break;
	case ICLASS_GUILDITEM: 
		Assert(FALSE);
		break;
	case ICLASS_MOUNT:
		{
			MOUNT_INFO_TB*	pGET		=		g_ItemTable.GetMountInfoTB(it.ToSerial());
			Assert(pGET);
			if (!pGET)
				{
				g_pLog->FastSaveLog( LOG_FILE_1, "[ShopMgr::ConvertItemType2Money] GetMountInfoTB index is error [%d]",it.ToSerial());			
				return -1;
				}
			return	pGET->m_nPrice;
			break;
		}
	default:
		Assert(FALSE);
		break;
	}
	Assert(FALSE);

	__LEAVE_FUNCTION
	return -1;
}

_SHOP*	ShopMgr::GetShopByID(INT id)
{
	__ENTER_FUNCTION
	for(INT i = 0; i<m_Count; i++)
	{
		if(m_Shops[i].m_ShopId == id)
		{
			return &m_Shops[i];
		}
	}
	return	NULL;
	__LEAVE_FUNCTION
	return	NULL;
}

INT		ShopMgr::GetShopIndexByID(INT id)
{
	__ENTER_FUNCTION
	for(INT i = 0; i<m_Count; i++)
	{
		if(m_Shops[i].m_ShopId == id)
		{
			return i;
		}
	}
	return	-1;
	__LEAVE_FUNCTION
	return	-1;
}

/*
StaticShopManager
*/
StaticShopManager::~StaticShopManager()
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}
BOOL	StaticShopManager::Init()
{
	return LoadShopsFromFile( FILE_SHOP );
}

VOID	StaticShopManager::CleanUp()
{
	__ENTER_FUNCTION
	SAFE_DELETE_ARRAY(m_Shops)
	__LEAVE_FUNCTION
}

BOOL	StaticShopManager::LoadShopsFromFile( CHAR* filename )
{
	__ENTER_FUNCTION

	UINT d = sizeof(_ITEM);
	DBCFile ShopFile(0);
	BOOL ret = ShopFile.OpenFromTXT(filename);
	if( !ret )
		return FALSE;

	INT iTableCount		= ShopFile.GetRecordsNum();
	INT iTableColumn	= ShopFile.GetFieldsNum();

	m_Count		=	iTableCount;
	m_Shops		=	new _SHOP[m_Count];
	INT itemnum = 0;
	INT i,j,k;
	INT itemTypeSn;
	_ITEM_TYPE	itemType;
	INT			PerItemNum	= 0;
	INT			MaxItemNum	= 0;
	FLOAT		PerRate		= 0.0;

	for(i =0;i<iTableCount;i++)
	{
		m_Shops[i].m_ShopId			= 		ShopFile.Search_Posistion(i,SHOP_ID)->iValue;
		m_Shops[i].m_nRepairLevel	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_LEVEL)->iValue;
		m_Shops[i].m_nBuyLevel		=		ShopFile.Search_Posistion(i,SHOP_BUY_LEVEL)->iValue;
		m_Shops[i].m_nRepairType	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_TYPE)->iValue;
		m_Shops[i].m_nBuyType		=		ShopFile.Search_Posistion(i,SHOP_BUY_TYPE)->iValue;
		m_Shops[i].m_nRepairSpend	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_SPEND)->fValue;
		m_Shops[i].m_nRepairOkProb	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_OKPROB)->fValue;
		m_Shops[i].m_scale			=		ShopFile.Search_Posistion(i,SHOP_SCALE)->fValue;
        m_Shops[i].m_nRate          =       ShopFile.Search_Posistion(i,SHOP_RATE)->iValue;
        m_Shops[i].m_IsDyShop       =       ShopFile.Search_Posistion(i,SHOP_TYPE)->iValue;
		m_Shops[i].m_refreshTime1	=		ShopFile.Search_Posistion(i,SHOP_REFRESH_TIME_1)->iValue;
		m_Shops[i].m_refreshTime2	=		ShopFile.Search_Posistion(i,SHOP_REFRESH_TIME_2)->iValue;
		itemnum						=		ShopFile.Search_Posistion(i,SHOP_ITEM_NUM)->iValue;

		//分析实际有的数据
		INT nNum = 0;
		for(k=0; k<itemnum*SHOP_ITEM_PROPERTY_NUM; k++)
		{
			itemTypeSn	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+k)->iValue;
			if(itemTypeSn == 0)
			{
				break;
			}
			nNum ++;
			k = k+2;
		}
		itemnum = nNum;

		m_Shops[i].m_ItemList		=		new _SHOP::_MERCHANDISE_LIST(itemnum);

		for(j = 0; j<itemnum*SHOP_ITEM_PROPERTY_NUM; j++)
		{
			//Type
			itemTypeSn	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+j)->iValue;
			TSerialHelper help(itemTypeSn);
			itemType	= help.GetItemTypeStruct();

			//GroupNum 0~100
			PerItemNum  = ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+(++j))->iValue;
			if(PerItemNum<0) PerItemNum = 1;
			if(PerItemNum>100) PerItemNum = 100;

			//MaxNum -1代表无限，>0代表有限商品上限，不可以填0,<100
			MaxItemNum	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+(++j))->iValue;
			if(MaxItemNum == 0)	MaxItemNum = -1;
			if(PerItemNum>100) PerItemNum = 100;

			//Rate 0.0~1.0
			PerRate = 1.0;

			//ADD TO STRUCTURE
			m_Shops[i].m_ItemList->AddType(itemType, PerItemNum, MaxItemNum, PerRate);

		}
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

/*
DynamicShopManager
*/
DynamicShopManager::DynamicShopManager(Obj_Monster* pboss)
{
	__ENTER_FUNCTION
	m_pBoss			=	pboss;
	m_Count			=	MAX_SHOP_PER_PERSON;
	m_Shops			=	new _SHOP[m_Count];
	m_aRefeshTimer	=	new CMyTimer[m_Count];
	memset(m_aRefeshTimer,0, m_Count*sizeof(CMyTimer));
	__LEAVE_FUNCTION
}

DynamicShopManager::~DynamicShopManager()
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}

BOOL	DynamicShopManager::Init()
{
	m_nCurrent		=	0;
    return	(m_Shops!= NULL)? TRUE:FALSE;
}

VOID	DynamicShopManager::CleanUp()
{
	SAFE_DELETE_ARRAY(m_Shops)
	SAFE_DELETE_ARRAY(m_aRefeshTimer)
	return;
}

INT		DynamicShopManager::AddDynamicShop(_SHOP* pSource)
{
	__ENTER_FUNCTION

	if(m_nCurrent > MAX_SHOP_PER_PERSON)
		return -1;

	for(INT i = 0;i<m_nCurrent; i++)
	{
		if(m_Shops[i].m_ShopId == pSource->m_ShopId)
		{//表里已经有了
			return -1;
		}
	}

	INT	itemnum;

	_SHOP&	ShopRef = m_Shops[m_nCurrent];

	ShopRef.m_ShopId					= 		pSource->m_ShopId;
	ShopRef.m_scale						=		pSource->m_scale;
	ShopRef.m_refreshTime1				=		pSource->m_refreshTime1;
	ShopRef.m_refreshTime2				=		pSource->m_refreshTime2;
	itemnum								=		pSource->m_ItemList->m_ListCount;
	ShopRef.m_ItemList					=		new _SHOP::_MERCHANDISE_LIST(DY_SHOP_ITEM_MAX);
	ShopRef.m_ItemList->m_nCurrent		=		pSource->m_ItemList->m_nCurrent;
    ShopRef.m_nRate                     =       pSource->m_nRate;
	ShopRef.m_ItemList->m_ListCount		=		DY_SHOP_ITEM_MAX;
	ShopRef.m_IsDyShop					=		TRUE;

	//启动计时器  间隔时间刷新
	if(pSource->m_refreshTime2 >0)
		m_aRefeshTimer[m_nCurrent].BeginTimer(pSource->m_refreshTime2, g_pTimeManager->CurrentTime());

	return m_nCurrent++;
	__LEAVE_FUNCTION
	return -1;
}

BOOL	DynamicShopManager::Tick(UINT uTime)
{
	__ENTER_FUNCTION

	if(!m_pBoss)
		return FALSE;
	Scene* pCurScene = m_pBoss->getScene();
    INT GoodsCount = 0;

	for(INT i = 0; i< m_nCurrent; i++)
	{
        //动态商店
        if (m_Shops[i].m_IsDyShop)
        {
            if (!m_Shops[i].m_bInit)//服务器启动时初始化
            {
                Fresh(pCurScene, &m_Shops[i]);
                m_Shops[i].m_bInit = TRUE;
                return TRUE;
            }
            
            BOOL bRefesh = FALSE;
            //间隔时间刷新
            if (m_Shops[i].m_refreshTime2 > 0)
            {
                if(m_aRefeshTimer[i].CountingTimer(uTime))
                {
                    bRefesh = TRUE;
                }
            }//定时刷新  10406   10点 40分 星期六【0-6】 . 7为每天
            else if (m_Shops[i].m_refreshTime1 != 0 && !m_Shops[i].m_bRefreshTime2)
            {
                BYTE wday   =   m_Shops[i].m_refreshTime1%10;
                BYTE hour   =   (m_Shops[i].m_refreshTime1/1000);
                BYTE min    =   (m_Shops[i].m_refreshTime1%1000)/10;

                time_t time = g_pTimeManager->GetANSITime();
                tm* ptm = localtime(&time);
                if (ptm->tm_wday == wday || wday >=7 )
                {
                    if (ptm->tm_hour >= hour)
                    {
                        if (ptm->tm_min >= min)
                        {
                            bRefesh = TRUE;
                            m_Shops[i].m_bRefreshTime2 = TRUE;
                        }
                    }
                }
            }
            
            if (bRefesh)
            {
                Fresh(pCurScene, &m_Shops[i]);
            }
        }
	}
	return	TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID    DynamicShopManager::Fresh(Scene *pScence, _SHOP *pShop)
{
__ENTER_FUNCTION

    INT GoodsCount = 0;
    INT nShopID = pShop->m_ShopId -1;
    ++pShop->m_nVersion;

    Assert(nShopID >= 0 && nShopID < MAX_DYSHOP_GROUP);

    //清空前一次的商品
    for(INT i = 0;i<pShop->m_ItemList->m_ListCount;i++)
    {
        pShop->m_ItemList->m_ListTypeIndex[i]   = 0;
        pShop->m_ItemList->m_TypeMaxNum[i]      = 0;
        pShop->m_ItemList->m_Price[i]           = 0;
        pShop->m_ItemList->m_PriceType[i]       = 0;
    }

    //先做组随机
    for (INT j = 0; j< g_ShopInfoTbl.m_aShop[nShopID].m_ItemList->m_ListCount;j++)
    {
        INT nRand = pScence->GetRand100();
        //该组物品出现
        if (nRand <= g_ShopInfoTbl.m_aShop[nShopID].m_nRate)
        {
            //静态商店m_ListTypeIndex为ITEMID，动态商店则表示组ID，对应DYSHOPtab
            INT nGroupID = g_ShopInfoTbl.m_aShop[nShopID].m_ItemList->m_ListTypeIndex[j];

            //根据nGroupID去g_DyShopInfoTbl中查找具体的商品信息
            Assert(nGroupID < MAX_DYSHOP_GROUP && (nGroupID-1) >= 0);
            INT nGoodsNum = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_GoodsNum;

            for (INT k=0; k<nGoodsNum; ++k)
            {
                INT nRand2 = pScence->GetRand100();

                //组内该商品出现
                if (nRand2 < g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_Rate)
                {
                    INT nRand3 = pScence->GetRand100();
                    INT nPriceNUm = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_PriceNum;
                    //随机价格
                    switch (g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_RandPriceType)
                    {
                    case 0://随机
                        {
                            nRand3 %= nPriceNUm;
                            //这里出售价格由策划填表，不用ConvertItemType2Money换算m_ListType
                            pShop->m_ItemList->m_Price[GoodsCount]       = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_Price[nRand3];
                            pShop->m_ItemList->m_PriceType[GoodsCount]   = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_PriceType[nRand3];
                        }
                        break;
                    case 1://递增，逐个调用后面的价格
                        {
                            for (INT m=0; m<nPriceNUm; ++m)
                            {
                                if (pShop->m_ItemList->m_Price[GoodsCount]      == g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_Price[m] &&
                                    pShop->m_ItemList->m_PriceType[GoodsCount]  == g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_PriceType[m])
                                {
                                    INT n = (m+1)%nPriceNUm;
                                    pShop->m_ItemList->m_Price[GoodsCount]      = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_Price[n];
                                    pShop->m_ItemList->m_PriceType[GoodsCount]  = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_PriceType[n];
                                    break;
                                }
                            }

                            //都不相同  这里第一次初始化
                            pShop->m_ItemList->m_Price[GoodsCount]      = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_Price[0];
                            pShop->m_ItemList->m_PriceType[GoodsCount]  = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_PriceType[0];
                        }
                        break;
                    }

                    //ItemID
                    pShop->m_ItemList->m_ListTypeIndex[GoodsCount]              = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_ItemID;
                    //可售数量随机
                    nRand3 = pScence->GetRand100();
                    nRand3 %= g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_ItemNumMax; 
                    if (nRand3 <= g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_ItemNumMin)
                    {
                        nRand3 = g_DyShopInfoTbl.m_aGroupInfo[nGroupID-1].m_Goods[k].m_ItemNumMin;
                    }
                    pShop->m_ItemList->m_TypeMaxNum[GoodsCount]  = nRand3;
                    ++GoodsCount;

                    // 防止写越界
                    if (GoodsCount >= pShop->m_ItemList->m_ListCount)
                        break;
                }
            }
        }
    }


__LEAVE_FUNCTION
}
