
#pragma once


#include "Obj_Item.h"
#include "GameDefine2.h"
#include "GameDefine_Item.h"
#include "Obj_Item_Equip.h"
#include "GIDBC_Struct.h"
#include "../../ObjectDef.h"
#include "GameStruct_Item.h"

/*
|
|	药瓶类物品
|
*/
class CObject;
struct PET_GUID_t;
class CObject_Item_Task : public CObject_Item
{

 
 
public:
	//得到物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const ;
	// 得到type信息
	virtual INT				GetItemTableType(VOID);
	//得到表中的定义
	const _DBC_ITEM_TASK*	GetBaseDefine(void) const { return m_theBaseDef; }
	//物品的名称
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	//物品解释
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }
	//图标名
	virtual LPCTSTR			GetIconName(VOID) const;
	//详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID);
	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn ) {};
	/*virtual LPCTSTR	GetExtranInfo(){ return GetExtraDesc(); };*/
	//克隆详细信息
	virtual VOID			Clone(const CObject_Item * pItemSource);
	//得到是否可以广播
     BOOL				    GetBroadcast(VOID)const ;

	// 得到物品买入的价格
	virtual INT				GetItemBasePrice();
   // 得到卖出价
	virtual INT				GetItemPrice();

	// 得到物品会引起的技能ID
	INT						GetItemSkillID();
	//得到物品的选择对象类型
	virtual INT				GetItemTargetType();
	// 得到玩家使用这个物品需要的等级
	virtual INT				GetNeedLevel();

	//是否是区域范围使用物品
	bool					IsAreaTargetType(void) const;
	//是否目标对象是绝对唯一的(自己，自己的宠物)
	bool					IsTargetOne(void) const;

	// 得到物品的类型描述 
	LPCTSTR	GetItemTableTypeDesc();

	//掉落外形

	virtual LPCTSTR				GetDropVisualID(VOID)	const;

	virtual	LPCTSTR				GetDropVisColor(VOID)	const;

	//获取叠放数量
	virtual INT				GetLayNum(VOID) ;

public:
	// 得到消耗品在表中的类型
	INT GetTaskItemTableType();

public:
	CObject_Item_Task(INT id);
	virtual ~CObject_Item_Task();
	//生成一个任务物品
	VOID					AsTask(const _DBC_ITEM_TASK* pTaskDefine);

protected:
	//--------------------------------------------------------------
	//基本属性
	const _DBC_ITEM_TASK*	m_theBaseDef;			//基本属性定义(数据表)
	INT	WriteVarAttr( CHAR* pIn );
	BOOL	ReadVarAttr( CHAR* pOut, INT& OutLength );

	STRING					m_strExtraDesc;
};
