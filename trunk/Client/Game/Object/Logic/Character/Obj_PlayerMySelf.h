
/** Obj_PlayerMySelf.h
 */

#pragma once


#include "Obj_PlayerOther.h"



/** 玩家自身数据
 */
class CObject_PlayerMySelf : public CObject_PlayerOther
{
public:

	//// 压入一条指令
	//virtual BOOL			PushCommand(const SCommand_Object *pCmd );

	/** 左键指令的分析 */
	virtual BOOL			FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	/** 右键指令的分析 */
	virtual BOOL			FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );

	/** 更新头顶信息 */
	virtual VOID			Tick_UpdateInfoBoard( VOID );
	/** 设置头顶称号信息的状态 */
	virtual VOID			SetBoardTitleState( BOOL bShow, INT nType );
	/** 设置头顶状态信息的状态 */
	virtual VOID			SetBoardHpLevelState( BOOL bShow );

	/** 设置搜侠录中的模型播放的动作 */
	VOID					SetSouXiaUseSkill_id( SkillID_t id );

	/** 设置搜侠录中的武器预览 */
	VOID					SetSouXiaPrewArm( INT ArmID, INT EquipPoint );
	/** 清除搜侠录中的武器预览 */
	VOID					ClearSouXiaPrewArm();
	/** 设置搜侠录中的神兽召唤预览 */
	VOID					SetSouXiaPrewPet( INT modelId );
	/** 设置搜侠录中的坐骑召唤预览 */
	VOID					SetSouXiaPrewZuoJi( INT modelId );

public:
	
	/** 返回角色类型 */
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const					{ return CT_PLAYERMYSELF;	}
	BOOL					IsInVisualFiled( CObject_Character* pObj );

protected:
public:

/** 逻辑数据发生改变
 */

	friend class CCharacterData;

	virtual VOID			SetPosition(const fVector3& fvPosition);
	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_HairMesh(VOID);
	virtual VOID			OnDataChanged_HairColor(VOID);
	virtual VOID			OnDataChanged_FaceMesh(VOID);
	virtual VOID			OnDataChanged_Equip(HUMAN_EQUIP point);
	/** 头像改变事件函数 */
	virtual VOID			OnDataChanged_FaceImage(VOID);			
	virtual VOID			OnDataChanged_Die(VOID);
	virtual VOID			OnDataChanged_Level(VOID);
	virtual VOID			OnDataChanged_Profession(VOID);
	virtual VOID			OnDataChanged_HP(VOID);				// 更新生命值
	virtual VOID			OnDataChanged_CurTitles(INT nType);

protected:

	/// 用于UI显示的逻辑对象
	CObject_PlayerOther*	m_pAvatar;

	/// 用于搜侠录技能预览显示的逻辑对象
	CObject_PlayerOther*	m_pAvatar2;

	/// 用于搜侠录武器预览显示的逻辑对象
	CObject_PlayerOther* m_pAvatarArm;

	/// 用于搜侠录预览显示的神兽，坐骑对象
	CObject_PlayerNPC*	m_pAvatarPet;
	CObject_PlayerNPC* m_pAvatarZuoJi;

	/// 用于UI显示的逻辑对象
//	CObject_PlayerOther*	m_pHeadAvatar;

public:

	INT						GetSimulantLogicCount(VOID) const				{ return m_nSimulantLogicCount; }
	VOID					SetSimulantLogicCount(INT nLogicCount)			{ m_nSimulantLogicCount = nLogicCount; }

//	virtual VOID			ResetLogicCount(VOID);
	virtual VOID			ResetBaseLogicCount(VOID);
	virtual VOID			ResetActionLogicCount(VOID);

protected:

	INT		m_nSimulantLogicCount;

public:
	
/** 核心继承
 */

	CObject_PlayerMySelf( VOID );
	virtual ~CObject_PlayerMySelf( VOID );

	/** 根据初始化物体，并同步到渲染层 */
	virtual	VOID			Initial(VOID*);
	virtual VOID			Release(VOID);

protected:
	
	KL_DECLARE_DYNAMIC(CObject_PlayerMySelf);
	KL_DECLARE_LOGICAL(true);

};
