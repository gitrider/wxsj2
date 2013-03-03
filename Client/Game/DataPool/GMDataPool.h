
/** 数据池
 *
 *	功能：	
 *
 *	修改记录：
 *			080307	帮会家族信息
 *			080314	Relation_GetFriendByName
 *					Relation_GetBlackByName
 *			080318	Relation_ClearEnemy
 *					Relation_AddEnemy
 *					Relation_DelEnemy
 *					Relation_GetEnemyNum
 *					Relation_SetEnemyNum
 *					Relation_GetEnemy
 *					Relation_GetEnemyByName
 *					m_uEnemyCount
 *					m_EnemyInfo
 *
 *					m_FingerPlayerNum
 *					m_FingerPlayerList
 *			080331	添加称号相关的函数
 *			080409  玩家的套装激活 王振
 *			080429	SystemTip_GetIndexByType
 *			
 */

#pragma once


#include "GIDataPool.h"
#include "GMDP_Struct_Pet.h"
#include "GameStruct_Script.h"
#include "GameStruct_Pet.h"
#include "GMDP_Struct_Guild.h"
#include "GMDP_Struct_RecordList.h"
#include "GMDP_Struct_PlayerShop.h"
#include "GMDP_Struct_Relation.h"
#include "GIDBC_Struct.h"
#include "GMDP_Struct_Title.h"
#include "GMDP_Struct_SystemTip.h"
#include "GMDP_Struct_Country.h"



class CObject;
class CObject_Character;
class CCharacterData;
class CObject_PlayerNPC;
enum HUMAN_EQUIP;
class tObject_Item;
struct _ITEM;
struct _BUFF_IMPACT_INFO;
struct X_PARAM;
struct X_SCRIPT;
struct EVENT;
class Relation;
class CMailPool;
struct _TITLE_;
struct SOUXIA_DATA;


class CDataPool : public tDataPool
{
public:

	static void WINAPI _Skill_OnUpdatePlayerAttrib(const EVENT* pEvent, UINT dwOwnerData);
	
	//---------------------------------------------
	//公共接口
	//---------------------------------------------
public:

	/************************************************************************/
	/*		CopyTextToClipboardd
	/*		将一段文本数据拷贝到剪贴板
	/************************************************************************/
	BOOL CopyTextToClipboard(HWND hWnd,LPCSTR content);

	BOOL CopyTextFromClipboard(HWND hWnd, LPCSTR pBuf );

	/// 商店类型
	enum TYPE_PLAYER_SHOP
	{
		TYPE_PLAYER_SHOP_INVALID = 0,		/// 无效类别，还没有分配出去
		TYPE_PLAYER_SHOP_ITEM,				/// 物品店
		TYPE_PLAYER_SHOP_GEM,				/// 宝石店
		TYPE_PLAYER_SHOP_WEAPON,			/// 武器店
		TYPE_PLAYER_SHOP_ARMOR,				/// 护甲店
		TYPE_PLAYER_SHOP_MATERIAL,			/// 材料店
		TYPE_PLAYER_SHOP_PET,				/// 宠物店
	};

	/// 冷却组
	struct COOLDOWN_GROUP
	{
		INT		nTime;
		INT		nTotalTime;
		INT 	nTimeElapsed;
	};

public:

/** 角色属性创建/销毁
 */
	CCharacterData* CharacterData_Create(CObject_Character* pCharacterData);
	void			CharacterData_Destroy(CObject_Character* pCharacterData);
	CCharacterData*	CharacterData_Get(INT nID);


/** 冷却组
 */

	/** 取得冷却组 */
	virtual const COOLDOWN_GROUP*	CoolDownGroup_Get(INT nCoolDownID) const;
	/** 更新冷却组 */
	virtual VOID					CoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum);
	/** 取得宠物冷却组 */
	virtual const COOLDOWN_GROUP*	PetSkillCoolDownGroup_Get(INT nCoolDownID) const;
	/** 更新宠物冷却组 */
	virtual VOID					PetSkillCoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum,const PET_GUID_t nPet_GUID);


/** 公共冷却组 
 */
	virtual const INT				CommonCoolDown_Get(VOID) const { return m_nCommonCoolDown; }
	virtual VOID					CommonCoolDown_Update(VOID) { m_nCommonCoolDown = COMMONCOOLDOWN_TIME;}
	

/** 任务时间
 */

	/** 取得任务时间 */
	virtual const INT				QuestTimeGroup_Get(INT nQuestTimeID) const;
	/** 设置任务时间 */
	virtual VOID					QuestTimeGroup_UpdateList(const INT nQuestTime, INT nUpdateNum);
	

/** 玩家装备系列
 */
	
	/** 清空 */
	virtual VOID			UserEquip_Clear(VOID);
	/** 设置装备 */
	virtual VOID			UserEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** 取得装备 */
	virtual tObject_Item*	UserEquip_GetItem(HUMAN_EQUIP ptEquip);
	/** 套装是否组合成功,返回套装数量  */
	virtual INT				UserEquip_IsUnion(VOID);

	/**牌型是否组合成功,	局部的激活 */
	virtual BOOL			UserEquipSprite_IsActive( EM_EQUIP_CARD_GROUP index); 

	/**牌型是否组合成功,	大激活 */
	virtual BOOL			UserEquipSprite_IsActiveAll( ); 
	/**牌型是否组合成功,	大激活  判断除了将以外的4组合*/
	virtual BOOL			UserEquipSprite_ActiveFour(INT index ); 


/** 玩家坐骑系列 [7/8/2010 陈军龙] 
*/
	/** 清空 */
	virtual VOID            UserMount_Clear(VOID);
	/** 设置坐骑 */
	virtual VOID            UserMount_SetItem(INT nIndex, tObject_Item* pMountItem, BOOL bClearOld);
	/** 取得坐骑 */
	virtual	tObject_Item*   UserMount_GetItem(INT nIndex);
	/** 取得坐骑位置 */
	virtual INT             UserMount_GetItemIndex(tObject_Item* itemMount);


/** 别人装备系列
 */
	
	/** 清空 */
	virtual VOID			OtherPlayerEquip_Clear(VOID);
	/** 设置装备 */
	virtual VOID			OtherPlayerEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** 取得装备 */
	virtual tObject_Item*	OtherPlayerEquip_GetItem(HUMAN_EQUIP ptEquip);
	/** 套装是否组合成功,返回套装数量  */
	virtual INT				OtherPlayerEquip_IsUnion(VOID);


	/**牌型是否组合成功,	局部的激活 */
	virtual BOOL			OtherUserEquipSprite_IsActive( EM_EQUIP_CARD_GROUP index); 

	/**牌型是否组合成功,	大激活 */
	virtual BOOL			OtherUserEquipSprite_IsActiveAll( ); 

	/**牌型是否组合成功,	大激活  判断除了将以外的4组合*/
	virtual BOOL			OtherUserEquipSprite_ActiveFour(INT index ); 



/** 搜侠录道具
/
	/** 清空 */
	virtual VOID			UserSouXia_Clear(VOID);
	
	/** 设置搜侠录道具详细属性 */
	virtual VOID			UserSouXia_SetInfo( INT nSouXiaIndex, SOUXIA_DATA* pInfo );

	/** 清除指定位置的搜侠录道具属性信息 */
	virtual VOID			UserSouXia_ClearInfo( INT nSouXiaIndex );

	/* 取得详细信息 */
	virtual SOUXIA_DATA*		UserSouXia_GetInfo( INT nSouXiaIndex );

	/* 学习过的搜侠录总数 */
	virtual VOID			UserSouXia_SetCount( BYTE nCount ){ m_nSouXiaCount = nCount; };
	virtual BYTE			UserSouXia_GetCount() { return m_nSouXiaCount; };

	/* 该搜侠录具有的类型总数 */
	virtual VOID			UserSouXia_SetTypeCount( BYTE nCount ) { m_nSouXiaTypeCount = nCount; };
	virtual BYTE			UserSouXia_GetTypeCount() { return m_nSouXiaTypeCount; };



/** 背包物品
 */

	/** 清空 */
	virtual VOID			UserBag_Clear(VOID);
	/** 设置物品 */
	virtual VOID			UserBag_SetItem(INT nBagIndex, tObject_Item* pItem, BOOL bClearOld=TRUE, BOOL reCalulate=FALSE);
	/** 重新设置物品属性 */
	virtual VOID			UserBag_SetItemInfo(INT nBagIndex, BOOL bEmpty, const _ITEM* pItem);
	/** 取得装备 */
	virtual tObject_Item*	UserBag_GetItem(INT nBagIndex);
	/** 通过GUID取得装备位置 */
	virtual INT				UserBag_GetItemIndexByGUID(__int64	 ItemGUID);
	/** 通过ID取得物品位置 */
	virtual INT				UserBag_GetItemIndexByID( INT nID );
	/** 请求装备详细信息 */
	virtual VOID			UserBag_AskExtraInfo(INT nBagIndex);
	/** 通过GUID取得装备的数量 */
	virtual INT				UserBag_CountItemByIDTable(INT	 ItemGUID);

	// 背包的克隆，目前只用来实现整理背包的功能
	struct USER_BAG_CLONE
	{
		tObject_Item*					m_pItem;
		INT								m_nItemNum;
	};
	std::vector< USER_BAG_CLONE >		m_vtUserBagClone;
	//------------
	// Clone背包物品
	//------------
	// 设置物品
	VOID					UserBagClone_SetItem(INT nBagIndex, CDataPool::USER_BAG_CLONE* pCloneObj);
	// 获得物品
	const USER_BAG_CLONE*	UserBagClone_GetItem(INT nBagIndex) const;
	// 把背包内的物品Clone到这里
	VOID					UserBagClone_Clone(VOID);
	// 清空
	VOID					UserBagClone_Clear(VOID);



/** 打开的箱子
 */

	/** 清空 */
	virtual VOID			ItemBox_Clear(VOID);
	/** 设置箱子ID */
	virtual VOID			ItemBox_SetID(INT id)		{ m_idItemBox = id; }
	/** 取得箱子ID */
	virtual INT				ItemBox_GetID(VOID) const	{ return m_idItemBox; }
	/** 设置其中物品 */
	virtual VOID			ItemBox_SetItem(INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** 取得其中物品 */
	virtual tObject_Item*	ItemBox_GetItem(INT nBoxIndex);
	/** 按照ID取得物品 */
	virtual tObject_Item*	ItemBox_GetItem(WORD idWorld, WORD idServer, INT idSerial, INT& nIndex);
	/** 返回目前箱子中物品个数 */
	virtual INT				ItemBox_GetNumber(VOID);

//掉落的物品


	/** 清空 */
	virtual VOID			ItemDrop_Clear(VOID);
   // 清楚特定项
	virtual VOID			ItemDrop_Destroy(INT id);

	/** 设置掉落物ID */
	virtual VOID			ItemDrop_SetID(INT id)		{ m_idItemDrop = id; }
	/** 取得掉落物ID */
	virtual INT				ItemDrop_GetID(VOID) const	{ return m_idItemDrop; }

	/** 设置物品 */
	virtual VOID			ItemDrop_SetItem(INT nDropObjId, tObject_Item* pItem, BOOL bClearOld=TRUE);

	/** 取得其中物品 */
	virtual tObject_Item*	ItemDrop_GetItem(INT nDropObjId);

	/** 返回目前物品个数 */
	virtual INT				ItemDrop_GetNumber(VOID);
    //保护时间的tick
	VOID					ItemDrop_Tick(VOID);

/** 商店
 */

	virtual VOID				Booth_Clear( VOID );
	virtual VOID				Booth_Sold_Clear( VOID );
	virtual VOID				Booth_SetItem(INT nBoothIndex, tObject_Item* pItem);
	virtual VOID				Booth_SetItemPrice(INT nBoothIndex, UINT nPrice);
	virtual VOID				Booth_SetSoldItem(INT nBoothIndex, tObject_Item* pItem);
	virtual VOID				Booth_SetNumber(INT nBoothNumber)			{ m_nBoothNumber = nBoothNumber; }
	virtual VOID				Booth_SetSoldNumber(INT nBoothNumber)		{ m_nBoothSoldNumber = nBoothNumber; }
	virtual tObject_Item*		Booth_GetItem(INT nBoothIndex);
	virtual UINT				Booth_GetItemPrice(INT nBoothIndex);

	virtual tObject_Item*		Booth_GetSoldItem(INT nBoothIndex);
	virtual	tObject_Item*		Booth_GetItemByID(INT IDtable);
	virtual INT					Booth_GetNumber(VOID) const					{ return m_nBoothNumber; }
	virtual INT					Booth_GetSoldNumber(VOID) const				{ return m_nBoothSoldNumber ; }
//	virtual	BOOL				Booth_IsCursorRepairing(VOID)	const		{return	m_bIsRepairing;}
//	virtual	VOID				Booth_SetCursorRepairing(BOOL flag)			{m_bIsRepairing = flag;}
	virtual	BOOL				Booth_IsClose(VOID)	const					{return	m_bBoothWindowIsDirty;}
	virtual VOID				Booth_Open(VOID)							{m_bBoothWindowIsDirty = FALSE;}
	virtual VOID				Booth_Close(VOID)							{m_bBoothWindowIsDirty = TRUE;}
	virtual VOID				Booth_SetBuyType(INT nBuyType)				{m_nBuyType = nBuyType;}
	virtual INT					Booth_GetBuyType(VOID)						{return m_nBuyType;}
	virtual VOID				Booth_SetRepairType(INT nRepairType)		{m_nRepairType = nRepairType;}
	virtual INT					Booth_GetRepairType(VOID)					{return m_nRepairType;}

	virtual VOID				Booth_SetRepairLevel(INT nRepairLevel)		{ m_nRepairLevel = nRepairLevel; }
	virtual INT					Booth_GetRepairLevel(VOID)					{ return m_nRepairLevel; }
	virtual VOID				Booth_SetBuyLevel(INT nBuyLevel)			{ m_nBuyLevel = nBuyLevel; }
	virtual INT					Booth_GetBuyLevel(VOID)						{ return m_nBuyLevel; }
	virtual VOID				Booth_SetRepairSpend(FLOAT nRepairSpend)	{ m_nRepairSpend = nRepairSpend; }
	virtual FLOAT				Booth_GetRepairSpend(VOID)					{ return m_nRepairSpend; }
	virtual VOID				Booth_SetRepairOkProb(FLOAT nRepairOkProb)	{ m_nRepairOkProb = nRepairOkProb; }
	virtual FLOAT				Booth_GetRepairOkProb(VOID)					{ return m_nRepairOkProb; }

	virtual VOID				Booth_SetShopNpcId(INT nShopNpcId)			{ m_nShopNpcId = nShopNpcId; }
	virtual INT					Booth_GetShopNpcId(VOID)					{ return m_nShopNpcId; }

	virtual BOOL				Booth_IsCanRepair(tObject_Item* pItem);
	virtual BOOL				Booth_IsCanBuy(tObject_Item* pItem);
	
	//  IB SHOP 
	virtual VOID				IBShop_Clear( VOID );
	virtual VOID				IBShop_SetItem(INT nIBShopIndex, tObject_Item* pItem);
	virtual VOID				IBShop_SetItemPrice(INT nIBShopIndex, UINT nPrice);
	virtual VOID				IBShop_SetNumber(INT nIBShopNumber)			{ m_nIBShopNumber = nIBShopNumber; }
	virtual tObject_Item*		IBShop_GetItem(INT nIBShopIndex);
	virtual UINT				IBShop_GetItemPrice(INT nIBShopIndex);

	virtual	tObject_Item*		IBShop_GetItemByID(INT IDtable);
	virtual INT					IBShop_GetNumber(VOID) const				{ return m_nIBShopNumber; }
	virtual VOID				IBShop_SetItemIsNewSell(INT nIBShopIndex, INT isNew);

/** 银行数据的访问
 */

	virtual VOID				UserBank_Clear(VOID);
	virtual VOID				UserBank_SetItem(INT nBankIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		UserBank_GetItem(INT nBankIndex);
	virtual	VOID				UserBank_SetBankEndIndex(INT endindex)		{ m_CurBankEndIndex = endindex; }
	virtual	INT					UserBank_GetBankEndIndex()					{ return m_CurBankEndIndex; }
	virtual	VOID				UserBank_SetBankMoney(INT Money)			{ m_CurBankMoney = Money; }
	virtual	INT					UserBank_GetBankMoney()						{ return m_CurBankMoney; }
	virtual	VOID				UserBank_SetItemExtraInfo(INT nBankIndex, BOOL bEmpty, const _ITEM* pItem);
	/** 查询银行nIndex编号的租赁箱子是不是有空格 */
	virtual BOOL				UserBank_IsEmpty(INT nIndex);
	virtual VOID				UserBank_SetNpcId(INT nNpcId)				{ m_nBankNpcID = nNpcId; }
	virtual INT					UserBank_GetNpcId(VOID)						{ return m_nBankNpcID; }


/** 关系系统部分
 */

	virtual Relation*			GetRelation()	{ return m_pRelation; }
	virtual CMailPool*			GetMailPool()	{ return m_pMailPool; }


/** 交易盒数据访问
 */
	
	virtual VOID				MyExBox_Clear(VOID);
	virtual VOID				MyExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		MyExBox_GetItem(INT nExBoxIndex);
	virtual	VOID				MyExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	virtual	VOID				MyExBox_SetMoney(INT Money)					{ m_MyExchangeBox.m_Money	= Money; }
	virtual	INT					MyExBox_GetMoney()							{ return m_MyExchangeBox.m_Money;}
	virtual	VOID				MyExBox_SetLock(BOOL Setlock)				{ m_MyExchangeBox.m_IsLocked	=	Setlock; }
	virtual	BOOL				MyExBox_GetLock()							{ return m_MyExchangeBox.m_IsLocked; }
	virtual	VOID				MyExBox_SetConfirm(BOOL SetConfirm)			{ m_MyExchangeBox.m_CanConform	=	SetConfirm; }
	virtual	BOOL				MyExBox_GetConfirm()						{ return m_MyExchangeBox.m_CanConform; }
	virtual	INT					MyExBox_GetItemNum()						{ return (INT)m_MyExchangeBox.m_ItemList.size(); }


	INT							MyExBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						MyExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	SDATA_PET*					MyExBox_GetPet(INT nIndex);
	INT							MyExBox_GetCount(VOID);
	VOID						MyExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					MyExBox_GetSkill(INT nIndex,INT nSkillIndex);


	// 查询自己的交易框里头是否有空格
	virtual	BOOL				MyExBox_IsEmpty(VOID);


	virtual VOID				OtExBox_Clear(BYTE bFlagClearPet = TRUE);
	virtual VOID				OtExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		OtExBox_GetItem(INT nExBoxIndex);
	virtual	VOID				OtExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	virtual	VOID				OtExBox_SetMoney(INT Money)					{ m_OtherExchangeBox.m_Money	= Money; }
	virtual	INT					OtExBox_GetMoney()							{ return m_OtherExchangeBox.m_Money;}
	virtual	VOID				OtExBox_SetLock(BOOL Setlock)				{ m_OtherExchangeBox.m_IsLocked	=	Setlock; }
	virtual	BOOL				OtExBox_GetLock()							{ return m_OtherExchangeBox.m_IsLocked; }
	virtual	VOID				OtExBox_SetConfirm(BOOL SetConfirm)			{ m_OtherExchangeBox.m_CanConform	=	SetConfirm; }
	virtual	BOOL				OtExBox_GetConfirm()						{ return m_OtherExchangeBox.m_CanConform; }
	virtual	INT					OtExBox_GetItemNum()						{ return (INT)m_OtherExchangeBox.m_ItemList.size(); }

	INT							OtExBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						OtExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	SDATA_PET*					OtExBox_GetPet(INT nIndex);
	INT							OtExBox_GetCount(VOID);
	VOID						OtExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	VOID						OtExBox_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld);
	PET_SKILL*					OtExBox_GetSkill(INT nIndex,INT nSkillIndex);

	/** 当前交易对方ID */
	virtual	VOID				SetExchangObjID(INT ObjID)					{ m_CurExID = ObjID;}
	virtual	INT					GetExchangObjID()							{ return m_CurExID;}
	
	/** 加入到申请人列表中 */
	virtual	VOID				AddToAppList(INT appid);				
	/** 从申请人列表中取出一个 */
	virtual	INT					GetFromAppList();						
	/** 申请人列表是否空 */
	virtual	BOOL				IsStillAnyAppInList();			


/*
	//------------------------------------------------------
	// 宝石合成/镶嵌界面的数据
	//------------------------------------------------------
	virtual VOID				EnchasePage_Clear(VOID);
	virtual VOID				EnchasePage_SetItem(INT nEnchaseIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		EnchasePage_GetItem(INT nEnchaseIndex);
*/

/*以后再加
	//清空附加物品的列表
	virtual VOID				AppendItem_Clear(VOID);
	//设置附加物品的列表
	virtual VOID				AppendItem_SetItem(INT nIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//取得附加物品的列表的第i个物品。
	virtual tObject_Item*		AppendItem_GetItem(INT nIndex);			
*/

	/** 清空递交任务物品的列表 */
	virtual VOID				MissionBox_Clear(VOID);
	/** 设置递交任务物品的列表 */
	virtual VOID				MissionBox_SetItem(INT nMissionBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** 取得递交任务物品的列表 */
	virtual tObject_Item*		MissionBox_GetItem(INT nMissionBoxIndex);

	/** 清空任务物品的列表 */
	virtual VOID				QuestLogBox_Clear(VOID);							
	/** 设置任务物品的列表 */
	virtual VOID				QuestLogBox_SetItem(INT nQuestIndex, INT nItemIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** 取得任务物品的列表 */
	virtual tObject_Item*		QuestLogBox_GetItem(INT nQuestIndex, INT nItemIndex);	

	VOID						Copy_To_TargetEquip(CObject* pObj)			{ m_pTargetEquip = pObj; }
	CObject*					GetTargetEquip(VOID)						{ return m_pTargetEquip;}



/** 摊位盒数据访问
 */

	virtual VOID				MyStallBox_SetDefaultPage(INT nPage)		{ m_MyStallBox.m_nDefaultPage = nPage;}
	virtual INT					MyStallBox_GetDefaultPage()					{ return m_MyStallBox.m_nDefaultPage;}
	/** 设置摊位费 */
	virtual VOID				MyStallBox_SetPosTax(UINT unPosTax)			{ m_MyStallBox.m_PosTax = unPosTax;}
	/** 获得摊位费 */
	virtual UINT				MyStallBox_GetPosTax(VOID)					{ return m_MyStallBox.m_PosTax;}
	/** 设置交易税 */
	virtual VOID				MyStallBox_SetTradeTax(UINT unTradeTax)		{ m_MyStallBox.m_TradeTax = unTradeTax;}
	/** 获得交易税 */
	virtual UINT				MyStallBox_GetTradeTax(VOID)				{ return m_MyStallBox.m_TradeTax;}

	/** 清空自己摊位盒 */
	virtual VOID				MyStallBox_Clear(VOID);	
	/** 设置摊主ID */
	virtual VOID				MyStallBox_SetIsOpen(BOOL IsOpen)			{ m_MyStallBox.m_bIsOpen = IsOpen; }
	/** 获得摊主ID */
	virtual BOOL				MyStallBox_GetIsOpen()						{ return m_MyStallBox.m_bIsOpen; }
	/** 设置摊主ID */
	virtual VOID				MyStallBox_SetObjID(UINT nObjID)			{ m_MyStallBox.m_ObjID = nObjID; }
	/** 获得摊主ID */
	virtual UINT				MyStallBox_GetObjID()						{ return m_MyStallBox.m_ObjID; }
	/** 设置摊位名 */
	virtual VOID				MyStallBox_SetStallName(LPCTSTR szName)		{ m_MyStallBox.m_szStallName = szName;}
	/** 获得摊位名 */
	virtual LPCTSTR				MyStallBox_GetStallName()					{ return m_MyStallBox.m_szStallName.c_str(); }
	/** 设置摊主名 */
	virtual VOID				MyStallBox_SetOwnerName(LPCTSTR szName)		{ m_MyStallBox.m_szOwnerName = szName;}
	/** 获得摊主名 */
	virtual LPCTSTR				MyStallBox_GetOwnerName()					{ return m_MyStallBox.m_szOwnerName.c_str(); }
	/** 设置首页 */
	virtual VOID				MyStallBox_SetFirstPage(UINT nFirstPage)	{ m_MyStallBox.m_nFirstPage = nFirstPage;}
	/** 获得首页 */
	virtual UINT				MyStallBox_GetFirstPage()					{ return m_MyStallBox.m_nFirstPage; }
	/** 设置摊位中的物品 */
	virtual VOID				MyStallBox_SetItem(INT nStallBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	/** 设置摊位物品价格 */
	virtual VOID				MyStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice);
	/** 设置摊位物品序列号 */
	virtual VOID				MyStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial);
	/** 获得摊位中指定位置的物品 */
	virtual tObject_Item*		MyStallBox_GetItem(INT nStallBoxIndex);
	/** 获得物品价格 */
	virtual UINT				MyStallBox_GetItemPrice(INT nStallBoxIndex);
	/** 获得物品序列号 */
	virtual UINT				MyStallBox_GetItemSerial(INT nStallBoxIndex);
	/** 设置物品详细信息 */
	virtual	VOID				MyStallBox_SetItemExtraInfo(INT nStallBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	/** 获得物品数量 */
	virtual	INT					MyStallBox_GetItemNum()						{ return (INT)m_MyStallBox.m_ItemList.size(); }
	/** 查询自己是否处于摆摊状态 */
	virtual	BOOL				MyStallBox_IsOpen()							{ return (INT)m_MyStallBox.m_bIsOpen; }
	/** 当前选中的物品 */
	virtual VOID				MyStallBox_SetCurSelectItem(tObject_Item* pItem)	{ m_CurSelectItem.pItem = pItem;}
	/** 当前选中的物品 */
	virtual tObject_Item*		MyStallBox_GetCurSelectItem()				{ return m_CurSelectItem.pItem;}
	/** 当前选中的物品所在的容器 */
	virtual VOID				MyStallBox_SetSelectItemInPos(INT nSelectItemInPos)	{ m_nSelectItemInPos = nSelectItemInPos;}
	/** 当前选中的物品所在的容器 */
	virtual INT					MyStallBox_GetSelectItemInPos()				{ return m_nSelectItemInPos; }
	/** 根据GUID查找物品位置 */
	virtual	INT					MyStallBox_GetItemIndexByGUID(__int64 ItemGUID);
	virtual	UINT				MyStallBox_GetOwnerGUID()					{ return m_MyStallBox.m_OtGUID; }
	virtual	VOID				MyStallBox_SetOwnerGUID(UINT GUID)			{ m_MyStallBox.m_OtGUID = GUID; }

	/** 当前选中的物品所在的容器 */
	virtual INT					MyStallBox_GetnSelectPetOnStall(){return m_nSelectPetOnStall;}
	/** 根据GUID查找物品位置 */
	virtual	VOID				MyStallBox_SetnSelectPetOnStall(INT nSelectPetOnStall){m_nSelectPetOnStall = nSelectPetOnStall;};
	virtual	BOOL				MyStallBox_IsHaveEmpty(INT nType);


/** 摆摊宠物
 */

	INT							MyStallBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						MyStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	VOID						MyStallBox_SetPetPrice(INT nIndex, INT nPetPrice);
	VOID						MyStallBox_SetPetSerial(INT nIndex, INT nSerial);
	INT							MyStallBox_GetPetPrice(INT nIndex);
	INT							MyStallBox_GetPetSerial(INT nIndex);
	SDATA_PET*					MyStallBox_GetPet(INT nIndex);
	INT							MyStallBox_GetCount(VOID);
	VOID						MyStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					MyStallBox_GetSkill(INT nIndex,INT nSkillIndex);
	VOID						MyStallBox_GetSelectpetGuid(PET_GUID_t &CurSelectpetGuid)
	{
		CurSelectpetGuid.m_uHighSection = m_CurSelectpetGuid.m_uHighSection;
		CurSelectpetGuid.m_uLowSection  = m_CurSelectpetGuid.m_uLowSection;
	};           
	VOID						MyStallBox_SetSelectpetGuid(PET_GUID_t CurSelectpetGuid)
	{
		m_CurSelectpetGuid.m_uHighSection = CurSelectpetGuid.m_uHighSection;
		m_CurSelectpetGuid.m_uLowSection  = CurSelectpetGuid.m_uLowSection;
	}
	virtual	BOOL				MyStallBox_IsPetOnStall(PET_GUID_t PetGuid);

	virtual	VOID				MyStallMsg_ClearUp(VOID);
	virtual	VOID				MyStallMsg_SetSerial( UINT nSerial)			{ m_MyStallBox.m_BBS.m_nSerial = nSerial;}
	virtual	INT					MyStallMsg_SetIDByIndex(UINT nIndex, UINT nID);
	virtual	INT					MyStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin);
	virtual	INT					MyStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply);
	virtual	INT					MyStallMsg_SetReTimeByIndex(UINT nIndex, BYTE nHour, BYTE nMin);
	virtual	VOID				MyStallMsg_SetTitle(LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetAuthorNameByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetProposedMessageByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	UINT				MyStallMsg_GetSerial()						{ return m_MyStallBox.m_BBS.m_nSerial;}
	virtual	INT					MyStallMsg_GetIDByIndex(UINT nIndex);
	virtual	BYTE				MyStallMsg_GetHasReplyByIndex(UINT nIndex);
	virtual	LPCTSTR				MyStallMsg_GetTitle();
	virtual	LPCTSTR				MyStallMsg_GetAuthorNameByIndex(UINT nIndex);
	virtual	LPCTSTR				MyStallMsg_GetProposedMessageByIndex(UINT nIndex );
	virtual	LPCTSTR				MyStallMsg_GetReplyMessageByIndex(UINT nIndex);
	virtual	INT					MyStallMsg_GetMessageNum();
	virtual	VOID				MyStallMsg_SetMessageNum(INT nNum);
	virtual	INT					MyStallMsg_GetHourByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetMinByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetReHourByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetReMinByIndex(UINT	nIndex);


	virtual VOID				OtStallBox_SetDefaultPage(INT nPage)		{ m_OtStallBox.m_nDefaultPage = nPage;}
	virtual INT					OtStallBox_GetDefaultPage()					{ return m_OtStallBox.m_nDefaultPage;}
	virtual VOID				OtStallBox_SetTradeTax(UINT unTradeTax)		{ m_OtStallBox.m_TradeTax = unTradeTax;}	//设置交易税
	virtual UINT				OtStallBox_GetTradeTax(VOID)				{ return m_OtStallBox.m_TradeTax;}			//获得交易税

	virtual VOID				OtStallBox_Clear(BYTE bFlagClearPet = TRUE);											//清空对方摊位盒
	virtual VOID				OtStallBox_SetIsOpen(BOOL IsOpen)			{ m_OtStallBox.m_bIsOpen = IsOpen; }		//设置摊主ID
	virtual BOOL				OtStallBox_GetIsOpen()						{ return m_OtStallBox.m_bIsOpen; }			//获得摊主ID
	virtual VOID				OtStallBox_SetObjID(UINT nObjID)			{ m_OtStallBox.m_ObjID = nObjID; }			//设置摊主ID
	virtual UINT				OtStallBox_GetObjID()						{ return m_OtStallBox.m_ObjID; }			//获得摊主ID
	virtual VOID				OtStallBox_SetStallName(LPCTSTR szName)		{ m_OtStallBox.m_szStallName = szName;}		//设置摊位名
	virtual LPCTSTR				OtStallBox_GetStallName()					{ return m_OtStallBox.m_szStallName.c_str(); }//获得摊位名
	virtual VOID				OtStallBox_SetOwnerName(LPCTSTR szName)		{ m_OtStallBox.m_szOwnerName = szName;}		//设置摊主名
	virtual LPCTSTR				OtStallBox_GetOwnerName()					{ return m_OtStallBox.m_szOwnerName.c_str(); }//获得摊主名
	virtual VOID				OtStallBox_SetFirstPage(UINT nFirstPage)	{m_OtStallBox.m_nFirstPage = nFirstPage;}	//设置摊位首页
	virtual UINT				OtStallBox_GetFirstPage()					{ return m_OtStallBox.m_nFirstPage; }		//获得摊位首页
	virtual VOID				OtStallBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);			//设置摊位物品
	virtual VOID				OtStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice);								//设置物品价格
	virtual VOID				OtStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial);								//设置物品序列号
	virtual tObject_Item*		OtStallBox_GetItem(INT nExBoxIndex);													//获得制定位置的物品
	virtual UINT				OtStallBox_GetItemPrice(INT nStallBoxIndex);											//获得物品价格
	virtual UINT				OtStallBox_GetItemSerial(INT nStallBoxIndex);											//获得物品序列号
	virtual	VOID				OtStallBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);			//设置物品详细信息
	virtual	INT					OtStallBox_GetItemNum()						{ return (INT)m_MyStallBox.m_ItemList.size(); }//获得摊位中的物品数量
	virtual	INT					OtStallBox_GetItemIndexByGUID(__int64 ItemGUID);										//根据GUID查找物品位置

	virtual	UINT				OtStallBox_GetOwnerGUID()					{ return m_OtStallBox.m_OtGUID;}
	virtual	VOID				OtStallBox_SetOwnerGUID(UINT GUID)			{ m_OtStallBox.m_OtGUID = GUID;}

	//摆摊宠物
	INT							OtStallBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						OtStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	VOID						OtStallBox_SetPetPrice(INT nIndex, INT nPetPrice);
	VOID						OtStallBox_SetPetSerial(INT nIndex, INT nSerial);
	INT							OtStallBox_GetPetPrice(INT nIndex);
	INT							OtStallBox_GetPetSerial(INT nIndex);
	SDATA_PET*					OtStallBox_GetPet(INT nIndex);
	INT							OtStallBox_GetCount(VOID);
	VOID						OtStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					OtStallBox_GetSkill(INT nIndex,INT nSkillIndex);

	virtual	VOID				OtStallMsg_ClearUp(VOID);
	virtual	VOID				OtStallMsg_SetSerial( UINT nSerial)			{ m_OtStallBox.m_BBS.m_nSerial = nSerial;}
	virtual	INT					OtStallMsg_SetIDByIndex(UINT nIndex, UINT nID);
	virtual	INT					OtStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin);
	virtual	INT					OtStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply);
	virtual	INT					OtStallMsg_SetReTimeByIndex(UINT nIndex, BYTE nHour, BYTE nMin);
	virtual	VOID				OtStallMsg_SetTitle(LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetAuthorNameByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetProposedMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	UINT				OtStallMsg_GetSerial()						{ return m_OtStallBox.m_BBS.m_nSerial;}
	virtual	INT					OtStallMsg_GetIDByIndex(UINT nIndex);
	virtual	BYTE				OtStallMsg_GetHasReplyByIndex(UINT nIndex);
	virtual	LPCTSTR				OtStallMsg_GetTitle();
	virtual	LPCTSTR				OtStallMsg_GetAuthorNameByIndex(UINT nIndex);
	virtual	LPCTSTR				OtStallMsg_GetProposedMessageByIndex(UINT nIndex );
	virtual	LPCTSTR				OtStallMsg_GetReplyMessageByIndex(UINT nIndex);
	virtual	INT					OtStallMsg_GetHourByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetMinByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetReHourByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetReMinByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetMessageNum();
	virtual	VOID				OtStallMsg_SetMessageNum(INT nNum);									   
	

//---------------------------------------------------------------------------------------------------
/** 学习（升级）门派心法相关
 */

	/** 门派心法教师的ID */
//	VOID						StudyXinfa_SetMenpaiInfo(INT idXinfaTeacher)		{ m_idXinfaTeacher = idXinfaTeacher;}
//	INT							StudyXinfa_GetMenpaiInfo(VOID)						{ return m_idXinfaTeacher;}
//	VOID						StudyXinfa_SetXinfaTeacherId(INT nXinfaTeacherId)	{ m_nXinfaTeacherId = nXinfaTeacherId;}
//	INT							StudyXinfa_GetXinfaTeacherId(VOID)					{ return m_nXinfaTeacherId;}
	/** 向服务器发送加入门派的事件 */
//$$	VOID						SendStudyXinfaEvent(INT nXinfaID);

	/** 生活技能教师 */
	INT							StudyAbility_GetID(VOID)					{ return m_nStudyAbility_ID; }
	VOID						StudyAbility_SetID(INT nAbilityID)			{ m_nStudyAbility_ID = nAbilityID; }

	INT							StudyAbility_GetNeedMoney(VOID)				{ return m_uStudyAbility_NeedMoney; }
	VOID						StudyAbility_SetNeedMoney(INT uNeedMoney)	{ m_uStudyAbility_NeedMoney = uNeedMoney; }

	INT							StudyAbility_GetNeedExp(VOID)				{ return m_uStudyAbility_NeedExp; }
	VOID						StudyAbility_SetNeedExp(INT uNeedExp)		{ m_uStudyAbility_NeedExp = uNeedExp; }

	INT							StudyAbility_GetSkillExp(VOID)				{ return m_uStudyAbility_SkillExp; }
	VOID						StudyAbility_SetSkillExp(INT uSkillExp)		{ m_uStudyAbility_SkillExp = uSkillExp; }

	INT							StudyAbility_GetLevelLimit(VOID)							{ return m_nStudyAbility_LevelLimit;}
	VOID						StudyAbility_SetLevelLimit(INT nStudyAbility_LevelLimit)	{ m_nStudyAbility_LevelLimit = nStudyAbility_LevelLimit;}

	INT							StudyAbility_GetScriptId(VOID)								{ return m_uStudyAbility_ScriptId;}
	VOID						StudyAbility_SetScriptId(INT uStudyAbility_ScriptId)		{ m_uStudyAbility_ScriptId = uStudyAbility_ScriptId;}

	INT							StudyAbility_GetNpcId(VOID)									{ return m_StudyAbility_NpcId;}
	VOID						StudyAbility_SetNpcId(INT StudyAbility_NpcId)				{ m_StudyAbility_NpcId = StudyAbility_NpcId;}



/**  技能的骑宠相关数据
*/

	INT							Pet_GetPetByGuid(const PET_GUID_t& PetGuid);
	SDATA_PET*					Pet_GetPet(INT nIndex);
	SDATA_PET*					Pet_GetPet(const PET_GUID_t& PetGuid);
	VOID						Pet_ClearPet(INT nIndex);
	INT							Pet_GetCount(VOID);
	VOID						Pet_SetPetFakeModel(INT nIndex, INT nMountID);
	const CHAR*					Pet_GetPetFakeName(INT nIndex);

	// 设置骑宠属性
	VOID						Pet_SetPet( SDATA_PET Pet );
	// 增加骑宠
	INT							Pet_AddPet( SDATA_PET Pet );


	// 获得宠物的食物类型
	INT							Pet_GetPetFoodType(PET_GUID_t guidPet);											
	// 获得宠物相应食物类型得idTable限制
	VOID						Pet_GetPetFoodTypeMinMax(INT nFoodType, INT* idMin = NULL, INT* idMax = NULL);	
	// 从背包获得喂养所需最低级的食物
	tObject_Item*				Pet_GetLowestLevel_Food_From_Package(PET_GUID_t pg, INT *idxPackage = NULL);	
	// 从背包获得驯养所需最低级的食物
	tObject_Item*				Pet_GetLowestLevel_Dome_From_Package(PET_GUID_t pg, INT *idxPackage = NULL);	

	VOID						Pet_CopyToTarget(SDATA_PET* pSourcePet);

	SDATA_PET*					TargetPet_GetPet(VOID)						{ return &m_TargetPet; }
	VOID						TargetPet_Clear(VOID)						{  m_TargetPet.CleanUp();}
	VOID						TargetPet_SetPetModel(INT nMountID);
	PET_SKILL*					TargetPet_GetSkill(INT nSkillIndex);
	VOID						TargetPet_SetSkill(INT nSkillIndex, INT nSkillID, BOOL bCanUse);
	VOID						TargetPet_SetSkill(INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);
	const CHAR*					TargetPet_GetPetFakeModel();
	VOID						TargetPet_SetPetFakeModel(INT nMountID);

	VOID						PetStudySkill_SetPetModel(INT nRaceID);

	VOID						Pet_SetSkill(INT nIndex, INT nSkillIndex, INT nSkillID, BOOL bCanUse);
	VOID						Pet_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);
	PET_SKILL*					Pet_GetSkill(INT nIndex,INT nSkillIndex);
	BOOL						Pet_SendUseSkillMessage( SkillID_t idSkill, ObjID_t idTargetObj, FLOAT fTargetX, FLOAT fTargetZ );


//---------------------------------------------------------------------------------------------------
/**  坐骑相关数据  [7/8/2010 陈军龙] 
*/
    const CHAR*                 Mount_GetMountFakeName(INT nIndex);
	VOID                        Mount_SetMountFakeModel(INT nIndex, INT nMountID);
	INT                         Mount_GetMountCount(VOID);


//---------------------------------------------------------------------------------------------------
/**  技能的BUFF数据
 */

	INT							BuffImpact_GetCount( VOID )const			{ return (INT)(m_mapBuffImpact.size()); }
	const _BUFF_IMPACT_INFO*	BuffImpact_Get( INT nSN )const;
	const _BUFF_IMPACT_INFO*	BuffImpact_GetByIndex( INT nIndex )const;
	const _BUFF_IMPACT_INFO*	BuffImpact_GetByID( INT nID )const;
	BOOL						BuffImpact_Add( const _BUFF_IMPACT_INFO *pBuffImpact );
	VOID						BuffImpact_Remove( INT nSN );
	VOID						BuffImpact_RemoveByIndex( INT nIndex );
	VOID						BuffImpact_RemoveAll( VOID );
	const CHAR*					BuffImpact_GetToolTips( INT nSN )const;
	const CHAR*					BuffImpact_GetToolTipsByIndex( INT nIndex )const;
	BOOL						BuffImpact_Dispel( INT nSN );
	BOOL						BuffImpact_DispelByIndex( INT nIndex );
	INT							BuffImpact_GetTime( INT nIndex );
	INT							BuffImpact_GetTimeByIndex( INT nIndex );


	/** 当前需要销毁物品 */
	INT							DisCard_GetItemConta()						{ return m_CurDisCardItem.m_nContainer;}
	INT							DisCard_GetItemPos()						{ return m_CurDisCardItem.m_nPosition;}
	VOID						DisCard_SetItemConta(INT nContainer)		{ m_CurDisCardItem.m_nContainer = nContainer;}
	VOID						DisCard_SetItemPos(INT nPosition)			{ m_CurDisCardItem.m_nPosition = nPosition;}


	X_PARAM						m_X_PARAM;
	VOID						X_PARAM_Set(X_PARAM *pBuf,INT nUIIndex);
	X_PARAM*					X_PARAM_Get( VOID )							{ return &m_X_PARAM;};

	X_SCRIPT					m_X_SCRIPT;
	X_SCRIPT*					X_XCRIPT_Get( VOID )						{ return &m_X_SCRIPT;};


	/** 当前正在拆分的物品 */
	INT							Split_GetItemConta()						{ return m_CurSplitItem.m_nContainer;}
	INT							Split_GetItemPos()							{ return m_CurSplitItem.m_nPosition;}
	INT							Split_GetSum()								{ return m_CurSplitItem.m_nSum;}
	INT							Split_GetNumber()							{ return m_CurSplitItem.m_nNumber;}
	VOID						Split_SetItemConta(INT nContainer)			{ m_CurSplitItem.m_nContainer = nContainer;}
	VOID						Split_SetItemPos(INT nPosition)				{ m_CurSplitItem.m_nPosition = nPosition;}
	VOID						Split_SetSum(INT nSum)						{ m_CurSplitItem.m_nSum = nSum;}
	VOID						Split_SetNumber(INT nNumber)				{ m_CurSplitItem.m_nNumber = nNumber;}

 
  


	/** 当前正在打星的物品 */
	VOID						AddStar_CleanUP()								{  m_CurAddStarEquip.CleanUp();}
	INT							AddStar_GetItemConta()							{ return m_CurAddStarEquip.m_nContainer;}
	INT							AddStar_GetItemPos()							{ return m_CurAddStarEquip.m_nPosition;}
	INT							AddStar_GetGemId()								{ return m_CurAddStarEquip.m_GemId;}
	INT							AddStar_GetGemNum()								{ return m_CurAddStarEquip.m_GemNum;}
	INT							AddStar_GetMoney()								{ return m_CurAddStarEquip.m_nMoney;}
	INT							AddStar_GetIntensifyInfo()						{ return m_CurAddStarEquip.m_nIntensifyInfo;}

	VOID						AddStar_SetItemConta(INT nContainer)			{ m_CurAddStarEquip.m_nContainer = nContainer;}
	VOID						AddStar_SetItemPos(INT nPosition)				{ m_CurAddStarEquip.m_nPosition = nPosition;}
	VOID						AddStar_SetGemId(INT nGemID)					{ m_CurAddStarEquip.m_GemId = nGemID;}
	VOID						AddStar_SetGemNum(INT nGemNum)					{ m_CurAddStarEquip.m_GemNum = nGemNum;}
	VOID						AddStar_SetMoney( INT nMoney)					{ m_CurAddStarEquip.m_nMoney = nMoney;}
	VOID						AddStar_SetIntensifyInfo( INT nIntensifyInfo)	{ m_CurAddStarEquip.m_nIntensifyInfo = nIntensifyInfo;}
	


	/** 当前正在打孔的物品 */
	VOID						AddHole_CleanUP()								{  m_CurAddHoleEquip.CleanUp();}
	INT							AddHole_GetItemConta()							{ return m_CurAddHoleEquip.m_nContainer;}
	INT							AddHole_GetItemPos()							{ return m_CurAddHoleEquip.m_nPosition;}
	INT							AddHole_GetNeedItemId()							{ return m_CurAddHoleEquip.m_ItemId;}
	INT							AddHole_GetNeedItemNum()						{ return m_CurAddHoleEquip.m_ItemNum;}
	INT							AddHole_GetNeedMoney()							{ return m_CurAddHoleEquip.m_Money;}
	INT							AddHole_GetNpcId()								{ return m_CurAddHoleEquip.m_NpcId;}
	

	VOID						AddHole_SetItemConta(INT nContainer)			{ m_CurAddHoleEquip.m_nContainer = nContainer;}
	VOID						AddHole_SetItemPos(INT nPosition)				{ m_CurAddHoleEquip.m_nPosition = nPosition;}
	VOID						AddHole_SetNeedItemId(INT nNeedID)				{ m_CurAddHoleEquip.m_ItemId = nNeedID;}
	VOID						AddHole_SetNeedItemNum(INT nNeedNum)			{ m_CurAddHoleEquip.m_ItemNum = nNeedNum;}
	VOID						AddHole_SetNeedMoney(INT nNeedMoney)			{ m_CurAddHoleEquip.m_Money = nNeedMoney;}
	VOID						AddHole_SetNpcId(INT npc)						{ m_CurAddHoleEquip.m_NpcId = npc;}


	/** 当前正在绑定的物品 */
	VOID						Bind_CleanUP()								{  m_CurBangDingEquip.CleanUp();}
	INT							Bind_GetItemConta()							{ return m_CurBangDingEquip.m_nContainer;}
	INT							Bind_GetItemPos()							{ return m_CurBangDingEquip.m_nPosition;}
	INT							Bind_GetNeedItemId()						{ return m_CurBangDingEquip.m_ItemId;}
	INT							Bind_GetNeedItemPos()						{ return m_CurBangDingEquip.m_NeedItemPos;}

	
	 
	VOID						Bind_SetItemConta(INT nContainer)			{ m_CurBangDingEquip.m_nContainer = nContainer;}
	VOID						Bind_SetItemPos(INT nPosition)				{ m_CurBangDingEquip.m_nPosition = nPosition;}
	VOID						Bind_SetNeedItemId(INT nNeedItemID)			{ m_CurBangDingEquip.m_ItemId = nNeedItemID;}
	VOID						Bind_SetNeedItemPos(INT nNeedItemPos)		{ m_CurBangDingEquip.m_NeedItemPos = nNeedItemPos;}


	/** 当前正在改造牌型的物品 */
	VOID						ChangeCard_CleanUP()							{  m_CurChangeCardEquip.CleanUp();}
	INT							ChangeCard_GetItemConta()						{ return m_CurChangeCardEquip.m_nContainer;}
	INT							ChangeCard_GetItemPos()							{ return m_CurChangeCardEquip.m_nPosition;}
	INT							ChangeCard_GetNeedItemId()						{ return m_CurChangeCardEquip.m_ItemId;}
	INT							ChangeCard_GetNeedItemPos()						{ return m_CurChangeCardEquip.m_NeedItemPos;}



	VOID						ChangeCard_SetItemConta(INT nContainer)			{ m_CurChangeCardEquip.m_nContainer = nContainer;}
	VOID						ChangeCard_SetItemPos(INT nPosition)			{ m_CurChangeCardEquip.m_nPosition = nPosition;}
	VOID						ChangeCard_SetNeedItemId(INT nNeedItemID)		{ m_CurChangeCardEquip.m_ItemId = nNeedItemID;}
	VOID						ChangeCard_SetNeedItemPos(INT nNeedItemPos)		{ m_CurChangeCardEquip.m_NeedItemPos = nNeedItemPos;}





	/** 当前正在交换牌型的物品 */
	VOID						SwapCard_CleanUP()								{  m_CurSwapCardEquip.CleanUp();}
	INT							SwapCard_GetItemConta1()						{ return m_CurSwapCardEquip.m_nContainer1;}
	INT							SwapCard_GetItemPos1()							{ return m_CurSwapCardEquip.m_nPosition1;}
	INT							SwapCard_GetItemConta2()						{ return m_CurSwapCardEquip.m_nContainer2;}
	INT							SwapCard_GetItemPos2()							{ return m_CurSwapCardEquip.m_nPosition2;}
	INT							SwapCard_GetNeedItemId()						{ return m_CurSwapCardEquip.m_ItemId;}
	INT							SwapCard_GetNeedItemPos()						{ return m_CurSwapCardEquip.m_NeedItemPos;}

	VOID						SwapCard_SetItemConta1(INT nContainer)			{ m_CurSwapCardEquip.m_nContainer1 = nContainer;}
	VOID						SwapCard_SetItemPos1(INT nPosition)				{ m_CurSwapCardEquip.m_nPosition1 = nPosition;}
	VOID						SwapCard_SetItemConta2(INT nContainer)			{ m_CurSwapCardEquip.m_nContainer2 = nContainer;}
	VOID						SwapCard_SetItemPos2(INT nPosition)				{ m_CurSwapCardEquip.m_nPosition2 = nPosition;}
	VOID						SwapCard_SetNeedItemId(INT nNeedItemID)			{ m_CurSwapCardEquip.m_ItemId = nNeedItemID;}
	VOID						SwapCard_SetNeedItemPos(INT nNeedItemPos)		{ m_CurSwapCardEquip.m_NeedItemPos = nNeedItemPos;}





	/** 当前正在改造的装备 */
	VOID						Rework_CleanUP()							{  m_CurEquip_ReWork.CleanUp();}
	INT							Rework_GetItemConta()						{ return m_CurEquip_ReWork.m_nContainer;}
	INT							Rework_GetItemPos()							{ return m_CurEquip_ReWork.m_nPosition;}
	INT							Rework_GetIdTable()							{ return m_CurEquip_ReWork.m_IdTable;}



	VOID						Rework_SetItemConta(INT nContainer)			{ m_CurEquip_ReWork.m_nContainer = nContainer;}
	VOID						Rework_SetItemPos(INT nPosition)			{ m_CurEquip_ReWork.m_nPosition = nPosition;}
	VOID					    Rework_SetIdTable(INT id)					{ m_CurEquip_ReWork.m_IdTable = id;}	

	INT							Rework_GetMatId(int index)							
	{ 

		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_ReWork.m_Mat[index].m_materialId;

	}

	INT							Rework_GetMatNum(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_ReWork.m_Mat[index].m_materialNum;

	}

	INT							Rework_GetMatGrade(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_ReWork.m_Mat[index].m_materialGrade;

	}

	VOID						Rework_RecordMatNum()					    { 	m_CurSynEquip.m_MatNum ++;} 
	VOID						Rework_SetMatId(INT Id,int index)			

	{

		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  ;
		m_CurEquip_ReWork.m_Mat[index].m_materialId = Id;	

	}	
	VOID						Rework_SetMatNum(INT Num,int index)			
	{
		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  ;
		m_CurEquip_ReWork.m_Mat[index].m_materialNum = Num;

	}	
	VOID					   Rework_SetMatGrade(INT Grade,int index)		

	{
		if( 0 >index  && index >= MAX_EQUIP_RWMATL_NUM)   
			return  ;
		m_CurEquip_ReWork.m_Mat[index].m_materialGrade = Grade;
	}	



	////装备灵魂改造

	/** 当前正在灵魂改造的装备 */
	VOID						SiritRework_CleanUP()							{  m_CurEquip_SpiritReWork.CleanUp();}
	INT							SiritRework_GetItemConta()						{ return m_CurEquip_SpiritReWork.m_nContainer;}
	INT							SiritRework_GetItemPos()						{ return m_CurEquip_SpiritReWork.m_nPosition;}
	INT							SiritRework_GetIdTable()						{ return m_CurEquip_SpiritReWork.m_IdTable;}


	VOID						SiritRework_SetItemConta(INT nContainer)		{ m_CurEquip_SpiritReWork.m_nContainer = nContainer;}
	VOID						SiritRework_SetItemPos(INT nPosition)			{ m_CurEquip_SpiritReWork.m_nPosition = nPosition;}
	VOID					    SiritRework_SetIdTable(INT id)					{ m_CurEquip_SpiritReWork.m_IdTable = id;}	

	INT							SiritRework_GetMatId(int index)							
	{ 

		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_SpiritReWork.m_Mat[index].m_materialId;

	}

	INT							SiritRework_GetMatNum(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_SpiritReWork.m_Mat[index].m_materialNum;

	}

	INT							SiritRework_GetMatGrade(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  -1;
		return m_CurEquip_SpiritReWork.m_Mat[index].m_materialGrade;

	}

	VOID						SiritRework_RecordMatNum()					    { 	m_CurSynEquip.m_MatNum ++;} 
	VOID						SiritRework_SetMatId(INT Id,int index)			

	{

		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  ;
		m_CurEquip_SpiritReWork.m_Mat[index].m_materialId = Id;	

	}	
	VOID						SiritRework_SetMatNum(INT Num,int index)			
	{
		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  ;
		m_CurEquip_SpiritReWork.m_Mat[index].m_materialNum = Num;

	}	
	VOID					   SiritRework_SetMatGrade(INT Grade,int index)		

	{
		if( 0 >index  && index >= MAX_EQUIP_SPIRIT_RWMATL_NUM)   
			return  ;
		m_CurEquip_SpiritReWork.m_Mat[index].m_materialGrade = Grade;
	}	





	/** 当前正在合成的装备 */
	VOID						SynEquip_CleanUP()								{  m_CurSynEquip.CleanUp();} 

	INT							SynEquip_GetIdTable()							{ return m_CurSynEquip.m_IdTable;}
	INT							SynEquip_GetRecordMatNum()						{ return m_CurSynEquip.m_MatNum;}
	INT							SynEquip_GetMatId(int index)							
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  -1;

		return m_CurSynEquip.m_Mat[index].m_materialId;

	}

	INT							SynEquip_GetMatNum(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  -1;

		return m_CurSynEquip.m_Mat[index].m_materialNum;

	}

	INT							SynEquip_GetMatGrade(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  -1;

		return m_CurSynEquip.m_Mat[index].m_materialGrade;

	}
 
	VOID						SynEquip_RecordMatNum()					    { 	m_CurSynEquip.m_MatNum ++;} 
	VOID					    SynEquip_SetIdTable(INT id)					{  m_CurSynEquip.m_IdTable = id;}	
	VOID						SynEquip_SetMatId(INT Id,int index)			

	{

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  ;
		m_CurSynEquip.m_Mat[index].m_materialId = Id;	

	}	
	VOID						SynEquip_SetMatNum(INT Num,int index)			
	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

       		return  ;
		m_CurSynEquip.m_Mat[index].m_materialNum = Num;

	}	
	VOID					   SynEquip_SetMatGrade(INT Grade,int index)		

	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   

			return  ;
		m_CurSynEquip.m_Mat[index].m_materialGrade = Grade;
	}	


	/** 当前正在拆解的装备 */
	VOID						Dismantle_CleanUP()								{  m_CurEquip_Dis.CleanUp();}
	INT							Dismantle_GetItemConta()						{ return m_CurEquip_Dis.m_nContainer;}
	INT							Dismantle_GetItemPos()							{ return m_CurEquip_Dis.m_nPosition;}
	INT							Dismantle_GetIdTable()							{ return m_CurEquip_Dis.m_IdTable;}

	INT							Dismantle_GetRecordMatNum()						{ return m_CurEquip_Dis.m_MatNum;}
	
	INT							Dismantle_GetMatId(int index)							
		{ 
			
			if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
				return  -1;
			return m_CurEquip_Dis.m_Mat[index].m_materialId;		
		 
		}

	INT							Dismantle_GetMatNum(int index)					
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  -1;
		return m_CurEquip_Dis.m_Mat[index].m_materialNum;
		
	}

	INT							Dismantle_GetMatGrade(int index)					
	
	{ 

		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  -1;
		return m_CurEquip_Dis.m_Mat[index].m_materialGrade;
	
	}
	INT							Dismantle_GetMatLoss(int index)				  
	
	{ 
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  -1;
		return m_CurEquip_Dis.m_Mat[index].m_nMaterialLoss;

	}

	VOID						Dismantle_RecordMatNum()					    { 	m_CurEquip_Dis.m_MatNum ++;}
	VOID						Dismantle_SetItemConta(INT nContainer)			{ m_CurEquip_Dis.m_nContainer = nContainer;}
	VOID						Dismantle_SetItemPos(INT nPosition)				{ m_CurEquip_Dis.m_nPosition = nPosition;}
	VOID					    Dismantle_SetIdTable(INT id)					{  m_CurEquip_Dis.m_IdTable = id;}	

	VOID						Dismantle_SetMatId(INT Id,int index)			
		
	{
		
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  ;
		m_CurEquip_Dis.m_Mat[index].m_materialId = Id;	
	
	}	
	VOID						Dismantle_SetMatNum(INT Num,int index)			
	
	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  ;

		if( Num <=0)
			Num = 0;

		m_CurEquip_Dis.m_Mat[index].m_materialNum = Num;
	
	}	

	VOID						Dismantle_SetMatGrade(INT Grade,int index)		

	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  ;
		m_CurEquip_Dis.m_Mat[index].m_materialGrade = Grade;
	}	

	VOID						Dismantle_SetMatLoss(INT Loss,int index)	

	{
		if( 0 >index  && index >= MAX_EQUIP_PROMATL_NUM)   
			return  ;
			m_CurEquip_Dis.m_Mat[index].m_nMaterialLoss = Loss;
	
	}	
 

	/** 当前正在特修的物品 */
	VOID						SpRepair_CleanUP()								{  m_CurSpRepairEquip.CleanUp();}
	INT							SpRepair_GetItemConta()							{ return m_CurSpRepairEquip.m_nContainer;}
	INT							SpRepair_GetItemPos()							{ return m_CurSpRepairEquip.m_nPosition;}
	INT							SpRepair_GetNeedItemId()						{ return m_CurSpRepairEquip.m_NeedItemId;}
	INT							SpRepair_GetNeedItemNum()						{ return m_CurSpRepairEquip.m_NeedItemNum;}
	INT							SpRepair_GetIntensifyInfo()						{ return m_CurSpRepairEquip.m_nIntensifyInfo;}

	VOID						SpRepair_SetItemConta(INT nContainer)			{ m_CurSpRepairEquip.m_nContainer = nContainer;}
	VOID						SpRepair_SetItemPos(INT nPosition)				{ m_CurSpRepairEquip.m_nPosition = nPosition;}
	VOID						SpRepair_SetNeedItemId(INT nNeedItemId)			{ m_CurSpRepairEquip.m_NeedItemId = nNeedItemId;}
	VOID						SpRepair_SetNeedItemNum(INT nNeedItemNum)		{ m_CurSpRepairEquip.m_NeedItemNum = nNeedItemNum;}
	VOID						SpRepair_SetIntensifyInfo( INT nIntensifyInfo)	{ m_CurSpRepairEquip.m_nIntensifyInfo = nIntensifyInfo;}


	/** 当前正在修理的物品 */
	VOID						ComRepair_CleanUP()								{  m_CurComRepairEquip.CleanUp();}
	INT							ComRepair_GetItemConta()						{ return m_CurComRepairEquip.m_nContainer;}
	INT							ComRepair_GetItemPos()							{ return m_CurComRepairEquip.m_nPosition;}

	VOID						ComRepair_SetItemConta(INT nContainer)			{ m_CurComRepairEquip.m_nContainer = nContainer;}
	VOID						ComRepair_SetItemPos(INT nPosition)				{ m_CurComRepairEquip.m_nPosition = nPosition;}


//---------------------------------------------------------------------------------------------------
/** 好友名单和黑名单 
 */

	/** 清空好友列表 */
	VOID						Relation_ClearFriend();	
	/** 清空黑名单 */
	VOID						Relation_ClearBlack();
	/** 清空仇人 */
	VOID						Relation_ClearEnemy();

	/** 数量 */
	UINT						Relation_GetFriendNum()						{ return m_uFriendCount; }
	UINT						Relation_GetBlackNum()						{ return m_uBlackCount; }
	UINT						Relation_GetEnemyNum()						{ return m_uEnemyCount; }

	VOID						Relation_SetFriendNum(UINT uFriendCount)	{ m_uFriendCount = uFriendCount; }
	VOID						Relation_SetBlackNum(UINT uBlackCount)		{ m_uBlackCount = uBlackCount; }
	VOID						Relation_SetEnemyNum(UINT uEnemyCount)		{ m_uEnemyCount = uEnemyCount; }

	/** 添加一个朋友 */
	VOID						Relation_AddFriend(INT index, _OWN_RELATION* pRelation);	
	/** 添加一个黑名单 */
	VOID						Relation_AddBlack(INT index, _OWN_RELATION* pRelation);	
	//
	VOID						Relation_AddEnemy(INT index, _OWN_RELATION* pRelation);

	/** 删除一个朋友 */
	VOID						Relation_DelFriend();	
	/** 删除一个黑名单 */
	VOID						Relation_DelBlack();
	//
	VOID						Relation_DelEnemy();


	/** 根据索引获取好友数据 */
	BOOL						Relation_GetFriend(INT index, _OWN_RELATION* pRelation);
	/** 根据索引获取黑名单数据 */
	BOOL						Relation_GetBlack(INT index, _OWN_RELATION* pRelation);
	//
	BOOL						Relation_GetEnemy(INT index, _OWN_RELATION* pRelation);

	/** 根据名字来获取数据 */
	BOOL						Relation_GetFriendByName( LPCTSTR name, _OWN_RELATION* pRelation );
	BOOL						Relation_GetBlackByName( LPCTSTR name, _OWN_RELATION* pRelation );
	BOOL						Relation_GetEnemyByName( LPCTSTR name, _OWN_RELATION* pRelation );


	/** 添加新组 */
	VOID						Relation_SetNewGroup( INT index, LPCTSTR name );
	/** 获取新组 */
	LPCTSTR						Relation_GetNewGroup( INT index );

	//存入邀请加为好友的GUID 
	VOID						Relation_SetInviteName( const CHAR* nInviteName )
	{
		strncpy( m_FriendInviteName, nInviteName, MAX_CHARACTER_NAME-1);
	}
	//获得邀请加为好友的名字
	const CHAR*						Relation_GetInviteName()				{ return m_FriendInviteName; }

//---------------------------------------------------------------------------------------------------
/** 搜索相关数据
 */

	/** 清空所有玩家信息 */
	VOID						Finger_ClearInfo();

	/** 索引玩家信息 */
	FingerPlayerInfo_t*			Finger_GetPlayerInfoByIndex(INT nIndex);
	VOID						Finger_SetPlayerInfoByIndex(INT nIndex, FingerPlayerInfo_t* pPalyerInfo);

	/** 玩家数量 */
	INT							Finger_GetPlayerInfoNum()						{ return m_FingerPlayerNum;}
	VOID						Finger_SetPlayerInfoNum(INT iFingerPlayerNum)	{ m_FingerPlayerNum = iFingerPlayerNum;}



//---------------------------------------------------------------------------------------------------
/** 玩家商店数据访问
 */
	
	BOOL						PlayerShop_GetMeIsOwner()					{return m_bMeIsOwner;}
	VOID						PlayerShop_SetMeIsOwner(BOOL bOwner)		{m_bMeIsOwner = bOwner;}

	INT							PlayerShop_GetNpcID()						{return m_nPSNpcID;}
	VOID						PlayerShop_SetNpcID(INT nPSNpcID)			{m_nPSNpcID = nPSNpcID;}

	INT							PlayerShop_GetShopNum()						{return m_nShopNum;}
	VOID						PlayerShop_SetShopNum(INT nShopNum)			{m_nShopNum = nShopNum;}

	FLOAT						PlayerShop_GetCommercialFactor()			{return m_CommerFactor;}
	VOID						PlayerShop_SetCommercialFactor(FLOAT fComFactor)	{ m_CommerFactor = fComFactor;}

	UINT						PlayerShop_GetCost()						{return m_PlayerShopApplyInfo.m_Cost;}
	VOID						PlayerShop_SetCost(UINT uCost)				{m_PlayerShopApplyInfo.m_Cost = uCost;}

	BYTE						PlayerShop_GetType()						{return m_PlayerShopApplyInfo.m_Type;}
	VOID						PlayerShop_SetType(BYTE nType)				{m_PlayerShopApplyInfo.m_Type = nType;}

	LPCTSTR						PlayerShop_GetShopNameByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopNameByIndex(UINT uIndex, LPCTSTR pszShopName);

	LPCTSTR						PlayerShop_GetShopDescByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopDescByIndex(UINT uIndex, LPCTSTR pszShopDesc);

	LPCTSTR						PlayerShop_GetShopFoundedTimeByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopFoundedTimeByIndex(UINT uIndex, LPCTSTR pszTime);

	BYTE						PlayerShop_GetStallNumOpened(UINT uIndex);
	VOID						PlayerShop_SetStallNumOpened(UINT uIndex, BYTE nStallNumOpened);

	BYTE						PlayerShop_GetStallNumOnSale(UINT uIndex);
	VOID						PlayerShop_SetStallNumOnSale(UINT uIndex, BYTE nStallNumOnSale);

	_PLAYERSHOP_GUID			PlayerShop_GetIDByIndex(UINT uIndex);
	VOID						PlayerShop_SetIDByIndex(UINT uIndex, _PLAYERSHOP_GUID nId);

	BYTE						PlayerShop_GetIsFavorByIndex(UINT uIndex);
	VOID						PlayerShop_SetIsFavorByIndex(UINT uIndex, BYTE bIsFavor);

	INT							PlayerShop_GetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex);
	VOID						PlayerShop_SetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex, BYTE bIsEnable);

	BYTE						PlayerShop_GetTypeByIndex(UINT uIndex);
	VOID						PlayerShop_SetTypeByIndex(UINT uIndex, BYTE nType);

	LPCTSTR						PlayerShop_GetOwnerNameByIndex(UINT uIndex);
	VOID						PlayerShop_SetOwnerNameByIndex(UINT uIndex, LPCTSTR pszOwnerName);

	UINT						PlayerShop_GetOwnerGuidByIndex(UINT uIndex);
	VOID						PlayerShop_SetOwnerGuidByIndex(UINT uIndex, UINT uOwnerGuid);

	/** 清空玩家商店 */
	VOID						PlayerShop_CleanUp(BOOL bIsMine);

	// 设置商店ID
	VOID						PlayerShop_SetShopID(BOOL bIsMine, _PLAYERSHOP_GUID nShopID);
	/** 获得商店ID */
	_PLAYERSHOP_GUID			PlayerShop_GetShopID(BOOL bIsMine);

	// 设置商店序列号
	VOID						PlayerShop_SetShopSerial(BOOL bIsMine, BYTE nShopSerial);
	// 获得商店序列号
	BYTE						PlayerShop_GetShopSerial(BOOL bIsMine);

	//设置摊位物品
	VOID						PlayerShop_SetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//获得制定位置的物品
	tObject_Item*				PlayerShop_GetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);

	//设置物品价格
	VOID						PlayerShop_SetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nPrice);
	//获得物品价格
	UINT						PlayerShop_GetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);

	//设置物品序列号
	VOID						PlayerShop_SetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nSerial);
	//获得物品序列号
	UINT						PlayerShop_GetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);

	//此物体是否上架
	VOID						PlayerShop_SetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BYTE bIsEnable);
	//此物体是否上架
	BYTE						PlayerShop_GetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);

	//设置商店本金
	VOID						PlayerShop_SetBaseMoney(BOOL bIsMine, UINT uBaseMoney);
	//设置商店本金
	UINT						PlayerShop_GetBaseMoney(BOOL bIsMine);

	//设置商店盈利金
	VOID						PlayerShop_SetProfitMoney(BOOL bIsMine, UINT uProfitPrice);
	//设置商店盈利金
	UINT						PlayerShop_GetProfitMoney(BOOL bIsMine);

	//只有自己的店可以看到
	INT							PlayerShop_GetPartnerNum()					{ return m_nPartnerNum;}
	VOID						PlayerShop_SetPartnerNum(INT nPartnerNum)	{ m_nPartnerNum = nPartnerNum;}

	//设置指定位置的合作伙伴
	VOID						PlayerShop_SetPartnerByIndex(UINT uIndex, GUID_t Guid, LPCTSTR szName);
	//获得指定位置的合作伙伴
	PlayerShopPartners_t* 		PlayerShop_GetPartnerByIndex(UINT uIndex);

	//设置商店类型
	VOID						PlayerShop_SetShopType(BOOL bIsMine, INT nShopType);
	//设置商店类型
	UINT						PlayerShop_GetShopType(BOOL bIsMine);

	//设置商店此商店是否是出售状态
	VOID						PlayerShop_SetIsSaleOut(BOOL bIsMine, BOOL bIsSaleOut);
	//设置商店此商店是否是出售状态
	BOOL						PlayerShop_GetIsSaleOut(BOOL bIsMine);

	//设置商店此商店是否是出售状态
	VOID						PlayerShop_SetSaleOutPrice(BOOL bIsMine, UINT uSaleOutPrice);
	//获得商店的盘出价
	UINT						PlayerShop_GetSaleOutPrice(BOOL bIsMine);

	//设置店主guid
	VOID						PlayerShop_SetOwnerGuid(BOOL bIsMine, UINT uOwnerGuid);
	//获得店主guid
	UINT						PlayerShop_GetOwnerGuid(BOOL bIsMine);

	//设置交易记录数量
	VOID						PlayerShop_SetExRecNum(BOOL bIsMine, UINT uExRecNum);
	//获得交易记录数量
	UINT						PlayerShop_GetExRecNum(BOOL bIsMine);

	//设置管理记录数量
	VOID						PlayerShop_SetMaRecNum(BOOL bIsMine, UINT uMaRecNum);
	//获得管理记录数量
	UINT						PlayerShop_GetMaRecNum(BOOL bIsMine);

	//设置已经拥有的柜台数
	VOID						PlayerShop_SetOpenStallNum(BOOL bIsMine, UINT uOpenStallNum);
	//获得已经拥有的柜台数
	UINT						PlayerShop_GetOpenStalldNum(BOOL bIsMine);

	//设置商店的类型
	VOID						PlayerShop_SetShopSubType(BOOL bIsMine, UINT uShopSubType);
	//获得商店的类型
	UINT						PlayerShop_GetShopSubType(BOOL bIsMine);

	//设置店主名
	VOID						PlayerShop_SetOwnerName(BOOL bIsMine, LPCTSTR pszOwnerName);
	//获得店主名
	LPCTSTR						PlayerShop_GetOwnerName(BOOL bIsMine);

	//设置商店名
	VOID						PlayerShop_SetShopName(BOOL bIsMine, LPCTSTR pszShopName);
	//获得商店名
	LPCTSTR						PlayerShop_GetShopName(BOOL bIsMine);

	//设置商店描述
	VOID						PlayerShop_SetShopDesc(BOOL bIsMine, LPCTSTR pszShopDesc);
	//获得商店名
	LPCTSTR						PlayerShop_GetShopDesc(BOOL bIsMine);

	//设置物品详细信息
	VOID						PlayerShop_SetItemExtraInfo(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	//获得摊位中的物品数量
	INT							PlayerShop_GetItemNum(BOOL bIsMine, INT nStallIndex);
	//根据GUID查找物品位置
	INT							PlayerShop_GetItemIndexByGUID(BOOL bIsMine, INT nStallIndex, __int64 ItemGUID);

	SDATA_PET*					PlayerShop_GetPet(BOOL bIsMine, INT nStallIndex, INT nIndex);

	//根据GUID在玩家商店指定柜台中查找物品
	INT							PlayerShop_GetPetByGuid(BOOL bIsMine, INT nStallIndex, PET_GUID_t PetGuid);
	//在指定柜台中设置宠物
	VOID						PlayerShop_SetPet(BOOL bIsMine, INT nStallIndex, INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	//设置指定柜台的格子序列号
	VOID						PlayerShop_SetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSerial);
	//获得指定柜台的格子序列号
	INT							PlayerShop_GetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex);
	VOID						PlayerShop_SetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nPetPrice);
	INT							PlayerShop_GetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex);
	VOID						PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);	
	VOID						PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex, const _OWN_SKILL *Skill);
	PET_SKILL*					PlayerShop_GetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex);

	//清除
	VOID						PlayerShop_CleanObjItem(BOOL bIsMine);

	//玩家商店上选中的物品
	VOID						PlayerShop_MyClearSelect()					{ m_MyPsSelectItem.m_nConIndex = -1;m_MyPsSelectItem.m_nPosition = -1;}
	VOID						PlayerShop_OtClearSelect()					{ m_OtPsSelectItem.m_nConIndex = -1;m_OtPsSelectItem.m_nPosition = -1;}

	INT							PlayerShop_GetMySelectConTa()				{ return m_MyPsSelectItem.m_nConIndex;}
	INT							PlayerShop_GetMySelectPos()					{ return m_MyPsSelectItem.m_nPosition;}
	INT							PlayerShop_GetOtSelectConTa()				{ return m_OtPsSelectItem.m_nConIndex;}
	INT							PlayerShop_GetOtSelectPos()					{ return m_OtPsSelectItem.m_nPosition;}

	VOID						PlayerShop_SetMySelectConTa(INT nConIndex)	{ m_MyPsSelectItem.m_nConIndex = nConIndex;}
	VOID						PlayerShop_SetMySelectPos(INT nPosition)	{ m_MyPsSelectItem.m_nPosition = nPosition;}
	VOID						PlayerShop_SetOtSelectConTa(INT nConIndex)	{ m_OtPsSelectItem.m_nConIndex = nConIndex;}
	VOID						PlayerShop_SetOtSelectPos(INT nPosition)	{ m_OtPsSelectItem.m_nPosition = nPosition;}


//---------------------------------------------------------------------------------------------------
/** 帮派列表
 */
	
	//清空所有工会信息
	VOID						Guild_ClearAllInfo();
	//通过索引获得工会信息
	GuildInfo_t*				Guild_GetInfoByIndex(INT nIndex);
	//通过索引设置工会信息
	VOID						Guild_SetInfoByIndex(INT nIndex, GuildInfo_t* pGuildInfo);

	//获得工会数量
	INT							Guild_GetInfoNum()							{ return m_GuildNum;}
	//设置工会数量
	VOID						Guild_SetInfoNum(INT iGuildNum)				{ m_GuildNum = iGuildNum;}


/** 帮派成员列表
 */
	
	/** 清空所有帮众信息 */
	VOID						Guild_ClearMemInfo();
	//通过索引获得帮众信息
	GuildMemberInfo_t*			Guild_GetMemInfoByIndex(INT nIndex);
	//通过索引设置帮众信息
	VOID						Guild_SetMemInfoByIndex(INT nIndex, GuildMemberInfo_t* pMemberInfo);

	//获得帮众信息
	INT							Guild_GetMemInfoNum()						{ return m_GuildMemNum;}
	//设置帮众信息
	VOID						Guild_SetMemInfoNum(INT iMemNum)			{ m_GuildMemNum = iMemNum;}

	//获得帮众的最大数量
	INT							Guild_GetMaxMemNum()						{ return m_GuildMaxMemNum;}
	//设置帮众的最大数量
	VOID						Guild_SetMaxMemNum(INT iMaxMemNum)			{ m_GuildMaxMemNum = iMaxMemNum;}

	//获得帮派宗旨
	LPCTSTR						Guild_GetDesc()								{return m_GuildDesc.c_str();}
	//设置帮派宗旨
	VOID						Guild_SetDesc(LPCTSTR pGuildDesc)			{m_GuildDesc = pGuildDesc;}

	//获得帮派宗旨
	LPCTSTR						Guild_GetName()								{return m_GuildName.c_str();}
	//设置帮派宗旨
	VOID						Guild_SetName(LPCTSTR pGuildName)			{m_GuildName = pGuildName;}

	//获得当前人物权限
	BYTE						Guild_GetCurAccess()						{return m_uAccess;}
	//设置当前人物权限
	VOID						Guild_SetCurAccess(BYTE uAccess)			{m_uAccess = uAccess;}

	//获得当前人物职位
	BYTE						Guild_GetCurPosition()						{return m_uPosition;}
	//设置当前人物职位
	VOID						Guild_SetCurPosition(BYTE uPosition)		{m_uPosition = uPosition;}


/** 帮派的详细信息
 */

	//清空帮派的详细信息
	VOID						Guild_ClearDetailInfo();
	//获得帮派的详细信息
	GuildDetailInfo_t*			Guild_GetDetailInfo();
	//设置帮派的详细信息
	VOID						Guild_SetDetailInfo(GuildDetailInfo_t* pDetailInfo);


/** 帮派可任命职位
 */
	
	//清空帮派可任命职位
	VOID						Guild_ClearAppointInfo();
	//获得帮派可任命职位
	GuildAppointPos_t*			Guild_GetAppointInfoByIndex(INT nIndex);
	//设置帮派可任命职位
	VOID						Guild_SetAppointInfoByIndex( INT nIndex, GuildAppointPos_t* pDetailInfo);


/** 帮会邀请信息
*/

	// 保存邀请者和其帮会的ID
	VOID						Guild_SetInviteGuildId( INT nGuildId );
	VOID						Guild_SetInviteGuildName( LPCTSTR pGuildName );
	VOID						Guild_SetInviteGuildObjName( LPCTSTR pGuildObjName );
	// 
	INT							Guild_GetInviteGuildId( VOID );
	LPCTSTR						Guild_GetInviteGuildName( VOID );
	LPCTSTR						Guild_GetInviteGuildObjName( VOID );


	// 发送获取成员信息的消息
	VOID						Guild_AskInfo( INT index );



/**	帮会家族信息
*/

	//获得家族数量
	INT							Guild_GetFamilyNum()								{ return m_iGuildFamilyNum;}
	//设置家族数量
	VOID						Guild_SetFamilyNum(INT iGuildFamilyNum)				{ m_iGuildFamilyNum = iGuildFamilyNum;}

	//清空帮派家族信息
	VOID						Guild_ClearFamilyInfo();
	//获得帮派家族信息
	GuildFamilyInfo*			Guild_GetFamilyInfoByIndex(INT nIndex);
	//设置帮派的详细信息
//	VOID						Guild_SetDetailFamilyInfo(GuildFamilyInfo* pDetailInfo);


	//通过索引设置家族信息
	VOID						Guild_SetFamilyMemInfoByIndex(INT nIndex, GuildFamilyInfo* pFamilyInfo);

	// 
	VOID						Guild_SetSelectFamilyId( INT familyId )				{ m_iSelectFamilyId = familyId; };
	INT							Guild_GetSelectFamilyId()							{ return m_iSelectFamilyId; };



	// 界面的家族数据
	VOID						Guild_AddFamilyWindowData( INT index, STRING winName );
	VOID						Guild_DelFamilyWindowData( INT index );
	STRING						Guild_GetFamilyWindowData( INT index );
	


//---------------------------------------------------------------------------------------------------

/** 称号数据
 */


	//称号数量
	VOID						Title_SetTitleNum( INT nType, INT num );
	INT							Title_GetTitleNum( INT nType );
	//当前称号
	VOID						Title_SetCurTitle( INT nType, INT id );
	INT							Title_GetCurTitle( INT nType );
	//设置称号
//	VOID						Title_SetCountry( INT index, _TITLE_* title );
	VOID						Title_SetTitle( INT nType, INT nIndex, INT nTitleIndex );
	INT							Title_GetTitle( INT nType, INT index );		// 取出的是在人物数据中保存的数组的索引
	
	//temp
/*
	//家族称号数量
	VOID						Title_SetGuildNum( INT num )						{ m_iGuildTitleNum = num; };
	INT							Title_GetGuildNum( VOID )							{ return m_iGuildTitleNum;}
	// 
	VOID						Title_SetCurGuild( INT id )							{ m_iCurrGuildTitleId = id; };
	INT							Title_GetCurGuild( VOID )							{ return m_iCurrGuildTitleId; };
	//设置家族称号
	VOID						Title_SetGuild( INT index, _TITLE_* title );
	_TITLE_*					Title_GetGuild( INT index );


	//获得玩法称号数量
	VOID						Title_SetWanfaNum( INT num )						{ m_iWanfaTitleNum = num; };
	INT							Title_GetWanfaNum( VOID )							{ return m_iWanfaTitleNum; }
	// 
	VOID						Title_SetCurWanfa( INT id )							{ m_iCurrWanfaTitleId = id; };
	INT							Title_GetCurWanfa( VOID )							{ return m_iCurrWanfaTitleId; };
	//设置玩法称号
	VOID						Title_SetWanfa( INT index, _TITLE_* title );
	_TITLE_*					Title_GetWanfa( INT index );
*/

/*
	//称号数量
	VOID						Title_SetNum( INT num, INT nType );
	INT							Title_GetNum( INT nType );
	//当前称号id
	VOID						Title_SetCur( INT id, INT nType );
	INT							Title_GetCur( INT nType );
	//称号
	VOID						Title_Set( INT index, INT nType, _TITLE_* title );
	_TITLE_*					Title_Get( INT index, INT nType );
*/


	
//---------------------------------------------------------------------------------------------------

/** 国家数据
 */

	VOID						Country_Set( INT nType, STRING strName );
	CountryFunctionaryInfo*		Country_Get( INT nType );



//---------------------------------------------------------------------------------------------------

/** 宠物门派技能学习数据访问
 */

	VOID						PetSkillStudy_MenPaiList_AddSkill(INT skillId);
	VOID						PetSkillStudy_MenPaiList_Clear();
	PET_SKILL*					PetSkillStudy_MenPaiList_Get(INT idx);	//idx 是 m_petSkillStudyList 的索引
	PET_SKILL*					PetSkillStudy_MenPaiList_Get_BySkillId(INT sdx); //sdx 是 PET_SKILL中的 m_pDefine->m_nID


/**	宠物公告板
 */
	
	INT							PetPlacard_GetItemCount(VOID)const;
	const _PET_PLACARD_ITEM*	PetPlacard_GetItem(INT nIndex)const;
	VOID						PetPlacard_AddItem(const _PET_PLACARD_ITEM *pItem);
	VOID						PetPlacard_CleanUp();



//---------------------------------------------------------------------------------------------------

/** 玩家资料
 */

	virtual INT					PI_GetMyInfoType()								{return m_MyPrivateInfo.m_nInfoType;}
	virtual INT					PI_GetMyAge()									{return m_MyPrivateInfo.m_nAge;}
	virtual INT					PI_GetMySex()									{return m_MyPrivateInfo.m_nSex;}
	virtual INT					PI_GetMyBloodType()								{return m_MyPrivateInfo.m_nBloodType;}
	virtual INT					PI_GetMyYearAnimal()							{return m_MyPrivateInfo.m_nYearAnimal;}
	virtual INT					PI_GetMyConsella()								{return m_MyPrivateInfo.m_nConsella;}
	virtual INT					PI_GetMyProvince()								{return m_MyPrivateInfo.m_nProvince;}

	virtual LPCTSTR				PI_GetMyJobInfo()								{return m_MyPrivateInfo.m_szJobInfo.c_str();}
	virtual LPCTSTR				PI_GetMySchoolInfo()							{return m_MyPrivateInfo.m_szSchoolInfo.c_str();}
	virtual LPCTSTR				PI_GetMyCity()									{return m_MyPrivateInfo.m_szCity.c_str();}
	virtual LPCTSTR				PI_GetMyEmailInfo()								{return m_MyPrivateInfo.m_szEmailInfo.c_str();}
	virtual LPCTSTR				PI_GetMyLuckWord()								{return m_MyPrivateInfo.m_szLuckWord.c_str();}

	virtual INT					PI_GetOtInfoType()								{return m_OtPrivateInfo.m_nInfoType;}
	virtual INT					PI_GetOtAge()									{return m_OtPrivateInfo.m_nAge;}
	virtual INT					PI_GetOtSex()									{return m_OtPrivateInfo.m_nSex;}
	virtual INT					PI_GetOtBloodType()								{return m_OtPrivateInfo.m_nBloodType;}
	virtual INT					PI_GetOtYearAnimal()							{return m_OtPrivateInfo.m_nYearAnimal;}
	virtual INT					PI_GetOtConsella()								{return m_OtPrivateInfo.m_nConsella;}
	virtual INT					PI_GetOtProvince()								{return m_OtPrivateInfo.m_nProvince;}

	virtual LPCTSTR				PI_GetOtName()									{return m_OtPrivateInfo.m_szName.c_str();}
	virtual LPCTSTR				PI_GetOtJobInfo()								{return m_OtPrivateInfo.m_szJobInfo.c_str();}
	virtual LPCTSTR				PI_GetOtSchoolInfo()							{return m_OtPrivateInfo.m_szSchoolInfo.c_str();}
	virtual LPCTSTR				PI_GetOtCity()									{return m_OtPrivateInfo.m_szCity.c_str();}
	virtual LPCTSTR				PI_GetOtEmailInfo()								{return m_OtPrivateInfo.m_szEmailInfo.c_str();}
	virtual LPCTSTR				PI_GetOtLuckWord()								{return m_OtPrivateInfo.m_szLuckWord.c_str();}

	virtual VOID				PI_SetMyInfoType(int nInfoType)					{m_MyPrivateInfo.m_nInfoType = nInfoType;}
	virtual VOID				PI_SetMyAge(int nAge)							{m_MyPrivateInfo.m_nAge = nAge;}
	virtual VOID				PI_SetMySex(int nSex)							{m_MyPrivateInfo.m_nSex = nSex;}
	virtual VOID				PI_SetMyBloodType(int nBloodType)				{m_MyPrivateInfo.m_nBloodType = nBloodType;}
	virtual VOID				PI_SetMyYearAnimal(int nYearAnimal)				{m_MyPrivateInfo.m_nYearAnimal = nYearAnimal;}
	virtual VOID				PI_SetMyConsella(int nConsella)					{m_MyPrivateInfo.m_nConsella = nConsella;}
	virtual VOID				PI_SetMyProvince(int nProvince)					{m_MyPrivateInfo.m_nProvince = nProvince;}

	virtual VOID				PI_SetMyJobInfo(LPCTSTR szJobInfo)				{m_MyPrivateInfo.m_szJobInfo = szJobInfo;}
	virtual VOID				PI_SetMySchoolInfo(LPCTSTR szSchoolInfo)		{m_MyPrivateInfo.m_szSchoolInfo = szSchoolInfo;}
	virtual VOID				PI_SetMyCity(LPCTSTR szCity)					{m_MyPrivateInfo.m_szCity = szCity;}
	virtual VOID				PI_SetMyEmailInfo(LPCTSTR szEmailInfo)			{m_MyPrivateInfo.m_szEmailInfo = szEmailInfo;}
	virtual VOID				PI_SetMyLuckWord(LPCTSTR szLuckWord)			{m_MyPrivateInfo.m_szLuckWord = szLuckWord;}

	virtual VOID				PI_SetOtInfoType(int nInfoType)					{m_OtPrivateInfo.m_nInfoType = nInfoType;}
	virtual VOID				PI_SetOtAge(int nAge)							{m_OtPrivateInfo.m_nAge = nAge;}
	virtual VOID				PI_SetOtSex(int nSex)							{m_OtPrivateInfo.m_nSex = nSex;}
	virtual VOID				PI_SetOtBloodType(int nBloodType)				{m_OtPrivateInfo.m_nBloodType = nBloodType;}
	virtual VOID				PI_SetOtYearAnimal(int nYearAnimal)				{m_OtPrivateInfo.m_nYearAnimal = nYearAnimal;}
	virtual VOID				PI_SetOtConsella(int nConsella)					{m_OtPrivateInfo.m_nConsella = nConsella;}
	virtual VOID				PI_SetOtProvince(int nProvince)					{m_OtPrivateInfo.m_nProvince = nProvince;}

	virtual VOID				PI_SetOtName(LPCTSTR szName)					{m_OtPrivateInfo.m_szName = szName;}
	virtual VOID				PI_SetOtJobInfo(LPCTSTR szJobInfo)				{m_OtPrivateInfo.m_szJobInfo = szJobInfo;}
	virtual VOID				PI_SetOtSchoolInfo(LPCTSTR szSchoolInfo)		{m_OtPrivateInfo.m_szSchoolInfo = szSchoolInfo;}
	virtual VOID				PI_SetOtCity(LPCTSTR szCity)					{m_OtPrivateInfo.m_szCity = szCity;}
	virtual VOID				PI_SetOtEmailInfo(LPCTSTR szEmailInfo)			{m_OtPrivateInfo.m_szEmailInfo = szEmailInfo;}
	virtual VOID				PI_SetOtLuckWord(LPCTSTR szLuckWord)			{m_OtPrivateInfo.m_szLuckWord = szLuckWord;}

	virtual UINT				PI_GetMyCharGUID()								{return m_MyPrivateInfo.m_unCharGUID;}
	virtual UINT				PI_GetOtCharGUID()								{return m_OtPrivateInfo.m_unCharGUID;}

	virtual VOID				PI_SetMyCharGUID(UINT unCharGUID) 				{m_MyPrivateInfo.m_unCharGUID = unCharGUID;}
	virtual VOID				PI_SetOtCharGUID(UINT unCharGUID) 				{m_OtPrivateInfo.m_unCharGUID = unCharGUID;}

	virtual VOID				PI_SetObjId(INT nObjId)							{m_nObjId = nObjId;}
	virtual INT					PI_GetObjId()									{return m_nObjId;}
	
	RecordList_t*				GetRecordList()									{return &m_RecordList;}



//---------------------------------------------------------------------------------------------------

/** 系统提示信息
 */

	// 添加事件( Param 现在只给组队的使用 )
	BOOL						SystemTip_AddEventInfo( INT type, const STRING& strDesc, INT Param = -1 );
	// 删除事件
	BOOL						SystemTip_DelEventInfo( INT nIndex );

	// 获取某一事件的信息
	SystemTipEvent*				SystemTip_GetEventInfo( INT nIndex );
	// 设置某一事件的状态
	BOOL						SystemTip_SetEventState(  );

	// 获取当前界面索引
	INT							SystemTip_GetUIIndex();

	// 根据类型和参数来获取相应的事件在UI的索引
	INT							SystemTip_GetIndexByType( INT type, INT Param );


//---------------------------------------------------------------------------------------------------
// 财产保护界面

	VOID						Rich_SetPasswordData( INT data )				{ m_PasswordData = data; }
	INT							Rich_GetPasswordData( VOID )					{ return m_PasswordData; }

	BOOL						Rich_IsSet( MINORPASSWD_INFO::PROTECT_TYPE type );

	// 输入的密码， 勇于密码保护时的密码输入
	VOID						Rich_SetInputPassword( const STRING& passwd )	{ m_InputPassword = passwd; };
	STRING						Rich_GetInputPassword( VOID )					{ return m_InputPassword; };

	// 设置密码输入的状态（分类）
	VOID						Rich_SetInputPasswdState( INT nType, BOOL bState );
	BOOL						Rich_GetInputPasswdState( INT nType );




//---------------------------------------------------------------------------------------------------
/** 鼠标命令
 */

	//temp 只对好友一项测试

	// 鼠标命令
	void SetMouseCmd();
	// 取消鼠标命令
	void CancelMouseCmd();



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//数据存储			

protected:


/** 角色属性池
 */

	/// 所有角色的逻辑数据
	std::map< INT, CCharacterData* >	m_mapAllCharacterData;


/** 冷却时间组
 */

	std::vector< COOLDOWN_GROUP >		m_vCoolDownGroup;
	std::vector< COOLDOWN_GROUP >		m_vPetSkillCoolDownGroup;
	
/** 公共冷却
 */

	INT									m_nCommonCoolDown;
	
/** 任务剩余时间组
 */

	std::vector< INT >					m_vQuestTimeGroup;
	
/** 玩家自己装备
 */

	/// 玩家装备实例数组
	std::vector< tObject_Item* >		m_vUserEquip;

/** 别人装备
 */

	/// 别人装备实例数组
	std::vector< tObject_Item* >		m_vOtherPlayerEquip;

/** 任务列表界面
 */

	/// 玩家任务的所有物品
	struct QUESTLOG_BOX
	{
		std::vector<tObject_Item*>		m_ItemList;					// 标示交易栏中自己的物品
		VOID CleanUp();
	};

	std::vector< QUESTLOG_BOX >			m_vQuestLogItem;

/** 查看对方角色属性界面
 */

	/// 对方的Obj
	CObject*							m_pTargetEquip;


/** 主角背包列表
 */

	/// 玩家背包装备实例数组
	std::vector< tObject_Item* >		m_vUserBag;

	/// 详细请求队列
	std::set< INT >						m_setAskBagExtra;


/** 主角坐骑列表  [7/8/2010 陈军龙] 
*/

	/// 玩家坐骑实例数组
	std::vector< tObject_Item* >		m_vUserMount;


/** 主角搜侠录道具列表
*/
	
	///玩家搜侠录道具详细信息实例数组
	SOUXIA_DATA*	m_vUserSouXia[MAX_SOUXIA_CONTAINER];

	BYTE		m_nSouXiaCount;		//学习过的搜侠录总数
	BYTE		m_nSouXiaTypeCount;		//该搜侠录具有的类型总数



/** 当前打开的箱子
 */

	/// 箱子ID
	INT									m_idItemBox;
	/// 箱子物品实例数组
	std::vector< tObject_Item* >		m_vItemBox;


	/** 当前掉落的物品
	*/

	INT									m_idItemDrop;

	///掉落 物品实例
	std::map< INT,tObject_Item*>  		m_vItemDrop;


/** 当前打开的商人
 */

	INT									m_nBoothNumber;
	INT									m_nBoothSoldNumber;
	std::vector< tObject_Item* >		m_listBooth;
	std::vector< tObject_Item* >		m_listBoothSold;
	BOOL								m_bBoothWindowIsDirty;
//	BOOL								m_bIsRepairing;
	std::vector< UINT >					m_nlistPrice;
	INT									m_nRepairLevel;		/// 修理等级
	INT									m_nBuyLevel;		/// 收购等级
	INT									m_nRepairType;		/// 修理类型
	INT									m_nBuyType;			/// 商店的收购类型
	FLOAT								m_nRepairSpend;		/// 修理花费
	FLOAT								m_nRepairOkProb;	/// 修理成功几率

	/// 当前Shop的NpcId
	INT									m_nShopNpcId;		// 
	
	// add for IB Shop
	INT									m_nIBShopNumber;
	std::vector< tObject_Item* >		m_listIBShop;
	std::vector< UINT >					m_nlistIBPrice;
	std::vector< UINT >					m_nlistNewSell;		//新产品

	// end of  add

/** 当前打开的银行
 */

	/// 主角银行物品列表
	std::vector< tObject_Item* >		m_listUserBank;
	/// 当前银行中已经打开的位置
	INT									m_CurBankEndIndex;
	/// 当前银行中钱数
	INT									m_CurBankMoney;
	/// 当前银行NPC的ID
	INT									m_nBankNpcID;


/** 关系系统部分
 */
	Relation*							m_pRelation;
	CMailPool*							m_pMailPool;

	
/** 交易盒
 */

	struct  CLIENT_EXCHANGE_BOX
	{
		BOOL							m_IsLocked;						/// 标示界面上lock选项是否钩上
		BOOL							m_CanConform;					/// 标示是否显示确定按钮
		UINT							m_Money;						/// 标示交易的金钱

		std::vector<tObject_Item*>		m_ItemList;						/// 标示交易栏中自己的物品
		std::vector< SDATA_PET* >		m_Petlist;						/// 标示交易栏中自己的宠物

		VOID CleanUp();
	};

	CLIENT_EXCHANGE_BOX					m_MyExchangeBox;				/// 自己的交易盒
	CLIENT_EXCHANGE_BOX					m_OtherExchangeBox;				/// 对方的交易盒
	INT									m_CurExID;
	std::vector< INT >					m_listApplicantsID;				/// 申请交易序列
	SHORT								m_pOut;							/// 从队列中取出的位置指针
	SHORT								m_pIn;							/// 加入队列的位置指针


	//宝石合成/镶嵌界面数组
//	std::vector< tObject_Item* >		m_vPageEnchase;

	// 装备上附加物品的数组，以后再加
//	std::vector< tObject_Item* >		m_vAppendItem;					/// 附加的物品。


/** 任务盒
 */

	std::vector<tObject_Item*>			m_MissionItemList;				/// 递交任务物品界面


/** BBS上的留言
 */

	struct MessageEntry_t
	{
		STRING	AuthorName; 
		UINT	nID;
		BYTE	nHour;
		BYTE	nMin;
		BOOL	bHasReply;
		STRING	szMessage;
		BYTE	nReHour;
		BYTE	nReMin;
		STRING	szReplyMessage;

		MessageEntry_t()
		{
			AuthorName		=	"";		
			nID				=	0;
			nHour			=	0;
			nMin			=	0;
			bHasReply		=	0;
			szMessage		=	"";
			nReHour			=	0;
			nReMin			=	0;
			szReplyMessage	=	"";
		}
	};

	struct BBS_t
	{
		BBS_t()
		{
			CleanUp();
		}
		VOID	CleanUp();

		STRING							m_szTitle;
		UINT							m_nSerial;
		INT								m_nMessageNum;
		std::vector< MessageEntry_t >	m_MessageList;
		
	};


/** 摊位盒
 */

	enum STALL_DEFAULT_PAGE
	{
		ITEM_PAGE = 0,     //物品页
		PET_PAGE,
	};

	struct STALL_ITEM
	{
		STALL_ITEM()
		{
			pItem	= NULL;
			nPrice	= 0;
			nSerial = 0;
			nbIsEnable	=	0;
		}
		tObject_Item*	pItem;			/// 物品指针
		UINT			nPrice;			/// 价格
        UINT			nSerial;		/// 序列号
		BYTE			nbIsEnable;		/// 这个物品是否上架
	};

	struct STALL_PET
	{
		STALL_PET()
		{
			pPet	= NULL;
			nPrice	= 0;
			nSerial = 0;
			nbIsEnable	=	TRUE;
		}
		SDATA_PET*	pPet;			/// 宠物指针
		UINT		nPrice;			/// 价格
		UINT		nSerial;		/// 序列号
		BYTE		nbIsEnable;		/// 这个物品是否上架
	};

	struct CLIENT_STALL_BOX
	{
		INT								m_nDefaultPage;			/// 缺省的页面
		UINT							m_ObjID;
		BOOL							m_bIsOpen;				/// 自己摊位是否已经打开（是否处于交易状态）
		UINT							m_PosTax;				/// 摊位费
		UINT							m_TradeTax;				/// 每次交易税收
		STRING							m_szStallName;			/// 标示摊位名
		STRING							m_szOwnerName;			/// 标示摊主名
		UINT							m_nFirstPage;			/// 起始页
		std::vector< STALL_ITEM >	 	m_ItemList;				/// 标示摊位中的所有物品
		std::vector< STALL_PET >	 	m_PetList;				/// 标示摊位中的所有宠物
		BBS_t							m_BBS;					/// 摊位留言板
		UINT							m_OtGUID;
		VOID							CleanUp();		
	};

	CLIENT_STALL_BOX					m_MyStallBox;			/// 自己的摊位盒
	CLIENT_STALL_BOX					m_OtStallBox;			/// 对方的摊位盒
	STALL_ITEM							m_CurSelectItem;		/// 玩家选中的物品
	INT									m_nSelectItemInPos;		/// 选中的这个物品所在的容器
	PET_GUID_t							m_CurSelectpetGuid;		/// 选中等待上架的宠物的GUID
	INT									m_nSelectPetOnStall;	/// 在摊位上被选中的宠物编号


/** 玩家商店相关
 */

	PLAYERSHOP_APPLY_INFO				m_PlayerShopApplyInfo;		/// 申请商店信息

	std::vector< PLAYERSHOP_INFO >	 	m_PlayerShopList;			/// 商店列表
	INT									m_nShopNum;

	/// 标示摊位中的所有物品
	struct PLAYERSHOP_STALL_BOX										
	{
		INT								m_bIsEnable;
		std::vector< STALL_ITEM >	 	m_ItemList;
		std::vector< STALL_PET >	 	m_PetList;

		PLAYERSHOP_STALL_BOX()
		{
			m_bIsEnable = FALSE;
			STALL_ITEM	ItemInit;
			m_ItemList.resize(STALL_BOX_SIZE, ItemInit);
			STALL_PET	PetInit;
			m_PetList.resize(STALL_BOX_SIZE, PetInit);
		}

		VOID CleanUp()
		{
			m_bIsEnable = FALSE;
			for(INT i =0; i<STALL_BOX_SIZE; i++)
			{
				STALL_ITEM	ItemInit;
				m_ItemList[i] = ItemInit;

				STALL_PET	PetInit;
				m_PetList[i] = PetInit;
			}
		}
	};

	enum SHOP_TYPE
	{
		NO_SHOP = 0,
		ITEM_SHOP,
		PET_SHOP,
	};

	/// 标示商店中的所有柜台
	struct PLAYERSHOP_BOX												
	{
		std::vector< PLAYERSHOP_STALL_BOX >	m_StallList;
		UINT								m_BaseMoney;
		UINT								m_ProfitMoney;
		_PLAYERSHOP_GUID					m_ShopID;
		STRING								m_szShopName;		/// 标示商店名
		STRING								m_szShopDesc;		/// 标示商店描述
		STRING								m_szOwnerName;		/// 标示店主名
		UINT								m_OwnerGuid;		/// 店主GUID
		BYTE								m_bIsSaleOut;		/// 此商店已被盘出
		UINT								m_uSaleOutPrice;	/// 商店的盘出价
		BYTE								m_Serial;			/// 商店的序列号
		INT									m_nShopType;		/// 商店的类型
		BYTE								m_ExRecListNum;		/// 交易记录数量
		BYTE								m_MaRecListNum;		/// 管理记录数量
		BYTE								m_StallOpenNum;		/// 已经拥有的柜台数
		BYTE								m_ShopSubType;		/// 商店子类型
		std::vector< PlayerShopPartners_t > m_PartnerList;		/// 合伙人列表
		
		PLAYERSHOP_BOX()
		{
			m_BaseMoney		= 0;
			m_ProfitMoney	= 0;
			m_szShopName	=	"";
			m_szOwnerName	=	"";
			m_szShopDesc	=	"";
			m_OwnerGuid		=	0;
			m_bIsSaleOut	=	FALSE;
			m_uSaleOutPrice	=	0;
			m_Serial		=	0;
			m_ExRecListNum	=	0;
			m_MaRecListNum	=	0;
			m_StallOpenNum	=	0;
			PLAYERSHOP_STALL_BOX StallInit;
			m_StallList.resize(MAX_STALL_NUM_PER_SHOP, StallInit);
			PlayerShopPartners_t PartnerInit;
			m_PartnerList.resize(MAX_PARTNER_PER_SHOP, PartnerInit);
			m_nShopType		= NO_SHOP;
			m_ShopSubType	=	0;
		}

		VOID CleanUp()
		{
			m_BaseMoney		= 0;
			m_ProfitMoney	= 0;
			m_szShopName	=	"";
			m_szOwnerName	=	"";
			m_OwnerGuid		=	0;
			m_bIsSaleOut	=	FALSE;
			m_uSaleOutPrice	=	0;
			m_Serial		=	0;
			m_ExRecListNum	=	0;
			m_MaRecListNum	=	0;
			m_StallOpenNum	=	0;
			m_ShopID.Reset();
			m_nShopType		=  NO_SHOP;
			m_ShopSubType	=	0;
			for(INT i =0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				m_StallList[i].CleanUp();
			}
			for(INT i =0; i<MAX_PARTNER_PER_SHOP; i++)
			{
				m_PartnerList[i].CleanUp();
			}
		}
	};

	PLAYERSHOP_BOX		m_MyPlayerShop;			/// 自己的玩家商店
	PLAYERSHOP_BOX		m_OtPlayerShop;			/// 其他人的商店
	FLOAT				m_CommerFactor;			/// 商业指数
	BOOL				m_bMeIsOwner;			/// 标志自己是不是店主
	INT					m_nPartnerNum;			/// 合作伙伴人数

	struct SELECT_ITEM
	{
		INT		m_nConIndex;
		INT		m_nPosition;
		
		VOID CleanUp()
		{
			m_nConIndex = -1;
			m_nPosition = -1;
		}
	};

	SELECT_ITEM			m_MyPsSelectItem;		/// 自己的商店界面上选中的物品
	SELECT_ITEM			m_OtPsSelectItem;		/// 别人的商店界面上选中的物品

	/// 当前玩家商店NPC的ID
	INT						m_nPSNpcID;


	/// 客户端显示的帮派列表
	GuildInfo_t				m_GuildList[MAX_GUILD_SIZE];
	INT						m_GuildNum;

	/// 帮众列表
	GuildMemberInfo_t		m_GuildMemList[USER_ARRAY_SIZE];
	INT						m_GuildMaxMemNum;
	INT						m_GuildMemNum;
	STRING					m_GuildDesc;
	STRING					m_GuildName;
	BYTE					m_uPosition;
	BYTE					m_uAccess;

	/// 帮派的详细信息
	GuildDetailInfo_t		m_GuildDetailInfo;

	/// 可任命帮派的职位信息
	GuildAppointPos_t		m_GuildPosAvail[GUILD_POSITION_SIZE];

	/// 邀请加帮信息
	INT						m_GuildInviteId;
	STRING					m_GuildInviteName;		// 发出邀请的帮会
	STRING					m_GuildInviteObjName;	// 发出邀请的对象


	/// 帮会家族
	INT						m_iGuildFamilyNum;		// 帮会中家族的数量
	INT						m_GuildFamilyMemNum;	// 帮会中家族中成员的数量
	GuildFamilyInfo			m_FamilyList[GUILD_FAMILY_SIZE];
	INT						m_iSelectFamilyId;		// 当前选择的家族id

	// 界面的家族数据
	map< INT, STRING >			m_mFamilyWinData;


	/// 称号

//	_TITLE_
	INT						m_vTitles[3][MAX_TITLE_SIZE];		// 称号
	INT						m_iTitleNum[3];						// 称号数量
	INT						m_iCurrTitleId[3];					// 当前使用的称号id


/*
	INT						m_iTitleNum[3];			// 数量
	INT						m_iCurrTitleId[3];		// 当前使用的

	_TITLE_					m_vTitles[MAX_TITLE_SIZE];	// 称号
*/

	/// 国家
	CountryFunctionaryInfo	m_CountryFunctionInfo[COUNTRY_FUNCTION_TYPE_NUM];


	/// 需要销毁的物品的一个临时记忆区
	struct DISCARD_ITEM
	{
		INT			m_nContainer;
		INT			m_nPosition;

		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition = -1;
		}
	};

	DISCARD_ITEM						m_CurDisCardItem;

	/// 拆分信息
	struct SPLIT_ITEM
	{
		INT								m_nContainer;
		INT								m_nPosition;
		INT								m_nSum;			/// 总个数
		INT								m_nNumber;		/// 分出来的个数
		VOID							CleanUp()
		{
			m_nSum		 = 0;
			m_nNumber	 = 0;
			m_nContainer = -1;
			m_nPosition	 = -1;
		}

		SPLIT_ITEM()
		{
			CleanUp();
		}
	};
	SPLIT_ITEM							m_CurSplitItem;


	//打星信息

	struct ADD_START_EQUIP
	{
		INT								m_nContainer;    //装备或者背包
		INT								m_nPosition;	 //装备点	
		INT                             m_GemId;         //宝石id
        INT                             m_GemNum;		 //宝石数量	
		INT								m_nIntensifyInfo;//强化比例
		INT								m_nMoney;		 //所需金钱	
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;
			m_GemId      = -1;
			m_GemNum	 = -1;
			m_nIntensifyInfo = -1;  						 


		}

		ADD_START_EQUIP()
		{
			CleanUp();
		}

	};
	ADD_START_EQUIP							m_CurAddStarEquip;

	//装备合成 的材料      

	struct   EQUIP_SYNTHESIZE_MATERIAL
	{

		INT                             m_materialId;     //材料 id   
		INT                             m_materialNum;    //材料数量
		INT                             m_materialGrade;  //材料等级
	 
		VOID							CleanUp()
		{
			m_materialId = -1;
			m_materialNum	 = -1;
			m_materialGrade    = -1;	 						 

		}

		EQUIP_SYNTHESIZE_MATERIAL()
		{
			CleanUp();
		}

	};


	//装备合成信息

	struct EQUIP_SYNTHESIZE
	{
		 
		 
		EQUIP_SYNTHESIZE_MATERIAL          m_Mat[MAX_EQUIP_PROMATL_NUM];
		UINT								m_MatNum;	

		INT								m_IdTable; //改造表第一列

		VOID							CleanUp()
		{
			m_IdTable = 0;
			m_MatNum = 0;
			for ( INT i =0 ;i<MAX_EQUIP_PROMATL_NUM;++i)
			{
				m_Mat[i].CleanUp();
			}

		} 

		EQUIP_SYNTHESIZE()
		{
			CleanUp();
		}
	 
	};
	EQUIP_SYNTHESIZE							m_CurSynEquip;



	//装备改造


	//装备改造的材料      

	struct   EQUIP_REWORK_MATERIAL
	{

		INT                             m_materialId;     //材料 id  
		INT                             m_materialNum;    //材料位置
		INT                             m_materialGrade;  //材料等级

		VOID			CleanUp()
	  {
			m_materialId = -1;   
			m_materialNum = 0;  
			m_materialGrade = -1;


		}

	};


	struct  EQUIP_REWORK

	{
		INT								m_nContainer;    //装备或者背包
		INT								m_nPosition;	 //装备点	

		EQUIP_REWORK_MATERIAL		   m_Mat[MAX_EQUIP_RWMATL_NUM];

		INT								m_IdTable; //改造表第一列
		UINT							m_MatNum;	

		VOID							CleanUp()
		{
			m_nContainer	 = -1;
			m_nPosition		 = -1;
			m_IdTable  = -1;
			m_MatNum = 0;
			for ( INT i =0 ;i<MAX_EQUIP_RWMATL_NUM;++i)
			{
				m_Mat[i].CleanUp();
			}

		} 
		EQUIP_REWORK()
		{
			CleanUp();
		}


	};

	EQUIP_REWORK        m_CurEquip_ReWork;




	//装备灵魂改造


	//装备灵魂改造的材料    spirit  

	struct   EQUIP_SPIRIT_REWORK_MATERIAL
	{

		INT                             m_materialId;     //材料 id  
		INT                             m_materialNum;    //材料位置
		INT                             m_materialGrade;  //材料等级


		VOID							CleanUp()
		{
			m_materialId = -1;   
			m_materialNum = 0;  
			m_materialGrade = -1;


		}

	};

	struct  EQUIP_SPIRIT_REWORK
	{
		INT								m_nContainer;    //装备或者背包
		INT								m_nPosition;	 //装备点	

		EQUIP_SPIRIT_REWORK_MATERIAL	m_Mat[MAX_EQUIP_SPIRIT_RWMATL_NUM];

		INT								m_IdTable; //改造表第一列
		UINT							m_MatNum;	

		VOID							CleanUp()
		{
			m_nContainer	 = -1;
			m_nPosition		 = -1;
			m_IdTable  = -1;
			m_MatNum = 0;
			for ( INT i =0 ;i<MAX_EQUIP_SPIRIT_RWMATL_NUM;++i)
			{
				m_Mat[i].CleanUp();
			}

		} 

		EQUIP_SPIRIT_REWORK()
		{
			CleanUp();
		}

	};

	EQUIP_SPIRIT_REWORK        m_CurEquip_SpiritReWork;




	// 打孔信息
	struct ADD_HOLE_EQUIP
	{
		INT								m_nContainer;    //装备或者背包
		INT								m_nPosition;	 //装备点	
		INT                             m_ItemId;        //道具id
		INT                             m_ItemNum;		 //道具数量	
		INT			                    m_Money ;        //花费金钱数目
		INT								m_NpcId;		//相关npc

		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;
			m_ItemId     = -1;
			m_ItemNum	 = -1;
			m_Money      = -1;
			m_NpcId      = -1;


		}

		ADD_HOLE_EQUIP()
		{
			CleanUp();
		}
	};
	ADD_HOLE_EQUIP							m_CurAddHoleEquip;



	// 绑定信息
	struct BANG_DING_EQUIP
	{
		INT								m_nContainer;    //装备或者背包
		INT								m_nPosition;	 //装备点	
		INT                             m_ItemId;        //道具id
		INT                             m_NeedItemPos;    //道具位置
	 
		 
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;
			m_ItemId     = -1;
			m_NeedItemPos = -1;
	


		}

		BANG_DING_EQUIP()
		{
			CleanUp();
		}
	};
	BANG_DING_EQUIP							m_CurBangDingEquip;




   // 装备交换牌型


	struct   SWAP_CARD_EQUIP
	{
		INT								m_nContainer1;    //装备或者背包
		INT								m_nPosition1;	 //装备点	
		INT								m_nContainer2;    //装备或者背包
		INT								m_nPosition2;	 //装备点	
		INT                             m_ItemId;        //道具id
		INT                             m_NeedItemPos;    //道具位置

		VOID							CleanUp()
		{
			m_nContainer1 = -1;
			m_nPosition1 = -1;
			m_nContainer2 = -1;
			m_nPosition2 = -1;
			m_ItemId     = -1;
			m_NeedItemPos = -1;

		}


		SWAP_CARD_EQUIP()
		{
			CleanUp();
		}



	};

	SWAP_CARD_EQUIP        m_CurSwapCardEquip;


	// 装备 牌型改造


	struct   CJANGE_CARD_EQUIP
	{
		INT								m_nContainer;    //装备或者背包
 
		INT								m_nPosition;	 //装备点	
		INT                             m_ItemId;        //道具id
		INT                             m_NeedItemPos;    //道具位置

		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition = -1;
			m_ItemId     = -1;
			m_NeedItemPos = -1;

		}

		CJANGE_CARD_EQUIP()
		{
			CleanUp();
		}


	};

	CJANGE_CARD_EQUIP        m_CurChangeCardEquip;




   //装备拆解后的材料      

	struct   EQUIP_DISMANTLE_MATERIAL
	{
		
		INT                             m_materialId;     //材料 id   
		INT                             m_materialNum;    //材料数量
		INT                             m_materialGrade;  //材料等级
		INT								m_nMaterialLoss;//拆解损耗比例4

		VOID	CleanUp()
		{
			m_materialId =-1;   
			m_materialNum = 0;  
			m_materialGrade = -1;
			m_nMaterialLoss = -1;


		}

	};

    
   //装备拆解        

   struct  EQUIP_DISMANTLE

   {

	   INT								m_nContainer;    //装备或者背包
	   INT								m_nPosition;	 //装备点	

	  EQUIP_DISMANTLE_MATERIAL          m_Mat[MAX_EQUIP_PROMATL_NUM];
	   UINT								m_MatNum;	

	  
	   INT								m_IdTable; //改造表第一列

	   VOID							CleanUp()
	   {
		   m_nContainer	 = -1;
		   m_nPosition	= -1;
		   m_IdTable = 0;
		   m_MatNum = 0;
		   for ( INT i =0 ;i<MAX_EQUIP_PROMATL_NUM;++i)
		   {
			   m_Mat[i].CleanUp();
		   }

	   } 

	   EQUIP_DISMANTLE()
	   {
		   CleanUp();
	   }

   };

   EQUIP_DISMANTLE        m_CurEquip_Dis;






	//特修信息

	//特修后装备的耐久上限=当前耐久上限+特修道具数量*N；
	struct ADD_SPECIAL_RAPAIR
	{
		INT								m_nContainer;    //装备或者背包
		INT								m_nPosition;	 //装备点	
		INT                             m_NeedItemId;     //材料id
		INT                             m_NeedItemNum;	 //材料数量	
		INT								m_nIntensifyInfo;//N
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;
			m_NeedItemId      = -1;
			m_NeedItemNum	 = -1;
			m_nIntensifyInfo = -1;  						 

		}

		ADD_SPECIAL_RAPAIR()
		{
			CleanUp();
		}
	};
	ADD_SPECIAL_RAPAIR							m_CurSpRepairEquip;






	//修理信息  单修的物品
	struct ADD_COM_RAPAIR
	{
		INT								m_nContainer;    //装备或者背包
		INT								m_nPosition;	 //装备点	
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition	 = -1;					 
		}
		ADD_COM_RAPAIR()
		{
			CleanUp();
		}
	};
	ADD_COM_RAPAIR							m_CurComRepairEquip;



	/// 玩家资料
	struct PRIVATE_INFO
	{
		STRING		m_szName;				/// 在CleanUp中不要清除
		INT			m_nInfoType;			/// 保密程度
		UINT		m_unCharGUID;			/// 角色编号
		INT			m_nAge;					/// 年龄
		INT			m_nSex;					/// 性别
		STRING		m_szJobInfo;			/// 职业信息		[MAX_JOBINFO_LENGTH]
		STRING		m_szSchoolInfo;			/// 毕业院校		[MAX_SCHOOLINFO_LENGTH]
		INT			m_nBloodType;			/// 血型
		INT			m_nYearAnimal;			/// 生肖
		INT			m_nConsella;			/// 星座
		INT			m_nProvince;			/// 省份
		STRING		m_szCity;				/// 城市			[MAX_CITY_LENGTH]
		STRING		m_szEmailInfo;			/// 邮件			[MAX_EMAIL_LENGTH]
		STRING		m_szLuckWord;			/// 心情寄语		[MAX_LUCKWORD_LENGTH]

		VOID		CleanUp()
		{
			m_nInfoType		= 0;
			m_unCharGUID	= 0;
			m_nAge			= 0;
			m_nSex			= 0;
			m_szJobInfo		= "";
			m_szSchoolInfo	= "";
			m_nBloodType	= 0;
			m_nYearAnimal	= 0;
			m_nConsella		= 0;
			m_nProvince		= 0;
			m_szCity		= "";
			m_szEmailInfo	= "";
			m_szLuckWord	= "";
		}
	};
	PRIVATE_INFO	m_MyPrivateInfo;
	PRIVATE_INFO	m_OtPrivateInfo;
	INT				m_nObjId;


/** 交易记录，或者操作记录
 */
	
	RecordList_t						m_RecordList;

	
/** 门派心法老师
 */

	/// 心法老师的心法ID
//	INT									m_idXinfaTeacher;
//	INT									m_nXinfaTeacherId;

	
/** 生活技能老师的相关内容
 */

	/// 所教的生活技能的ID
	INT									m_nStudyAbility_ID;
	/// 需要花费的金钱
	UINT								m_uStudyAbility_NeedMoney;	
	/// 需要消耗的经验值
	UINT								m_uStudyAbility_NeedExp;	
	/// 技能需要达到的点数（学习中不会消耗）
	UINT								m_uStudyAbility_SkillExp;
	/// 学习这个生活技能需要的等级
	INT									m_nStudyAbility_LevelLimit;
	/// 学习这个技能的服务器脚本号
	UINT								m_uStudyAbility_ScriptId;
	/// 学习生活技能NpcServer ID
	UINT								m_StudyAbility_NpcId;

	SDATA_PET							m_listPet[HUMAN_PET_MAX_COUNT];

	SDATA_PET							m_TargetPet;

	struct MountAvatar
	{
		INT					nMountID;
		STRING				strAvatarName;
		CObject_PlayerNPC * pAvatar;

		MountAvatar()
		{
			nMountID		= INVALID_ID;
			pAvatar			= NULL;
			strAvatarName	= "";

		}
		VOID CleanUp()
		{
			nMountID		= INVALID_ID;
			pAvatar			= NULL;
			strAvatarName	= "";
		}
	};

	/// 用于Pet UI显示的宠物对象
	MountAvatar							m_listPetAvatar[HUMAN_PET_MAX_COUNT];

	/// 用于TargetPet UI显示的宠物对象
	MountAvatar							m_TargetPetAvatar;

	/// 用于Mount UI显示的坐骑对象  [7/14/2010 陈军龙]
	MountAvatar							m_listMountAvatar[MAX_MOUNT_SIZE];


/** 技能的BUFF数据
 */
	
	typedef std::map< INT, _BUFF_IMPACT_INFO* >	CBuffImpactMap;
	CBuffImpactMap						m_mapBuffImpact;
	INT									m_nBuffImpactTime;


	/// 好友数 
	UINT								m_uFriendCount;		
	/// 黑名单数 
	UINT								m_uBlackCount;
	/// 仇人数
	UINT								m_uEnemyCount;

	_FRIEND_INFO						m_FriendInfo[ RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET ];	// 200 - 0 = 200
	_BLACKNAME_INFO						m_BlackNameInfo[ MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET - 40 ] ;	// 270 - 200-40 = 30
	_ENEMY_INFO							m_EnemyInfo[ 40 ];

	/// 自定义组的名称
	STRING								m_NewGroup[5];

	//请求添加为好友的玩家的GUID
	CHAR								m_FriendInviteName[MAX_CHARACTER_NAME];

/** 宠物门派技能学习
 */

	/// 服务器传过来的技能列表，只在学习宠物门派技能的时候使用
	std::vector<PET_SKILL>				m_petSkillStudyList;				


/** 宠物公告板
 */
	
	/// 当前条数
	INT									m_nPetPlacardItemCount;								
	/// 宠物公告板数据
	_PET_PLACARD_ITEM					m_aPetPlacardItems[MAX_PETPLACARD_LIST_ITEM_NUM];	



/** 搜索好友
 */
	UINT								m_FingerPlayerNum;
	FingerPlayerInfo_t					m_FingerPlayerList[USER_ARRAY_SIZE];



//================
/** 界面系统提示
 */


	typedef std::map< INT, std::list< SystemTipEvent > >	MAP_SYSTIP;
	typedef MAP_SYSTIP::iterator MAP_SYSTIP_ITER;

	
	// 界面索引（和保存的数据一一对应）
	INT				m_UISysTipIndex;

	// 数据保存
	MAP_SYSTIP		m_mapSysTip;
	// 对应到map索引
	INT				m_nMapIndex[11];
	INT				m_nMapIndexNum;


//================
/** 财产保护选项
 */

	// 
	INT				m_PasswordData;
	// 保存当前输入的密码
	STRING			m_InputPassword;
	// 各密码保护是否已经设置了（各操作时是否弹出密码输入）
	BOOL			m_InputPasswdState[5];


public:

/** 鉴定相关
 */

	/** 锁定鉴定轴 */
	void Identify_Lock(INT iBagPos);

	/** 取消鉴定轴锁定 */
	void Identify_UnLock();

	/// 当前鉴定卷轴的位置 
	INT			m_nIdentifyPosition;
	/// 使用卷轴的背包索引位置
	int			m_iIdentifyInBagIndex;
	/// 鉴定卷轴等级
	int			m_iIdentifyLevel;
public:

	// 游戏中所有的套装
	std::map<INT , INT>	 AllSetInfo; //保存套装id对应的装备数量
	INT  m_nSetRow  ;  //套装标的行数

	INT  m_SelfActiveSetId[14];  // 激活的套装id
	INT  m_OtherActiveSetId[14]; // 激活的套装id
	INT  m_SelfActiveSetEquipNum[14];  // 自己每套已经有的装备的数量
	INT  m_OtherActiveSetEquipNUm[14]; // 别人每套已经有的装备的数量
	INT  m_SelfActiveSetNum ;	// 自己激活的套装数
	INT  m_OtherActiveSetNum ;  // 别人激活的套装数



 struct Euuip_Sprite_Chain
 {
	SHORT m_pai1;
	SHORT m_pai2;
	SHORT m_pai3;

	Euuip_Sprite_Chain()
		{
			m_pai1  = -1;
			m_pai2  = -1;
			m_pai3  = -1;
		}
	VOID CleanUp()
	{
		m_pai1  = -1;
		m_pai2  = -1;
		m_pai3  = -1;
	}
 };

 struct Euuip_Sprite_Soul
 {
	 SHORT m_Chain1;
	 SHORT m_Chain2;
	 SHORT m_Chain3;
	 SHORT m_Chain4;
	 SHORT m_Chain5;

	 Euuip_Sprite_Soul ()
	 {
		m_Chain1 = -1; 
		m_Chain2 = -1;
		m_Chain3 = -1;
		m_Chain4 = -1;
		m_Chain5 = -1;    //将组合
	
	 }
VOID CleanUp()
	 
	 {
		 m_Chain1 = -1;
		 m_Chain2 = -1;
		 m_Chain3 = -1;
		 m_Chain4 = -1;
		 m_Chain5 = -1;


	 }

 };



	// 游戏中所有的灵魂小激活
	std::map<INT , Euuip_Sprite_Chain>	 AllSpritActiveChainEqual; //保存小激活信息  相等的

	// 游戏中所有的灵魂小激活
	std::map<INT , Euuip_Sprite_Chain>	 AllSpritActiveChainSeries; //保存小激活信息 连着的

	// 游戏中所有的灵魂小激活
	std::map<INT , Euuip_Sprite_Chain>	 AllSpritActiveChainJiang; //保存小激活信息 将

	// 游戏中所有的灵魂大激活
	std::map<INT , Euuip_Sprite_Soul>	 AllSpritActiveInfo ;//保存大激活信息
	


	INT  m_SelfSpritActive[EM_EQUIP_CARD_GROUP_MAXNUM]; //标记玩家灵魂小激活的数组  里面存的是小激活的id 
	INT  m_SelfSpritActiveAny[EM_EQUIP_CARD_GROUP_MAXNUM]; //标记玩家灵魂小激活的数组  里面存的是小激活的id  只是任意舜 将 刻
	INT  m_OterSpritActive[EM_EQUIP_CARD_GROUP_MAXNUM]; //标记玩家灵魂小激活的数组
	INT  m_OterSpritActiveAny[EM_EQUIP_CARD_GROUP_MAXNUM]; //标记玩家灵魂小激活的数组

	INT m_SelfAllSprite_Active;//标记玩家灵魂大激活的id组
	INT m_OtherAllSprite_Active;//标记玩家灵魂大激活的id组
	struct  Equip_Id
	{
		INT  nIdList[50];   //大小为每套套装数量。 允许多选一组合成套装

		INT  nValue;// 有效大小

		Equip_Id()
		{
			memset(nIdList,0,sizeof(nIdList));
			nValue = 0;
		}

	  VOID CleanUp()
		{
			memset(nIdList,0,sizeof(nIdList));
			nValue = 0;
		}
	};


	// 游戏中所有的套装    套装号 对应相应数量的装备id
	std::map<INT , Equip_Id>	 AllSetEquipList; //保存套装id对应的装备数量

	struct  EquipSet_Attr_Name
	{
		string  m_Name;
		EQUIP_SET_ATTR  m_stAttr[EQUIP_SET_ATTR_NUM]; //套装属性
		INT	m_nAttrActiveNum[EQUIP_SET_ATTR_NUM];
		EquipSet_Attr_Name()
		{
			CleanUp();
		}
		VOID CleanUp()
		{
			m_Name = "";
			memset(m_stAttr,-1,sizeof(m_stAttr));
			memset(m_nAttrActiveNum, 0, sizeof(m_nAttrActiveNum));
		}

	};
	// 游戏中所有的套装  name
	std::map<INT , EquipSet_Attr_Name>	 AllSetEquipName; //保存套装id对应的装备name

public:
	INT		GetAttriAutoPoint(INT nPos);
	VOID	SetAttriAutoPoint(INT val){m_AtriAutoPoint = val;}

protected:
	INT	m_AtriAutoPoint;
public:

	//---------------------------------------------
	//核心继承
	//---------------------------------------------

	CDataPool();
	virtual ~CDataPool();

	static CDataPool*	GetMe(VOID) { return s_pMe; }

public:

	virtual VOID		Initial(VOID*);
	virtual VOID		Release(VOID);
	virtual VOID		Tick(VOID);

protected:

	static CDataPool*	s_pMe;

	KL_DECLARE_DYNAMIC( CDataPool );
};