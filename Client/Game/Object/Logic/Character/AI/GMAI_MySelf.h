
/** AI_MySelf.h
 */

#pragma once


#include "GIMath.h"
#include "GMAI_Base.h"
#include "Type.h"



/// AI要调用一个或多个LOGIC
enum ENUM_MYSELF_AI
{
	MYSELF_AI_INVALID	= -1,
	MYSELF_AI_IDLE,					// 空闲中
	MYSELF_AI_MOVE,					// 移动中
	MYSELF_AI_USE_SKILL,			// 技能使用中
	MYSELF_AI_ACTIVE_TRIPPEROBJ,	// 对TripperObj激活中
	MYSELF_AI_DEFAULT_EVENT,		// 对npc谈话请求中
	//MYSELF_AI_DEAD,				// 死亡
	MYSELF_AI_FOLLOW,				// 跟随
	//...

	//test5
	MYSELF_AI_OPERATE_SKILL,		// 处理从服务器返回的整合了 技能使用 和 结果的消息包

	MYSELF_AI_NUMBERS
};

class CPath;

class CAI_MySelf : public CAI_Base
{
public:

	/** 
		逻辑桢 
	*/
	virtual BOOL Tick( VOID );

	/** 重置 */
	virtual VOID Reset(VOID);


	virtual BOOL PushCommand_MoveTo(FLOAT fDestX, FLOAT fDestZ);
	virtual BOOL PushCommand_Jump(VOID);
	virtual BOOL PushCommand_UseSkill(INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir);
	//test5
	virtual BOOL PushCommand_OperateSkill( INT nLogicCount, INT nSkillId, INT nTargetId, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir );


	/** 压入一条指令 */
	virtual BOOL PushCommand( const SCommand_AI *pCmd );

	BOOL IsMoving(VOID) const												{ return GetMySelfAI() == MYSELF_AI_MOVE; }

	BOOL IsFollow(VOID) const												{ return GetMySelfAI() == MYSELF_AI_FOLLOW; }

	/** 当前AI逻辑状态 */
	ENUM_MYSELF_AI GetMySelfAI(VOID) const									{ return m_eMySelfAI; }

protected:

	VOID				SetForbidTime(UINT nForbidTime);

protected:

	/// 保存压进来的指令， 根据禁止操作时间来判断何时执行
	SCommand_AI			m_SaveCommand;
	/// 保存某一技能
	//  保存在某一技能执行过程中， 插入一次施放的技能， 然后再转到前面的技能继续施放
	SCommand_AI			m_SaveOldSkillCommand;

	/// 禁止操作的时间
	UINT				m_uForbidTime;
	/// 禁止操作的起始时间
	UINT				m_uForbidStartTime;

	/// 点选的攻击的状态
	BOOL				m_bAttkState;

protected:

	/** 
		执行AI指令 
	*/
	virtual RC_RESULT	OnCommand( const SCommand_AI *pCmd );

protected:

	RC_RESULT			AI_MoveTo( FLOAT fDestX, FLOAT fDestZ );
	RC_RESULT			AI_UseSkill( INT idSkill, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir );

public:

/** 跳跃相关
 */

	BOOL				IsCanJump( VOID );
	RC_RESULT			Jump( VOID );

	INT					Find_Platform( INT idAbility );
	RC_RESULT			ComposeItem( INT idPrescription );

public:

	/** 
		是否限制了某个操作的指令 
	*/
	BOOL IsLimitCmd( const SCommand_AI *pCmd );

protected:
	
/** 进入某逻辑状态
 */

	RC_RESULT Enter_Idle( VOID );
	RC_RESULT Enter_Move( FLOAT fDestX, FLOAT fDestZ );
	RC_RESULT Enter_UseSkill( INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir );
	RC_RESULT Enter_ActiveTripperObj( INT idItemBox );
	RC_RESULT Enter_DefaultEvent( INT idTargetObj );
	//RC_RESULT Enter_Dead( VOID );
	BOOL Enter_Follow( ObjID_t idTargetObj );

	RC_RESULT Enter_OperateSkill( INT nLogicCount, INT nSkillId, INT nTargetId, FLOAT fTargetPosX, FLOAT fTargetPosZ, FLOAT fDir );

protected:
	
/** 某逻辑状态中的Tick桢
 */

	BOOL Tick_Idle( VOID );
	BOOL Tick_Move( VOID );
	BOOL Tick_UseSkill( VOID );
	BOOL Tick_ActiveTripperObj( VOID );
	BOOL Tick_DefaultEvent( VOID );
	//BOOL Tick_Dead( VOID );
	BOOL Tick_Follow( VOID );

	BOOL Tick_OperateSkill( VOID );

protected:

	virtual VOID OnAIStopped( VOID );
	VOID StopFollow( VOID );
	fVector3 GetTargetPos(fVector3 MePos, fVector3 TargetPos, FLOAT fDist);

protected:

	VOID			SetMySelfAI(ENUM_MYSELF_AI eAI)							{ m_eMySelfAI = eAI; }

/** 向服务器发送命令
 */

	VOID			SendMoveMessage( const CPath *pPath, INT nLogicCount );
	VOID			SendUseSkillMessage( INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir  , BOOL autoRedo);
	VOID			SendActiveTripperObjMessage( INT idTripperObj );
	VOID			SendComposeItemMessage( INT idAbility, INT idPrescription,	ObjID_t guidPlatform );
	VOID			SendDefaultEventMessage( INT idTargetObj );

	/** 判断现在是否可以使用技能 */
	RC_RESULT		IsCanUseSkill( INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, BOOL& bNeedMove );
	BOOL			IsCanFollow();
	SCommand_AI		ConvertCommand( const SCommand_Object* pCmd );
	BOOL	IsSkillTargetDie(VOID);

private:

	ENUM_MYSELF_AI	m_eMySelfAI;

protected:

	struct SAIParam_Move
	{
		WORLD_POS	m_posTarget;
	};

	struct SAIParam_UseSkill
	{
		SkillID_t	m_idSkill;
		GUID_t		m_guidTarget;
		INT			m_idTarget;
		fVector3	m_posTarget;
		FLOAT		m_fDir;
		fVector3	m_posPrevTarget;

		const SAIParam_UseSkill& operator = ( const SAIParam_UseSkill& in )
		{
			if( this != &in )
			{
				m_idSkill			= in.m_idSkill;
				m_guidTarget		= in.m_guidTarget;
				m_idTarget			= in.m_idTarget;
				m_posTarget			= in.m_posTarget;
				m_fDir				= in.m_fDir;
				m_posPrevTarget		= in.m_posPrevTarget;
			}

			return *this;
		}

		VOID Reset( VOID )
		{
			m_idSkill			= INVALID_ID;
			m_guidTarget		= INVALID_ID;
			m_idTarget			= INVALID_ID;
			m_posTarget			= fVector3(0.0f, 0.0f, 0.0f);
			m_fDir				= 0.0f;
			m_posPrevTarget		= fVector3(0.0f, 0.0f, 0.0f);
		}

	};

	struct SAIParam_ActiveTripperObj
	{
		INT		m_idTripperObj;
	};

	struct SAIParam_DefaultEvent
	{
		INT			m_idTargetObj;
		fVector3	m_posPrevTarget;
	};

	struct SAIParam_Follow
	{
		const static INT		m_nMaxTickCount = 10;
		INT						m_nTickCount;
		ObjID_t					m_idTargetObj;
		fVector3				m_LastPos;
	};

	//test5
	struct SAIParam_OperateSkill
	{
		BOOL		m_bEnd;

		SAIParam_OperateSkill()
		{
			m_bEnd = FALSE;
		};
	};


	SAIParam_Move				m_paramAI_Move;
	SAIParam_UseSkill			m_paramAI_UseSkill;
	SAIParam_ActiveTripperObj	m_paramAI_ActiveTripperObj;
	SAIParam_DefaultEvent		m_paramAI_DefaultEvent;
	SAIParam_Follow				m_paramAI_Follow;

	SAIParam_OperateSkill		m_paramAI_OperateSkill;

	/// 保存不被打断的技能
//	SAIParam_UseSkill			m_paramAI_SaveSkill;


public:

	CAI_MySelf(CObject_Character* pCharObj);
	virtual ~CAI_MySelf( VOID );

public:

	/// 当前有正在使用的技能
	BOOL						m_bCurrSkill;

};	// class
