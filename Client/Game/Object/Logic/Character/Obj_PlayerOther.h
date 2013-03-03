
#pragma once


#include "Obj_PlayerNPC.h"



class CObject_Item_Equip;

/** 其他玩家角色类
 */
class CObject_PlayerOther : public CObject_PlayerNPC
{
public:

	CObject_PlayerOther();
	virtual ~CObject_PlayerOther();

	/** 根据初始化物体，并同步到渲染层 */
	virtual	VOID			Initial(VOID*);


	/** 返回角色类型 */
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const					{ return CT_PLAYEROTHER; }
	/** 发出请求更详细信息的请求 */
	virtual VOID			SendEuipRequest(VOID);
	/** 指令接收, 外界控制角色的唯一接口 */
	virtual RC_RESULT		OnCommand(const SCommand_Object *pCmd );

	/** 左键指令的分析 */
	virtual BOOL			FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	/** 右键指令的分析 */
	virtual BOOL			FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );


	//================
	// 装配物品
	//================

	// 替换身体部件
	VOID					EquipItem_BodyPart(HUMAN_EQUIP nPart, INT nID );
	// 清除特性装备的特效
	VOID					DelEquipEffect( HUMAN_EQUIP nPart );
	// 设置装备特效
	VOID					SetBodyEquipEffect( INT nEquipId, HUMAN_EQUIP nPart );
	VOID					SetBodyEquipEffect( HUMAN_EQUIP nPart, LPCTSTR szBindPoint, LPCTSTR szEffect );

	//释放某个技能
	VOID					SetUseSkillID( SkillID_t id );

	/** 改变动作
	*/
	BOOL					ChangeAction( INT nSetID, BOOL bLoop, ENUM_ACTION_TYPE ePart=ACTION_BODY, FLOAT fFuseParam=-0.03 );

	// 添加武器上默认的特效
	VOID					AddEquipEffect( INT nEquipId, HUMAN_EQUIP eEquipType );

	//	挂武器
	virtual VOID			EquipItem_BodyLocator( INT nID, ENUM_WEAPON_LOCATOR_TYPE loc );

	//  卸载物品(显示缺省外型) 
	virtual VOID			UnEquipItem( HUMAN_EQUIP nPart );

	virtual INT				GetBusObjID(VOID)const;

	


protected:

	//================
	// 逻辑数据发生改变
	//================

	friend class CCharacterData;

	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_FaceMesh(VOID);
	virtual VOID			OnDataChanged_HairMesh(VOID);
	virtual VOID			OnDataChanged_HairColor(VOID);
	virtual VOID			OnDataChanged_EquipVer(VOID);
	virtual VOID			OnDataChanged_Equip(HUMAN_EQUIP point);
//	virtual VOID			OnDataChanged_Titles(INT index);
	/** 头像改变事件函数 */
	virtual VOID			OnDataChanged_FaceImage(VOID);
	virtual VOID			OnDataChanged_Profession(VOID);

protected:

	VOID					UpdateFaceMesh(VOID);
	VOID					UpdateHairMesh(VOID);
	VOID					UpdateHairColor(VOID);
	VOID					UpdateEquip(HUMAN_EQUIP point);

protected:

	virtual VOID			OnHideWeapon( INT nAppointedWeaponID, ENUM_WEAPON_LOCATOR_TYPE loc );
	virtual VOID			OnShowWeapon(VOID);

protected:

	/** RenderInterface创建/删除 */
	virtual VOID			CreateRenderInterface(VOID);

	virtual INT				CalcCharModelID( VOID ) const;

	virtual UINT			GetIdleInterval(VOID) const;

	virtual VOID			UpdateCharRace(VOID);


protected:

	/// CharRace表中的定义
	const _DBC_CHAR_RACE*	m_pCharRace;

	// 装备上的特效列表
	// map< HUMAN_EQUIP，特效列表 > 
	typedef std::map<INT, std::vector<UINT> > EquitEffectMap;

	EquitEffectMap m_EquitEffectMap;

protected:

	KL_DECLARE_DYNAMIC(CObject_PlayerOther);
	KL_DECLARE_LOGICAL(true);

};	// class
