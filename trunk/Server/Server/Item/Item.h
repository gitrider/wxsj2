/////////////////////////////////////////////////////////////////////////////////
//文件名：Item.h
//功能描述：物品数据的封装类，通过此类来读取及设置一个物品的信息
//开发者：jack
//修改记录：2005-11-2创建
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __ITEM_H__
#define __ITEM_H__

#include "Type.h"

struct _PET_DB_LOAD;
struct  PET_GUID_t;
struct _PET_SKILL;


struct _CREATEITEM_INIT
{
	UINT					m_nItemIndex ;//
	BYTE					m_nEquipFromType;
};

struct _HUMAN_EQUIPCREATE_INIT: 
	public _CREATEITEM_INIT
{
	UINT					m_nEquipScore;
	UINT					m_nPlayerJob;
};

struct _HUMAN_EQUIPDROP_INIT: 
	public _CREATEITEM_INIT
{
	UINT					m_nMonsterLevel;
	UINT					m_nMonsterType;
};

class _ITEM_INIT
{
public:
	_ITEM_INIT(const _ITEM*	pItem)
	{
		m_pItem = pItem ;
		m_pPet  = NULL;
	}

	_ITEM_INIT(const _PET_DB_LOAD* pPet)
	{
		m_pPet	= pPet;
		m_pItem = NULL;
	}
	const	_ITEM*				m_pItem ;//物品详细数据
	const	_PET_DB_LOAD*		m_pPet;	 //宠物详细数据
};

enum ITEM_DATA_TYPE
{
	IDT_ITEM,
	IDT_PET,
};

struct _CAMP_DATA;
class Item
{
friend class ItemOperator ;
friend class ItemContainer ;
friend struct Obj_ItemContaner;
friend class Obj_ItemManager;

public :
	Item( ) ;
	~Item( ) ;

	ITEM_DATA_TYPE			GetDataType(){ return m_Type;}//取得数据类型

	VOID					SaveValueTo(_ITEM* pItem);
	VOID					SaveValueTo(_PET_DB_LOAD* pPet);
	inline  _ITEM_GUID		GetGUID( ) ;					//取得物品GUID
	inline  UINT			GetItemTableIndex( ) const; 	//取得物品表里的物品索引编号
	inline	UCHAR			GetItemClass()		 const;		//取得物品的	enum ITEM_CLASS
	
	inline	UCHAR			GetItemType()		 const;		//取得物品的小类WEAPON_TYPE DEFENCE_TYPE 等
	inline	UCHAR			GetItemIndex()		 const;		//取得物品的流水号

	BOOL					IsCanEquipBind()	const;		//取得物品是否具有绑定属性
	BOOL					IsRuler(ITEM_RULER_LIST	ruler)	const;			//取得物品是否可以执行指定规则
	BOOL					IsBind()			const;		//查询是否绑定
	
	UINT					GetPrice()	const;
	UINT					GetSellPrice() const;
	INT						GetItemParam(UINT start,ItemParamValue& ipv);

	BOOL					GetItemTransferString(CHAR* pOutBuf, UINT nOutBufLength);		//取得物品传输系统用物品字串	20100706 BLL
	BOOL					GetPetTransferString(CHAR* pOutBuf, UINT nOutBufLength);		//取得物品传输系统用珍兽字串	20100706 BLL

	/*=============================================
	 *	装备属性
	 =============================================*/
	inline _ITEM*			GetItemData()				const;			//获得装备的所有属性
	inline INT				GetSetNum()					const;			//套装编号
	inline BYTE				GetMaxSetCount()			const;			//套装个数
	inline BYTE				GetEquipPoint()				const;			//物品装佩点
	inline INT				GetMaxDurPoint()			const ;			//最大耐久值
	inline BYTE				GetRequireLevel()			const;			//需求等级
	inline BYTE				GetCurLevel()				const;			//需求等级
	inline BYTE				GetGemSlotMax()				const;			//最大宝石个数
	inline BYTE				GetModelID()				const;			//模型
	inline INT				GetDurPoints()				const; 			//当前耐久值
	inline INT				GetCurMaxDurPoint()			const; 			//当前耐久上限
	inline BYTE				GetAttrCount()				const;			//属性的数量
	inline USHORT			GetDamagePoint()			const;			//物品损伤累计值
	inline BOOL				GetItemBind()				const;			//物品是否绑定
	inline BOOL				GetItemIdent()				const;			//物品是否鉴定
	inline BYTE				GetEquipGemCount()			const;			//镶嵌宝石的数量
	inline BYTE				GetAffixSkillCount()		const;			//获得附加技能个数
	inline BYTE				GetEffectSkillCount()		const;			//获得装备影响技能个数
	inline _ITEM_EFFECT_SKILL*	GetEffectSkill(UINT pos)		const;			//获得装备影响技能个数
	inline BOOL				IsWuMenPai()				const;			//获得装备是否无门派需求
	inline BOOL				InReqJob(MENPAI_ATTRIBUTE menpaiID)const;	//指定的门派是否与装备的门派需求一致
	inline BYTE				GetCurGemHole()				const;			//
	inline BOOL				AddHole();									//装备加孔
	inline BOOL				EquipLevelup(INT nLevel);								//装备升级
	inline VOID				EquipBind(UINT nBindLevel);					//装备绑定
	inline BOOL				AddGem(INT nHolePos, INT GemId);			//加宝石
	inline BYTE				GetEquipBindLevel();						//装备绑定
	inline VOID				UpDataEquipGem()			const;			//更新激活属性
	inline UINT				GetEquipGem(INT nPos)		const;			//镶嵌的宝石
	
	inline UINT				GetEquipScore()				const;			//获得装备分
	inline BYTE				GetEquipFromType()		    const;			//获得装备来源类型

	inline VOID				SetEquipScore(UINT nScore)	const;			//装备分
	inline VOID				SetEquipFromType(BYTE nType)const;			//装备来源类型

	inline const _ITEM_ATTR		GetEquipAttr(UCHAR	nIndex)		const;		//获得装备属性
	inline VOID					SetEquipAttr(UCHAR	nIndex, _ITEM_ATTR ItemAttr);
	inline const _ITEM_GEMINFO	GetEquipGemInfo(UCHAR nIndex)	const;		//获得装备上宝石的编号信息

	inline BYTE				GetEquipPaiType()		    const;			//获得装备牌型
	inline VOID				SetEquipPaiType(BYTE nPaiType);
	inline BOOL				IsAdsorbMagic()				const;			//装备是否附魔
	inline BOOL				AdsorbMagic(_ITEM* Bead, BYTE Kind );// 装备曾加魂珠的属性
	inline BOOL				CancelMagic();						 // 装备除去附魔的属性
	/*==========================================
	 *	普通物品属性
	 ============================================*/		
	inline UINT				GetItemSellPrice()		const;				
	inline BYTE				GetItemLevel()			const;				//物品级别,策划需求
	inline BYTE				GetReqSkillLevel()		const;				//获得技能级别需求
	inline INT				GetReqSkill()			const;				//需要拥有的技能号
	inline INT				GetScriptID()			const;				//获得相关脚本编号
	inline INT				GetSkillID()			const;				//获得相关技能编号
	inline BYTE				GetTargetType()			const;
	inline BOOL				IsBroadCast()			const;				//
	/*==============================================
	 *宝石属性	
	 ===============================================*/
	inline const _ITEM_ATTR		GetGemAttr(UINT i)			const;				//宝石属性
	
	inline UINT				GetGemReqNum()			const;					//前置镶嵌的宝石

	inline const _GEM_REQ_ATT			GetReqGemAtt(INT nPos)	const;
	inline UINT				GetReqEquip(INT nEquipType)const;				//装备需求
	/*==============================================
	*额外包裹属性	
	===============================================*/
	inline UINT				GetExtraBagSellPrice()		const;				
	inline UINT				GetExtraBagBasePrice()		const;			
	inline UINT				GetExtraBagValidTime()		const;				//
	inline UINT				GetExtraBagRemainTime()		const;	
	inline BOOL				IsExtraBagCanUse()		const;				//
	inline UINT				GetExtraBagSize()			const;					//
	inline UINT				GetExtraBagBeginTime()		const;	
	inline VOID				SetExtraBagBeginTime();
	/*==========================================
	 *藏宝图属性	
	============================================*/

	inline INT				GetMapLevel()			const;				//藏宝图级别
	inline FLOAT			GetXPos()				const;				//x坐标
	inline FLOAT			GetZPos()				const;				//z坐标
	inline INT				GetSceneID()			const;				//场景编号
	inline INT				GetGrowType()			const;				//对应生长点类型

	/*==========================================
	 *魂珠属性	add by gh 2010/06/28
	============================================*/
	VOID					SetWuSoulBeadAttr(const BYTE attrCount, const _ITEM_ATTR* tmpItemAttr);//设置武魂珠所有属性
	VOID					SetQiSoulBeadAttr(const BYTE attrCount, const _ITEM_ATTR* tmpItemAttr);//设置器魂珠所有属性
	inline VOID				SetSoulBeadPrice(UINT price);	//设置魂珠卖价属性
	inline VOID				SetSoulBeadNeedLevel(BYTE needLevel);//设置魂珠需求等级
	inline UINT				GetSoulBeadPrice()		const;	//获得魂珠卖价
	inline BYTE				GetSoulBeadNeedLevel()	const;	//获得魂珠需求等级
	
	inline BYTE				GetSoulBeadAttrCount()	const;			//魂珠属性的数量
	inline const _ITEM_ATTR		GetWuSoulBeadAttr(UCHAR	nIndex)		const;		//获得武魂珠属性
	inline const _ITEM_ATTR		GetQiSoulBeadAttr(UCHAR	nIndex)		const;		//获得器魂珠属性

private:

	inline VOID				SetWuSoulBeadAttr(const _ITEM_ATTR&  ItemAttr);
	inline VOID				SetQiSoulBeadAttr(const _ITEM_ATTR&  ItemAttr);

public:
	//ITEM 和PET 公用函数
	
	BOOL					Init( const _ITEM_INIT* pInit );//初始化物品
	BOOL					IsEmpty( ) ;					//判断物品是否为空，即没有物品数据
	BOOL					IsEquip( ) const;				//判断此物品是否为一件装备
	BOOL					IsLock( ){ return m_bLock ; } ; //判断此物品是否被锁定
	BOOL					IsCanLay( )			const;		//取得物品是否可以叠放
	inline	INT				GetLayedNum( )  const;			//取得叠放物品的数量
	inline	INT				GetMaxLayedNum( ) const;		//取得能够叠放物品的最大数量
	inline  BOOL			IsFull()	const;				//是否已经达到最大叠加数目

	inline  BOOL			IsPWLock()	const;				//受密码保护
public:
	//宠物函数

	inline  PET_GUID_t			GetPetGUID( ) ;								//取得宠物GUID
	inline  PET_GUID_t			GetSpouseGUID( ) ;							//取得宠物配偶GUID
	inline	INT					GetDataID();								//取得宠物模型		
	inline  const CHAR*			GetName()	const;							//取得名字
	inline  const CHAR*			GetNick() const;							//取得昵称
	inline  INT					GetLevel() const;							//取得等级
	inline  INT					GetTakeLevel() const;						//取得携带等级
	inline  INT					GetAttackType() const;						//取得进攻类型（物/法）
	inline  INT					GetAIType() const;							//取得AI类型
	inline  const _CAMP_DATA	*GetCampData() const;						//取得阵营
	inline  INT					GetHP() const{return 0;};					//取得生命值
	inline  INT					GetBaseMaxHP() const{return 0;};			//取得生命值上限
	inline  INT					GetLife() const {return 0;};				//取得当前寿命
	inline  BYTE				GetPetType() const;							//宝宝，变异，野生
	inline  BYTE				GetGeneration() const;						//几代宠
	inline  BYTE				GetHappiness() const;						//快乐度		
	//资质相关
	inline  INT					GetStrengthPer() const;						// 力量资质	
	inline  INT					GetSmartnessPer() const;					// 敏捷资质	
	inline  INT 				GetMindPer() const;							// 智力资质
	inline  INT 				GetConstitutionPer() const;					// 体质资质
	//养成属性相关
	inline	INT					GetGenGu() const;							//根骨
	inline  FLOAT				GetGrowRate() const ;						//成长率
	inline  INT					GetSavvy() const;							//悟性
	inline  INT					IsCheckup();								//是否已鉴定
	inline  INT					GetRemainPoint() const;						//一级属性剩余点数
	inline  INT					GetExp() const;								//经验值
	inline  INT					GetLvl1Attr(CHAR_ATTR_LEVEL1 type)const;	// 基础一级战斗属性（不包括技能和装备增加的部分）
	inline  _PET_SKILL			GetSkill(UINT SkillIndex);					// 宠物技能
	inline  VOID				SetCooldown(CooldownID_t nID, Time_t nTime);
	inline	BOOL				IsCooldowned(CooldownID_t nID) const;
	inline	VOID				HeartBeat_Cooldown(Time_t nDeltaTime);
	inline	Time_t				GetCooldownRemained(CooldownID_t nID) const;
	inline	BOOL				IsMating() const;
	inline  INT					GetSex() const;
	/*==============================================
	*功能接口
	===============================================*/
	//交易
	BOOL						IsInExchange(){return m_bIsInExchange;}
	VOID						SetInExchange(BOOL bIsInExchange){m_bIsInExchange = bIsInExchange;}

protected :
	VOID					Lock( ){ m_bLock = TRUE ; }
	VOID					Unlock( ){ m_bLock = FALSE ; }
	VOID					SetItemValue(_ITEM* pItem);
	VOID					CleanUp( ) ;//清除物品数据

private :
	ITEM_DATA_TYPE			m_Type;
	_ITEM*					m_pItem ;					//物品详细数据
	_PET_DB_LOAD*			m_pPet;						//
	BOOL					m_bLock ;
	BOOL					m_bIsInExchange;			//标记此物品正在被放到交易栏中
};

#include "Item.inl"

struct _EXCHANGE_ITEM_LIST 
{
	INT				m_ListCount;
	Item*			m_ListItem[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];

	_EXCHANGE_ITEM_LIST()
	{
		memset(this,0,sizeof(_EXCHANGE_ITEM_LIST));
	}

	VOID	AddItem(Item* it)
	{
		INT i;

		for(i=0;i<EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE;i++)
		{
			if(m_ListItem[i] == NULL)
			{
				break;
			}
		}

		if(i == EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE)
		{
			Assert(0);
			return;
		}

		m_ListCount++;
		m_ListItem[i] = it;
	}

	VOID Init()
	{
		memset(this,0,sizeof(_EXCHANGE_ITEM_LIST));
	}
};


struct  Obj_ItemContaner
{
	Obj_ItemContaner(){
		CleanUp();
	}
	
	VOID    CleanUp()
	{
		memset(m_Item,0,sizeof(_ITEM)*MAX_DROP_ITEM_NUMBER);
		m_nCount = 0;
	}

	VOID	AddItem(_ITEM& item)
	{
		Assert(!item.IsNullType());
		memcpy(&m_Item[m_nCount], &item, sizeof(_ITEM));
		m_nCount++;
	}
	_ITEM				m_Item[MAX_DROP_ITEM_NUMBER]; 
	INT					m_nCount;
	UINT				m_uDropType;
};
#endif
