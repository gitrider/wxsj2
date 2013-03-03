
/** Obj_Character_Logic.cpp
 *	
 *	功能： 处理人物的逻辑
 *	
 *	逻辑概念：
 *			基本逻辑包括：休闲（属于最基础的，没有其他的逻辑时即这个）、移动、生活技能（原因是它不能移动）、死亡、摆摊
 *			行为逻辑包括：技能（聚气、引导、发招）、行动（喝药）
 *	
 *	修改： 
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

const static INT DAMAGE_INFO_DISPLAY_CD = 150; // 人物伤害信息的时间显示间隔



//======已经改了
// 处理人物的逻辑时要判断是否是瞬发的技能使用：
// 1。在没有技能使用的条件下直接使用瞬发的技能
// 2。在有技能使用的情况下只显示瞬发造成的效果， 不放到行为逻辑队列中等待
//======改为：
// 瞬发的打断前一个， 但要求前一个的特效和声音继续播放


// 压入一条指令
BOOL CObject_Character::PushCommand( const SCommand_Object *pCmd )
{
	if( pCmd == NULL )
		return FALSE;

	switch( pCmd->m_wID )
	{
	// 处理直接的效果或子弹事件
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
		// 施法者有效 或 不是上一个skill的Impact就放入事件队列中
		else
		{
			AddLogicEvent(pLogicEvent);
		}
	}
	break ;


	// 中断逻辑指令
	case OC_STOP_ACTION:
	case OC_STOP_MOVE:
	case OC_STOP_SIT:
	{
		// 创建停止逻辑命令
		CObjectCommand_StopLogic *pStopCommand = (CObjectCommand_StopLogic*)(NewObjectCommand(pCmd));
		if(pStopCommand != NULL)
		{
			CObjectCommand_Logic *pLogicCommand = NULL;

			// 动作单独处理
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
							// 玩家自身
							if( CObjectManager::GetMe()->GetMySelf() == this )
							{
								// 当位置差大于某个值时
								if(fStopToCurrentDist > DEF_CLIENT_ADJUST_POS_WARP_DIST)
								{
									// 瞬移到当前服务器对应的位置
									SetMapPosition( fvPos );
								}
								else
								{
									// 调整服务器位置到当前主角客户端对应的位置
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
								// 瞬移到当前服务器对应的位置
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


	// 逻辑指令（分为基础逻辑和行为逻辑）

	case OC_MOVE:		// 移动
	case OC_ABILITY:	// 生活技能
	{
		//temp
		if( CT_PLAYERMYSELF == GetCharacterType() && (OC_MOVE == pCmd->m_wID) )
		{
			INT ttt = GetCharacterType();
		}


		CObjectCommand_Logic *pLogicCommand = (CObjectCommand_Logic*)(NewObjectCommand(pCmd));
		if(pLogicCommand != NULL)
		{
			// 如果和当前的逻辑记数相等 或 是在它之前的逻辑命令就丢弃
			if( pLogicCommand->GetLogicCount() <= GetBaseLogicCount() )
			{
				DeleteObjectCommand(pLogicCommand);
			}
			else
			{
				CObjectCommand_Logic *pFindCommand = FindBaseLogicCommand( pLogicCommand->GetLogicCount() );
				// 没有找到对应的指令才加入，主要是防止与客户端主角的预测指令相冲突
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

		// 当前没有执行的逻辑
		if( CharacterLogic_IsStopped(LOGIC_BASE) )
		{
			ProcessLogicCommand();
		}
	}
	break ;

	case OC_ACTION:			// 动作（喝药等）
	case OC_MAGIC_SEND:		// 技能发招
	case OC_MAGIC_CHARGE:	// 技能聚气
	case OC_MAGIC_CHANNEL:	// 技能引导
	case OC_MAGIC_PREP_SEND:// 技能发招准备	//test2
	{
		CObjectCommand_Logic *pLogicCommand = (CObjectCommand_Logic*)(NewObjectCommand(pCmd));
		if( pLogicCommand != NULL )
		{
			if( !PushActionLogicCommand(pLogicCommand) )
			{
				SAFE_DELETE(pLogicCommand);
			}
		}

		// 当前没有执行的逻辑
//		if( CharacterLogic_IsStopped(LOGIC_ACTION) )	//test1 如果是瞬发的技能就忽略这个
		{
			ProcessLogicCommand();
		}
	}
	break ;

	// 干扰动作
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
		// 直接处理命令
		DoCommand(pCmd);
	}
	break;

	};

	return TRUE;
}


//================-================
// 逻辑执行操作
//================-================

// 处理缓存的指令
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


	// 判断基础逻辑是否停止
	if( CharacterLogic_IsStopped(LOGIC_BASE) )
	{
		// 执行下一条命令
		BOOL bResult = DoNextLogicCommand(LOGIC_BASE);
		if(!bResult)
		{
			// 没有可以处理的就调用休闲
			Start_Idle();
		}
	}
	// 当前基础逻辑是休闲
	else if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_IDLE )
	{
		DoNextLogicCommand(LOGIC_BASE);
	}

	// 判断行为逻辑是否停止
	if( CharacterLogic_IsStopped(LOGIC_ACTION) )	//test1 如果是瞬发的技能就忽略这个
	{
		DoNextLogicCommand(LOGIC_ACTION);	
	}

	return TRUE;
}

// 执行下一条逻辑命令
BOOL CObject_Character::DoNextLogicCommand( LOGIC_TYPE nLogicTag )
{
	// 基础逻辑
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
	// 行为逻辑
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

// 处理逻辑命令
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
// 逻辑相关操作
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

	//忽略比现在逻辑还晚的逻辑
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

	// 根据不同的逻辑来清不同的列表

	// 基础逻辑
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
	// 行为逻辑
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
	// 当前正在执行的指令
	if(m_pBaseLogicCommand != NULL && m_pBaseLogicCommand->GetLogicCount() == nLogicCount)
	{
		return m_pBaseLogicCommand;
	}

	// 缓存的指令
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

	// 缓存的指令
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
	// 基础逻辑
	if( LOGIC_BASE == nLogicTag )
	{
		if(!m_listBaseLogicCommand.empty())
		{
			CObjectCommand_Logic *pCmd = m_listBaseLogicCommand.front();
			return pCmd;
		}
	}
	// 行为逻辑
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
	// 基础逻辑
	if( LOGIC_BASE == nLogicTag )
	{
		if(!m_listBaseLogicCommand.empty())
		{
			CObjectCommand_Logic *pCmd = m_listBaseLogicCommand.front();
			m_listBaseLogicCommand.pop_front();
			return pCmd;
		}
	}
	// 行为逻辑
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
	// 逻辑记数还在存储的之前就立即执行
	if( GetBaseLogicCount() >= nLogicCount )
	{
		return TRUE;
	}

	return FALSE;
}
BOOL CObject_Character::IsActionLogicReady( INT nLogicCount ) const
{
	// 逻辑记数还在存储的之前就立即执行
	if( GetActionLogicCount() > nLogicCount )
	{
		return TRUE;
	}
	// 是当前的逻辑
	else if( GetActionLogicCount() == nLogicCount )
	{
		// 这是真对使用技能的
		if( CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_SEND )
		{
			// 判断攻击特效是否显示完成
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
	// 设置某逻辑状态的开始
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
		// 标记基础逻辑停止
		m_bCharacterBaseLogicStopped = TRUE;

	}
	else
	{
		// 标记行为逻辑停止
		m_bCharacterActionLogicStopped = TRUE;
	}


	// 如果坐下了， 改变为站起来
	if(GetCharacterData() &&GetCharacterData()->IsSit())
	{
		StandUp();
	}

	// 设置进度条
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_HIDE);
	}

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_ABILITY_ACTION )
	{
		if(m_pLifeAbilitySound != NULL)
		{
			// 停止生活技能声音
			m_pLifeAbilitySound->Stop();
			CGameProcedure::s_pSoundSystem->Source_Destroy(m_pLifeAbilitySound);
			m_pLifeAbilitySound = 0;
		}
	}

	// 停止移动的声音
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
// Enter逻辑操作
//================-================

//1 休闲（最基础的逻辑状态）
BOOL CObject_Character::Start_Idle(VOID)
{
	// 如果是跳的状态
	if( m_bJumping )
	{
		// 不做处理
	}
	// 如果存在表情动作
	else if( IsHaveChatMoodAction() )
	{
		// 播放
		PlayChatMoodAction();
	}
	// 非跳、无表情动作
	else
	{
		// 根据行为改变动作
		BOOL bFightState, bSit;
		INT nBaseAction;
		bFightState	= IsFightState();		// 战斗状态
		bSit = GetCharacterData() ? GetCharacterData()->IsSit() : FALSE;	// 是否坐下
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
		// 改变休闲动作
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
						if(pModel->getCurrentAnimName().find("攻击") != STRING::npos )
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

	// 保存数据
	m_paramLogic_Idle.m_uIdleInterval		= CalcIdleRandInterval();
	m_paramLogic_Idle.m_uLastIdleMotionTime	= CGameProcedure::s_pTimeSystem->GetTimeNow();
	m_paramLogic_Idle.m_uStartTime			= CGameProcedure::s_pTimeSystem->GetTimeNow();

	// 标记休闲逻辑状态
	CharacterLogic_Set( CHARACTER_LOGIC_IDLE, LOGIC_BASE );

	return TRUE;
}

//1 移动
BOOL CObject_Character::Start_Move( CObjectCommand_Logic *pLogicCommand )
{
	// 执行条件判断

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
	// 逻辑状态不是移动
	if( CHARACTER_LOGIC_MOVE != ePrevLogic )
	{
		// 非跳跃状态
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

			// 改变动作

			m_eChangeAnimLogicType = LOGIC_BASE;
			m_nMoveAnimId = nBaseAction;
//			m_nBaseAnimId = nBaseAction;

			// 判断是主角的话
//			if( CT_PLAYERMYSELF == GetCharacterType() )
			{
				// 设置移动状态为真， 去改变坐骑的动作
				m_bMoveState = TRUE;
			}

			// 判断有无行为逻辑
			if( !CharacterLogic_IsStopped(LOGIC_ACTION) )
			{
				// 是否为... 此处的判断是为了来处理进行什么行为逻辑时才改变动作
//				if( CHARACTER_LOGIC_ == CharacterLogic_Get() )
				{
					// 存在行为逻辑时只改变下部的动作为移动
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

	// 设置移动为当前逻辑命令
	SetBaseLogicCommand(pLogicCommand);
//	SetLogicCount( pLogicCommand->GetLogicCount() );
	SetBaseLogicCount( pLogicCommand->GetLogicCount() );

	// 设置最后一次更改位置的逻辑数
	SetLastModifyPosLogicCount(pLogicCommand->GetLogicCount());

	// 保存数据（移动节点表）
	const WORLD_POS *pNodeList = pMoveCommand->GetNodeList();
	m_paramLogic_Move.m_nCurrentNodeIndex	= 0;
	m_paramLogic_Move.m_posSaveStart		= WORLD_POS(GetPosition().x, GetPosition().z);
	m_paramLogic_Move.m_posSaveTarget		= pNodeList[0];

	// 设置逻辑状态
	CharacterLogic_Set( CHARACTER_LOGIC_MOVE, LOGIC_BASE );

	// 创建移动声音，当技能音效开关打开时		20100714 BLL
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

//1 生活技能
BOOL CObject_Character::Start_Ability( CObjectCommand_Logic *pLogicCommand )
{
	if(pLogicCommand == NULL)
		return FALSE;

	if(pLogicCommand->GetCommandID() != OBJECT_COMMAND_ABILITY)
		return FALSE;

	if ( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_Ability *pAbilityCommand = (CObjectCommand_Ability*)pLogicCommand;

	// 读表取出数据
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

		// 记录起始时间和总共需要的时间
		if(pDefine != NULL && pDefine->nProficiencyTime > 0)
			uTotalTime = (UINT)(pDefine->nProficiencyTime);
	}

	if(uTotalTime == 0)
	{
		// 记录起始时间和总共需要的时间
		if(pAbilityDef->nTimeOperation > 0)
			uTotalTime = (UINT)(pAbilityDef->nTimeOperation);
	}

	if(uTotalTime == 0)
		return FALSE;

	// 方向
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

	//// 隐藏武器
	//if(GetCharacterType() == CT_PLAYEROTHER || GetCharacterType() == CT_PLAYERMYSELF)
	//	((CObject_PlayerOther*)this)->EquipItem_BodyLocator( HEQUIP_WEAPON, pAbilityDef->nItemVisualLocator);

	m_eChangeAnimLogicType = LOGIC_BASE;
//	m_nBaseAnimId = pAbilityDef->nAnimOperation;

	// 更改人物动作
	ChangeAction( pAbilityDef->nAnimOperation, GetLogicSpeed(), TRUE );

	// 更新生活技能声音
	if(CObjectManager::GetMe()->GetMySelf() == this)
	{
		// 创建声音，当技能音效打开时		2010714 BLL
		STRING strRet;
		CVariableSystem::GetMe()->GetVariable( CSoundSystemFMod::szSKSoundVar, strRet );
		if( !m_pLifeAbilitySound && strRet == "1" )
		{
			m_pLifeAbilitySound = CGameProcedure::s_pSoundSystem->Source_Create(CSoundSourceFMod::type_skill, FALSE, FALSE, &m_pLifeAbilitySound );
			KLAssert(m_pLifeAbilitySound);

		}

		// 根据生活技能开始播放不同声音
		switch(pAbilityCommand->GetAbilityID())
		{
		case 1:			//烹饪
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(16+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
		case 2:			//中医
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(17+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 3:			//加工
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(20+59));//还有18
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 4:			//铸剑
		case 5:			//制衣
		case 6:			//艺术制作
		case 7:			//工程学
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(18+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 8:			//采矿
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(12+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 9:			//采药
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(10+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 10:		//钓鱼
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

	// 进度条
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pAbilityDef->szProgressbarName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 0);
	}

	// 设置人物逻辑为“生活技能进行中...”
	CharacterLogic_Set( CHARACTER_LOGIC_ABILITY_ACTION, LOGIC_BASE );

	return TRUE;
}

//1 摆摊
BOOL CObject_Character::Start_Stall( BOOL bPlayAni )
{
	m_eChangeAnimLogicType = LOGIC_BASE;
//	m_nBaseAnimId = BASE_ACTION_N_SIT_DOWN;

	// 改变动作
	ChangeAction( BASE_ACTION_N_SIT_DOWN, GetLogicSpeed(), FALSE );

	m_paramLogic_Stall.m_nCurrentAnimation	= SLogicParam_Stall::STALL_ANIMATION_SITDOWN;

	CharacterLogic_Set( CHARACTER_LOGIC_STALL, LOGIC_BASE );
	return TRUE;
}

//1 死亡
BOOL CObject_Character::Start_Dead( BOOL bPlayDieAni )	// 在 GCNewMonster_Death 消息时次参数是 FALSE
{
	// 停止逻辑事件
	CharacterLogic_Stop(FALSE, LOGIC_BASE);
	CharacterLogic_Stop(FALSE, LOGIC_ACTION);
	// 清空逻辑命令列表
	CleanupLogicCommandList(LOGIC_BASE);
	CleanupLogicCommandList(LOGIC_ACTION);


	//// 产生掉落包
	//if(m_bDropBox_HaveData)
	//{
	//	// 创建ItemBox
	//	CTripperObject_ItemBox* pBox = (CTripperObject_ItemBox*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemBox", m_nDropBox_ItemBoxID);
	//	pBox->Initial(NULL);	
	//	// 设置位置
	//	pBox->SetMapPosition( fVector2(m_posDropBox_CreatePos.m_fX, m_posDropBox_CreatePos.m_fZ));
	//	// 设置掉落箱的归属
	//	pBox->SetOwnerGUID(m_DropBox_OwnerGUID);

	//	m_bDropBox_HaveData = FALSE;
	//}

	//// 产生掉落包

	//if(m_bDropItem_HaveData)
	//{

	//	// 创建掉落物品
	//	CTripperObject_ItemDrop* pDrop = (CTripperObject_ItemDrop*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemDrop", m_nDropItem_ItemDropID);
	//	pDrop->SetDropCanPick(m_bIsCanPick);
	//	
	//    pDrop->Initial(NULL);	

	//	// 设置位置
	//	pDrop->SetMapPosition( fVector2(m_posDropItem_CreatePos.m_fX, m_posDropItem_CreatePos.m_fZ));

 //
	//	// 设置掉落箱的归属
	//	pDrop->SetOwnerGUID(m_DropItem_OwnerObjId);

	//	pDrop->SetProtectTime(m_ProtectTime);
	//	pDrop->SetStartTime(timeGetTime());
	//	m_bDropItem_HaveData = FALSE;
	//}

	// 保存死亡位置
	m_paramLogic_Dead.m_DeadPos = GetPosition();
	m_paramLogic_Dead.m_CurrPos = m_paramLogic_Dead.m_DeadPos;

	// 计算目标位置
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

	// 计算方向
	m_paramLogic_Dead.m_TarDir = m_paramLogic_Dead.m_DeadPos - targetPos;
	m_paramLogic_Dead.m_TarDir.normalise();

	// 最终位置
	fVector3 vFlyLength;
	vFlyLength.x = m_paramLogic_Dead.m_TarDir.x * m_fBeatFlyDist;		// 此距离从表中读出， 通过服务器发过来
	vFlyLength.y = m_paramLogic_Dead.m_TarDir.y * m_fBeatFlyDist;
	vFlyLength.z = m_paramLogic_Dead.m_TarDir.z * m_fBeatFlyDist;

	m_paramLogic_Dead.m_TarPos = vFlyLength + m_paramLogic_Dead.m_DeadPos;
	m_paramLogic_Dead.m_bDeadIsEnd = FALSE;
	m_paramLogic_Dead.m_bDeadFlyBegin = FALSE;
	m_paramLogic_Dead.m_bDeadFlyEnd = FALSE;
	m_paramLogic_Dead.m_bDeadBegin	= FALSE;
	m_paramLogic_Dead.m_bDeadEnd	= FALSE;

	m_paramLogic_Dead.m_nCadaverAction = -1;


	// 必需下马
	Dismount();

	// 设置逻辑状态
	CharacterLogic_Set( CHARACTER_LOGIC_DEAD, LOGIC_BASE );


	INT nBaseAction = BASE_ACTION_INVALID;


//	击飞_01=尸体_01
//	击飞_02=尸体_02
//
//	死亡_01=尸体_03
//	死亡_02=尸体_04


	// 是否播放死亡动作
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
	// 是否击飞
	else
	{
		nBaseAction = rand() % 2 + BASE_ACTION_F_DIEFLY0;
		// 随机播放击飞动作
		ChangeAction( nBaseAction, GetLogicSpeed(), FALSE, ACTION_BODY );		// , 0.0f

		m_paramLogic_Dead.m_bDeadFlyBegin = TRUE;

		if( BASE_ACTION_F_DIEFLY0 == nBaseAction )
			m_paramLogic_Dead.m_nCadaverAction = BASE_ACTION_F_CADAVER0;
		else
			m_paramLogic_Dead.m_nCadaverAction = BASE_ACTION_F_CADAVER1;
	}

	// 清掉战斗状态
	SetFightState( FALSE );
	// 标记更新小地图
	CGameProcedure::m_bWaitNeedFreshMinimap = true;

	// 设置选择优先级
	if(m_pRenderInterface)
	{
		if(CT_PLAYEROTHER == GetCharacterType())
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLAYER_DEADBODY);
		else
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_CREATURE_DEADBODY);
	}

	// 清掉所有的伤害信息
	RemoveAllImpact();

	if(this == CObjectManager::GetMe()->GetMySelf())
	{
		STRING strTemp = COLORMSGFUNC("DIE_YOU_DIED_MSG");
		ADDTALKMSG(strTemp);
	}

	return TRUE;
}



//2 做动作
BOOL CObject_Character::Start_Action( CObjectCommand_Logic *pLogicCommand )
{
	// 执行条件判断

	if( pLogicCommand == NULL )
		return FALSE;

	if( pLogicCommand->GetCommandID() != OBJECT_COMMAND_ACTION )
		return FALSE;

	CObjectCommand_Action *pActionCommand = (CObjectCommand_Action*)pLogicCommand;
	if( pActionCommand->GetActionID() == INVALID_ID )
		return FALSE;

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;

	// 在不是跳的状态下才可以动作
	if( !m_bJumping )
	{
		m_eChangeAnimLogicType = LOGIC_ACTION;
//		m_nActionAnimId	= pActionCommand->GetActionID();
		ChangeAction( pActionCommand->GetActionID(), GetLogicSpeed(), FALSE );
	}

	// 设置当前逻辑命令
	SetActionLogicCommand( pLogicCommand );
//	SetLogicCount( pLogicCommand->GetLogicCount() );
	SetActionLogicCount( pLogicCommand->GetLogicCount() );

	// 标记动作逻辑状态
	CharacterLogic_Set( CHARACTER_LOGIC_ACTION, LOGIC_ACTION );

	return TRUE;
}

//2 技能聚气
BOOL CObject_Character::Start_MagicCharge( CObjectCommand_Logic *pLogicCommand )
{
	// 执行条件判断

	if( pLogicCommand == NULL )
		return FALSE;

	if( pLogicCommand->GetCommandID() != OBJECT_COMMAND_MAGIC_CHARGE )
		return FALSE;

	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_MagicCharge *pMagicChargeCommand = (CObjectCommand_MagicCharge*)pLogicCommand;
	if(pMagicChargeCommand->GetTotalTime() == 0)
		return FALSE;

	// 取得技能数据
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(pMagicChargeCommand->GetMagicID());
	if( pClientSkill == NULL )
		return FALSE;

	// 通过技能的鼠标选择类型来设置人物的朝向
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

	// 改变动作
	INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(pMagicChargeCommand->GetMagicID(), 0, FALSE);
	if( nRandAnimID != -1 )
	{
		m_eChangeAnimLogicType = LOGIC_ACTION;
//		m_nActionAnimId	= nRandAnimID;
//		ChangeAction( nRandAnimID, GetLogicSpeed(), TRUE );

		BOOL bAnimLoop = TRUE;
		// 判断基础逻辑
		if( !CharacterLogic_IsStopped(LOGIC_BASE) )
		{
			// 是否为移动
			if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
			{
				// 存在移动只改变上部动作
				ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
			}
			else 
			{
				ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
			}
		}
		// 无基础逻辑
		else
		{
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}
	}

	// 显示进度
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pClientSkill->m_lpszName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 0);
	}

	// 设置当前的逻辑命令
	SetActionLogicCommand(pLogicCommand);
//	SetLogicCount(pLogicCommand->GetLogicCount());
	SetActionLogicCount(pLogicCommand->GetLogicCount());

	// 记时
	m_paramLogic_MagicCharge.m_nSkillID			= pMagicChargeCommand->GetMagicID();
	m_paramLogic_MagicCharge.m_uCurrentTime		= 0;

	if(pClientSkill->m_nFriendness < 0)
	{
		SetFightState(TRUE);
	}

	// 设置逻辑状态
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_GATHER, LOGIC_ACTION );

	return TRUE;
}

//2 技能引导
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

	// 技能数据 
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(pMagicChannelCommand->GetMagicID());
	if( pClientSkill == NULL )
		return FALSE;

	// 根据鼠标选择类型改变人物朝向
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

	// 改变动作
	INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(pMagicChannelCommand->GetMagicID(), 0, FALSE);
	if( nRandAnimID != -1 )
	{
		m_eChangeAnimLogicType = LOGIC_ACTION;	
//		m_nActionAnimId	= nRandAnimID;
//		ChangeAction( nRandAnimID, GetLogicSpeed(), TRUE );

		BOOL bAnimLoop = TRUE;
		// 判断基础逻辑
		if( !CharacterLogic_IsStopped(LOGIC_BASE) )
		{
			// 是否为移动
			if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
			{
				// 存在移动只改变上部动作
				ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
			}
			else 
			{
				ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
			}
		}
		// 无基础逻辑
		else
		{
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}
	}

	// 显示进度
	if( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pClientSkill->m_lpszName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 1.f);
	}

	// 设置当前的逻辑命令
	SetActionLogicCommand(pLogicCommand);
//	SetLogicCount(pLogicCommand->GetLogicCount());
	SetActionLogicCount(pLogicCommand->GetLogicCount());

	// 记录时间
	m_paramLogic_MagicChannel.m_nSkillID	 = pMagicChannelCommand->GetMagicID();
	m_paramLogic_MagicChannel.m_uCurrentTime = pMagicChannelCommand->GetTotalTime();

	if(pClientSkill->m_nFriendness < 0)
	{
		SetFightState(TRUE);
	}

	// 改变逻辑状态
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



	// 获取技能数据 
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( iMagicId );
	if( pClientSkill == NULL )
		return FALSE;


	// 改变动作为技能发招预备

	INT nRandAnimID = BASE_ACTION_FP;	// GetSkillDataMgr()->GetRandAnim( m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE );
	BOOL bAnimLoop = FALSE;

	// 判断基础逻辑
	if( !CharacterLogic_IsStopped(LOGIC_BASE) )
	{
		// 是否为移动
		if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
		{
			// 存在移动只改变上部动作
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
		}
		else 
		{
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}
	}
	// 无基础逻辑
	else
	{
		ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
	}


	SetActionLogicCommand( pLogicCommand );
	SetActionLogicCount( pLogicCommand->GetLogicCount() );

	// 设置逻辑状态
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_PREP_SEND, LOGIC_ACTION );


	return TRUE;
}

//2 技能发招
BOOL CObject_Character::Start_MagicSend( CObjectCommand_Logic *pLogicCommand )
{
	// 执行条件判断
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

	// 获取技能数据 
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( iMagicId );
	if( pClientSkill == NULL )
		return FALSE;

	// 根据技能的鼠标选择类型来设置人物的朝向
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

					//如果角色已经死去了,那么就不做动作了
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
					//找不到这个目标的话,也不做动作.
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

	// 保存数据
	m_paramLogic_MagicSend.m_nSaveMagicID					= pMagicSendCommand->GetMagicID();
	m_paramLogic_MagicSend.m_bDoNextAction_Concatenation	= FALSE;
	m_paramLogic_MagicSend.m_bBeAttackEffectShowed			= FALSE;
	m_paramLogic_MagicSend.m_bCanBreak						= FALSE;
	m_paramLogic_MagicSend.m_nActionCount					= GetSkillDataMgr()->GetAnimCount( iMagicId );
	m_paramLogic_MagicSend.m_uAnimationTime					= 0;
	m_paramLogic_MagicSend.m_uAnimationEndElapseTime		= 0;

	BOOL bOldSkillID = FALSE;
	// 技能逻辑处理
	switch (CharacterLogic_Get(LOGIC_ACTION))
	{
		// 聚气
	case CHARACTER_LOGIC_SKILL_GATHER:
		{
			CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_HIDE);
		}
		break;
		// 引导
	case CHARACTER_LOGIC_SKILL_LEAD:
		{
		}
		break;
		// 连续技
	case CHARACTER_LOGIC_SKILL_SEND:
		{
			// 判断是否为上一次使用的连招, 上次使用的技能的id被保存起来了
			if (m_paramLogic_MagicSend.m_nSaveMagicID == iMagicId )	
			{
				bOldSkillID = TRUE;
			}
		}
		break;
	default:
		break;
	}


	// 如果是连招的特殊处理
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


	// 改变动作
	if( bPlayAction || IsActionAnimationEnd() )
	{
		INT nRandAnimID = GetSkillDataMgr()->GetRandAnim( m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE );


		BOOL bAnimLoop = FALSE;
		BOOL bChangeOK = FALSE;

		m_eChangeAnimLogicType = LOGIC_ACTION;
		
		// 判断基础逻辑
		if( !CharacterLogic_IsStopped(LOGIC_BASE) )
		{
			// 是否为移动
			if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
			{
				// 存在移动只改变上部动作
				bChangeOK = ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
			}
			else 
			{
				bChangeOK = ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
			}
		}
		// 无基础逻辑
		else
		{
			bChangeOK = ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}	

		if (TRUE == bChangeOK)
		{
			Fairy::Skill* pSkill = m_pRenderInterface->GetCurrentSkill();
			if (pSkill)
			{
				// 技能带有子弹效果
				unsigned short  nFlowNum = pSkill->getNumAnimationBulletFlows();
				if (nFlowNum > 0)
				{
					const ObjID_t* pTargetIDList = pMagicSendCommand->GetTargetObjID();
					INT nTargetNum = pMagicSendCommand->GetTargetNum();
					if(nTargetNum> 0)// 向技能内添加目标列表信息
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
		// 物品使用不触发其他的技能公共cooldowm 
		if( !pClientSkill->m_bAutoRedo && pClientSkill->m_nClassByUser != 3 )	// 自动连续释放 和 使用者类型分类
			CActionSystem::GetMe()->UpdateCommonCoolDown(INVALID_ID);
	}

	// 设置技能发招为当前的逻辑命令
	SetActionLogicCommand( pLogicCommand );
//	SetLogicCount( pLogicCommand->GetLogicCount() );
	SetActionLogicCount( pLogicCommand->GetLogicCount() );

	// 负面技能,设置战斗状态
	if( pClientSkill->m_nFriendness < 0 )
	{
		SetFightState( TRUE );
	}

	// 设置逻辑状态
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_SEND, LOGIC_ACTION );

//	m_UseSkillTag = TRUE;

	return TRUE;
}




//================-================
// 逻辑Tick操作
//================-================

//1 休闲
BOOL CObject_Character::Tick_Idle( UINT uElapseTime )
{
	m_paramLogic_Idle.m_uStartTime += uElapseTime;
	if(GetCharacterData() && GetCharacterData()->IsSit() && IsHaveChatMoodAction())
	{
		FreshAnimation();
	}

	return TRUE;
}

//1 移动
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

	// 当前位置与当前的目标路径点路径长度的平方
	fVector2 fvDistToTarget = fvTargetPos2D - fvCurrentPos2D;
	FLOAT fDistToTargetSq = fvDistToTarget.x * fvDistToTarget.x + fvDistToTarget.y * fvDistToTarget.y;
	FLOAT fDistToTarget = sqrt(fDistToTargetSq) ;

	// 这一帧可移动的路径长度
	FLOAT fElapseTime	= ((FLOAT)(uElapseTime))/1000.0f;
	FLOAT fSpeed		= GetCharacterData()->Get_MoveSpeed();

	//616======* 单机人物移动速度
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
	//修复角色行走会突然停下来的BUG LAIM ---〉	return FALSE;
	}


	if(fDistToTarget < 0.01f)
	{
		// 判断是主角的话, 设置移动状态为真， 去改变坐骑的动作
//		if( CT_PLAYERMYSELF == GetCharacterType() )
		{
			m_bMoveState = FALSE;
		}

		// 20100415 AddCodeBegin
		// 自动寻路到达后向服务器发任务列表请求，打开NPC对话框

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
				// 走到了
				bStopMove	= TRUE;
				fvSetToPos	= fvTargetPos2D;
				fSetToDir	= KLU_GetYAngle(fvCurrentPos2D, fvTargetPos2D);
				break;
			}
			else
			{
				// 改变m_paramLogic_Move中的各个值
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

			// 需要位置修正，防止因为误差走入阻挡区内部
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

	// 判断地面的类型是否发生变化， 不变就不改变声音， 继续循环
	bool bstate = false;
	if(mTerrainType != soundType)
	{
		mTerrainType = soundType;
		if(-1 != mTerrainType)
			bstate = true;
	}

	// 根据地面播放不同声音（只播放主角的声音）
	if(!bStopMove && (CObjectManager::GetMe()->GetMySelf() == this))	// && bstate 
	{
		INT id = -1;
		switch(soundType)
		{
		// 普通地形
		case GENERAL_TER:
				id = 0;
				break;
		// 草地
		case GRASS_TER:
				id = 1;
				break;
		// 土地
		case GROUND_TER:
				id = 2;
				break;
		// 雪地
		case SNOW_TER:
				id = 3;
				break;
		// 水
		case WATER_TER:
				id = 4;
				break;
		// 石头
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
		// 判断是主角的话, 设置移动状态为真， 去改变坐骑的动作
//		if( CT_PLAYERMYSELF == GetCharacterType() )
		{
			m_bMoveState = FALSE;
		}

		// 20100415 AddCodeBegin
		// 自动寻路到达后向服务器发任务列表请求，打开NPC对话框

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

//1 生活技能
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
			CharacterLogic_Stop(TRUE, LOGIC_BASE);	// 完成并结束
		}
		else
		{
			CharacterLogic_Stop(FALSE, LOGIC_BASE);	// 未完成结束
		}
	}

	return TRUE;
}

//1 摆摊
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

//1 死亡
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

	// 如果已经结束了就不继续执行位移了
	if( m_paramLogic_Dead.m_bDeadIsEnd )
		return TRUE;


	// 是否击飞 或 主角不被击飞
	if( !m_bBeatFly || ( CT_PLAYERMYSELF == GetCharacterType() ) )
	{
		if( m_paramLogic_Dead.m_bDeadEnd )
		{
			// 不击飞直接切换成死亡动作
			FLOAT height = 0.0f;
			CWorldManager::GetMe()->GetTerrainHeight_GamePos(m_paramLogic_Dead.m_CurrPos.x, m_paramLogic_Dead.m_CurrPos.z, height);
			m_paramLogic_Dead.m_CurrPos.y = height;
			//SetPosition( m_paramLogic_Dead.m_CurrPos );


			// 设为尸体动作
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


	// 只有击飞的情况下才处理下面的
	if( m_bBeatFly )	// && !m_paramLogic_Dead.m_bDeadEnd
	{
		// 计算距离（目标 - 当前）
		fVector3 fvDistToTarget = m_paramLogic_Dead.m_TarPos - m_paramLogic_Dead.m_CurrPos;
		FLOAT fDistToTargetSq = fvDistToTarget.x * fvDistToTarget.x + fvDistToTarget.z * fvDistToTarget.z;
		FLOAT fDistToTarget = sqrt(fDistToTargetSq) ;
		if( fDistToTarget < 0.01f )
		{
			return TRUE;
		}


		// 测试速度
		FLOAT fSpeed = 15;


		// 这一帧可移动的路径长度
		FLOAT fElapseTime = ((FLOAT)(CGameProcedure::s_pTimeSystem->GetDeltaTime()))/1000.0f;
		FLOAT fMoveDist = fSpeed * fElapseTime;

		// 判断是否到达目标位置 或 击飞动作已经结束
		if( (fMoveDist > fDistToTarget) || (TRUE == m_paramLogic_Dead.m_bDeadFlyEnd) )
		{
			// 产生震荡效果
			CGameProcedure::s_pGfxSystem->Camera_VibrateOneTime(2.0f, 1.0f, 1);

			// 改变到尸体的动作
			//ChangeAction( m_paramLogic_Dead.m_nCadaverAction, GetLogicSpeed(), TRUE, ACTION_BODY, 0.0f );
			StopCurrentAction();

			// 创建血迹
			CreateBloodStain();

			// 设置到目标位置
			//SetPosition( m_paramLogic_Dead.m_CurrPos );

			// 标记完成
			m_paramLogic_Dead.m_bDeadIsEnd = TRUE;

			RemoveAllLogicEvent();
		}
		else
		{
			// 计算偏移
			FLOAT fDistX = ( fMoveDist * ( m_paramLogic_Dead.m_TarPos.x - m_paramLogic_Dead.m_CurrPos.x) ) / fDistToTarget;
			FLOAT fDistZ = ( fMoveDist * ( m_paramLogic_Dead.m_TarPos.z - m_paramLogic_Dead.m_CurrPos.z) ) / fDistToTarget;

			m_paramLogic_Dead.m_CurrPos.x = m_paramLogic_Dead.m_CurrPos.x + fDistX;
			m_paramLogic_Dead.m_CurrPos.y = m_paramLogic_Dead.m_CurrPos.z + fDistZ;

			// 获取地面高度
			FLOAT height = 0.0f;
			CWorldManager::GetMe()->GetTerrainHeight_GamePos(m_paramLogic_Dead.m_CurrPos.x, m_paramLogic_Dead.m_CurrPos.z, height);
			m_paramLogic_Dead.m_CurrPos.y = height;

			SetPosition( m_paramLogic_Dead.m_CurrPos );
		}
	}

	return TRUE;
}




//2 动作
BOOL CObject_Character::Tick_Action( UINT uElapseTime )
{
	if(IsActionAnimationEnd() || !IsActionLogicCommandListEmpty())
	{
		CharacterLogic_Stop(TRUE, LOGIC_ACTION);
	}

	return TRUE;
}

//2 技能聚气
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

	// 人物的方向
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

//2 技能引导
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

	// 人物的方向
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
	// 增加记时器， 判断超时就转到休闲

	return TRUE;
}

//2 技能发招
BOOL CObject_Character::Tick_MagicSend( UINT uElapseTime )
{
	// 取得技能数据
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
	// 动作时间长于5秒
	if( m_paramLogic_MagicSend.m_uAnimationTime > 5000 )
	{
		m_UseSkillTag = FALSE;
		CharacterLogic_Stop(TRUE, LOGIC_ACTION);
		return TRUE;
	}

	// 根据动作的回调函数判断
	if( IsActionAnimationEnd() )
	{
		m_paramLogic_MagicSend.m_uAnimationEndElapseTime += uElapseTime;

		BOOL bStop = TRUE;
		INT nSkillActionType = pClientSkill->m_nSkillActionType;

		// 如果行为逻辑命令列表不为空 并且 技能为连招
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

	// 根据能否结束逻辑标记判断
	if( m_paramLogic_MagicSend.m_bCanBreak && !IsActionLogicCommandListEmpty() )
	{
		m_UseSkillTag = FALSE;
		CharacterLogic_Stop(TRUE, LOGIC_ACTION);
	}

	return TRUE;
}



//================-================
// 事件相关操作
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

// 立即执行此指令
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

			// buff的倒计时参数		20100531 BLL
			INT		nTimeCount; 
			time_t	nTimeNow;		

			idImpact	= (ImpactID_t)(pCmd->m_adwParam[0]);
			nSN			= pCmd->m_adwParam[1];
			nNumOflayer	= pCmd->m_anParam[2];
			nCreatorID	= pCmd->m_anParam[3];
			bEnable		= pCmd->m_abParam[4];
			nDelayTime	= pCmd->m_anParam[5];

			// 获取buff的倒计时秒数		20100531 BLL
			nTimeCount = pCmd->m_anParam[6];

			// 更新或生成
			if( bEnable )
			{
				// 需要做时间延迟，将其加入等待队列。
				if( nDelayTime > 0 )
				{
					CImpactEffectMap::iterator it = m_mapImpactDelay.find(nSN);// 序列号检测
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

						// 设置持续buff的倒计时参数		20100531 BLL
						pImpactEffect->SetContinuance( nTimeCount );
						pImpactEffect->SetTimeNow( uTimeNow );
					}					
				}
				// 没有延迟，直接执行。
				else
				{
					InsertImpact( nSN, idImpact, nTimeCount, nCreatorID, nNumOflayer, TRUE );

					// 20100428 AddCodeBegin
					// 发送角色释放buff需要刷新消息，由targetframe响应，并更新buff图标
					CGameProcedure::s_pEventSystem->PushEvent( GE_MAINTARGET_BUFF_REFRESH );
					// 20100428 AddCodeEnd
				}				
			}
			// 删除
			else
			{
				RemoveImpact( nSN );

				// 20100428 AddCodeBegin
				// 发送角色释放buff需要刷新消息，由targetframe响应，并更新buff图标
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
		// 主角不走回头路，不瞬移
		BOOL bAdjustPos = FALSE;
		WORLD_POS posAdjust;

		// 当前索引超出了移动命令的节点数
		if(m_paramLogic_Move.m_nCurrentNodeIndex >= pMoveCommand->GetNodeCount())
		{
			const WORLD_POS *paPos = pMoveCommand->GetNodeList();
			INT nEndNodeIndex = pMoveCommand->GetNodeCount() - 1;
			// 取最后一个位置
			posAdjust = paPos[nEndNodeIndex];

			bAdjustPos	= TRUE;
		}
		// 当前为最后节点
		else if(m_paramLogic_Move.m_nCurrentNodeIndex == pMoveCommand->GetNodeCount() - 1)
		{
			const WORLD_POS *paPos = pMoveCommand->GetNodeList();
			INT nEndNodeIndex = pMoveCommand->GetNodeCount() - 1;
			WORLD_POS posCommandTarget = paPos[nEndNodeIndex];

			FLOAT fLenCSTarget = fabsf(m_paramLogic_Move.m_posSaveTarget.m_fX - posCommandTarget.m_fX)
				+ fabsf(m_paramLogic_Move.m_posSaveTarget.m_fZ - posCommandTarget.m_fZ);

			// 目标点变动了
			if(fLenCSTarget > 0.01f)
			{
				FLOAT fSaveToServerDist = KLU_GetDist(fVector2(m_paramLogic_Move.m_posSaveTarget.m_fX, m_paramLogic_Move.m_posSaveTarget.m_fZ),
					fVector2(posCommandTarget.m_fX, posCommandTarget.m_fZ));

				FLOAT fSaveToCurrentDist = KLU_GetDist(fVector2(m_paramLogic_Move.m_posSaveTarget.m_fX, m_paramLogic_Move.m_posSaveTarget.m_fZ),
					fVector2(GetPosition().x, GetPosition().z));

				// 这里忽略了服务器传过来的目标点不在路径上的情况
				if(fSaveToServerDist - fSaveToCurrentDist >= 0.0f)
				{
					posAdjust = posCommandTarget;
					bAdjustPos = TRUE;
				}

				m_paramLogic_Move.m_posSaveTarget.m_fX	= posCommandTarget.m_fX;
				m_paramLogic_Move.m_posSaveTarget.m_fZ	= posCommandTarget.m_fZ;
			}
		}

		// 需要调整
		if(bAdjustPos)
		{
			// 是玩家自身
			if(CObjectManager::GetMe()->GetMySelf() == this)
			{
				// 当位置差大于某个值时
				FLOAT fAdjustToCurrentDist = KLU_GetDist(fVector2(posAdjust.m_fX, posAdjust.m_fZ),
					fVector2(GetPosition().x, GetPosition().z));

				if(fAdjustToCurrentDist > DEF_CLIENT_ADJUST_POS_WARP_DIST)
				{
					// 瞬移到当前服务器对应的位置
					fVector2 fvPos(posAdjust.m_fX, posAdjust.m_fZ);
					SetMapPosition( fvPos );
				}
				else
				{
					// 调整服务器位置到当前主角客户端对应的位置
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
					// 瞬移到当前服务器对应的位置
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

	// 变身
	if(pCharacterData->Get_ModelID() != INVALID_ID)
	{
		return FALSE;
	}

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		// 组队跟随
		if(pCharacterData->Get_TeamFollowFlag())
			return FALSE;

		// 摆摊
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
		// 由物理层去计算
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

	// 骑乘
	if(pCharacterData->Get_MountID() != INVALID_ID)
	{
		return FALSE;
	}

	// 变身
	if(pCharacterData->Get_ModelID() != INVALID_ID)
	{
		return FALSE;
	}

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		// 组队跟随
		if(pCharacterData->Get_TeamFollowFlag())
			return FALSE;

		// 摆摊
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

	// 变身
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


	INT iMagicId	= pCmd->m_anParam[2];	// 技能ID
	INT iTargetId	= pCmd->m_anParam[3];	// 目标ID
	fVector3 fvServerPos;
	fvServerPos.x	= pCmd->m_afParam[4];	// 目标位置
	fvServerPos.z	= pCmd->m_afParam[5];	// 目标位置
	FLOAT fDir		= pCmd->m_afParam[6];	// 目标方向

	// 获取技能数据 
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( iMagicId );
	if( pClientSkill == NULL )
		return FALSE;


	// 改变动作为技能发招预备

	INT nRandAnimID = BASE_ACTION_FP;	// GetSkillDataMgr()->GetRandAnim( m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE );
	BOOL bAnimLoop = TRUE;

	// 判断基础逻辑
	if( !CharacterLogic_IsStopped(LOGIC_BASE) )
	{
		// 是否为移动
		if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
		{
			// 存在移动只改变上部动作
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop, ACTION_TOP );
		}
		else 
		{
			ChangeAction( nRandAnimID, GetLogicSpeed(), bAnimLoop );
		}
	}
	// 无基础逻辑
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
	// 记录连击招的第几次Hit
	static INT nHitCount; 

	if( CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_SEND )
	{
		CObjectCommand_MagicSend* pMagicSendCommand = (CObjectCommand_MagicSend*)GetActionLogicCommand();
		const SSkillData* pClientSkill = GetSkillDataMgr()->GetSkillData(m_paramLogic_MagicSend.m_nSaveMagicID);
		
		if( pClientSkill != NULL && pMagicSendCommand != NULL )
		{
			// 设置当前目标位置数据
			INT idTarget = pMagicSendCommand->GetTargetObjID(0);
			CObject* pTarget = (CObject*)(CObjectManager::GetMe()->FindServerObject( idTarget ));
			
			if(pTarget != NULL) 
			{
				BOOL bShowAll = FALSE;
				
				if(pClientSkill->m_nHitsOrINTerval > 0)
				{
					// 如果是连招
					++nHitCount;
					
					if(nHitCount == pClientSkill->m_nHitsOrINTerval)
					{ 
						// 如果是最后一招
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

	// 基础逻辑动作

	BOOL bBaseAnim = FALSE;
//	if( LOGIC_BASE == eLogic )
	{
		switch( CharacterLogic_Get(LOGIC_BASE) )
		{
			// 休闲
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

				// 表情动作
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
						nBaseAction = BASE_ACTION_N_SIT_IDLE;		// 坐下休息
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
							nBaseAction = BASE_ACTION_F_IDLE;		// 警戒
						}
						else
						{
							if(bPlayIdleMotion)
							{
								nBaseAction = rand() % 2 + BASE_ACTION_N_IDLE_EX0;	// 休息小动作（在两个动作中随机）
							}
							else
							{
								nBaseAction = BASE_ACTION_N_IDLE;	// 休息
							}
						}
					}

					bBaseAnim = TRUE;
					ChangeAction( nBaseAction, GetLogicSpeed(), FALSE, ePart );
				}
			}
			break;

			// 移动
		case CHARACTER_LOGIC_MOVE:
			{
				// 现在策划还没有战斗状态下的跑和走的需求

	/*			BOOL bFightState;	// 是否战斗状态
				INT nBaseAction;	// 动作

				bFightState	= IsFightState();

				int tmp = rand() % 2;	// 随机动作
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

				// 判断此次是否为在移动过程中的技能施放结束
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

				// 击飞动作已经开始了
				if( TRUE == m_paramLogic_Dead.m_bDeadFlyBegin )
				{
					m_paramLogic_Dead.m_bDeadFlyEnd = TRUE;
				}

				if( TRUE == m_paramLogic_Dead.m_bDeadBegin )
				{
					m_paramLogic_Dead.m_bDeadEnd	= TRUE;
				}

				// 死亡后的处理不放在这里
				//INT nBaseAction = rand() % 4 + BASE_ACTION_F_CADAVER0;
				//ChangeAction( m_paramLogic_Dead.m_nCadaverAction, GetLogicSpeed(), FALSE, ePart, 0.0f );
				StopCurrentAction();//停止播放死亡动作，因为最后一桢数据和尸体动作一样(todo:临时解决击飞或死亡加上尸体播放效果出错问题,这段代码需要推敲一番)
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


	// 行为逻辑动作

	BOOL bActionAnim = FALSE;
//	if( LOGIC_ACTION == eLogic )
	{
		switch( CharacterLogic_Get(LOGIC_ACTION) )
		{
		// 动作
		case CHARACTER_LOGIC_ACTION:
			break;

		case CHARACTER_LOGIC_SKILL_GATHER:
			break;

		case CHARACTER_LOGIC_SKILL_LEAD:
			break;

		//test2
		// 准备发技能
		case CHARACTER_LOGIC_SKILL_PREP_SEND:
			// 什么都不做， 不改变这个动作
			break ;

		// 发技能
		case CHARACTER_LOGIC_SKILL_SEND:
			{
				bActionAnim = TRUE;

				// 技能停止了， 但是还有移动
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
						// 如果是连招的特殊处理
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

	// 标识动作结束
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
		pNewLogicEvent->m_uRemoveTime	= 10000;	// 10秒
		
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
	{// 只删除该逻辑ID对应的伤害信息，即只删除该技能对应的伤害信息
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
	{// 只删除该逻辑ID对应的伤害信息，即只删除该技能对应的伤害信息
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
		// 只显示该技能释放者的该逻辑ID对应的伤害信息
		pLogicEvent = *itCur;
		itPrev = itCur;
		itCur++;
		if( pLogicEvent->m_nSenderID == nServerObjID && pLogicEvent->m_nSenderLogicCount <= nLogicCount )
		{
			DoLogicEvent(pLogicEvent);
			SAFE_DELETE( pLogicEvent );
			m_listLogicEvent.erase( itPrev );
			// 如果只显示一条记录则break;

			if( m_listLogicEvent.empty() && IsDie() && CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_DEAD )
			{
				Start_Dead(TRUE);
			}

			if( !bShowAll )
				break;
		}
	}

}

// 更新所有的伤害信息
VOID CObject_Character::Update_LogicEvent( VOID )
{
	// 如果死亡了就直接清掉
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

		// 增加延迟判断
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
				// 逻辑计数条件
//				if( pObject->IsBaseLogicReady  ( pLogicEvent->m_nSenderLogicCount ) ||
//					( pObject->IsActionLogicReady( pLogicEvent->m_nSenderLogicCount ) ))
//				{
//					bMustDo = TRUE;
//				}
//				// 如果超过该伤害的持续时间则强制删除
//				else if( pLogicEvent->m_uBeginTime + pLogicEvent->m_uRemoveTime < uTimeNow )
//				{
//					bMustRemove = TRUE;
//				}

				bMustDo = TRUE;

				// 如果超过该伤害的持续时间则强制删除
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

			// 当执行成功后才可删除
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
		// 判断有无延迟
		if( pLogicEvent->m_damage.m_nDelayTime > 0 )
			return FALSE;

		// 处理不同的事件
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
	//	if(0 > pDamageInfo->m_nHealthIncrement)//伤害
	//	{
	//		_sntprintf(szDamage, 32, "伤害 %d", pDamageInfo->m_nHealthIncrement);
	//		CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 6, 6);
	//	}
	//}
//	else 
	if( TRUE == IsMySelf(nCasterID) && FALSE == IsMySelf(nReceiverID) )
	{
		if( 0 > pDamageInfo->m_nHealthIncrement )//伤害
		{
			_sntprintf(szDamage, 32, " %d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 7, 7);
			AxTrace(1, 0, "%d", pDamageInfo->m_nHealthIncrement);
		}

		// 20100514 AddCodeBegin
		// 如果是本人对其他玩家的加血治疗，则显示加血数值
		if (  0 < pDamageInfo->m_nHealthIncrement ) //治疗
		{
			_sntprintf(szDamage, 32, " +%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 0, 0);
		}
		// 20100514 AddCodeEnd

	}
	//else if (TRUE == IsMyPet(nReceiverID))
	//{
	//	if(0 > pDamageInfo->m_nHealthIncrement)//伤害
	//	{
	//		_sntprintf(szDamage, 32, "伤害 %d", pDamageInfo->m_nHealthIncrement);
	//		CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 1, 1);
	//	}
	//}
	
	if( TRUE == IsMySelf(nReceiverID) )
	{
		if(0 < pDamageInfo->m_nHealthIncrement)//治疗
		{
			_sntprintf(szDamage, 32, " +%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 0, 0);
		}
		if(0 > pDamageInfo->m_nHealthIncrement)//伤害
		{
			_sntprintf(szDamage, 32, " %d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 1, 1);
			AxTrace(4, 0, "%d", pDamageInfo->m_nHealthIncrement);
		}
		if(0 < pDamageInfo->m_nRageIncrement)//怒气增加
		{
			_sntprintf(szDamage, 32, "怒气 +%d", pDamageInfo->m_nRageIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 2, 2);
		}
		if(0 > pDamageInfo->m_nRageIncrement)//怒气减少
		{
			_sntprintf(szDamage, 32, "怒气 %d", pDamageInfo->m_nRageIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 3, 3);
		}
		if(0 < pDamageInfo->m_nManaIncrement)//mana增加
		{
			_sntprintf(szDamage, 32, "威望 +%d", pDamageInfo->m_nManaIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 4, 4);
		}
		//if(0 > pDamageInfo->m_nManaIncrement)//mana减少
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
			//// 创建ItemBox
			//CTripperObject_ItemBox* pBox = (CTripperObject_ItemBox*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemBox", ObjID);
			//pBox->Initial(NULL);	
			//// 设置位置
			//pBox->SetMapPosition(fVector2(fX, fZ));
			//// 设置掉落箱的归属
			//pBox->SetOwnerGUID(idOwner);


			//ObjID_t ObjID = pDamageInfo->m_aAttachedParams[0];
			//INT idOwner = pDamageInfo->m_aAttachedParams[1];
			//FLOAT fX = pDamageInfo->m_aAttachedParams[2]/1000.f;
			//FLOAT fZ = pDamageInfo->m_aAttachedParams[3]/1000.f;
			//// 创建ItemBox
			//CTripperObject_ItemDrop* pBox = (CTripperObject_ItemDrop*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemDrop", ObjID);
			//pBox->Initial(NULL);	
			//// 设置位置
			//pBox->SetMapPosition(fVector2(fX, fZ));
			//// 设置掉落箱的归属
			//pBox->SetOwnerGUID(idOwner);
		}
		break;

	case _DAMAGE_INFO::TYPE_EFFECT:
		{
			// 表现特效效果
			if( pDamageInfo->m_nImpactID != INVALID_ID )
			{
				_DIRECT_IMPACT *pDirectlyImpact = GetDirectlyImpactMgr()->GetDirectlyImpact( pDamageInfo->m_nImpactID );
				if( pDirectlyImpact != NULL )
				{
					fVector3 fvPos;
					// 取的特效绑定点位置， 没有就直接用当前的位置
					if( GetRenderInterface() != NULL && strlen( pDirectlyImpact->m_pszEffectLocator ) > 0 )
					{
						GetRenderInterface()->Actor_GetLocator( pDirectlyImpact->m_pszEffectLocator, fvPos );
					}
					else
					{
						fvPos = GetPosition();
					}

					// 特效
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

					// 声音
					if( strlen( pDirectlyImpact->m_pszSound ) > 0 )
					{
						fVector3 fvPos = GetPosition();
						// 此接口是提供给调用的音效接口，所以传入坐标必须是gfx坐标
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
			if (IsDisplayDamageInfo()) // 信息显示的时间间隔
			{
				// 如果是伤害，则显示被击动作
				if( pDamageInfo->m_nHealthIncrement < 0 )
				{
					SetFightState(TRUE);

					DisplayHitAnimation();

					m_bBeatFly = pDamageInfo->m_bBeatFly;
					if( m_bBeatFly )
					{
						// 击飞距离
						m_fBeatFlyDist = pDamageInfo->m_fBeatFlyDist;
					}
				}

			// 显示伤血信息
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
			if (IsDisplayDamageInfo()) // 信息显示的时间间隔
			{
				// 显示未击中和免疫信息
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
	// 休闲状态才做被击动作
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
		else // 是玩家就直接播放融合动作
		{
			//暂时不播放,美术没有资源,播放会导致抽风
			//PlayAnimation(BASE_ACTION_F_SYNCRETIZE, 0.0f, 3.0f);
		}
	}
}

//例:
//
// 鞠躬,0,鞠躬,0
//  |	|___________________不循环
//	|_______________________动作名称
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

	// 变身
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
			m_listChatMoodAction.push_back(cData);	//填加到动作列表
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

		m_listChatMoodAction.push_back(cData);			//填加到动作列表
		if(sE >= sLen || cData.m_bLoop == TRUE) break;	//只有最后一个动作才能是循环动作，因为循环动作不触发finish事件

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

		if(pChatMood->m_bLoop)		// todo_yangjun 等有更好的逻辑的是欧，再单独处理站立的问题
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


VOID CObject_Character::TickBuffElapseCount()	// 每秒更新下buff的剩余时间
{
	time_t		timeNow;

	// 更新特效剩余时间
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

// 释放技能时，角色头顶冒技能名称	20100601 BLL
VOID	CObject_Character::DisplaySkillInfo( INT nSkillID )
{
	if( GetRenderInterface() == NULL || CGameProcedure::s_pUISystem == NULL )
		return;


	if( this->GetCharacterType() == CT_MONSTER && ( ((CObject_PlayerNPC*)this)->IsDisplayBoard() == FALSE) )
		return;


	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);

	DBC_DEFINEHANDLE( s_pSkillDBC, DBC_SKILL_DATA );
	const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU( nSkillID );

	// 普通攻击和进餐类等技能不显示技能名称
	if ( pSkillData->m_bAutoRedo == TRUE || pSkillData->m_nClassByUser == 3 )
	{
		return;
	}

	if ( pSkillData != NULL && pSkillData->m_lpszName != "" )
	{
		CGameProcedure::s_pUISystem->AddNewBeHitBoard( TRUE, pSkillData->m_lpszName, fvPos.x, fvPos.y, 9, 9 );
	}
}