
#ifndef __GAMESTRUCT_ITEM_H__
#define __GAMESTRUCT_ITEM_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

#define MAX_ITEM_STRING_LENGTH	255


BYTE		GetSerialType(UINT Serial);
BYTE		GetSerialClass(UINT Serial);
UINT		GetSerialIndex(UINT Serial);
BOOL        ISCommonEquip(UINT Serial);
UINT        GenMaterialIndex(UINT nMaterialType, UINT nMaterialLevel );

template <typename T> inline unsigned int ConvertUCharNew(T x, BOOL bLog, char* file, int line)
{
	unsigned int rt = ((unsigned int)((x%300000000)*13>>3));
	if(bLog) printf("%s:%d(%ld) crc=%d\n", file,line,x,rt);
	//g_pLog->SaveLog("./Log/Crc32.log","%s:%d(%ld) crc=%d",file,line,x,rt);
	return rt;
};

#define ConvertUChar(x,bLog)	ConvertUCharNew(x,bLog,__FILE__,__LINE__);





//范围
struct _RANGE_VALUE
{
	WORD				m_MinValue ;
	WORD				m_MaxValue ;
};

struct _ITEM_SKILL 
{
	BOOL				m_bActive;
	WORD				m_SkillID;
	WORD				m_SkillRate;

	BOOL				IsActive()
	{
		return m_bActive;
	}

	VOID				SetActive(BOOL bActive)
	{	
		m_bActive = bActive;
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(_ITEM_SKILL));
	}

	_ITEM_SKILL()
	{
		CleanUp();
	}
};

struct _ITEM_IMPACT 
{
	WORD				m_ImpactID;
	WORD				m_ImpactSum;

	VOID	CleanUp()
	{
		memset(this,0,sizeof(_ITEM_IMPACT));
	}

	_ITEM_IMPACT()
	{
		CleanUp();
	}
};

struct _ITEM_EFFECT_SKILL 
{
	WORD				m_nSkillID;
	BYTE				m_nLevel;

	VOID	CleanUp()
	{
		m_nSkillID = -1;
		m_nLevel   = 0;
	}
};

struct _ITEM_AFFIX_SKILL 
{
	WORD				m_nSkillID;
	BYTE				m_nLevel;

	VOID	CleanUp()
	{
		m_nSkillID = -1;
		m_nLevel   = -1;
	}
};

//物品属性值
struct _ITEM_VALUE
{
	SHORT			m_Value ;
	VOID	CleanUp( )
	{
		memset( this, 0, sizeof(*this) ) ;
	}

	BOOL	operator == (_ITEM_VALUE& iV)
	{
		return iV.m_Value = m_Value;
	}
};

//物品属性
struct _ITEM_ATTR
{
	INT					m_AttrType ;
	_ITEM_VALUE			m_Value ;

	VOID	CleanUp( )
	{
		m_AttrType	= -1;
		m_Value.CleanUp();
	}
	
	BOOL	operator==(_ITEM_ATTR& iA)
	{
		return	(iA.m_AttrType == m_AttrType) 
			   &&(iA.m_Value == m_Value);

	}
};


//物品属性
struct _GEM_REQ_ATT
{
	BYTE				m_GemType ;
	_ITEM_VALUE			m_Value ;

	VOID	CleanUp( )
	{
		m_GemType	= -1;
		m_Value.CleanUp();
	}

	BOOL	operator==(_GEM_REQ_ATT& iA)
	{
		return	(iA.m_GemType == m_GemType) 
			&&(iA.m_Value == m_Value);

	}
};


//物品唯一ID
struct _ITEM_GUID
{
	BYTE	m_World ;		//世界号: (例)101
	BYTE	m_Server ;		//服务端程序号：(例)5
	INT		m_Serial ;		//物品序列号：(例)123429
	
	_ITEM_GUID()
	{
		m_Serial = 0;
		m_Server = 0;
		m_World	 = 0;	
	}
	VOID Reset(VOID)
	{
		m_Serial = 0;
		m_Server = 0;
		m_World	 = 0;	
	}
	_ITEM_GUID& operator=(_ITEM_GUID const& rhs)
	{
		m_Serial = rhs.m_Serial;
		m_Server = rhs.m_Server;
		m_World  = rhs.m_World;
		return *this;
	}
	BOOL	operator ==(_ITEM_GUID& Ref)	const
	{
		return (Ref.m_Serial==m_Serial)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}

	BOOL	operator ==(const _ITEM_GUID& Ref)	const
	{
		return (Ref.m_Serial==m_Serial)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}

	BOOL	isNull() const
	{
		return (m_World ==0)&&(m_Serial==0)&&(m_Server == 0);
	}
	
};

//-------------------------------------------------------------------------------
struct _ITEM_TYPE
{

	struct  
	{
		UINT	m_Class : 7 ;		//基本类型，比如：装备类、消耗品、等
		UINT	m_Type: 7 ;		//类别，比如：大刀、长枪、等
		UINT	m_Index: 14 ;		//物品号，比如：柳叶刀、鸳鸯刀、等
	};

	UINT	ToUINT()
	{
		UINT	uid = 0 ;

		uid	+= m_Class;
		uid= uid<<7;
		uid	+= m_Type;
		uid= uid<<14;
		uid += m_Index;

		return uid;

	}

	UINT	ToSerial()	const
	{
		UINT Serial;
		Serial = m_Class;
		Serial	= Serial*100+m_Type;
		Serial	= Serial*10000+m_Index;
		return Serial;
	}



	BOOL	isNull() const
	{
		return (m_Class==0)&&(m_Type==0)&&(m_Index == 0);
	}

	BOOL	operator==(_ITEM_TYPE& Ref) const
	{
		return (m_Class==Ref.m_Class)&&(m_Type==Ref.m_Type)&&(m_Index==Ref.m_Index);
	}

	BOOL	operator>(_ITEM_TYPE& Ref) const
	{
		return ToSerial()>Ref.ToSerial();
	}

	BOOL	operator<(_ITEM_TYPE& Ref) const
	{
		return ToSerial()<Ref.ToSerial();		
	}
	VOID	CleanUp()
	{
		m_Class		=	0;
		m_Type		=	0;
		m_Index		=	0;
	}
};
//-------------------------------------------------------------------------------

_ITEM_TYPE	ConvertSerial2ItemType(UINT Serial);


struct _ITEM_RULER
{
	BOOL	m_Discard;		//丢弃
	BOOL	m_Tile;			//重叠
	BOOL	m_ShortCut;		//快捷
	BOOL	m_CanSell;		//出售
	BOOL	m_CanExchange;	//交易
	BOOL	m_CanUse;		//使用
	BOOL	m_PickBind;		//拾取绑定
	BOOL	m_EquipBind;	//装备绑定
	BOOL	m_Unique;		//是否唯一

	BOOL	m_Ident;		//是否需要鉴定	
	BOOL	m_DummyItem;	//是否虚拟道具
	BOOL	m_Bank;			//是否可以存入银行
	BOOL	m_Consume;		//是否消耗	

	BOOL	CanDiscard()	{return m_Discard;}		//丢弃
	BOOL	CanTile()		{return m_Tile;}		//重叠
	BOOL	CanShortCut()	{return m_ShortCut;}	//快捷
	BOOL	CanSell()		{return m_CanSell;}		//出售
	BOOL	CanExchange()	{return m_CanExchange;}	//交易
	BOOL	CanUse()		{return m_CanUse;}		//使用
	BOOL	isPickBind()	{return m_PickBind;}	//拾取绑定
	BOOL	isEquipBind()	{return m_EquipBind;}	//装备绑定
	BOOL	isUnique()		{return m_Unique;}		//是否唯一


	BOOL	isIdent()		{return m_Ident;}		//是否需要鉴定	
	BOOL	isDummyItem()	{return m_DummyItem;}		//是否虚拟道具
	BOOL	isBack()		{return m_Bank;}		//是否可以存入银行
	BOOL	isConsume()		{return m_Consume;}		//是否消耗	
};


enum	ITEM_RULER_LIST
{
	IRL_DISCARD,			//丢弃
	IRL_TILE,				//重叠
	IRL_SHORTCUT,			//快捷
	IRL_CANSELL,			//出售
	IRL_CANEXCHANGE,		//交易
	IRL_CANUSE,				//使用
	IRL_PICKBIND,			//拾取绑定
	IRL_EQUIPBIND,			//装备绑定
	IRL_UNIQUE,				//是否唯一

	IRL_IDENT,				//是否需要鉴定	　
	IRL_DUMMYITEM,			//是否虚拟道具	　
	IRL_BANK,				//是否可以存入银行	　
	IRL_CONSUME,			//是否消耗	
};



//镶嵌宝石信息
struct _ITEM_GEMINFO
{
	UINT	m_GemIndex;
	BOOL	m_bEnable;
	VOID	CleanUp()
	{
		m_GemIndex	= INVALID_ITEM_ID;
		m_bEnable   = FALSE;
	}
};




struct GEM_INFO
{
	UINT					m_nPrice;
	UINT					m_nSellPrice;
	BOOL					m_bBroadCast;
	UINT					m_EquipType[MAX_BASE_EQUIP_COUNT];
	UINT					m_AttCount;
	_ITEM_ATTR				m_GenAttr[MAX_GEM_ATT];
	UINT					m_GemReqCount;
	_GEM_REQ_ATT			m_GemReqAttr[MAX_GEM_REQ];

	VOID	CleanUp()
	{
		m_nPrice		=0;
		m_nSellPrice	=0;
		m_bBroadCast	=FALSE;
		m_AttCount		=0;
		m_GemReqCount	=0;
		for(INT i=0; i<MAX_BASE_EQUIP_COUNT; ++i)
			m_EquipType[i] = 0;
		for(int i=0; i<MAX_GEM_ATT; ++i)
			m_GenAttr[i].CleanUp();
		for(int i=0; i<MAX_GEM_REQ; ++i)
			m_GemReqAttr[i].CleanUp();
	}
};


struct STORE_MAP_INFO 
{
	INT						m_nLevel;
	FLOAT					m_xPos;
	FLOAT					m_zPos;
	INT						m_SceneID;
	INT						m_GrowType;
	
};

struct MEDIC_INFO
{
	
	BYTE					m_nCount;	   //当前数量
	BYTE					m_nLevel;
	BYTE					m_nLayedNum;   //叠放数量
	BYTE					m_nReqSkillLevel;

	UINT					m_nBasePrice;
	UINT					m_nSellPrice;
	INT						m_nScriptID;
	INT						m_nSkillID;
	INT						m_nReqSkill;
	BYTE					m_TargetType;  //物品选择类型
	BOOL					m_bBroadcast;	//是否广播
	
	INT GetTileNum()		{return m_nCount;}

	INT GetMaxTileNum()		{return m_nLayedNum;}

	VOID	CleanUp()
	{
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nSellPrice		=   0;
		m_nLayedNum			=	0;
		m_nScriptID			=	INVALID_ID;
		m_nSkillID			=	0;
		m_nCount			=	0;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_TargetType		=	0;
		m_bBroadcast			=   FALSE;
	}
};

struct EXTRABAG_INFO
{
	UINT					m_nBasePrice;
	UINT					m_nSellPrice;
	UINT					m_nValidTime;
	UINT					m_nBeginTime;
	UINT					m_nSize;
	VOID	CleanUp()
	{
		m_nBasePrice		=	0;
		m_nSellPrice		=   0;
		m_nValidTime		=   0;
		m_nBeginTime		=	0;
		m_nSize				=   0;
	}
};
// add by gh for 魂珠 2010/06/24
struct	SOUL_BEAD_INFO
{
	UINT					m_nSellPrice;//售出价格没有基本价格因为商店不卖
	//BYTE					m_nLevel;	//道具等级
	BYTE					m_nNeedLevel;//需要等级

	BYTE					m_nAttrCount;//属性总数
	union
	{
		_ITEM_ATTR			m_pWuSoulBeadAttr[MAX_WU_SOUL_BEAD_ATTR];	//武魂珠属性
		_ITEM_ATTR			m_pQiSoulBeadAttr[MAX_QI_SOUL_BEAD_ATTR];	//器魂珠属性
	};

	VOID	CleanUp()
	{
		memset(this, 0, sizeof(SOUL_BEAD_INFO));
	}
};
// end of add
//-------------------------------------------------------------------------------------
enum	VAR_ATT
{
	ATT_CUR_DUR,
	ATT_CUR_DAMAGE,
};

//为了减少装备运行数据目前对装备系统数据结构进行简化
//简化方案
//1 将 EQUIP_INFO 中固定属性，退化为通过查表获取。

struct EQUIP_INFO 
{
	CHAR					m_RulerID;									//规则编号
	UINT					m_BasePrice;								//售出价格
	INT						m_MaxDurPoint;								//最大耐久
	BYTE                    m_BaseGemHole;								//天然孔
	//////////////////////////////////////////////////////////////////////////
	//以上为固定属性
	BYTE					m_Bind;										//二级邦定
	BYTE					m_PaiType;									//装备牌型
	UINT					m_EquipScore;								//装备得分
	BYTE					m_EquipType;								//装备类型
	BYTE					m_AttrCount;								//属性的数量
	_ITEM_ATTR				m_pAttr[MAX_ITEM_ATTR];						//物品属性
	BYTE					m_NeedLevel;								//需要等级
	//INT						m_nAttSet;									//属性组
	BYTE					m_Level;									//星级
	INT						m_CurMaxDur;								//当前最大耐久
	INT						m_CurDurPoint;								//当前耐久值
	USHORT					m_CurDamagePoint;							//当前损伤度
	BYTE					m_CurGemHole;								//当前宝石孔
	BYTE					m_StoneCount;								//镶嵌宝石的数量
	_ITEM_GEMINFO			m_pGemInfo[MAX_ITEM_GEM] ;					//物品里镶嵌的宝石
	INT                     m_nAttSet;
	
	BYTE					m_SoulBeadType;							//附魔的魂珠类型add by gh for Soul_bead 2010/06/30
	BYTE					m_SoulBeadAttrCount;						//魂珠属性的数量
	union
	{
		_ITEM_ATTR			m_pWuSoulBeadAttr[MAX_WU_SOUL_BEAD_ATTR];	//武魂珠属性
		_ITEM_ATTR			m_pQiSoulBeadAttr[MAX_QI_SOUL_BEAD_ATTR];	//器魂珠属性
	};

	VOID CleanUp()
	{
		memset(this,0,sizeof(EQUIP_INFO));
	};
};

// [7/8/2010 陈军龙]
struct MOUNT_INFO
{
	VOID CleanUp()
	{
		memset(this,0,sizeof(MOUNT_INFO));
	};

	UINT             m_nNeedLevel;     // 需要达到的等级
	INT		         m_nAddtionSpeed;  // 增加的速度
	INT              m_nLife;          // 存在时限（单位：天）
	INT              m_nBindType;      // 绑定类型 MOUNT_BIND_TYPE
	INT              m_nPrice;         // 所需金钱

	BOOL             m_bIsBinded;      // 当前是否已绑定
	UINT             m_nObtainDate;    // 获取坐骑时的时间
	UINT             m_nRemainTime;    // 坐骑剩余使用时间
};

//-------------------------------------------------------------------------------------

#define MAX_ITEM_LIST_COUNT			8
#define MAX_ITEM_STRING_LENGTH		255
struct _ITEM_LIST 
{
	INT				m_ListCount;
	_ITEM_TYPE		m_ListType[MAX_ITEM_LIST_COUNT];
	INT				m_TypeCount[MAX_ITEM_LIST_COUNT];

	_ITEM_LIST()
	{
		memset(this,0,sizeof(_ITEM_LIST));
	}
	
	VOID	AddType(_ITEM_TYPE it,INT Count)
	{
		Assert(Count>0);
		Assert(!it.isNull());
		
		BOOL bFindSame = FALSE;
		for(INT i=0;i<m_ListCount;i++)
		{
			if(m_ListType[i]==it)
			{
				m_TypeCount[i]+=Count;
				bFindSame = TRUE;
				break;
			}
		}
		if(m_ListCount>=MAX_ITEM_LIST_COUNT)
		{
			AssertEx(FALSE,"添加物品个数太多!");
		}
		if(!bFindSame)
		{
			m_TypeCount[m_ListCount] = Count;
			m_ListType[m_ListCount]  = it;	
			m_ListCount++;
		}
	}
	
	VOID Init()
	{
		memset(this,0,sizeof(_ITEM_LIST));
	}
};


//物品信息, 包含各种物品的所有数据
#define	ITEMISVALID(ITEM) (ITEM.m_ItemIndex != 0 )//物品是否合法
#define	ITEMPTRISVALID(PITEM) ( (PITEM == NULL) ? FALSE:PITEM->m_ItemIndex )//物品指针是否合法
#define	ITEMREFPTRISVALID(PITEM) ( (PITEM == NULL) ? FALSE:PITEM->GetItemTableIndex() )//物品指针是否合法

#define MAX_FIX_ATTR_LENGTH		1000
#define MAX_PET_SKILL_LENGTH	50
#define MAX_ITEM_CREATOR_NAME	13
#define MAX_ITEM_PARAM			3


enum ItemParamValue
{
	IPV_CHAR	=	0,
	IPV_SHORT	=	1,
	IPV_INT		=	2	
};

struct _ITEM
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
	CHAR				m_nsBind;
	CHAR				m_Creator[MAX_ITEM_CREATOR_NAME];
	INT					m_Param[MAX_ITEM_PARAM];// 对于捜侠录类的特殊装备0代表该捜侠录的索引,1代表该捜侠录占的位置如果1位的值是0则没有占 2搜侠记忆棒的内容（类型ID）或者残页记录的类型
	union 
	{
		EQUIP_INFO			m_Equip;
		GEM_INFO			m_Gem;
		MEDIC_INFO			m_Medic;
		EXTRABAG_INFO		m_ExtraBag;
		STORE_MAP_INFO		m_StoreMap;
		SOUL_BEAD_INFO		m_SoulBead;//魂珠类型
		MOUNT_INFO          m_Mount;   //坐骑        [7/8/2010 陈军龙]
	};

	_ITEM( )
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset( this, 0, sizeof(*this) ) ;
	}
	
	BYTE		ItemClass( )	const
	{
			return GetSerialClass(m_ItemIndex);
	};

	BYTE		ItemType()		const
	{
		return GetSerialType(m_ItemIndex);
	}

	BYTE		GetIndex()	const
	{
		return GetSerialIndex(m_ItemIndex);
	}

	EQUIP_INFO*	   GetEquipData() const
	{
		return	(EQUIP_INFO*)(&m_Equip);
	}

	GEM_INFO*	  GetGemData()	const
	{
		return	(GEM_INFO*)(&m_Gem);
	}

	MEDIC_INFO*		GetMedicData()	const
	{
		return (MEDIC_INFO*)(&m_Medic);
	}
	
	EXTRABAG_INFO*	GetExtraBagData()	const
	{
		return (EXTRABAG_INFO*)(&m_ExtraBag);
	}

	STORE_MAP_INFO*	GetStoreMapData()	const
	{
		return (STORE_MAP_INFO*)(&m_StoreMap);
	}
	// add by gh 2010/06/28
	SOUL_BEAD_INFO*	   GetSoulBeadData() const
	{
		return	(SOUL_BEAD_INFO*)(&m_SoulBead);
	}

    // [7/8/2010 陈军龙]
	MOUNT_INFO*	   GetMountData() const
	{
		return	(MOUNT_INFO*)(&m_Mount);
	}

	//////////////////////////////////////////////////////////////////////////
	// 物品个数方法
	// 能叠放物品		返回 实际大小
	// 不能叠放物品		返回 1
	//
	BYTE	GetItemCount()		const;

	VOID	SetItemCount(INT nCount);

	BYTE	GetItemTileMax()	const;
	
	BOOL	isFullTile()		const;


	//增加叠放数量
	BOOL	IncCount(UINT nCount =1);	
	//减少叠放数量
	BOOL	DecCount(UINT nCount =1);


	BOOL	IsNullType()	const
	{
		return m_ItemIndex == 0;
	}

	VOID	ReadItemVarAttr(SocketInputStream& iStream);
	VOID	WriteItemVarAttr(SocketOutputStream& oStream) const;
	
	//VOID	ReadNewItemVarAttr(SocketInputStream& iStream);
	//VOID	WriteNewItemVarAttr(SocketOutputStream& oStream) const;

	INT		WriteFixAttr(CHAR* pIn,INT BuffLength);
	BOOL	ReadFixAttr(CHAR* pOut,INT& OutLength,INT BuffLength);

	INT		WriteVarAttr(CHAR* pIn,INT BuffLength);
	BOOL	ReadVarAttr (CHAR* pOut,INT& OutLength,INT BuffLength);

	BOOL	DBWriteAttr(CHAR* pIn,INT BuffLength);
	BOOL	DBReadAttr (CHAR* pOut,INT& OutLength,INT BuffLength);

	BOOL	IsFixAttr(BYTE attrType);
	BYTE	GetFixAttrCount();
		
	BOOL	GetItemBind() const;
	VOID	SetItemBind(BOOL bBind);

	BOOL	GetItemIdent() const;
	VOID	SetItemIdent(BOOL bIdent);

	BOOL	GetItemPLock() const;	//密码锁定
	VOID	SetItemPLock(BOOL bIdent);
	
	//创建者
	BOOL	GetCreatorVar() const;
	VOID	SetCreatorVar(BOOL bCreator);

	INT		GetItemParamValue(UINT Start,ItemParamValue& ipv);
	VOID	SetItemParam(UINT start,ItemParamValue& ipv,INT value);
};

//pItem			需要转换的_ITEM
//pOut			转换后输出的数据
//OutLength		输出数据的长度
//BuffLength	pOut 的实际长度
BOOL		Item2String(_ITEM* pItem,CHAR* pOut,INT& OutLength,INT BuffLength);
BOOL		String2Item(const CHAR* pIn,INT BuffLength,_ITEM* pItem);


//A,B 交换
VOID	SwapItem(_ITEM* pItemA,_ITEM* pItemB);
//A 覆盖B 空间
//同时A 清空
VOID	OverWriteItem(_ITEM* pItemA,_ITEM* pItemB);

UINT    GetItemVarSize(const _ITEM& it);

UINT    GetNewItemVarSize(const _ITEM& it);


struct  ItemBoxContaner
{

	ItemBoxContaner(){
		memset(m_Item,0,sizeof(_ITEM)*MAX_BOXITEM_NUMBER);
		memset(m_nQuality,0,sizeof(INT)*MAX_BOXITEM_NUMBER);
		m_nCount = 0;
	}

	VOID	AddItemType(_ITEM_TYPE& it,INT iQuality)
	{
		m_Item[m_nCount].m_ItemIndex = it.ToSerial();
		m_nQuality[m_nCount]	=	iQuality;
		m_nCount++;
	}
	_ITEM				m_Item[MAX_BOXITEM_NUMBER]; 
	INT					m_nQuality[MAX_BOXITEM_NUMBER];
	INT					m_nCount;
	UINT				m_uDropType;
};

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

#endif
