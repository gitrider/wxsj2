
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
|	扩展背包类物品
|
*/
class CObject;
struct PET_GUID_t;
class CObject_Item_ExtraBag : public CObject_Item
{
public:

	//得到物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const ;
	// 得到type信息
	virtual INT				GetItemTableType(VOID);

	//图标名
	virtual LPCTSTR			GetIconName(VOID) const;

	//物品的名称
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	//物品解释
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }

	// 得到物品买入的价格
	virtual INT				GetItemBasePrice();

	// 得到卖出价
	virtual INT				GetItemPrice();

	//得到有效时间
	INT		GetEffectTime();

	//得到有效格子数
	INT		GetEffectGridNum();

	// 得到物品的类型描述 
	virtual LPCTSTR					GetItemTableTypeDesc();

	//得到掉落时的模型id
	virtual LPCTSTR				GetDropVisualID(VOID)	const;
	//显示名称颜色
	virtual LPCTSTR				GetDropVisColor(VOID) const ;


	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn ){};

	//详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID);


public:
	CObject_Item_ExtraBag(INT id);
	virtual ~CObject_Item_ExtraBag();
	//生成扩展背包
	VOID					AsExtraBag(const _DBC_ITEM_EXTRABAG* pMedicDefine);

protected:
	//--------------------------------------------------------------
	//基本属性
	const _DBC_ITEM_EXTRABAG*	m_theBaseDef;			//基本属性定义(数据表)

	STRING					m_strExtraDesc;
};
