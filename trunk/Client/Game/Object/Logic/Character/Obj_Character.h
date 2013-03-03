
/** Obj_Character.h
 *	
 *	���ܣ�	��������߼�����
 *
 *	�޸ļ�¼��
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
 *	��ɫϵ�еĻ��࣬��̳г� PlayerOther,PlayerMySelf, PlayerNPC��
 */
class CObject_Character : public CObject_Dynamic
{
public:

	/// ��ɫobj������
	enum CHARACTER_TYPE
	{
		CT_INVALID	= -1,

		CT_PLAYERMYSELF,				// ����Լ�
		CT_PLAYEROTHER,					// �������
		CT_MONSTER						// ����/NPC
	};

	/// ��ǰ��ɫ��Object���߼�״̬
	enum ENUM_CHARACTER_LOGIC
	{
		CHARACTER_LOGIC_INVALID	= -1,	// ��Ч

		CHARACTER_LOGIC_IDLE,			// ����
		CHARACTER_LOGIC_ACTION,			// ����
		CHARACTER_LOGIC_MOVE,			// �ƶ�
		CHARACTER_LOGIC_SKILL_GATHER,	// �ۼ�
		CHARACTER_LOGIC_SKILL_LEAD,		// ����
		CHARACTER_LOGIC_SKILL_PREP_SEND,// ׼������		//test2
		CHARACTER_LOGIC_SKILL_SEND,		// ����
		CHARACTER_LOGIC_ABILITY_ACTION,	// �����
		CHARACTER_LOGIC_DEAD,			// ����
		CHARACTER_LOGIC_STALL,			// ��̯

		CHARACTER_LOGIC_NUMBERS
	};

	/// �����������
	enum TERRAIN_TYPE
	{
		GENERAL_TER = 1,				// ��ͨ����
		GRASS_TER	= 2,				// �ݵ�
		GROUND_TER	= 4,				// ����
		SNOW_TER	= 8,				// ѩ��
		WATER_TER	= 16,				// ˮ
		ROCK_TER	= 32,				// ʯͷ
	};

	/// �߼�������
	enum LOGIC_TYPE
	{
		LOGIC_BASE,						// �����߼�
		LOGIC_ACTION,					// ��Ϊ�߼�
	};


public:

/** ================ �����ӿ�
 */

	/** 
	 *	���ؽ�ɫ���� 
	 */
	virtual CHARACTER_TYPE	GetCharacterType(VOID) const					{ return CT_INVALID; }
	
	/** ��ȡͷ����Ϣ�� 
	 */
	tCreatureBoard*			GetInfoBoard(void)								{ return m_pInfoBoard; }

	GUID_t	GetGUID(VOID) ;
	
	/** �Ƿ��ܹ�����Ϊ��Ŀ��ѡ�� 
	 */
	virtual BOOL			CanbeSelect(VOID) const							{ return true; }
	VOID					MouseHoverState( BOOL bState ) const;
	
	//
	virtual UINT			GetIdleInterval(VOID) const						{ return UINT_MAX; }
	
	UINT					CalcIdleRandInterval(VOID);

	BOOL					IsDie(VOID) const;

	// 20100415 AddCodeBegin
	NPC_AUTOPATH*			GetIsAutoNPCPath()const		{ return (NPC_AUTOPATH*)&m_ObjAutoPath; } // ��ȡNPC�Զ�Ѱ·���ܵ��������
	// 20100415 AddCodeEnd  

	// 20100415 AddCodeBegin
	VOID					SetIsAutoNPCPath( INT nSceneID, STRING strNPCName, BOOL isAutoPath ); // ����NPC�Զ�Ѱ·���ܵ��������
	// 20100415 AddCodeEnd

	// 20100416 AddCodeBegin
	VOID					QuestOpenNPCDlg();	 // �����NPC�Ի�����
	// 20100416 AddCodeEnd  

public:

/** ================ ��ɫ�߼��������
 */

	/** �õ��߼����� 
	 */
	CCharacterData*			GetCharacterData(VOID)							{ return m_pCharacterData; }
	const CCharacterData*	GetConstCharacterData(VOID) const				{ return m_pCharacterData; }

	/** ��ȡ������ɫ������� 
	 */
	STRING					GetNameWithColor(void);

protected:

	friend class CCharacterData;
	friend class CAI_MySelf;

	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_ModelID(VOID);
	virtual VOID			OnDataChanged_MountID( VOID );
	virtual VOID			OnDataChanged_Mount( VOID );        // ����ӵ�е�������� [7/14/2010 �¾���]
	virtual VOID			OnDataChanged_Name(VOID);
	virtual VOID			OnDataChanged_CurTitles(INT nType);
	virtual VOID			OnDataChanged_MoveSpeed(VOID);
	virtual VOID			OnDataChanged_StallName(VOID);
	virtual VOID			OnDataChanged_IsInStall(VOID);
	virtual VOID			OnDataChanged_CountryFlag(VOID);
	virtual VOID			OnDataChanged_TeamLeaderFlag(VOID);	// ���¶ӳ����
	virtual VOID			OnDataChanged_TeamFlag(VOID);		// ������ӱ��
	virtual VOID			OnDataChanged_Die(VOID);
	virtual VOID			OnDataChanged_StealthLevel(VOID);
	virtual VOID			OnDataChanged_FightState(VOID);
	virtual VOID			OnDataChanged_Sit(VOID);
	virtual VOID			OnDataChanged_Level(VOID);			// ���µȼ�
	virtual VOID			OnDataChanged_Profession(VOID);
	virtual VOID			OnDataChanged_HP(VOID);				// ��������ֵ
	virtual VOID			OnDataChanged_Penuma(VOID);			// ����Ԫ��
	virtual VOID			OnDataChanged_BusObjID(VOID);		//Bus

	/// ��ɫ�߼�����
	CCharacterData*			m_pCharacterData;


public:

/**  ================ ����
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
	 *	�����������ͻ�ȡ��Ӧ�� �ַ���
	 */
	LPCSTR					GetWeaponName( ENUM_WEAPON_TYPE wType );

	/**
	 *	���������ϵ���Ч
	*/
	VOID					ChangerWeaponEffect( tEntityNode::eWEAPATTR ePart, LPCTSTR szEffectName, LPCTSTR nLocatorName, UINT color );

protected:

	/** �������� 
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

	/// ��ǰ��������
	ENUM_WEAPON_TYPE		m_theLWeaponType;
	ENUM_WEAPON_TYPE		m_theRWeaponType;


public:

/** ================ ��ɫ����Ч���߼����
 */

	const CImpactEffectMap*	GetImpactEffectMap( VOID ) const				{ return &m_mapImpactEffect; }
	VOID					RemoveAllImpact( VOID );

protected:

	VOID					UpdateBuffEffect(VOID);

	VOID					TickBuffElapseCount();	// ÿ�������buff��ʣ��ʱ��		20100531 BLL

	BOOL					InsertImpact( UINT nSN, INT idImpact, INT nTimeCount, ObjID_t nCreatorID, INT nNumOflayer,  BOOL bShowEnableEffect );

	VOID					RemoveImpact( UINT nSN );
	VOID					UpdatePetFrame();

	VOID					Tick_UpdateEffect(VOID);
	VOID					UpdateEffectTargetPos(const SImpactEffect *pImpactEffect);

	VOID					SetElement_Title(LPCTSTR szTitle, INT nType);

protected:

	/// ����Ч������������Ч, ʵʱ������
	CImpactEffectMap		m_mapImpactEffect;
	// �ӳٴ�����
	CImpactEffectMap		m_mapImpactDelay;


	BOOL					m_bHaveHitAnimation;
	
public:

/** ================ ��Ծ���
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

/** ================ ָ�����
 */

	/** ѹ��һ��ָ�� 
	 */
	virtual BOOL			PushCommand( const SCommand_Object *pCmd );

protected:

	/** ִ�п����˵�ģ��ָ�ֻ����AI���ã������ط����ã��������ջ����ָ��� 
	 */
	RC_RESULT				DoSimulantCommand(const SCommand_Object *pCmd);

	virtual RC_RESULT		DoCommand( const SCommand_Object *pCmd );
	virtual RC_RESULT		OnCommand( const SCommand_Object *pCmd );

	/** �������ָ�� 
	 */
	virtual BOOL				ProcessLogicCommand( VOID );

	/** ������һ���߼�
     */
	BOOL						DoNextLogicCommand( LOGIC_TYPE nLogicTag );

	/** �����߼�
     */
	BOOL						DoLogicCommand( CObjectCommand_Logic *pLogicCmd );

	VOID						SetLogicSpeed(FLOAT fSpeed);
	FLOAT						GetLogicSpeed(VOID) const						{ return m_fLogic_Speed; }

	VOID						OnLogicSpeedChanged(VOID);

protected:

	/** �ж��б��Ƿ�Ϊ��
	 */
	BOOL						IsBaseLogicCommandListEmpty( VOID );
	BOOL						IsActionLogicCommandListEmpty( VOID );

	/** �����߼�����
	 */
	BOOL						PushBaseLogicCommand( CObjectCommand_Logic *pCmd );
	BOOL						PushActionLogicCommand( CObjectCommand_Logic *pCmd );
	
	/** ��ȡ��һ���߼�����
	 */
	const CObjectCommand_Logic*	GetNextLogicCommand( LOGIC_TYPE nLogicTag ) const;
	CObjectCommand_Logic*		NextLogicCommand( LOGIC_TYPE nLogicTag );

	/** ����ָ�����߼� 
	 */
	CObjectCommand_Logic*		FindBaseLogicCommand( INT nLogicCount );
	CObjectCommand_Logic*		FindActionLogicCommand( INT nLogicCount );

	/** ����б�
 	 */
	virtual BOOL				CleanupLogicCommandList( LOGIC_TYPE nLogicTag );


	/**
	 *	�����ͷŵ�Ԥ������
	 */
	BOOL						PrepMagicSend( const SCommand_Object *pCmd );

protected:

	/// �߼������б�
	typedef std::list<CObjectCommand_Logic*>	CObjectLogicCommandList;
	CObjectLogicCommandList						m_listBaseLogicCommand;
	CObjectLogicCommandList						m_listActionLogicCommand;
	
	/// ��ǰ��Ҿִ�е��ٶ�
	FLOAT										m_fLogic_Speed;					

public:

/** ================ ��ɫ��Ϊ�߼����
 */

	/** �õ�AI 
	 */
	CAI_Base*				CharacterLogic_GetAI(VOID)					{ return m_pTheAI; }

	/** �õ���ǰ���߼�״̬ 
	 */
	ENUM_CHARACTER_LOGIC	CharacterLogic_Get( LOGIC_TYPE nLogicTag ) const;

	/** ��ǰ�߼��Ƿ�ֹͣ 
	 */
	BOOL					CharacterLogic_IsStopped( LOGIC_TYPE nLogicTag ) const;

	/** �ж��߼��Ƿ�׼����
	 */
//	virtual BOOL			IsLogicReady( INT nLogicCount ) const;
	virtual BOOL			IsBaseLogicReady( INT nLogicCount ) const;
	virtual BOOL			IsActionLogicReady( INT nLogicCount ) const;

	INT						GetLastModifyPosLogicCount(VOID) const		{ return m_nLastModifyPosLogicCount; }

	BOOL					IsIdle(VOID) const							{ return (CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_IDLE)?(TRUE):(FALSE); }

	BOOL					IsMoving(VOID) const						{ return (CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_MOVE && !CharacterLogic_IsStopped(LOGIC_BASE)) ? (TRUE):(FALSE); }

	BOOL					IsFightState(VOID) const						{ return m_uFightStateTime > 0; }
protected:

	/** ������ǰ���߼�״̬
	 */
	VOID					CharacterLogic_Set(ENUM_CHARACTER_LOGIC eLogic, LOGIC_TYPE nLogicTag );

	/** �����߼�����ֹͣ��Ӧ���߼�����
	 */
	VOID					CharacterLogic_Stop( BOOL bFinished, LOGIC_TYPE nLogicTag );

	/** �߼�ֹͣ�¼����κ��߼����κ�����ֹͣ���õ���(��������ĵ�������������һ�߼�֮ǰ)
	 *	bFinished	:	�Ƿ�Ϊ�ɹ�ִ�����
	 */
	virtual VOID			CharacterLogic_OnStopped( BOOL bFinished, LOGIC_TYPE nLogicTag );

	VOID					SetLastModifyPosLogicCount(INT nLogicCount)			{ m_nLastModifyPosLogicCount = nLogicCount; }


/** ================ ����ĳһ���߼�״̬
 */

	// ����
	BOOL					Start_Idle(VOID);
	// ����
	BOOL					Start_Action(CObjectCommand_Logic *pLogicCommand);
	// �ƶ�
	BOOL					Start_Move(CObjectCommand_Logic *pLogicCommand);
	// ��������
	BOOL					Start_MagicCharge(CObjectCommand_Logic *pLogicCommand);
	// ��������
	BOOL					Start_MagicChannel(CObjectCommand_Logic *pLogicCommand);
	//test2
	// ׼������
	BOOL					Start_MagicPrepSend(CObjectCommand_Logic *pLogicCommand);
	// ��������
	BOOL					Start_MagicSend(CObjectCommand_Logic *pLogicCommand);
	// ����
	BOOL					Start_Dead( BOOL bPlayDieAni );
	// �����
	BOOL					Start_Ability(CObjectCommand_Logic *pLogicCommand);
	// ��̯
	BOOL					Start_Stall(BOOL bPlayAni);

protected:

/** ================ ��һ���߼�״̬�µ��߼�Tick
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

	/** ������ǰʹ�õ��߼�����
	 */
	CObjectCommand_Logic*	GetBaseLogicCommand(VOID);
	VOID					SetBaseLogicCommand( CObjectCommand_Logic *pLogicCommand );
	CObjectCommand_Logic*	GetActionLogicCommand(VOID);
	VOID					SetActionLogicCommand( CObjectCommand_Logic *pLogicCommand );

	VOID					CheckMoveStop(VOID);

protected:

/** ================ ��ĳ���߼�״̬�µĲ���
 */



	struct SLogicParam_Idle
	{
		UINT		m_uIdleInterval;				/// �������ʱ��������һ�����ж���
		UINT		m_uLastIdleMotionTime;			/// ��һ�������ж�����ʱ��
		UINT		m_uStartTime;					/// �����߼�����ʼʱ��
	};

	struct SLogicParam_Move
	{
		INT			m_nCurrentNodeIndex;			/// ��ǰ�ڵ������ֵ
		WORLD_POS	m_posSaveStart;					/// ���浱ǰ����ʼ��
		WORLD_POS	m_posSaveTarget;				/// ���浱ǰ��Ŀ���
	};

	struct SLogicParam_MagicCharge
	{
		INT			m_nSkillID;
		UINT		m_uCurrentTime;					/// ��ǰ����ʱ��
	};

	struct SLogicParam_MagicChannel
	{
		INT			m_nSkillID;
		UINT		m_uCurrentTime;					/// ��ǰ����ʱ��
	};

	struct SLogicParam_MagicSend
	{
		INT			m_nSaveMagicID;					/// ��ǰ����ID
		BOOL		m_bBeAttackEffectShowed;		/// ��˴ι����������ı������ߵı����Ƿ��Ѿ�����
		BOOL		m_bCanBreak;					/// �Ƿ���Խ������߼���
		INT			m_nActionIndex;					/// Ϊ���ö�����˳�򲥷�
		INT			m_nActionCount;					/// ��������
		BOOL		m_bDoNextAction_Concatenation;	/// �����ܵ���ʽ����ΪSKILL_ACTION_TYPE_CONCATENATIONʱ����
		UINT		m_uAnimationTime;				/// ���Ѿ����ŵ�ʱ��
		UINT		m_uAnimationEndElapseTime;		/// ����ֹͣ��ʱ����
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
		fVector3	m_DeadPos;		// ����λ��
		fVector3	m_CurrPos;		// ��ǰλ��
		fVector3	m_TarPos;		// Ŀ��λ��
		fVector3	m_TarDir;		// Ŀ�귽��
		BOOL		m_bDeadFlyBegin;// ��ʶ���ɶ�����ʼ
		BOOL		m_bDeadFlyEnd;	// ��ʶ���ɶ�������
		BOOL		m_bDeadBegin;	// ��ʶ����������ʼ
		BOOL		m_bDeadEnd;		// ��ʶ������������
		INT			m_nCadaverAction;// ʬ�嶯���� Ҫ�����������ƥ��
		BOOL		m_bDeadIsEnd;		// ��ʶ������ �����������
	};



	/// ����ִ�е��߼�״̬
	ENUM_CHARACTER_LOGIC		m_nCharacterBaseLogic;
	ENUM_CHARACTER_LOGIC		m_nCharacterActionLogic;

	/// �ı䶯��ʱ�õ��ļ���߼�״̬
	LOGIC_TYPE					m_eChangeAnimLogicType;

	/// �߼��Ƿ�ֹͣ
	BOOL						m_bCharacterBaseLogicStopped;
	BOOL						m_bCharacterActionLogicStopped;

	/// AI����
	CAI_Base*					m_pTheAI;

	INT							m_nLastServerLogicCount;		/// ���õ��ķ������˵��߼�����
	INT							m_uLastServerTime;				/// ���õ��ķ�������ʱ��
	INT							m_nLastModifyPosLogicCount;		/// ���ִ�еĸı�λ�õ�ָ��

	/// ��ǰ�߼�����
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


	// �⼸����Ҫ�Ǹ������õģ� ����ֻ���ƶ�����������״̬

	INT							m_nMoveAnimId;	/// �ƶ��Ķ���
	INT							m_nIdleAnimId;	/// ���еĶ���

	BOOL						m_bMoveState;	/// �Ƿ��ƶ�״̬


	/// ��¼�����߼� �� ��Ϊ�߼��Ķ���
	INT							m_nBaseAnimId;		/// 
	INT							m_nActionAnimId;	/// 

	/// �Ƿ񱻻���
	BOOL						m_bBeatFly;
	/// ���ɾ���
	FLOAT						m_fBeatFlyDist;
	/// �Ƿ��Ǹ���״̬�������ǣ�
	BOOL						m_bTail;

	// ͷ���˺���Ϣ�Ĺ���ʱ����
	UINT						m_uDisplayInfoTimer;

	// 20100415 AddCodeBegin
	static NPC_AUTOPATH		m_ObjAutoPath;
	// 20100415 AddCodeEnd
public:

	BOOL			m_UseSkillTag;


public:

/** ================ �˺��Ĺ�������
 */

	BOOL					IsMySelf(INT ObjID) const;
	BOOL					IsMyPet(INT ObjID) const;
	VOID					ShowLogicEvent( INT nServerObjID, INT nLogicCount, BOOL bShowAll = FALSE );

	// �ͷż���ʱ����ɫͷ��ð��������		20100531 BLL
	VOID					DisplaySkillInfo( INT nSkillID );

protected:

	VOID					AddLogicEvent(const _LOGIC_EVENT *pLogicEvent);
	VOID					RemoveLogicEvent( INT nLogicCount );
	VOID					RemoveAllLogicEvent( VOID );

	VOID					Update_LogicEvent( VOID );
	
	/** ��˴ι����������ı������ߵı����Ƿ��Ѿ�����
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

	/// �����߼��¼�����������ִ�еģ�
	CLogicEventList			m_listLogicEvent;

	UINT					m_uTime_LogicEventListTick;


public:
	// �����Ч,���⿪�ŵĽӿڣ�UI���ģ�ͻ��õ�
	static VOID CreateMountEffect(tEntityNode* pMountInterface, INT nMountID, const SDATA_PET* pPetData=NULL);
	VOID					OnBulletHit();
protected:

/** ================ ��ɫ�������
 */

	/** ����Ⱦ�㴴����ɫ��Ⱦָ�� 
	 */
	virtual VOID			CreateRenderInterface(VOID);

public:
	virtual VOID			ReleaseRenderInterface(VOID);

	/** ����Ⱦ�㴴��������Ⱦָ�� 
	 */
protected:
	virtual tEntityNode*	CreateMountRenderInterface(INT nMountID);
	virtual VOID			ReleaseMountRenderInterface( tEntityNode* mountInterface );
	/**
	 *	����װ������
	 */
	virtual VOID			OnDataChanged_Equip( HUMAN_EQUIP point )		{ };
	virtual VOID			OnDataChanged_MountEquip( HORSE_EQUIP point );

	/** ѡ�л���С 
	 */
	virtual FLOAT			GetProjtexRange(VOID)							{ return 200.0f; }
	/** ��Ӱ��С 
	 */
	virtual FLOAT			GetShadowRange(VOID)							{ return 100.0f; }


	/**
	 *	���˻�����������Ѫ������������˿��ܲ�ֹһ�εĵ��ã�
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
	 *	�������������
	 */
	VOID					UpdateMountEquip( HORSE_EQUIP point );	/* ��������β���ö�� */
	VOID					UnMountEquip( HORSE_EQUIP point );

protected:

	VOID					StopCurrentAction();
	/** �ı䶯��
	 */
	BOOL					ChangeAction( INT nSetID, FLOAT fSpeed, BOOL bLoop, ENUM_ACTION_TYPE ePart=ACTION_BODY, FLOAT fFuseParam=-0.03 );
	/** �ı䶯���ٶ�
	 */
	VOID					ChangeActionSpeed(FLOAT fSpeed);

	VOID					PlayAnimation(INT nSetID, FLOAT fStartTime, FLOAT fWeight);

	FLOAT				GetDefaultFUseParam(VOID)
	{
		return m_fDefaultFuse;
	}

	/** ����������־
	 */
	BOOL					IsBaseAnimationEnd(VOID) const					{ return m_bBaseAnimationEnd; }
	VOID					SetBaseAnimationEnd(BOOL bSet)					{ m_bBaseAnimationEnd = bSet; }
	BOOL					IsActionAnimationEnd(VOID) const				{ return m_bActionAnimationEnd; }
	VOID					SetActionAnimationEnd(BOOL bSet)				{ m_bActionAnimationEnd = bSet; }

	VOID					UpdateTransparent(VOID);
	VOID					SetTransparent(FLOAT fTransparency, FLOAT fTime = 1.f);

	/** ˢ�µ�ǰ���� 
	 */
	VOID					FreshAnimation( ENUM_ACTION_TYPE ePart=ACTION_BODY, LOGIC_TYPE eLogic=LOGIC_BASE );

protected:

	/** ����Դ���л�����ﶯ���ļ�
	 *	nActionSetID	:	ENUM_BASE_ACTION or ����
	 *	nWeaponType		:	ENUM_WEAPON_TYPE
	 */
	virtual LPCSTR			GetCharActionNameByActionSetID(INT nActionSetID, INT nWeaponType, BOOL *pbHideWeapon = NULL, INT *pnAppointedWeaponID = NULL);
	virtual LPCSTR			GetMountActionNameByActionSetID(INT nActionSetID, INT nWeaponType);
	/**	
	 *	������������
	 *
	 *	nActionSetID	����id
	 *	nWeaponType		��������
	 */
	LPCSTR					GetWeaponActionNameByActionSetID( INT nActionSetID, INT nWeaponType );

	virtual INT				GetCharModelID( VOID ) const;


	virtual INT				CalcCharModelID( VOID ) const;

	VOID					SetFightState(BOOL bSet);
protected:

	VOID					UpdateCharModel(VOID);
	VOID					UpdateMountModel(VOID);
	VOID					UpdateMountList(VOID);  // ����ӵ�е���  [7/14/2010 �¾���]
	virtual VOID			UpdateCharRace(VOID)							{ }
	VOID					UpdateCharActionSetFile(VOID);
	VOID					UpdateMountActionSetFile(VOID);
	VOID					UpdateWeaponActionSetFile( LPCTSTR szWeaponAnimName );

	virtual VOID			OnHideWeapon( INT nAppointedWeaponID, ENUM_WEAPON_LOCATOR_TYPE loc )			{ }
	virtual VOID			OnShowWeapon( VOID )								{ }

	BOOL					IsHideWeapon(VOID) const						{ return m_bHideWeapon; }


protected:

	/// �����ļ���
	const DBC::DBCFile		*m_pCharActionSetFile;
	const DBC::DBCFile		*m_pMountActionSetFile;
	const DBC::DBCFile*		m_pWeaponActionSetFile;

	// ��������
	struct MountData
	{
		tEntityNode*	pMountRenderInterface;	/// �������Ⱦ��ӿ�
		INT				nMountID;				/// ����ID����Ӧchar_mount.tab���� 
		INT				nMountModelID;			/// ��������ģ�Ͷ�Ӧ���е�ID				
		FLOAT			fMountAddHeight;		/// ���������ͷ����Ϣ��߶�ƫ��

		MountData()
		{
			pMountRenderInterface	= NULL;
			nMountID				= INVALID_ID;
			nMountModelID			= INVALID_ID;
			fMountAddHeight			= 0.0f;
		}
	};
	// map< ����ID���������� >
	typedef std::map<INT, MountData> MapMountData;

	/// ӵ�е�����������Ϣ�� �����ʵ����һֱ�����ţ�����ɫ�ͷ�ʱһ���ͷš�
	MapMountData			m_mapMyMount;

	// ��ɫ��ǰ���ŵ�����,һ���м�ָ�룬��Ҫ�����������ͷ�
	tEntityNode*			m_pMountRenderInterface;

	// ��ǰ���������ͷ����Ϣ��߶�ƫ��
	FLOAT					m_fMountAddHeight;

	/// ����ģ�Ͷ�Ӧ���е�ID
	INT						m_nCharModelID;
	
	BOOL					m_bHideWeapon;

	FLOAT				m_fDefaultFuse;

	/// ��ʾ�����Ƿ������
	BOOL					m_bBaseAnimationEnd;
	BOOL					m_bActionAnimationEnd;

	/// ����״̬ʱ��
	UINT					m_uFightStateTime;


	/// �������
	BOOL					m_bRightWeapon;
	BOOL					m_bLeftWeapon;

protected:

	/** ��ǰ����Ⱦ����ִ�еĶ����������¼�֪ͨ���� */
	static bool			_OnAnimationEnd(LPCTSTR szAnimationName, UINT dwParam, INT nPart, INT nLogic);
	virtual BOOL		OnAnimationEnd( LPCTSTR szAnimationName, ENUM_ACTION_TYPE ePart=ACTION_BODY, LOGIC_TYPE eLogic=LOGIC_BASE );

	/** ��ǰ����Ⱦ����ִ�еĶ�������ֹͣ���¼�֪ͨ���� */
	static bool			_OnAnimationCanBreak(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationCanBreak(LPCTSTR szAnimationName);

	/** ��ǰ����Ⱦ����ִ�еĶ����������¼�֪ͨ���� */
	static bool			_OnAnimationHit(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationHit(LPCTSTR szAnimationName);

	/** ��ǰ����Ⱦ����ִ�еĶ�����Ļ�𶯵��¼�֪ͨ���� */
	static bool			_OnAnimationShakeEvent(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationShakeEvent(LPCTSTR szAnimationName);

	/** ֪ͨһ�������¼����� */
	virtual VOID		NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam = NULL);
protected:
/** ================ ����� 
*/

	// ȡ��ӵ�е���
	MountData*				GetMyMount( INT nMountID );
	// ��������
	VOID					CreateNewMount(INT nMountID);
	// ������Ŀɼ�
	inline VOID				SetMountVisible(tEntityNode* mountInterface, BOOL bShow);
	// ����
	VOID					Mounting(tEntityNode* mountInterface);
	// ����
	VOID					Dismount();


public:

/** ================ �ƶ�·����� 
 */

	/** ����Ŀ��㴴������·��
	 */
	virtual VOID				OnMoveStop( VOID );

public:

/** ================ ������� 
 */

	virtual INT					CannotBeAttack( VOID ) const				{ return 0; }
	virtual const _CAMP_DATA*	GetCampData(VOID) const;
	virtual ObjID_t				GetOwnerID(VOID) const;
	// ��ȡ�ж���ʱ��ļ��ܵ�ID (�������ܻ���������)
	INT							GetActivatedSkillID();


protected:

/** ================ ͷ����Ϣ����� 
 */

	/// UI�ӿ�
	tCreatureBoard*			m_pInfoBoard;
	/// λ��
	fVector2				m_fvInfoBoardPos;


/** ================ ��ɫ��Ч��� 
 */

	/// �Ų�����
	tSoundSource*			m_pWlakSound;
	/// ���������
	tSoundSource*			m_pLifeAbilitySound;


	/// ������������
	INT						m_SoundId[MAX_SOUND_NUMBER];

private:	
	
// �������ڲ�ͬ���������

	/// �Ƿ񲥷�״̬
	bool					mPlayState;
	/// ������һ�εĵ�������
	int						mTerrainType;


public:

/** ================ ��ɫ���춯����� 
 */

	/** ���ö����б� 
	 */
	VOID					SetChatMoodAction(STRING& strOrder);	

protected:

	/** ���������ʣ��Ķ���
	 */
	VOID					ClearAllChatMoodAction();				
	/** ÿ�δӶ����ﵯ��һ���������в��ţ���ɫ�����߼����봦��idle״̬���� 
	 */
	VOID					PlayChatMoodAction();					
	/** �Ƿ������춯��
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

/** ================ ������¼� 
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

	// �����б�����
	struct missionData
	{
		INT nSceneID;		// �������ͼID
	};

//	std::vector< missionData > m_
	


public:

/** ================ ���ļ̳� 
 */

	CObject_Character();
	virtual ~CObject_Character();

	/** ��ɫ��ʼ�� 
	 */
	virtual VOID			Initial(VOID* pParam);
	/** �ͷ�������ӵ�е���Դ 
	 */
	virtual VOID			Release(VOID);
	/** �߼����� 
	 */
	virtual VOID			Tick(VOID);
	
	/** ����Ƿ��ٿɼ� 
	 */
	//virtual VOID			Tick_CheckAlive(VOID);

	/** ��Ϣ��Tick 
	 */
	virtual VOID			Tick_UpdateInfoBoard(VOID);

	// ��̯�󣬻ָ���̯ǰ��ͷ����Ϣ��״̬	20100527 BLL
	virtual VOID			RestoreInfoBoard(VOID);

	virtual VOID			SetPosition(const fVector3& fvPosition);
	virtual VOID			SetRotation(const fVector3& fvRotation);
	virtual VOID			Enable(UINT dwFlag);
	virtual VOID			Disalbe(UINT dwFlag);

	virtual VOID			SetScale(FLOAT fScale);

	/**
	 *	����ͷ���ƺ���Ϣ��״̬
	 */
	virtual VOID			SetBoardTitleState( BOOL bShow, INT nType );
	/**
	 *	����ͷ��״̬��Ϣ��״̬
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
