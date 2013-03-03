
#pragma once


#include "Obj_Item.h"
#include "GameDefine2.h"
#include "GameDefine_Item.h"
#include "Obj_Item_Equip.h"
#include "GIDBC_Struct.h"
#include "../../ObjectDef.h"
#include "GameStruct_Item.h"
#include "DBC\GMDataBase.h"
#include "Procedure/GameProcedure.h"
#include "GIException.h"

/*
|
|	坐骑  [7/8/2010 陈军龙]
|
*/
class CObject;
struct PET_GUID_t;
class CObject_Item_Mount : public CObject_Item
{
public:
	enum MOUNT_BIND_TYPE
	{
		GETUP_BIND = 0,  		// 获得绑定
		EQUIP_BIND,				// 装备绑定  
	};
	//const INT MILLISECOND_IN_ONE_DAY = 86400000; //1000 * 60 * 60 * 24

public:

	// 得到物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const ;
	// 得到type信息
	virtual INT				GetItemTableType(VOID);

	// 图标名
	virtual LPCTSTR			GetIconName(VOID) const;

	// 物品的名称
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	// 物品解释
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }

	// 得到物品买入的价格
	virtual INT			    GetItemBasePrice();

	// 得到卖出价
	virtual INT				GetItemPrice();

	// 得到有效时间
	INT		                GetEffectTime();

	// 得到有效格子数
	INT		                GetEffectGridNum();

	// 得到物品的类型描述 
	virtual LPCTSTR			GetItemTableTypeDesc();

	// 得到掉落时的模型id
	virtual LPCTSTR			GetDropVisualID(VOID)	const;
	// 显示名称颜色
	virtual LPCTSTR			GetDropVisColor(VOID) const ;


	// 设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn ){};

	// 详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID);

	// 得到玩家使用这个物品需要的等级
	virtual INT				GetNeedLevel()  { return  m_theBaseDef ->nNeedLevel; }


    /////////////////////////////////////  CObject_Item_Mount扩展部分  /////////////////////////////////////

	// 得到坐骑mount_att表索引 ID
	INT	        	        GetMountAttID() { return m_theBaseDef->nID;}

	// 得到坐骑char_mount索引 ID
	INT		                GetMountCharID() { return m_theBaseDef->nCharMountID;}

	// 得到坐骑模型 ID
	INT		                GetMountModelID(){ return m_characterData->m_nModelID;}

    // 得到增加的速度
	INT                     GetMountAddtionSpeed() { return m_theBaseDef->nAddtionSpeed; }

	// 得到存在时限
	INT                     GetMountLife() { return m_theBaseDef->nLife; }

	// 得到绑定类型
	MOUNT_BIND_TYPE         GetMountBindType() { return (MOUNT_BIND_TYPE) m_theBaseDef->nBindType; }

	// 得到是否已绑定
	BOOL                    GetMountIsBinded() { return m_bIsBinded; }

	// 设置是否已绑定
	VOID                    SetMountIsBinded(BOOL bBind) { m_bIsBinded = bBind; }

	// 得到获得时间
	UINT                    GetMountObtainDate() { return m_nObtainDate; }

	// 设置获得时间
	VOID                    SetMountObtainDate(UINT nDate) { m_nObtainDate = nDate; }

	// 得到是否已到期
	BOOL                    GetMountIsDue() { return m_bIsDue; }

	// 设置是否已到期
	VOID                    SetMountIsDue(BOOL bDue) { m_bIsDue = bDue; }

	// 得到剩余使用时间
	UINT                    GetMountRemainTime() { return m_nRemainTime; }

	// 设置剩余使用时间
	VOID                    SetMountRemainTime(UINT nTime) { m_nRemainTime = nTime; }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	                        CObject_Item_Mount(INT id);
	virtual                ~CObject_Item_Mount();

	// 生成坐骑
	VOID					AsMount(const _DBC_ITEM_MOUNT_ATT* pMedicDefine);

protected:

	_DBC_ITEM_MOUNT_ATT*	      m_theBaseDef;			// 基本属性定义(数据表)
	const _DBC_CHARACTER_MOUNT*   m_characterData;        // 显示属性数据(数据表)
	BOOL                          m_bIsBinded;            // 当前是否已绑定
	UINT                          m_nObtainDate;          // 获取坐骑时的时间
	UINT                          m_nRemainTime;          // 剩余使用时间
	BOOL                          m_bIsDue;               // 是否已到期
	STRING					      m_strExtraDesc;
};
