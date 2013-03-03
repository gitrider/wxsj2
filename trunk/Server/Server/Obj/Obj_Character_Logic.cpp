// Character_Logic.cpp

#include "stdafx.h"
#include "Scene.h"
#include "Log.h"
#include "Skill_Core.h"
#include "TimeManager.h"

#include "GCCharJump.h"
#include "GCCharMove.h"
#include "GCArrive.h"
#include "GCCharStopAction.h"
#include "GCOperateResult.h"
#include "GCCharDoAction.h"

#include "GCCharUseSkill.h" 

#include "Obj_Character.h"
#include "ActionDelegator.h"
#include "AI_Character.h"

using namespace Combat_Module::Skill_Module;
using namespace Combat_Module;
using namespace Action_Module;

VOID Obj_Character::SetCharacterLogic(ENUM_CHARACTER_LOGIC eCur)
{
	__ENTER_FUNCTION
	m_eCharacterLogic = eCur;
	m_bCharacterLogicStopped = FALSE;
	__LEAVE_FUNCTION
}

//������ƶ���ֹͣ�ƶ��������ʹ�ü�����ֹͣʹ�ü���
VOID Obj_Character::OnCharacterLogicStopped(BOOL bAbortive)
{
__ENTER_FUNCTION

	ENUM_CHARACTER_LOGIC eLogic = GetCharacterLogic();
	switch(eLogic)
	{
	case CHARACTER_LOGIC_MOVE:
		{
			if(bAbortive)
			{
				GCArrive	msgArrive;
				msgArrive.setObjID( GetID() );
				msgArrive.setHandleID( GetLogicCount() );
				msgArrive.setWorldPos( *(getWorldPos()) );

				getScene()->BroadCast( &msgArrive, this, TRUE ) ;
			}
		}
		break;
	case CHARACTER_LOGIC_USE_SKILL:
		{
			if(bAbortive)
			{
				GCCharStopAction	msgStopAction;
				msgStopAction.setObjID( GetID() );
				SkillInfo_T& oSkillInf = GetSkillInfo();
				if( oSkillInf.GetSkillLogicCount() != INVALID_ID )
				{
					msgStopAction.setLogicCount( oSkillInf.GetSkillLogicCount() );
				}
				else
				{
					msgStopAction.setLogicCount( GetLogicCount() );
				}
				ActionParams_T &rParams = GetActionParams();
				msgStopAction.setStopTime( rParams.GetContinuance() );

				getScene()->BroadCast( &msgStopAction, this, TRUE ) ;
			}
		}
		break;
	//case CHARACTER_LOGIC_DEAD:
	//case CHARACTER_LOGIC_SIT:
	case CHARACTER_LOGIC_IDLE:
	default:
		break;
	}

__LEAVE_FUNCTION
}

VOID Obj_Character::StopCharacterLogic(BOOL bAbortive/* = TRUE*/)
{
__ENTER_FUNCTION
	m_bCharacterLogicStopped = TRUE;
	OnCharacterLogicStopped(bAbortive);
__LEAVE_FUNCTION
}

BOOL Obj_Character::IsMoving( )
{
__ENTER_FUNCTION

	if( GetCharacterLogic()==CHARACTER_LOGIC_MOVE  
		&&	!IsCharacterLogicStopped() )
		return TRUE ;
	else
		return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}
VOID Obj_Character::DoAction(INT nActionID, ObjID_t nTargetObjID/* = INVALID_ID*/)
{
__ENTER_FUNCTION

	Scene *pScene = getScene();
	ZoneID_t nZoneID = getZoneID();
	if(pScene != NULL && nZoneID != INVALID_ID)
	{
		GCCharDoAction msgDoAction;
		msgDoAction.setObjID(GetID());
		msgDoAction.setActionID(nActionID);
		msgDoAction.setTargetID(nTargetObjID);

		pScene->BroadCast(&msgDoAction, this, TRUE);
	}

__LEAVE_FUNCTION
}

VOID Obj_Character::MoveTo( INT nHandleID, WORLD_POS* pTar )
{
__ENTER_FUNCTION

	if ( GetObjType()==OBJ_TYPE_HUMAN )
	{
		Do_Move( nHandleID, 1, (const WORLD_POS*)(pTar), TRUE );
	}
	else
	{
		Do_Move( nHandleID, 1, (const WORLD_POS*)(pTar) );
	}

__LEAVE_FUNCTION
}

VOID Obj_Character::DirectMoveTo(const WORLD_POS* pTar)
{
__ENTER_FUNCTION
	//setWorldPos(pTar);
	//AddLogicCount();
	MoveTo(-1, const_cast<WORLD_POS*>(pTar));
	// send msg to client...
	//GCCharMove Msg;
	//Msg.setObjID( GetID() );
	//Msg.setStartTime( g_pTimeManager->CurrentTime() );
	//Msg.setHandleID( GetLogicCount() );
	//Msg.setTargetPos( *pTar );
	//getScene()->BroadCast( &Msg, this, TRUE );
__LEAVE_FUNCTION
}

VOID Obj_Character::MoveTo( INT nHandleID, WORLD_POS* pCur, WORLD_POS* pTar )
{
	__ENTER_FUNCTION

	Do_Move( nHandleID, 1, (const WORLD_POS*)(pTar) );

	__LEAVE_FUNCTION
}

BOOL Obj_Character::IsLimitMove( VOID )
{
	__ENTER_FUNCTION

	if ( CanMove() )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	__LEAVE_FUNCTION

	return TRUE ;
}
/*
BOOL Obj_Character::IsLimitUseSkill( VOID )
{
	__ENTER_FUNCTION

	if ( CanAction() )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	__LEAVE_FUNCTION
	return TRUE ;
}

BOOL Obj_Character::IsLimitAttack( VOID )
{
	__ENTER_FUNCTION

	if ( CanAction() )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	__LEAVE_FUNCTION

	return TRUE ;
}

BOOL Obj_Character::IsLimitAllHandle( VOID )
{
	return FALSE;

	static BOOL bTest = FALSE;
	if ( bTest )
	{
		return TRUE;
	}

	if ( CanAction() )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
*/
ORESULT Obj_Character::Do_Idle( VOID )
{
	__ENTER_FUNCTION

	//interrupt skill execution
	if(FALSE == GetGlobalActionDelegator().CanDoNextAction(*this))
	{
		GetGlobalActionDelegator().InterruptCurrentAction(*this);
	}

	if(!IsCharacterLogicStopped())
		StopCharacterLogic(TRUE);

	SetCharacterLogic( CHARACTER_LOGIC_IDLE );
	__LEAVE_FUNCTION

	return OR_OK;
}

ORESULT Obj_Character::Do_Jump( VOID )
{
	__ENTER_FUNCTION
	if (IsLimitMove()) 
	{
		return OR_LIMIT_MOVE;
	}
	//Interrupt current action
	GetGlobalActionDelegator().InterruptCurrentAction(*this);
	// send msg to client...
	GCCharJump Msg ;
	Msg.setObjID( GetID() ) ;

	getScene()->BroadCast( &Msg, this ) ;
	m_bSendMsgDirty	= TRUE;

	return OR_OK;
	__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT Obj_Character::Do_Stop(VOID)
{
	__ENTER_FUNCTION

	//interrupt skill execution
	if(FALSE == GetGlobalActionDelegator().CanDoNextAction(*this))
	{
		GetGlobalActionDelegator().InterruptCurrentAction(*this);
	}

	if(!IsCharacterLogicStopped())
		StopCharacterLogic(TRUE);

	SetCharacterLogic( CHARACTER_LOGIC_IDLE );

	__LEAVE_FUNCTION

	return OR_OK;	
}

VOID Obj_Character::FaceTo( const WORLD_POS* pPos )
{
__ENTER_FUNCTION

	Assert( pPos );

	setDir( MyAngle( getWorldPos(), pPos ) );

__LEAVE_FUNCTION
}

VOID Obj_Character::FaceTo( const Obj* pObj )
{
__ENTER_FUNCTION

	Assert( pObj );

	if ( pObj == NULL || pObj == (Obj*)this )
	{
		return;
	}

	FaceTo( pObj->getWorldPos() );

__LEAVE_FUNCTION
}

ORESULT Obj_Character::Do_Move( INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos, BOOL bFlag )
{
	__ENTER_FUNCTION

	//BOOL bStopMove = TRUE;
	INT nStopLogicCount = GetLogicCount();
	if(!IsCharacterLogicStopped())
	{
		if(GetCharacterLogic() != CHARACTER_LOGIC_MOVE)
		{
			//�ƶ����жϼ���
			StopCharacterLogic(FALSE);
		}
		else
		{
			//bStopMove = TRUE;
			StopCharacterLogic(FALSE);
		}
	}

	if (IsDie())
		return OR_DIE;
		
	if (TRUE == IsLimitMove()) 
	{
		return OR_LIMIT_MOVE;
	}

	if ( nHandleID != -1 )
	{
		//m_paramLogic_Move.m_nHandleID	= nHandleID;
		SetLogicCount(nHandleID);
	}
	else
	{
		//m_paramLogic_Move.m_nHandleID++;
		AddLogicCount();
	}

	WORD i;
	m_paramLogic_Move.Reset();
	for ( i = 0; i < wNumTargetPos; i++ )
	{
		m_paramLogic_Move.AddTargetPos( &paTargetPos[i] );
	}

	FLOAT fDir;
	const WORLD_POS *pTargetPos, *pWorldPos;

	pWorldPos	= getWorldPos();
	pTargetPos	= m_paramLogic_Move.GetCurTargetPos();

	fDir = MyAngle( getWorldPos(), pTargetPos );
	setDir( fDir );
	//interrupt skill execution
	if(FALSE == GetGlobalActionDelegator().CanDoNextAction(*this))
	{
		GetGlobalActionDelegator().InterruptCurrentAction(*this);
	}

	SetCharacterLogic( CHARACTER_LOGIC_MOVE );

	// send msg to client...
	GCCharMove Msg ;
	Msg.setObjID( GetID() );
	Msg.setStartTime( g_pTimeManager->CurrentTime() );
	Msg.setHandleID( GetLogicCount() );
	Msg.setTargetPos( *pTargetPos );
	//if(bStopMove)
	//{
	Msg.setStopInfo(nStopLogicCount, getWorldPos());
	//}

	getScene()->BroadCast( &Msg, this, TRUE );
	m_bSendMsgDirty	= TRUE;

	//if ( this->GetObjType() == OBJ_TYPE_HUMAN )
	//{
	//	this->setWorldPos(pTargetPos);
	//}
	__LEAVE_FUNCTION

	return OR_OK;
}

VOID Obj_Character::DirectTo( const WORLD_POS* pTar, BOOL bMove )
{
	setWorldPos( pTar );
	GCCharMove Msg ;
	Msg.setObjID( GetID() );
	Msg.setStartTime( g_pTimeManager->CurrentTime() );
	Msg.setHandleID( GetLogicCount() );
	Msg.setTargetPos( *pTar );
	Msg.SetMove( bMove );
	//˲�ƣ���״̬��Ϊս��
	if( !bMove )
	{
		m_pAI_Character->ChangeState( ESTATE_COMBAT );
	}
	getScene()->BroadCast( &Msg, this, TRUE );
	m_bSendMsgDirty	= TRUE;
}

ORESULT Obj_Character::Do_UseSkill( SkillID_t idSkill, ObjID_t idTarget, const WORLD_POS *pTargetPos, INT iHurtDelayTime, FLOAT fDir, GUID_t guidTarget)
{
__ENTER_FUNCTION

	//�������Ҽ��ܲ��ж��ƶ�
	if(!IsCharacterLogicStopped() /*&& GetObjType() != OBJ_TYPE_HUMAN*/)
		StopCharacterLogic(TRUE);

	//if ( GetCharacterLogic() == CHARACTER_LOGIC_MOVE && !m_bCharacterLogicStopped )
	//{
	//	GCArrive	msgArrive;
	//	msgArrive.setObjID( GetID() );
	//	msgArrive.setHandleID( GetLogicCount() );
	//	msgArrive.setWorldPos( *(getWorldPos()) );

	//	getScene()->BroadCast( &msgArrive, this, TRUE ) ;
	//}

	AddLogicCount();
	WORLD_POS pos;
	if(NULL!=pTargetPos)
	{
		pos.m_fX=pTargetPos->m_fX;
		pos.m_fZ=pTargetPos->m_fZ;
	}
	else
	{
		pos.m_fX=0.0f;
		pos.m_fZ=0.0f;
	}
	if(FALSE == g_SkillCore.ProcessSkillRequest(*this, idSkill, idTarget, pos, fDir, guidTarget, iHurtDelayTime))
	{
		return (ORESULT)GetTargetingAndDepletingParams().GetErrCode();
	}
	else // ʹ�ü��ܳɹ��󣬷��ͼ������Ը��ͻ��ˣ�������ʾ��������		20100602 BLL
	{
		if ( this->GetObjType() == OBJ_TYPE_HUMAN ) // ��������ʹ�ü��ܣ��������Լ���
		{
			GCCharUseSkill msg;

			msg.SetIsMyself( TRUE );
			msg.SetSenderID( GetID() );
			msg.SetSkillID( idSkill );

			Player* pGamePlayer = ((Obj_Human*)this)->GetPlayer();

			if( NULL != pGamePlayer )
			{
				pGamePlayer->SendPacket( &msg );
			}
		}
		else if ( this->GetObjType() == OBJ_TYPE_MONSTER ) // ����ǹ���ʹ�ü��ܣ����Ŀ����ҷ�
		{
			GCCharUseSkill msg;

			msg.SetIsMyself( FALSE );
			msg.SetSenderID( GetID() );
			msg.SetSkillID( idSkill );

			Obj_Human* pHuman = getScene()->GetHumanManager()->GetHuman( idTarget );

			if ( NULL != pHuman )
			{
				Player* pGamePlayer = ((Obj_Human*)pHuman)->GetPlayer();

				if( NULL != pGamePlayer )
				{
					pGamePlayer->SendPacket( &msg );
				}

			}//if ( NULL != pHuman )		

		}//if ( this->GetObjType() == OBJ_TYPE_HUMAN )

	}//if(FALSE == g_SkillCore.ProcessSkillRequest

	if (GetCharacterLogic() != CHARACTER_LOGIC_USE_SKILL || TRUE==IsCharacterLogicStopped())
	{
		SetCharacterLogic( CHARACTER_LOGIC_USE_SKILL );
	}
	if (TRUE==GetGlobalActionDelegator().CanDoNextAction(*this))
	{
		if (GetCharacterLogic() == CHARACTER_LOGIC_USE_SKILL || FALSE==IsCharacterLogicStopped())
		{
			StopCharacterLogic(FALSE);
		}		
	}
	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

BOOL Obj_Character::Obj_Logic_Idle( UINT uTime )
{
	return TRUE;
}

BOOL Obj_Character::Obj_Logic_Move( UINT uTime )
{
	__ENTER_FUNCTION

	if(IsDie())
	{
		return FALSE;
	}

	if (TRUE == IsLimitMove())
	{// �����޷��ƶ���״̬��������ֹ��ǰ�ƶ��߼�״̬
		return FALSE;
	}
	if ( m_paramLogic_Move.IsEmpty() )
		return FALSE;

	const WORLD_POS *pCurPos, *pTargetPos;

	pCurPos					= getWorldPos();
	pTargetPos				= m_paramLogic_Move.GetCurTargetPos();

	// ��һ֡���ƶ���·������
	FLOAT fSpeed = GetMoveSpeed() ;
	if(fSpeed <= 0.1) 
	{
		fSpeed = 0.11;
	}
	if(fSpeed >= 20.0) 
	{
		fSpeed = 19.99;
	}
	Assert( fSpeed > 0.1 && fSpeed < 20.0 ); // ������֤����Ҫ��fancy
	UINT uLogicTime = GetLogicTime() ;
	FLOAT fMoveDist = (fSpeed*uLogicTime)/1000.0f ;
	if(fMoveDist <= 0.01f)
	{
		return TRUE;
	}

	// ��ǰλ���뵱ǰ��Ŀ��·����·���ĳ���
	FLOAT fDistToTarget = MySqrt( pCurPos, pTargetPos ) ;

	// �����һ֡���ƶ���·������С�ڵ�ǰλ�õ���ǰ��Ŀ��·����·������
	while(TRUE)
	{
		if(fMoveDist > fDistToTarget)
		{
			if ( !m_paramLogic_Move.NextTargetPos() )
			{//û����һ��·����
				// Ŀ��·�������һ��
				if (this->GetObjType() == OBJ_TYPE_HUMAN )
				{
					int h = 9;
				}

				WORLD_POS posMustTo;
				posMustTo.m_fX = pTargetPos->m_fX;
				posMustTo.m_fZ = pTargetPos->m_fZ;
				getScene()->GetMap()->VerifyPos( &posMustTo );

				FLOAT fDir = MyAngle( getWorldPos(), &posMustTo );
				setDir( fDir );

				setWorldPos( &posMustTo );
				StopCharacterLogic( FALSE );
				break;
			}
			else
			{
				if (this->GetObjType() == OBJ_TYPE_HUMAN )
				{
					int h = 9;
				}

				fMoveDist -= fDistToTarget;
				pCurPos		= pTargetPos;
				pTargetPos	= m_paramLogic_Move.GetCurTargetPos();
				fDistToTarget = MySqrt( pCurPos, pTargetPos ) ;

				AddLogicCount();

				// send msg to client...
				GCCharMove Msg ;
				Msg.setObjID( GetID() );
				Msg.setStartTime( g_pTimeManager->CurrentTime() );
				Msg.setHandleID( GetLogicCount() );
				Msg.setTargetPos( *pTargetPos );

				getScene()->BroadCast( &Msg, this, TRUE );
			}
		}
		else
		{
			if (this->GetObjType() == OBJ_TYPE_HUMAN )
			{
				int h = 9;
			}

			WORLD_POS Cur ;
			if(fDistToTarget > 0.f)
			{
				Cur.m_fX = pCurPos->m_fX + (fMoveDist*(pTargetPos->m_fX-pCurPos->m_fX))/fDistToTarget ;
				Cur.m_fZ = pCurPos->m_fZ + (fMoveDist*(pTargetPos->m_fZ-pCurPos->m_fZ))/fDistToTarget ;
			}
			else
			{
				Cur.m_fX = pTargetPos->m_fX;
				Cur.m_fZ = pTargetPos->m_fZ;
			}

			FLOAT fDir = MyAngle( pCurPos, pTargetPos );
			setDir( fDir );

			getScene()->GetMap()->VerifyPos( &Cur );
			setWorldPos( &Cur );
			break;
		}
	}

	//if ( fDistToTarget>fMoveDist && fDistToTarget>0)
	//{
	//	WORLD_POS Cur ;
	//	Cur.m_fX = pCurPos->m_fX + (fMoveDist*(pTargetPos->m_fX-pCurPos->m_fX))/fDistToTarget ;
	//	Cur.m_fZ = pCurPos->m_fZ + (fMoveDist*(pTargetPos->m_fZ-pCurPos->m_fZ))/fDistToTarget ;

	//	getScene()->GetMap()->VerifyPos( &Cur );
	//	setWorldPos( &Cur );
	//}
	//else
	//{
	//	if ( !m_paramLogic_Move.NextTargetPos() )
	//	{//û����һ��·����
	//		// Ŀ��·�������һ��
	//		WORLD_POS posMustTo;
	//		posMustTo.m_fX = pTargetPos->m_fX;
	//		posMustTo.m_fZ = pTargetPos->m_fZ;
	//		getScene()->GetMap()->VerifyPos( &posMustTo );
	//		setWorldPos( &posMustTo );
	//		
	//		StopCharacterLogic( FALSE );
	//	}
	//	else
	//	{//����·���㣬������
	//		// Ŀ��·�������һ��
	//		fMoveDist -= fDistToTarget;
	//		pCurPos		= pTargetPos;
	//		pTargetPos	= m_paramLogic_Move.GetCurTargetPos();
	//		fDistToTarget = MySqrt( pCurPos, pTargetPos ) ;

	//		WORLD_POS Cur ;
	//		if(fDistToTarget > 0.f)
	//		{
	//			Cur.m_fX = pCurPos->m_fX + (fMoveDist*(pTargetPos->m_fX-pCurPos->m_fX))/fDistToTarget ;
	//			Cur.m_fZ = pCurPos->m_fZ + (fMoveDist*(pTargetPos->m_fZ-pCurPos->m_fZ))/fDistToTarget ;
	//		}
	//		else
	//		{
	//			Cur.m_fX = pTargetPos->m_fX;
	//			Cur.m_fZ = pTargetPos->m_fZ;
	//		}

	//		getScene()->GetMap()->VerifyPos( &Cur );
	//		setWorldPos( &Cur );

	//		FLOAT fDir = MyAngle( &Cur, pTargetPos );
	//		setDir( fDir );

	//		AddLogicCount();

	//		// send msg to client...
	//		GCCharMove Msg ;
	//		Msg.setObjID( GetID() );
	//		Msg.setStartTime( g_pTimeManager->CurrentTime() );
	//		Msg.setHandleID( GetLogicCount() );
	//		Msg.setTargetPos( *pTargetPos );

	//		getScene()->BroadCast( &Msg, this, TRUE );
	//		m_bSendMsgDirty	= TRUE;
	//	}
	//}
	__LEAVE_FUNCTION
	return TRUE;
}

BOOL Obj_Character::Obj_Logic_UseSkill( UINT uTime )
{
	__ENTER_FUNCTION
	if(IsDie())
	{
		return FALSE;
	}
	Time_t nDeltaTime = GetLogicTime();
	return GetGlobalActionDelegator().OnHeartBeat(*this, nDeltaTime);
	__LEAVE_FUNCTION
	return TRUE;
}
