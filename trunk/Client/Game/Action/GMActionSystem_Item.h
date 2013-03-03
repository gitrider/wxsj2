
/** 操作管理器-物品
 */

#pragma once


#include "GMActionSystem.h"


class CActionItem_Item : public CActionItem
{
public:

	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_ITEM;	}
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID);
	//得到定义ID
	virtual INT				GetDefineID(VOID);
	//得到数量
	virtual INT				GetNum(VOID);
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetItemImpl();	}
	//得到解释
	virtual LPCTSTR			GetDesc(VOID);
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID);


	//----------------------------------------------------------------------------------------------------------
	//-- super tool tip 使用
	//--

	// 得到使用物品的等级
	virtual INT				GetItemLevel();
	// 得到物品的耐久
	virtual INT				GetItemDur();
	// 得到物品的最大耐久
	virtual INT				GetItemMaxDur();
	// 得到物品的修理次数
	virtual INT				GetItemRepairCount();
	// 得到物品的绑定信息 
	virtual INT				GetItemBindInfo();

	// 得到装备的2次绑定信息 
	virtual INT				GetItemSecLevelBindInfo();

	//	得到装备的2次绑定加成
	virtual LPCTSTR			GetItemBindAddInfo();

	//	得到装备的颜色
	virtual LPCTSTR			GetDropVisColor();

	// 20100408 AddCodeBegin
	// 得到物品买入的价格
	virtual INT	GetItemBasePrice();
	// 20100408 AddCodeEnd

	// 得到物品卖给npc的价格
	virtual INT				GetItemPrice();
	//得到物品是否唯一
	virtual	INT				GetItemRuleOnly();
	//得到物品职业
	virtual INT				GetItemProfession();
	//得到装备牌型
	virtual INT				GetEquipCard();

	// 得到物品的制作人
	virtual LPCTSTR			GetManufacturer();
	//得到装备的孔数量
	virtual	INT				GetEquipHoleNum();
	//得到装备的套装信息
	virtual LPCTSTR			GetEquipSetInfo();

	//得到装备牌型激活的信息 
	virtual LPCTSTR			GetEquipSpritePaiInfo();

	//得到其他玩家装备小激活的信息
	virtual LPCTSTR			GetOtherEquipSpriteChainInfo();
 
	//得到装备小激活的信息
	virtual	LPCTSTR			GetEquipSpriteChainInfo();

	//得到其他玩家装备大激活的信息
	virtual	LPCTSTR			GetOtherEquipSpriteAllActiveInfo();

	//得到装备大激活的信息
	virtual	LPCTSTR			GetEquipSpriteAllActiveInfo();
 
	//得到装备的套装信息
	virtual LPCTSTR			GetOtherEquipSetInfo();
	

	//得到装备套装激活的属性信息

	virtual LPCTSTR			GetEquipSetActiveAttr();
	//得到装备套装激活的属性信息

	virtual LPCTSTR			GetOtherEquipSetActiveAttr();


	
	//得到装备的星级
	virtual	INT				GetStartLevel();
	 
	// 得到白色属性
     
	virtual LPCTSTR			GetBaseWhiteAttrInfo();
	// 得到扩展蓝色属性
//	virtual LPCTSTR			GetExtBlueAttrInfo(); //remove
	// 得到action所引用的item 的类型
	virtual INT				GetItemType();

	// 得到装备属性
	LPCTSTR	GetEquipBaseAttrInfo();
	// 得到装备属性
	LPCTSTR	GetEquipAddAttrInfo();

	//得到装备技能属性
	LPCTSTR GetEquipSkillInfo();

	
	// 得到类型描述
	virtual LPCTSTR			GetTypeDesc();



	//得到所在容器的索引
	//	箱子中的物品	- 在箱子中的位置
	//	货架上的物品	- 货架上位置
	//
	virtual INT				GetPosIndex(VOID);
	//是否能够自动继续进行
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//激活动作
	virtual VOID			DoAction(VOID);
	//激活动作(副操作)
	virtual VOID			DoSubAction(VOID);
	//是否有效
	virtual BOOL			IsValidate(VOID);
	//刷新
	virtual VOID			Update(tActionReference* pRef);
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID);
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);
	//查询逻辑属性
	virtual STRING			GetAttributeValue(LPCTSTR szAttributeName);

	//获得银行租赁箱的开始索引值
	virtual INT				GetCurBankRentBegin(VOID);


public:

	// 得到装备的属性(白, 绿, 蓝)
	//INT GetEquipQuantity(); //remove

	// 得到装备上的宝石个数
	INT GetGemInEquipCount();

	// 得到装备的最大耐久
	//INT GetItemMaxRepair();

	// 得到装备上的宝石的图标
	VOID GetGemIcon(INT iGemIndex, STRING& strIconName);

	// 得到装备上的宝石描述
	VOID GetGemInEquipDesc(STRING& strIconName);

	// 得到属于第几个任务
	INT	GetQuestIndex(VOID)	{return m_Quest_Number;}

	// 得到装备的鉴定信息
	INT GetEquipIdentify();


public:

	//-----------------------------------------------------------------------------------------------------------------------
	//
	// 宝石信息
	//

	// 得到宝石等级
	INT GetGemLevel();

	// 得到宝石属性信息
	LPCTSTR	GetGemArribInfo();

	
	
	//-----------------------------------------------------------------------------------------------------------------------
	//
	// 魂珠信息
	//

	// 得到魂珠的属性类型
	LPCTSTR GetFecthHeadInfo_TypeDesc( INT Index );

	// 得到魂珠的属性值
	INT GetFecthHeadInfo_Value( INT Index );

	//得到魂珠的属性的数量
	INT GetFecthHeadInfo_Num();

	//-----------------------------------------------------------------------------------------------------------------------
	//
	// 原料和药品
	//

	// 得到原料或药品的质量
	//INT GetItemQuality(); //remove

	//得到装备是普通的还是高级的
	INT  GetItemEquipQua();
	// 得到消耗品在表中的类型
	INT GetMedicineItemTableType();
	// 得到配方需要的技能名称。
	LPCTSTR GetPrescrNeedSkillName();
	// 得到配方需要的技能等级。
	int GetPrescrNeedSkillLevel();
	// 得到配方需要技能的当前等级。
	int GetPrescrNeedSkillCurLevel();


	// 使用鉴定卷轴 
	VOID UseIdentify();

	// 准备使用鉴定卷轴 
	VOID PrepareUseIdentify();

	// 取消鉴定状态 
	VOID CancelIdentify();

	// 得到坐骑增加速度         [7/21/2010 陈军龙]
	INT	 GetMountAddtionSpeed();

	// 得到坐骑存在时限         [7/21/2010 陈军龙]
	INT  GetMountLife();

	// 得到坐骑获取时间         [7/21/2010 陈军龙]
	INT	GetMountObtainDate();

	// 得到坐骑剩余使用时间     [7/21/2010 陈军龙]
	INT  GetMountRemainTime();

	// 得到坐骑当前是否已绑定   [7/21/2010 陈军龙]
	BOOL GetMountIsBinded();

	// 得到坐骑绑定类型         [7/21/2010 陈军龙]
	BOOL  GetMountBindType();

public:

	CActionItem_Item(INT nID);
	virtual ~CActionItem_Item();

	// 
	VOID	Update_Item(tObject_Item* pItem);

	// 发送请求详细数据消息
	void SendAskItemInfoMsg();

	//得到物品
	tObject_Item*			GetItemImpl(VOID);
	

protected:

	//用于物品操作时，物品ID(指向tObject_Item)
	INT		m_idItemImpl;

	//属于第几个任务
	INT		m_Quest_Number;

private:

	// 把装备从身上卸下
	VOID	UnUseEquip();
	//DoAction_Packet
	VOID	DoAction_Packet(VOID);
	//销毁物品的操作
	VOID	DestoryItem(LPCSTR szSourceName);

	//Ctrl+Click
	BOOL InjectItemInfo( BOOL bSubAction );

};
