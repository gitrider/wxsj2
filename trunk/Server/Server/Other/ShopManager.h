#ifndef _SHOP_MANAGER_H_
#define _SHOP_MANAGER_H_
#include "Type.h"
#include "ItemManager.h"
#include "ItemBoxManager.h"

#define MAX_SHOP_PER_PERSON				10

//macro
#define	SHOP_ITEM_PROPERTY_NUM			3
#define	SHOP_ID							0
#define	SHOP_REPAIR_LEVEL				2
#define	SHOP_BUY_LEVEL					3
#define	SHOP_REPAIR_TYPE				4
#define	SHOP_REPAIR_SPEND				5
#define	SHOP_BUY_TYPE					6
#define	SHOP_REPAIR_OKPROB				7
#define	SHOP_SCALE						8
#define	SHOP_RATE                       9
#define	SHOP_TYPE                       10
#define	SHOP_REFRESH_TIME_1				11
#define	SHOP_REFRESH_TIME_2				12
#define	SHOP_ITEM_NUM					13
#define	SHOP_ITEM_PROPERTY_BEGIN		14
#define	DY_SHOP_ITEM_MAX        		30

#define	NEW_AND_COPY_ARRAY(PDEST, PSOURCE, NUM, TYPE)\
    PDEST = new TYPE[NUM];\
    memcpy((CHAR*)PDEST, (CHAR*)PSOURCE, NUM*sizeof(TYPE));\
//#define MAX_SOLDOUT_ITEM_PER_PERSON		5

class Obj_Human;
class Obj_Monster;

/*
_SHOP,一个商店
*/
struct _SHOP
{
	/*
	_MERCHANDISE_LIST,商店中的商品列表
	*/
	struct _MERCHANDISE_LIST
	{
		BYTE			m_ListCount;
		_ITEM_TYPE*		m_ListType;
        INT*			m_Price;
        INT*			m_PriceType;
		INT*			m_ListTypeIndex;
		INT*			m_TypeCount;
		INT*			m_TypeMaxNum;
		FLOAT*			m_AppearRate;
		INT				m_nCurrent;

		//
		_MERCHANDISE_LIST() 
		{
			m_ListCount = 0;
			m_nCurrent  = 0;
            m_Price     = NULL;
            m_PriceType = NULL;
			m_ListType	= NULL;
			m_ListTypeIndex = NULL;
			m_TypeCount = NULL;
			m_AppearRate = NULL;
		};
		//
		_MERCHANDISE_LIST(BYTE num)
		{
			m_ListCount		= num;
			m_nCurrent		= 0;
            m_Price         = new INT[m_ListCount];
            m_PriceType     = new INT[m_ListCount];
			m_ListType		= new _ITEM_TYPE[m_ListCount];
			m_ListTypeIndex	= new INT[m_ListCount];
			m_TypeCount		= new INT[m_ListCount];
			m_TypeMaxNum	= new INT[m_ListCount];
			m_AppearRate	= new FLOAT[m_ListCount];

            memset(m_Price, 0, sizeof(INT)*m_ListCount);
            memset(m_PriceType, 0, sizeof(INT)*m_ListCount);
            memset(m_ListTypeIndex, 0, sizeof(INT)*m_ListCount);
            memset(m_TypeCount, 0, sizeof(INT)*m_ListCount);
            memset(m_TypeMaxNum, 0, sizeof(INT)*m_ListCount);
            memset(m_AppearRate, 0, sizeof(FLOAT)*m_ListCount);
		}
		~_MERCHANDISE_LIST()
		{
			SAFE_DELETE_ARRAY(m_ListType)
			SAFE_DELETE_ARRAY(m_ListTypeIndex)
			SAFE_DELETE_ARRAY(m_TypeCount)
			SAFE_DELETE_ARRAY(m_TypeMaxNum)
			SAFE_DELETE_ARRAY(m_AppearRate)
            SAFE_DELETE_ARRAY(m_Price)
            SAFE_DELETE_ARRAY(m_PriceType)
		}

		//
		VOID	AddType(_ITEM_TYPE it, INT Count, INT MaxCount, FLOAT Rate)
		{
			Assert(!it.isNull());
			Assert(m_nCurrent<m_ListCount);
			m_ListType[m_nCurrent]		=	it;
			m_ListTypeIndex[m_nCurrent]	=	it.ToSerial();
			m_TypeCount[m_nCurrent]		=	Count;
			m_TypeMaxNum[m_nCurrent]	=	MaxCount;
			m_AppearRate[m_nCurrent++]	=	Rate;
		}
	};

	_SHOP()
	{
		m_ShopId        =   -1;
		m_nRepairLevel  =   0;	
		m_nBuyLevel     =   0;	
		m_nRepairType   =   -1;	
		m_nBuyType      =   -1;		
		m_nRepairSpend  =   0;	
		m_nRepairOkProb =   0;
		m_scale         =   0.0f;
		m_ItemList      =   NULL;
		m_refreshTime1  =   0;
		m_refreshTime2  =   0;
		m_IsDyShop      =   FALSE;
		m_nCustomNum    =   0;
        m_nRate         =   0;
        m_bRefreshTime2 =   FALSE;
        m_nVersion      =   0;
        m_bInit         =  FALSE;
	}
	_SHOP(INT itemNum)
	{
		m_ShopId        =   -1;
		m_nRepairLevel  =   0;	
		m_nBuyLevel     =   0;	
		m_nRepairType   =   -1;	
		m_nBuyType      =   -1;		
		m_nRepairSpend  =   0;	
		m_nRepairOkProb =   0;
		m_scale         =   0.0f;
		m_ItemList      =   new _MERCHANDISE_LIST(itemNum);
        m_refreshTime1  =   0;
        m_refreshTime2  =   0;
		m_IsDyShop      =   FALSE;
		m_nCustomNum    =   0;
        m_nRate         =   0;
        m_bRefreshTime2 = FALSE;
        m_nVersion      =   0;
        m_bInit         =  FALSE;
	}


	~_SHOP()
	{
		SAFE_DELETE(m_ItemList)
	}

	VOID				Custom(Obj_Human* pC) { m_nCustomNum = (pC == NULL)? (m_nCustomNum-1):(m_nCustomNum+1);}
	INT					Custom(VOID) {return (m_nCustomNum<0)?0:m_nCustomNum;}

	//
	INT					m_ShopId;

	INT					m_nRepairLevel;		// 修理等级
	INT					m_nBuyLevel;		// 收购等级
	INT					m_nRepairType;		// 修理类型
	INT					m_nBuyType;			// 商店的收购类型
	FLOAT				m_nRepairSpend;		// 修理花费
	FLOAT				m_nRepairOkProb;	// 修理成功几率

	FLOAT				m_scale;
	INT					m_refreshTime1;     // 定时刷新，格式 (星期-时-分)
	INT					m_refreshTime2;     // 间隔刷新，精确到分钟
    BOOL				m_bRefreshTime2;    // 标示定时刷新是否完成
	INT					m_nRate;            // 几率
	_MERCHANDISE_LIST*	m_ItemList;
	BOOL				m_IsDyShop;
	INT					m_nCustomNum;
    UINT                m_nVersion;         // 商店刷新的版本号，用于校验
    BOOL                m_bInit;            // 初始化标志

};

/*
动态商店分组信息 
*/
struct DYSHOP_GROUP
{
    enum
    {
        DY_GROUPID = 0,             //组ID
        DY_NUM,                     //该组商品上限
        DY_BEGIN   = 2,

        DY_ITEM_PROPERTY_NUM = 26,  //动态商店物品属性数量
    };

    struct DY_ITEM
    {
        INT     m_ItemID;                                   //物品index
        INT     m_ItemNumMin;                               //物品可售数量下限
        INT     m_ItemNumMax;                               //物品可售数量上限

        INT     m_Rate;                                     //出现几率

        CHAR    m_RandPriceType;                            //随机价格类型
        INT     m_PriceNum;                                 //价格种类数量
        INT     m_PriceType[MAX_DY_GROUP_PRICE_NUM];        //价格类型
        INT     m_Price[MAX_DY_GROUP_PRICE_NUM];            //价格
    };
    
    INT         m_GroupId;                                  //组ID
    INT         m_GoodsNum;                                 //商品数量
    DY_ITEM     m_Goods[MAX_DY_PER_GROUP_GOODS_NUM];        //商品属性
};



/*
ShopMgr,负责数据的访问与管理
*/
class ShopMgr
{
public:
	friend class Obj_Human;
//initial+destory
public:
	ShopMgr();
	~ShopMgr();
public:
	virtual	BOOL		Init(){return TRUE;};
	virtual VOID		CleanUp();
	_SHOP*				GetShopByIndex(INT index){return &m_Shops[index];}
	_SHOP*				GetShopByID(INT id);
	INT					GetShopIndexByID(INT id);

//globle
public:
	static INT			ConvertItemType2Money(_ITEM_TYPE it);

//data
protected:
	_SHOP*				m_Shops;
	INT					m_Count;
};

/*
StaticShopManager,静态态的商店管理器,整个游戏世界只有一个静态商店管理器的实例，
用来保存所有不会变化的商店的信息．它同时也是所有动态商店管理器的生成商店的模版．
*/
class StaticShopManager	: public ShopMgr
{
public:
	StaticShopManager(){;}
	~StaticShopManager();
public:
	BOOL				Init();
	VOID				CleanUp();
private:
	BOOL				LoadShopsFromFile( CHAR* filename ) ;	
};

/*
DynamicShopManager,顾名思义,动态的商店管理器,
它的每个实例挂在每个NPC上，由NPC心跳触发Ｔｉｃｋ()
动态的商店可以随着时间刷新，来更改自己的局部数据
*/
class DynamicShopManager : public ShopMgr
{

public:
	DynamicShopManager(Obj_Monster* pboss);
	~DynamicShopManager();
public:
	INT					GetShopsNum()const{return m_nCurrent;}
public:
	BOOL				Init();
	VOID				CleanUp();
	INT					AddDynamicShop(_SHOP* pSource);
	BOOL				Tick(UINT uTime);
    VOID                Fresh(Scene *pScence, _SHOP *pShop);

private:
	INT					m_nCurrent;
	CMyTimer*			m_aRefeshTimer;
	Obj_Monster*		m_pBoss;

};

extern StaticShopManager* g_pStaticShopManager;

#endif