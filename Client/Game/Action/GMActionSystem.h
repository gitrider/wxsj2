
/** 操作管理器
 */

#pragma once

#include <Imm.h>
#include "GIActionSystem.h"
#include "DB_Struct.h"



//struct SCLIENT_XINFA;
struct SCLIENT_SKILL;
struct SCLIENT_LIFEABILITY;
struct PET_SKILL;
struct TALK_ACT_STRUCT;

class tObject_Item;

namespace LuaPlus
{
	class LuaObject;
	class LuaState;
};

struct EVENT;

//-----------------------------------------------------------------------------------------------------------------------------

class CActionItem : public tActionItem
{
public:

	// 得到id
	virtual INT GetID(VOID) const				{ return m_ID; }
	// 得到名称
	virtual LPCTSTR GetName(VOID) const			{ return m_strName.c_str(); }
	// 得到图标
	virtual LPCTSTR GetIconName(VOID) const		{ return m_strIconName.c_str(); }

	virtual VOID SetIconName(STRING& name)		{ m_strIconName = name; }
	// 设置Check状态
	virtual VOID SetCheckState(BOOL bCheck);
	// 添加引用
	virtual VOID AddReference(tActionReference* pRef,BOOL bIsInMenuToolbar=FALSE);
	// 断开引用
	virtual VOID RemoveRefrence(tActionReference* pRef);
	// Is Enabled
	virtual BOOL IsEnabled(VOID) const			{ return m_bEnabled; }
	// Enable
	virtual VOID Enable(VOID)					{ m_bEnabled=TRUE; }
	// Disable
	virtual VOID Disable(VOID)					{ m_bEnabled=FALSE; }
	// 设置Default状态(只能通过ActionSystem调用)
	virtual VOID SetDefaultState(BOOL bDefault);
	

	//--------------------------------------
	//-- super tool tip 使用
	//--

	// 得到物品的等级
	virtual INT GetItemLevel();

	// 得到物品的耐久
	virtual INT GetItemDur();

	// 得到物品的最大耐久
	virtual INT GetItemMaxDur();

	// 得到物品的修理次数
	virtual INT GetItemRepairCount();

	// 得到物品的绑定信息 
	virtual INT GetItemBindInfo();

	// 得到装备的2次绑定信息 
	virtual INT GetItemSecLevelBindInfo();

	//	得到装备的2次绑定加成
	virtual LPCTSTR	 GetItemBindAddInfo();

	//	得到掉落颜色
	virtual LPCTSTR  GetDropVisColor();

	// 得到物品卖给npc的价格
	virtual INT GetItemPrice();

    // 得到物品职业
	virtual INT GetItemProfession();

	//得到装备牌型
	virtual INT GetEquipCard();

	//得到物品是否唯一
	virtual INT  GetItemRuleOnly();

	// 得到物品的制作人
	virtual LPCTSTR GetManufacturer();

	//得到装备孔数量
	virtual INT		GetEquipHoleNum();
	//得到装备的套装信息
	virtual LPCTSTR	GetEquipSetInfo();

	//得到装备牌型激活的信息 
	virtual LPCTSTR	GetEquipSpritePaiInfo();

	//得到其他玩家装备小激活的信息
	virtual LPCTSTR	GetOtherEquipSpriteChainInfo();

	//得到装备小激活的信息
	virtual LPCTSTR	GetEquipSpriteChainInfo();

	//得到其他玩家装备大激活的信息
	virtual LPCTSTR	GetOtherEquipSpriteAllActiveInfo();

	//得到装备大激活的信息
	virtual LPCTSTR	GetEquipSpriteAllActiveInfo();
 
	//得到装备的套装信息
	virtual LPCTSTR	GetOtherEquipSetInfo();
	//得到装备套装激活的属性信息
	virtual LPCTSTR	GetEquipSetActiveAttr();
	//得到装备套装激活的属性信息
	virtual LPCTSTR	GetOtherEquipSetActiveAttr();
	
	//得到装备星数量
	virtual INT		GetStartLevel();
 
	// 得到白色属性 
 	virtual LPCTSTR GetBaseWhiteAttrInfo();


	// 得到action所引用的item 的类型
	virtual INT GetItemType();

	// 得到类型描述
	virtual LPCTSTR GetTypeDesc();


	//--------------------------

	// 得到操作类型
	virtual ACTION_OPTYPE GetType(VOID) const		{ return AOT_EMPTY; }
	// 类型字符串
	virtual	LPCTSTR GetType_String(VOID)			{ return ""; }
	
	// 得到定义ID
	/*
	|	对于战斗技能, 是技能表中的ID (DBC_SKILL_DATA)
	|	对于生活技能，是生活技能表中的ID(DBC_LIFEABILITY_DEFINE)
	|	对于物品，是物品表中的ID(DBC_ITEM_*)
	|	对于心法，是心法表中的ID(DBC_SKILL_XINFA)
	*/
	virtual INT GetDefineID(VOID)					{ return -1; }
	// 得到数量
	virtual INT GetNum(VOID)						{ return 0;	}
	// 得到内部数据
	virtual VOID* GetImpl(VOID)						{ return NULL;}
	// 得到解释
	virtual LPCTSTR GetDesc(VOID)					{ return _T(""); }
	// 得到冷却状组ID
	virtual INT GetCoolDownID(VOID)					{ return -1; }
	// 得到所在容器的索引
	virtual INT GetPosIndex(VOID)					{ return -1; }
	// 激活动作
	virtual VOID DoAction(VOID)						{ }
	// 激活动作(副操作)
	virtual VOID DoSubAction(VOID)					{ }
	// 是否有效
	virtual BOOL IsValidate(VOID)					{ return FALSE;}
	// 检查冷却是否结束
	virtual BOOL CoolDownIsOver(VOID)				{ return TRUE; }
	// 拖动结束
	virtual VOID NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName) { }
	// 显示tooltips
	virtual VOID NotifyTooltipsShow(int btn_left, int btn_top, int btn_right, int btn_bottom);
	// 隐藏tooltips
	virtual VOID NotifyTooltipsHide(VOID);
	// 查询逻辑属性
	virtual STRING GetAttributeValue(LPCTSTR szAttributeName)	{ return ""; }
	// 得到心法等级
	virtual INT GetXinfaLevel()						{ return -1; }

	virtual INT	GetCooldownTime() { return 0;}

	// 提供特殊操作接口,各个子类根据自己需要定义操作
	virtual void SpecialOperation(void* pData)		{}
	
public:

	// 清空链接
	VOID ClearRefrence(VOID);
	// 进入冷却
	virtual VOID UpdateCoolDown(VOID);
	// 通知UI
	VOID UpdateToRefrence(VOID);
	// 激活动作仅显示按钮动画
	VOID DoActionPrepare(BOOL bCheck);
	// 进入公共冷却
	VOID UpdateCommonCoolDown(VOID);

protected:

	friend class CScriptSystem;

	//--------------------------------------------
	//导出到脚本中的函数

	INT Lua_GetID(LuaPlus::LuaState* pState);			// 得到ID
	INT Lua_GetType(LuaPlus::LuaState* pState);			// 得到类型
	INT Lua_GetName(LuaPlus::LuaState* pState);			// 得到名字
	INT Lua_GetDesc(LuaPlus::LuaState* pState);			// 得到解释
	INT Lua_GetDefineID(LuaPlus::LuaState* pState);		// 得到在资源表中的ID
														/*
														|	对于战斗技能, 是技能表中的ID (DBC_SKILL_DATA)
														|	对于宠物技能, 是技能表中的ID (DBC_SKILL_DATA)
														|	对于生活技能，是生活技能表中的ID(DBC_LIFEABILITY_DEFINE)
														|	对于物品，是物品表中的ID(DBC_ITEM_*)
														|	对于心法，是心法表中的ID(DBC_SKILL_XINFA)
														|
														*/
	INT Lua_GetOwnerXinfa(LuaPlus::LuaState* pState);	// 只针对战斗技能，得到所属的心法
	INT Lua_GetPetSkillOwner(LuaPlus::LuaState* pState);// 只针对宠物技能，得到技能是属于第几只宠物的。
	INT Lua_GetOwnerQuest(LuaPlus::LuaState* pState);	// 只针对任务奖励虚拟物品，得到物品是属于第几个任务的。
	INT Lua_GetPrice(LuaPlus::LuaState* pState);		// 得到价格
	INT Lua_GetNum(LuaPlus::LuaState* pState);			// 得到每一组的数量

public:

	CActionItem(INT id=0);
	virtual ~CActionItem();

	static LuaPlus::LuaObject*		s_pMetaTable;
	// 非法ActionItem
	static CActionItem s_InvalidAction;

protected:

	/// ID(客户端内部标示)
	INT				m_ID;
	/// 名称
	STRING			m_strName;
	/// 图标
	STRING			m_strIconName;
	/// UI引用
	std::set< tActionReference* >	m_setReference;
	/// Checked
	BOOL			m_bChecked;
	/// 有效无效
	BOOL 			m_bEnabled;

	BOOL			m_bLocked;

};



//-----------------------------------------------------------------------------------------------------------------------------

class CActionSystem : public tActionSystem
{
public:

	//根据id取得Action
	virtual tActionItem*	GetAction(INT id);
	//得到缺省操作
	virtual tActionItem*	GetDefaultAction(VOID) { return m_pDefaultAction; }
	virtual VOID			SetDefaultAction(tActionItem* pDefAction);

	//得到本次技能使用完毕后，下一次操作的技能
	virtual tActionItem*	GetSetoutAction(VOID) { return m_pSetoutAction; }
	virtual VOID			SetSetoutAction(tActionItem* pSetoutAction) { m_pSetoutAction = pSetoutAction; }

public:
	void SetNotCancleIden(bool yes) {m_NotCancleIden=yes;}
	bool GetNotCancleIden(){return m_NotCancleIden;}

public:

	//得到某类Action的数目
	virtual INT				GetActionNum(LPCTSTR szType, INT PosIndex = -1);

	//枚举Action
	virtual tActionItem*	EnumAction(INT nPackageIndex, LPCTSTR szFilter);
	//枚举宠物Skill
	virtual tActionItem*	EnumPetSkill(INT nPetIndex, INT nPetSkill, LPCTSTR szFilter);

	//当前正在显示Tootips的按钮
	virtual tActionItem*	GetTooltipsFocus(void);

	//主工具条快捷操作 
	// nIndex	(0,1,2,3,4,5,6,7,8,9)
	// bPrepare 是否是预备(仅显示Check状态)
	virtual VOID			MainMenuBar_DoAction(INT nIndex, BOOL bPrepare);
	//设置主菜单上项目
	virtual VOID			MainMenuBar_Set(INT nIndex, INT idAction);
	//设置主菜单上项目，从服务器传过来
	virtual VOID			MainMenuBar_SetID(INT nIndex, INT idType, INT idData);

	//取得主菜单上项目
	virtual INT				MainMenuBar_Get(INT nIndex);

	//设置背包
	virtual VOID			UserBag_Set(INT nIndex, INT idAction);
	//存储快捷
	virtual VOID			SaveAction(VOID);


public:

	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

public:

	VOID			UpdateToolBarForPetSkill(VOID);
	//----------------------------------------------------
	//技能跟新
	VOID			UserSkill_Update(VOID);

	tActionItem*    GetMyDefaultSkillAtionBySkillId(INT nSkillId);
    tActionItem*	GetMyDefaultSkillAtion();

	//技能开始使用(服务器返回)
	static VOID	WINAPI		_OnSkillActive(const EVENT* pEvent, UINT dwOwnerData);

	VOID			UserPetSkill_Update(INT nPetNum);
	VOID			UserPetSkillStudy_Update(VOID);
	VOID			UserTargetPetSkill_Update(VOID);


//protected:
//	struct  AbilityStudyInfo
//	{
//		INT nAbilityId;
//		INT nLevel;
//		CActionItem* pAction;
//	};
//	typedef std::vector< AbilityStudyInfo>	LIFEABILITYSTUDY_INFO;
//	typedef std::vector< AbilityStudyInfo>::iterator	LIFEABILITYSTUDY_INFO_ITER;
//	LIFEABILITYSTUDY_INFO	m_AllAbilityStudyInfo;

public:
	//----------------------------------------------------
	//创建用于学习时显示的生活技能的相关Action
	VOID			UserLifeAbilityStudy_Init(VOID);
	
	//----------------------------------------------------
	//获得用于学习时显示的生活技能的相关Action
	tActionItem*			UserLifeAbilityStudy_Get( INT nAbilityID, INT nLevel );


	//----------------------------------------------------
	//生活技能更新
	VOID			UserLifeAbility_Update(VOID);

	//----------------------------------------------------
	//根据最新的包裹数据更新
	VOID			UserBag_Update(VOID);

	//----------------------------------------------------
	//更新银行数据
	VOID			UserBank_Update(VOID);

	//----------------------------------------------------
	//更新交易界面自己信息
	VOID			UserExchangeSelf_Update(VOID);
	//对方
	VOID			UserExchangeOther_Update(VOID);

	// 任务物品提交界面的更新
	VOID			UserMission_Update(VOID);

	// 玩家商店的更新
	VOID			UserPS_Self_Update(VOID);
	VOID			UserPS_Other_Update(VOID);


	//----------------------------------------------------
	//新的箱子产生
	VOID			ItemBox_Created(INT ObjId=-1);
	//箱子中的项目发生改变
	VOID			ItemBox_Update(VOID);

	//----------------------------------------------------
	//商店货架数据更新
	VOID			Booth_Update(VOID);
	//ibshop更新
	VOID			IBShop_Update(VOID);
	//----------------------------------------------------
	//任务列表数据更新
	VOID			QuestLogItem_Update(VOID);

	//----------------------------------------------------
	//心法数据更新
//	VOID			XinFa_Update(VOID);


	//----------------------------------------------------
	//冷却时间更新
	VOID			UpdateCoolDown(INT nCoolDownID);
	//公共冷却更新
	VOID			UpdateCommonCoolDown(INT nSkillID);

	//----------------------------------------------------
	// 装备更新, 从数据池中的数据更新装备action数据

	// 通过数据池中的数据, 创建出来装备action item
	VOID			UserEquip_Update(VOID);

	// 通过数据池中的数据, 创建出来坐骑action item  [7/8/2010 陈军龙] 
	VOID            UserMount_Update(VOID);

	// 通过数据池中的数据, 创建出来装备action item
	VOID			OtherPlayerEquip_Update(VOID);

	//----------------------------------------------------
	//摆摊（出售）
	VOID			UserStallSale_Update(VOID);
	//摆摊（购买）
	VOID			UserStallBuy_Update(VOID);

	//----------------------------------------------------
	//聊天人物动作
	VOID			UserChatMood_Update(VOID);

	//物品传输系统相关
	VOID			TransferSystem_Update(tObject_Item* pItem, BOOL bDelete = FALSE);
	
	//设置快捷键状态 Nick 2008.1.12
	VOID			SetActionSystemKeyState( INT keyState, BOOL isFKey );

	// 为一键换装更新
	VOID			UpdateAction_FromChangeSuit(STRING& iconName, INT itemId, INT suitId);

	INT				CreateObjectItemByTableId(tObject_Item* pItem);

protected:
	//删除某个Action
	VOID			EraseActionItem(CActionItem* pItem);

	//根据玩家技能更新操作
	VOID			UpdateAction_FromSkill(const SCLIENT_SKILL* pSkill);
	//根据宠物技能更新操作
	VOID			UpdateAction_FromPetSkill(const PET_GUID_t& PetGUID, const PET_SKILL* pSkill, INT nSkillIndex);
	//根据宠物门派技能更新操作
	VOID			UpdateAction_FromPetSkillStudy(const PET_SKILL* pSk);
	//根据物品更新
	INT			UpdateAction_FromItem(tObject_Item* pItem);
	//根据生活技能
	VOID			UpdateAction_FromLifeAbility(const SCLIENT_LIFEABILITY* pLifeAbility);
	//根据门派创建心法item
//$$	VOID			UpdateAction_FromXinFa(const SCLIENT_XINFA* pXinFaDef);
	//根据聊天动作
	VOID			UpdateAction_FromChatMood(const TALK_ACT_STRUCT* pActStruct);

	//----------------------------------------------------
	//刷新工具条
	VOID			UpdateToolBar(VOID);

protected:
	//-----------------------------------------------------
	//所有操作
	std::map< INT, CActionItem* >		m_mapAllAction;
	//缺省操作(物理攻击)
	tActionItem*						m_pDefaultAction;
	//准备发出的action
	tActionItem*						m_pSetoutAction;
	
protected:

	//-----------------------------------------------------
	//工具条设置
	struct ACTIONBAR_ITEM
	{
		
		ACTION_OPTYPE	typeItem;	// 所引用的ActionItem的类型

		INT			idImpl;			// 引用项目的定义ID
									/*
									|
									|	技能		- 技能ID
									|	生活技能	- 生活技能ID
									|	物品		- client的idtable
									|	宠物技能	- 技能ID
									*/
		//---运行时
		INT			idActionItem;		// 引用的ActionItem
		
		INT			idItemCount;		// 物品数量

		PET_GUID_t	m_GUID;				// 宠物索引
	};

	//一个工具条的定义数据
	typedef std::vector< ACTIONBAR_ITEM >	ACTIONBAR_BARDEF;

	//主工具条 10 X ITEM
	const static INT	s_MainMenuBarNum = 30;
	ACTIONBAR_BARDEF	m_barMain;

public:

	//根据技能id检索Action
	//bAsDefID - 根据技能表中定义id
	tActionItem*						GetAction_SkillID(INT idSkill);
	//根据技能表中定义id
	tActionItem*						GetAction_PetSkillID(const PET_GUID_t& petGUID, INT idPetSkill);
	//根据物品id检索Action
	tActionItem*						GetAction_ItemID(INT idItem);
	//根据物品的idtable检索Action
	tActionItem*						GetAction_ItemIDTable_FromMyPacket(INT idItem);
	//根据生活技能id
	tActionItem*						GetAction_LifeAbilityID(INT idAbility);
	//根据心法id
//	tActionItem*						GetAction_XinFaID(INT idXinFa);
	//根据聊天动作id
	tActionItem*						GetAction_ChatMoodID(INT idAct);

	//清空无效的Action
	VOID								CleanInvalidAction(BOOL bForceCleanItemBox=FALSE);

	//当前银行租赁箱的编号
	VOID		SetCurBankRentBoxIndex(INT nCurBankRentBoxIndex){m_nCurBankRentBoxIndex=nCurBankRentBoxIndex;}
	INT			GetCurBankRentBoxIndex(VOID){return m_nCurBankRentBoxIndex;}

public:

	CActionSystem();
	virtual ~CActionSystem();

	static CActionSystem*	GetMe(VOID) { return s_pMe; }
private:

	INT		_CreateID(VOID) const;

	//当前的银行租赁箱的编号
	INT		m_nCurBankRentBoxIndex;

	//是否初始化完毕快捷栏
//	BOOL			m_bIsInitalDone;

	// Nick 2008.1.12
	// 快捷键存在共用，所以要记录当前的ACTIONBAR_ITEM位置
	// 记录1-6快捷键
	INT		m_nCurShortcutKeyState;
	//// 记录F1-F6快捷键
	//INT		m_nCurShortcutKeyFState;

	bool m_NotCancleIden;

protected:

	static CActionSystem*	s_pMe;
	KL_DECLARE_DYNAMIC(CActionSystem);

};



extern const char*		NAMETYPE_SKILL;
extern const char*		NAMETYPE_LIFESKILL;
extern const char*		NAMETYPE_PACKAGEITEM;
extern const char*		NAMETYPE_BANKITEM;
extern const char*		NAMETYPE_BOOTITEM;
extern const char*		NAMETYPE_LOOTITEM;
extern const char*		NAMETYPE_XINFA;
extern const char*		NAMETYPE_PETSKILL;
extern const char*		NAMETYPE_VIRTUALITEM;
extern const char*		NAMETYPE_EQUIP;
extern const char*		NAMETYPE_EXCHANGESELF;
extern const char*		NAMETYPE_EXCHANGEOTHER;
extern const char*		NAMETYPE_MISSIONREFER;
extern const char*		NAMETYPE_APPENDITEM;
extern const char*		NAMETYPE_STALLSELF;
extern const char*		NAMETYPE_STALLOTHER;
extern const char*		NAMETYPE_OTHEREQUIP;
extern const char*		NAMETYPE_PS_SELF;
extern const char*		NAMETYPE_PS_OTHER;
extern const char*		NAMETYPE_CHATMOOD;
extern const char*		NAMETYPE_PS_DROP;
extern const char*		NAMETYPE_CHANGE_SUIT;
extern const char*		NAMETYPE_TRANSFER_ITEM;
extern const char*		NAMETYPE_IB_SHOP;
extern const char*		NAMETYPE_MOUNTITEM;

