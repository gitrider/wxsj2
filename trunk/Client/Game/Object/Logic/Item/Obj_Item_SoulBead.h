
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
|	魂珠类物品
|
*/
#define SOULBEAD_PARAMNUM 4
class CObject;
struct PET_GUID_t;
class CObject_Item_SoulBead : public CObject_Item
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

	//得到有效时间
	INT		GetEffectTime();

	//得到是否广播
	INT		GetIsRadio();

	// 得到物品的类型描述 
	virtual LPCTSTR					GetItemTableTypeDesc();


	//得到选择对象类型
	INT		GetSelectObjType();

	//得到掉落时的模型id
	virtual LPCTSTR				GetDropVisualID(VOID)	const;
	//显示名称颜色
	virtual LPCTSTR				GetDropVisColor(VOID) const ;


	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn ){};

	//详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID);

	//INT	GetFecthHeadInfo_Type( INT Index );
	INT	GetFecthHeadInfo_Value( INT Index );

	LPCTSTR GetFecthHeadInfo_TypeDesc( INT Index );

	//得到魂珠的属性数量
	INT	GetFecthHeadInfo_Num(){ return m_soulbead.m_nAttrCount; };


public:
	CObject_Item_SoulBead(INT id);
	virtual ~CObject_Item_SoulBead();
	//生成扩展背包
	VOID					AsSoulBead(const _DBC_ITEM_SOULBEAD* pMedicDefine);

protected:
	//--------------------------------------------------------------
	//基本属性
	const _DBC_ITEM_SOULBEAD*	m_theBaseDef;			//基本属性定义(数据表)
	SOUL_BEAD_INFO				m_soulbead;			//魂珠属性

	std::map<ITEM_ATTRIBUTE, std::string> m_soulbeadTypeName;		//魂珠的属性名称

	STRING					m_strExtraDesc;
};
