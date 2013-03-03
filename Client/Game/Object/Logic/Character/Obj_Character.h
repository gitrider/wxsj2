
/** Obj_Character.h
 *	
 *	功能：	对人物的逻辑操作
 *
 *	修改记录：
 *			080401	SetBoardTitleState
 *					SetBoardHpLevelState
 *			
 */

#ifndef __OBJ_CHARACTER_H__
#define __OBJ_CHARACTER_H__


#include "Type.h"
#include "..\Obj_Dynamic.h"
#include "World\Path.h"
#include "..\..\ObjectDef.h"
#include "CharacterStruct.h"
#include "GIDBC_Struct.h"
#include "DataPool/GMDP_Struct_Pet.h"



#define TERRAIN_TYPE_NUM 6


namespace DBC
{
	class DBCFile;
}

class CAI_Base;
class CObjectCommand_Logic;
class CCharacterData;
class tCreatureBoard;
class tSoundSource;
class CObject_ProjTex_BloodStain;
struct SDATA_CHARACTER;
struct _DBC_CHAR_RACE;

/** 
 *	角色系列的基类，会继承出 PlayerOther,PlayerMySelf, PlayerNPC等
 */
class CObject_Character : public CObject_Dynamic
{
public:

	/// 角色obj的类型
	enum CHARACTER_TYPE
	{
		CT_INVALID	= -1,

		CT_PLAYERMYSELF,				// 玩家自己
		CT_PLAYEROTHER,					// 其他玩家
		CT_MONSTER						// 怪物/NPC
	};

	/// 当前角色类Object的逻辑状态
	enum ENUM_CHARACTER_LOGIC
	{
		CHARACTER_LOGIC_INVALID	= -1,	// 无效

		CHARACTER_LOGIC_IDLE,			// 休闲
		CHARACTER_LOGIC_ACTION,			// 动作
		CHARACTER_LOGIC_MOVE,			// 移动
		CHARACTER_LOGIC_SKILL_GATHER,	// 聚集
		CHARACTER_LOGIC_SKILL_LEAD,		// 引导
		CHARACTER_LOGIC_SKILL_PREP_SEND,// 准备发射		//test2
		CHARACTER_LOGIC_SKILL_SEND,		// 发射
		CHARACTER_LOGIC_ABILITY_ACTION,	// 生活技能
		CHARACTER_LOGIC_DEAD,			// 死亡
		CHARACTER_LOGIC_STALL,			// 摆摊

		CHARACTER_LOGIC_NUMBERS
	};

	/// 定义地面类型
	enum TERRAIN_TYPE
	{
		GENERAL_TER = 1,				// 普通地面
		GRASS_TER	= 2,				// 草地
		GROUND_TER	= 4,				// 土地
		SNOW_TER	= 8,				// 雪地
		WATER_TER	= 16,				// 水
		ROCK_TER	= 32,				// 石头
	};

	/// 逻辑的类型
	enum LOGIC_TYPE
	{
		LOGIC_BASE,						// 基础逻辑
		LOGIC_ACTION,					// 行为逻辑
	};


public:

/** ================ 基本接口
 */

	/** 
	 *	返回角色类型 
	 */
	virtual CHARACTER_TYPE	GetCharacterType(VOID) const					{ return CT_INVALID; }
	
	/** 获取头顶信息板 
	 */
	tCreatureBoard*			GetInfoBoard(void)								{ return m_pInfoBoard; }

	GUID_t	GetGUID(VOID) ;
	
	/** 是否能够被作为主目标选择 
	 */
	virtual BOOL			CanbeSelect(VOID) const							{ return true; }
	VOID					MouseHoverState( BOOL bState ) const;
	
	//
	virtual UINT			GetIdleInterval(VOID) const						{ return UINT_MAX; }
	
	UINT					CalcIdleRandInterval(VOID);

	BOOL					IsDie(VOID) const;

	// 20100415 AddCodeBegin
	NPC_AUTOPATH*			GetIsAutoNPCPath()const		{ return (NPC_AUTOPATH*)&m_ObjAutoPath; } // 获取NPC自动寻路功能的相关属性
	// 20100415 AddCodeEnd  

	// 20100415 AddCodeBegin
	VOID					SetIsAutoNPCPath( INT nSceneID, STRING strNPCName, BOOL isAutoPath ); // 设置NPC自动寻路功能的相关属性
	// 20100415 AddCodeEnd

	// 20100416 AddCodeBegin
	VOID					QuestOpenNPCDlg();	 // 请求打开NPC对话窗口
	// 20100416 AddCodeEnd  

public:

/** ================ 角色逻辑数据相关
 */

	/** 得到逻辑数据 
	 */
	CCharacterData*			GetCharacterData(VOID)							{ return m_pCharacterData; }
	const CCharacterData*	GetConstCharacterData(VOID) const				{ return m_pCharacterData; }

	/** 获取增加颜色后的名称 
	 */
	STRING					GetNameWithColor(void);

protected:

	friend class CCharacterData;
	friend class CAI_MySelf;

	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_ModelID(VOID);
	virtual VOID			OnDataChanged_MountID( VOID );
	virtual VOID			OnDataChanged_Mount( VOID );        // 更新拥有的马的数据 [7/14/2010 陈军龙]
	virtual VOID			OnDataChanged_Name(VOID);
	virtual VOID			OnDataChanged_CurTitles(INT nType);
	virtual VOID			OnDataChanged_MoveSpeed(VOID);
	virtual VOID			OnDataChanged_StallName(VOID);
	virtual VOID			OnDataChanged_IsInStall(VOID);
	virtual VOID			OnDataChanged_CountryFlag(VOID);
	virtual VOID			OnDataChanged_TeamLeaderFlag(VOID);	// 更新队长标记
	virtual VOID			OnDataChanged_TeamFlag(VOID);		// 更新组队标记
	virtual VOID			OnDataChanged_Die(VOID);
	virtual VOID			OnDataChanged_StealthLevel(VOID);
	virtual VOID			OnDataChanged_FightState(VOID);
	virtual VOID			OnDataChanged_Sit(VOID);
	virtual VOID			OnDataChanged_Level(VOID);			// 更新等级
	virtual VOID			OnDataChanged_Profession(VOID);
	virtual VOID			OnDataChanged_HP(VOID);				// 更新生命值
	virtual VOID			OnDataChanged_Penuma(VOID);			// 更新元气
	virtual VOID			OnDataChanged_BusObjID(VOID);		//Bus

	/// 角色逻辑数据
	CCharacterData*			m_pCharacterData;


public:

/**  ================ 武器
 */

	ENUM_WEAPON_TYPE		GetWeaponType( ENUM_WEAPON_LOCATOR_TYPE loc ) const						
	{
		if( WL_RIGHT == loc )
			return m_theRWeaponType; 
		else if( WL_LEFT == loc )
			return m_theLWeaponType; 

		return WEAPON_TYPE_INVALID;
	}

	/**
	 *	根据武器类型获取相应的 字符串
	 */
	LPCSTR					GetWeaponName( ENUM_WEAPON_TYPE wType );

	/**
	 *	更改武器上的特效
	*/
	VOID					ChangerWeaponEffect( tEntityNode::eWEAPATTR ePart, LPCTSTR szEffectName, LPCTSTR nLocatorName, UINT color );

protected:

	/** 武器类型 
	 */
	VOID					SetWeaponType( ENUM_WEAPON_TYPE type, ENUM_WEAPON_LOCATOR_TYPE loc = WL_BOTH )			
	{
		switch( loc )
		{
		case WL_RIGHT:
			m_theRWeaponType = type;
			break;
		case WL_LEFT:
			m_theLWeaponType = type;
			break;
		case WL_BOTH:
			m_theRWeaponType = type;
			m_theLWeaponType = type;
			break;
		};
	}

	/// 当前武器类型
	ENUM_WEAPON_TYPE		m_theLWeaponType;
	ENUM_WEAPON_TYPE		m_theRWeaponType;


public:

/** ================ 角色附加效果逻辑相关
 */

	const CImpactEffectMap*	GetImpactEffectMap( VOID ) const				{ return &m_mapImpactEffect; }
	VOID					RemoveAllImpact( VOID );

protected:

	VOID					UpdateBuffEffect(VOID);

	VOID					TickBuffElapseCount();	// 每秒更新下buff的剩余时间		20100531 BLL

	BOOL					InsertImpact( UINT nSN, INT idImpact, INT nTimeCount, ObjID_t nCreatorID, INT nNumOflayer,  BOOL bShowEnableEffect );

	VOID					RemoveImpact( UINT nSN );
	VOID					UpdatePetFrame();

	VOID					Tick_UpdateEffect(VOID);
	VOID					UpdateEffectTargetPos(const SImpactEffect *pImpactEffect);

	VOID					SetElement_Title(LPCTSTR szTitle, INT nType);

protected:

	/// 附加效果所产生的特效, 实时更新用
	CImpactEffectMap		m_mapImpactEffect;
	// 延迟处理用
	CImpactEffectMap		m_mapImpactDelay;


	BOOL					m_bHaveHitAnimation;
	
public:

/** ================ 跳跃相关
 */

	VOID					SetbJumping( BOOL bJumping )					{ m_bJumping = bJumping; }
	BOOL					GetbJumping( VOID ) const						{ return m_bJumping; }
	BOOL					Jump( VOID );

private:

	BOOL					m_bJumping;

public:

	BOOL					SitDown(VOID);
	BOOL					StandUp(VOID);

public:

/** ================ 指令相关
 */

	/** 压入一条指令 
	 */
	virtual BOOL			PushCommand( const SCommand_Object *pCmd );

protected:

	/** 执行客气端的模拟指令，只充许AI调用，其他地方勿用，这个将清空缓存的指令表 
	 */
	RC_RESULT				DoSimulantCommand(const SCommand_Object *pCmd);

	virtual RC_RESULT		DoCommand( const SCommand_Object *pCmd );
	virtual RC_RESULT		OnCommand( const SCommand_Object *pCmd );

	/** 处理缓存的指令 
	 */
	virtual BOOL				ProcessLogicCommand( VOID );

	/** 处理下一条逻辑
     */
	BOOL						DoNextLogicCommand( LOGIC_TYPE nLogicTag );

	/** 处理逻辑
     */
	BOOL						DoLogicCommand( CObjectCommand_Logic *pLogicCmd );

	VOID						SetLogicSpeed(FLOAT fSpeed);
	FLOAT						GetLogicSpeed(VOID) const						{ return m_fLogic_Speed; }

	VOID						OnLogicSpeedChanged(VOID);

protected:

	/** 判断列表是否为空
	 */
	BOOL						IsBaseLogicCommandListEmpty( VOID );
	BOOL						IsActionLogicCommandListEmpty( VOID );

	/** 保存逻辑命令
	 */
	BOOL						PushBaseLogicCommand( CObjectCommand_Logic *pCmd );
	BOOL						PushActionLogicCommand( CObjectCommand_Logic *pCmd );
	
	/** 获取下一条逻辑命令
	 */
	const CObjectCommand_Logic*	GetNextLogicCommand( LOGIC_TYPE nLogicTag ) const;
	CObjectCommand_Logic*		NextLogicCommand( LOGIC_TYPE nLogicTag );

	/** 查找指定的逻辑 
	 */
	CObjectCommand_Logic*		FindBaseLogicCommand( INT nLogicCount );
	CObjectCommand_Logic*		FindActionLogicCommand( INT nLogicCount );

	/** 清空列表
 	 */
	virtual BOOL				CleanupLogicCommandList( LOGIC_TYPE nLogicTag );


	/**
	 *	技能释放的预备动作
	 */
	BOOL						PrepMagicSend( const SCommand_Object *pCmd );

protected:

	/// 逻辑命令列表
	typedef std::list<CObjectCommand_Logic*>	CObjectLogicCommandList;
	CObjectLogicCommandList						m_listBaseLogicCommand;
	CObjectLogicCommandList						m_listActionLogicCommand;
	
	/// 当前逻揖执行的速度
	FLOAT										m_fLogic_Speed;					

public:

/** ================ 角色行为逻辑相关
 */

	/** 得到AI 
	 */
	CAI_Base*				CharacterLogic_GetAI(VOID)					{ return m_pTheAI; }

	/** 得到当前的逻辑状态 
	 */
	ENUM_CHARACTER_LOGIC	CharacterLogic_Get( LOGIC_TYPE nLogicTag ) const;

	/** 当前逻辑是否停止 
	 */
	BOOL					CharacterLogic_IsStopped( LOGIC_TYPE nLogicTag ) const;

	/** 判断逻辑是否准备好
	 */
//	virtual BOOL			IsLogicReady( INT nLogicCount ) const;
	virtual BOOL			IsBaseLogicReady( INT nLogicCount ) const;
	virtual BOOL			IsActionLogicReady( INT nLogicCount ) const;

	INT						GetLastModifyPosLogicCount(VOID) const		{ return m_nLastModifyPosLogicCount; }

	BOOL					IsIdle(VOID) const							{ return (CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_IDLE)?(TRUE):(FALSE); }

	BOOL					IsMoving(VOID) const						{ return (CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_MOVE && !CharacterLogic_IsStopped(LOGIC_BASE)) ? (TRUE):(FALSE); }

	BOOL					IsFightState(VOID) const						{ return m_uFightStateTime > 0; }
protected:

	/** 操作当前的逻辑状态
	 */
	VOID					CharacterLogic_Set(ENUM_CHARACTER_LOGIC eLogic, LOGIC_TYPE nLogicTag );

	/** 根据逻辑类型停止相应的逻辑命令
	 */
	VOID					CharacterLogic_Stop( BOOL bFinished, LOGIC_TYPE nLogicTag );

	/** 逻辑停止事件，任何逻辑的任何条件停止都得调用(这个函数的调用是在设置下一逻辑之前)
	 *	bFinished	:	是否为成功执行完毕
	 */
	virtual VOID			CharacterLogic_OnStopped( BOOL bFinished, LOGIC_TYPE nLogicTag );

	VOID					SetLastModifyPosLogicCount(INT nLogicCount)			{ m_nLastModifyPosLogicCount = nLogicCount; }


/** ================ 进入某一种逻辑状态
 */

	// 休闲
	BOOL					Start_Idle(VOID);
	// 动作
	BOOL					Start_Action(CObjectCommand_Logic *pLogicCommand);
	// 移动
	BOOL					Start_Move(CObjectCommand_Logic *pLogicCommand);
	// 法术聚气
	BOOL					Start_MagicCharge(CObjectCommand_Logic *pLogicCommand);
	// 法术引导
	BOOL					Start_MagicChannel(CObjectCommand_Logic *pLogicCommand);
	//test2
	// 准备发招
	BOOL					Start_MagicPrepSend(CObjectCommand_Logic *pLogicCommand);
	// 法术发招
	BOOL					Start_MagicSend(CObjectCommand_Logic *pLogicCommand);
	// 死亡
	BOOL					Start_Dead( BOOL bPlayDieAni );
	// 生活技能
	BOOL					Start_Ability(CObjectCommand_Logic *pLogicCommand);
	// 摆摊
	BOOL					Start_Stall(BOOL bPlayAni);

protected:

/** ================ 在一种逻辑状态下的逻辑Tick
 */
	virtual BOOL			Tick_Idle(UINT uElapseTime);
	virtual BOOL			Tick_Action(UINT uElapseTime);
	virtual BOOL			Tick_Move(UINT uElapseTime);
	virtual BOOL			Tick_MagicCharge(UINT uElapseTime);
	virtual BOOL			Tick_MagicChannel(UINT uElapseTime);
	virtual BOOL			Tick_MagicPrepSend(UINT uElapseTime);	//test2
	virtual BOOL			Tick_MagicSend(UINT uElapseTime);
	virtual BOOL			Tick_Dead(UINT uElapseTime);
	virtual BOOL			Tick_Ability(UINT uElapseTime);
	virtual BOOL			Tick_Stall(UINT uElapseTime);

protected:

	/** 操作当前使用的逻辑命令
	 */
	CObjectCommand_Logic*	GetBaseLogicCommand(VOID);
	VOID					SetBaseLogicCommand( CObjectCommand_Logic *pLogicCommand );
	CObjectCommand_Logic*	GetActionLogicCommand(VOID);
	VOID					SetActionLogicCommand( CObjectCommand_Logic *pLogicCommand );

	VOID					CheckMoveStop(VOID);

protected:

/** ================ 在某种逻辑状态下的参数
 */



	struct SLogicParam_Idle
	{
		UINT		m_uIdleInterval;				/// 间隔多少时间可以随机一次休闲动作
		UINT		m_uLastIdleMotionTime;			/// 上一次做休闲动作的时间
		UINT		m_uStartTime;					/// 待命逻辑的起始时间
	};

	struct SLogicParam_Move
	{
		INT			m_nCurrentNodeIndex;			/// 当前节点的索引值
		WORLD_POS	m_posSaveStart;					/// 保存当前的起始点
		WORLD_POS	m_posSaveTarget;				/// 保存当前的目标点
	};

	struct SLogicParam_MagicCharge
	{
		INT			m_nSkillID;
		UINT		m_uCurrentTime;					/// 当前进度时间
	};

	struct SLogicParam_MagicChannel
	{
		INT			m_nSkillID;
		UINT		m_uCurrentTime;					/// 当前进度时间
	};

	struct SLogicParam_MagicSend
	{
		INT			m_nSaveMagicID;					/// 当前法术ID
		BOOL		m_bBeAttackEffectShowed;		/// 因此次攻击所产生的被攻击者的表现是否已经表现
		BOOL		m_bCanBreak;					/// 是否可以结束该逻辑了
		INT			m_nActionIndex;					/// 为了让动作按顺序播放
		INT			m_nActionCount;					/// 动作数量
		BOOL		m_bDoNextAction_Concatenation;	/// 当技能的招式类型为SKILL_ACTION_TYPE_CONCATENATION时所用
		UINT		m_uAnimationTime;				/// 动已经播放的时间
		UINT		m_uAnimationEndElapseTime;		/// 动画停止的时间间隔
	};	

	struct SLogicParam_Ability
	{
		UINT		m_uTotalTime;
		UINT		m_uCurrentTime;
	};

	struct SLogicParam_Sit
	{
		BOOL		m_bStandUp;
	};

	struct SLogicParam_Stall
	{
		enum ENUM_STALL_ANIMATION
		{
			STALL_ANIMATION_INVALID	= -1,
			STALL_ANIMATION_SITDOWN,
			STALL_ANIMATION_IDLE,
			STALL_ANIMATION_STANDUP,

			STALL_ANIMATION_NUMBER
		};

		INT			m_nCurrentAnimation;			/// ENUM_STALL_ANIMATION
	};

	struct SLogicParam_Dead
	{
		fVector3	m_DeadPos;		// 死亡位置
		fVector3	m_CurrPos;		// 当前位置
		fVector3	m_TarPos;		// 目标位置
		fVector3	m_TarDir;		// 目标方向
		BOOL		m_bDeadFlyBegin;// 标识击飞动作开始
		BOOL		m_bDeadFlyEnd;	// 标识击飞动作结束
		BOOL		m_bDeadBegin;	// 标识死亡动作开始
		BOOL		m_bDeadEnd;		// 标识死亡动作结束
		INT			m_nCadaverAction;// 尸体动作， 要和死亡或击飞匹配
		BOOL		m_bDeadIsEnd;		// 标识结束， 动作都完成了
	};



	/// 正在执行的逻辑状态
	ENUM_CHARACTER_LOGIC		m_nCharacterBaseLogic;
	ENUM_CHARACTER_LOGIC		m_nCharacterActionLogic;

	/// 改变动作时用到的检测逻辑状态
	LOGIC_TYPE					m_eChangeAnimLogicType;

	/// 逻辑是否停止
	BOOL						m_bCharacterBaseLogicStopped;
	BOOL						m_bCharacterActionLogicStopped;

	/// AI对象
	CAI_Base*					m_pTheAI;

	INT							m_nLastServerLogicCount;		/// 最后得到的服务器端的逻辑计数
	INT							m_uLastServerTime;				/// 最后得到的服务器端时间
	INT							m_nLastModifyPosLogicCount;		/// 最后执行的改变位置的指令

	/// 当前逻辑命令
	CObjectCommand_Logic*		m_pBaseLogicCommand;	
	CObjectCommand_Logic*		m_pActionLogicCommand;
	
	SLogicParam_Idle			m_paramLogic_Idle;
	SLogicParam_Move			m_paramLogic_Move;
	SLogicParam_MagicCharge		m_paramLogic_MagicCharge;
	SLogicParam_MagicChannel	m_paramLogic_MagicChannel;
	SLogicParam_MagicSend		m_paramLogic_MagicSend;
	SLogicParam_Ability			m_paramLogic_Ability;
	SLogicParam_Sit				m_paramLogic_Sit;
	SLogicParam_Stall			m_paramLogic_Stall;
	SLogicParam_Dead			m_paramLogic_Dead;


	// 这几个主要是给坐骑用的， 坐骑只有移动和休闲两种状态

	INT							m_nMoveAnimId;	/// 移动的动作
	INT							m_nIdleAnimId;	/// 休闲的动作

	BOOL						m_bMoveState;	/// 是否移动状态


	/// 记录基本逻辑 和 行为逻辑的动作
	INT							m_nBaseAnimId;		/// 
	INT							m_nActionAnimId;	/// 

	/// 是否被击飞
	BOOL						m_bBeatFly;
	/// 击飞距离
	FLOAT						m_fBeatFlyDist;
	/// 是否是跟进状态（仅主角）
	BOOL						m_bTail;

	// 头顶伤害信息的公共时间间隔
	UINT						m_uDisplayInfoTimer;

	// 20100415 AddCodeBegin
	static NPC_AUTOPATH		m_ObjAutoPath;
	// 20100415 AddCodeEnd
public:

	BOOL			m_UseSkillTag;


public:

/** ================ 伤害的关联操作
 */

	BOOL					IsMySelf(INT ObjID) const;
	BOOL					IsMyPet(INT ObjID) const;
	VOID					ShowLogicEvent( INT nServerObjID, INT nLogicCount, BOOL bShowAll = FALSE );

	// 释放技能时，角色头顶冒技能名称		20100531 BLL
	VOID					DisplaySkillInfo( INT nSkillID );

protected:

	VOID					AddLogicEvent(const _LOGIC_EVENT *pLogicEvent);
	VOID					RemoveLogicEvent( INT nLogicCount );
	VOID					RemoveAllLogicEvent( VOID );

	VOID					Update_LogicEvent( VOID );
	
	/** 因此次攻击所产生的被攻击者的表现是否已经表现
	 */
	BOOL					IsBeAttackEffectShowed( VOID ) const			{ return m_paramLogic_MagicSend.m_bBeAttackEffectShowed; }

	BOOL					DoLogicEvent(const _LOGIC_EVENT *pLogicEvent);
	BOOL					DoLogicEvent_Damage(const _LOGIC_EVENT *pLogicEvent);

	VOID					DisplayDamageBoard(const _DAMAGE_INFO* pDamageInfo);
	VOID					DisplayMissImmuAndSoOn(const _DAMAGE_INFO* pDamageInfo);
	VOID					DisplayHitAnimation();


	BOOL					IsDisplayDamageInfo();
	VOID					UpdataDisplayInfoTimer( INT nAddTime );

protected:

	/// 保存逻辑事件（不能立即执行的）
	CLogicEventList			m_listLogicEvent;

	UINT					m_uTime_LogicEventListTick;


public:
	// 骑宠特效,对外开放的接口，UI骑宠模型会用到
	static VOID CreateMountEffect(tEntityNode* pMountInterface, INT nMountID, const SDATA_PET* pPetData=NULL);
	VOID					OnBulletHit();
protected:

/** ================ 角色外形相关
 */

	/** 在渲染层创建角色渲染指针 
	 */
	virtual VOID			CreateRenderInterface(VOID);

public:
	virtual VOID			ReleaseRenderInterface(VOID);

	/** 在渲染层创建坐骑渲染指针 
	 */
protected:
	virtual tEntityNode*	CreateMountRenderInterface(INT nMountID);
	virtual VOID			ReleaseMountRenderInterface( tEntityNode* mountInterface );
	/**
	 *	更改装备数据
	 */
	virtual VOID			OnDataChanged_Equip( HUMAN_EQUIP point )		{ };
	virtual VOID			OnDataChanged_MountEquip( HORSE_EQUIP point );

	/** 选中环大小 
	 */
	virtual FLOAT			GetProjtexRange(VOID)							{ return 200.0f; }
	/** 阴影大小 
	 */
	virtual FLOAT			GetShadowRange(VOID)							{ return 100.0f; }


	/**
	 *	受伤或死亡后地面的血迹（如果是受伤可能不止一次的调用）
	 */
	CObject_ProjTex_BloodStain*	m_pBloodStain;
public:
	BOOL					CreateBloodStain( VOID );
	tEntityNode*			GetMountRenderInterface() { return m_pMountRenderInterface; }

	VOID Bus_UpdateCharActionSetFile(VOID)
	{
		UpdateCharActionSetFile();
	}
	VOID Bus_ChangeAction(INT nSetID, FLOAT fSpeed, BOOL bLoop, FLOAT fFuseParam)
	{
		ChangeAction(nSetID, fSpeed, bLoop, ACTION_BODY, fFuseParam);
	}
	virtual INT				GetBusObjID(VOID)const{ return INVALID_ID; }

private:
	/**	
	 *	更新坐骑的外形
	 */
	VOID					UpdateMountEquip( HORSE_EQUIP point );	/* 坐骑的外形部件枚举 */
	VOID					UnMountEquip( HORSE_EQUIP point );

protected:

	VOID					StopCurrentAction();
	/** 改变动作
	 */
	BOOL					ChangeAction( INT nSetID, FLOAT fSpeed, BOOL bLoop, ENUM_ACTION_TYPE ePart=ACTION_BODY, FLOAT fFuseParam=-0.03 );
	/** 改变动作速度
	 */
	VOID					ChangeActionSpeed(FLOAT fSpeed);

	VOID					PlayAnimation(INT nSetID, FLOAT fStartTime, FLOAT fWeight);

	FLOAT				GetDefaultFUseParam(VOID)
	{
		return m_fDefaultFuse;
	}

	/** 动作结束标志
	 */
	BOOL					IsBaseAnimationEnd(VOID) const					{ return m_bBaseAnimationEnd; }
	VOID					SetBaseAnimationEnd(BOOL bSet)					{ m_bBaseAnimationEnd = bSet; }
	BOOL					IsActionAnimationEnd(VOID) const				{ return m_bActionAnimationEnd; }
	VOID					SetActionAnimationEnd(BOOL bSet)				{ m_bActionAnimationEnd = bSet; }

	VOID					UpdateTransparent(VOID);
	VOID					SetTransparent(FLOAT fTransparency, FLOAT fTime = 1.f);

	/** 刷新当前动作 
	 */
	VOID					FreshAnimation( ENUM_ACTION_TYPE ePart=ACTION_BODY, LOGIC_TYPE eLogic=LOGIC_BASE );

protected:

	/** 从资源表中获得人物动作文件
	 *	nActionSetID	:	ENUM_BASE_ACTION or 其它
	 *	nWeaponType		:	ENUM_WEAPON_TYPE
	 */
	virtual LPCSTR			GetCharActionNameByActionSetID(INT nActionSetID, INT nWeaponType, BOOL *pbHideWeapon = NULL, INT *pnAppointedWeaponID = NULL);
	virtual LPCSTR			GetMountActionNameByActionSetID(INT nActionSetID, INT nWeaponType);
	/**	
	 *	读武器动作表
	 *
	 *	nActionSetID	动作id
	 *	nWeaponType		武器类型
	 */
	LPCSTR					GetWeaponActionNameByActionSetID( INT nActionSetID, INT nWeaponType );

	virtual INT				GetCharModelID( VOID ) const;


	virtual INT				CalcCharModelID( VOID ) const;

	VOID					SetFightState(BOOL bSet);
protected:

	VOID					UpdateCharModel(VOID);
	VOID					UpdateMountModel(VOID);
	VOID					UpdateMountList(VOID);  // 更新拥有的马  [7/14/2010 陈军龙]
	virtual VOID			UpdateCharRace(VOID)							{ }
	VOID					UpdateCharActionSetFile(VOID);
	VOID					UpdateMountActionSetFile(VOID);
	VOID					UpdateWeaponActionSetFile( LPCTSTR szWeaponAnimName );

	virtual VOID			OnHideWeapon( INT nAppointedWeaponID, ENUM_WEAPON_LOCATOR_TYPE loc )			{ }
	virtual VOID			OnShowWeapon( VOID )								{ }

	BOOL					IsHideWeapon(VOID) const						{ return m_bHideWeapon; }


protected:

	/// 动作文件表
	const DBC::DBCFile		*m_pCharActionSetFile;
	const DBC::DBCFile		*m_pMountActionSetFile;
	const DBC::DBCFile*		m_pWeaponActionSetFile;

	// 坐骑数据
	struct MountData
	{
		tEntityNode*	pMountRenderInterface;	/// 坐骑的渲染层接口
		INT				nMountID;				/// 坐骑ID，对应char_mount.tab索引 
		INT				nMountModelID;			/// 人物坐骑模型对应表中的ID				
		FLOAT			fMountAddHeight;		/// 骑上坐骑后头顶信息板高度偏移

		MountData()
		{
			pMountRenderInterface	= NULL;
			nMountID				= INVALID_ID;
			nMountModelID			= INVALID_ID;
			fMountAddHeight			= 0.0f;
		}
	};
	// map< 坐骑ID，坐骑数据 >
	typedef std::map<INT, MountData> MapMountData;

	/// 拥有的所有坐骑信息。 坐骑的实例会一直保存着，待角色释放时一起释放。
	MapMountData			m_mapMyMount;

	// 角色当前骑着的坐骑,一个中间指针，不要拿来创建或释放
	tEntityNode*			m_pMountRenderInterface;

	// 当前骑上坐骑后头顶信息板高度偏移
	FLOAT					m_fMountAddHeight;

	/// 人物模型对应表中的ID
	INT						m_nCharModelID;
	
	BOOL					m_bHideWeapon;

	FLOAT				m_fDefaultFuse;

	/// 表示动作是否结束了
	BOOL					m_bBaseAnimationEnd;
	BOOL					m_bActionAnimationEnd;

	/// 攻击状态时间
	UINT					m_uFightStateTime;


	/// 武器标记
	BOOL					m_bRightWeapon;
	BOOL					m_bLeftWeapon;

protected:

	/** 当前在渲染层所执行的动画结束的事件通知函数 */
	static bool			_OnAnimationEnd(LPCTSTR szAnimationName, UINT dwParam, INT nPart, INT nLogic);
	virtual BOOL		OnAnimationEnd( LPCTSTR szAnimationName, ENUM_ACTION_TYPE ePart=ACTION_BODY, LOGIC_TYPE eLogic=LOGIC_BASE );

	/** 当前在渲染层所执行的动画可以停止的事件通知函数 */
	static bool			_OnAnimationCanBreak(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationCanBreak(LPCTSTR szAnimationName);

	/** 当前在渲染层所执行的动画打击点的事件通知函数 */
	static bool			_OnAnimationHit(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationHit(LPCTSTR szAnimationName);

	/** 当前在渲染层所执行的动画屏幕震动点事件通知函数 */
	static bool			_OnAnimationShakeEvent(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationShakeEvent(LPCTSTR szAnimationName);

	/** 通知一个物理事件发生 */
	virtual VOID		NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam = NULL);
protected:
/** ================ 马相关 
*/

	// 取得拥有的马
	MountData*				GetMyMount( INT nMountID );
	// 创建新马
	VOID					CreateNewMount(INT nMountID);
	// 设置马的可见
	inline VOID				SetMountVisible(tEntityNode* mountInterface, BOOL bShow);
	// 上马
	VOID					Mounting(tEntityNode* mountInterface);
	// 下马
	VOID					Dismount();


public:

/** ================ 移动路径相关 
 */

	/** 根据目标点创建行走路径
	 */
	virtual VOID				OnMoveStop( VOID );

public:

/** ================ 技能相关 
 */

	virtual INT					CannotBeAttack( VOID ) const				{ return 0; }
	virtual const _CAMP_DATA*	GetCampData(VOID) const;
	virtual ObjID_t				GetOwnerID(VOID) const;
	// 获取有动作时间的技能的ID (聚气技能或引导技能)
	INT							GetActivatedSkillID();


protected:

/** ================ 头顶信息板相关 
 */

	/// UI接口
	tCreatureBoard*			m_pInfoBoard;
	/// 位置
	fVector2				m_fvInfoBoardPos;


/** ================ 角色音效相关 
 */

	/// 脚步声音
	tSoundSource*			m_pWlakSound;
	/// 生活技能声音
	tSoundSource*			m_pLifeAbilitySound;


	/// 地面声音数组
	INT						m_SoundId[MAX_SOUND_NUMBER];

private:	
	
// 处理走在不同地面的声音

	/// 是否播放状态
	bool					mPlayState;
	/// 保存上一次的地面类型
	int						mTerrainType;


public:

/** ================ 角色聊天动作相关 
 */

	/** 设置动作列表 
	 */
	VOID					SetChatMoodAction(STRING& strOrder);	

protected:

	/** 清除队列里剩余的动作
	 */
	VOID					ClearAllChatMoodAction();				
	/** 每次从队列里弹出一个动作进行播放，角色动作逻辑必须处于idle状态才行 
	 */
	VOID					PlayChatMoodAction();					
	/** 是否有聊天动作
	 */
	BOOL					IsHaveChatMoodAction();					

	BOOL					IsChatMoodActionPlaying()						{ return m_bIsChatMoodPlaying; };
	VOID					SetChatMoodActionPlaying(BOOL bPlaying)			{ m_bIsChatMoodPlaying = bPlaying; };

protected:

	BOOL					m_bIsChatMoodPlaying;
	BOOL					m_bNeedStandUp;
	BOOL					m_bStanding;

	struct CHAT_MOOD_DATA
	{
		INT			m_ActionId;
		BOOL		m_bLoop;

		CHAT_MOOD_DATA()
		{
			m_ActionId = -1;
			m_bLoop = FALSE;
		};
	};

	typedef std::list<CHAT_MOOD_DATA> CHAT_MOOD_LIST;
	CHAT_MOOD_LIST	m_listChatMoodAction;

public:

/** ================ 掉落包事件 
 */

	BOOL					AddDropBoxEvent(ObjID_t idItemBox, GUID_t idOwner, const WORLD_POS *pCreatePos);
	BOOL					AddDropItemEvent(ObjID_t idItemDrop, GUID_t* idOwner, const WORLD_POS *pCreatePos,UINT time ,BOOL isCanPick);
private:

	BOOL			m_bDropBox_HaveData;
	ObjID_t			m_nDropBox_ItemBoxID;
	GUID_t			m_DropBox_OwnerGUID;
	WORLD_POS		m_posDropBox_CreatePos;

	BOOL			m_bDropItem_HaveData;
	ObjID_t			m_nDropItem_ItemDropID;
	GUID_t			m_DropItem_OwnerObjId[MAX_TEAM_MEMBER];
	WORLD_POS		m_posDropItem_CreatePos;
	UINT			m_ProtectTime;
	BOOL            m_bIsCanPick;


private:

	// 任务列表数据
	struct missionData
	{
		INT nSceneID;		// 接任务地图ID
	};

//	std::vector< missionData > m_
	


public:

/** ================ 核心继承 
 */

	CObject_Character();
	virtual ~CObject_Character();

	/** 角色初始化 
	 */
	virtual VOID			Initial(VOID* pParam);
	/** 释放物体所拥有的资源 
	 */
	virtual VOID			Release(VOID);
	/** 逻辑函数 
	 */
	virtual VOID			Tick(VOID);
	
	/** 检查是否不再可见 
	 */
	//virtual VOID			Tick_CheckAlive(VOID);

	/** 信息板Tick 
	 */
	virtual VOID			Tick_UpdateInfoBoard(VOID);

	// 收摊后，恢复摆摊前的头顶信息板状态	20100527 BLL
	virtual VOID			RestoreInfoBoard(VOID);

	virtual VOID			SetPosition(const fVector3& fvPosition);
	virtual VOID			SetRotation(const fVector3& fvRotation);
	virtual VOID			Enable(UINT dwFlag);
	virtual VOID			Disalbe(UINT dwFlag);

	virtual VOID			SetScale(FLOAT fScale);

	/**
	 *	设置头顶称号信息的状态
	 */
	virtual VOID			SetBoardTitleState( BOOL bShow, INT nType );
	/**
	 *	设置头顶状态信息的状态
	 */
	virtual VOID			SetBoardHpLevelState( BOOL bShow );



	//---- for debuger
	virtual STRING			GetDebugDesc(VOID);
	//---- for debuger

	void					Say( const STRING& str );

protected:

	//--------------------------------------------------------
	KL_DECLARE_DYNAMIC(CObject_Character);
	KL_DECLARE_LOGICAL(true);

};	// class


#endif // __OBJ_CHARACTER_H__
