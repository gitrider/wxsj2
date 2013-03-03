/********************************************************************
	创建日期:	2005年11月2日
	创建时间:	16:54
	文件名称:	ItemTable.h
	文件路径:	d:\Prj\Server\Server\Item\ItemTable.h
	创建人:		曾鹏翔
	
	文件功能： 物品基础数据表操作
	修改纪录：
	
*********************************************************************/

#ifndef _ITEM_TABLE_H_
#define _ITEM_TABLE_H_

#include "ItemTypes.h"
#include "Type.h"
#include "GameStruct_Item.h"
#include "GameDefine2.h"

#define SELF_CONS(x)	x(){memset(this,0,sizeof(x));}
#define DEFAULT_CONS(theClass,parentClass)	theClass(){memset(this,0,sizeof(theClass)); parentClass();}


//掉落包结构
struct DROP_BOX_TB {

	SELF_CONS(DROP_BOX_TB);
	uint				m_DropBoxId;						//DropBox的编号表
	INT					m_DropValue;						//DropBox价值，参与掉落计算
	_ITEM_TYPE			m_DropItem[MAX_DROPBOX_CARRAGE];	//DropBox携带物品表
	INT					m_Quality[MAX_DROPBOX_CARRAGE];		//物品品质表	
};

struct MINMAX_VALUE 
{
	SELF_CONS(MINMAX_VALUE)
	INT			m_minValue;
	INT			m_maxValue;
};


enum  MINMAX_TYPE
{
	VT_MIN,
	VT_MAX
};
//物品数值段结构体
struct ITEM_VALUE_TB 
{
	SELF_CONS(ITEM_VALUE_TB)
	uint					m_ValueType;
	MINMAX_VALUE			m_Values[MAX_ITEM_TABLE_VALUES];
};

//----------------------------------------------------------------------------------------
//物品属性等级结构体
struct EQUIP_ATT_LEVEL_TB 
{
	SELF_CONS(EQUIP_ATT_LEVEL_TB)
	INT						m_AttID;
	INT						m_Attr[MAX_EQUIP_LEVEL];			//装备属性数值
};

//物品数值段结构体
struct EQUIP_ATT_SET_TB 
{
	SELF_CONS(EQUIP_ATT_SET_TB);
	INT						m_IndexID;
	CHAR					m_SetName[100];
	INT                     m_AttSetRate;
	INT						m_ReqLevel;
	INT						m_SumRate;
	INT						m_AttCount;
	INT						m_Attr[MAX_EQUIP_EXT_ATTR];		//装备属性
};


//装备都有的基础属性
struct EQUIP_TB 
{
	SELF_CONS(EQUIP_TB);
	INT						m_IndexID;
	_ITEM_TYPE				m_ItemType;
	BYTE					m_EquipPoint;					//装配点
	CHAR					m_RulerID;						//规则编号
	INT						m_EquipSetNum;					//套装

	INT						m_ReqLevelCount;
	BYTE					m_ReqLevel[MAX_RAND_LEVEL_NUM];	//需要等级
	UINT					m_BasePrice;					//售出价格
	INT						m_MaxDur;						//最大耐久
	INT						m_Level;						//星级
	BYTE                    m_BaseGemHole;					//天然孔
	BYTE					m_MAXGemHole;					//最大镶嵌孔
	BYTE					m_PrefessionReq[MATTRIBUTE_NUMBER];	//职业需求	从理论上支持全部的职业
	BYTE					m_PaiType;						//牌型
	INT						m_EquipScore;					//装备分

	INT                     m_NearAttack;					//近程攻击
	INT						m_FarAttack;					//远程攻击

	INT						m_MagicNearAttack;				//近程内功攻击
	INT						m_MagicFarAttack;				//近程外功攻击

	INT                     m_NearDefence;					//近程防御
	INT						m_FarDefence;					//远程防御

	INT						m_MagicNearDefense;				//近程内功防御
	INT						m_MagicFarDefense;				//近程外功防御

	INT						m_EquipSetMaxNum;				//最大套装数目
	BYTE					m_AttCount;						//装备属性个数
	BYTE					m_AffSkillCount;				//附加技能个数
	BYTE					m_EffSkillCount;				//影响技能个数
	_ITEM_ATTR				m_Attr[MAX_EQUIP_EXT_ATTR];		//装备属性
	_ITEM_AFFIX_SKILL		m_AffixSkill[MAX_EQUIP_AFFIX_SKILL];		//附加技能
	_ITEM_EFFECT_SKILL		m_EffectSkill[MAX_EQUIP_EFFECT_SKILL];		//装备影响的技能
};
//----------------------------------------------------------------------------------------

//怪物掉落包结构体
struct MONSTER_DROP_TB 
{
	SELF_CONS(MONSTER_DROP_TB);
	uint					m_DropId;
	uint					m_ValidCount;
	MONSTER_DROPCONTENT		m_Drops[MAX_MONSTER_DROPBOX];
};

//宝石表结构体
struct GEMINFO_TB 
{
	GEMINFO_TB()
	{
		m_nTableIndex	=	INVALID_ID;
		m_nRulerID		=	INVALID_RULERID;
		m_nPrice		=	INVALID_PRICE;
		m_nSellPrice	=	INVALID_PRICE;
		m_bBroadCast	=   FALSE;
		m_AttCount		=	0;
		m_GemReqCount	=	0;
		m_ItemType.CleanUp();
		for(INT i=0; i<MAX_BASE_EQUIP_COUNT; ++i)
			m_EquipType[i] = 0;
		for(INT i=0; i<MAX_GEM_ATT; ++i)
			m_GenAttr[i].CleanUp();
		for(INT i=0; i<MAX_GEM_REQ; ++i)
			m_GemReqAttr[i].CleanUp();
	}

	UINT					m_nTableIndex;
	_ITEM_TYPE				m_ItemType;
	CHAR					m_nRulerID;
	UINT					m_EquipType[MAX_BASE_EQUIP_COUNT];
	UINT					m_nPrice;
	UINT					m_nSellPrice;
	BOOL					m_bBroadCast;
	UINT					m_AttCount;
	_ITEM_ATTR				m_GenAttr[MAX_GEM_ATT];
	UINT					m_GemReqCount;
	_GEM_REQ_ATT			m_GemReqAttr[MAX_GEM_REQ];
};

struct EQUIPHOLE_INFO_TB
{
	SELF_CONS(EQUIPHOLE_INFO_TB);
	UINT					m_ReqItem;
	UINT					m_ReqItemNum;
	UINT					m_ReqMoney;
	UINT					m_Rate;
};

struct EQUIPCHAIN_INFO_TB 
{
	EQUIPCHAIN_INFO_TB()
	{
		m_ChainID	= INVALID_PAI_TYPE;
		m_ReqPai1	= INVALID_PAI_TYPE;
		m_ReqPai2	= INVALID_PAI_TYPE;
		m_ReqPai3	= INVALID_PAI_TYPE;
		m_Att.CleanUp();
		m_Flat.CleanUp();
	}
	UINT					m_ChainID;
	UINT					m_ReqPai1;
	UINT					m_ReqPai2;
	UINT					m_ReqPai3;
	Flag128					m_Flat;
	_ITEM_ATTR				m_Att;
};

struct EQUIPSOUL_INFO_TB 
{
	EQUIPSOUL_INFO_TB()
	{
		m_SoulID			= INVALID_SOUL_TYPE;
		m_ChainID1			= INVALID_PAI_TYPE;
		m_ChainID2			= INVALID_PAI_TYPE;
		m_ChainID3			= INVALID_PAI_TYPE;
		m_ChainID4			= INVALID_PAI_TYPE;
		m_ChainID5			= INVALID_PAI_TYPE;
		memset(SoulType, 0, sizeof(UINT)*1000);
		memset(ValidSoul, 0, sizeof(UINT)*4);
		ValidSoulNum		= 0;
		m_EffectAttCount	= 0;
		m_AnyShunNum		= 0;
		m_AnyKeNum			= 0;
	}
	UINT					m_SoulID;
	UINT					m_ChainID1;
	UINT					m_ChainID2;
	UINT					m_ChainID3;
	UINT					m_ChainID4;
	UINT					m_ChainID5;
	UINT					m_AnyShunNum;
	UINT					m_AnyKeNum;
	UINT					SoulType[1000];
	UINT					ValidSoul[4];
	UINT					ValidSoulNum;
	UINT					m_EffectAttCount;
	_ITEM_ATTR				m_Att[MAX_SOUL_EFFECTATT];
};

struct CHANGEPAI_INFO_TB 
{
	UINT					m_nTableIndex;
	UINT					m_DestPaiType;					
};
;
//普通物品表结构体
struct COMMITEM_INFO_TB 
{
	COMMITEM_INFO_TB()
	{
		m_nTableIndex		=	0;
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nSellPrice		=   0;
		m_nLayedNum			=	0;
		m_nScriptID			=	0;
		m_nSkillID			=	0;
		m_nRulerID			=	0;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_HoldNum			=   -1;
		m_TargetType		=	0;
		m_bBroadcast		=   FALSE;
		m_ItemType.CleanUp();
		
	}

	INT					m_nTableIndex;
	_ITEM_TYPE			m_ItemType;
	INT					m_nLevel;
	INT					m_nUseLeve;
	UINT				m_nBasePrice;
	UINT				m_nSellPrice;
	CHAR				m_nRulerID;	 //规则ID
	INT					m_nLayedNum; //叠放数量
	INT					m_nScriptID;
	INT					m_nSkillID;
	INT					m_nReqSkill;
	INT					m_nReqSkillLevel;
	INT					m_HoldNum;
	BYTE				m_TargetType;
	BOOL				m_bBroadcast;
};

//任务物品表结构体
struct TASKITEM_INFO_TB 
{
	TASKITEM_INFO_TB()
	{
		m_nTableIndex		=	0;
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nSellPrice		=   0;
		m_nLayedNum			=	0;
		m_nScriptID			=	0;
		m_nSkillID			=	0;
		m_nRulerID			=	0;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_HoldNum			=   -1;
		m_TargetType		=	0;
		m_bBroadcast		=   FALSE;
		m_ItemType.CleanUp();

	}

	INT					m_nTableIndex;
	_ITEM_TYPE			m_ItemType;
	INT					m_nLevel;
	INT					m_nUseLeve;
	UINT				m_nBasePrice;
	UINT				m_nSellPrice;
	CHAR				m_nRulerID;	 //规则ID
	INT					m_nLayedNum; //叠放数量
	INT					m_nScriptID;
	INT					m_nSkillID;
	INT					m_nReqSkill;
	INT					m_nReqSkillLevel;
	INT					m_HoldNum;
	BYTE				m_TargetType;
	BOOL				m_bBroadcast;
};

//藏宝图结构体
struct STORE_MAP_INFO_TB 
{
	STORE_MAP_INFO_TB()
	{
		m_nTableIndex	=	0;
		m_ItemType.CleanUp();
		m_ResourceID	=	0;
		m_nRulerID		=	0;
		m_nLevel		=	0;
		m_nBasePrice	=	0;
		m_xPos			=	-1.0f;
		m_zPos			=	-1.0f;
		m_SceneID		=	-1;
		m_GrowPointType	=	-1;

	}
	INT						m_nTableIndex;;
	_ITEM_TYPE				m_ItemType;
	WORD					m_ResourceID;
	CHAR					m_nRulerID;
	INT						m_nLevel;
	UINT					m_nBasePrice;
	FLOAT					m_xPos;
	FLOAT					m_zPos;
	INT						m_SceneID;
	INT						m_GrowPointType;
	INT						m_ScriptID;
	
};
//add by gh for souxia 2010/05/05
struct SOUXIA_INFO_TB
{
	SOUXIA_INFO_TB()
	{
		memset(this, -1, sizeof(SOUXIA_INFO_TB));
	}
	INT					m_nTableIndex;
	INT					m_nNeedLevel;	//限制等级
	INT					m_nUseType;		//对应着不同的使用类型,表示后面数值的作用对象不同.0:搜侠录道具,1:搜侠录记忆棒 2:缝合道具 3:炼化道具4:表示缝合道具-1:该道具无效
	INT					m_nSouXiaLevel;	//对于类型为0的，该子类有作用，填写的数值代表着搜侠录的等级。其中在该列填写0，表示该搜侠录为入门搜侠录，填写1表示为初级搜侠录，填写2表示为中级搜侠录，填写3表示为高级搜侠录，填写4表示特殊类型搜侠录。填写-1表示该列无效

	INT					m_nMaxPages[5];	//记录的最大页数 0为最大总页数，1是技能最大页数...
	INT					m_nMemoryType;	//对于类型填写为1的，该子类有作用。填写的数值代表着搜侠录记忆棒里记录的道具种类。填写1表示记录的是技能，填写2表示的为记录的是神器配方，填写3表示记录的是神兽召唤道具，填写4表示的是骑乘召唤道具。填写-1表示该列无效
	
	INT					m_nMemoryContent[4];// 在子类1有数值的情况下，该参数0位起作用。表示该搜侠记忆棒里记录的是什么技能，表示该道具需要的技能编号，要求支持～和｜解析填写规则．规则如下：１００～１０５，表示按照平均概率在（１００到１０５）之间随机选取1个号码．１２｜１７｜１９．表示在非连续数值之间随机选取．该列的技能ＩＤ按照一般引用技能ＩＤ的规则
	//该参数1位起作用。表示该搜侠录的神器配方的ＩＤ，要求支持解析填写格式，该参数2位起作用表示该神兽召唤的技能的ＩＤ，该ＩＤ是技能表里的ＩＤ，要求支持解析格式；该参数3位起作用，表示该搜侠录坐骑召唤技能的ＩＤ，该ＩＤ是技能表里的ＩＤ，要求支持解析格式

	INT					m_nDismantleLevel;		//对于类型填写为2的，该子类有作用。填写的数值代表着拆卸道具的品质。该列填写0，表示低级拆卸道具，填写1，表示初级拆卸道具，填写2表示中级拆卸道具，填写3表示高级拆卸道具，填写4表示特殊类型拆卸道具。填写-1表示无效
	INT					m_nDismantleRule[4][2];	//在子类2有数值的情况下，该参数起作用。表示拆卸道具的功能。其中0表示为不保留技能页数内容的拆卸。1表示随机保留1个技能页的拆卸。2表示随即保留2个技能页的拆卸，填写3表示随机3个技能页的拆卸。填写4表示完全保留的拆卸 
	
	INT					m_nRefiningLevel;		//对于类型填写为3的，该子类有作用。填写的数值代表着炼化道具的品质。该列填写0，表示低级炼化道具，填写1，表示初级炼化道具，填写2表示中级炼化道具，填写３表示高级炼化，填写4表示特殊类型炼化道具。填写-1表示无效
	INT					m_nRefiningRule[4][2];	//在子类2有数值的情况下，该参数起作用。表示拆卸道具的功能。其中0表示为不保留技能页数内容的拆卸。1表示随机保留1个技能页的拆卸。2表示随即保留2个技能页的拆卸，填写3表示随机3个技能页的拆卸。填写4表示完全保留的拆卸 
	//在子类３有数值的情况下，该参数起作用。表示炼化道具的功能。其中0表示为不保留技能页数内容的炼化。1表示在２本搜侠录里随机保留1个技能页的炼化。2表示在２本搜侠录里随机保留２个技能页的炼化填写3表示在２本搜侠录里随机保留３个技能页的炼化。填写4表示完全保留的炼化（玩家通过ＵＩ选择需要保存的炼化的页数）
	
	INT					m_nSewRate;	//对于类型填写为４的，该子类有作用。填写的数值代表着缝合道具成功缝合的概率数值填写规则是（１－１００）的整数．数值越小表示成功的概率越小
	INT					m_nUseTime; //表示对应（神兽／坐骑）召唤技能使用的次数，数值范围为1-99有效，填写-1和代表无效

};

struct	SOUL_BEAD_INFO_TB
{
	SOUL_BEAD_INFO_TB()
	{
		memset(this, 0, sizeof(SOUL_BEAD_INFO_TB));
	}
	INT					m_nTableIndex;
	BYTE				m_nRuleID;
	INT					m_nValidTime;
	INT					m_nImpactId;
};
// end of add
//掉落衰减表结构体
struct DROP_ATT_TB 
{
	SELF_CONS(DROP_ATT_TB);
	INT				m_DeltaLevel;
	FLOAT			m_AttValue;
	FLOAT			m_ExpValue;
	FLOAT			m_TeamAttValue;
	FLOAT			m_TeamExpValue;
};

//掉落位置结构体
struct DROP_POS_TB 
{
	SELF_CONS(DROP_POS_TB);
	FLOAT			m_DropX;
	FLOAT			m_DropZ;
};

struct EQUIP_LEVELUP_TB
{
	SELF_CONS(EQUIP_LEVELUP_TB);
	UINT	m_LowLevelGemID;
	UINT	m_HighLevelGemID;
	UINT	m_OtherLevelGemID;
	UINT	m_SuccessRate[MAX_LEVELUP_GEMREQ_NUM];
	UINT	m_Punishment;
	UINT	m_ReqMoney;
	UINT	m_EffectRate;
};

struct EQUIP_BIND_TB
{
	SELF_CONS(EQUIP_BIND_TB);
	UINT	m_GemID;
	UINT	m_EffectRate;
};

struct EQUIP_SPECIALREPAIR_TB
{
	SELF_CONS(EQUIP_SPECIALREPAIR_TB);
	UINT	m_ItemID;
	UINT	m_RecoverPoint;
};

struct  CREATEITEM_INFO
{
	UINT	m_MaterialType;	//材料类型
	BYTE	m_MaterialNum;	//材料数量	
	BYTE	m_NeedLevel;	//材料最低等级	
	BYTE	m_SplitRate;	//拆解损耗比例	
};

struct REMOULDITEM_INFO
{
	UINT	m_MaterialType;	//材料类型
	BYTE	m_MaterialNum;	//材料数量	
	BYTE	m_NeedLevel;	//材料最低等级	
};

struct  ITEM_PRODUCT_INFO_TB
{
	SELF_CONS(ITEM_PRODUCT_INFO_TB);
	UINT				m_ProductID;		//产品ID	
	UINT				m_PrescriptionID;	//配方号	
	BYTE				m_ProductNum;		//产品数量
	UINT				m_nCreateInfoCount;	
	CREATEITEM_INFO		m_CreateInfo[MAX_CREATE_MATERIAL_NUM];		//材料类1
	UINT				m_nRemouldInfoCount;
	REMOULDITEM_INFO	m_RemouldInfo[MAX_REMOULD_MATERIAL_NUM];
	UINT				m_OperateTime;
	UINT				m_ScriptID;
	UINT				m_CreateMoney;
	UINT				m_RemouldMoney;
	BYTE				m_NeedNPC;
};


struct MATERIAL_LEVEL_INFO_TB
{
	MATERIAL_LEVEL_INFO_TB()
	{
		m_MaterialValue = 0;
	}
	UINT m_MaterialValue;
};

struct  EQUIP_EXTRAATT_SET
{
	INT				m_AttrCount;
	INT				m_Attr[10];
};

struct EQUIPEXTRATT
{
	UINT					m_ValidCount;
	EQUIP_EXTRAATT_SET		m_GenAttr[MAX_ITEM_NATUREATTR];
};

//装备附加属性
struct EQUIPEXTRATT_TB 
{
	SELF_CONS(EQUIPEXTRATT_TB);

	UINT					m_nEquipScore;
	EQUIPEXTRATT			m_EquipExtrAtt[MAX_JOB_SUM];
};

struct EXTRABAG_INFO_TB 
{
	EXTRABAG_INFO_TB()
	{
		m_nTableIndex	=	0;
		m_ItemType.CleanUp();
		m_nRulerID		=	0;
		m_nBasePrice	=	0;
		m_nSellPrice	=   0;
		m_nValidTime	=   0;
		m_nSize			=   0;
	}	
	UINT					m_nTableIndex;
	_ITEM_TYPE				m_ItemType;
	CHAR					m_nRulerID;	 //规则ID
	UINT					m_nBasePrice;
	UINT					m_nSellPrice;
	UINT					m_nValidTime;
	UINT					m_nSize;
};

struct  PAI_INFO
{
	PAI_INFO()
	{
		m_nPaiID = INVALID_PAI_TYPE;
		m_nRate  = 0;
		m_Att.CleanUp();
	}
	UINT					m_nPaiID;
	_ITEM_ATTR				m_Att;
	UINT					m_nRate;
};

//装备集合结构体
struct EQUIP_SET_TB
{
	SELF_CONS(EQUIP_SET_TB);
	INT				m_nEquipSetSerial;
	INT				m_nEquipSetCount;
	INT				m_nAttrCount;
	_ITEM_ATTR		m_ItemAttr[MAX_ITEM_SET_ATTR];
	INT				m_ActiveAttrNeedNum[MAX_ITEM_SET_ATTR];
};


//物品规则结构体
struct ITEM_RULER_TB :public _ITEM_RULER{
	
	SELF_CONS(ITEM_RULER_TB);
	INT		m_RulerIndex;
	
};

//坐骑结构体  [7/8/2010 陈军龙]
struct MOUNT_INFO_TB 
{
	MOUNT_INFO_TB()
	{
		m_nTableIndex	=	0;
		m_nRule	        =	0;
		m_nNeedLevel    =   0;
		m_nAddtionSpeed	=   0;
		m_nLife     	=   0;
        m_nBindType     =   0;
        m_nPrice        =   0;
	}	
	UINT	         m_nTableIndex;    //id
	CHAR	         m_nRule;		   //适应规则
	UINT             m_nNeedLevel;     //需要达到的等级
	INT	             m_nAddtionSpeed;  //增加的速度
	INT              m_nLife;          //存在时限（单位：天）
	INT              m_nBindType;      //绑定类型 
	INT              m_nPrice;         //所需金钱
};

/*
 *物品相关表资源数据类	
 */
class ItemTable
{
	public:
		ItemTable() ;
		~ItemTable() ;

		VOID	CleanUp( ) ;
		BOOL	Init();


	public:
		MONSTER_DROP_TB*			GetMonsterDropTB(INT iMonsterType);
		
		//------------------------------------------------------------------------
		DROP_POS_TB*				GetDropPosTB(INT nDropPoint);

		EQUIP_TB*					GetEquipTB(UINT itemSerial);			//获取装备信息
		UINT						GetEquipPointValue(HUMAN_EQUIP equipPoint);
		PAI_INFO*					GetPaiInfo(UINT nType);
		UINT						GetZiPai(UINT nSeed);
		UINT						GetZiPaiCount();
		UINT						GetNoZiPai(UINT nSeed);
		UINT						GetNoZiPaiCount();

		EQUIP_ATT_SET_TB*			GetAttSetTB(INT nSetID);				//获取套装信息
		EQUIP_ATT_SET_TB*			GetAttSetTBByRate(INT nRate);
		UINT						GetSumRateByReqLevel(INT nLevel);
		EQUIP_ATT_LEVEL_TB*			GetAttLevelValue(ITEM_ATTRIBUTE iAtt);	//获取属性等级信息
		//------------------------------------------------------------------------
		MINMAX_VALUE				GetItemValue(ITEM_ATTRIBUTE iAtt,INT QualityLevel);

		DROP_ATT_TB*				GetDropAttTB(INT iDeltaLevel);

		GEMINFO_TB*					GetGemInfoTB(UINT	itemSerial);
		EQUIPHOLE_INFO_TB*			GetEquipHoleInfoTB(UINT	nHoleNum);
		EQUIPSOUL_INFO_TB*			GetEquipSoulInfoTB(UINT	nIndex);
		EQUIPCHAIN_INFO_TB*			GetEquipChainInfoTB(UINT nIndex);
		EQUIP_LEVELUP_TB*			GetEquipLeveupTB(UINT nLevel);
		EQUIP_BIND_TB*				GetEquipBindTB(UINT nLevel);
		EQUIP_SPECIALREPAIR_TB*		GetSpecialRepairTB(/*UINT itemSerial*/);
		ITEM_PRODUCT_INFO_TB*		GetItemProductInfoTB(UINT nProductID);
		MATERIAL_LEVEL_INFO_TB*		GetMaterialInfoTB(UINT nLevel);
		EQUIPEXTRATT_TB*			GetEquipExtraAttInfoTB(UINT nScore);
		EXTRABAG_INFO_TB*			GetExtraBagInfoTB(UINT	itemSerial);
		CHANGEPAI_INFO_TB*			GetChangePaiInfoTB(UINT	itemSerial);
		MOUNT_INFO_TB*	            GetMountInfoTB(UINT	itemSerial);           // [7/8/2010 陈军龙]

		COMMITEM_INFO_TB*			GetCommItemInfoTB(UINT	itemSerial);
		TASKITEM_INFO_TB*			GetTaskItemInfoTB(UINT	itemSerial);
		EQUIP_SET_TB*				GetEquipSetTB(INT EquipSetSerial);
		
		ITEM_RULER_TB*				GetRuleValueByID(INT& iIndex);

		STORE_MAP_INFO_TB*			GetStoreMapTB(UINT	itemSerial);;
		SOUXIA_INFO_TB*				GetSouXiaTB(UINT	itemSerial);	// add by gh for souxia_item
		SOUL_BEAD_INFO_TB*			GetSoulBeadTB(UINT	itemSerial);
		INT							GetAttCount(){ return m_nAttLevelCount;}
		INT							GetAttSetCount(){ return m_nEquipAttSetCount;}
		INT							GetEquipChainInfoCount(){ return m_nEquipChainInfoCount;}
		INT							GetEquipSoulInfoCount(){ return m_nEquipSoulInfoCount;}
	protected:

		//---------------------------------------------------------------------------
		VOID						InitEquipTable();
		VOID						InitAttLevelTable();
		VOID						InitEquipATTSetTable();
		
		//---------------------------------------------------------------------------
		VOID						InitMonsterDropTable();
		VOID						InitGemInfoTable();
		VOID						InitCommItemInfoTable();
		VOID						InitEquipHoleInfoTable();
		VOID						InitEquipLevelupInfoTable();
		VOID						InitEquipBindInfoTable();
		VOID						InitSpecialRepairInfoTable();
		VOID						InitProductInfoTable();
		VOID						InitMaterialLevelInfoTable();
		VOID						InitEquipExtraInfoTable();
		VOID						InitEquipPointInfoTable();
		VOID						InitExtraBagInfoTable();
		VOID						InitChangePaiInfoTable();
		VOID						InitPaiInfoTable();
		VOID						InitEquipChainInfoTable();
		VOID						InitEquipSoulInfoTable();

		VOID						InitTaskItemInfoTable();
		VOID						InitDropAttTable();
		VOID						InitItemDropPos();
		VOID						InitEquipSetAttrTable();
		VOID						InitItemRulerTable();
		VOID						InitStoreMapTable();
		VOID						InitSouXiaTable();//初始化捜侠录表格
		VOID						InitSoulBeadTable();//初始化魂珠表格结构
		VOID	                    InitMountAttrTable();// [7/14/2010 陈军龙]
	private:
		//-----------------------------------------------------------------------------------
		uint						m_nAttLevelCount;
		EQUIP_ATT_LEVEL_TB*         m_pAttLevelTableData;

		uint						m_nEquipAttSetCount;
		EQUIP_ATT_SET_TB*			m_pEquipAttSetTableData;

		uint						m_nEquipCount;
		EQUIP_TB*					m_pEquipTableData;
		//------------------------------------------------------------------------------------
		uint						m_nDropBoxCount;
		DROP_BOX_TB*				m_pDropBoxTableData;

		uint						m_nMonsterDropCount;
		MONSTER_DROP_TB*			m_pMonsterDropTableData;

		uint						m_nGemCount;
		GEMINFO_TB*					m_pGemInfoData;

		uint						m_nEquipHoleInfoCount;
		EQUIPHOLE_INFO_TB*			m_EquipHoleInfoData;	
		
		uint						m_nLeveupInfoCount;
		EQUIP_LEVELUP_TB*			m_EquipLeveupInfoData;
		
		uint						m_nEquipBindInfoCount;
		EQUIP_BIND_TB*				m_EquipBindInfoData;

		uint						m_nSpecialRepairCount;
		EQUIP_SPECIALREPAIR_TB*		m_EquipSpecialRepairData;

		uint						m_nProductCount;
		ITEM_PRODUCT_INFO_TB*		m_pProductInfoData;
		
		uint						m_nMaterialLevelInfoCount;
		MATERIAL_LEVEL_INFO_TB*		m_MaterialLevelInfoData;

		uint						m_nEquipExtraInfoCount;
		EQUIPEXTRATT_TB*			m_EquipExtraInfoData;
		
		uint						m_nExtraBagInfoCount;
		EXTRABAG_INFO_TB*			m_pExtraBagInfoData;
		
		uint						m_nEquipChainInfoCount;
		EQUIPCHAIN_INFO_TB*			m_EquipChainInfoData;
		
		uint						m_nEquipSoulInfoCount;
		EQUIPSOUL_INFO_TB*			m_EquipSoulInfoData;
		
		uint						m_nChangePaiInfoCount;
		CHANGEPAI_INFO_TB*			m_pChangePaiInfoData;
		
		uint						m_nCommItemCount;
		COMMITEM_INFO_TB*			m_pCommItemInfoData;

		uint						m_nTaskItemCount;
		TASKITEM_INFO_TB*			m_pTaskItemInfoData;	

		uint						m_nDropAttCount;
		DROP_ATT_TB*				m_pDropAttData;

		uint						m_nDropPosCount;
		DROP_POS_TB*				m_pDropPosData;

		uint						m_nEquipSetCount;
		EQUIP_SET_TB*				m_pEquipSetData;

		uint						m_nItemRulerCount;
		INT							m_nHashOffSet;
		ITEM_RULER_TB*				m_pItemRulerData;

		uint						m_nStoreMapCount;
		STORE_MAP_INFO_TB*			m_pStoreMapData;

		uint						m_nSouXiaCount;		// add by gh 2010/05/05
		SOUXIA_INFO_TB*				m_pSouXiaData;		// add by gh 2010/05/05

		UINT						m_nSoulBeadCount;
		SOUL_BEAD_INFO_TB*			m_pSoulBeadData;	
		uint						m_nMountAttInfoCount;  // [7/14/2010 陈军龙]
		MOUNT_INFO_TB*			    m_pMountAttInfoData;   // [7/14/2010 陈军龙]
		UINT						m_nEquipPointInfo[HEQUIP_NUMBER];
		PAI_INFO					m_nPaiInfo[EQUIP_PAI_NUM];
		UINT						m_nZiPaiCount;
		UINT						m_nZIPai[EQUIP_PAI_NUM];
		UINT						m_nNoZIPaiCount;
		UINT						m_nNoZIPai[EQUIP_PAI_NUM];
};

BYTE	GetItemTileMax(_ITEM_TYPE& it);


extern ItemTable g_ItemTable ;

#endif
