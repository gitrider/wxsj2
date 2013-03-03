
/** GMAI_MySelf.cpp
 *	
 *	功能： 处理玩家的模拟操作， 即先在客户端进行处理
 *	
 *	修改记录：
 *			
 *			
 */

#include "StdAfx.h"

#include "NetWork\NetManager.h"
#include "GIUtil.h"
#include "GITimeSystem.h"
#include "Event\GMEventSystem.h"
#include "Action\GMActionSystem.h"
#include "Procedure\GameProcedure.h"
#include "Procedure\GamePro_Main.h"
#include "DataPool\DetailAttrib_Player.h"
#include "DataPool\GMUIDataPool.h"
#include "..\Obj_PlayerOther.h"
#include "..\..\..\Manager\ObjectManager.h"
#include "World\WorldManager.h"
#include "Cursor\CursorMng.h"

#include "SkillDataMgr.h"
#include "CGCharJump.h"
#include "CGCharMove.h"
#include "CGCharIdle.h"
#include "CGCharUseSkill.h"
#include "CGOpenItemBox.h"
#include "CGUseAbility.h"

#include "GMAICommandDef.h"
#include "..\..\..\ObjectCommandDef.h"

#include "..\..\TripperObj\GMTripperObj.h"
#include "..\..\TripperObj\GMTripperObj_Platform.h"
#include "..\..\TripperObj\GMTripperObj_Resource.h"
#include "GIGameInterface.h"
#include "GICursorSystem.h"

#include "CGCharDefaultEvent.h"

#include "DBC\GMDataBase.h"
#include "Global.h"
#include "..\Obj_PlayerMySelf.h"
#include "DataPool\GMDP_CharacterData.h"
#include "DataPool\GMDataPool.h"

#include "Interface/GMInterface_Script_Talk.h"

#include "GMAI_MySelf.h"
#include "Interface\GMGameInterface.h"

#include "GIVariable.h"




#define USE_SKILL_FORBID_TIME	(200)



CAI_MySelf::CAI_MySelf(CObject_Character* pCharObj) : CAI_Base(pCharObj)
{
	SetMySelfAI( MYSELF_AI_IDLE );

	m_SaveCommand.Cleanup();
	m_SaveOldSkillCommand.Cleanup();

	m_uForbidTime		= 0;
	m_uForbidStartTime	= 0;

	m_bCurrSkill		= FALSE;
	m_bAttkState		= FALSE;
}

CAI_MySelf::~CAI_MySelf( VOID )
{
}


VOID CAI_MySelf::Reset(VOID)
{
}

VOID CAI_MySelf::SetForbidTime(UINT nForbidTime)
{
	m_uForbidTime = nForbidTime;
	m_uForbidStartTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
}

BOOL CAI_MySelf::Tick( VOID )
{
	// 判断禁止时间
	if( m_uForbidTime > 0 )
	{
		if( CGameProcedure::s_pTimeSystem->GetTimeNow() - m_uForbidStartTime >= m_uForbidTime )
			m_uForbidTime = 0;
	}

	// 判断是否有未执行的命令
	if( m_SaveCommand.m_wID != AIC_NONE )
	{
		if( m_uForbidTime == 0 )
		{
			if( !IsLimitCmd( &m_SaveCommand ) )
			{
				RC_RESULT rcResult = OnCommand( &m_SaveCommand );
				if( RC_WAIT != rcResult )
					m_SaveCommand.m_wID = AIC_NONE;
			}
			else
				m_SaveCommand.m_wID = AIC_NONE;
		}
	}


/*
	// 当前技能施放完毕， 并且 存在着保存的旧技能
	if( !m_bCurrSkill && ( AIC_NONE != m_SaveOldSkillCommand .m_wID ) )
	{
		if( m_uForbidTime == 0 )
		{
			if( !IsLimitCmd( &m_SaveOldSkillCommand ) )
			{
				RC_RESULT rcResult = OnCommand( &m_SaveOldSkillCommand );
				if( RC_WAIT != rcResult )
					m_SaveOldSkillCommand.m_wID = AIC_NONE;
			}
			else
				m_SaveOldSkillCommand.m_wID = AIC_NONE;
		}
	}
*/


	BOOL bResult;
	switch( GetMySelfAI() )
	{
		case MYSELF_AI_IDLE:
			bResult = Tick_Idle();
			break;
		case MYSELF_AI_MOVE:
			bResult = Tick_Move();
			break;
		case MYSELF_AI_ACTIVE_TRIPPEROBJ:
			bResult = Tick_ActiveTripperObj();
			break;
		case MYSELF_AI_DEFAULT_EVENT:
			bResult = Tick_DefaultEvent();
			break;
		//case MYSELF_AI_DEAD:
		//	bResult = Tick_Dead();
		//	break;
		case MYSELF_AI_FOLLOW:
			bResult = Tick_Follow();
			break;

		//test5
		case MYSELF_AI_OPERATE_SKILL:
			bResult = Tick_OperateSkill();
			break;

		default:
			bResult = FALSE;
			break;
	}

	// 使用技能单独来处理
	if( m_bCurrSkill )
		bResult = Tick_UseSkill();

	return bResult;
}

BOOL CAI_MySelf::PushCommand_MoveTo(FLOAT fDestX, FLOAT fDestZ)
{
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_MOVE;
	cmdTemp.m_afParam[0]	= fDestX;
	cmdTemp.m_afParam[1]	= fDestZ;

	return PushCommand(&cmdTemp);
}

BOOL CAI_MySelf::PushCommand_Jump(VOID)
{
	if(RC_OK == Jump())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CAI_MySelf::PushCommand_UseSkill(INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir)
{
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= (UINT)idSkill;
	cmdTemp.m_adwParam[1]	= (UINT)idTargetObj;
	cmdTemp.m_afParam[2]	= fTargetX;
	cmdTemp.m_afParam[3]	= fTargetZ;
	cmdTemp.m_afParam[4]	= fDir;
	cmdTemp.m_auParam[5]	= guidTarget;

	return PushCommand(&cmdTemp);
}

BOOL CAI_MySelf::PushCommand_OperateSkill( INT nLogicCount, INT nSkillId, INT nTargetId, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )
{
	SCommand_AI cmdTemp;

	cmdTemp.m_wID			= AIC_OPERATE_SKILL;
	cmdTemp.m_adwParam[0]	= (UINT)nLogicCount;
	cmdTemp.m_adwParam[1]	= (UINT)nSkillId;
	cmdTemp.m_adwParam[2]	= (UINT)nTargetId;
	cmdTemp.m_afParam[2]	= fTargetX;
	cmdTemp.m_afParam[3]	= fTargetZ;
	cmdTemp.m_afParam[4]	= fDir;


	return PushCommand(&cmdTemp);
}



// 压入一条指令
BOOL CAI_MySelf::PushCommand( const SCommand_AI *pCmd )
{
	// 先清空，不管此次操作是否为空
	m_SaveCommand.m_wID = AIC_NONE;


/*
	m_SaveOldSkillCommand;

	// 当前有正在执行的技能 并 新的命令也是施放技能
	// 这种情况说明新的技能命令是插入的操作（根据上面函数的条件判断）
	if( m_bCurrSkill && ( AIC_USE_SKILL == pCmd->wID ) )
	{
		// 保存旧的技能
		m_SaveOldSkillCommand  = *pCmd;		// 临时：这句保存的是新的技能， 要保存旧的此处的逻辑需要修改
											// 要判断是否技能施放， 不是就可以清掉， 是就要保存， 直到下次调用时再判断是否清掉
	}
	else
	{
		// 清空
		m_SaveCommand.m_wID = AIC_NONE;
		m_SaveOldSkillCommand.m_wID = AIC_NONE;
	}
*/



	if( !IsLimitCmd( pCmd ) )
	{
		// 先将pCmd缓存起来
		m_SaveCommand = *pCmd;

		if( m_uForbidTime == 0 )
		{
			RC_RESULT rcResult = OnCommand(pCmd);
			
			if( rcResult != RC_WAIT )
			{
				// 如果执行成功则将缓存清空
				m_SaveCommand.m_wID = AIC_NONE;
			}
		}
	}

	return TRUE;
}

RC_RESULT CAI_MySelf::OnCommand( const SCommand_AI *pCmd )
{
	RC_RESULT rcResult;

	// 限制摆摊状态下不能进行的活动
	if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_You_Stalling");
		CEventSystem::GetMe()->PushEvent(GE_PERSONAL_MESSAGE, strTemp.c_str());		// "你正在摆摊……"

		return RC_OK;
	}

	switch( pCmd->m_wID )
	{
	// 移动
	case AIC_MOVE:
		{
			fVector2 fv2TargetPos;
			fv2TargetPos.x	= pCmd->m_afParam[0];
			fv2TargetPos.y	= pCmd->m_afParam[1];
			
			rcResult = Enter_Move( fv2TargetPos.x, fv2TargetPos.y );
		}
		break;

	// 使用技能
	case AIC_USE_SKILL:
		{
			UINT dwTargetID, dwSkillID;
			WORLD_POS posTarget;
			FLOAT fDir;
			GUID_t guidTarget;

			dwSkillID		= pCmd->m_adwParam[0];
			dwTargetID		= pCmd->m_adwParam[1];
			posTarget.m_fX	= pCmd->m_afParam[2];
			posTarget.m_fZ	= pCmd->m_afParam[3];
			fDir			= pCmd->m_afParam[4];
			guidTarget		= pCmd->m_auParam[5];

			rcResult = Enter_UseSkill( dwSkillID, guidTarget, dwTargetID, posTarget.m_fX, posTarget.m_fZ, fDir );
		}
		break;

	case AIC_TRIPPER_ACTIVE:
		{
			INT	idItemBox;
			idItemBox	= pCmd->m_adwParam[0];

			rcResult = Enter_ActiveTripperObj( idItemBox );
		}
		break;

	case AIC_COMPOSE_ITEM:
		{
			INT	idPrescription;
			idPrescription	= pCmd->m_adwParam[0];

			rcResult = ComposeItem( idPrescription );
		}
		break;

	case AIC_DEFAULT_EVENT:
		{
			UINT dwTargetID;
			dwTargetID		= pCmd->m_adwParam[0];

			rcResult = Enter_DefaultEvent( dwTargetID );
		}
		break;

	//case AIC_DIE:
	//	{
	//		rcResult = Enter_Dead( );
	//	}
	//	break;

	case AIC_JUMP:
		{
			rcResult = Jump( );
		}
		break;

	case AIC_FOLLOW:
		{
			ObjID_t idTargetObj;
			idTargetObj = pCmd->m_adwParam[0];
			BOOL bResult = Enter_Follow(idTargetObj);
			if ( bResult )
			{
				rcResult = RC_OK;
			}
			else
			{
				rcResult = RC_ERROR;
			}
		}
		break;

	//test5
	case AIC_OPERATE_SKILL:
		{

			UINT dwLogicCount, dwTargetID, dwSkillID;
			WORLD_POS posTarget;
			FLOAT fDir;

			dwLogicCount	= pCmd->m_adwParam[0];
			dwSkillID		= pCmd->m_adwParam[1];
			dwTargetID		= pCmd->m_adwParam[2];
			posTarget.m_fX	= pCmd->m_afParam[2];
			posTarget.m_fZ	= pCmd->m_afParam[3];
			fDir			= pCmd->m_afParam[4];

			rcResult = Enter_OperateSkill( dwLogicCount, dwSkillID, dwTargetID, posTarget.m_fX, posTarget.m_fZ, fDir );
		}
		break;

	default:
		rcResult = RC_SKIP;
		break;
	};

	return rcResult;
}



//================-================
// AI逻辑Enter
//================-================

RC_RESULT CAI_MySelf::Enter_Idle( VOID )
{
	CObject_Character *pCharacter = GetCharacter();

	SCommand_Object cmd;
	cmd.m_wID			= OC_IDLE;
	cmd.m_afParam[0]	= pCharacter->GetPosition().x;
	cmd.m_afParam[1]	= pCharacter->GetPosition().z;

	pCharacter->DoSimulantCommand(&cmd);
	//RC_RESULT rcResult = pCharacter->DoCommand(&cmd);
	//if ( rcResult != RC_OK )
	//	return RC_ERROR;

	if( MYSELF_AI_IDLE != GetMySelfAI() )
	{
		SetMySelfAI( MYSELF_AI_IDLE );
	}

	return RC_OK;
}

RC_RESULT CAI_MySelf::Jump( VOID )
{
	// 监测现在是否可以JUMP
	if( IsCanJump() )
	{
		CObject_Character *pCharacter = GetCharacter();
		BOOL bResult = pCharacter->Jump();
		if (!bResult)
			return RC_ERROR;

		// send jump msg to server.
		CGCharJump msg;
		msg.setObjID( pCharacter->GetServerID() );
		
		CNetManager::GetMe()->SendPacket( &msg );

		return RC_OK;
	}

	return RC_ERROR;
}

RC_RESULT CAI_MySelf::Enter_Move( FLOAT fDestX, FLOAT fDestZ )
{
	static UINT uSaveTime = 0;
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	UINT uSleepTime = 500;
	//if(CGameProcedure::s_pProcMain->IsAutoRun())
	//{
	//	uSleepTime = 500;
	//}
	if( uSaveTime + uSleepTime > uTimeNow )
	{
		return RC_WAIT;
	}

	//if ( GetMySelfAI() == MYSELF_AI_DEAD )
	//	return RC_ERROR;

	if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Stall_Can_Not_Move");
		CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );		// "摆摊时不能移动"

		return RC_ERROR;
	}
	
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();

	//616======* 单机
	if( CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 0 )
	{

	if( pCharData->IsLimitMove() )
		return RC_ERROR;

	if( pCharData->IsDie() )
		return RC_ERROR;

	}//


	RC_RESULT rcResult = AI_MoveTo( fDestX, fDestZ );
	if( rcResult != RC_OK )
		return rcResult;

	m_paramAI_Move.m_posTarget.m_fX	= fDestX;
	m_paramAI_Move.m_posTarget.m_fZ	= fDestZ;

	if( MYSELF_AI_MOVE != GetMySelfAI() )
	{
		SetMySelfAI( MYSELF_AI_MOVE );
	}

	uSaveTime = uTimeNow;


	// 存在自动点选攻击的状态， 停掉技能的判断
	if( m_bAttkState )
	{
		m_bCurrSkill = FALSE;
		m_bAttkState = FALSE;
	}


	return RC_OK;
}

RC_RESULT CAI_MySelf::Enter_UseSkill( INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )
{
	static UINT uSaveTime = 0;
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
//	UINT uSleepTime = 500;
	if( uSaveTime + USE_SKILL_FORBID_TIME > uTimeNow )
	{
		return RC_WAIT;
	}

	// 限制摆摊的时候人物不能使用技能
	if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Stall_Can_User_Skill");
		CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "摆摊时不能使用技能"
		
		return RC_ERROR;
	}


	BOOL bNeedMove = FALSE;
	// 判断能否使用技能
	RC_RESULT rcResult = IsCanUseSkill( idSkill, guidTarget, idTargetObj, fTargetX, fTargetZ, fDir, bNeedMove );
	if( rcResult != RC_OK )
		return rcResult;


	uSaveTime = uTimeNow;

	// 是自动打怪的技能
	const SSkillData* pSkillData = GetSkillDataMgr()->GetSkillData(idSkill);
	if (pSkillData && pSkillData->m_bAutoRedo && FALSE == pSkillData->m_nImmediately)
	{
		// 标识技能激活
		m_bCurrSkill = TRUE;
		return RC_OK;
	}
	// 不需要移动
	if (FALSE == bNeedMove)
	{
		// 就在攻击范围内
		SetForbidTime( USE_SKILL_FORBID_TIME );
		SendUseSkillMessage( idSkill, guidTarget, idTargetObj, fTargetX, fTargetZ, fDir ,pSkillData->m_bAutoRedo);

		return RC_OK;
	}
	else
	{
		// 标识技能激活, 其他技能也应该执行这种策略
		m_bCurrSkill = TRUE;
	}

	// 保存
//	m_paramAI_SaveSkill = m_paramAI_UseSkill;

	//uSaveTime = uTimeNow;

	//// 标识技能激活
	//m_bCurrSkill = TRUE;

	return RC_OK;
}

RC_RESULT CAI_MySelf::Enter_ActiveTripperObj( INT idTripperObj )
{
	static UINT uSaveTime = 0;
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	UINT uSleepTime = 500;
	if(uSaveTime + uSleepTime > uTimeNow)
	{
		return RC_WAIT;
	}

	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();

	// 已经死亡
	if( pCharData->IsDie() )
		return RC_ERROR;
	if( pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;

	// 判断目标是否存在
	fVector3 fvTarget;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( idTripperObj ));
	if( pTarget == NULL )
		return RC_ERROR;

	if( tObject::TOT_RESOURCE == pTarget->Tripper_GetType() ) 
	{
		if( !((CTripperObject_Resource*)(CTripperObject*)pTarget)->Resource_CanOperation() )
			return RC_ERROR;
	}

	fvTarget = pTarget->GetPosition();

	FLOAT fOperationDistance;
//	if ( fDistSq > MAX_OPEN_ITEMBOX_DIST_SQ )
	if(	tObject::TOT_RESOURCE == pTarget->Tripper_GetType() )
	{
		CTripperObject_Resource *pResource = (CTripperObject_Resource*)(CTripperObject*)pTarget;
		fOperationDistance = pResource->Resource_GetOperationDistance();
	}
	else
	{
		fOperationDistance = MAX_OPEN_ITEMBOX_DIST;
	}

	BOOL bCanDo = TRUE;
	if( fOperationDistance > 0.f )
	{
		FLOAT fOperationDistanceSq = fOperationDistance * fOperationDistance;
		FLOAT fDistSq = KLU_GetDistSq( fvTarget, pCharacter->GetPosition() );
		if(fOperationDistanceSq < fDistSq)
		{
			bCanDo = FALSE;
		}
	}

	if( bCanDo )
	{
		SendActiveTripperObjMessage( idTripperObj );
		//Enter_Idle();
		CGameProcedure::s_pGfxSystem->PushDebugString("CGCharTripperObj[%d]", idTripperObj);
	}
	else
	{
		//如果是钓鱼的话，就不用走到河里去了。
		//但是也需要距离合适，如果距离不够，也不能开始钓。
		if(	tObject::TOT_RESOURCE == pTarget->Tripper_GetType() && ((CTripperObject_Resource*)pTarget)->Resource_IsFish())
		{
			ADDTALKMSG("距离太远！");
			return RC_ERROR;
		}
		else
		{
			fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fOperationDistance);
			RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
			if ( rcResult != RC_OK )
				return rcResult;

			// 参数付值
			m_paramAI_ActiveTripperObj.m_idTripperObj	= idTripperObj;
			SetMySelfAI( MYSELF_AI_ACTIVE_TRIPPEROBJ );
			CGameProcedure::s_pGfxSystem->PushDebugString("CGCharTripperObj[%d]", idTripperObj);
		}
	}

	uSaveTime = uTimeNow;
	return RC_OK;
}

RC_RESULT CAI_MySelf::Enter_DefaultEvent( INT idTargetObj )
{
	static UINT uSaveTime = 0;
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	UINT uSleepTime = 500;
	if(uSaveTime + uSleepTime > uTimeNow)
	{
		return RC_WAIT;
	}

	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	
	// 死亡
	if( pCharData->IsDie() )
		return RC_ERROR;
	if( pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;


	// 目标位置
	fVector3 fvTarget;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( idTargetObj ));
	if( pTarget == NULL )
		return RC_ERROR;

	fVector3 fvTargetPos = pTarget->GetPosition();

	FLOAT fDistSq = KLU_GetDistSq( fvTargetPos, GetCharacter()->GetPosition() );
	if( fDistSq <= MAX_CLIENT_DEFAULT_EVENT_DIST_SQ )
	{
		SendDefaultEventMessage(idTargetObj);

		// 如果是任务NPC，设置其自身的方向
		if(!g_theKernel.IsKindOf(pTarget->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			if( ((CObject_PlayerNPC*)pTarget)->IsCanFaceToPlayer() )
			{
				// 判断该NPC是否可以转向
				((CObject_PlayerNPC*)pTarget)->SetNeedFaceToPlayer(TRUE);
			}
		}
	}
	else
	{
		// 参数付值
		m_paramAI_DefaultEvent.m_idTargetObj	= idTargetObj;
		m_paramAI_DefaultEvent.m_posPrevTarget  = fvTargetPos;
		SetMySelfAI( MYSELF_AI_DEFAULT_EVENT );
	}

	uSaveTime = uTimeNow;

	return RC_OK;
}


BOOL CAI_MySelf::Enter_Follow( ObjID_t idTargetObj )
{
	// 如果不符合跟随条件 return FALSE;
	if( IsCanFollow() == FALSE )
	{
		return FALSE;
	}

	if( (GetMySelfAI() == MYSELF_AI_FOLLOW) && (m_paramAI_Follow.m_idTargetObj == idTargetObj) )
	{ // 不重复对一个对象进行跟随操作
		return TRUE;
	}

	SetMySelfAI( MYSELF_AI_FOLLOW );

	CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();

	m_paramAI_Follow.m_nTickCount = m_paramAI_Follow.m_nMaxTickCount;
	m_paramAI_Follow.m_idTargetObj = idTargetObj;
	m_paramAI_Follow.m_LastPos = pObj->GetPosition();

	if( Tick_Follow( ) )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Enter_Follow_Staus");
		CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "进入跟随状态。"

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


RC_RESULT CAI_MySelf::Enter_OperateSkill( INT nLogicCount, INT nSkillId, INT nTargetId, FLOAT fTargetPosX, FLOAT fTargetPosZ, FLOAT fDir )
{
	CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
	if( !pObj )
		return RC_ERROR;

	SetMySelfAI( MYSELF_AI_OPERATE_SKILL );

	m_paramAI_OperateSkill;


	return RC_OK;
}



//================-================
// AI逻辑Tick
//================-================

BOOL CAI_MySelf::Tick_Idle( VOID )
{
	return TRUE;
}

BOOL CAI_MySelf::Tick_Move( VOID )
{
	CObject_Character *pCharacter = GetCharacter();

	if( (pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE) != CObject_Character::CHARACTER_LOGIC_MOVE) ||
		 pCharacter->CharacterLogic_IsStopped(CObject_Character::LOGIC_BASE) )
	{
		SetMySelfAI(MYSELF_AI_IDLE);
	}

	return TRUE;
}

BOOL CAI_MySelf::Tick_UseSkill( VOID )
{
	// 根据保存的技能id获取技能数据
	const SSkillData *pSkillData = GetSkillDataMgr()->GetSkillData( m_paramAI_UseSkill.m_idSkill );	
	if( pSkillData == NULL )
		return FALSE;

	//AxTrace(0,0,"Tick_UseSkill skill id = %d", m_paramAI_UseSkill.m_idSkill);

	FLOAT fMaxSkillRange		= pSkillData->m_fMaxAttackRange;	// 最大攻击范围
	FLOAT fMoveRange			= fMaxSkillRange;
	FLOAT fMoveRangeForecast	= 1.f;
	const SCLIENT_SKILL* pImpl = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_paramAI_UseSkill.m_idSkill);
	FLOAT fDamageRange = pImpl ? pImpl->GetDemageRange() : 0.f;

	fMoveRange -= fMoveRangeForecast;
	if( fMoveRange < 0.f )
		fMoveRange = 0.f;

	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	if( pCharData->IsDie() )
	{
		// 清掉默认技能
		CGameInterface::GetMe()->Skill_SetActive(NULL);
		m_bCurrSkill = FALSE;

		return FALSE;
	}
	if ( IsSkillTargetDie() && pSkillData->m_nSelectType == SELECT_TYPE_CHARACTER)
	{
		m_bCurrSkill = FALSE;
		return FALSE;
	}


	//* 自动连续施放 并且 不是点选对象的


	// 以自己为中心， 自动施放技能
	// 检测攻击范围内是否有怪物, 有就直接发送使用技能的消息 
	if( TARGET_AE_AROUND_SELF == pSkillData->m_nTargetingLogic )
	{

		//test2
//		if( pCharacter->m_UseSkillTag )
//			return FALSE;


		// 玩家的位置和方向
		fVector3 fvCurrentPos = pCharacter->GetPosition();
		FLOAT dir = pCharacter->GetFaceDir();

		// 目标
		CObject_Character* pTargetObj = NULL;
		fVector3 fvTargetPos;

		// 距离, 用来判断怪是否在攻击范围内
		FLOAT fDist = 0.0f;

		const std::vector<INT> npcId = CObjectManager::GetMe()->GetNpcObjectId();
		std::vector<INT>::const_iterator iter = npcId.begin();
		std::vector<INT>::const_iterator iterEnd = npcId.end();

		// 遍历对象管理器中的Npc列表， 找到在攻击范围中的
		for( ; iter != iterEnd; ++iter )
		{
			// 通过保存的id取得目标对象
			pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindObject(*iter));
			if( !pTargetObj )
				continue ;

			// 人物已经是死亡状态
			CCharacterData* pCharacterData = pTargetObj->GetCharacterData();
			if( pCharacterData != NULL )
			{
				if( pCharacterData->IsDie() )
					continue ;
			}

			// 如果是玩家自身就不处理了
			if( CObject_Character::CT_PLAYERMYSELF == pTargetObj->GetCharacterType() )
				continue ;
			// 不能被攻击， 应该是Npc不是怪
			if( pTargetObj->CannotBeAttack() )
				continue ;

			// 目标位置
			fvTargetPos = pTargetObj->GetPosition();

			// 当前位置到目标的直线距离
			fDist = KLU_GetDist( fvTargetPos, fvCurrentPos ); 
			if( fDist <= fDamageRange )
			{
				// 发送使用技能的消息
				SendUseSkillMessage( m_paramAI_UseSkill.m_idSkill, m_paramAI_UseSkill.m_guidTarget, m_paramAI_UseSkill.m_idTarget, fvCurrentPos.x, fvCurrentPos.z, dir,pSkillData->m_bAutoRedo);
				break;
			}
		}

		// 如果不是自动连续施放
		if( !pSkillData->m_bAutoRedo )
		{
			// 中断查找
			m_bCurrSkill = FALSE;
		}
	}
	// 点选目标攻击
	else
	{
		// 获取目标位置
		fVector3 fvTarget;
		if( fDamageRange <= 0.f )	// 攻击范围
		{
			// 对单体
			CObject_Character* pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(m_paramAI_UseSkill.m_idTarget));
			
			if( pTargetObj == NULL || pTargetObj->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD)
			{
				m_bCurrSkill = FALSE;
				OnAIStopped();

				return FALSE;
			}

			fvTarget = pTargetObj->GetPosition();
		}
		else
		{
			fvTarget = m_paramAI_UseSkill.m_posTarget;
		}

		// 判断距离
		FLOAT fDist = KLU_GetDist( fvTarget, m_paramAI_UseSkill.m_posPrevTarget );
		if( fDist > fMoveRange + 1.f )
		{
			// 设置移动到的目标位置
			fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fMoveRange);
			m_paramAI_UseSkill.m_posPrevTarget = fvTargetPos;

			RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
			if( rcResult == RC_ERROR )
			{
				m_bCurrSkill = FALSE;
				OnAIStopped();

				return FALSE;
			}
		}
		// 人物处于休闲状态
		else if( pCharacter->CharacterLogic_Get( CObject_Character::LOGIC_BASE ) == CObject_Character::CHARACTER_LOGIC_IDLE )
		{
			FLOAT fMTDist = KLU_GetDist(pCharacter->GetPosition(), fvTarget);
			if( fMTDist > fMaxSkillRange )
			{
				// 设置移动到的目标位置
				fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fMoveRange);
				m_paramAI_UseSkill.m_posPrevTarget = fvTargetPos;

				RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
				if( rcResult == RC_ERROR )
				{
					m_bCurrSkill = FALSE;
					OnAIStopped();

					return FALSE;
				}
			}
			else
			{
				SendUseSkillMessage( m_paramAI_UseSkill.m_idSkill, m_paramAI_UseSkill.m_guidTarget, m_paramAI_UseSkill.m_idTarget, m_paramAI_UseSkill.m_posTarget.x, m_paramAI_UseSkill.m_posTarget.z, m_paramAI_UseSkill.m_fDir,pSkillData->m_bAutoRedo );

				if( !pSkillData->m_bAutoRedo )
				{
					SetForbidTime( USE_SKILL_FORBID_TIME );
					m_bCurrSkill = FALSE;
				}
				else
				{
					// 自动攻击 不设禁止时间
					//SetForbidTime( USE_SKILL_FORBID_TIME );
				}

				//只有不是连续攻击的技能才转入IDLE状态
				//if (pSkillData)
				//{
				//	if (FALSE == pSkillData->m_bAutoRedo)
				//	{
				if( MYSELF_AI_IDLE != GetMySelfAI() )
				{
					m_bCurrSkill = FALSE;
					SetMySelfAI( MYSELF_AI_IDLE );
				}
				//	}
				//}
			}
		}
	}

	return TRUE;
}

BOOL CAI_MySelf::Tick_ActiveTripperObj( VOID )
{
	CObject_Character *pCharacter = GetCharacter();

	fVector3 fvTarget;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( m_paramAI_ActiveTripperObj.m_idTripperObj ));
	if( pTarget == NULL )
	{
		OnAIStopped( );

		return FALSE;
	}

	if(pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE) != CObject_Character::CHARACTER_LOGIC_MOVE)
	{
		SendActiveTripperObjMessage( m_paramAI_ActiveTripperObj.m_idTripperObj );
		if( MYSELF_AI_IDLE != GetMySelfAI() )
		{
			SetMySelfAI( MYSELF_AI_IDLE );
		}
	}

	return TRUE;
}

BOOL CAI_MySelf::Tick_DefaultEvent( VOID )
{
	CObject_Character *pCharacter = GetCharacter();

	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( m_paramAI_DefaultEvent.m_idTargetObj ));
	if( pTarget == NULL )
	{
		OnAIStopped( );

		return FALSE;
	}

	fVector3 fvTarget;
	fvTarget = pTarget->GetPosition();


	FLOAT fDistSq = KLU_GetDistSq( fvTarget, GetCharacter()->GetPosition() );
	if(fDistSq <= MAX_CLIENT_DEFAULT_EVENT_DIST_SQ)
	{
		SendDefaultEventMessage( m_paramAI_DefaultEvent.m_idTargetObj );

		// 如果是任务NPC，设置其自身的方向
		if(!g_theKernel.IsKindOf(pTarget->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			if( ((CObject_PlayerNPC*)pTarget)->IsCanFaceToPlayer() )
			{ 
				// 判断该NPC是否可以转向
				((CObject_PlayerNPC*)pTarget)->SetNeedFaceToPlayer(TRUE);
			}
		}

		OnAIStopped( );

		return TRUE;
	}

	FLOAT fPrevTargetToTargetSq = KLU_GetDistSq( fvTarget, m_paramAI_DefaultEvent.m_posPrevTarget );
	if( pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_IDLE ||	
		fPrevTargetToTargetSq > MAX_CLIENT_DEFAULT_EVENT_DIST_SQ + 1.f )
	{
		fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, MAX_CLIENT_DEFAULT_EVENT_DIST);
		RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
		
		if( rcResult != RC_OK )
		{
			OnAIStopped( );

			return FALSE;
		}

		m_paramAI_DefaultEvent.m_posPrevTarget = fvTargetPos;
	}

	return TRUE;
}

BOOL CAI_MySelf::Tick_Follow( VOID )
{
	if( m_paramAI_Follow.m_nTickCount < m_paramAI_Follow.m_nMaxTickCount )
	{
		++(m_paramAI_Follow.m_nTickCount);

		return TRUE;
	}

	m_paramAI_Follow.m_nTickCount = 0;

	CObject* pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject( m_paramAI_Follow.m_idTargetObj ));
	
	if( pObj == NULL || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)) )
	{
		// 暂时不被动退出跟随，而是自动跑到最后一个位置
//		OnAIStopped( );
//		return FALSE;
	}
	else
	{
		const fVector3& fTargetPos = pObj->GetPosition();
		FLOAT fDistance = KLU_GetDist(m_paramAI_Follow.m_LastPos, fTargetPos);

		if( fDistance < 2 )
		{ 
			// 距离太短就不跑了
			return TRUE;
		}
		//else if ( fDistance > 30 )
		//{ // 距离太长就跟不上了，距离可调
		//	OnAIStopped( );
		//	return FALSE;
		//}
		else
		{
			fVector3 fPos;

			fPos.x = (m_paramAI_Follow.m_LastPos.x - fTargetPos.x) / fDistance + fTargetPos.x;
			fPos.y = fTargetPos.y; // 这个值应该会和实际情况有一定误差，但是基本上不影响实际结果
			fPos.z = (m_paramAI_Follow.m_LastPos.z - fTargetPos.z) / fDistance + fTargetPos.z;
			RC_RESULT Result = AI_MoveTo( fPos.x, fPos.z );
			if ( Result != RC_OK )
				return FALSE;

			m_paramAI_Follow.m_LastPos = fPos;
		}
	}

	return TRUE;
}

BOOL CAI_MySelf::Tick_OperateSkill( VOID )
{
	// 开始的时间
	UINT uStartTime = 0;
	// 获取当前时间
	UINT uTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
	// 延迟时间
	UINT uDelayTime = 5000;

	// 判断是否到达消亡时间
	if(	(uTime - uStartTime) > uDelayTime )
	{
	}


	m_paramAI_OperateSkill;



	CObject* pObj = (CObject*)( CObjectManager::GetMe()->FindServerObject( -1 ) );
	if( ( pObj == NULL ) || !g_theKernel.IsKindOf( pObj->GetClass(), GETCLASS(CObject_Character )) )
	{
	}
	else
	{
		// 满足条件就给逻辑发送命令处理

/*
OC_LOGIC_EVENT:
	GCCharDirectImpactHandler::Execute
	GCDetailHealsAndDamagesHandler::Execute
	GCCharSkill_MissedHandler::Execute

OC_MAGIC_SEND:
	GCCharSkill_SendHandler::Execute
*/

		SCommand_Object cmdTemp;

		cmdTemp.m_wID = OC_MAGIC_SEND;
		cmdTemp.m_wID = OC_LOGIC_EVENT;

		pObj->PushCommand( &cmdTemp );
	}


	// 都处理完了就转换状态
	if( m_paramAI_OperateSkill.m_bEnd )
		SetMySelfAI( MYSELF_AI_IDLE );

	return TRUE;
}



//================-================
// AI逻辑
//================-================


RC_RESULT CAI_MySelf::AI_MoveTo( FLOAT fDestX, FLOAT fDestZ )
{
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	
	//616======* 单机
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 0)
	{

	// 限制移动
	if( pCharData->IsLimitMove() )
		return RC_ERROR;

	// 死亡
	if( pCharData->IsDie() )
		return RC_ERROR;

	}//

	CPath pathFind;
	fVector2 fvCurPos, fvTarPos;
	fvCurPos.x	= pCharacter->GetPosition().x;
	fvCurPos.y	= pCharacter->GetPosition().z;
	fvTarPos.x	= fDestX;
	fvTarPos.y	= fDestZ;
	
	BOOL bResult;
//	INT xx = CCursorMng::GetMe()->GetCursor_State();
//	if(CURSOR_AUTORUN == CCursorMng::GetMe()->GetCursor_State() )

	// 是否是按住鼠标自动行走
	if(	CGameProcedure::s_pProcMain->IsAutoRun() )
	{
		// 点击的位置是否在阻挡区内
		if( CPath::IsPointInUnreachRegion(fvCurPos) )
		{
			bResult = FALSE;
		}
		else
		{
			bResult = (BOOL)(pathFind.GetPathDirect( fvCurPos, fvTarPos ));
		}
	}
	else
	{
		// 创建移动路径
		bResult = (BOOL)(pathFind.CreateMovePath( fvCurPos, fvTarPos ));
		if( !bResult )
		{
			 if(CPath::IsPointInUnreachRegion(fvCurPos))
			 {
				bResult = FALSE;
			 }
			 else
			 {
				bResult = (BOOL)(pathFind.GetPathDirect( fvCurPos, fvTarPos ));
			 }
		}

	}

	if( !bResult )
		return RC_ERROR;

	static WORLD_POS aPathNode[1024];
	UINT dwNumPathNode = (UINT)(pathFind.GetUnitNum());
	if( dwNumPathNode == 0 )
		return RC_ERROR;

	UINT i;
	for( i = 0; i < dwNumPathNode; i++ )
	{
		const CPath::PathUnit &puTemp = pathFind.GetConstPathUnit( i );

		aPathNode[i].m_fX	= puTemp.fvTarget.x;
		aPathNode[i].m_fZ	= puTemp.fvTarget.y;
	}

	CObject_PlayerMySelf *pMySelf = (CObject_PlayerMySelf*)pCharacter;
	INT nSimulantLogicCount = pMySelf->GetSimulantLogicCount();
//	if(pMySelf->GetLogicCount() > nSimulantLogicCount)
	if( pMySelf->GetBaseLogicCount() > nSimulantLogicCount )
	{
//		nSimulantLogicCount = pMySelf->GetLogicCount();
		nSimulantLogicCount = pMySelf->GetBaseLogicCount();
	}

	INT nLogicCount = nSimulantLogicCount + DEF_CLIENT_LOGIC_COUNT_ADD_STEP;
	pMySelf->SetSimulantLogicCount(nLogicCount);

	SCommand_Object cmd;
	cmd.m_wID			= OC_MOVE;
	cmd.m_auParam[0]	= 0;
	cmd.m_anParam[1]	= nLogicCount;
	cmd.m_anParam[2]	= 1;//(INT)(dwNumPathNode);
	cmd.m_apParam[3]	= aPathNode;
	RC_RESULT rcResult = pCharacter->DoSimulantCommand(&cmd);
	
	if(rcResult != RC_OK)
		return rcResult;

	SendMoveMessage( &pathFind, nLogicCount );
	
	return RC_OK;
}

// 
RC_RESULT CAI_MySelf::AI_UseSkill( INT idSkill, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )
{
/*
	人物AI -> 使用技能 -> 向服务器发消息 -> 使用技能
					   -> 做攻击准备动作

	// 判断下是否需要做这个准备动作， 可能有的技能不需要这个

	// 应该当作逻辑指令直接压到人物行为逻辑列表中， 直接处理， 技能使用Tick时判断是否切换到技能的施放动作

	// 在向服务器发消息之前就先调用攻击准备动作
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( idSkill );
	if( pClientSkill == NULL )
		return ;

	// 通过表中的动作数据id 来改变动作 （需要在skl_template.tab 表中增加一列， 表示攻击准备动作）
	pCharacter->ChangeAction( 0, pCharacter->GetLogicSpeed(), FALSE, ACTION_DOWN );
*/


	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();


//	if( pCharacter->m_UseSkillTag )
//		return RC_ERROR;


	// 死亡
	if( pCharData->IsDie() )
		return RC_ERROR;

	CObject_PlayerMySelf *pMySelf = (CObject_PlayerMySelf*)pCharacter;

	INT nSimulantLogicCount = pMySelf->GetSimulantLogicCount();
	if( pMySelf->GetBaseLogicCount() > nSimulantLogicCount )
	{
		nSimulantLogicCount = pMySelf->GetBaseLogicCount();
	}

	INT nLogicCount = nSimulantLogicCount + DEF_CLIENT_LOGIC_COUNT_ADD_STEP;
//	pMySelf->SetSimulantLogicCount( nLogicCount) ;


	SCommand_Object cmd;

	cmd.m_wID			= OC_MAGIC_PREP_SEND;
	cmd.m_auParam[0]	= 0;
	cmd.m_anParam[1]	= nLogicCount;
	cmd.m_anParam[2]	= idSkill;		// 技能ID
	cmd.m_anParam[3]	= idTargetObj;	// 目标ID
	cmd.m_afParam[4]	= fTargetX;		// 目标位置
	cmd.m_afParam[5]	= fTargetZ;		// 目标位置
	cmd.m_afParam[6]	= fDir;			// 目标方向

	// 模拟命令
//	RC_RESULT rcResult = pCharacter->DoSimulantCommand( &cmd );
//	
//	if( rcResult != RC_OK )
//		return rcResult;

	return RC_OK;
}



BOOL CAI_MySelf::IsCanJump( VOID )
{
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();

	if( pCharData->IsLimitMove() )
		return FALSE;

	if( pCharData->IsDie() )
		return FALSE;

	if( pCharacter->GetbJumping() )
		return FALSE;


	BOOL bCharacterLogicCanJump = FALSE;
	CObject_Character::ENUM_CHARACTER_LOGIC eCharacterLogic = pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE);
	switch( eCharacterLogic )
	{
	case CObject_Character::CHARACTER_LOGIC_IDLE:
	case CObject_Character::CHARACTER_LOGIC_MOVE:
	case CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION:
		bCharacterLogicCanJump = TRUE;
		break;

	case CObject_Character::CHARACTER_LOGIC_DEAD:
	case CObject_Character::CHARACTER_LOGIC_STALL:
	default:
		bCharacterLogicCanJump = FALSE;
		break;
	}

	eCharacterLogic = pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_ACTION);
	switch(eCharacterLogic)
	{
	case CObject_Character::CHARACTER_LOGIC_SKILL_GATHER:
	case CObject_Character::CHARACTER_LOGIC_SKILL_LEAD:
		bCharacterLogicCanJump = TRUE;
		break;

	case CObject_Character::CHARACTER_LOGIC_ACTION:
	case CObject_Character::CHARACTER_LOGIC_SKILL_SEND:
	default:
		bCharacterLogicCanJump = FALSE;
		break;
	}


/*
	BOOL bCharacterLogicCanJump = FALSE;
	CObject_Character::ENUM_CHARACTER_LOGIC eCharacterLogic = pCharacter->CharacterLogic_Get();
	switch(eCharacterLogic)
	{
	case CObject_Character::CHARACTER_LOGIC_IDLE:
	case CObject_Character::CHARACTER_LOGIC_MOVE:
	case CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION:
	case CObject_Character::CHARACTER_LOGIC_SKILL_GATHER:
	case CObject_Character::CHARACTER_LOGIC_SKILL_LEAD:
		bCharacterLogicCanJump = TRUE;
		break;
	case CObject_Character::CHARACTER_LOGIC_DEAD:
	case CObject_Character::CHARACTER_LOGIC_STALL:
	case CObject_Character::CHARACTER_LOGIC_ACTION:
	case CObject_Character::CHARACTER_LOGIC_SKILL_SEND:
	default:
		bCharacterLogicCanJump = FALSE;
		break;
	}
*/

	if(!bCharacterLogicCanJump)
		return FALSE;

	return TRUE;
}


RC_RESULT CAI_MySelf::ComposeItem( INT idPrescription )
{
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();

	if( pCharData->IsDie() )
		return RC_ERROR;
	if( pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;

	INT guidPlatform = INVALID_ID;
	INT find_platform = 0;
	
	// 此处应检查是否满足合成之条件，如原材料，技能，配方，平台等

	// item_mix.tab
	static const tDataBase* pPrescrDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_ITEMCOMPOSE);
	const _DBC_LIFEABILITY_ITEMCOMPOSE* m_pPrescrDef = (const _DBC_LIFEABILITY_ITEMCOMPOSE*)pPrescrDBC->Search_Index_EQU(idPrescription);

	INT idAbility = m_pPrescrDef->nLifeAbility;

	// ability.tab
	static const tDataBase* pLifeAbilityDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_DEFINE);
	const _DBC_LIFEABILITY_DEFINE* m_pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)pLifeAbilityDBC->Search_Index_EQU(idAbility);

	// 操作平台
	INT idPlatform	= m_pAbilityDef->nPlatformID;
	FLOAT distPlatform	= m_pAbilityDef->fPlatformDist;

	// 判断等级
	INT my_level =  CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
	if( my_level < m_pAbilityDef->nLevelNeed ) 
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Payer_Level_Not_Enough");
		//CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "人物等级不够。"
		ADDTALKMSG(strTemp);
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
		
		return RC_ERROR;
	}

	// 生活技能数据
	const SCLIENT_LIFEABILITY* player_ability = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(idAbility);
	INT player_ability_level = player_ability->m_nLevel;
	if( player_ability_level < m_pPrescrDef->nLifeAbility_Level) 
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Payer_Skill_Level_Not_Enough");
		//CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "技能等级不够。"
		ADDTALKMSG(strTemp);
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );

		return RC_ERROR;
	}


	// 操作平台
	if( idPlatform >= 0 )
	{
		guidPlatform = Find_Platform(idAbility);
		if( guidPlatform == INVALID_ID )
			return RC_ERROR;
	}
	// 站在原地即可开始合成，无需移动

	// 参数赋值
	SendComposeItemMessage( idAbility, idPrescription, guidPlatform);
	CGameProcedure::s_pGfxSystem->PushDebugString("CGCharComposeItem[%d],[%d],[%d]", idAbility, idPrescription, guidPlatform);
	
	return RC_OK;
}

INT CAI_MySelf::Find_Platform( INT idAbility )
{
	INT guidPlatform = INVALID_ID;
	INT find_platform = 0;
	CObject_Character *pCharacter = GetCharacter();

	static const tDataBase* pLifeAbilityDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_DEFINE);
	const _DBC_LIFEABILITY_DEFINE* m_pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)pLifeAbilityDBC->Search_Index_EQU(idAbility);

	INT idPlatform	= m_pAbilityDef->nPlatformID;
	FLOAT distPlatform	= m_pAbilityDef->fPlatformDist;

	INT my_level =  CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
	if( my_level < m_pAbilityDef->nLevelNeed ) 
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Payer_Level_Not_Enough");
		//CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "人物等级不够。"
		ADDTALKMSG(strTemp);
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );

		return INVALID_ID;
	}

/*
	const SCLIENT_LIFEABILITY* player_ability = 
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(idAbility);
	INT player_ability_level = player_ability->m_nLevel;
	if( player_ability_level < m_pPrescrDef->nLifeAbility_Level) 
	{
		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"Your ability level is not enough..");				
		return INVALID_ID;
	}
*/

	if( idPlatform >=0 )
	{
		CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
		if(!pActiveScene) return FALSE;

		fVector3 fvTarget,fvPlayerMySelf;

		fvPlayerMySelf	= pCharacter->GetPosition();

		INT zone_x = pActiveScene->GetZoneX(fvPlayerMySelf.x);
		INT zone_z = pActiveScene->GetZoneX(fvPlayerMySelf.z);
		CZone* pZone;

		INT final_x = zone_x + 1;
		INT final_z = zone_z + 1;
		// 在当前zone和周围8个zone之中，寻找这个platform
		for( zone_x = final_x - 2; zone_x < final_x; zone_x++ )
		{
			for( zone_z=final_z - 2; zone_z < final_z; zone_z++ )
            {
	
				pZone = pActiveScene->GetZone(zone_x,zone_z);
				if(!pZone) 
					continue;

				FLOAT fDistSq = 0.0f;
				for (std::set<INT>::iterator it = pZone->m_setRegisterObject.begin();it!=pZone->m_setRegisterObject.end();it++)
				{		
					CObject *pTarget = (CObject*)(CObjectManager::GetMe()->FindObject((INT)*it));
					if(!pTarget) 
					{
						continue;
					}

					if(  ( tObject::TOT_PLATFORM == pTarget->Tripper_GetType() ) && ( idPlatform == ((CTripperObject_Platform*)(CTripperObject*)pTarget)->GetPlatformID() ) )
					{
						fvTarget	= pTarget->GetPosition();
						fDistSq		= KLU_GetDistSq( fvTarget, fvPlayerMySelf );
						if ( fDistSq < distPlatform ) 
						{
							guidPlatform = pTarget->GetServerID();
							find_platform = 1;				
							break;
						}
					}
				}
				//此处应检查是否满足合成之条件，如原材料，技能，配方，平台等
				//
				//距离不够，不会走到平台前
				//if ( fDistSq > distPlatform )
				//{

				//	BOOL bResult = MoveTo( fvTarget.x, fvTarget.z );
				//	if ( !bResult )
				//		return FALSE;
				//}
			}

			if(!find_platform) 
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_No_Operate_Platform");
				ADDTALKMSG(strTemp);
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				//ADDTALKMSG("附近没有操作平台。");
				return INVALID_ID;
			}
		}
	}

	return guidPlatform;
}


VOID CAI_MySelf::OnAIStopped( VOID )
{
	switch( GetMySelfAI() )
	{
	case MYSELF_AI_IDLE:
	case MYSELF_AI_MOVE:
	case MYSELF_AI_ACTIVE_TRIPPEROBJ:
	case MYSELF_AI_DEFAULT_EVENT:
		break;

	case MYSELF_AI_FOLLOW:
		{
			StopFollow();
		}
		break;

	default:
		//Assert( FALSE && "CAI_MySelf::OnAIStopped  unknow GetMySelfAI" );
		break;
	}

	Enter_Idle( );
}

VOID CAI_MySelf::StopFollow( VOID )
{
	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Exit_Follow_Staus");

	CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );
}

fVector3 CAI_MySelf::GetTargetPos( fVector3 MePos, fVector3 TargetPos, FLOAT fDist )
{
	fVector3 fvTarget;
	FLOAT fMTDist = KLU_GetDist(MePos, TargetPos);
	FLOAT fGoDist = (fMTDist - fDist + 0.5f) > 0 ? (fMTDist - fDist + 0.5f) : 0;
	
	fvTarget.x = MePos.x + fGoDist * (TargetPos.x - MePos.x) / fMTDist;
	fvTarget.z = MePos.z + fGoDist * (TargetPos.z - MePos.z) / fMTDist;
	fvTarget.y = MePos.y + fGoDist * (TargetPos.y - MePos.y) / fMTDist;

	return fvTarget;
}

VOID CAI_MySelf::SendMoveMessage( const CPath *pPath, INT nLogicCount )
{
	CObject_Character *pCharacter = GetCharacter();

	WORLD_POS posCurrent;
	posCurrent.m_fX	= pCharacter->GetPosition().x;
	posCurrent.m_fZ	= pCharacter->GetPosition().z;

	//向服务器发送请求消息
	CGCharMove msg;
	msg.setObjID( pCharacter->GetServerID() );
	msg.setHandleID( nLogicCount );
	msg.setWorldPos( posCurrent );

	UINT dwNumPathNode = (UINT)(pPath->GetUnitNum());
	if ( dwNumPathNode == 0 )
		return ;

	UINT i;
	WORLD_POS posTarget;
	for ( i = 0; i < dwNumPathNode; i++ )
	{
		const CPath::PathUnit &puTemp = pPath->GetConstPathUnit( i );

		posTarget.m_fX	= puTemp.fvTarget.x;
		posTarget.m_fZ	= puTemp.fvTarget.y;

		// 检测是否合法
		if( !CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(puTemp.fvTarget) )
		{
			return;
		}

		msg.addTargetPos( &posTarget );
	}

	CNetManager::GetMe()->SendPacket( &msg );
//	AxTrace(0, 3, "send move message logic to [%.1f    %.1f] path",posTarget.m_fX, posTarget.m_fZ);
}

VOID CAI_MySelf::SendUseSkillMessage( INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir , BOOL autoRedo)
{
//	CObject_Character* pobj = CObjectManager::GetMe()->GetMySelf();	//->GetCharacterData()

    CObject_Character* pCharacter = GetCharacter();
	if( !pCharacter )
		return ;

	// 不要打开下面的代码，打开会出现连招攻击的技能第一招发不出来
	//// temp code {
	//pCharacter->m_paramLogic_SkillSend.m_fCurTime = 0;
	//pCharacter->CharacterLogic_Set(CObject_Character::CHARACTER_LOGIC_SKILL_SEND);
	//// }

	AxTrace(0, 0, "%d", pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_ACTION));

	WORLD_POS posCurrent, posTarget;
	posCurrent.m_fX	= pCharacter->GetPosition().x;
	posCurrent.m_fZ	= pCharacter->GetPosition().z;
	posTarget.m_fX	= fTargetX;
	posTarget.m_fZ	= fTargetZ;


	RC_RESULT rcResult = AI_UseSkill( idSkill, idTargetObj, posTarget.m_fX, posTarget.m_fZ, fDir );
	if( rcResult != RC_OK )
		return ;


//	if( !pCharacter->m_UseSkillTag )
		pCharacter->m_UseSkillTag = TRUE;


	// 发往服务器
	CGCharUseSkill msg;
	msg.setObjID( pCharacter->GetServerID() );
	msg.setSkillDataID( idSkill );
	msg.setTargetGUID( guidTarget );
	msg.setTargetID( idTargetObj );
	msg.setTargetPos( posTarget );
	msg.setDir( fDir );

	CNetManager::GetMe()->SendPacket( &msg );


	//自动技能,不进入
	if ( !autoRedo )
	{
		CActionSystem::GetMe()->UpdateCommonCoolDown(idSkill);
	}
	else
	{
		m_bCurrSkill = FALSE;
	}
	

/*
//	if( CT_PLAYERMYSELF == GetCharacterType() )
	{
		CHAR szDamage[32];
		memset(szDamage, 0, sizeof(szDamage));

		fVector2 fvPos = pCharacter->m_pInfoBoard ? pCharacter->m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
		fVector3 fvCurPos = pCharacter->GetPosition();

		_sntprintf(szDamage, 32, "%d", idSkill);
		CGameProcedure::s_pUISystem->AddNewBeHitBoard(FALSE, szDamage, fvPos.x, fvPos.y, 10, 0);
	}
*/


	//AxTrace(0, 0, "%s", "Send Skill...");

	CGameProcedure::s_pGfxSystem->PushDebugString("CGCharUseSkill[%d]->[%d]", idSkill, idTargetObj);
}

VOID CAI_MySelf::SendActiveTripperObjMessage( INT idTripperObj )
{
	CTripperObject* pTripperObj = (CTripperObject*)CObjectManager::GetMe()->FindServerObject(idTripperObj);
	if(!pTripperObj) 
		return;

	pTripperObj->Tripper_Active();
}

VOID CAI_MySelf::SendComposeItemMessage( INT idAbility, INT idPrescription,	ObjID_t guidPlatform )
{
	CObject_Character *pCharacter = GetCharacter();

	// 发往服务器
	CGUseAbility msg;
	msg.SetAbilityID( idAbility );
	msg.SetPrescriptionID( idPrescription );
	msg.SetPlatformGUID( guidPlatform );


	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGCharUseAbility[%d],[%d],[%d]", idAbility, idPrescription,	guidPlatform);
}

VOID CAI_MySelf::SendDefaultEventMessage( INT idTargetObj )
{
	CGCharDefaultEvent msg;
	msg.setObjID( idTargetObj );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGCharDefaultEvent");
}

RC_RESULT CAI_MySelf::IsCanUseSkill( INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, BOOL& bNeedMove )
{
	//if ( // GetMySelfAI() == MYSELF_AI_USE_SKILL &&		//======
	//	m_paramAI_UseSkill.m_idSkill == idSkill
	//	&& m_paramAI_UseSkill.m_idTarget == idTargetObj
	//	&& fabsf( m_paramAI_UseSkill.m_posTarget.x - fTargetX ) < 0.1f
	//	&& fabsf( m_paramAI_UseSkill.m_posTarget.z - fTargetZ ) < 0.1f
	//	&& fabsf( m_paramAI_UseSkill.m_fDir - fDir ) < 0.1f )
	//{
	//	return RC_ERROR;
	//}

	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	// 已经死亡
	if( pCharData->IsDie() )
		return RC_ERROR;

	// 正在处理死亡逻辑
	if( pCharacter->CharacterLogic_Get( CObject_Character::LOGIC_BASE ) == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;

	const SSkillData* pSkillData = GetSkillDataMgr()->GetSkillData(idSkill);
	if (NULL == pSkillData)
		return RC_ERROR;
	

	const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(idSkill);
	if(!pSkill)
		return RC_ERROR;

	CObject_PlayerMySelf *pMySelf = CObjectManager::GetMe()->GetMySelf( );
	INT idUser = (pMySelf)?(pMySelf->GetID()):(INVALID_ID);

	// 通过技能内部的条件判断
	ORESULT oResult = pSkill->IsCanUse(idUser, idTargetObj, fTargetX, fTargetZ, fDir );
	if( oResult != OR_OK )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, GetOResultText( oResult ) );
		return RC_ERROR;
	}


	// 当前是否有技能动作正在进行 (聚气动作或引导动作)
	CObject_Character::ENUM_CHARACTER_LOGIC charLogic = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_ACTION);
	if (charLogic == CObject_Character::CHARACTER_LOGIC_SKILL_GATHER ||
		charLogic == CObject_Character::CHARACTER_LOGIC_SKILL_LEAD)
	{
		// 要施展的技能是瞬发技能
		if (TRUE == pSkillData->m_nImmediately)
		{
			INT nOldSkill = CObjectManager::GetMe()->GetMySelf()->GetActivatedSkillID();
			const SSkillData* pOldSkillData = GetSkillDataMgr()->GetSkillData(nOldSkill);
			//if (pOldSkillData && /*不能被打断*/)
			//{
			//	return RC_ERROR;
			//}

			// 不改变当前状态，直接返回成功，让服务器来直接改变人物状态。
			return RC_OK;
		}
		else
		{
			return RC_ERROR;
		}
	} 


	
	

	// 不需要Character目标的技能则直接返回成功
	if( pSkill->m_pDefine->m_nSelectType != SELECT_TYPE_CHARACTER && pSkill->m_pDefine->m_nSelectType != SELECT_TYPE_POS )
	{
		// 以自身为中心的范围攻击
//		if( TARGET_AE_AROUND_SELF == pSkillData->m_nTargetingLogic )
		{
			// 保存数据
			m_paramAI_UseSkill.m_idSkill		= idSkill;
			m_paramAI_UseSkill.m_guidTarget		= guidTarget;
			m_paramAI_UseSkill.m_idTarget		= idTargetObj;
			m_paramAI_UseSkill.m_posTarget.x	= fTargetX;
			m_paramAI_UseSkill.m_posTarget.z	= fTargetZ;
			m_paramAI_UseSkill.m_fDir			= fDir;
			m_bCurrSkill = FALSE;
		}

		return RC_OK;
	}


	fVector3 fvTarget;
	FLOAT fMaxSkillRange, fMaxSkillRangeSq, fDistToTargetSq;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( idTargetObj ));
	if( pTarget == NULL )
	{
		FLOAT fHeight = 0.0f;
		CWorldManager::GetMe()->GetActiveScene()->GetMapHeight(fVector2(fTargetX, fTargetZ), fHeight);
		fvTarget = fVector3(fTargetX, fHeight,fTargetZ);
	}
	else
		// 目标位置
		fvTarget = pTarget->GetPosition();

	// 最大攻击范围
	fMaxSkillRange = pSkill->m_pDefine->m_fMaxAttackRange;
	fMaxSkillRangeSq = fMaxSkillRange * fMaxSkillRange;
	fDistToTargetSq = KLU_GetDistSq( fvTarget, pCharacter->GetPosition() );

	m_paramAI_UseSkill.m_idSkill				= idSkill;
	m_paramAI_UseSkill.m_guidTarget				= guidTarget;
	m_paramAI_UseSkill.m_idTarget				= idTargetObj;
	m_paramAI_UseSkill.m_posTarget				= fvTarget;
	m_paramAI_UseSkill.m_fDir					= fDir;
	
	// 根据距离来标志是否要移动
	if( fMaxSkillRangeSq > fDistToTargetSq )
	{
		bNeedMove = FALSE;
	}
	else
	{
        // 设置移动到的目标位置
		fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fMaxSkillRange);

		RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
		if( rcResult != RC_OK )
		{
			return rcResult;
		}

		// 参数赋值
		m_paramAI_UseSkill.m_posPrevTarget			= fvTargetPos;

		// 设置AI状态
//		SetMySelfAI( MYSELF_AI_USE_SKILL );

		bNeedMove = TRUE;
	}
	// 表示点选攻击的状态
	m_bAttkState = TRUE;


	return RC_OK;
}

BOOL CAI_MySelf::IsCanFollow()
{	
	// 判断玩家当前的状态
	if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Stall_Can_Not_Move");
		CEventSystem::GetMe()->PushEvent(GE_PERSONAL_MESSAGE, strTemp.c_str());	// "摆摊时不能移动。"

		return FALSE;
	}

	// 判断当前的 AI
	switch( GetMySelfAI() )
	{
	case MYSELF_AI_MOVE:
	case MYSELF_AI_IDLE:
	case MYSELF_AI_DEFAULT_EVENT:
	case MYSELF_AI_FOLLOW:
		return TRUE;

	case MYSELF_AI_ACTIVE_TRIPPEROBJ:
	case MYSELF_AI_USE_SKILL:
	//case MYSELF_AI_DEAD:
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Can_Not_Enter_Follow_Staus");
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "无法进入跟随状态！"
		}
		return FALSE;

	default:
		Assert( FALSE && "CAI_MySelf::IsCanFollow  unknow GetMySelfAI" );
		return FALSE;
	}

	return TRUE;
}

BOOL CAI_MySelf::IsLimitCmd( const SCommand_AI *pCmd )
{
	CObject_PlayerMySelf* pMySelf = (CObject_PlayerMySelf*)GetCharacter();
	CCharacterData* pCharData = pMySelf->GetCharacterData();
	CUIDataPool* pUIDataPool = CUIDataPool::GetMe();

	// 实际上这一步不应该放以这里检测，应该放在UI里
	// 检测是否为组队跟随状态
	{
		BOOL bTeamFollowMode = pCharData->Get_TeamFollowFlag();
		BOOL bRet = FALSE;

		if( bTeamFollowMode )
		{
			// 组队跟随状态
			CTeamOrGroup* pTeam = pUIDataPool->GetTeamOrGroup();
			BOOL bIsLeader = (pTeam->GetLeaderGUID() == pMySelf->GetServerGUID());

			switch( pCmd->m_wID )
			{
			case AIC_MOVE:
			case AIC_DEFAULT_EVENT:
			case AIC_FOLLOW:
				{
					if( bIsLeader==FALSE )
					{
						bRet = TRUE;
					}
				}
				break;

			case AIC_USE_SKILL:
			case AIC_TRIPPER_ACTIVE:
			case AIC_COMPOSE_ITEM:
				{
					bRet = TRUE;
				}
				break;

			default:
				break;
			};

			if( bRet == TRUE )
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Team_Follow_Staus");
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "组队跟随状态。"

				strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Not_Allow_Operate");
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "不允许进行这种操作！"
			}
		}

		if( bRet )
			return TRUE;
	}

	CDataPool* pDataPool = CDataPool::GetMe();
	switch( pCmd->m_wID )
	{
	case AIC_MOVE:
		{
			// 是否有限制移动的状态
			const _BUFF_IMPACT_INFO* pBuffImpactInfo;
			INT nLimitMoveBuffID = INVALID_ID;
			pBuffImpactInfo = pDataPool->BuffImpact_GetByID( nLimitMoveBuffID );
			if ( pBuffImpactInfo != NULL )
			{
				return TRUE;
			}
		}
		return FALSE;

	case AIC_USE_SKILL:
		// 是否有限制施法的状态
		return FALSE;
	case AIC_TRIPPER_ACTIVE:
	case AIC_DEFAULT_EVENT:
	case AIC_COMPOSE_ITEM:
	//case AIC_DIE:
	case AIC_JUMP:
	case AIC_FOLLOW:
		return FALSE;

	default:
		Assert( FALSE && "CAI_MySelf::IsLimitCmd  unknow pCmd->m_wID" );
		return TRUE;
	}
}


BOOL	CAI_MySelf::IsSkillTargetDie(VOID)
{
	CObject_Character* pChar = (CObject_Character* )CObjectManager::GetMe()->FindServerObject(m_paramAI_UseSkill.m_idTarget);
	if ( pChar )
	{
		return pChar->IsDie();
	}
	return TRUE;
}