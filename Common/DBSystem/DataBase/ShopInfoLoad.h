/********************************************************************
	created:	2007-9-28   12:47
	filename: 	Common\DBSystem\DataBase\ShopInfoLoad.h
	author:		YangJun
	
	purpose:	各个不同版本商店/宠物繁殖的加载结构
*********************************************************************/

#ifndef _SHOPINFOLOAD_H_
#define _SHOPINFOLOAD_H_

#include "Type.h"
#include "DB_Struct.h"

//////////////////////////////////////////////////////////////////////////
//当前商店柜台的版本号(每次修改柜台信息后需要手动增加此版本号)
#define		SHOP_STALL_VERSION_0				0
			/*
			<Version0>
			扩展“珍兽称号功能”之前
			_PET_TITLE		m_PetTitle[PET_TITLE_NUM];		// 珍兽称号列表
			TitleID_t		m_tCurPetTitleID;				// 当前头顶称号
			*/
#define		SHOP_STALL_VERSION_1				1
			/*
			<Version1>
			扩展“珍兽解锁功能”之前
			USHORT			m_usPWUnlockTime;				// 二级密码解锁时间（单位：小时）
			USHORT			m_usReserve;					// 预留位
			*/
#define		SHOP_CURRENT_STALL_VERSION			2

namespace PLAYER_SHOP
{
	struct StallBoxDB_t;
	struct PlayerShopBuyShopStallDB_t;
	struct PlayerShopBuyShopDB_t;
}
//////////////////////////////////////////////////////////////////////////
//当前宠物繁殖的版本号
#define		PETPROCREATE_ITEM_VERSION_0			0
			/*
			<Version0>
			扩展“珍兽称号功能”之前
			_PET_TITLE		m_PetTitle[PET_TITLE_NUM];		// 珍兽称号列表
			TitleID_t		m_tCurPetTitleID;				// 当前头顶称号
			*/
#define		PETPROCREATE_ITEM_VERSION_1			1
			/*
			<Version1>
			扩展“珍兽解锁功能”之前
			USHORT			m_usPWUnlockTime;				// 二级密码解锁时间（单位：小时）
			USHORT			m_usReserve;					// 预留位
			*/
#define		PETPROCREATE_CURRENT_ITEM_VERSION	2

class PetProcreateItem;
//////////////////////////////////////////////////////////////////////////
//宠物称号带来的结构体扩展
#pragma pack(push, 1)
struct _PET_DB_LOAD_V0
{
	PET_GUID_t		m_GUID;							// 唯一编号
	PET_GUID_t		m_SpouseGUID;					// 配偶唯一编号
	INT				m_nDataID;						// 珍兽模型
	CHAR			m_szName[MAX_CHARACTER_NAME];	// 名字
	CHAR			m_szNick[MAX_NICK_NAME];		// 昵称
	INT				m_nLevel;						// 等级
	INT				m_nLastProcreateLevel;			// 最后繁殖的等级
	INT				m_nTakeLevel;					// 携带等级
	INT				m_nAttackType;					// 进攻类型（物/法）
	INT				m_AIType;						// AI类型 PET_AI_TYPE
	INT				m_nHP;							// 生命值
	INT				m_nLife;						// 当前寿命
	BYTE			m_byPetType;					// PET_TYPE 宝宝，变异，野生
	BYTE			m_byGeneration;					// 几代宠
	BYTE			m_byHappiness;					// 快乐度

	INT				m_nStrPerception;				// 力量资质
	INT				m_nConPerception;				// 体力资质
	INT 			m_nDexPerception;				// 身法资质
	INT				m_nSprPerception;				// 灵气资质
	INT 			m_nIntPerception;				// 定力资质
	INT				m_nGenGu;						// 根骨
	FLOAT			m_fGrowRate;					// 成长率
	INT				m_nRemainPoint;					// 一级属性剩余点数
	INT				m_nExp;							// 经验值
	BYTE			m_yFlags;						// 标志位属性 PET_FLAG_MASK_PASSWORD_LOCKED
	_ATTR_LEVEL1	m_BaseAttrLevel1;				// 基础一级战斗属性（不包括技能和装备增加的部分）

	_PET_SKILL		m_SkillList[PET_MAX_SKILL_COUNT];	// 珍兽技能列表,其中第0个是主动技能槽

	_COOLDOWN_DB_LOAD_FOR_PET m_CooldownList;			//珍兽的冷却列表
	INT				m_nSavvy;						// 悟性

	_PET_DB_LOAD_V0()
	{
		CleanUp();
	}

	VOID CleanUp( VOID );
};
struct _PET_DB_LOAD_V1
{
	_PET_DB_LOAD_V0	m_V0;

	//〖Lybin Sep.13 2007〗
	// 扩展：珍兽称号
	_PET_TITLE		m_PetTitle[PET_TITLE_NUM];		// 珍兽称号列表
	TitleID_t		m_tCurPetTitleID;				// 当前头顶称号

	_PET_DB_LOAD_V1()
	{
		CleanUp();
	}

	VOID CleanUp( VOID );
};
#pragma pack(pop)
class PetInfoLoad
{
public:
	static VOID LoadFrom_VO(_PET_DB_LOAD& petDB,_PET_DB_LOAD_V0& petDB_V0);
	static VOID LoadFrom_V1(_PET_DB_LOAD& petDB,_PET_DB_LOAD_V1& petDB_V1);
};

struct _ITEM_V1
{
	_ITEM_GUID			m_ItemGUID ;				//物品固定信息（不能改变）
	UINT				m_ItemIndex;				//物品类型
													/*
													|	    1 ~ 10000		白色装备
													|	10001 ~ 20000		绿色装备	
													|	20001 ~ 30000		蓝色装备
													|	30001 ~ 35000		药瓶
													|	35001 ~ 40000		宝石
													|						...
													*/

	CHAR				m_RulerID;
	CHAR				m_nsBind;			//物品标记
	CHAR				m_Creator[MAX_ITEM_CREATOR_NAME];
	INT					m_Param[MAX_ITEM_PARAM];
	
	union 
	{
		EQUIP_INFO			m_Equip;
		GEM_INFO			m_Gem;
		MEDIC_INFO			m_Medic;
		STORE_MAP_INFO		m_StoreMap;
	};

	_ITEM_V1()
	{
		CleanUp();
	}

	VOID CleanUp( VOID )
	{
		memset( this, 0, sizeof(*this) ) ;
	}
};
class ItemInfoLoad
{
public:
	static VOID LoadFrom_V1(_ITEM& itemDB,_ITEM_V1& itemDB_V1);
};

//柜台版本0	扩展的是_PET_DB_LOAD的宠物称号部分
struct StallBoxDB_t_V0
{
	BYTE			m_StallStatus;							//当前柜台的状态（开张，打烊，开放）
	BYTE			m_StallType;							//当前柜台的类型（物品1，珍兽2）
	BYTE			m_ValidNum;								//有效个数
	UINT			m_nFirstPage;							//起始页
	BOOL			m_bItemIsOnSale[STALL_BOX_SIZE];		//标示该位置的商品是否上架
	UINT			m_ItemPrice[STALL_BOX_SIZE];			//标示该位置的商品的价格
	_ITEM			m_ItemList[STALL_BOX_SIZE];				//物品数据
	_PET_DB_LOAD_V0	m_PetList[STALL_PET_BOX_SIZE];			//珍兽数据

	VOID CleanUp()
	{
		memset(this, 0, sizeof(StallBoxDB_t_V0));
		m_StallType	= 1;									//默认是物品柜台
	}
};
struct StallBoxDB_t_V1
{
	BYTE			m_StallStatus;							//当前柜台的状态（开张，打烊，开放）
	BYTE			m_StallType;							//当前柜台的类型（物品1，珍兽2）
	BYTE			m_ValidNum;								//有效个数
	UINT			m_nFirstPage;							//起始页
	BOOL			m_bItemIsOnSale[STALL_BOX_SIZE];		//标示该位置的商品是否上架
	UINT			m_ItemPrice[STALL_BOX_SIZE];			//标示该位置的商品的价格
	_ITEM_V1		m_ItemList[STALL_BOX_SIZE];				//物品数据
	_PET_DB_LOAD_V1	m_PetList[STALL_PET_BOX_SIZE];			//珍兽数据

	VOID CleanUp()
	{
		memset(this, 0, sizeof(StallBoxDB_t_V1));
		m_StallType	= 1;									//默认是物品柜台
	}
};

//回购商店
struct PlayerShopBuyShopStallDB_t_V1
{
	INT				m_iNeededItemNum[PLAYER_SHOP_BUY_STALL_BOX_SIZE];
	UINT			m_iNeededItemIdx[PLAYER_SHOP_BUY_STALL_BOX_SIZE];
	INT				m_iPrice[PLAYER_SHOP_BUY_STALL_BOX_SIZE];								
	_ITEM_V1		m_ItemList[PLAYER_SHOP_BUY_STALL_BOX_SIZE];		//物品数据		
	BYTE			m_InStallNum[PLAYER_SHOP_BUY_STALL_BOX_SIZE];	//现在柜台中已经收购到的
	CHAR			m_Reserve[20];									//预留

	VOID CleanUp()
	{
		memset(this, 0, sizeof(PlayerShopBuyShopStallDB_t_V1));
	}
};
struct PlayerShopBuyShopDB_t_V1
{
	CHAR							m_szDesc[PLAYER_SHOP_BUY_STALL_DESC_DB];
	PlayerShopBuyShopStallDB_t_V1	m_StallList[PLAYER_SHOP_BUY_STALL_NUM];
	CHAR							m_Reserve[40];					//预留

	VOID CleanUp()
	{
		memset(this, 0, sizeof(PlayerShopBuyShopDB_t_V1));
	}
};

class ShopInfoLoad
{
public:
	static VOID LoadFrom_V0(PLAYER_SHOP::StallBoxDB_t& stBox,StallBoxDB_t_V0& stBox_V0);
	static VOID LoadFrom_V1(PLAYER_SHOP::StallBoxDB_t& stBox,StallBoxDB_t_V1& stBox_V1);
	static VOID LoadFrom_BuyShop_V1(PLAYER_SHOP::PlayerShopBuyShopDB_t& stBuyShop,PlayerShopBuyShopDB_t_V1& stBuyShop_V1);
};

//////////////////////////////////////////////////////////////////////////
//宠物繁殖结构对宠物称号的扩展
struct PetProcreateItem_VO
{
	CHAR			m_cState;				// 状态 ENUM_STATE
	GUID_t			m_guidHumanFirst;
	GUID_t			m_guidHumanSecond;
	CHAR			m_szFirstHumanName[MAX_CHARACTER_NAME];
	CHAR			m_szSecondHumanName[MAX_CHARACTER_NAME];
	_PET_DB_LOAD_V0			m_dbPetFirst;
	_PET_DB_LOAD_V0			m_dbPetSecond;
	UINT			m_uProcreateInterval;	// 离可以繁殖的时间间隔
	UINT			m_uWaitTakeOutTime;		// 繁殖已经完毕了多少时间
	UINT			m_uPrevMailToHumanTime;	// 上次通知取出珍兽的时间
	UINT			m_uStateInterval;		// 状态设置的时间间隔
};
struct PetProcreateItem_V1
{
	CHAR			m_cState;				// 状态 ENUM_STATE
	GUID_t			m_guidHumanFirst;
	GUID_t			m_guidHumanSecond;
	CHAR			m_szFirstHumanName[MAX_CHARACTER_NAME];
	CHAR			m_szSecondHumanName[MAX_CHARACTER_NAME];
	_PET_DB_LOAD_V1			m_dbPetFirst;
	_PET_DB_LOAD_V1			m_dbPetSecond;
	UINT			m_uProcreateInterval;	// 离可以繁殖的时间间隔
	UINT			m_uWaitTakeOutTime;		// 繁殖已经完毕了多少时间
	UINT			m_uPrevMailToHumanTime;	// 上次通知取出珍兽的时间
	UINT			m_uStateInterval;		// 状态设置的时间间隔
};

class PetProcreateInfoLoad
{
public:
	static VOID LoadFrom_V0(PetProcreateItem& petPro,PetProcreateItem_VO& petPro_V0);
	static VOID LoadFrom_V1(PetProcreateItem& petPro,PetProcreateItem_V1& petPro_V1);
};
#endif