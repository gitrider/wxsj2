
/** Obj_Character_Logic.cpp
 *	
 *	���ܣ� ����������߼�
 *	
 *	�߼����
 *			�����߼����������У�����������ģ�û���������߼�ʱ����������ƶ�������ܣ�ԭ�����������ƶ�������������̯
 *			��Ϊ�߼����������ܣ����������������У����ж�����ҩ��
 *	
 *	�޸ģ� 
 *	
 */

#include "StdAfx.h"
#include "Global.h"
#include "NetWork\NetManager.h"

#include "GIUtil.h"
#include "GISound.h"
#include "GITimeSystem.h"
#include "GIDBC_Struct.h"
#include "GIException.h"

#include "SkillDataMgr.h"
#include "CGCharPositionWarp.h"
#include "CGCharSit.h"

#include "..\..\Common\BuffImpactMgr.h"
#include "..\..\Common\DirectlyImpactMgr.h"
#include "World\WorldManager.h"
#include "Action\GMActionSystem.h"
#include "Event\GMEventSystem.h"
#include "DBC\GMDataBase.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDP_CharacterData.h"
#include "DataPool\GMDataPool.h"
#include "Sound\GMSoundSource.h"
#include "Sound\GMSoundSystem.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "..\..\ObjectDef.h"
#include "..\..\Manager\ObjectManager.h"
#include "..\..\ObjectCommand.h"
#include "..\..\ObjectCommandDef.h"
#include "..\TripperObj\GMTripperObj_ItemBox.h"
#include "..\TripperObj\GMTripperObj_ItemDrop.h"
#include "..\Surface\Obj_Effect.h"
#include "..\Surface\Obj_Bullet.h"

#include "Obj_PlayerMySelf.h"
#include "Obj_Character.h"

#include "GIVariable.h"
#include "../../Manager/BulletEntityManager.h"
#include "../Game/Interface\GMGameInterface.h"

#include "..\..\Entity\ObjEntityActor.h"
#include "ModelSystem/FairyLogicModel.h"

#include "..\Game\Variable\GMVariable.h" 


#define OUTMSG(x) { ADDTALKMSG(x); }

const static INT DAMAGE_INFO_DISPLAY_CD = 150; // �����˺���Ϣ��ʱ����ʾ���



//======�Ѿ�����
// ����������߼�ʱҪ�ж��Ƿ���˲���ļ���ʹ�ã�
// 1����û�м���ʹ�õ�������ֱ��ʹ��˲���ļ���
// 2�����м���ʹ�õ������ֻ��ʾ˲����ɵ�Ч���� ���ŵ���Ϊ�߼������еȴ�
//======��Ϊ��
// ˲���Ĵ��ǰһ���� ��Ҫ��ǰһ������Ч��������������


// ѹ��һ��ָ��
BOOL CObject_Character::PushCommand( const SCommand_Object *pCmd )
{
	if( pCmd == NULL )
		return FALSE;

	switch( pCmd->m_wID )
	{
	// ����ֱ�ӵ�Ч�����ӵ��¼�
	case OC_LOGIC_EVENT:
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		_LOGIC_EVENT *pLogicEvent;
		pLogicEvent = (_LOGIC_EVENT*)(pCmd->m_apParam[0]);

		INT nCreaterID			= pLogicEvent->m_nSenderID;
		INT nCreaterLogicCount	= pLogicEvent->m_nSenderLogicCount;

		CObject* pCreater = (CObject*)(pObjectManager->FindServerObject( nCreaterID ));
		if (pCreater != NULL)
		{
			if (FALSE == DoLogicEvent(pLogicEvent))
			{
				AddLogicEvent( pLogicEvent );
			}
		}
		// ʩ������Ч �� ������һ��skill��Impact�ͷ����¼�������
		else
		{
			AddLogicEvent(pLogicEvent);
		}
	}
	break ;


	// �ж��߼�ָ��
	case OC_STOP_ACTION:
	case OC_STOP_MOVE:
	case OC_STOP_SIT:
	{
		// ����ֹͣ�߼�����
		CObjectCommand_StopLogic *pStopCommand = (CObjectCommand_StopLogic*)(NewObjectCommand(pCmd));
		if(pStopCommand != NULL)
		{
			CObjectCommand_Logic *pLogicCommand = NULL;

			// ������������
			if( OC_STOP_ACTION == pCmd->m_wID )
			{
				pLogicCommand = FindActionLogicCommand( pStopCommand->GetLogicCount() );
			}
			else
			{
				pLogicCommand = FindBaseLogicCommand( pStopCommand->GetLogicCount() );
			}

			if( pLogicCommand != NULL )
			{
				pLogicCommand->Modify(pStopCommand);

				if( (pStopCommand->GetCommandID() == OBJECT_COMMAND_STOP_MOVE) && 
					(pStopCommand->GetLogicCount() == GetBaseLogicCount() ||
					 pStopCommand->GetLogicCount() == GetActionLogicCount() ) )
				{
					CheckMoveStop();
				}
			}
			else
			{
				if( (pStopCommand->GetCommandID()  == OBJECT_COMMAND_STOP_MOVE) && 
					(pStopCommand->GetLogicCount() == GetLastModifyPosLogicCount() ))
				{
					if( CharacterLogic_IsStopped(LOGIC_BASE) || CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_MOVE )
					{
						CObjectCommand_StopMove *pStopMoveCommand = (CObjectCommand_StopMove*)pStopCommand;
						fVector2 fvPos(pStopMoveCommand->GetEndPos()->m_fX, pStopMoveCommand->GetEndPos()->m_fZ);

						FLOAT fStopToCurrentDist = KLU_GetDist(fvPos, fVector2(GetPosition().x, GetPosition().z));
						if( fStopToCurrentDist > DEF_CHARACTER_POS_ADJUST_DIST )
						{
							// �������
							if( CObjectManager::GetMe()->GetMySelf() == this )
							{
								// ��λ�ò����ĳ��ֵʱ
								if(fStopToCurrentDist > DEF_CLIENT_ADJUST_POS_WARP_DIST)
								{
									// ˲�Ƶ���ǰ��������Ӧ��λ��
									SetMapPosition( fvPos );
								}
								else
								{
									// ����������λ�õ���ǰ���ǿͻ��˶�Ӧ��λ��
									CGCharPositionWarp msgWarp;
									WORLD_POS posCur(GetPosition().x, GetPosition().z);
									WORLD_POS posServer(fvPos.x, fvPos.y);
									msgWarp.setObjID(GetServerID());
									msgWarp.setServerPos(posServer);
									msgWarp.setClientPos(posCur);
									CNetManager::GetMe()->SendPacket(&msgWarp);
								}
							}
							else
							{
								// ˲�Ƶ���ǰ��������Ӧ��λ��
								SetMapPosition( fvPos );
							}
						}
					}
				}
			}

			DeleteObjectCommand(pStopCommand);
		}
	}
	break;


	// �߼�ָ���Ϊ�����߼�����Ϊ�߼���

	case OC_MOVE:		// �ƶ�
	case OC_ABILITY:	// �����
	{
		//temp
		if( CT_PLAYERMYSELF == GetCharacterType() && (OC_MOVE == pCmd->m_wID) )
		{
			INT ttt = GetCharacterType();
		}


		CObjectCommand_Logic *pLogicCommand = (CObjectCommand_Logic*)(NewObjectCommand(pCmd));
		if(pLogicCommand != NULL)
		{
			// ����͵�ǰ���߼�������� �� ������֮ǰ���߼�����Ͷ���
			if( pLogicCommand->GetLogicCount() <= GetBaseLogicCount() )
			{
				DeleteObjectCommand(pLogicCommand);
			}
			else
			{
				CObjectCommand_Logic *pFindCommand = FindBaseLogicCommand( pLogicCommand->GetLogicCount() );
				// û���ҵ���Ӧ��ָ��ż��룬��Ҫ�Ƿ�ֹ��ͻ������ǵ�Ԥ��ָ�����ͻ
				if(pFindCommand == NULL)
				{
					PushBaseLogicCommand(pLogicCommand);
				}
				else
				{
					DeleteObjectCommand(pLogicCommand);
				}
			}
		}

		// ��ǰû��ִ�е��߼�
		if( CharacterLogic_IsStopped(LOGIC_BASE) )
		{
			ProcessLogicCommand();
		}
	}
	break ;

	case OC_ACTION:			// ��������ҩ�ȣ�
	case OC_MAGIC_SEND:		// ���ܷ���
	case OC_MAGIC_CHARGE:	// ���ܾ���
	case OC_MAGIC_CHANNEL:	// ��������
	case OC_MAGIC_PREP_SEND:// ���ܷ���׼��	//test2
	{
		CObjectCommand_Logic *pLogicCommand = (CObjectCommand_Logic*)(NewObjectCommand(pCmd));
		if( pLogicCommand != NULL )
		{
			if( !PushActionLogicCommand(pLogicCommand) )
			{
				SAFE_DELETE(pLogicCommand);
			}
		}

		// ��ǰû��ִ�е��߼�
//		if( CharacterLogic_IsStopped(LOGIC_ACTION) )	//test1 �����˲���ļ��ܾͺ������
		{
			ProcessLogicCommand();
		}
	}
	break ;

	// ���Ŷ���
	case OC_MODIFY_ACTION:	
	{
		INT nModifyTime, nLogicCount;
		nLogicCount		= pCmd->m_anParam[0];
		nModifyTime		= pCmd->m_anParam[1];

		if( GetActionLogicCount() == nLogicCount && !CharacterLogic_IsStopped(LOGIC_ACTION) )
		{
			if( CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_GATHER )
			{
				CObjectCommand_MagicCharge *pMagicChargeCommand = (CObjectCommand_MagicCharge*)( GetActionLogicCommand() );
				if(nModifyTime >= 0)
				{
					UINT uModifyTime = (UINT)nModifyTime;
					if(m_paramLogic_MagicCharge.m_uCurrentTime > uModifyTime)
					{
						m_paramLogic_MagicCharge.m_uCurrentTime -= uModifyTime;
					}
					else
					{
						m_paramLogic_MagicCharge.m_uCurrentTime = 0;
					}
				}
				else
				{
					UINT uModifyTime = (UINT)(abs(nModifyTime));
					if(m_paramLogic_MagicCharge.m_uCurrentTime + uModifyTime > pMagicChargeCommand->GetTotalTime())
					{
						m_paramLogic_MagicCharge.m_uCurrentTime = pMagicChargeCommand->GetTotalTime();
					}
					else
					{
						m_paramLogic_MagicCharge.m_uCurrentTime += uModifyTime;
					}
				}
			}
			else if( CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_LEAD )
			{
				CObjectCommand_MagicChannel *pMagicChannelCommand = (CObjectCommand_MagicChannel*)(GetActionLogicCommand());
				if(nModifyTime >= 0)
				{
					UINT uModifyTime = (UINT)nModifyTime;
					if(m_paramLogic_MagicChannel.m_uCurrentTime > uModifyTime)
					{
						m_paramLogic_MagicChannel.m_uCurrentTime -= uModifyTime;
					}
					else
					{
						m_paramLogic_MagicChannel.m_uCurrentTime = 0;
					}
				}
				else
				{
					UINT uModifyTime = (UINT)(abs(nModifyTime));
					if(m_paramLogic_MagicChannel.m_uCurrentTime + uModifyTime > pMagicChannelCommand->GetTotalTime())
					{
						m_paramLogic_MagicChannel.m_uCurrentTime = pMagicChannelCommand->GetTotalTime();
					}
					else
					{
						m_paramLogic_MagicChannel.m_uCurrentTime += uModifyTime;
					}
				}
			}
		}
	}
	break;


	default:
	{
		// ֱ�Ӵ�������
		DoCommand(pCmd);
	}
	break;

	};

	return TRUE;
}


//================-================
// �߼�ִ�в���
//================-================

// �������ָ��
BOOL CObject_Character::ProcessLogicCommand( VOID )
{
	FLOAT fSpeed;
	INT nLogicCommandCount = (INT)(m_listBaseLogicCommand.size());
	if(nLogicCommandCount > 0)
	{
		fSpeed = (FLOAT)(nLogicCommandCount) * 0.5f + 1.f;
	}
	else
	{
		fSpeed = 1.f;
	}

	SetLogicSpeed(fSpeed);


	// �жϻ����߼��Ƿ�ֹͣ
	if( CharacterLogic_IsStopped(LOGIC_BASE) )
	{
		// ִ����һ������
		BOOL bResult = DoNextLogicCommand(LOGIC_BASE);
		if(!bResult)
		{
			// û�п��Դ���ľ͵�������
			Start_Idle();
		}
	}
	// ��ǰ�����߼�������
	else if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_IDLE )
	{
		DoNextLogicCommand(LOGIC_BASE);
	}

	// �ж���Ϊ�߼��Ƿ�ֹͣ
	if( CharacterLogic_IsStopped(LOGIC_ACTION) )	//test1 �����˲���ļ��ܾͺ������
	{
		DoNextLogicCommand(LOGIC_ACTION);	
	}

	return TRUE;
}

// ִ����һ���߼�����
BOOL CObject_Character::DoNextLogicCommand( LOGIC_TYPE nLogicTag )
{
	// �����߼�
	if( LOGIC_BASE == nLogicTag )
	{
		if( !IsBaseLogicCommandListEmpty() )
		{
			CObjectCommand_Logic *pLogicCommand = NextLogicCommand( LOGIC_BASE );

			BOOL bResult = DoLogicCommand(pLogicCommand);

			if(!bResult)
			{
				DeleteObjectCommand(pLogicCommand);
			}

			return bResult;
		}
	}
	// ��Ϊ�߼�
	else
	{
		if( !IsActionLogicCommandListEmpty() )
		{
			CObjectCommand_Logic *pLogicCommand = NextLogicCommand( LOGIC_ACTION );

			BOOL bResult = DoLogicCommand(pLogicCommand);

			if(!bResult)
			{
				DeleteObjectCommand(pLogicCommand);
			}

			return bResult;
		}
	}

	return FALSE;
}

// �����߼�����
BOOL CObject_Character::DoLogicCommand( CObjectCommand_Logic *pLogicCmd )
{
	BOOL bResult = FALSE;
	switch(pLogicCmd->GetCommandID())
	{
	case OBJECT_COMMAND_ACTION:
		{
			bResult = Start_Action(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_MOVE:
		{
			bResult = Start_Move(pLogicCmd);
		}
		break;
		//test2
	case OBJECT_COMMAND_MAGIC_PREP_SEND:
		{
			bResult = Start_MagicPrepSend( pLogicCmd );
		}
		break ;
	case OBJECT_COMMAND_MAGIC_SEND:
		{
			bResult = Start_MagicSend(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_MAGIC_CHARGE:
		{
			bResult = Start_MagicCharge(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_MAGIC_CHANNEL:
		{
			bResult = Start_MagicChannel(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_ABILITY:
		{
			bResult = Start_Ability(pLogicCmd);
		}
		break;
	default:
		//Assert(FALSE && "CObject_Character::DoLogicCommand unknow pLogicCmd id");
		break;
	}

	return bResult;
}


//================-================
// �߼���ز���
//================-================

BOOL CObject_Character::PushBaseLogicCommand(CObjectCommand_Logic *pCmd)
{
	INT nLogicCount = pCmd->GetLogicCount();
	if(!m_listBaseLogicCommand.empty())
	{
		CObjectLogicCommandList::iterator itCur, itEnd;
		CObjectCommand_Logic *pCommand;
		
		itCur = m_listBaseLogicCommand.begin();
		itEnd = m_listBaseLogicCommand.end();
		
		while( itCur != itEnd )
		{
			pCommand = *itCur;
			if(pCommand->GetLogicCount() > nLogicCount)
			{
				m_listBaseLogicCommand.insert(itCur, pCmd);
				return TRUE;
			}

			itCur++;
		}
	}

	m_listBaseLogicCommand.push_back(pCmd);
	return TRUE;
}
BOOL CObject_Character::PushActionLogicCommand(CObjectCommand_Logic *pCmd)
{
	INT nLogicCount = pCmd->GetLogicCount();

	//���Ա������߼�������߼�
	if (nLogicCount < GetActionLogicCount())
	{
		return FALSE;
	}

	if( !m_listActionLogicCommand.empty() )
	{
		CObjectLogicCommandList::iterator itCur, itEnd;
		CObjectCommand_Logic *pCommand;
		
		itCur = m_listActionLogicCommand.begin();
		itEnd = m_listActionLogicCommand.end();
		
		while( itCur != itEnd )
		{
			pCommand = *itCur;
			if(pCommand->GetLogicCount() > nLogicCount)
			{
				m_listActionLogicCommand.insert(itCur, pCmd);
				return TRUE;
			}

			itCur++;
		}
	}

	m_listActionLogicCommand.push_back(pCmd);
	return TRUE;
}

BOOL CObject_Character::IsBaseLogicCommandListEmpty(VOID)
{
	return m_listBaseLogicCommand.empty();
}
BOOL CObject_Character::IsActionLogicCommandListEmpty(VOID)
{
	return m_listActionLogicCommand.empty();
}

BOOL CObject_Character::CleanupLogicCommandList( LOGIC_TYPE nLogicTag )
{
	CObjectLogicCommandList::iterator itCur, itEnd;
	CObjectCommand_Logic *pCommand;

	// ���ݲ�ͬ���߼����岻ͬ���б�

	// �����߼�
	if( LOGIC_BASE == nLogicTag )
	{
		itCur = m_listBaseLogicCommand.begin();
		itEnd = m_listBaseLogicCommand.end();
		
		while( itCur != itEnd )
		{
			pCommand = *itCur;
			itCur++;
			DeleteObjectCommand(pCommand);
		}

		m_listBaseLogicCommand.erase( m_listBaseLogicCommand.begin(), m_listBaseLogicCommand.end() );
	}
	// ��Ϊ�߼�
	else	// LOGIC_ACTION
	{
		itCur = m_listActionLogicCommand.begin();
		itEnd = m_listActionLogicCommand.end();
		
		while( itCur != itEnd )
		{
			pCommand = *itCur;
			itCur++;
			DeleteObjectCommand(pCommand);
		}

		m_listActionLogicCommand.erase( m_listActionLogicCommand.begin(), m_listActionLogicCommand.end() );
	}

	return TRUE;
}

CObjectCommand_Logic *CObject_Character::FindBaseLogicCommand( INT nLogicCount )
{
	// ��ǰ����ִ�е�ָ��
	if(m_pBaseLogicCommand != NULL && m_pBaseLogicCommand->GetLogicCount() == nLogicCount)
	{
		return m_pBaseLogicCommand;
	}

	// �����ָ��
	CObjectLogicCommandList::iterator itCur, itEnd;
	CObjectCommand_Logic *pCommand;
	
	itCur = m_listBaseLogicCommand.begin();
	itEnd = m_listBaseLogicCommand.end();
	
	while( itCur != itEnd )
	{
		pCommand = *itCur;
		itCur++;
		if(pCommand->GetLogicCount() == nLogicCount)
		{
			return pCommand;
		}
	}

	return NULL;
}

CObjectCommand_Logic* CObject_Character::FindActionLogicCommand( INT nLogicCount )
{
	if(m_pActionLogicCommand != NULL && m_pActionLogicCommand->GetLogicCount() == nLogicCount)
	{
		return m_pActionLogicCommand;
	}

	// �����ָ��
	CObjectLogicCommandList::iterator itCur, itEnd;
	CObjectCommand_Logic *pCommand;

	itCur = m_listActionLogicCommand.begin();
	itEnd = m_listActionLogicCommand.end();

	while( itCur != itEnd )
	{
		pCommand = *itCur;
		itCur++;
		if(pCommand->GetLogicCount() == nLogicCount)
		{
			return pCommand;
		}
	}

	return NULL;
}

CObjectCommand_Logic *CObject_Character::GetBaseLogicCommand(VOID)
{
	return m_pBaseLogicCommand;
}
VOID CObject_Character::SetBaseLogicCommand( CObjectCommand_Logic *pLogicCommand )
{
	if(m_pBaseLogicCommand != NULL)
	{
		DeleteObjectCommand(m_pBaseLogicCommand);
	}
	m_pBaseLogicCommand = pLogicCommand;
}

CObjectCommand_Logic *CObject_Character::GetActionLogicCommand(VOID)
{
	return m_pActionLogicCommand;
}
VOID CObject_Character::SetActionLogicCommand( CObjectCommand_Logic *pLogicCommand )
{
	if(m_pActionLogicCommand != NULL)
	{
		DeleteObjectCommand(m_pActionLogicCommand);
	}
	m_pActionLogicCommand = pLogicCommand;
}


const CObjectCommand_Logic *CObject_Character::GetNextLogicCommand( LOGIC_TYPE nLogicTag ) const
{
	// �����߼�
	if( LOGIC_BASE == nLogicTag )
	{
		if(!m_listBaseLogicCommand.empty())
		{
			CObjectCommand_Logic *pCmd = m_listBaseLogicCommand.front();
			return pCmd;
		}
	}
	// ��Ϊ�߼�
	else
	{
		if(!m_listActionLogicCommand.empty())
		{
			CObjectCommand_Logic *pCmd = m_listActionLogicCommand.front();
			return pCmd;
		}
	}

	return NULL;
}

CObjectCommand_Logic *CObject_Character::NextLogicCommand( LOGIC_TYPE nLogicTag )
{
	// �����߼�
	if( LOGIC_BASE == nLogicTag )
	{
		if(!m_listBaseLogicCommand.empty())
		{
			CObjectCommand_Logic *pCmd = m_listBaseLogicCommand.front();
			m_listBaseLogicCommand.pop_front();
			return pCmd;
		}
	}
	// ��Ϊ�߼�
	else
	{
		if(!m_listActionLogicCommand.empty())
		{
			CObjectCommand_Logic *pCmd = m_listActionLogicCommand.front();
			m_listActionLogicCommand.pop_front();
			return pCmd;
		}
	}

	return NULL;
}


BOOL CObject_Character::IsBaseLogicReady( INT nLogicCount ) const
{
	// �߼��������ڴ洢��֮ǰ������ִ��
	if( GetBaseLogicCount() >= nLogicCount )
	{
		return TRUE;
	}

	return FALSE;
}
BOOL CObject_Character::IsActionLogicReady( INT nLogicCount ) const
{
	// �߼��������ڴ洢��֮ǰ������ִ��
	if( GetActionLogicCount() > nLogicCount )
	{
		return TRUE;
	}
	// �ǵ�ǰ���߼�
	else if( GetActionLogicCount() == nLogicCount )
	{
		// �������ʹ�ü��ܵ�
		if( CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_SEND )
		{
			// �жϹ�����Ч�Ƿ���ʾ���
//			if(IsBeAttackEffectShowed())
			{
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}


VOID CObject_Character::CharacterLogic_Set( ENUM_CHARACTER_LOGIC eLogic, LOGIC_TYPE nLogicTag )
{
	// ����ĳ�߼�״̬�Ŀ�ʼ
	if(LOGIC_BASE == nLogicTag)
	{
		m_bCharacterBaseLogicStopped = FALSE;
		m_nCharacterBaseLogic = eLogic;
	}
	else
	{
		m_bCharacterActionLogicStopped = FALSE;
		m_nCharacterActionLogic = eLogic;
	}
}
CObject_Character::ENUM_CHARACTER_LOGIC CObject_Character::CharacterLogic_Get( LOGIC_TYPE nLogicTag ) const			
{ 
	if( LOGIC_BASE == nLogicTag )
	{
		return m_nCharacterBaseLogic; 
	}
	else
	{
		return m_nCharacterActionLogic;
	}
}


VOID CObject_Character::CharacterLogic_Stop( BOOL bFinished, LOGIC_TYPE nLogicTag )
{
	if( LOGIC_BASE == nLogicTag )
	{
		SetBaseLogicCommand(NULL);
	}
	else
	{
		SetActionLogicCommand(NULL);
	}

	CharacterLogic_OnStopped( bFinished, nLogicTag );
}

VOID CObject_Character::CharacterLogic_OnStopped( BOOL bFinished, LOGIC_TYPE nLogicTag )
{
	if(LOGIC_BASE == nLogicTag)
	{
		// ��ǻ����߼�ֹͣ
		m_bCharacterBaseLogicStopped = TRUE;

	}
	else
	{
		// �����Ϊ�߼�ֹͣ
		m_bCharacterActionLogicStopped = TRUE;
	}


	// ��������ˣ� �ı�Ϊվ����
	if(GetCharacterData() &&GetCharacterData()->IsSit())
	{
		StandUp();
	}

	// ���ý�����
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_HIDE);
	}

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_ABILITY_ACTION )
	{
		if(m_pLifeAbilitySound != NULL)
		{
			// ֹͣ���������
			m_pLifeAbilitySound->Stop();
			CGameProcedure::s_pSoundSystem->Source_Destroy(m_pLifeAbilitySound);
			m_pLifeAbilitySound = 0;
		}
	}

	// ֹͣ�ƶ�������
	if( (m_pWlakSound != NULL) && (CObjectManager::GetMe()->GetMySelf() == this) )
	{
		m_pWlakSound->Stop();
		mPlayState = true;
	}
}

BOOL CObject_Character::CharacterLogic_IsStopped( LOGIC_TYPE nLogicTag ) const		
{
	if( LOGIC_BASE == nLogicTag )
	{
		return m_bCharacterBaseLogicStopped; 
	}
	else
	{
		if( CHARACTER_LOGIC_SKILL_PREP_SEND == CharacterLogic_Get(nLogicTag) )
			return TRUE;

		return m_bCharacterActionLogicStopped;
	}
}


//================-================
// Enter�߼�����
//================-================

//1 ���У���������߼�״̬��
BOOL CObject_Character::Start_Idle(VOID)
{
	// ���������״̬
	if( m_bJumping )
	{
		// ��������
	}
	// ������ڱ��鶯��
	else if( IsHaveChatMoodAction() )
	{
		// ����
		PlayChatMoodAction();
	}
	// �������ޱ��鶯��
	else
	{
		// ������Ϊ�ı䶯��
		BOOL bFightState, bSit;
		INT nBaseAction;
		bFightState	= IsFightState();		// ս��״̬
		bSit = GetCharacterData() ? GetCharacterData()->IsSit() : FALSE;	// �Ƿ�����
		if(bSit)
		{
			nBaseAction = BASE_ACTION_N_SIT_IDLE;
		}
		else
		{
			nBaseAction = (bFightState) ? (BASE_ACTION_F_IDLE) : (BASE_ACTION_N_IDLE);
		}

		m_eChangeAnimLogicType = LOGIC_BASE;
		m_nIdleAnimId = nBaseAction;
//		m_nBaseAnimId = nBaseAction;

		if( CT_PLAYERMYSELF == GetCharacterType() )
		{
			m_eChangeAnimLogicType = LOGIC_BASE;
		}

		BOOL bPlayAnimWhole = FALSE;//temp_fix by yangfei
		// �ı����ж���
		if(bFightState)
		{
			if(m_pRenderInterface != NULL)
			{	
				if(m_pRenderInterface->GetType() == tEntityNode::ETYPE_ACTOR)
				{
					CObjEntityActor* pActor = (CObjEntityActor*)m_pRenderInterface;
					Fairy::LogicModel* pModel = pActor->GetActorImpl();
					if(pModel)
					{
						if(pModel->getCurrentAnimName().find("����") != STRING::npos )
						{
							AxTrace(0,0,pModel->getCurrentAnimName().c_str());
							bPlayAnimWhole = TRUE;
						}
					}			
				}	
			}
		}

		if(!bPlayAnimWhole)	
			ChangeAction( nBaseAction, GetLogicSpeed(), FALSE );
	}

	// ��������
	m_paramLogic_Idle.m_uIdleInterval		= CalcIdleRandInterval();
	m_paramLogic_Idle.m_uLastIdleMotionTime	= CGameProcedure::s_pTimeSystem->GetTimeNow();
	m_paramLogic_Idle.m_uStartTime			= CGameProcedure::s_pTimeSystem->GetTimeNow();

	// ��������߼�״̬
	CharacterLogic_Set( CHARACTER_LOGIC_IDLE, LOGIC_BASE );

	return TRUE;
}

//1 �ƶ�
BOOL CObject_Character::Start_Move( CObjectCommand_Logic *pLogicCommand )
{
	// ִ�������ж�

	if( pLogicCommand == NULL )
		return FALSE;

	if( pLogicCommand->GetCommandID() != OBJECT_COMMAND_MOVE )
		return FALSE;

	CObjectCommand_Move *pMoveCommand = (CObjectCommand_Move*)pLogicCommand;
	if( pMoveCommand->GetNodeCount() <= 0 )
		return FALSE;

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;


	ENUM_CHARACTER_LOGIC ePrevLogic = CharacterLogic_Get(LOGIC_BASE);
	// �߼�״̬�����ƶ�
	if( CHARACTER_LOGIC_MOVE != ePrevLogic )
	{
		// ����Ծ״̬
		if( !m_bJumping )
		{
			BOOL bFightState;
			INT nBaseAction;
			bFightState	= IsFightState();

			if( bFightState )
			{
				nBaseAction	= BASE_ACTION_N_RUN;	// BASE_ACTION_F_RUN;
			}
			else
			{
				nBaseAction	= BASE_ACTION_N_RUN;
			}

			// �ı䶯��

			m_eChangeAnimLogicType = LOGIC_BASE;
			m_nMoveAnimId = nBaseAction;
//			m_nBaseAnimId = nBaseAction;

			// �ж������ǵĻ�
//			if( CT_PLAYERMYSELF == GetCharacterType() )
			{
				// �����ƶ�״̬Ϊ�棬 ȥ�ı�����Ķ���
				m_bMoveState = TRUE;
			}

			// �ж�������Ϊ�߼�
			if( !CharacterLogic_IsStopped(LOGIC_ACTION) )
			{
				// �Ƿ�Ϊ... �˴����ж���Ϊ�����������ʲô��Ϊ�߼�ʱ�Ÿı䶯��
//				if( CHARACTER_LOGIC_ == CharacterLogic_Get() )
				{
					// ������Ϊ�߼�ʱֻ�ı��²��Ķ���Ϊ�ƶ�
					ChangeAction( nBaseAction, GetLogicSpeed(), TRUE, ACTION_DOWN );
				}
			}
			else
			{
				ChangeAction( nBaseAction, GetLogicSpeed(), TRUE );
			}

			ChangeAction( nBaseAction, GetLogicSpeed(), TRUE );		/*GetCharacterData()->Get_SpeedRate()*/
		}
	}

	// �����ƶ�Ϊ��ǰ�߼�����
	SetBaseLogicCommand(pLogicCommand);
//	SetLogicCount( pLogicCommand->GetLogicCount() );
	SetBaseLogicCount( pLogicCommand->GetLogicCount() );

	// �������һ�θ���λ�õ��߼���
	SetLastModifyPosLogicCount(pLogicCommand->GetLogicCount());

	// �������ݣ��ƶ��ڵ��
	const WORLD_POS *pNodeList = pMoveCommand->GetNodeList();
	m_paramLogic_Move.m_nCurrentNodeIndex	= 0;
	m_paramLogic_Move.m_posSaveStart		= WORLD_POS(GetPosition().x, GetPosition().z);
	m_paramLogic_Move.m_posSaveTarget		= pNodeList[0];

	// �����߼�״̬
	CharacterLogic_Set( CHARACTER_LOGIC_MOVE, LOGIC_BASE );

	// �����ƶ���������������Ч���ش�ʱ		20100714 BLL
	STRING strRet;
	CVariableSystem::GetMe()->GetVariable( CSoundSystemFMod::szSKSoundVar, strRet );
	if( !m_pWlakSound && strRet == "1" )
	{
		m_pWlakSound = CGameProcedure::s_pSoundSystem->Source_Create( CSoundSourceFMod::type_skill, FALSE, FALSE, &m_pWlakSound );
		KLAssert(m_pWlakSound);
	}

	if (m_pRenderInterface && g_theKernel.IsKindOf(this->GetClass(), GETCLASS(CObject_PlayerOther)))
	{
		m_pRenderInterface->AddAreaEffect("<template>");
	}

	return TRUE;
}

//1 �����
BOOL CObject_Character::Start_Ability( CObjectCommand_Logic *pLogicCommand )
{
	if(pLogicCommand == NULL)
		return FALSE;

	if(pLogicCommand->GetCommandID() != OBJECT_COMMAND_ABILITY)
		return FALSE;

	if ( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_Ability *pAbilityCommand = (CObjectCommand_Ability*)pLogicCommand;

	// ����ȡ������
	DBC_DEFINEHANDLE(s_pAbilityDbc, DBC_LIFEABILITY_DEFINE);
	const _DBC_LIFEABILITY_DEFINE* pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)s_pAbilityDbc->Search_Index_EQU(pAbilityCommand->GetAbilityID());
	if(pAbilityDef == NULL)
		return FALSE;

	UINT uTotalTime = 0;
	if(pAbilityCommand->GetPrescriptionID() >= 0)
	{
		DBC_DEFINEHANDLE(s_pPrescrDBC, DBC_LIFEABILITY_ITEMCOMPOSE);
		const _DBC_LIFEABILITY_ITEMCOMPOSE* pDefine = 
			(const _DBC_LIFEABILITY_ITEMCOMPOSE*)s_pPrescrDBC->Search_Index_EQU(pAbilityCommand->GetPrescriptionID());

		// ��¼��ʼʱ����ܹ���Ҫ��ʱ��
		if(pDefine != NULL && pDefine->nProficiencyTime > 0)
			uTotalTime = (UINT)(pDefine->nProficiencyTime);
	}

	if(uTotalTime == 0)
	{
		// ��¼��ʼʱ����ܹ���Ҫ��ʱ��
		if(pAbilityDef->nTimeOperation > 0)
			uTotalTime = (UINT)(pAbilityDef->nTimeOperation);
	}

	if(uTotalTime == 0)
		return FALSE;

	// ����
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( pAbilityCommand->GetTargetObjID() ));
	if(pTarget != NULL && pTarget != this)
	{
		fVector3 fvTarget = pTarget->GetPosition();
		fVector2 fvThis;
		fVector2 fvTarget2;

		fvThis.x = GetPosition().x;
		fvThis.y = GetPosition().z;
		fvTarget2.x = fvTarget.x;
		fvTarget2.y = fvTarget.z;

		FLOAT fDir = KLU_GetYAngle(fvThis, fvTarget2);
		SetFaceDir( fDir );
	}

	//// ��������
	//if(GetCharacterType() == CT_PLAYEROTHER || GetCharacterType() == CT_PLAYERMYSELF)
	//	((CObject_PlayerOther*)this)->EquipItem_BodyLocator( HEQUIP_WEAPON, pAbilityDef->nItemVisualLocator);

	m_eChangeAnimLogicType = LOGIC_BASE;
//	m_nBaseAnimId = pAbilityDef->nAnimOperation;

	// �������ﶯ��
	ChangeAction( pAbilityDef->nAnimOperation, GetLogicSpeed(), TRUE );

	// �������������
	if(CObjectManager::GetMe()->GetMySelf() == this)
	{
		// ������������������Ч��ʱ		2010714 BLL
		STRING strRet;
		CVariableSystem::GetMe()->GetVariable( CSoundSystemFMod::szSKSoundVar, strRet );
		if( !m_pLifeAbilitySound && strRet == "1" )
		{
			m_pLifeAbilitySound = CGameProcedure::s_pSoundSystem->Source_Create(CSoundSourceFMod::type_skill, FALSE, FALSE, &m_pLifeAbilitySound );
			KLAssert(m_pLifeAbilitySound);

		}

		// ��������ܿ�ʼ���Ų�ͬ����
		switch(pAbilityCommand->GetAbilityID())
		{
		case 1:			//���
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(16+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
		case 2:			//��ҽ
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(17+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 3:			//�ӹ�
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(20+59));//����18
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 4:			//����
		case 5:			//����
		case 6:			//��������
		case 7:			//����ѧ
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(18+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 8:			//�ɿ�
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(12+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 9:			//��ҩ
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(10+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 10:		//����
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(14+59));
			m_pLifeAbilitySound->SetLooping(FALSE);
			m_pLifeAbilitySound->Play();
			break;
		default:		//...
			break;
		}

	}

	SetBaseLogicCommand(pLogicCommand);
//	SetLogicCount(pLogicCommand->GetLogicCount());
	SetBaseLogicCount(pLogicCommand->GetLogicCount());

	m_paramLogic_Ability.m_uCurrentTime		= 0;
	m_paramLogic_Ability.m_uTotalTime		= uTotalTime;

	// ������
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pAbilityDef->szProgressbarName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 0);
	}

	// ���������߼�Ϊ������ܽ�����...��
	CharacterLogic_Set( CHARACTER_LOGIC_ABILITY_ACTION, LOGIC_BASE );

	return TRUE;
}

//1 ��̯
BOOL CObject_Character::Start_Stall( BOOL bPlayAni )
{
	m_eChangeAnimLogicType = LOGIC_BASE;
//	m_nBaseAnimId = BASE_ACTION_N_SIT_DOWN;

	// �ı䶯��
	ChangeAction( BASE_ACTION_N_SIT_DOWN, GetLogicSpeed(), FALSE );

	m_paramLogic_Stall.m_nCurrentAnimation	= SLogicParam_Stall::STALL_ANIMATION_SITDOWN;

	CharacterLogic_Set( CHARACTER_LOGIC_STALL, LOGIC_BASE );
	return TRUE;
}

//1 ����
BOOL CObject_Character::Start_Dead( BOOL bPlayDieAni )	// �� GCNewMonster_Death ��Ϣʱ�β����� FALSE
{
	// ֹͣ�߼��¼�
	CharacterLogic_Stop(FALSE, LOGIC_BASE);
	CharacterLogic_Stop(FALSE, LOGIC_ACTION);
	// ����߼������б�
	CleanupLogicCommandList(LOGIC_BASE);
	CleanupLogicCommandList(LOGIC_ACTION);


	//// ���������
	//if(m_bDropBox_HaveData)
	//{
	//	// ����ItemBox
	//	CTripperObject_ItemBox* pBox = (CTripperObject_ItemBox*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemBox", m_nDropBox_ItemBoxID);
	//	pBox->Initial(NULL);	
	//	// ����λ��
	//	pBox->SetMapPosition( fVector2(m_posDropBox_CreatePos.m_fX, m_posDropBox_CreatePos.m_fZ));
	//	// ���õ�����Ĺ���
	//	pBox->SetOwnerGUID(m_DropBox_OwnerGUID);

	//	m_bDropBox_HaveData = FALSE;
	//}

	//// ���������

	//if(m_bDropItem_HaveData)
	//{

	//	// ����������Ʒ
	//	CTripperObject_ItemDrop* pDrop = (CTripperObject_ItemDrop*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemDrop", m_nDropItem_ItemDropID);
	//	pDrop->SetDropCanPick(m_bIsCanPick);
	//	
	//    pDrop->Initial(NULL);	

	//	// ����λ��
	//	pDrop->SetMapPosition( fVector2(m_posDropItem_CreatePos.m_fX, m_posDropItem_CreatePos.m_fZ));

 //
	//	// ���õ�����Ĺ���
	//	pDrop->SetOwnerGUID(m_DropItem_OwnerObjId);

	//	pDrop->SetProtectTime(m_ProtectTime);
	//	pDrop->SetStartTime(timeGetTime());
	//	m_bDropItem_HaveData = FALSE;
	//}

	// ��������λ��
	m_paramLogic_Dead.m_DeadPos = GetPosition();
	m_paramLogic_Dead.m_CurrPos = m_paramLogic_Dead.m_DeadPos;

	// ����Ŀ��λ��
	fVector3 targetPos(0, 0, 0);
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CObject_Character* obj = (CObject_Character*)CObjectManager::GetMe()->GetMainTarget();
		if(obj)
			targetPos = obj->GetPosition();
	}
	else
	{
		targetPos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
	}

	// ���㷽��
	m_paramLogic_Dead.m_TarDir = m_paramLogic_Dead.m_DeadPos - targetPos;
	m_paramLogic_Dead.m_TarDir.normalise();

	// ����λ��
	fVector3 vFlyLength;
	vFlyLength.x = m_paramLogic_Dead.m_TarDir.x * m_fBeatFlyDist;		// �˾���ӱ��ж����� ͨ��������������
	vFlyLength.y = m_paramLogic_Dead.m_TarDir.y * m_fBeatFlyDist;
	vFlyLength.z = m_paramLogic_Dead.m_TarDir.z * m_fBeatFlyDist;

	m_paramLogic_Dead.m_TarPos = vFlyLength + m_paramLogic_Dead.m_DeadPos;
	m_paramLogic_Dead.m_bDeadIsEnd = FALSE;
	m_paramLogic_Dead.m_bDeadFlyBegin = FALSE;
	m_paramLogic_Dead.m_bDeadFlyEnd = FALSE;
	m_paramLogic_Dead.m_bDeadBegin	= FALSE;
	m_paramLogic_Dead.m_bDeadEnd	= FALSE;

	m_paramLogic_Dead.m_nCadaverAction = -1;


	// ��������
	Dismount();

	// �����߼�״̬
	CharacterLogic_Set( CHARACTER_LOGIC_DEAD, LOGIC_BASE );


	INT nBaseAction = BASE_ACTION_INVALID;


//	����_01=ʬ��_01
//	����_02=ʬ��_02
//
//	����_01=ʬ��_03
//	����_02=ʬ��_04


	// �Ƿ񲥷���������
//	if( bPlayDieAni )
	if( !m_bBeatFly )
	{
		m_eChangeAnimLogicType = LOGIC_BASE;
		nBaseAction = rand() % 2 + BASE_ACTION_F_DIE0;
		ChangeAction( nBaseAction, GetLogicSpeed(), FALSE );

		m_paramLogic_Dead.m_bDeadBegin	= TRUE;

		if( BASE_ACTION_F_DIE0 == nBaseAction )
			m_paramLogic_Dead.m_nCadaverAction = BASE_ACTION_F_CADAVER2;
		else
			m_paramLogic_Dead.m_nCadaverAction = BASE_ACTION_F_CADAVER3;
	}
	// �Ƿ����
	else
	{
		nBaseAction = rand() % 2 + BASE_ACTION_F_DIEFLY0;
		// ������Ż��ɶ���
		ChangeAction( nBaseAction, GetLogicSpeed(), FALSE, ACTION_BODY );		// , 0.0f

		m_paramLogic_Dead.m_bDeadFlyBegin = TRUE;

		if( BASE_ACTION_F_DIEFLY0 == nBaseAction )
			m_paramLogic_Dead.m_nCadaverAction = BASE_ACTION_F_CADAVER0;
		else
			m_paramLogic_Dead.m_nCadaverAction = BASE_ACTION_F_CADAVER1;
	}

	// ���ս��״̬
	SetFightState( FALSE );
	// ��Ǹ���С��ͼ
	CGameProcedure::m_bWaitNeedFreshMinimap = true;

	// ����ѡ�����ȼ�
	if(m_pRenderInterface)
	{
		if(CT_PLAYEROTHER == GetCharacterType())
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLAYER_DEADBODY);
		else
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_CREATURE_DEADBODY);
	}

	// ������е��˺���Ϣ
	RemoveAllImpact();

	if(this == CObjectManager::GetMe()->GetMySelf())
	{
		STRING strTemp = COLORMSGFUNC("DIE_YOU_DIED_MSG");
		ADDTALKMSG(strTemp);
	}

	return TRUE;
}



//2 ������
BOOL CObject_Character::Start_Action( CObjectCommand_Logic *pLogicCommand )
{
	// ִ�������ж�

	if( pLogicCommand == NULL )
		return FALSE;

	if( pLogicCommand->GetCommandID() != OBJECT_COMMAND_ACTION )
		return FALSE;

	CObjectCommand_Action *pActionCommand = (CObjectCommand_Action*)pLogicCommand;
	if( pActionCommand->GetActionID() == INVALID_ID )
		return FALSE;

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;

	// �ڲ�������״̬�²ſ��Զ���
	if( !m_bJumping )
	{
		m_eChangeAnimLogicType = LOGIC_ACTION;
//		m_nActionAnimId	= pActionCommand->GetActionID();
		ChangeAction( pActionCommand->GetActionID(), GetLogicSpeed(), FALSE );
	}

	// ���õ�ǰ�߼�����
	SetActionLogicCommand( pLogicCommand );
//	SetLogicCount( pLogicCommand->GetLogicCount() );
	SetActionLogicCount( pLogicCommand->GetLogicCount() );

	// ��Ƕ����߼�״̬
	CharacterLogic_Set( CHARACTER_LOGIC_ACTION, LOGIC_ACTION );

	return TRUE;
}

//2 ���ܾ���
BOOL CObject_Character::Start_MagicCharge( CObjectCommand_Logic *pLogicCommand )
{
	// ִ�������ж�

	if( pLogicCommand == NULL )
		return FALSE;

	if( pLogicCommand->GetCommandID() != OBJECT_COMMAND_MAGIC_CHARGE )
		return FALSE;

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_MagicCharge *pMagicChargeCommand = (CObjectCommand_MagicCharge*)pLogicCommand;
	if(pMagicChargeCommand->GetTotalTime() == 0)
		return FALSE;

	// ȡ�ü�������
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(pMagicChargeCommand->GetMagicID());
	if( pClientSkill == NULL )
		return FALSE;

	// ͨ�����ܵ����ѡ����������������ĳ���
	switch( pClientSkill->m_nSelectType )
	{
	case SELECT_TYPE_CHARACTER:
	case SELECT_TYPE_POS:
		{
			BOOL bCalcDir;
			fVector2 fvTarget;
			bCalcDir = FALSE;
			if( pMagicChargeCommand->GetTargetObjID() != INVALID_ID )
			{
				if ( pMagicChargeCommand->GetTargetObjID() != GetServerID() )
				{
					CObjectManager* pObjectManager = CObjectManager::GetMe();
					CObject* pTarget = (CObject*)(pObjectManager->FindServerObject( pMagicChargeCommand->GetTargetObjID() ));
					if ( pTarget != NULL )
					{
						fvTarget.x = pTarget->GetPosition().x;
						fvTarget.y = pTarget->GetPosition().z;
						bCalcDir = TRUE;
					}
				}
			}
			else
			{
				fvTarget.x = pMagicChargeCommand->GetTargetPos()->m_fX;
				fvTarget.y = pMagicChargeCommand->GetTargetPos()->m_fZ;
				bCalcDir = TRUE;
			}

			if( bCalcDir )
			{
				FLOAT fDir;
				fVector2 fvThis;
				fvThis.x = GetPosition().x;
				fvThis.y = GetPosition().z;

				fDir = KLU_GetYAngle( fvThis, fvTarget );
				SetFaceDir( fDir );
			}
		}
		break;

	case SELECT_TYPE_DIR:
		SetFaceDir(pMagicChargeCommand->GetTargetDir());
		break;

	case SELECT_TYPE_NONE:
		break;
	}

	// �ı䶯��
	INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(pMagicChargeCommand->GetMagicID(), 0, FALSE);
	if( nRandAnimID != -1 )
	{
		m_eChangeAnimLogicType = LOGIC_ACTION;
//		m_nActionAnimId	= nRandAnimID;
//		ChangeAction( nRandAnimID, GetLogicSpeed(), TRUE );

		BOOL bAnimLoop = TRUE;
		// �жϻ����߼�
		if( !CharacterLogic_IsStopped(LOGIC_BASE) )
		{
			// �Ƿ�Ϊ�ƶ�
			if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
			{
				// �����ƶ�ֻ�ı��ϲ�����
				ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
			}
			else 
			{
				ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
			}
		}
		// �޻����߼�
		else
		{
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}
	}

	// ��ʾ����
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pClientSkill->m_lpszName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 0);
	}

	// ���õ�ǰ���߼�����
	SetActionLogicCommand(pLogicCommand);
//	SetLogicCount(pLogicCommand->GetLogicCount());
	SetActionLogicCount(pLogicCommand->GetLogicCount());

	// ��ʱ
	m_paramLogic_MagicCharge.m_nSkillID			= pMagicChargeCommand->GetMagicID();
	m_paramLogic_MagicCharge.m_uCurrentTime		= 0;

	if(pClientSkill->m_nFriendness < 0)
	{
		SetFightState(TRUE);
	}

	// �����߼�״̬
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_GATHER, LOGIC_ACTION );

	return TRUE;
}

//2 ��������
BOOL CObject_Character::Start_MagicChannel( CObjectCommand_Logic *pLogicCommand )
{
	if( pLogicCommand == NULL )
		return FALSE;

	if( pLogicCommand->GetCommandID() != OBJECT_COMMAND_MAGIC_CHANNEL)
		return FALSE;

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_MagicChannel *pMagicChannelCommand = (CObjectCommand_MagicChannel*)pLogicCommand;
	if( pMagicChannelCommand->GetTotalTime() == 0)
		return FALSE;

	// �������� 
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(pMagicChannelCommand->GetMagicID());
	if( pClientSkill == NULL )
		return FALSE;

	// �������ѡ�����͸ı����ﳯ��
	switch ( pClientSkill->m_nSelectType )
	{
	case SELECT_TYPE_CHARACTER:
	case SELECT_TYPE_POS:
		{
			BOOL bCalcDir;
			fVector2 fvTarget;
			bCalcDir = FALSE;
			if ( pMagicChannelCommand->GetTargetObjID() != INVALID_ID )
			{
				if ( pMagicChannelCommand->GetTargetObjID() != GetServerID() )
				{
					CObjectManager* pObjectManager = CObjectManager::GetMe();
					CObject* pTarget = (CObject*)(pObjectManager->FindServerObject( pMagicChannelCommand->GetTargetObjID() ));
					if ( pTarget != NULL )
					{
						fvTarget.x = pTarget->GetPosition().x;
						fvTarget.y = pTarget->GetPosition().z;
						bCalcDir = TRUE;
					}
				}
			}
			else
			{
				fvTarget.x = pMagicChannelCommand->GetTargetPos()->m_fX;
				fvTarget.y = pMagicChannelCommand->GetTargetPos()->m_fZ;
				bCalcDir = TRUE;
			}
			if ( bCalcDir )
			{
				FLOAT fDir;
				fVector2 fvThis;
				fvThis.x = GetPosition().x;
				fvThis.y = GetPosition().z;

				fDir = KLU_GetYAngle( fvThis, fvTarget );
				SetFaceDir( fDir );
			}
		}
		break;
	case SELECT_TYPE_DIR:
		SetFaceDir(pMagicChannelCommand->GetTargetDir());
		break;
	case SELECT_TYPE_NONE:
		break;
	}

	// �ı䶯��
	INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(pMagicChannelCommand->GetMagicID(), 0, FALSE);
	if( nRandAnimID != -1 )
	{
		m_eChangeAnimLogicType = LOGIC_ACTION;	
//		m_nActionAnimId	= nRandAnimID;
//		ChangeAction( nRandAnimID, GetLogicSpeed(), TRUE );

		BOOL bAnimLoop = TRUE;
		// �жϻ����߼�
		if( !CharacterLogic_IsStopped(LOGIC_BASE) )
		{
			// �Ƿ�Ϊ�ƶ�
			if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
			{
				// �����ƶ�ֻ�ı��ϲ�����
				ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
			}
			else 
			{
				ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
			}
		}
		// �޻����߼�
		else
		{
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}
	}

	// ��ʾ����
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pClientSkill->m_lpszName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 1.f);
	}

	// ���õ�ǰ���߼�����
	SetActionLogicCommand(pLogicCommand);
//	SetLogicCount(pLogicCommand->GetLogicCount());
	SetActionLogicCount(pLogicCommand->GetLogicCount());

	// ��¼ʱ��
	m_paramLogic_MagicChannel.m_nSkillID	 = pMagicChannelCommand->GetMagicID();
	m_paramLogic_MagicChannel.m_uCurrentTime = pMagicChannelCommand->GetTotalTime();

	if(pClientSkill->m_nFriendness < 0)
	{
		SetFightState(TRUE);
	}

	// �ı��߼�״̬
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_LEAD, LOGIC_ACTION );

	return TRUE;
}

//test2
BOOL CObject_Character::Start_MagicPrepSend( CObjectCommand_Logic *pLogicCommand )
{
	if( CT_PLAYERMYSELF == GetCharacterType() )
	{
		if( pLogicCommand == NULL )
			return FALSE;

	}

	if( pLogicCommand == NULL )
		return FALSE;

	if( pLogicCommand->GetCommandID() != OBJECT_COMMAND_MAGIC_PREP_SEND )
		return FALSE;

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;


	CObjectCommand_MagicPrepSend *pMagicPrepSendCommand = (CObjectCommand_MagicPrepSend*)pLogicCommand;


	INT iMagicId = pMagicPrepSendCommand->GetMagicID();

//	if ( pMagicSendCommand->GetTargetObjID() != INVALID_ID )



	// ��ȡ�������� 
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( iMagicId );
	if( pClientSkill == NULL )
		return FALSE;


	// �ı䶯��Ϊ���ܷ���Ԥ��

	INT nRandAnimID = BASE_ACTION_FP;	// GetSkillDataMgr()->GetRandAnim( m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE );
	BOOL bAnimLoop = FALSE;

	// �жϻ����߼�
	if( !CharacterLogic_IsStopped(LOGIC_BASE) )
	{
		// �Ƿ�Ϊ�ƶ�
		if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
		{
			// �����ƶ�ֻ�ı��ϲ�����
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
		}
		else 
		{
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}
	}
	// �޻����߼�
	else
	{
		ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
	}


	SetActionLogicCommand( pLogicCommand );
	SetActionLogicCount( pLogicCommand->GetLogicCount() );

	// �����߼�״̬
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_PREP_SEND, LOGIC_ACTION );


	return TRUE;
}

//2 ���ܷ���
BOOL CObject_Character::Start_MagicSend( CObjectCommand_Logic *pLogicCommand )
{
	// ִ�������ж�
	//{
	if( pLogicCommand == NULL )
		return FALSE;

	if( pLogicCommand->GetCommandID() != OBJECT_COMMAND_MAGIC_SEND )
		return FALSE;

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;
	//}

	CObjectCommand_MagicSend *pMagicSendCommand = (CObjectCommand_MagicSend*)pLogicCommand;

	INT iMagicId = pMagicSendCommand->GetMagicID();

	AxTrace(0,2,"Use Skill Send ID = %d LogicID = %d  CurrentLogicID = %d",iMagicId, pLogicCommand->GetLogicCount(), GetActionLogicCount());

	// ��ȡ�������� 
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( iMagicId );
	if( pClientSkill == NULL )
		return FALSE;

	// ���ݼ��ܵ����ѡ����������������ĳ���
	switch ( pClientSkill->m_nSelectType )
	{
	case SELECT_TYPE_CHARACTER:
		{
			if (pMagicSendCommand->GetTargetObjID(0) != GetServerID())
			{
				CObjectManager* pObjectManager = CObjectManager::GetMe();
				CObject* pTarget = (CObject*)(pObjectManager->FindServerObject( pMagicSendCommand->GetTargetObjID(0) ));
				if ( pTarget != NULL )
				{
					FLOAT fDir;
					fVector2 fvThis;
					fVector2 fvTarget;

					//�����ɫ�Ѿ���ȥ��,��ô�Ͳ���������
					if ( ((CObject_Character*)pTarget)->IsDie() )
					{
						return FALSE;
					}

					fvTarget.x = pTarget->GetPosition().x;
					fvTarget.y = pTarget->GetPosition().z;

					fvThis.x = GetPosition().x;
					fvThis.y = GetPosition().z;

					fDir = KLU_GetYAngle( fvThis, fvTarget );
					SetFaceDir( fDir );
				}
				else
				{
					//�Ҳ������Ŀ��Ļ�,Ҳ��������.
					return FALSE;
				}
			}
		}
		break;
	case SELECT_TYPE_POS:
		{
			FLOAT fDir;
			fVector2 fvThis;
			fVector2 fvTarget;

			fvTarget.x = pMagicSendCommand->GetTargetPos()->m_fX;
			fvTarget.y = pMagicSendCommand->GetTargetPos()->m_fZ;

			fvThis.x = GetPosition().x;
			fvThis.y = GetPosition().z;

			fDir = KLU_GetYAngle( fvThis, fvTarget );
			SetFaceDir( fDir );			
		}
		break;
	case SELECT_TYPE_DIR:
		SetFaceDir(pMagicSendCommand->GetTargetDir());
		break;
	case SELECT_TYPE_NONE:
		break;
	default:
		break;
	}

	// ��������
	m_paramLogic_MagicSend.m_nSaveMagicID					= pMagicSendCommand->GetMagicID();
	m_paramLogic_MagicSend.m_bDoNextAction_Concatenation	= FALSE;
	m_paramLogic_MagicSend.m_bBeAttackEffectShowed			= FALSE;
	m_paramLogic_MagicSend.m_bCanBreak						= FALSE;
	m_paramLogic_MagicSend.m_nActionCount					= GetSkillDataMgr()->GetAnimCount( iMagicId );
	m_paramLogic_MagicSend.m_uAnimationTime					= 0;
	m_paramLogic_MagicSend.m_uAnimationEndElapseTime		= 0;

	BOOL bOldSkillID = FALSE;
	// �����߼�����
	switch (CharacterLogic_Get(LOGIC_ACTION))
	{
		// ����
	case CHARACTER_LOGIC_SKILL_GATHER:
		{
			CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_HIDE);
		}
		break;
		// ����
	case CHARACTER_LOGIC_SKILL_LEAD:
		{
		}
		break;
		// ������
	case CHARACTER_LOGIC_SKILL_SEND:
		{
			// �ж��Ƿ�Ϊ��һ��ʹ�õ�����, �ϴ�ʹ�õļ��ܵ�id������������
			if (m_paramLogic_MagicSend.m_nSaveMagicID == iMagicId )	
			{
				bOldSkillID = TRUE;
			}
		}
		break;
	default:
		break;
	}


	// ��������е����⴦��
	BOOL bPlayAction = TRUE;
	INT nSkillActionType = pClientSkill->m_nSkillActionType;
	switch( nSkillActionType )
	{
	case SKILL_ACTION_TYPE_CONCATENATION:
		{
			if( bOldSkillID )
			{
				m_paramLogic_MagicSend.m_nActionIndex++;
				if( m_paramLogic_MagicSend.m_nActionIndex >= m_paramLogic_MagicSend.m_nActionCount )
				{
					m_paramLogic_MagicSend.m_nActionIndex = 0;
				}

				bPlayAction = FALSE;
			}
			else
			{
				m_paramLogic_MagicSend.m_nActionIndex = 0;
			}
		}
		break;

	case SKILL_ACTION_TYPE_CONCATENATION_EX:
		if( bOldSkillID )
		{
			m_paramLogic_MagicSend.m_nActionIndex++;
			if( m_paramLogic_MagicSend.m_nActionIndex >= m_paramLogic_MagicSend.m_nActionCount - 1 )
			{
				m_paramLogic_MagicSend.m_nActionIndex = 1;
			}

			bPlayAction = FALSE;
		}
		else
		{
			m_paramLogic_MagicSend.m_nActionIndex = 0;
		}
		break;

	case SKILL_ACTION_TYPE_NONE:
	default:
		{
			if( m_paramLogic_MagicSend.m_nActionCount > 0 )
				m_paramLogic_MagicSend.m_nActionIndex = (::rand())%m_paramLogic_MagicSend.m_nActionCount;
			else
				m_paramLogic_MagicSend.m_nActionIndex = 0;
		}		
		break;
	}


	// �ı䶯��
	if( bPlayAction || IsActionAnimationEnd() )
	{
		INT nRandAnimID = GetSkillDataMgr()->GetRandAnim( m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE );


		BOOL bAnimLoop = FALSE;
		BOOL bChangeOK = FALSE;

		m_eChangeAnimLogicType = LOGIC_ACTION;
		
		// �жϻ����߼�
		if( !CharacterLogic_IsStopped(LOGIC_BASE) )
		{
			// �Ƿ�Ϊ�ƶ�
			if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
			{
				// �����ƶ�ֻ�ı��ϲ�����
				bChangeOK = ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
			}
			else 
			{
				bChangeOK = ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
			}
		}
		// �޻����߼�
		else
		{
			bChangeOK = ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}	

		if (TRUE == bChangeOK)
		{
			Fairy::Skill* pSkill = m_pRenderInterface->GetCurrentSkill();
			if (pSkill)
			{
				// ���ܴ����ӵ�Ч��
				unsigned short  nFlowNum = pSkill->getNumAnimationBulletFlows();
				if (nFlowNum > 0)
				{
					const ObjID_t* pTargetIDList = pMagicSendCommand->GetTargetObjID();
					INT nTargetNum = pMagicSendCommand->GetTargetNum();
					if(nTargetNum> 0)// ���������Ŀ���б���Ϣ
					{
						CBulletEntityManager::GetMe()->CreateSkillBulletInfo(pSkill, GetServerID(), pTargetIDList, nTargetNum);
					}
				}
			}
		}
	}
	else
	{
		m_paramLogic_MagicSend.m_bDoNextAction_Concatenation = TRUE;
	}	


	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		// ��Ʒʹ�ò����������ļ��ܹ���cooldowm 
		if( !pClientSkill->m_bAutoRedo && pClientSkill->m_nClassByUser != 3 )	// �Զ������ͷ� �� ʹ�������ͷ���
			CActionSystem::GetMe()->UpdateCommonCoolDown(INVALID_ID);
	}

	// ���ü��ܷ���Ϊ��ǰ���߼�����
	SetActionLogicCommand( pLogicCommand );
//	SetLogicCount( pLogicCommand->GetLogicCount() );
	SetActionLogicCount( pLogicCommand->GetLogicCount() );

	// ���漼��,����ս��״̬
	if( pClientSkill->m_nFriendness < 0 )
	{
		SetFightState( TRUE );
	}

	// �����߼�״̬
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_SEND, LOGIC_ACTION );

//	m_UseSkillTag = TRUE;

	return TRUE;
}




//================-================
// �߼�Tick����
//================-================

//1 ����
BOOL CObject_Character::Tick_Idle( UINT uElapseTime )
{
	m_paramLogic_Idle.m_uStartTime += uElapseTime;
	if(GetCharacterData() && GetCharacterData()->IsSit() && IsHaveChatMoodAction())
	{
		FreshAnimation();
	}

	return TRUE;
}

//1 �ƶ�
BOOL CObject_Character::Tick_Move( UINT uElapseTime )
{
	CObjectCommand_Move *pMoveCommand = (CObjectCommand_Move*)GetBaseLogicCommand();

	if(pMoveCommand == NULL || !GetCharacterData())
	{
		return FALSE;
	}
 
	if( pMoveCommand->GetNodeCount() <= 0 )
	{
		
		return FALSE;
	}

	
	const WORLD_POS *paPos = pMoveCommand->GetNodeList();

	fVector2 fvStartPos2D(m_paramLogic_Move.m_posSaveStart.m_fX, m_paramLogic_Move.m_posSaveStart.m_fZ);
	fVector2 fvCurrentPos2D(GetPosition().x, GetPosition().z);
	fVector2 fvTargetPos2D(paPos[m_paramLogic_Move.m_nCurrentNodeIndex].m_fX, paPos[m_paramLogic_Move.m_nCurrentNodeIndex].m_fZ);

	// ��ǰλ���뵱ǰ��Ŀ��·����·�����ȵ�ƽ��
	fVector2 fvDistToTarget = fvTargetPos2D - fvCurrentPos2D;
	FLOAT fDistToTargetSq = fvDistToTarget.x * fvDistToTarget.x + fvDistToTarget.y * fvDistToTarget.y;
	FLOAT fDistToTarget = sqrt(fDistToTargetSq) ;

	// ��һ֡���ƶ���·������
	FLOAT fElapseTime	= ((FLOAT)(uElapseTime))/1000.0f;
	FLOAT fSpeed		= GetCharacterData()->Get_MoveSpeed();

	//616======* ���������ƶ��ٶ�
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1)
	{
		FLOAT tmp = CGameProcedure::s_pVariableSystem->GetAs_Float("GameMoveSpeed");

		if(fSpeed < 0.01f)
		{
			fSpeed = tmp;
		}
	}//

	FLOAT fMoveDist		= fSpeed * fElapseTime;

	if(fMoveDist < 0.01f)
	{
	//�޸���ɫ���߻�ͻȻͣ������BUG LAIM ---��	return FALSE;
	}


	if(fDistToTarget < 0.01f)
	{
		// �ж������ǵĻ�, �����ƶ�״̬Ϊ�棬 ȥ�ı�����Ķ���
//		if( CT_PLAYERMYSELF == GetCharacterType() )
		{
			m_bMoveState = FALSE;
		}

		// 20100415 AddCodeBegin
		// �Զ�Ѱ·�������������������б����󣬴�NPC�Ի���

		QuestOpenNPCDlg();

		// 20100415 AddCodeEnd

		CharacterLogic_Stop(TRUE, LOGIC_BASE);
		return TRUE;
	}

	BOOL bStopMove = FALSE;
	fVector2 fvSetToPos = fvCurrentPos2D;
	FLOAT fSetToDir = GetFaceDir();
	while(true)
	{
		if(fMoveDist > fDistToTarget)
		{
			m_paramLogic_Move.m_nCurrentNodeIndex++;
			if(m_paramLogic_Move.m_nCurrentNodeIndex >= pMoveCommand->GetNodeCount())
			{
				// �ߵ���
				bStopMove	= TRUE;
				fvSetToPos	= fvTargetPos2D;
				fSetToDir	= KLU_GetYAngle(fvCurrentPos2D, fvTargetPos2D);
				break;
			}
			else
			{
				// �ı�m_paramLogic_Move�еĸ���ֵ
				fMoveDist		-= fDistToTarget;
				fvStartPos2D	= fvTargetPos2D;
				fvCurrentPos2D	= fvTargetPos2D;
				fvTargetPos2D	= fVector2(paPos[m_paramLogic_Move.m_nCurrentNodeIndex].m_fX, paPos[m_paramLogic_Move.m_nCurrentNodeIndex].m_fZ);

				m_paramLogic_Move.m_posSaveStart.m_fX	= fvStartPos2D.x;
				m_paramLogic_Move.m_posSaveStart.m_fZ	= fvStartPos2D.y;
				m_paramLogic_Move.m_posSaveTarget.m_fX	= fvTargetPos2D.x;
				m_paramLogic_Move.m_posSaveTarget.m_fZ	= fvTargetPos2D.y;
			}
		}
		else
		{
			FLOAT fDistX = (fMoveDist*(fvTargetPos2D.x-fvCurrentPos2D.x))/fDistToTarget;
			FLOAT fDistZ = (fMoveDist*(fvTargetPos2D.y-fvCurrentPos2D.y))/fDistToTarget;

			fvSetToPos.x = fvCurrentPos2D.x + fDistX;
			fvSetToPos.y = fvCurrentPos2D.y + fDistZ;

			// ��Ҫλ����������ֹ��Ϊ��������赲���ڲ�
			if( CObjectManager::GetMe()->GetMySelf() == this && CPath::IsPointInUnreachRegion(fvSetToPos))
			{
				fvSetToPos = KLU_GetReflect(fvStartPos2D, fvTargetPos2D, fvSetToPos);
			}

			fSetToDir = KLU_GetYAngle(fvCurrentPos2D, fvTargetPos2D);
			break;
		}
	}

	SetFaceDir(fSetToDir);
	SetMapPosition( fvSetToPos );

	
	int soundType = -1;
	if(m_pRenderInterface != NULL)
	{
		soundType = m_pRenderInterface->GetTerrainSoundType();
	}

	// �жϵ���������Ƿ����仯�� ����Ͳ��ı������� ����ѭ��
	bool bstate = false;
	if(mTerrainType != soundType)
	{
		mTerrainType = soundType;
		if(-1 != mTerrainType)
			bstate = true;
	}

	// ���ݵ��沥�Ų�ͬ������ֻ�������ǵ�������
	if(!bStopMove && (CObjectManager::GetMe()->GetMySelf() == this))	// && bstate 
	{
		INT id = -1;
		switch(soundType)
		{
		// ��ͨ����
		case GENERAL_TER:
				id = 0;
				break;
		// �ݵ�
		case GRASS_TER:
				id = 1;
				break;
		// ����
		case GROUND_TER:
				id = 2;
				break;
		// ѩ��
		case SNOW_TER:
				id = 3;
				break;
		// ˮ
		case WATER_TER:
				id = 4;
				break;
		// ʯͷ
		case ROCK_TER:
				id = 5;
				break;
		}

		if(mPlayState || bstate)
		{
			if((-1 != id) && (id < MAX_SOUND_NUMBER))
			{
				if ( m_pWlakSound != NULL )
				{
					m_pWlakSound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(m_SoundId[id]));
					m_pWlakSound->SetLooping(TRUE);
					m_pWlakSound->Play();
				}
			}
		}

		mPlayState = false;

	}


	if(bStopMove)
	{
		// �ж������ǵĻ�, �����ƶ�״̬Ϊ�棬 ȥ�ı�����Ķ���
//		if( CT_PLAYERMYSELF == GetCharacterType() )
		{
			m_bMoveState = FALSE;
		}

		// 20100415 AddCodeBegin
		// �Զ�Ѱ·�������������������б����󣬴�NPC�Ի���

		QuestOpenNPCDlg();

		// 20100415 AddCodeEnd

		CharacterLogic_Stop(TRUE, LOGIC_BASE);

		if (m_pRenderInterface&& g_theKernel.IsKindOf(this->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			m_pRenderInterface->RemoveAreaEffect("<template>");
		}
		
	}

	return TRUE;
}

//1 �����
BOOL CObject_Character::Tick_Ability( UINT uElapseTime )
{
	CObjectCommand_Ability *pAbilityCommand = (CObjectCommand_Ability*)GetBaseLogicCommand();
	if(pAbilityCommand == NULL)
	{
		return FALSE;
	}

	m_paramLogic_Ability.m_uCurrentTime += uElapseTime;

	UINT uEndTime = pAbilityCommand->GetEndTime();
	if(uEndTime == UINT_MAX)
	{
		uEndTime = m_paramLogic_Ability.m_uTotalTime;
	}

	if(m_paramLogic_Ability.m_uCurrentTime > uEndTime)
	{
		m_paramLogic_Ability.m_uCurrentTime = uEndTime;
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, (FLOAT)m_paramLogic_Ability.m_uCurrentTime/(FLOAT)m_paramLogic_Ability.m_uTotalTime);
	}

	if(m_paramLogic_Ability.m_uCurrentTime >= uEndTime)
	{
		if(m_paramLogic_Ability.m_uTotalTime == uEndTime)
		{
			CharacterLogic_Stop(TRUE, LOGIC_BASE);	// ��ɲ�����
		}
		else
		{
			CharacterLogic_Stop(FALSE, LOGIC_BASE);	// δ��ɽ���
		}
	}

	return TRUE;
}

//1 ��̯
BOOL CObject_Character::Tick_Stall( UINT uElapseTime )
{
	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	if(IsBaseAnimationEnd())
	{
		if(m_paramLogic_Stall.m_nCurrentAnimation == SLogicParam_Stall::STALL_ANIMATION_SITDOWN)
		{
			ChangeAction(BASE_ACTION_N_SIT_IDLE, GetLogicSpeed(), TRUE);
			m_paramLogic_Stall.m_nCurrentAnimation = SLogicParam_Stall::STALL_ANIMATION_IDLE;
		}

		if(m_paramLogic_Stall.m_nCurrentAnimation == SLogicParam_Stall::STALL_ANIMATION_STANDUP)
		{
			CharacterLogic_Stop(TRUE, LOGIC_BASE);
		}
	}

	if(!(pCharacterData->Get_IsInStall())
		&& m_paramLogic_Stall.m_nCurrentAnimation != SLogicParam_Stall::STALL_ANIMATION_STANDUP)
	{
		ChangeAction(BASE_ACTION_N_SIT_STAND, GetLogicSpeed(), FALSE);
		m_paramLogic_Stall.m_nCurrentAnimation = SLogicParam_Stall::STALL_ANIMATION_STANDUP;
	}

	return TRUE;
}

//1 ����
BOOL CObject_Character::Tick_Dead( UINT uElapseTime )
{
	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	if(!pCharacterData->IsDie())
	{
		CharacterLogic_Stop(TRUE, LOGIC_BASE);
		CGameInterface::GetMe()->Skill_SetActive( CActionSystem::GetMe()->GetMyDefaultSkillAtion() );
	}

	// ����Ѿ������˾Ͳ�����ִ��λ����
	if( m_paramLogic_Dead.m_bDeadIsEnd )
		return TRUE;


	// �Ƿ���� �� ���ǲ�������
	if( !m_bBeatFly || ( CT_PLAYERMYSELF == GetCharacterType() ) )
	{
		if( m_paramLogic_Dead.m_bDeadEnd )
		{
			// ������ֱ���л�����������
			FLOAT height = 0.0f;
			CWorldManager::GetMe()->GetTerrainHeight_GamePos(m_paramLogic_Dead.m_CurrPos.x, m_paramLogic_Dead.m_CurrPos.z, height);
			m_paramLogic_Dead.m_CurrPos.y = height;
			//SetPosition( m_paramLogic_Dead.m_CurrPos );


			// ��Ϊʬ�嶯��
			//ChangeAction( m_paramLogic_Dead.m_nCadaverAction, GetLogicSpeed(), TRUE, ACTION_BODY,0.0f);
			StopCurrentAction();
			CreateBloodStain();

	//		CharacterLogic_Stop(TRUE, LOGIC_BASE);
	//		CharacterLogic_Stop(TRUE, LOGIC_ACTION);
			
			m_paramLogic_Dead.m_bDeadIsEnd = TRUE;

			RemoveAllLogicEvent();
		}

		return TRUE;
	}


	// ֻ�л��ɵ�����²Ŵ��������
	if( m_bBeatFly )	// && !m_paramLogic_Dead.m_bDeadEnd
	{
		// ������루Ŀ�� - ��ǰ��
		fVector3 fvDistToTarget = m_paramLogic_Dead.m_TarPos - m_paramLogic_Dead.m_CurrPos;
		FLOAT fDistToTargetSq = fvDistToTarget.x * fvDistToTarget.x + fvDistToTarget.z * fvDistToTarget.z;
		FLOAT fDistToTarget = sqrt(fDistToTargetSq) ;
		if( fDistToTarget < 0.01f )
		{
			return TRUE;
		}


		// �����ٶ�
		FLOAT fSpeed = 15;


		// ��һ֡���ƶ���·������
		FLOAT fElapseTime = ((FLOAT)(CGameProcedure::s_pTimeSystem->GetDeltaTime()))/1000.0f;
		FLOAT fMoveDist = fSpeed * fElapseTime;

		// �ж��Ƿ񵽴�Ŀ��λ�� �� ���ɶ����Ѿ�����
		if( (fMoveDist > fDistToTarget) || (TRUE == m_paramLogic_Dead.m_bDeadFlyEnd) )
		{
			// ������Ч��
			CGameProcedure::s_pGfxSystem->Camera_VibrateOneTime(2.0f, 1.0f, 1);

			// �ı䵽ʬ��Ķ���
			//ChangeAction( m_paramLogic_Dead.m_nCadaverAction, GetLogicSpeed(), TRUE, ACTION_BODY, 0.0f );
			StopCurrentAction();

			// ����Ѫ��
			CreateBloodStain();

			// ���õ�Ŀ��λ��
			//SetPosition( m_paramLogic_Dead.m_CurrPos );

			// ������
			m_paramLogic_Dead.m_bDeadIsEnd = TRUE;

			RemoveAllLogicEvent();
		}
		else
		{
			// ����ƫ��
			FLOAT fDistX = ( fMoveDist * ( m_paramLogic_Dead.m_TarPos.x - m_paramLogic_Dead.m_CurrPos.x) ) / fDistToTarget;
			FLOAT fDistZ = ( fMoveDist * ( m_paramLogic_Dead.m_TarPos.z - m_paramLogic_Dead.m_CurrPos.z) ) / fDistToTarget;

			m_paramLogic_Dead.m_CurrPos.x = m_paramLogic_Dead.m_CurrPos.x + fDistX;
			m_paramLogic_Dead.m_CurrPos.y = m_paramLogic_Dead.m_CurrPos.z + fDistZ;

			// ��ȡ����߶�
			FLOAT height = 0.0f;
			CWorldManager::GetMe()->GetTerrainHeight_GamePos(m_paramLogic_Dead.m_CurrPos.x, m_paramLogic_Dead.m_CurrPos.z, height);
			m_paramLogic_Dead.m_CurrPos.y = height;

			SetPosition( m_paramLogic_Dead.m_CurrPos );
		}
	}

	return TRUE;
}




//2 ����
BOOL CObject_Character::Tick_Action( UINT uElapseTime )
{
	if(IsActionAnimationEnd() || !IsActionLogicCommandListEmpty())
	{
		CharacterLogic_Stop(TRUE, LOGIC_ACTION);
	}

	return TRUE;
}

//2 ���ܾ���
BOOL CObject_Character::Tick_MagicCharge( UINT uElapseTime )
{
	CObjectCommand_MagicCharge *pMagicChargeCommand = (CObjectCommand_MagicCharge*)GetActionLogicCommand();
	if(pMagicChargeCommand == NULL)
	{
		return FALSE;
	}

	m_paramLogic_MagicCharge.m_uCurrentTime += uElapseTime;
	if(m_paramLogic_MagicCharge.m_uCurrentTime >= pMagicChargeCommand->GetEndTime())
	{
		m_paramLogic_MagicCharge.m_uCurrentTime = pMagicChargeCommand->GetEndTime();
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		FLOAT fCurTime		= (FLOAT)(m_paramLogic_MagicCharge.m_uCurrentTime);
		FLOAT fTotalTime	= (FLOAT)(pMagicChargeCommand->GetTotalTime());
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, fCurTime / fTotalTime);
	}

	if(m_paramLogic_MagicCharge.m_uCurrentTime >= pMagicChargeCommand->GetEndTime())
	{
		BOOL bFinished;
		if(pMagicChargeCommand->GetEndTime() == pMagicChargeCommand->GetTotalTime())
		{
			bFinished = TRUE;
		}
		else
		{
			bFinished = FALSE;
		}
		CharacterLogic_Set(CHARACTER_LOGIC_INVALID, LOGIC_ACTION);
		CharacterLogic_Stop(bFinished, LOGIC_ACTION);
	}

	// ����ķ���
	ObjID_t nTargetObjID = pMagicChargeCommand->GetTargetObjID();
	if(nTargetObjID != INVALID_ID)
	{
		CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(nTargetObjID));
		if(pObj != NULL && pObj != this)
		{
			fVector2 fvThis(GetPosition().x, GetPosition().z);
			fVector2 fvTarget(pObj->GetPosition().x, pObj->GetPosition().z);

			FLOAT fDir = KLU_GetYAngle(fvThis, fvTarget);
			SetFaceDir(fDir);
		}
	}
	return TRUE;
}

//2 ��������
BOOL CObject_Character::Tick_MagicChannel( UINT uElapseTime )
{
	CObjectCommand_MagicChannel *pMagicChannelCommand = (CObjectCommand_MagicChannel*)GetActionLogicCommand();
	if(pMagicChannelCommand == NULL)
	{
		return FALSE;
	}

	if(m_paramLogic_MagicChannel.m_uCurrentTime > uElapseTime)
	{
		m_paramLogic_MagicChannel.m_uCurrentTime -= uElapseTime;
	}
	else
	{
		m_paramLogic_MagicChannel.m_uCurrentTime = 0;
	}

	if(m_paramLogic_MagicChannel.m_uCurrentTime < pMagicChannelCommand->GetEndTime())
	{
		m_paramLogic_MagicChannel.m_uCurrentTime = pMagicChannelCommand->GetEndTime();
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		FLOAT fCurTime		= (FLOAT)(m_paramLogic_MagicChannel.m_uCurrentTime);
		FLOAT fTotalTime	= (FLOAT)(pMagicChannelCommand->GetTotalTime());
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, fCurTime / fTotalTime);
	}

	if(m_paramLogic_MagicChannel.m_uCurrentTime <= pMagicChannelCommand->GetEndTime())
	{
		BOOL bFinished;
		if(pMagicChannelCommand->GetEndTime() == 0)
		{
			bFinished = TRUE;
		}
		else
		{
			bFinished = FALSE;
		}

		CharacterLogic_Set(CHARACTER_LOGIC_INVALID, LOGIC_ACTION);
		CharacterLogic_Stop(bFinished, LOGIC_ACTION);
	}

	// ����ķ���
	ObjID_t nTargetObjID = pMagicChannelCommand->GetTargetObjID();
	if(nTargetObjID != INVALID_ID)
	{
		CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(nTargetObjID));
		if(pObj != NULL && pObj != this)
		{
			fVector2 fvThis(GetPosition().x, GetPosition().z);
			fVector2 fvTarget(pObj->GetPosition().x, pObj->GetPosition().z);

			FLOAT fDir = KLU_GetYAngle(fvThis, fvTarget);
			SetFaceDir(fDir);
		}
	}
	return TRUE;
}

//test2
BOOL CObject_Character::Tick_MagicPrepSend( UINT uElapseTime )
{
	// ���Ӽ�ʱ���� �жϳ�ʱ��ת������

	return TRUE;
}

//2 ���ܷ���
BOOL CObject_Character::Tick_MagicSend( UINT uElapseTime )
{
	// ȡ�ü�������
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( m_paramLogic_MagicSend.m_nSaveMagicID );
	if( pClientSkill == NULL )
		return FALSE;


/*
	CObjectCommand_MagicSend* pMagicSendCommand = (CObjectCommand_MagicSend*)GetActionLogicCommand();
	if( pMagicSendCommand == NULL)
	{
		return FALSE;
	}

	if( pMagicSendCommand->GetTargetObjID() != INVALID_ID )
	{
		CObject* pTarget = (CObject*)(pObjectManager->FindServerObject( pMagicSendCommand->GetTargetObjID() ));
		if( pTarget != NULL )
		{
			fvTarget.x = pTarget->GetPosition().x;
			fvTarget.y = pTarget->GetPosition().z;
		}
	}
*/

	m_paramLogic_MagicSend.m_uAnimationTime += uElapseTime;
	// ����ʱ�䳤��5��
	if( m_paramLogic_MagicSend.m_uAnimationTime > 5000 )
	{
		m_UseSkillTag = FALSE;
		CharacterLogic_Stop(TRUE, LOGIC_ACTION);
		return TRUE;
	}

	// ���ݶ����Ļص������ж�
	if( IsActionAnimationEnd() )
	{
		m_paramLogic_MagicSend.m_uAnimationEndElapseTime += uElapseTime;

		BOOL bStop = TRUE;
		INT nSkillActionType = pClientSkill->m_nSkillActionType;

		// �����Ϊ�߼������б�Ϊ�� ���� ����Ϊ����
		if( !IsActionLogicCommandListEmpty() && 
			(/*nSkillActionType == SKILL_ACTION_TYPE_CONCATENATION || */
			   nSkillActionType == SKILL_ACTION_TYPE_CONCATENATION_EX) )
		{
			if( m_paramLogic_MagicSend.m_uAnimationEndElapseTime < 500 )
			{
				bStop = FALSE;
			}
		}

		if( bStop )
		{
			m_UseSkillTag = FALSE;
			CharacterLogic_Stop(TRUE, LOGIC_ACTION);
		}
	}

	// �����ܷ�����߼�����ж�
	if( m_paramLogic_MagicSend.m_bCanBreak && !IsActionLogicCommandListEmpty() )
	{
		m_UseSkillTag = FALSE;
		CharacterLogic_Stop(TRUE, LOGIC_ACTION);
	}

	return TRUE;
}



//================-================
// �¼���ز���
//================-================


VOID CObject_Character::SetLogicSpeed(FLOAT fSpeed)
{
	if(fabsf(m_fLogic_Speed - fSpeed) > 0.1f)
	{
		m_fLogic_Speed = fSpeed;
		OnLogicSpeedChanged();
	}
}

VOID CObject_Character::OnLogicSpeedChanged(VOID)
{
	ChangeActionSpeed(GetLogicSpeed());
}

RC_RESULT CObject_Character::DoSimulantCommand(const SCommand_Object *pCmd)
{
	if( OC_MAGIC_PREP_SEND != pCmd->m_wID )
	{
	CleanupLogicCommandList( LOGIC_BASE );
//	if( CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_SEND )
//	{
//		if( !m_paramLogic_MagicSend.m_bCanBreak && !CharacterLogic_IsStopped(LOGIC_ACTION) )
//		{
//			return RC_WAIT;
//		}
//	}

	CharacterLogic_Stop( FALSE, LOGIC_BASE );
	}

	PushCommand( pCmd );

	return RC_OK;
}

// ����ִ�д�ָ��
RC_RESULT CObject_Character::DoCommand( const SCommand_Object *pCmd )
{
	return OnCommand( pCmd );
}

RC_RESULT CObject_Character::OnCommand( const SCommand_Object *pCmd )
{
	RC_RESULT rcResult = RC_SKIP;

	switch ( pCmd->m_wID )
	{
	case OC_TELEPORT:
		{
			fVector3 fvServerPos;
			fvServerPos.x		= pCmd->m_afParam[0];
			fvServerPos.z		= pCmd->m_afParam[1];

			CWorldManager::GetMe()->GetActiveScene()->GetMapHeight(fVector2(fvServerPos.x, fvServerPos.z), fvServerPos.y);

			SetMapPosition( fVector2(fvServerPos.x, fvServerPos.z) );
			rcResult = RC_OK;
		}
		break;

	case OC_JUMP:
		{
			Jump();

			rcResult = RC_OK;
		}
		break;

	case OC_DEATH:
		{
			BOOL bPlayDieAni;
			bPlayDieAni		= pCmd->m_abParam[0];
			BOOL bResult = Start_Dead( bPlayDieAni );
			if ( bResult )
				rcResult = RC_OK;
			else
				rcResult = RC_ERROR;
		}
		break;

	case OC_UPDATE_IMPACT:
		{
			ImpactID_t	idImpact;
			BOOL		bEnable;
			ObjID_t		nCreatorID;
			UINT		nNumOflayer;
			UINT		nSN;
			UINT		nDelayTime;

			// buff�ĵ���ʱ����		20100531 BLL
			INT		nTimeCount; 
			time_t	nTimeNow;		

			idImpact	= (ImpactID_t)(pCmd->m_adwParam[0]);
			nSN			= pCmd->m_adwParam[1];
			nNumOflayer	= pCmd->m_anParam[2];
			nCreatorID	= pCmd->m_anParam[3];
			bEnable		= pCmd->m_abParam[4];
			nDelayTime	= pCmd->m_anParam[5];

			// ��ȡbuff�ĵ���ʱ����		20100531 BLL
			nTimeCount = pCmd->m_anParam[6];

			// ���»�����
			if( bEnable )
			{
				// ��Ҫ��ʱ���ӳ٣��������ȴ����С�
				if( nDelayTime > 0 )
				{
					CImpactEffectMap::iterator it = m_mapImpactDelay.find(nSN);// ���кż��
					if (it == m_mapImpactDelay.end())
					{
						SImpactEffect* pImpactEffect = new SImpactEffect;
						m_mapImpactDelay[nSN] = pImpactEffect;

						pImpactEffect->SetLayerNum( nNumOflayer );
						pImpactEffect->SetImpactID( idImpact );
						pImpactEffect->SetCreatorID( nCreatorID );				

						UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();

						pImpactEffect->SetShowEffect( TRUE );
						pImpactEffect->SetBeginTime( uTimeNow );
						pImpactEffect->SetDelayTime( nDelayTime );
						pImpactEffect->SetArrayIndex( nSN );

						// ���ó���buff�ĵ���ʱ����		20100531 BLL
						pImpactEffect->SetContinuance( nTimeCount );
						pImpactEffect->SetTimeNow( uTimeNow );
					}					
				}
				// û���ӳ٣�ֱ��ִ�С�
				else
				{
					InsertImpact( nSN, idImpact, nTimeCount, nCreatorID, nNumOflayer, TRUE );

					// 20100428 AddCodeBegin
					// ���ͽ�ɫ�ͷ�buff��Ҫˢ����Ϣ����targetframe��Ӧ��������buffͼ��
					CGameProcedure::s_pEventSystem->PushEvent( GE_MAINTARGET_BUFF_REFRESH );
					// 20100428 AddCodeEnd
				}				
			}
			// ɾ��
			else
			{
				RemoveImpact( nSN );

				// 20100428 AddCodeBegin
				// ���ͽ�ɫ�ͷ�buff��Ҫˢ����Ϣ����targetframe��Ӧ��������buffͼ��
				CGameProcedure::s_pEventSystem->PushEvent( GE_MAINTARGET_BUFF_REFRESH );
				// 20100428 AddCodeEnd
			}	
			rcResult = RC_OK;
		}
		break;

	case OC_UPDATE_IMPACT_EX:
		{
			// 
			/*ImpactID_t  idImpact;
			UINT		nSN;
			INT			nNumOflayer;

			idImpact	= (ImpactID_t)(pCmd->m_adwParam[0]);
			nSN			= pCmd->m_adwParam[1];			
			nNumOflayer	= pCmd->m_anParam[0];

			ChangeImpact( nSN, idImpact, INVALID_ID, nNumOflayer, TRUE, FALSE );
			rcResult = RC_OK;*/
		}
		break;

	case OC_LEVEL_UP:
		{
			CHAR szText[128];
			sprintf(szText, "%d;0", (INT)(BASE_ACTION_LEVEL_UP));
			SetChatMoodAction(STRING(szText));
			rcResult = RC_OK;
		}
		break;

//	case OC_PREP_SKILL_SEND:
//		{
//			PrepMagicSend( pCmd );
//			rcResult = RC_OK;
//		}
//		break;

	default:
		rcResult = RC_SKIP;
		break;
	}

	return rcResult;
}




VOID CObject_Character::CheckMoveStop(VOID)
{
	if(CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_MOVE)
		return ;

	CObjectCommand_Move *pMoveCommand = (CObjectCommand_Move*)GetBaseLogicCommand();
	if(pMoveCommand != NULL && pMoveCommand->GetNodeCount() > 0)
	{
		// ���ǲ��߻�ͷ·����˲��
		BOOL bAdjustPos = FALSE;
		WORLD_POS posAdjust;

		// ��ǰ�����������ƶ�����Ľڵ���
		if(m_paramLogic_Move.m_nCurrentNodeIndex >= pMoveCommand->GetNodeCount())
		{
			const WORLD_POS *paPos = pMoveCommand->GetNodeList();
			INT nEndNodeIndex = pMoveCommand->GetNodeCount() - 1;
			// ȡ���һ��λ��
			posAdjust = paPos[nEndNodeIndex];

			bAdjustPos	= TRUE;
		}
		// ��ǰΪ���ڵ�
		else if(m_paramLogic_Move.m_nCurrentNodeIndex == pMoveCommand->GetNodeCount() - 1)
		{
			const WORLD_POS *paPos = pMoveCommand->GetNodeList();
			INT nEndNodeIndex = pMoveCommand->GetNodeCount() - 1;
			WORLD_POS posCommandTarget = paPos[nEndNodeIndex];

			FLOAT fLenCSTarget = fabsf(m_paramLogic_Move.m_posSaveTarget.m_fX - posCommandTarget.m_fX)
				+ fabsf(m_paramLogic_Move.m_posSaveTarget.m_fZ - posCommandTarget.m_fZ);

			// Ŀ���䶯��
			if(fLenCSTarget > 0.01f)
			{
				FLOAT fSaveToServerDist = KLU_GetDist(fVector2(m_paramLogic_Move.m_posSaveTarget.m_fX, m_paramLogic_Move.m_posSaveTarget.m_fZ),
					fVector2(posCommandTarget.m_fX, posCommandTarget.m_fZ));

				FLOAT fSaveToCurrentDist = KLU_GetDist(fVector2(m_paramLogic_Move.m_posSaveTarget.m_fX, m_paramLogic_Move.m_posSaveTarget.m_fZ),
					fVector2(GetPosition().x, GetPosition().z));

				// ��������˷�������������Ŀ��㲻��·���ϵ����
				if(fSaveToServerDist - fSaveToCurrentDist >= 0.0f)
				{
					posAdjust = posCommandTarget;
					bAdjustPos = TRUE;
				}

				m_paramLogic_Move.m_posSaveTarget.m_fX	= posCommandTarget.m_fX;
				m_paramLogic_Move.m_posSaveTarget.m_fZ	= posCommandTarget.m_fZ;
			}
		}

		// ��Ҫ����
		if(bAdjustPos)
		{
			// ���������
			if(CObjectManager::GetMe()->GetMySelf() == this)
			{
				// ��λ�ò����ĳ��ֵʱ
				FLOAT fAdjustToCurrentDist = KLU_GetDist(fVector2(posAdjust.m_fX, posAdjust.m_fZ),
					fVector2(GetPosition().x, GetPosition().z));

				if(fAdjustToCurrentDist > DEF_CLIENT_ADJUST_POS_WARP_DIST)
				{
					// ˲�Ƶ���ǰ��������Ӧ��λ��
					fVector2 fvPos(posAdjust.m_fX, posAdjust.m_fZ);
					SetMapPosition( fvPos );
				}
				else
				{
					// ����������λ�õ���ǰ���ǿͻ��˶�Ӧ��λ��
					CGCharPositionWarp msgWarp;
					WORLD_POS posCur(GetPosition().x, GetPosition().z);
					msgWarp.setObjID(GetServerID());
					msgWarp.setServerPos(posAdjust);
					msgWarp.setClientPos(posCur);
					CNetManager::GetMe()->SendPacket(&msgWarp);
				}
			}
			else
			{
				FLOAT fAdjustToCurrentDist = KLU_GetDist(fVector2(posAdjust.m_fX, posAdjust.m_fZ),
					fVector2(GetPosition().x, GetPosition().z));

				if(fAdjustToCurrentDist > DEF_CHARACTER_POS_ADJUST_DIST)
				{
					// ˲�Ƶ���ǰ��������Ӧ��λ��
					fVector2 fvPos(posAdjust.m_fX, posAdjust.m_fZ);
					SetMapPosition( fvPos );
				}
			}

			CharacterLogic_Stop(TRUE, LOGIC_BASE);
		}
	}
}

BOOL CObject_Character::Jump( VOID )
{
	if(IsDie())
	{
		return FALSE;
	}

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	// ����
	if(pCharacterData->Get_ModelID() != INVALID_ID)
	{
		return FALSE;
	}

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		// ��Ӹ���
		if(pCharacterData->Get_TeamFollowFlag())
			return FALSE;

		// ��̯
		if(pCharacterData->Get_IsInStall())
			return FALSE;
	}

	INT nBaseAction;
	nBaseAction = BASE_ACTION_N_JUMP_R;
	
	if (CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_MOVE)
	{
		nBaseAction = BASE_ACTION_N_JUMP_N;
	}
	else
	{
		if ( GetCharacterData()->Get_MountID() == -1 )
		{
			INT nRet = rand() % 4;
			if (nRet == 0)
				nBaseAction = BASE_ACTION_N_JUMP_R;
			else
				nBaseAction = BASE_ACTION_N_JUMP_N;
		}
		else
		{
			nBaseAction = BASE_ACTION_N_JUMP_R;
		}
	}

	//ChangeAction( GetActionNameByActionSetID( nBaseAction, GetWeaponType() ), 1.f, FALSE );
	ChangeAction( nBaseAction, 1.f, FALSE );

	if(CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_MOVE || GetCharacterData()->Get_MountID() == -1)
	{
		SetbJumping(TRUE);
		RegisterPhyEvent(PE_COLLISION_WITH_GROUND);
		// �������ȥ����
		PhyEnable(TRUE);
		FLOAT fTEST = 12.0; 
		AddLinearSpeed(fVector3(0.0, fTEST, 0.0));
	}

	StandUp();

	return TRUE;
}

BOOL CObject_Character::SitDown(VOID)
{
	if(IsDie())
	{
		return FALSE;
	}

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	// ���
	if(pCharacterData->Get_MountID() != INVALID_ID)
	{
		return FALSE;
	}

	// ����
	if(pCharacterData->Get_ModelID() != INVALID_ID)
	{
		return FALSE;
	}

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		// ��Ӹ���
		if(pCharacterData->Get_TeamFollowFlag())
			return FALSE;

		// ��̯
		if(pCharacterData->Get_IsInStall())
			return FALSE;
	}

	if (CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_IDLE && GetCharacterData() != NULL)
	{
		if(!GetCharacterData()->IsSit() && GetCharacterData()->Get_MountID() == INVALID_ID)
		{
			GetCharacterData()->Set_Sit(TRUE);
			if(CObjectManager::GetMe()->GetMySelf() == this)
			{
				CGCharSit cmdSit;
				cmdSit.setObjID(GetServerID());
				cmdSit.setSit(GetCharacterData()->IsSit());

				CNetManager::GetMe()->SendPacket(&cmdSit);
			}
		}
	}

	return TRUE;
}

BOOL CObject_Character::StandUp(VOID)
{
	if(IsDie())
	{
		return FALSE;
	}

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	if(pCharacterData->Get_MountID() != INVALID_ID)
	{
		return FALSE;
	}

	// ����
	if(pCharacterData->Get_ModelID() != INVALID_ID)
	{
		return FALSE;
	}

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		if(pCharacterData->Get_TeamFollowFlag())
			return FALSE;

		if(pCharacterData->Get_IsInStall())
			return FALSE;
	}

	if(GetCharacterData()->IsSit())
	{
		GetCharacterData()->Set_Sit(FALSE);
		if(CObjectManager::GetMe()->GetMySelf() == this)
		{
			CGCharSit cmdSit;
			cmdSit.setObjID(GetServerID());
			cmdSit.setSit(GetCharacterData()->IsSit());

			CNetManager::GetMe()->SendPacket(&cmdSit);
		}
	}
	return TRUE;
}

BOOL CObject_Character::PrepMagicSend( const SCommand_Object *pCmd )
{
	if( NULL == pCmd )
		return FALSE;


	INT iMagicId	= pCmd->m_anParam[2];	// ����ID
	INT iTargetId	= pCmd->m_anParam[3];	// Ŀ��ID
	fVector3 fvServerPos;
	fvServerPos.x	= pCmd->m_afParam[4];	// Ŀ��λ��
	fvServerPos.z	= pCmd->m_afParam[5];	// Ŀ��λ��
	FLOAT fDir		= pCmd->m_afParam[6];	// Ŀ�귽��

	// ��ȡ�������� 
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( iMagicId );
	if( pClientSkill == NULL )
		return FALSE;


	// �ı䶯��Ϊ���ܷ���Ԥ��

	INT nRandAnimID = BASE_ACTION_FP;	// GetSkillDataMgr()->GetRandAnim( m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE );
	BOOL bAnimLoop = TRUE;

	// �жϻ����߼�
	if( !CharacterLogic_IsStopped(LOGIC_BASE) )
	{
		// �Ƿ�Ϊ�ƶ�
		if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
		{
			// �����ƶ�ֻ�ı��ϲ�����
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
		}
		else 
		{
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}
	}
	// �޻����߼�
	else
	{
		ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
	}

	return TRUE;
}


VOID CObject_Character::OnMoveStop(VOID)
{
	if ( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_MOVE )
		CharacterLogic_Stop(TRUE, LOGIC_BASE);
}

BOOL CObject_Character::OnAnimationEnd( LPCTSTR szAnimationName, ENUM_ACTION_TYPE ePart, LOGIC_TYPE eLogic )
{
    FreshAnimation( ePart, eLogic );
	return true;
}

BOOL CObject_Character::OnAnimationCanBreak(LPCTSTR szAnimationName)
{
	if(CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_SEND)
	{
		m_paramLogic_MagicSend.m_bCanBreak	= TRUE;
	}

	//if(!IsLogicCommandListEmpty())
	//{
	//	CharacterLogic_Stop(TRUE);
	//}

	return true;
}

BOOL CObject_Character::OnAnimationHit(LPCTSTR szAnimationName)
{
	// ��¼�����еĵڼ���Hit
	static INT nHitCount; 

	if( CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_SEND )
	{
		CObjectCommand_MagicSend* pMagicSendCommand = (CObjectCommand_MagicSend*)GetActionLogicCommand();
		const SSkillData* pClientSkill = GetSkillDataMgr()->GetSkillData(m_paramLogic_MagicSend.m_nSaveMagicID);
		
		if( pClientSkill != NULL && pMagicSendCommand != NULL )
		{
			// ���õ�ǰĿ��λ������
			INT idTarget = pMagicSendCommand->GetTargetObjID(0);
			CObject* pTarget = (CObject*)(CObjectManager::GetMe()->FindServerObject( idTarget ));
			
			if(pTarget != NULL) 
			{
				BOOL bShowAll = FALSE;
				
				if(pClientSkill->m_nHitsOrINTerval > 0)
				{
					// ���������
					++nHitCount;
					
					if(nHitCount == pClientSkill->m_nHitsOrINTerval)
					{ 
						// ��������һ��
						m_paramLogic_MagicSend.m_bBeAttackEffectShowed = TRUE;	
						bShowAll = TRUE;
						nHitCount = 0;
					}
				}
				else
				{
					m_paramLogic_MagicSend.m_bBeAttackEffectShowed = TRUE;
				}
			}
			else
			{
				m_paramLogic_MagicSend.m_bBeAttackEffectShowed = TRUE;
			}
		}

	}

	return TRUE;
}

VOID CObject_Character::FreshAnimation( ENUM_ACTION_TYPE ePart, LOGIC_TYPE eLogic )
{
//	SetAnimationEnd(TRUE);

	// �����߼�����

	BOOL bBaseAnim = FALSE;
//	if( LOGIC_BASE == eLogic )
	{
		switch( CharacterLogic_Get(LOGIC_BASE) )
		{
			// ����
		case CHARACTER_LOGIC_IDLE:
			{
				BOOL bPlayIdleMotion = FALSE;
				UINT nTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
				
				if(m_paramLogic_Idle.m_uLastIdleMotionTime + m_paramLogic_Idle.m_uIdleInterval < nTimeNow)
				{
					bPlayIdleMotion = TRUE;
					m_paramLogic_Idle.m_uLastIdleMotionTime	= nTimeNow;
					m_paramLogic_Idle.m_uIdleInterval		= CalcIdleRandInterval();
				}

				// ���鶯��
				if( IsHaveChatMoodAction() )
				{
					if( GetCharacterData() && GetCharacterData()->IsSit() )
					{
						StandUp();
					}
					else
					{
						PlayChatMoodAction();
					}
				}
				else
				{
					BOOL bFightState, bSit;
					INT nBaseAction;
					bFightState	= IsFightState();
					bSit = GetCharacterData() ? GetCharacterData()->IsSit() : FALSE;
					
					if( bSit )
					{
						nBaseAction = BASE_ACTION_N_SIT_IDLE;		// ������Ϣ
					}
					else
					{
	//					if( m_UseSkillTag )
	//					{
	//						return ;
	//						nBaseAction = BASE_ACTION_FP;
	//					}
	//					else 
							if( bFightState )
						{
							nBaseAction = BASE_ACTION_F_IDLE;		// ����
						}
						else
						{
							if(bPlayIdleMotion)
							{
								nBaseAction = rand() % 2 + BASE_ACTION_N_IDLE_EX0;	// ��ϢС�����������������������
							}
							else
							{
								nBaseAction = BASE_ACTION_N_IDLE;	// ��Ϣ
							}
						}
					}

					bBaseAnim = TRUE;
					ChangeAction( nBaseAction, GetLogicSpeed(), FALSE, ePart );
				}
			}
			break;

			// �ƶ�
		case CHARACTER_LOGIC_MOVE:
			{
				// ���ڲ߻���û��ս��״̬�µ��ܺ��ߵ�����

	/*			BOOL bFightState;	// �Ƿ�ս��״̬
				INT nBaseAction;	// ����

				bFightState	= IsFightState();

				int tmp = rand() % 2;	// �������
				if( bFightState )
				{
					nBaseAction	= BASE_ACTION_N_RUN - tmp;	// BASE_ACTION_F_RUN
				}
				else
				{
					nBaseAction	= BASE_ACTION_N_RUN - tmp;
				}

				nBaseAction	= BASE_ACTION_N_RUN;

				bBaseAnim = TRUE;

				if( !CharacterLogic_IsStopped(LOGIC_ACTION) )
				{
					ChangeAction( nBaseAction, GetLogicSpeed(), TRUE, ACTION_DOWN );
				}
				else
				{
					ChangeAction( nBaseAction, GetLogicSpeed(), TRUE, ePart );
				}
	*/

				bBaseAnim = TRUE;
				INT nBaseAction = BASE_ACTION_N_RUN;

				// �жϴ˴��Ƿ�Ϊ���ƶ������еļ���ʩ�Ž���
	//			if( ( CHARACTER_LOGIC_SKILL_SEND == CharacterLogic_Get(LOGIC_ACTION) ) && ( ACTION_TOP == ePart ) )
	//			{
	//				ChangeAction( nBaseAction, GetLogicSpeed(), TRUE );
	//			}
	//			else
				{
					ChangeAction( nBaseAction, GetLogicSpeed(), TRUE, ACTION_DOWN );
				}
			}
			break;

		case CHARACTER_LOGIC_ABILITY_ACTION:
			break;

		case CHARACTER_LOGIC_DEAD:
			{
				bBaseAnim = TRUE;

				// ���ɶ����Ѿ���ʼ��
				if( TRUE == m_paramLogic_Dead.m_bDeadFlyBegin )
				{
					m_paramLogic_Dead.m_bDeadFlyEnd = TRUE;
				}

				if( TRUE == m_paramLogic_Dead.m_bDeadBegin )
				{
					m_paramLogic_Dead.m_bDeadEnd	= TRUE;
				}

				// ������Ĵ�����������
				//INT nBaseAction = rand() % 4 + BASE_ACTION_F_CADAVER0;
				//ChangeAction( m_paramLogic_Dead.m_nCadaverAction, GetLogicSpeed(), FALSE, ePart, 0.0f );
				StopCurrentAction();//ֹͣ����������������Ϊ���һ�����ݺ�ʬ�嶯��һ��(todo:��ʱ������ɻ���������ʬ�岥��Ч����������,��δ�����Ҫ����һ��)
			}
			break;

		case CHARACTER_LOGIC_STALL:
			if(m_paramLogic_Stall.m_nCurrentAnimation == SLogicParam_Stall::STALL_ANIMATION_IDLE)
			{
				bBaseAnim = TRUE;
				ChangeAction( BASE_ACTION_N_SIT_IDLE, GetLogicSpeed(), TRUE, ePart);
			}
			break;

		default:
			break;
		};
	}


	// ��Ϊ�߼�����

	BOOL bActionAnim = FALSE;
//	if( LOGIC_ACTION == eLogic )
	{
		switch( CharacterLogic_Get(LOGIC_ACTION) )
		{
		// ����
		case CHARACTER_LOGIC_ACTION:
			break;

		case CHARACTER_LOGIC_SKILL_GATHER:
			break;

		case CHARACTER_LOGIC_SKILL_LEAD:
			break;

		//test2
		// ׼��������
		case CHARACTER_LOGIC_SKILL_PREP_SEND:
			// ʲô�������� ���ı��������
			break ;

		// ������
		case CHARACTER_LOGIC_SKILL_SEND:
			{
				bActionAnim = TRUE;

				// ����ֹͣ�ˣ� ���ǻ����ƶ�
//				if( NULL == GetActionLogicCommand() )
//				{
//					if( ( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) ) && ( ACTION_TOP == ePart ) )
//					{
//						ChangeAction( BASE_ACTION_N_RUN, GetLogicSpeed(), FALSE, ePart );
//					}
//				}


				if(m_paramLogic_MagicSend.m_bDoNextAction_Concatenation)
				{
					m_paramLogic_MagicSend.m_bDoNextAction_Concatenation = FALSE;

					const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(m_paramLogic_MagicSend.m_nSaveMagicID);
					if( pClientSkill != NULL )
					{
						// ��������е����⴦��
						BOOL bPlayAction = TRUE;
						INT nSkillActionType = pClientSkill->m_nSkillActionType;
						
						switch( nSkillActionType )
						{
						case SKILL_ACTION_TYPE_CONCATENATION:
							{
								m_paramLogic_MagicSend.m_nActionIndex++;
								if(m_paramLogic_MagicSend.m_nActionIndex >= m_paramLogic_MagicSend.m_nActionCount)
								{
									m_paramLogic_MagicSend.m_nActionIndex = 0;
								}
								bPlayAction = TRUE;
							}
							break;

						case SKILL_ACTION_TYPE_CONCATENATION_EX:
							{
								m_paramLogic_MagicSend.m_nActionIndex++;
								if ( m_paramLogic_MagicSend.m_nActionIndex >= m_paramLogic_MagicSend.m_nActionCount - 1 )
								{
									m_paramLogic_MagicSend.m_nActionIndex = 1;
								}
								bPlayAction = TRUE;
							}
							break;

						case SKILL_ACTION_TYPE_NONE:
						default:
							break;
						}

						if(bPlayAction)
						{
							INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE);
							ChangeAction( nRandAnimID, GetLogicSpeed(), FALSE, ePart );
						}
					}
				}
			}
			break;
		};
	}

	// ��ʶ��������
	if( bBaseAnim )
		SetBaseAnimationEnd( TRUE );
	if( bActionAnim )
		SetActionAnimationEnd( TRUE );
}



VOID CObject_Character::AddLogicEvent(const _LOGIC_EVENT *pLogicEvent)
{
	if( pLogicEvent != NULL )
	{
		_LOGIC_EVENT *pNewLogicEvent	= new _LOGIC_EVENT;
		*pNewLogicEvent					= *pLogicEvent;
		pNewLogicEvent->m_uBeginTime	= CGameProcedure::s_pTimeSystem->GetTimeNow();
		pNewLogicEvent->m_uRemoveTime	= 10000;	// 10��
		
		m_listLogicEvent.push_back(pNewLogicEvent);
	}
}

VOID CObject_Character::RemoveLogicEvent( INT nLogicCount )
{
	CLogicEventList::iterator itCur, itEnd, itDel;
	_LOGIC_EVENT* pLogicEvent;
	itCur = m_listLogicEvent.begin();
	itEnd = m_listLogicEvent.end();
	while( itCur != itEnd )
	{// ֻɾ�����߼�ID��Ӧ���˺���Ϣ����ֻɾ���ü��ܶ�Ӧ���˺���Ϣ
		pLogicEvent = *itCur;
		itDel = itCur;
		itCur++;
		if ( pLogicEvent->m_nSenderLogicCount == nLogicCount )
		{
			SAFE_DELETE( pLogicEvent );
			m_listLogicEvent.erase( itDel );
			break;
		}
	}
}

VOID CObject_Character::RemoveAllLogicEvent( VOID )
{
	CLogicEventList::iterator itCur, itEnd, itDel;
	_LOGIC_EVENT* pLogicEvent;
	itCur = m_listLogicEvent.begin();
	itEnd = m_listLogicEvent.end();
	while( itCur != itEnd )
	{// ֻɾ�����߼�ID��Ӧ���˺���Ϣ����ֻɾ���ü��ܶ�Ӧ���˺���Ϣ
		pLogicEvent = *itCur;
		itDel = itCur;
		itCur++;
		SAFE_DELETE( pLogicEvent );
	}
	m_listLogicEvent.clear();
}

BOOL CObject_Character::IsMySelf(INT ObjID) const
{
	CObject *pMySelf = CObjectManager::GetMe()->GetMySelf();
	if (pMySelf)
	{
		return ObjID == pMySelf->GetServerID();
	}
	return FALSE;
}

BOOL CObject_Character::IsMyPet(INT ObjID) const
{
	/*PET_GUID_t CurrentPetGuid = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();
	SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(CurrentPetGuid);
	if (pPet)
	{
		return ObjID == pPet->m_idServer;
	}*/

	return FALSE;
}

VOID CObject_Character::ShowLogicEvent( INT nServerObjID, INT nLogicCount, BOOL bShowAll/* = FALSE */ )
{
	CLogicEventList::iterator itCur, itEnd, itPrev;
	_LOGIC_EVENT *pLogicEvent;
	
	itCur = m_listLogicEvent.begin();
	itEnd = m_listLogicEvent.end();

	while( itCur != itEnd )
	{
		// ֻ��ʾ�ü����ͷ��ߵĸ��߼�ID��Ӧ���˺���Ϣ
		pLogicEvent = *itCur;
		itPrev = itCur;
		itCur++;
		if( pLogicEvent->m_nSenderID == nServerObjID && pLogicEvent->m_nSenderLogicCount <= nLogicCount )
		{
			DoLogicEvent(pLogicEvent);
			SAFE_DELETE( pLogicEvent );
			m_listLogicEvent.erase( itPrev );
			// ���ֻ��ʾһ����¼��break;

			if( m_listLogicEvent.empty() && IsDie() && CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_DEAD )
			{
				Start_Dead(TRUE);
			}

			if( !bShowAll )
				break;
		}
	}

}

// �������е��˺���Ϣ
VOID CObject_Character::Update_LogicEvent( VOID )
{
	// ��������˾�ֱ�����
	//if( IsDie() )	//  && CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_DEAD )
	//{
	//	RemoveAllLogicEvent();
	//	Start_Dead(TRUE);
	//}

	if( m_listLogicEvent.empty() )
		return;

	CLogicEventList::iterator itCur, itEnd, itPrev;
	_LOGIC_EVENT *pLogicEvent;

	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	itCur = m_listLogicEvent.begin();
	itEnd = m_listLogicEvent.end();

	while( itCur != itEnd )
	{
		pLogicEvent = *itCur;
		itPrev = itCur;
		itCur++;

		// �����ӳ��ж�
		if( ( uTimeNow - pLogicEvent->m_uBeginTime ) < pLogicEvent->m_damage.m_nDelayTime )
		{
//			pLogicEvent->m_damage.m_nDelayTime = ;
			continue ;
		}

		BOOL bMustRemove = FALSE;
		BOOL bMustDo = FALSE;

		if( pLogicEvent->m_nSenderID != INVALID_ID )
		{
			CObject *pObject = (CObject*)( CObjectManager::GetMe()->FindServerObject( pLogicEvent->m_nSenderID ) );
			if( pObject != NULL )
			{
				// �߼���������
//				if( pObject->IsBaseLogicReady  ( pLogicEvent->m_nSenderLogicCount ) ||
//					( pObject->IsActionLogicReady( pLogicEvent->m_nSenderLogicCount ) ))
//				{
//					bMustDo = TRUE;
//				}
//				// ����������˺��ĳ���ʱ����ǿ��ɾ��
//				else if( pLogicEvent->m_uBeginTime + pLogicEvent->m_uRemoveTime < uTimeNow )
//				{
//					bMustRemove = TRUE;
//				}

				bMustDo = TRUE;

				// ����������˺��ĳ���ʱ����ǿ��ɾ��
				if( pLogicEvent->m_uBeginTime + pLogicEvent->m_uRemoveTime < uTimeNow )
					bMustRemove = TRUE;
				else
					bMustRemove = FALSE;

			}
			else
			{
				bMustDo = TRUE;
			}
		}
		else
		{
			bMustDo = TRUE;
		}

		if( bMustRemove || bMustDo )
		{
			BOOL bDel = FALSE;			

			// ��ִ�гɹ���ſ�ɾ��
			if( DoLogicEvent( pLogicEvent ) )
			{
				bDel = TRUE;
			}

			if( bMustRemove || bDel )
			{
				SAFE_DELETE( pLogicEvent );
				m_listLogicEvent.erase( itPrev );
			}

			if( // m_listLogicEvent.empty() && 
				IsDie() && CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_DEAD )
			{
				Start_Dead(TRUE);
				break;
			}
		}
	}
}

BOOL CObject_Character::DoLogicEvent(const _LOGIC_EVENT *pLogicEvent)
{
	if( pLogicEvent != NULL )
	{
		// �ж������ӳ�
		if( pLogicEvent->m_damage.m_nDelayTime > 0 )
			return FALSE;

		// ����ͬ���¼�
		switch( pLogicEvent->m_nEventType )
		{
		case LOGIC_EVENT_TYPE_DAMAGE:
			{
				if (!DoLogicEvent_Damage( pLogicEvent ))
					return FALSE;
			}			
			break;

		default:
			break;
		}
	}

	return TRUE;
}

VOID CObject_Character::DisplayDamageBoard(const _DAMAGE_INFO* pDamageInfo)
{
	if(pDamageInfo == NULL)
		return;
	if(GetRenderInterface() == NULL || CGameProcedure::s_pUISystem == NULL)
		return;
	CObject *pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(pMySelf == NULL)
		return;
	if(this->GetCharacterType() == CT_MONSTER && ( ((CObject_PlayerNPC*)this)->IsDisplayBoard() == FALSE))
		return;
	
	CHAR szDamage[32];
	memset(szDamage, 0, sizeof(szDamage));

	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
	fVector3 fvCurPos = GetPosition();
	INT nCasterID = pDamageInfo->m_nSenderID;
	INT nReceiverID = pDamageInfo->m_nTargetID;
	
	BOOL bNeedDisplay = TRUE;
	bool bDouble = pDamageInfo->m_bIsCriticalHit > 0;
	//if(TRUE == IsMyPet(nCasterID))
	//{
	//	if(0 > pDamageInfo->m_nHealthIncrement)//�˺�
	//	{
	//		_sntprintf(szDamage, 32, "�˺� %d", pDamageInfo->m_nHealthIncrement);
	//		CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 6, 6);
	//	}
	//}
//	else 
	if( TRUE == IsMySelf(nCasterID) && FALSE == IsMySelf(nReceiverID) )
	{
		if( 0 > pDamageInfo->m_nHealthIncrement )//�˺�
		{
			_sntprintf(szDamage, 32, " %d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 7, 7);
			AxTrace(1, 0, "%d", pDamageInfo->m_nHealthIncrement);
		}

		// 20100514 AddCodeBegin
		// ����Ǳ��˶�������ҵļ�Ѫ���ƣ�����ʾ��Ѫ��ֵ
		if (  0 < pDamageInfo->m_nHealthIncrement ) //����
		{
			_sntprintf(szDamage, 32, " +%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 0, 0);
		}
		// 20100514 AddCodeEnd

	}
	//else if (TRUE == IsMyPet(nReceiverID))
	//{
	//	if(0 > pDamageInfo->m_nHealthIncrement)//�˺�
	//	{
	//		_sntprintf(szDamage, 32, "�˺� %d", pDamageInfo->m_nHealthIncrement);
	//		CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 1, 1);
	//	}
	//}
	
	if( TRUE == IsMySelf(nReceiverID) )
	{
		if(0 < pDamageInfo->m_nHealthIncrement)//����
		{
			_sntprintf(szDamage, 32, " +%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 0, 0);
		}
		if(0 > pDamageInfo->m_nHealthIncrement)//�˺�
		{
			_sntprintf(szDamage, 32, " %d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 1, 1);
			AxTrace(4, 0, "%d", pDamageInfo->m_nHealthIncrement);
		}
		if(0 < pDamageInfo->m_nRageIncrement)//ŭ������
		{
			_sntprintf(szDamage, 32, "ŭ�� +%d", pDamageInfo->m_nRageIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 2, 2);
		}
		if(0 > pDamageInfo->m_nRageIncrement)//ŭ������
		{
			_sntprintf(szDamage, 32, "ŭ�� %d", pDamageInfo->m_nRageIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 3, 3);
		}
		if(0 < pDamageInfo->m_nManaIncrement)//mana����
		{
			_sntprintf(szDamage, 32, "���� +%d", pDamageInfo->m_nManaIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 4, 4);
		}
		//if(0 > pDamageInfo->m_nManaIncrement)//mana����
		//{
		//	_sntprintf(szDamage, 32, "%d", pDamageInfo->m_nManaIncrement);
		//	CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 5, 5);
		//}
	}
}

VOID CObject_Character::DisplayMissImmuAndSoOn(const _DAMAGE_INFO* pDamageInfo)
{
	if(pDamageInfo == NULL)
		return;
	if(GetRenderInterface() == NULL || CGameProcedure::s_pUISystem == NULL)
		return;

	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
	fVector3 fvCurPos = GetPosition();
	if( m_pMountRenderInterface != NULL )
	{
		fvCurPos.y += m_fMountAddHeight;
	}

	GetRenderInterface()->Actor_GetInfoBoardPos(fvPos, &fvCurPos);

	CHAR m_szDamage[32];
	memset(m_szDamage, 0, sizeof(m_szDamage));
	CObject *pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(!pMySelf)
	{
		return;
	}

	BOOL bFlag = FALSE;
	STRING strMsg = "";

	switch(pDamageInfo->m_nImpactID)
	{
	case MissFlag_T::FLAG_MISS:
		{
			strMsg = NOCOLORMSGFUNC("Combat_Miss");
		}
		bFlag = TRUE;
		break;

	case MissFlag_T::FLAG_IMMU:
		{
			strMsg = NOCOLORMSGFUNC("Combat_Immu");
		}
		bFlag = TRUE;
		break;

	case MissFlag_T::FLAG_ABSORB:
		{
			strMsg = NOCOLORMSGFUNC("Combat_Absorb");
		}
		bFlag = TRUE;
		break;

	case MissFlag_T::FLAG_COUNTERACT:
		{
			strMsg = NOCOLORMSGFUNC("Combat_Counteract");
		}
		bFlag = TRUE;
		break;

	case MissFlag_T::FLAG_TRANSFERED:
		{
			strMsg = NOCOLORMSGFUNC("Combat_Transfered");
		}
		bFlag = TRUE;
		break;

	default:
		break;
	}
	_sntprintf(m_szDamage, 32, "%s", strMsg.c_str());
	if(TRUE==bFlag)
	{
		CGameProcedure::s_pUISystem->AddNewBeHitBoard(false, m_szDamage, fvPos.x, fvPos.y, 8, 8 );
	}
}

BOOL CObject_Character::DoLogicEvent_Damage(const _LOGIC_EVENT *pLogicEvent)
{
	const _DAMAGE_INFO *pDamageInfo	= &(pLogicEvent->m_damage);
	switch (pDamageInfo->m_nType)
	{
	case _DAMAGE_INFO::TYPE_DIE:
		{
			Start_Dead(TRUE);
		}
		break;

	case _DAMAGE_INFO::TYPE_DROP_BOX:
		{
			//ObjID_t ObjID = pDamageInfo->m_aAttachedParams[0];
			//INT idOwner = pDamageInfo->m_aAttachedParams[1];
			//FLOAT fX = pDamageInfo->m_aAttachedParams[2]/1000.f;
			//FLOAT fZ = pDamageInfo->m_aAttachedParams[3]/1000.f;
			//// ����ItemBox
			//CTripperObject_ItemBox* pBox = (CTripperObject_ItemBox*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemBox", ObjID);
			//pBox->Initial(NULL);	
			//// ����λ��
			//pBox->SetMapPosition(fVector2(fX, fZ));
			//// ���õ�����Ĺ���
			//pBox->SetOwnerGUID(idOwner);


			//ObjID_t ObjID = pDamageInfo->m_aAttachedParams[0];
			//INT idOwner = pDamageInfo->m_aAttachedParams[1];
			//FLOAT fX = pDamageInfo->m_aAttachedParams[2]/1000.f;
			//FLOAT fZ = pDamageInfo->m_aAttachedParams[3]/1000.f;
			//// ����ItemBox
			//CTripperObject_ItemDrop* pBox = (CTripperObject_ItemDrop*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemDrop", ObjID);
			//pBox->Initial(NULL);	
			//// ����λ��
			//pBox->SetMapPosition(fVector2(fX, fZ));
			//// ���õ�����Ĺ���
			//pBox->SetOwnerGUID(idOwner);
		}
		break;

	case _DAMAGE_INFO::TYPE_EFFECT:
		{
			// ������ЧЧ��
			if( pDamageInfo->m_nImpactID != INVALID_ID )
			{
				_DIRECT_IMPACT *pDirectlyImpact = GetDirectlyImpactMgr()->GetDirectlyImpact( pDamageInfo->m_nImpactID );
				if( pDirectlyImpact != NULL )
				{
					fVector3 fvPos;
					// ȡ����Ч�󶨵�λ�ã� û�о�ֱ���õ�ǰ��λ��
					if( GetRenderInterface() != NULL && strlen( pDirectlyImpact->m_pszEffectLocator ) > 0 )
					{
						GetRenderInterface()->Actor_GetLocator( pDirectlyImpact->m_pszEffectLocator, fvPos );
					}
					else
					{
						fvPos = GetPosition();
					}

					// ��Ч
					if( strlen( pDirectlyImpact->m_pszEffect ) > 0 )
					{
						CObject_Effect *pEffectObj = (CObject_Effect*)(CObjectManager::GetMe()->NewObject( "CObject_Effect" ));
						if( pEffectObj != NULL )
						{
							SObject_EffectInit initEffect;

							initEffect.m_fvPos 			= fvPos;
							initEffect.m_fvRot 			= fVector3( 0.f, 0.f, 0.f );
							initEffect.m_pszEffectName	= pDirectlyImpact->m_pszEffect;
							initEffect.m_bLoopEffect	= FALSE;

							pEffectObj->Initial( &initEffect );
						}
					}

					// ����
					if( strlen( pDirectlyImpact->m_pszSound ) > 0 )
					{
						fVector3 fvPos = GetPosition();
						// �˽ӿ����ṩ�����õ���Ч�ӿڣ����Դ������������gfx����
						fVector3 fvGame;

						if( CGameProcedure::s_pGfxSystem->Axis_Trans(tGfxSystem::AX_GAME, fvPos, tGfxSystem::AX_GFX, fvGame) )
						{
							CSoundSystemFMod::_PlaySoundFunc( pDirectlyImpact->m_pszSound, &fvGame.x, false );
						}
					}
				}
			}
		}
		break;

	case _DAMAGE_INFO::TYPE_HEAL_AND_DAMAGE:
		{
			if (IsDisplayDamageInfo()) // ��Ϣ��ʾ��ʱ����
			{
				// ������˺�������ʾ��������
				if( pDamageInfo->m_nHealthIncrement < 0 )
				{
					SetFightState(TRUE);

					DisplayHitAnimation();

					m_bBeatFly = pDamageInfo->m_bBeatFly;
					if( m_bBeatFly )
					{
						// ���ɾ���
						m_fBeatFlyDist = pDamageInfo->m_fBeatFlyDist;
					}
				}

			// ��ʾ��Ѫ��Ϣ
			DisplayDamageBoard(pDamageInfo);
			}
			else
			{
				return FALSE;
			}
		}
		break;

	case _DAMAGE_INFO::TYPE_SKILL_TEXT:
		{
			if (IsDisplayDamageInfo()) // ��Ϣ��ʾ��ʱ����
			{
				// ��ʾδ���к�������Ϣ
				DisplayMissImmuAndSoOn(pDamageInfo);
			}
			else
			{
				return FALSE;
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}

VOID CObject_Character::DisplayHitAnimation()
{
	// ����״̬������������
	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_IDLE )
		//							&& CharacterLogic_Get(LOGIC_ACTION) != CHARACTER_LOGIC_SKILL_SEND )
	{
		INT nBaseAction = rand() % 2 + BASE_ACTION_F_BE_HIT0;

		ChangeAction( nBaseAction, 1.f, FALSE );
	}
	else
	{
		INT iCharacterType = GetCharacterType();
		if (iCharacterType  == CT_MONSTER)
		{
			if(m_bHaveHitAnimation)
			{
				PlayAnimation(BASE_ACTION_F_SYNCRETIZE, 0.0f, 3.0f);
			}
		}
		else // ����Ҿ�ֱ�Ӳ����ں϶���
		{
			//��ʱ������,����û����Դ,���Żᵼ�³��
			//PlayAnimation(BASE_ACTION_F_SYNCRETIZE, 0.0f, 3.0f);
		}
	}
}

//��:
//
// �Ϲ�,0,�Ϲ�,0
//  |	|___________________��ѭ��
//	|_______________________��������
//

VOID CObject_Character::SetChatMoodAction(STRING& strOrder)
{
	if(IsDie())
		return ;

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
		return ;

	if(pCharacterData->Get_MountID() != INVALID_ID)
		return ;

	// ����
	if(pCharacterData->Get_ModelID() != INVALID_ID)
		return ;

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		if(pCharacterData->Get_TeamFollowFlag())
			return ;

		if(pCharacterData->Get_IsInStall())
			return ;
	}

	ClearAllChatMoodAction();
	//SetChatMoodActionPlaying(FALSE);

	const CHAR splitKey = ';';

	STRING::size_type sB = 0;
	STRING::size_type sE = strOrder.find_first_of(splitKey);
	STRING::size_type sLen = strOrder.size();

	do {
		CHAT_MOOD_DATA cData;

		if(sE == STRING::npos)
		{
			//set last action
			cData.m_ActionId = atoi(strOrder.substr(sB).c_str());
			cData.m_bLoop = FALSE;
			m_listChatMoodAction.push_back(cData);	//��ӵ������б�
			break;
		}

		//copy action
		cData.m_ActionId = atoi(strOrder.substr(sB, sE-sB).c_str());

		//get loopdefin
		STRING strOp = strOrder.substr(sE+1, 1);

		//loop
		if(strOp == "1")
		{
			cData.m_bLoop = TRUE;
			sE += 3; //skip '1' adn ';'
		}
		else if(strOp == "0") //no loop
		{
			cData.m_bLoop = FALSE;
			sE += 3; //skip '0' and ';'
		}
		else
		{
			//ignore wrong ';'
			sE += 1;
		}

		m_listChatMoodAction.push_back(cData);			//��ӵ������б�
		if(sE >= sLen || cData.m_bLoop == TRUE) break;	//ֻ�����һ������������ѭ����������Ϊѭ������������finish�¼�

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strOrder.find(splitKey, sB);

	} while(TRUE);
}

VOID CObject_Character::ClearAllChatMoodAction()
{
	if(m_listChatMoodAction.empty())
		return;

	m_listChatMoodAction.erase(m_listChatMoodAction.begin(), m_listChatMoodAction.end());	
}

VOID CObject_Character::PlayChatMoodAction()
{
	//if(CharacterLogic_Get() == ENUM_CHARACTER_LOGIC::CHARACTER_LOGIC_IDLE)
	//{
	if(!m_listChatMoodAction.empty())
	{
		CHAT_MOOD_DATA* pChatMood = &(*(m_listChatMoodAction.begin()));

		if(m_pRenderInterface)
		{
			ChangeAction(pChatMood->m_ActionId, 1.0f, pChatMood->m_bLoop);
		}

		if(pChatMood->m_bLoop)		// todo_yangjun ���и��õ��߼�����ŷ���ٵ�������վ��������
			m_bNeedStandUp = TRUE;

		SetChatMoodActionPlaying(TRUE);
		m_listChatMoodAction.pop_front();
	}
	else
	{
		SetChatMoodActionPlaying(FALSE);
		m_bNeedStandUp = FALSE;
		m_bStanding = FALSE;
	}
	//}
	//else
	//{
	//	ClearAllChatMoodAction();
	//	SetChatMoodActionPlaying(FALSE);
	//	m_bNeedStandUp = FALSE;
	//	m_bStanding = FALSE;
	//}
}

BOOL CObject_Character::IsHaveChatMoodAction()
{
	return (m_listChatMoodAction.empty())?(FALSE):(TRUE);
}

VOID CObject_Character::UpdateBuffEffect(VOID)
{
	if( m_pRenderInterface != NULL )
	{
		CImpactEffectMap::iterator itCur, itEnd;
		SImpactEffect *pImpactEffect;
		UINT uImpactID;
		itEnd = m_mapImpactEffect.end();
		for(itCur = m_mapImpactEffect.begin(); itCur != itEnd; itCur++)
		{
			uImpactID		= itCur->first;
			pImpactEffect	= itCur->second;
			if(pImpactEffect != NULL)
			{
				_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact(uImpactID);
				if(pBuffImpact != NULL)
				{
					if(strlen(pBuffImpact->m_lpszEffect_Continuous) > 0 && m_pRenderInterface != NULL)
					{
						UINT uEffect = m_pRenderInterface->AddEffect(pBuffImpact->m_lpszEffect_Continuous, pBuffImpact->m_lpszBind_Continuous);
						pImpactEffect->SetEffect( uEffect );
					}
				}
			}
		}
	}
}
BOOL CObject_Character::IsDisplayDamageInfo()
{
	if (m_uDisplayInfoTimer >= DAMAGE_INFO_DISPLAY_CD) 
	{
		m_uDisplayInfoTimer = 0;
		return true;
	}
	return false;	
}

VOID CObject_Character::UpdataDisplayInfoTimer(INT nAddTime)
{
	m_uDisplayInfoTimer += nAddTime;
}


VOID CObject_Character::TickBuffElapseCount()	// ÿ�������buff��ʣ��ʱ��
{
	time_t		timeNow;

	// ������Чʣ��ʱ��
	if ( !m_mapImpactEffect.empty() )
	{
		CImpactEffectMap::iterator itCur, itEnd;
		SImpactEffect *pImpactEffect = NULL;

		itCur = m_mapImpactEffect.begin();
		itEnd = m_mapImpactEffect.end();

		while( itCur != itEnd )
		{			
			pImpactEffect = itCur->second;

			if ( pImpactEffect != NULL )
			{
				time_t	timeBuff = pImpactEffect->GetTimeNow();
				timeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();

				if ( ( timeNow - timeBuff ) >= 1000 && pImpactEffect->GetContinuance() > 0 )
				{
					pImpactEffect->SetContinuance( pImpactEffect->GetContinuance() - 1 );
					pImpactEffect->SetTimeNow( timeNow );

					CEventSystem::GetMe()->PushEvent( GE_MAINTARGET_BUFFTEXT_REFRESH );
				}

			}// if ( pImpactEffect != NULL )

			itCur++;

		}// while( itCur != itEnd )

	}// if ( !m_mapImpactEffect.empty() )
}

// �ͷż���ʱ����ɫͷ��ð��������	20100601 BLL
VOID	CObject_Character::DisplaySkillInfo( INT nSkillID )
{
	if( GetRenderInterface() == NULL || CGameProcedure::s_pUISystem == NULL )
		return;


	if( this->GetCharacterType() == CT_MONSTER && ( ((CObject_PlayerNPC*)this)->IsDisplayBoard() == FALSE) )
		return;


	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);

	DBC_DEFINEHANDLE( s_pSkillDBC, DBC_SKILL_DATA );
	const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU( nSkillID );

	// ��ͨ�����ͽ�����ȼ��ܲ���ʾ��������
	if ( pSkillData->m_bAutoRedo == TRUE || pSkillData->m_nClassByUser == 3 )
	{
		return;
	}

	if ( pSkillData != NULL && pSkillData->m_lpszName != "" )
	{
		CGameProcedure::s_pUISystem->AddNewBeHitBoard( TRUE, pSkillData->m_lpszName, fvPos.x, fvPos.y, 9, 9 );
	}
}