
/**	Obj_PlayerNPC.h
 */

#pragma once


#include "Obj_Character.h"
#include "../../../DataPool/GMDP_Struct_Pet.h"



/// NPC 类型
enum ENUM_NPC_TYPE
{
	NPC_TYPE_INVALID	= -1,
	NPC_TYPE_MONSTER,			// 怪物
	NPC_TYPE_PET,				// 宠物
	NPC_TYPE_MOUNT,				// 坐骑

	NPC_TYPE_NUMBERS
};

/// 任务状态（对应到玩家）
enum MISSION_STATE
{
	MS_INVALID = -1,
	
	MS_MISS,				// 有可接任务
	MS_HAVE_MISS_NO_DONE,	// 已接任务， 未完成
	MS_HAVE_MISS_DONE,		// 已接任务， 已完成
};


struct _DBC_CREATURE_ATT;


/** 
	NPC角色类，会继承出 PlayerOther
 */
class CObject_PlayerNPC : public CObject_Character
{
public:

	CObject_PlayerNPC();
	virtual ~CObject_PlayerNPC();


	/** 
		根据初始化物体，并同步到渲染层 
	 */
	virtual	VOID Initial(VOID*);


	/** 
		返回角色类型
	 */
	virtual CHARACTER_TYPE		GetCharacterType( VOID ) const				{ return CT_MONSTER; }
	ENUM_NPC_TYPE				GetNpcType(VOID) const;
	VOID						SetNpcType(ENUM_NPC_TYPE eType);
	
	const _DBC_CREATURE_ATT*	GetCreatureAtt(void) const					{ return m_pCreatureInfo; }

	/** 服务器GUID 
	 */
	virtual VOID				SetServerGUID(GUID_t guid)					{ m_GUIDServer = guid; }
	virtual GUID_t				GetServerGUID(VOID) const					{ return m_GUIDServer; }

	/** 左键指令的分析 */
	virtual BOOL				FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	/** 右键指令的分析 */
	virtual BOOL				FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );


	/** 装配挂接类物品 */
	virtual VOID				EquipItem_BodyLocator( INT nID, ENUM_WEAPON_LOCATOR_TYPE loc );	
	/** 卸载物品(显示缺省外型) */
	virtual VOID				UnEquipItem( HUMAN_EQUIP nPart );

protected:

	/** RenderInterface创建/删除 */
	virtual VOID				CreateRenderInterface(VOID);

	virtual INT					CalcCharModelID( VOID ) const;

	virtual UINT				GetIdleInterval(VOID) const;

	virtual VOID				UpdateCharRace(VOID);


	/**
	 *	从表中读出怪身上的特效绑到怪
	 */
	VOID						SetNpcEffect( VOID );
	/**
	 *	清掉特效
	 */
	VOID						ReleaseNpcEffect();

public:
	VOID						SetNpcMountEffect(INT nMountID, SDATA_PET* pPetData);

protected:

	virtual VOID				OnDataChanged_RaceID(VOID);

protected:

	/// 在数据表中的逻辑数据结构
	const _DBC_CREATURE_ATT*	m_pCreatureInfo;

	/// GUID 服务器传回
	GUID_t						m_GUIDServer; 

	ENUM_NPC_TYPE				m_eNpcType;

public:

	virtual INT					CannotBeAttack(VOID) const;


	/** 获得该npc对应到玩家的任务状态 */
	MISSION_STATE				GetNpcMissionState( VOID );

	/**  */
	VOID						SetNpcMissionData( VOID );

	/** 添加该npc对应到玩家的任务 */
	VOID						AddNpcMission( INT nMissionId, INT nNPCState ); // 20100511 ModifyCode

	/** 删除该npc对应到玩家的任务 */
	VOID						DelNpcMission( INT nMissionId );

	/** 获得该npc对应到玩家的任务状态 */
	VOID						SetNpcMissionState( INT nMissionId );



	/// 任务状态数据
	struct NPC_MISSION_DATA
	{
		INT		nMissionId;
		INT		nMissionNPCState;	// 任务NPC的状态 1,接任务NPC; 2,交任务NPC; 3,既接又交NPC  // 20100511 ModifyCode
	};

	/// 
	std::vector< NPC_MISSION_DATA >	m_vMissionData;

	/// 当前状态
	MISSION_STATE					m_eMissionState;
	/// 状态是否改变了  
	BOOL							m_bMissStateChange;


protected:

/**	改变怪物的武器
 */

	friend class CCharacterData;

	virtual VOID				OnDataChanged_Equip(HUMAN_EQUIP point);

	VOID						UpdateEquip(HUMAN_EQUIP point);


public:

/** 与移动NPC对话停下来后需要设置NPC的转向问题
 */

	VOID						SetNeedFaceToPlayer(BOOL bValue)			{ m_bNeedFaceToPlayer = bValue; }
	BOOL						GetNeedFaceToPlayer(VOID) const				{ return m_bNeedFaceToPlayer; }
	/** 是否会转向玩家 */
	BOOL						IsCanFaceToPlayer(VOID);
	/** 是否显示信息板 */
	BOOL						IsDisplayBoard(VOID);
	/** 头顶信息板高度 */
	FLOAT						GetBoardHeight(VOID);
	/** 选中环大小 */
	virtual FLOAT				GetProjtexRange(VOID);
	/** 阴影大小 */
	virtual FLOAT				GetShadowRange(VOID);

	LPCSTR						GetPortrait(VOID);

protected:

	/** 转向玩家 */
	VOID						FaceToPlayer(VOID);
	VOID						StopFaceToPlayer(VOID);

	/** 处理NPC转向 */
	VOID						ProcessFaceToPlayer(VOID); 

private:

	BOOL						m_bNeedFaceToPlayer;
	BOOL						m_bFaceToPlayer;
	UINT						m_uStartFaceToPlayerTime;
	/// 该Obj旋转之前的角度,初始化为<0.0f, 0.0f, 0.0f>
	fVector3					m_fvPreRotation;

protected:

	virtual VOID				CharacterLogic_OnStopped(BOOL bFinished, LOGIC_TYPE nLogicTag);
	virtual BOOL				Tick_Idle(UINT uElapseTime);
	const fVector3&				GetPreRotation(VOID)const					{ return m_fvPreRotation; }

public:

	KL_DECLARE_DYNAMIC(CObject_PlayerNPC);
	KL_DECLARE_LOGICAL(true);

};
