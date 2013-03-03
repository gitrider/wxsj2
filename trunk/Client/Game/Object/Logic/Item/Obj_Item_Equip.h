
#pragma once


#include "Obj_Item.h"
#include "GameDefine2.h"
#include "Obj_Item_Equip.h"
#include "GIDBC_Struct.h"
#include "../../ObjectDef.h"
#include "GameStruct_Item.h"


/*
|
|	装备类物品
|
*/


class CObject_Item_Equip : public CObject_Item
{
public:
	//得到物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const;
	//物品的名称
	virtual LPCTSTR			GetName(VOID) const;
	//物品解释
	virtual LPCTSTR			GetDesc(VOID) const;
	//图标名
	virtual LPCTSTR			GetIconName(VOID) const;
	//得到装备牌型
	INT                     GetEquipCard(VOID) ;

	// 交换装备牌型
	BOOL					SwapEquipCard(CObject_Item_Equip &ep);

	//设置装备牌型
	VOID					SetEquipCard( INT setNum );  
	//详细解释
	virtual LPCTSTR			GetExtraDesc(VOID);
	//得到星级
	INT						GetStartLevel(VOID) const;	
	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn );
	virtual LPCTSTR	GetExtranInfo();
	//得到装备点
	HUMAN_EQUIP				GetItemType(VOID)const ;

	INT						GetEquipScore();		

	//得到装备是否被附魔
	BOOL		GetIsFumo();


	/** 
	 *	得到显示数据ID ====== 高级
	 */
	

	virtual INT				GetVisualID(VOID)	const;

	//得到显示数据ID 掉落时的外形
	virtual LPCTSTR			GetDropVisualID(VOID)	const;

	//显示名称颜色
	virtual LPCTSTR			GetDropVisColor(VOID) const ;

	//查询逻辑属性,一般用于脚本调用
	virtual STRING			GetAttributeValue(LPCTSTR szValueName);

	//克隆详细信息
	virtual VOID			Clone(const CObject_Item * pItemSource);
	//得到套装编号
	INT						GetSetID(VOID) const;
	//得到武器类型(如果是非武器，返回WEAPON_TYPE_NONE)
	ENUM_WEAPON_TYPE		GetWeaponType(VOID) const;
	//设置装备当前耐久度对于一个已经生成的装备，只有这个值会被改变
	VOID					SetDur(INT Dur);
	//获得当前耐久度
	INT						GetDur(VOID) const;
	//设置装备当前最大耐久度
	VOID					SetMaxDur(INT Dur);
	//获得当前最大耐久度
	INT						GetMaxDur(VOID) const;
	//获得最大耐久度
	INT						GetMaxDurFromTable(VOID) const;
	
	//克隆详细信息
	VOID					ClonetExtraInfo(CObject_Item_Equip * pItemSource);
	//获得详细信息
	VOID*					GetExtraInfo() const{return m_pExtraDefine;}

	//得到装备是普通的还是高级的
	INT                     GetItemEquipQua(){ return m_eEquipStyle;}

    //获得装备特效
	 const  EQUIP_EFFECT*            GetEffect();	 


	//--------------------------------------------------------------------------------------------------------------
	//-- supertooltip 使用
	//

	// 得到玩家使用这个物品需要的等级
	virtual INT				GetNeedLevel();

	// 得到物品耐久度
	virtual INT				GetItemDur();

	// 得到物品最大耐久度
	virtual INT				GetItemMaxDur();

	// 得到物品的绑定信息 
	virtual INT				GetItemBindInfo();
	// 得到物品的2级绑定信息 
	virtual INT				GetItemSecLevelBindInfo();


	//得到装备牌型
	VOID					GetEquipCardForTip(STRING& strValue) ;
	//绑定加成信息
	virtual LPCTSTR			GetItemBindAddInfo();

	// 得到物品卖给npc的价格
	virtual INT				GetItemPrice();

	//得到物品职业
	virtual INT             GetItemProfession();

	//得到装备目前打孔的数目
	INT						GetEquipHoleNum();

	//得到装备目前最大孔的数目
	INT						GetEquipHoleMaxNum();

	//得到装备牌型激活的信息
	LPCSTR						GetEquipSpritePaiInfo();

	//得到其他玩家装备小激活的信息
	LPCSTR						GetOtherEquipSpriteChainInfo();
	//得到装备小激活的信息
	LPCSTR						GetEquipSpriteChainInfo();

	//得到其他玩家装备大激活的信息
	LPCSTR						GetOtherEquipSpriteAllActiveInfo();
	//得到装备大激活的信息
	LPCSTR						GetEquipSpriteAllActiveInfo();

	//得到装备套装信息
	LPCTSTR					GetEquipSetInfo();
	//得到装备套装信息
	LPCTSTR					GetOtherEquipSetInfo();

	//得到装备套装激活的属性信息
	LPCTSTR					GetEquipSetActiveAttr();
	//得到装备套装激活的属性信息
	LPCTSTR					GetOtherEquipSetActiveAttr();

	//设置装备目前打孔的数目

	VOID					SetEquipHoleNum( INT num);
	// 得到物品的制作人
	virtual LPCTSTR			GetManufacturer();

	// 得到装备基本属性
	LPCTSTR                 GetEquipBaseAttrInfo();
	 
	// 得到装备附加属性
	LPCTSTR			        GetEquipAddAttrInfo();


	// 得到装备技能属性
	LPCTSTR			        GetEquipSkillInfo();

	// 得到物品的类型描述 
	virtual LPCTSTR			GetItemTableTypeDesc();

	// 格式化属性字符串
	void					SetAttri(const _ITEM_ATTR& att, char* const pszBuf);

	
	// 得到宝石的个数.
	INT						GetGemMaxCount(VOID) const;

	//得到宝石 
	BOOL					GetEquipGem( INT  index ,_ITEM_GEMINFO &gem);

	// 得到宝石的图标
	VOID					GetGemIcon(INT iGemIndex, STRING& strIconName);

	// 得到宝石的附加功能
	VOID					GetGemExtAttr(INT iGemIndex, STRING& strGemAttr);

/*
	CObject_Item*			GetAppendItem(INT nIndex) const;

	VOID					SetAppendItem(CObject_Item* pItem,INT nIndex);
*/


public:

	//属性和宝石
	typedef std::vector< _ITEM_ATTR >		EQUIP_ATTRIBUTES;
	typedef std::vector< _ITEM_GEMINFO >	EQUIP_ATTACH_GEM;

	//附加技能和影响技能
	typedef std::vector< _ITEM_AFFIX_SKILL >  EQUIP_AFFIX_SKILL_VEC;  
	typedef std::vector< _ITEM_EFFECT_SKILL > EQUIP_EFFECT_SKILL_VEC;

public:
	CObject_Item_Equip(INT id);
	virtual ~CObject_Item_Equip();


	//生成一件装备
	VOID		AsEquip( const  _DBC_ITEM_EQUIP* pDefine);

   //高级装备
	VOID		AsAdvEquip( const _DBC_ITEM_EQUIP* pDefine ); 


   //所有装备基本属性 

	VOID		GetAllEquipBaseAttribute(STRING& strValue);
	//所有装备属性 

	VOID		GetAllEquipAddAttribute(STRING& strValue);

	//所有装备技能属性 

	VOID		GetAllEquipSkillAttr(STRING& strValue);

	//得到镶嵌的宝石个数.
	INT 		GetGemCount();

	// 得到物品表中type一列 
	virtual INT	GetItemTableType(VOID);


public:

	//--------------------------------------------------------------
	

	struct   Base_Attr
	{

		INT  m_nBase_Atack_Near;
		INT  m_nBase_Atack_Far;
		INT  m_nBase_Atack_Magic;

		INT  m_nBase_Def_Near;
		INT  m_nBase_Def_Far;
		INT  m_nBase_Def_Magic;

		Base_Attr()
		{
			m_nBase_Atack_Near = -1;
			m_nBase_Atack_Far  = -1;
			m_nBase_Atack_Magic = -1;

			m_nBase_Def_Near  = -1;
			m_nBase_Def_Far   = -1;
			m_nBase_Def_Magic = -1;

		}

		~Base_Attr()
		{
			m_nBase_Atack_Near = -1;
			m_nBase_Atack_Far  = -1;
			m_nBase_Atack_Magic = -1;

			m_nBase_Def_Near  = -1;
			m_nBase_Def_Far   = -1;
			m_nBase_Def_Magic = -1;

		}
	};

	//基本属性定义
	struct BASE_DEFINE
	{
		//装备数据表
		const	 _DBC_ITEM_EQUIP*          pDefineEquip	; 
		//union
		//{   //高级装备外形
		//	const _DBC_ITEM_ADVANCED_EQUIP_VISUAL*      pAdvVisual;
		//	//普通装备外形
		//	const _DBC_ITEM_COMMON_EQUIP_VISUAL*		pCommonVisual;
		//};
		//装备外形
		const _DBC_ITEM_EQUIP_VISUAL*      pEquipVisual ;

		Base_Attr			m_Base;
		
		BASE_DEFINE()
		{
			pEquipVisual = NULL;
			pDefineEquip = NULL;
		}

		~BASE_DEFINE()
		{
			pEquipVisual = NULL;
			pDefineEquip = NULL;
		}

		
	};



	//  区分普通和高级装备
	enum   EQUIP_ENUM
	{
		 INVAILD_EQUIP = -1,	//无效
		 COMMON_EQUIP ,			//普通 
		 ADVANCED_EQUIP,		//高级
			
	};

	//  区分普通和高级装备
	enum   EQUIP_ENUM_STYLE
	{
		INVAILD_EQUIP_STYLE = -1,	//无效
		IB_EQUIP_STYLE ,			//ib 
		MANUFACTURE_EQUIP_STYLE,	//玩家制作的装备


	};

 

	enum EQUIP_BIND_INFO
	{
		FREE_BIND = 0,			// 无绑定限制
		BINDED,					// 已经绑定
		GETUP_BIND,				// 拾取绑定
		EQUIP_BIND,				// 装备绑定  
	 	EQUIP_BIND_TWO  = 1,        // 使用材料进行的绑定，目前为2级绑定 ，以后可能有更高级的绑定    
	};
	//装备的type列
	enum EQUIP_ID_TYPE
	{
		EQ_TY_WEAPON  = 0,  //武器
		EQ_TY_WEAPON_MAXNUM =6 , //武器上限
		EQ_TY_DEFENCE = 7,	//防具 盾牌
		EQ_TY_RING = 28,	//戒指
        EQ_TY_ADORN = 29	//饰品

	};


	//扩展属性定义
	struct EXTRA_DEFINE
	{
		//---接收服务器参数
		 
		INT					m_nAttSet;					//属性组
		BYTE			    m_StartLevel;				//星级
		INT					m_CurDurPoint;				//当前耐久值                 
		USHORT				m_CurDamagePoint;		    //当前损伤度
		BYTE				m_GemCount;					//镶嵌的宝石个数
		EQUIP_BIND_INFO		m_nEquipBindInfo;			//装备是否绑定
		EQUIP_BIND_INFO     m_nEquipBindInfo2;          // 装备是否强化绑定（2级绑定）
		
		BYTE								m_nEquipAttachGemNum;	//附加的宝石的数量
		EQUIP_ATTACH_GEM	m_vEquipAttachGem;			//附加的宝石
		INT					m_nEquipHoleNum;			//装备已经有的孔洞的数目

		INT					m_nEquipScore;				//装备的分数（该分数大于某值后装备产生灵魂属性以及区分装备质量）
	
       //--------------------------------------------------------------

		BYTE							m_nEquipAttributesNum;		//装备的属性的数量
		EQUIP_ATTRIBUTES	m_vEquipAttributes;			//装备属性
		
		INT					m_nSellPrice;				//售出价格
		INT					m_nLevelNeed;				//需求等级 
		INT					m_nMaxDurPoint;				//最大耐久值
		INT					m_SetNum;					//套装id

		BYTE					m_nAffix_SkillNum;			//附加的技能的数量
		EQUIP_AFFIX_SKILL_VEC   m_Affix_Skill;			//附加技能
		
		BYTE					m_nEffect_SkillNum;			//影响的技能的数量
		EQUIP_EFFECT_SKILL_VEC  m_Effect_Skill;         //影响技能
		
		BYTE					m_nszAttrGroupNum;			//属性组名的大小
		STRING				m_szAttrGroup;				//属性组名

		CHAR				m_RulerID;            //新规则
		BYTE				m_EquipType;		  //装备类型
		BYTE				m_AttrCount;		  //属性的数量
		INT					m_PaiXing;			  //装备的牌型	

		BYTE					m_SoulBeadType;							//附魔的魂珠类型add by gh for soul_bead 2010/06/30
		BYTE					m_SoulBeadAttrCount;						//魂珠属性的数量

   
		EXTRA_DEFINE()
		{
     		 		
			m_nAttSet = 0;				
			m_StartLevel  = 0;				
			m_CurDurPoint = 0;		
			m_CurDamagePoint = 0;				
			m_GemCount   = 0;		
			m_nEquipBindInfo = FREE_BIND;	
			m_nEquipBindInfo2 =FREE_BIND;

			m_nEquipHoleNum = 0;
			m_nEquipScore  = 0;
			m_nSellPrice = 0;
			m_nLevelNeed = 0;
			m_nMaxDurPoint = 0;
			m_SetNum = -1;
			m_szAttrGroup = "";
			m_RulerID = -1;
			m_EquipType = -1;
			m_AttrCount =0;
			m_PaiXing = -1;

			m_nEquipAttachGemNum = 0;
			m_nEquipAttributesNum = 0;
			m_nAffix_SkillNum = 0;
			m_nEffect_SkillNum = 0;
			m_nszAttrGroupNum = 0;
		}

		INT	WriteVarAttr( CHAR* pIn );
		BOOL	ReadVarAttr( CHAR* pOut, INT& OutLength );
	};


public:

	// 得到装备基本定义
	BASE_DEFINE	GetEquipBaseDefine() { return m_theBaseDef;}

public:

	enum EQUIP_ATTRIB
	{
		EQUIP_ATTRIB_UNIDENTIFY,	// 装备未鉴定。
		EQUIP_ATTRIB_IDENTIFY,		// 装备已鉴定。
	};


	//鉴定
	EQUIP_ATTRIB	m_EquipAttrib;
	EQUIP_ATTRIB	GetEquipAttrib();

	// 是否是武器
	BOOL IsWeapon();

	// 是否是防俱
	BOOL IsDefence();

	// 是否是装饰物
	BOOL IsAppend();

protected:
	
	//区分普通装备和高级装备
     EQUIP_ENUM			m_eEquipStyle;
	 

	//--------------------------------------------------------------
	//基本属性
	BASE_DEFINE			m_theBaseDef;			//基本属性定义(数据表)

	//----------------------------------------------------------------
	//扩展属性(需要服务器传输)
	EXTRA_DEFINE*		m_pExtraDefine;
	STRING				m_strExtraDesc;

//	std::vector< CObject_Item* >	m_vAppendItem;
};


//装备牌型是否将
BOOL					Equip_Card_Is_Jiang( INT  c1 ,INT c2, BOOL isSpecial = FALSE);
//装备牌型是否想等
BOOL					Equip_Card_Is_Equal( INT  c1 ,INT c2,INT c3);
//装备牌型是否连续
BOOL					Equip_Card_Is_Series( INT  c1 ,INT c2,INT c3);	
//返回装备牌型最小的
INT					    Equip_Card_Min( INT  c1 ,INT c2, INT c3);
const  INT  max_sprite_chain_num  =   1000;   //最大的组合id
const  INT  max_sprite_all_active_num  =  2000;   //最大的组合id