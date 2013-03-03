
/** GMAI_MySelf.cpp
 *	
 *	���ܣ� ������ҵ�ģ������� �����ڿͻ��˽��д���
 *	
 *	�޸ļ�¼��
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
	// �жϽ�ֹʱ��
	if( m_uForbidTime > 0 )
	{
		if( CGameProcedure::s_pTimeSystem->GetTimeNow() - m_uForbidStartTime >= m_uForbidTime )
			m_uForbidTime = 0;
	}

	// �ж��Ƿ���δִ�е�����
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
	// ��ǰ����ʩ����ϣ� ���� �����ű���ľɼ���
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

	// ʹ�ü��ܵ���������
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



// ѹ��һ��ָ��
BOOL CAI_MySelf::PushCommand( const SCommand_AI *pCmd )
{
	// ����գ����ܴ˴β����Ƿ�Ϊ��
	m_SaveCommand.m_wID = AIC_NONE;


/*
	m_SaveOldSkillCommand;

	// ��ǰ������ִ�еļ��� �� �µ�����Ҳ��ʩ�ż���
	// �������˵���µļ��������ǲ���Ĳ������������溯���������жϣ�
	if( m_bCurrSkill && ( AIC_USE_SKILL == pCmd->wID ) )
	{
		// ����ɵļ���
		m_SaveOldSkillCommand  = *pCmd;		// ��ʱ����䱣������µļ��ܣ� Ҫ����ɵĴ˴����߼���Ҫ�޸�
											// Ҫ�ж��Ƿ���ʩ�ţ� ���ǾͿ�������� �Ǿ�Ҫ���棬 ֱ���´ε���ʱ���ж��Ƿ����
	}
	else
	{
		// ���
		m_SaveCommand.m_wID = AIC_NONE;
		m_SaveOldSkillCommand.m_wID = AIC_NONE;
	}
*/



	if( !IsLimitCmd( pCmd ) )
	{
		// �Ƚ�pCmd��������
		m_SaveCommand = *pCmd;

		if( m_uForbidTime == 0 )
		{
			RC_RESULT rcResult = OnCommand(pCmd);
			
			if( rcResult != RC_WAIT )
			{
				// ���ִ�гɹ��򽫻������
				m_SaveCommand.m_wID = AIC_NONE;
			}
		}
	}

	return TRUE;
}

RC_RESULT CAI_MySelf::OnCommand( const SCommand_AI *pCmd )
{
	RC_RESULT rcResult;

	// ���ư�̯״̬�²��ܽ��еĻ
	if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_You_Stalling");
		CEventSystem::GetMe()->PushEvent(GE_PERSONAL_MESSAGE, strTemp.c_str());		// "�����ڰ�̯����"

		return RC_OK;
	}

	switch( pCmd->m_wID )
	{
	// �ƶ�
	case AIC_MOVE:
		{
			fVector2 fv2TargetPos;
			fv2TargetPos.x	= pCmd->m_afParam[0];
			fv2TargetPos.y	= pCmd->m_afParam[1];
			
			rcResult = Enter_Move( fv2TargetPos.x, fv2TargetPos.y );
		}
		break;

	// ʹ�ü���
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
// AI�߼�Enter
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
	// ��������Ƿ����JUMP
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
		CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );		// "��̯ʱ�����ƶ�"

		return RC_ERROR;
	}
	
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();

	//616======* ����
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


	// �����Զ���ѡ������״̬�� ͣ�����ܵ��ж�
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

	// ���ư�̯��ʱ�����ﲻ��ʹ�ü���
	if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Stall_Can_User_Skill");
		CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "��̯ʱ����ʹ�ü���"
		
		return RC_ERROR;
	}


	BOOL bNeedMove = FALSE;
	// �ж��ܷ�ʹ�ü���
	RC_RESULT rcResult = IsCanUseSkill( idSkill, guidTarget, idTargetObj, fTargetX, fTargetZ, fDir, bNeedMove );
	if( rcResult != RC_OK )
		return rcResult;


	uSaveTime = uTimeNow;

	// ���Զ���ֵļ���
	const SSkillData* pSkillData = GetSkillDataMgr()->GetSkillData(idSkill);
	if (pSkillData && pSkillData->m_bAutoRedo && FALSE == pSkillData->m_nImmediately)
	{
		// ��ʶ���ܼ���
		m_bCurrSkill = TRUE;
		return RC_OK;
	}
	// ����Ҫ�ƶ�
	if (FALSE == bNeedMove)
	{
		// ���ڹ�����Χ��
		SetForbidTime( USE_SKILL_FORBID_TIME );
		SendUseSkillMessage( idSkill, guidTarget, idTargetObj, fTargetX, fTargetZ, fDir ,pSkillData->m_bAutoRedo);

		return RC_OK;
	}
	else
	{
		// ��ʶ���ܼ���, ��������ҲӦ��ִ�����ֲ���
		m_bCurrSkill = TRUE;
	}

	// ����
//	m_paramAI_SaveSkill = m_paramAI_UseSkill;

	//uSaveTime = uTimeNow;

	//// ��ʶ���ܼ���
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

	// �Ѿ�����
	if( pCharData->IsDie() )
		return RC_ERROR;
	if( pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;

	// �ж�Ŀ���Ƿ����
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
		//����ǵ���Ļ����Ͳ����ߵ�����ȥ�ˡ�
		//����Ҳ��Ҫ������ʣ�������벻����Ҳ���ܿ�ʼ����
		if(	tObject::TOT_RESOURCE == pTarget->Tripper_GetType() && ((CTripperObject_Resource*)pTarget)->Resource_IsFish())
		{
			ADDTALKMSG("����̫Զ��");
			return RC_ERROR;
		}
		else
		{
			fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fOperationDistance);
			RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
			if ( rcResult != RC_OK )
				return rcResult;

			// ������ֵ
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
	
	// ����
	if( pCharData->IsDie() )
		return RC_ERROR;
	if( pCharacter->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;


	// Ŀ��λ��
	fVector3 fvTarget;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( idTargetObj ));
	if( pTarget == NULL )
		return RC_ERROR;

	fVector3 fvTargetPos = pTarget->GetPosition();

	FLOAT fDistSq = KLU_GetDistSq( fvTargetPos, GetCharacter()->GetPosition() );
	if( fDistSq <= MAX_CLIENT_DEFAULT_EVENT_DIST_SQ )
	{
		SendDefaultEventMessage(idTargetObj);

		// ���������NPC������������ķ���
		if(!g_theKernel.IsKindOf(pTarget->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			if( ((CObject_PlayerNPC*)pTarget)->IsCanFaceToPlayer() )
			{
				// �жϸ�NPC�Ƿ����ת��
				((CObject_PlayerNPC*)pTarget)->SetNeedFaceToPlayer(TRUE);
			}
		}
	}
	else
	{
		// ������ֵ
		m_paramAI_DefaultEvent.m_idTargetObj	= idTargetObj;
		m_paramAI_DefaultEvent.m_posPrevTarget  = fvTargetPos;
		SetMySelfAI( MYSELF_AI_DEFAULT_EVENT );
	}

	uSaveTime = uTimeNow;

	return RC_OK;
}


BOOL CAI_MySelf::Enter_Follow( ObjID_t idTargetObj )
{
	// ��������ϸ������� return FALSE;
	if( IsCanFollow() == FALSE )
	{
		return FALSE;
	}

	if( (GetMySelfAI() == MYSELF_AI_FOLLOW) && (m_paramAI_Follow.m_idTargetObj == idTargetObj) )
	{ // ���ظ���һ��������и������
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
		CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "�������״̬��"

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
// AI�߼�Tick
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
	// ���ݱ���ļ���id��ȡ��������
	const SSkillData *pSkillData = GetSkillDataMgr()->GetSkillData( m_paramAI_UseSkill.m_idSkill );	
	if( pSkillData == NULL )
		return FALSE;

	//AxTrace(0,0,"Tick_UseSkill skill id = %d", m_paramAI_UseSkill.m_idSkill);

	FLOAT fMaxSkillRange		= pSkillData->m_fMaxAttackRange;	// ��󹥻���Χ
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
		// ���Ĭ�ϼ���
		CGameInterface::GetMe()->Skill_SetActive(NULL);
		m_bCurrSkill = FALSE;

		return FALSE;
	}
	if ( IsSkillTargetDie() && pSkillData->m_nSelectType == SELECT_TYPE_CHARACTER)
	{
		m_bCurrSkill = FALSE;
		return FALSE;
	}


	//* �Զ�����ʩ�� ���� ���ǵ�ѡ�����


	// ���Լ�Ϊ���ģ� �Զ�ʩ�ż���
	// ��⹥����Χ���Ƿ��й���, �о�ֱ�ӷ���ʹ�ü��ܵ���Ϣ 
	if( TARGET_AE_AROUND_SELF == pSkillData->m_nTargetingLogic )
	{

		//test2
//		if( pCharacter->m_UseSkillTag )
//			return FALSE;


		// ��ҵ�λ�úͷ���
		fVector3 fvCurrentPos = pCharacter->GetPosition();
		FLOAT dir = pCharacter->GetFaceDir();

		// Ŀ��
		CObject_Character* pTargetObj = NULL;
		fVector3 fvTargetPos;

		// ����, �����жϹ��Ƿ��ڹ�����Χ��
		FLOAT fDist = 0.0f;

		const std::vector<INT> npcId = CObjectManager::GetMe()->GetNpcObjectId();
		std::vector<INT>::const_iterator iter = npcId.begin();
		std::vector<INT>::const_iterator iterEnd = npcId.end();

		// ��������������е�Npc�б� �ҵ��ڹ�����Χ�е�
		for( ; iter != iterEnd; ++iter )
		{
			// ͨ�������idȡ��Ŀ�����
			pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindObject(*iter));
			if( !pTargetObj )
				continue ;

			// �����Ѿ�������״̬
			CCharacterData* pCharacterData = pTargetObj->GetCharacterData();
			if( pCharacterData != NULL )
			{
				if( pCharacterData->IsDie() )
					continue ;
			}

			// ������������Ͳ�������
			if( CObject_Character::CT_PLAYERMYSELF == pTargetObj->GetCharacterType() )
				continue ;
			// ���ܱ������� Ӧ����Npc���ǹ�
			if( pTargetObj->CannotBeAttack() )
				continue ;

			// Ŀ��λ��
			fvTargetPos = pTargetObj->GetPosition();

			// ��ǰλ�õ�Ŀ���ֱ�߾���
			fDist = KLU_GetDist( fvTargetPos, fvCurrentPos ); 
			if( fDist <= fDamageRange )
			{
				// ����ʹ�ü��ܵ���Ϣ
				SendUseSkillMessage( m_paramAI_UseSkill.m_idSkill, m_paramAI_UseSkill.m_guidTarget, m_paramAI_UseSkill.m_idTarget, fvCurrentPos.x, fvCurrentPos.z, dir,pSkillData->m_bAutoRedo);
				break;
			}
		}

		// ��������Զ�����ʩ��
		if( !pSkillData->m_bAutoRedo )
		{
			// �жϲ���
			m_bCurrSkill = FALSE;
		}
	}
	// ��ѡĿ�깥��
	else
	{
		// ��ȡĿ��λ��
		fVector3 fvTarget;
		if( fDamageRange <= 0.f )	// ������Χ
		{
			// �Ե���
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

		// �жϾ���
		FLOAT fDist = KLU_GetDist( fvTarget, m_paramAI_UseSkill.m_posPrevTarget );
		if( fDist > fMoveRange + 1.f )
		{
			// �����ƶ�����Ŀ��λ��
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
		// ���ﴦ������״̬
		else if( pCharacter->CharacterLogic_Get( CObject_Character::LOGIC_BASE ) == CObject_Character::CHARACTER_LOGIC_IDLE )
		{
			FLOAT fMTDist = KLU_GetDist(pCharacter->GetPosition(), fvTarget);
			if( fMTDist > fMaxSkillRange )
			{
				// �����ƶ�����Ŀ��λ��
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
					// �Զ����� �����ֹʱ��
					//SetForbidTime( USE_SKILL_FORBID_TIME );
				}

				//ֻ�в������������ļ��ܲ�ת��IDLE״̬
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

		// ���������NPC������������ķ���
		if(!g_theKernel.IsKindOf(pTarget->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			if( ((CObject_PlayerNPC*)pTarget)->IsCanFaceToPlayer() )
			{ 
				// �жϸ�NPC�Ƿ����ת��
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
		// ��ʱ�������˳����棬�����Զ��ܵ����һ��λ��
//		OnAIStopped( );
//		return FALSE;
	}
	else
	{
		const fVector3& fTargetPos = pObj->GetPosition();
		FLOAT fDistance = KLU_GetDist(m_paramAI_Follow.m_LastPos, fTargetPos);

		if( fDistance < 2 )
		{ 
			// ����̫�̾Ͳ�����
			return TRUE;
		}
		//else if ( fDistance > 30 )
		//{ // ����̫���͸������ˣ�����ɵ�
		//	OnAIStopped( );
		//	return FALSE;
		//}
		else
		{
			fVector3 fPos;

			fPos.x = (m_paramAI_Follow.m_LastPos.x - fTargetPos.x) / fDistance + fTargetPos.x;
			fPos.y = fTargetPos.y; // ���ֵӦ�û��ʵ�������һ�������ǻ����ϲ�Ӱ��ʵ�ʽ��
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
	// ��ʼ��ʱ��
	UINT uStartTime = 0;
	// ��ȡ��ǰʱ��
	UINT uTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
	// �ӳ�ʱ��
	UINT uDelayTime = 5000;

	// �ж��Ƿ񵽴�����ʱ��
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
		// ���������͸��߼����������

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


	// ���������˾�ת��״̬
	if( m_paramAI_OperateSkill.m_bEnd )
		SetMySelfAI( MYSELF_AI_IDLE );

	return TRUE;
}



//================-================
// AI�߼�
//================-================


RC_RESULT CAI_MySelf::AI_MoveTo( FLOAT fDestX, FLOAT fDestZ )
{
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	
	//616======* ����
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 0)
	{

	// �����ƶ�
	if( pCharData->IsLimitMove() )
		return RC_ERROR;

	// ����
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

	// �Ƿ��ǰ�ס����Զ�����
	if(	CGameProcedure::s_pProcMain->IsAutoRun() )
	{
		// �����λ���Ƿ����赲����
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
		// �����ƶ�·��
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
	����AI -> ʹ�ü��� -> �����������Ϣ -> ʹ�ü���
					   -> ������׼������

	// �ж����Ƿ���Ҫ�����׼�������� �����еļ��ܲ���Ҫ���

	// Ӧ�õ����߼�ָ��ֱ��ѹ��������Ϊ�߼��б��У� ֱ�Ӵ��� ����ʹ��Tickʱ�ж��Ƿ��л������ܵ�ʩ�Ŷ���

	// �������������Ϣ֮ǰ���ȵ��ù���׼������
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( idSkill );
	if( pClientSkill == NULL )
		return ;

	// ͨ�����еĶ�������id ���ı䶯�� ����Ҫ��skl_template.tab ��������һ�У� ��ʾ����׼��������
	pCharacter->ChangeAction( 0, pCharacter->GetLogicSpeed(), FALSE, ACTION_DOWN );
*/


	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();


//	if( pCharacter->m_UseSkillTag )
//		return RC_ERROR;


	// ����
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
	cmd.m_anParam[2]	= idSkill;		// ����ID
	cmd.m_anParam[3]	= idTargetObj;	// Ŀ��ID
	cmd.m_afParam[4]	= fTargetX;		// Ŀ��λ��
	cmd.m_afParam[5]	= fTargetZ;		// Ŀ��λ��
	cmd.m_afParam[6]	= fDir;			// Ŀ�귽��

	// ģ������
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
	
	// �˴�Ӧ����Ƿ�����ϳ�֮��������ԭ���ϣ����ܣ��䷽��ƽ̨��

	// item_mix.tab
	static const tDataBase* pPrescrDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_ITEMCOMPOSE);
	const _DBC_LIFEABILITY_ITEMCOMPOSE* m_pPrescrDef = (const _DBC_LIFEABILITY_ITEMCOMPOSE*)pPrescrDBC->Search_Index_EQU(idPrescription);

	INT idAbility = m_pPrescrDef->nLifeAbility;

	// ability.tab
	static const tDataBase* pLifeAbilityDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_DEFINE);
	const _DBC_LIFEABILITY_DEFINE* m_pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)pLifeAbilityDBC->Search_Index_EQU(idAbility);

	// ����ƽ̨
	INT idPlatform	= m_pAbilityDef->nPlatformID;
	FLOAT distPlatform	= m_pAbilityDef->fPlatformDist;

	// �жϵȼ�
	INT my_level =  CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
	if( my_level < m_pAbilityDef->nLevelNeed ) 
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Payer_Level_Not_Enough");
		//CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "����ȼ�������"
		ADDTALKMSG(strTemp);
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
		
		return RC_ERROR;
	}

	// ���������
	const SCLIENT_LIFEABILITY* player_ability = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(idAbility);
	INT player_ability_level = player_ability->m_nLevel;
	if( player_ability_level < m_pPrescrDef->nLifeAbility_Level) 
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Payer_Skill_Level_Not_Enough");
		//CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "���ܵȼ�������"
		ADDTALKMSG(strTemp);
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );

		return RC_ERROR;
	}


	// ����ƽ̨
	if( idPlatform >= 0 )
	{
		guidPlatform = Find_Platform(idAbility);
		if( guidPlatform == INVALID_ID )
			return RC_ERROR;
	}
	// վ��ԭ�ؼ��ɿ�ʼ�ϳɣ������ƶ�

	// ������ֵ
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
		//CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "����ȼ�������"
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
		// �ڵ�ǰzone����Χ8��zone֮�У�Ѱ�����platform
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
				//�˴�Ӧ����Ƿ�����ϳ�֮��������ԭ���ϣ����ܣ��䷽��ƽ̨��
				//
				//���벻���������ߵ�ƽ̨ǰ
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
				//ADDTALKMSG("����û�в���ƽ̨��");
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

	//�����������������Ϣ
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

		// ����Ƿ�Ϸ�
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

	// ��Ҫ������Ĵ��룬�򿪻�������й����ļ��ܵ�һ�з�������
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


	// ����������
	CGCharUseSkill msg;
	msg.setObjID( pCharacter->GetServerID() );
	msg.setSkillDataID( idSkill );
	msg.setTargetGUID( guidTarget );
	msg.setTargetID( idTargetObj );
	msg.setTargetPos( posTarget );
	msg.setDir( fDir );

	CNetManager::GetMe()->SendPacket( &msg );


	//�Զ�����,������
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

	// ����������
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
	// �Ѿ�����
	if( pCharData->IsDie() )
		return RC_ERROR;

	// ���ڴ��������߼�
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

	// ͨ�������ڲ��������ж�
	ORESULT oResult = pSkill->IsCanUse(idUser, idTargetObj, fTargetX, fTargetZ, fDir );
	if( oResult != OR_OK )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, GetOResultText( oResult ) );
		return RC_ERROR;
	}


	// ��ǰ�Ƿ��м��ܶ������ڽ��� (������������������)
	CObject_Character::ENUM_CHARACTER_LOGIC charLogic = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_ACTION);
	if (charLogic == CObject_Character::CHARACTER_LOGIC_SKILL_GATHER ||
		charLogic == CObject_Character::CHARACTER_LOGIC_SKILL_LEAD)
	{
		// Ҫʩչ�ļ�����˲������
		if (TRUE == pSkillData->m_nImmediately)
		{
			INT nOldSkill = CObjectManager::GetMe()->GetMySelf()->GetActivatedSkillID();
			const SSkillData* pOldSkillData = GetSkillDataMgr()->GetSkillData(nOldSkill);
			//if (pOldSkillData && /*���ܱ����*/)
			//{
			//	return RC_ERROR;
			//}

			// ���ı䵱ǰ״̬��ֱ�ӷ��سɹ����÷�������ֱ�Ӹı�����״̬��
			return RC_OK;
		}
		else
		{
			return RC_ERROR;
		}
	} 


	
	

	// ����ҪCharacterĿ��ļ�����ֱ�ӷ��سɹ�
	if( pSkill->m_pDefine->m_nSelectType != SELECT_TYPE_CHARACTER && pSkill->m_pDefine->m_nSelectType != SELECT_TYPE_POS )
	{
		// ������Ϊ���ĵķ�Χ����
//		if( TARGET_AE_AROUND_SELF == pSkillData->m_nTargetingLogic )
		{
			// ��������
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
		// Ŀ��λ��
		fvTarget = pTarget->GetPosition();

	// ��󹥻���Χ
	fMaxSkillRange = pSkill->m_pDefine->m_fMaxAttackRange;
	fMaxSkillRangeSq = fMaxSkillRange * fMaxSkillRange;
	fDistToTargetSq = KLU_GetDistSq( fvTarget, pCharacter->GetPosition() );

	m_paramAI_UseSkill.m_idSkill				= idSkill;
	m_paramAI_UseSkill.m_guidTarget				= guidTarget;
	m_paramAI_UseSkill.m_idTarget				= idTargetObj;
	m_paramAI_UseSkill.m_posTarget				= fvTarget;
	m_paramAI_UseSkill.m_fDir					= fDir;
	
	// ���ݾ�������־�Ƿ�Ҫ�ƶ�
	if( fMaxSkillRangeSq > fDistToTargetSq )
	{
		bNeedMove = FALSE;
	}
	else
	{
        // �����ƶ�����Ŀ��λ��
		fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fMaxSkillRange);

		RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
		if( rcResult != RC_OK )
		{
			return rcResult;
		}

		// ������ֵ
		m_paramAI_UseSkill.m_posPrevTarget			= fvTargetPos;

		// ����AI״̬
//		SetMySelfAI( MYSELF_AI_USE_SKILL );

		bNeedMove = TRUE;
	}
	// ��ʾ��ѡ������״̬
	m_bAttkState = TRUE;


	return RC_OK;
}

BOOL CAI_MySelf::IsCanFollow()
{	
	// �ж���ҵ�ǰ��״̬
	if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Stall_Can_Not_Move");
		CEventSystem::GetMe()->PushEvent(GE_PERSONAL_MESSAGE, strTemp.c_str());	// "��̯ʱ�����ƶ���"

		return FALSE;
	}

	// �жϵ�ǰ�� AI
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
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "�޷��������״̬��"
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

	// ʵ������һ����Ӧ�÷��������⣬Ӧ�÷���UI��
	// ����Ƿ�Ϊ��Ӹ���״̬
	{
		BOOL bTeamFollowMode = pCharData->Get_TeamFollowFlag();
		BOOL bRet = FALSE;

		if( bTeamFollowMode )
		{
			// ��Ӹ���״̬
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
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "��Ӹ���״̬��"

				strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_Not_Allow_Operate");
				CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strTemp.c_str() );	// "������������ֲ�����"
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
			// �Ƿ��������ƶ���״̬
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
		// �Ƿ�������ʩ����״̬
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