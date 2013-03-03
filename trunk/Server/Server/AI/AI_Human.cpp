// AI_Human.cpp

#include "stdafx.h"
#include "Scene.h"
#include "GameTable.h"
#include "TimeManager.h"
#include "GCPlayerDie.h"
#include "GCPlayerRelive.h"
#include "CGPlayerDieResult.h"
#include "GCCharEquipment.h"
#include "GCNotifyChangeScene.h"
#include "GCChat.h"
//#include "GCArrive.h"
#include "Obj_Character.h"
#include "ScriptDef.h"
#include "LuaInterface.h"
#include "AbilityManager.h"
#include "Ability.h"
//#include "SkillInfo.h"
#include "Skill_Core.h"
#include "AI_Human.h"
#include "Log.h"
#include "PathFinder.h"
#include "DiePenaltyManager.h"
#include "HumanItemLogic.h"
#include "Obj_Pet.h"
#include "CGAskDetailEquipList.h"
#include "GCDetailEquipList.h"
#include "GCDiscardEquipResult.h"
#include "GCDiscardItemResult.h"
#include "GCPlayerCallOf.h"
#include "CGPlayerCallOfResult.h"
#include "GamePlayer.h"
#include "AI_Monster.h"
#include "AI_Pet.h"
#include "Obj_Monster.h"
#include "AvoidOverlap.h"
#include "Obj.h"
#include "ActionDelegator.h"
#include "GCOperateResult.h"
#include "Obj_Character.h"

#include "GCStallClose.h"
#include "GWRelation.h"

#undef ___TEAMFOLLOW_LEADERDRIVEN___
#define ___TEAMFOLLOW_MEMBERFOLLOW___

using namespace Combat_Module::Skill_Module;
using namespace Packets;
using namespace Action_Module;


AI_Human::AI_Human( VOID )
{
//	m_eHumanAI		= HUMAN_AI_INVALID;
	m_TeamFollowCheckTick = 0;
}

AI_Human::~AI_Human( VOID )
{
}

BOOL AI_Human::Init( Obj_Character *pCharacter )
{
__ENTER_FUNCTION

	BOOL bResult = AI_Character::Init( pCharacter );
	if ( !bResult )
		return FALSE;

	//m_eHumanAI		= HUMAN_AI_INVALID;
	m_TeamFollowCheckTick = 0;
	m_paramAI_UseSkill.CleanUp();
	m_paramAI_UseItem.CleanUp();
	return TRUE;
__LEAVE_FUNCTION
	return FALSE ;
}

VOID AI_Human::Term( VOID )
{
__ENTER_FUNCTION

//	m_eHumanAI		= HUMAN_AI_INVALID;
	AI_Character::Term();

__LEAVE_FUNCTION
}

VOID AI_Human::Event_OnBeSkill( Obj_Character* pCharacter, INT nGoodEffect)
{
__ENTER_FUNCTION

	if (!pCharacter)
	{
		return ;
	}
	if(BEHAVIOR_TYPE_AMITY == nGoodEffect)
	{// ������Ѻõļ�������Ϊ�Ƕ��ѣ��������ó��Լ��ĸ�����
		Obj_Human* pHuman = (Obj_Human*)GetCharacter();
		if(NULL!=pHuman)
		{
			pHuman->SetAssistantID( pCharacter->GetID() ) ;
		}
	}
__LEAVE_FUNCTION
}

VOID AI_Human::Event_OnDie( Obj *pKiller/* = NULL*/ )
{
__ENTER_FUNCTION
	ObjID_t idKiller = INVALID_ID;
	if ( pKiller != NULL )
	{
		idKiller = pKiller->GetID();
	}

	ChangeState(ESTATE_DEAD);

	After_Die(idKiller);

__LEAVE_FUNCTION
}

VOID AI_Human::OnReliveInfoChanged( VOID )
{
__ENTER_FUNCTION

	if ( ESTATE_DEAD == GetAIState()->GetStateID() )
	{
		BOOL bCanRelive = GetCharacter()->IsCanRelive();

		GCPlayerDie msgPlayerDie;
		msgPlayerDie.SetCanRelive( bCanRelive );
		msgPlayerDie.SetTime( g_Config.m_ConfigInfo.m_OutGhostTime + (g_pTimeManager->CurrentTime() - m_paramAI_Dead.m_AutoTimer.GetTickOldTime()) );

		((Obj_Human*)GetCharacter())->GetPlayer()->SendPacket(&msgPlayerDie);
	}

__LEAVE_FUNCTION
}

VOID AI_Human::OnCallOfInfoChanged( VOID )
{
__ENTER_FUNCTION

	Obj_Human *pHuman = ((Obj_Human*)GetCharacter());
	if(pHuman != NULL)
	{
		const _CALLOF_INFO *pCallOfInfo = pHuman->GetCallOfInfo();
		if(pCallOfInfo != NULL)
		{
			GCPlayerCallOf msgPlayerCallOf;
			msgPlayerCallOf.SetCallerGUID(pHuman->GetGUID());
			msgPlayerCallOf.SetTime(pCallOfInfo->m_uDuration);

			pHuman->GetPlayer()->SendPacket(&msgPlayerCallOf);
		}
	}

__LEAVE_FUNCTION
}

ORESULT AI_Human::AcceptCallOf(VOID)
{
__ENTER_FUNCTION

	Obj_Human *pHuman = ((Obj_Human*)GetCharacter());
	if(pHuman != NULL)
	{
		const _CALLOF_INFO *pCallOfInfo = pHuman->GetCallOfInfo();
		if(pCallOfInfo != NULL)
		{
			if(pHuman->getScene()->SceneID() == pCallOfInfo->m_SceneID)
			{
				Stop();
				pHuman->Teleport(&pCallOfInfo->m_Pos);
			}
			else
			{
				pHuman->ChangeScene(pHuman->getScene()->SceneID(), pCallOfInfo->m_SceneID, pCallOfInfo->m_Pos, 0);
			}
			pHuman->ResetCallOfInfo();
		}
	}

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::RefuseCallOf(VOID)
{
__ENTER_FUNCTION

	Obj_Human *pHuman = ((Obj_Human*)GetCharacter());
	if(pHuman != NULL)
	{
		const _CALLOF_INFO *pCallOfInfo = pHuman->GetCallOfInfo();
		if(pCallOfInfo != NULL)
		{
			//GWCallOfHumanResult	msgCallOfHumanResult;
			//msgCallOfHumanResult.SetCallerGUID(pCallOfInfo->m_guidCaller);
			//msgCallOfHumanResult.SetTargetGUID(pHuman->GetGUID());
			//msgCallOfHumanResult.SetResultCode(GWCallOfHumanResult);
			//pHuman->GetPlayer()->SendPacket(&msgCallOfHumanResult);
		}
	}

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_DieResult( INT nResultCode )
{
__ENTER_FUNCTION
	Obj_Human *pHuman = (Obj_Human*)(GetCharacter());
	Assert( pHuman != NULL );
	if ( pHuman == NULL )
		return OR_ERROR;

	if ( !(pHuman->IsActiveObj()) )
		return OR_ERROR;

	if ( nResultCode >= DIE_RESULT_CODE_NUMBERS || nResultCode <= DIE_RESULT_CODE_INVALID )
		Assert(FALSE&&"��ɫ���ʽ����");

	Relive( nResultCode );
	
	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_CallOfResult( INT nResultCode )
{
__ENTER_FUNCTION

	Obj_Human *pHuman = (Obj_Human*)(GetCharacter());
	Assert( pHuman != NULL );
	if ( pHuman == NULL )
		return OR_ERROR;

	if ( !(pHuman->IsActiveObj()) )
		return OR_ERROR;

	ORESULT oResult = OR_ERROR;
	switch ( nResultCode )
	{
	case CALLOF_RESULT_CODE_ACCEPT:
		{
			oResult = AcceptCallOf();
		}
		break;
	case CALLOF_RESULT_CODE_REFUSE:
		{
			oResult = RefuseCallOf();
		}
		break;
	default:
		break;
	}

	return oResult;

__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_Jump( VOID )
{
__ENTER_FUNCTION
	
	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL && pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN);
	if ( pCharacter == NULL )
		return OR_ERROR;

	if ( !(pCharacter->IsActiveObj()) )
		return OR_ERROR;

	Jump();

	// ���skillQueue,autoQueue�е���ֵ
	m_paramAI_UseSkill.CleanUp();
	//Interrupt current action
	GetGlobalActionDelegator().InterruptCurrentAction(*pCharacter);
	//Interrupt current abilityoperation
	((Obj_Human*)pCharacter)->InterruptCurrentAbilityOpera();


	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_MoodState(INT nMoodState)
{
__ENTER_FUNCTION
	
	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	if ( !(pCharacter->IsActiveObj()) )
		return OR_ERROR;

	pCharacter->SetMoodState(nMoodState);

	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Human::Validate_Event( Obj* pTarget )
{
__ENTER_FUNCTION

	if( pTarget == NULL )
		return OR_NO_TARGET;

	Obj_Character* pCharacter = GetCharacter();
	if (!pCharacter)
	{
		Assert(NULL && "AI_Human::Validate_Event...pCharacter = NULL!!!");
		return OR_ERROR;
	}

	if( IsCharacterObj( pTarget->GetObjType() ) )
	{
		Obj_Character *pChar = (Obj_Character*)pTarget;
		if ( pChar->IsDie() )
			return OR_TARGET_DIE;

		if(FALSE == pChar->IsFriendCamp(*pCharacter))
		{
			return OR_ERROR;
		}
 	}

	BOOL bRadius;
	FLOAT fErrorDist = 2.f;
	bRadius = GetCharacter()->IsInValidRadius(pTarget, MAX_SERVER_DEFAULT_EVENT_DIST + fErrorDist);
	if( bRadius == FALSE )
	{
		Log::SaveLog( SERVER_LOGFILE, "not in range of %d",pTarget->GetID()) ;
		return OR_OUT_RANGE;
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

VOID AI_Human::Relive( INT nResultCode )
{
__ENTER_FUNCTION

	Obj_Human *pThisHuman = ((Obj_Human*)GetCharacter());

	GCPlayerRelive msgPlayerRelive;
	pThisHuman->GetPlayer()->SendPacket(&msgPlayerRelive);

	Scene *pScene = pThisHuman->getScene();
	if(pScene != NULL)
	{
		pScene->OnSceneHumanRelive(pThisHuman,nResultCode);
	}

	if ( nResultCode != CITY_POS_RELIVE )
	{
		pThisHuman->Teleport( pThisHuman->getWorldPos() );
	}

	//const Obj_Character::_RELIVE_INFO *pReliveInfo = pThisHuman->GetReliveInfo( bSkillRelive );
	//if ( pReliveInfo != NULL )
	//{
	//	INT nHP;
	//	if ( pReliveInfo->m_bHPUsePercent )
	//	{
	//		nHP = (pReliveInfo->m_nHPPercent * pThisHuman->GetMaxHP() ) / 100;
	//		pThisHuman->SetHP( nHP );
	//	}
	//	else
	//	{
	//		if ( pReliveInfo->m_nHP != -1 )
	//		{
	//			nHP = pReliveInfo->m_nHP;
	//			pThisHuman->SetHP( nHP );
	//		}
	//	}
	//	
	//	INT nMP;
	//	if ( pReliveInfo->m_bMPUsePercent )
	//	{
	//		nMP = (pReliveInfo->m_nMPPercent * pThisHuman->GetMaxMP() ) / 100;
	//		pThisHuman->SetMP( nMP );
	//	}
	//	else
	//	{
	//		if ( pReliveInfo->m_nMP != -1 )
	//		{
	//			nMP = pReliveInfo->m_nMP;
	//			pThisHuman->SetMP( nMP );
	//		}
	//	}

	//	INT nSP;
	//	if ( pReliveInfo->m_bSPUsePercent )
	//	{
	//		nSP = (pReliveInfo->m_nSPPercent * pThisHuman->GetMaxRage() ) / 100;
	//		pThisHuman->SetRage( nSP );
	//	}
	//	else
	//	{
	//		if ( pReliveInfo->m_nSP != -1 )
	//		{
	//			nSP = pReliveInfo->m_nSP;
	//			pThisHuman->SetRage( nSP );
	//		}
	//	}
	//	
	//	
	//	SceneID_t idCurScene;
	//	if ( pThisHuman->getScene() != NULL )
	//	{
	//		idCurScene = pThisHuman->getScene()->SceneID();
	//	}
	//	else
	//	{
	//		idCurScene = INVALID_ID;
	//	}

	//	if ( pReliveInfo->m_SceneID != idCurScene
	//		&& pReliveInfo->m_SceneID != INVALID_ID )
	//	{
	//		WORLD_POS pos = pReliveInfo->m_Pos ;
	//		pThisHuman->ChangeScene( idCurScene, pReliveInfo->m_SceneID, pos, 9 ) ;

	//	}
	//	else
	//	{
	//		pThisHuman->Teleport( &(pReliveInfo->m_Pos) );
	//	}
	//}

	// ��֤һ���Ḵ��
	//if(pThisHuman->GetHP() <= 0)
	//{
	//	pThisHuman->SetHP(1);
	//}

	//pThisHuman->ResetReliveInfo();

	ChangeState(ESTATE_IDLE);

__LEAVE_FUNCTION
}

ORESULT AI_Human::PushCommand_Idle( VOID )
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ���skillQueue,autoQueue�е���ֵ
	m_paramAI_UseSkill.CleanUp();
	//Interrupt current action
	GetGlobalActionDelegator().InterruptCurrentAction(*GetCharacter());
	//Interrupt current abilityoperation
	((Obj_Human*)GetCharacter())->InterruptCurrentAbilityOpera();

	ORESULT oResult = Stop();
	return oResult;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_Stall( VOID )
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	ORESULT oResult = Stall();
	return oResult;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_Move( INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos )
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	ORESULT oResult = Move( nHandleID, wNumTargetPos, paTargetPos );
	if(OR_SUCCEEDED(oResult))
	{
		// ֪ͨ�Լ��ĳ���Ҳ��ʼ�ƶ�
		Baby_Go(paTargetPos);
		// ���skillQueue,autoQueue�е���ֵ
		m_paramAI_UseSkill.CleanUp();
		// AvoidOverLap����λ
		((Obj_Human*)GetCharacter())->GetAvoidOverLap()->ResetUsedDir();
		// ������Ӹ����߼�
		TeamMemeberToMove();
	}
	return oResult;

__LEAVE_FUNCTION

	return OR_ERROR;
}

VOID AI_Human::Baby_Go(const WORLD_POS *paTargetPos)
{
__ENTER_FUNCTION
	Obj_Human* pHuman = (Obj_Human*)GetCharacter();	
	if (!pHuman)
	{
		return ;
	}

	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
	{
		Obj_Pet* pPet = pHuman->GetPet(i);
		if (pPet)
		{
			AI_Pet* pAIPet = (AI_Pet*)(pPet->GetAIObj() );
			if (pAIPet)
			{
				pAIPet->Baby_Go(pHuman->getWorldPos());
			}
		}
	}

	
__LEAVE_FUNCTION
}

ORESULT AI_Human::PushCommand_UseSkill( ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget, INT iHurtDelayTime )
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ֻ��ʹ�ü��ܳɹ�������²��ܽ���combat״̬
	ORESULT oResult = UseSkill(idSkill, idTarget, fTargetX, fTargetZ, iHurtDelayTime, fDir, guidTarget);
	m_paramAI_UseSkill.m_fQueueTargetDirection	= fDir;
	m_paramAI_UseSkill.m_iHurtDelayTime			= iHurtDelayTime;
	if (OR_OK == oResult) {
		ChangeState(ESTATE_COMBAT);	
		// ���ʹ���Զ������ͷŵļ��������ս��״̬�� ����ֻ��Ϊ�ﵽ����AIս��״̬��ͳһ����
		if (FALSE == IsEnterCombatState(idSkill, idTarget)) 
		{
			//������ܺϷ���Ч�����Ҳ����Զ������ͷż�����ѹ�뼼��ʹ�ö���
			PushSkillToQueue(idSkill, idTarget, fTargetX, fTargetZ, iHurtDelayTime, fDir, guidTarget);
		}
	}
	return oResult;
__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_UseItem(ID_t nBagIndex, ObjID_t nTargetObj, const WORLD_POS& posTarget, PET_GUID_t& guidTargetPet, ID_t nTargetItem)
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ֻ��ʹ�õ��߳ɹ�������²��ܽ���combat״̬
	ORESULT oResult = UseItem(nBagIndex, nTargetObj, posTarget, guidTargetPet, nTargetItem);
	if (OR_OK == oResult) {
        ChangeState(ESTATE_COMBAT);
		PushItemToQueue(nBagIndex, nTargetObj, posTarget, guidTargetPet, nTargetItem);
	}
	return oResult;
__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_UseAbility( VOID )
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ʹ��Humanͣ����
	Stop();

	ORESULT oResult = UseAbility( );
	if (OR_OK != oResult) {
		Obj_Human* pHuman = static_cast<Obj_Human*>(GetCharacter());
		if(NULL!=pHuman)
		{
			pHuman->SendOperateResultMsg(oResult);
		}
	}
	return oResult;

__LEAVE_FUNCTION

	return OR_ERROR;
}
//
//ORESULT AI_Human::PushCommand_Dead( ObjID_t idKiller )
//{
//__ENTER_FUNCTION
//
//	if ( !(GetCharacter()->IsActiveObj()) )
//		return OR_ERROR;
//
//	// ʹ��Humanͣ����
//	Stop();
//
//	ChangeState(ESTATE_DEAD);
//
//	return OR_OK;
//
//__LEAVE_FUNCTION
//
//	return OR_ERROR;
//}

ORESULT AI_Human::PushCommand_DefaultEvent( ObjID_t idNPC )
{
__ENTER_FUNCTION

	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	if ( ESTATE_TEAMFOLLOW == GetAIState()->GetStateID() )
	{
		return OR_ERROR;
	}

	Obj* pNPC = pCharacter->getScene()->GetObjManager()->GetObj(idNPC);
	if (!pNPC)
		return OR_ERROR;

	if( pNPC->GetObjType() == Obj::OBJ_TYPE_HUMAN)
		return OR_OK;		

	// ʹ��Humanͣ����
	if(pCharacter->GetCharacterLogic() == CHARACTER_LOGIC_MOVE)
	{
		Stop();
	}
	if ( pNPC->GetObjType() == Obj::OBJ_TYPE_MONSTER )
	{
		AI_Monster* pAIMonster = (AI_Monster*)(((Obj_Monster*)pNPC)->GetAIObj());
		if (!pAIMonster)
		{
			Assert(NULL && "AI_Human::PushCommand_DefaultEvent...pAIMonster=NULL...");
			return OR_ERROR;
		}
		if (TRUE == pAIMonster->IsConvoyNPC())
		{// ����ǻ���NPC��ʲô��������
			return OR_OK;
		}
		else
		{
			pAIMonster->StartService();
			pAIMonster->Stop();
		}
	}

	ORESULT oRes = Validate_Event( pNPC );
	if( oRes != OR_OK )
	{
		return oRes;
	}

	ScriptID_t idScript = pNPC->GetScriptID();
	if ( idScript == (ScriptID_t)(0xFFFFFFFF) )
	{
		if(pNPC->GetObjType() == Obj::OBJ_TYPE_MONSTER)
		{
			Obj_Monster* pMonster = (Obj_Monster*) pNPC;
			NPC_QUEST* pQuest = g_NpcQuestTbl.Get(pMonster->GetDataID());
			if(pQuest!=NULL)
			{
				if(pQuest->m_EventCount==0)
				{
					pCharacter->getScene()->OnNpcDefaultDialog((Obj_Human*)pCharacter,pNPC->GetID());
				
				}
				else
				{
					//for(int i = 0;i<pQuest->m_EventCount;i++)
					//{
						pCharacter->getScene()->OnNpcEventList((Obj_Human*)pCharacter,pNPC->GetID());
					//}
				}
			}
		}
		return OR_OK;
	}
	else
		pCharacter->getScene()->GetLuaInterface()->ExeScript_DDD( 
		idScript, 
		DEF_EVENT_ENTRY_FUNC_NAME, 
		(INT)pCharacter->getScene()->SceneID(), 
		(INT)pCharacter->GetID(), 
		(INT)pNPC->GetID() ) ;

	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_EventRequest( ObjID_t idNPC, ScriptID_t idEvent ,INT stateIndex,INT exIndex)
{
__ENTER_FUNCTION

	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ʹ��Humanͣ����
	Stop();

	Obj* pNPC = pCharacter->getScene()->GetObjManager()->GetObj(idNPC);

	ORESULT oRes = Validate_Event( pNPC );
	if( oRes != OR_OK )
	{
		return oRes;
	}
	///���Ͻű�

	/*
	ScriptID_t idScript = pNPC->GetScriptID();
	if ( idScript == (ScriptID_t)(0xFFFFFFFF) )
		return OR_NO_SCRIPT;
*/
	BOOL bVerifyEvent = FALSE;
	if(pNPC->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		Obj_Monster* pMonster = (Obj_Monster*) pNPC;
		NPC_QUEST* pQuest = g_NpcQuestTbl.Get(pMonster->GetDataID());
		if(pQuest)
		{
			for(int i = 0;i<pQuest->m_EventCount;i++)
			{
				if(pQuest->m_EventList[i] == idEvent)
				{
					bVerifyEvent = TRUE;
					break;
				}
			}
		}
	}
	if(bVerifyEvent)
	{
		pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDDD( 
		idEvent, 
		"OnDefaultEvent", 
		pCharacter->getScene()->SceneID(),
		pCharacter->GetID(),
		pNPC->GetID(),
		stateIndex,
		exIndex);
		
		return OR_OK;
	}
	else
		return OR_ERROR;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_MissionAccept( ObjID_t idNPC, ScriptID_t idMissionScript )
{
__ENTER_FUNCTION

	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ʹ��Humanͣ����
	Stop();

	//У���Human������������
	Assert(pCharacter->GetObjType()==Obj::OBJ_TYPE_HUMAN) ;

	// ���������ϵ���������Ƿ��Ѵ�����
	if( !pCharacter->getScene()->OnAcceptMissionCheck((Obj_Human*)pCharacter,idMissionScript) )
	{
		return OR_OK ;
	}

	if( idNPC!=INVALID_ID )
	{
		Obj* pNPC = pCharacter->getScene()->GetObjManager()->GetObj(idNPC);

		ORESULT oRes = Validate_Event( pNPC );
		if( oRes != OR_OK )
		{
			return oRes;
		}

		/*ScriptID_t idScript = pNPC->GetScriptID();
		if ( idScript == (ScriptID_t)(0xFFFFFFFF) )
			return OR_NO_SCRIPT;

		pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDD( 
			idScript, 
			"OnMissionAccept",
			(INT)pCharacter->getScene()->SceneID(),
			(INT)pCharacter->GetID(), 
			(INT)pNPC->GetID(), 
			(INT)idMissionScript );
			*/


		BOOL bVerifyEvent = FALSE;
		if(pNPC->GetObjType() == Obj::OBJ_TYPE_MONSTER)
		{
			Obj_Monster* pMonster = (Obj_Monster*) pNPC;
			NPC_QUEST* pQuest = g_NpcQuestTbl.Get(pMonster->GetDataID());
			if(pQuest)
			{
				for(int i = 0;i<pQuest->m_EventCount;i++)
				{
					if(pQuest->m_EventList[i] == idMissionScript)
					{
						bVerifyEvent = TRUE;
						break;
					}
				}
			}
		}
		if(bVerifyEvent)
		{
			int ret = pCharacter->getScene()->GetLuaInterface()->ExeScript_DDD( 
									idMissionScript, 
									"CheckAccept", 
									pCharacter->getScene()->SceneID(),
									pCharacter->GetID(),
									(INT)pNPC->GetID());

			if(ret>0)
			{
				pCharacter->getScene()->GetLuaInterface()->ExeScript_DD( 
					idMissionScript, 
					"OnAccept", 
					pCharacter->getScene()->SceneID(),
					pCharacter->GetID());

			}

			return OR_OK;
		}
		else
			return OR_ERROR;
	}
	else
	{
		/*pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDD( 
			idMissionScript, 
			"OnMissionAccept",
			(INT)pCharacter->getScene()->SceneID(),
			(INT)pCharacter->GetID(), 
			(INT)INVALID_ID, 
			(INT)idMissionScript );
			*/
		int ret = pCharacter->getScene()->GetLuaInterface()->ExeScript_DD( 
			idMissionScript, 
			"CheckAccept", 
			pCharacter->getScene()->SceneID(),
			pCharacter->GetID());

		if(ret>0)
		{
			pCharacter->getScene()->GetLuaInterface()->ExeScript_DD( 
				idMissionScript, 
				"OnAccept", 
				pCharacter->getScene()->SceneID(),
				pCharacter->GetID());

		}

		return OR_OK;
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_MissionAbandon( ScriptID_t idMissionScript )
{
__ENTER_FUNCTION

	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	pCharacter->getScene()->GetLuaInterface()->ExeScript_DD( 
		idMissionScript, 
		"OnAbandon",
		(INT)pCharacter->getScene()->SceneID(),
		(INT)pCharacter->GetID() );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_MissionRefuse( ObjID_t idNPC, ScriptID_t idMissionScript )
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ʹ��Humanͣ����
	Stop();

	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	Obj* pNPC = pCharacter->getScene()->GetObjManager()->GetObj(idNPC);

	ORESULT oRes = Validate_Event( pNPC );
	if( oRes != OR_OK )
	{
		return oRes;
	}

	
	if(pNPC->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		Obj_Monster* pMonster = (Obj_Monster*) pNPC;
		NPC_QUEST* pQuest = g_NpcQuestTbl.Get(pMonster->GetDataID());
		if(pQuest!=NULL)
		{
			for(int i = 0;i<pQuest->m_EventCount;i++)
			{
				pCharacter->getScene()->GetLuaInterface()->ExeScript_DDD(pQuest->m_EventList[i],
										"OnEnumerate",
										pCharacter->getScene()->SceneID(),
										pCharacter->GetID(),
										pNPC->GetID());
			}
		}
	}
	

	/*
	ScriptID_t idScript = pNPC->GetScriptID();
	if ( idScript == (ScriptID_t)(0xFFFFFFFF) )
		return OR_NO_SCRIPT;

	pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDD( 
		idScript, 
		"OnMissionRefuse",
		(INT)pCharacter->getScene()->SceneID(),
		(INT)pCharacter->GetID(), 
		(INT)pNPC->GetID(), 
		(INT)idMissionScript ) ;
	*/



	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_MissionSubmit( ObjID_t idNPC, ScriptID_t idMissionScript, UINT uSelectRadioID )
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ʹ��Humanͣ����
	Stop();

	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	Obj* pNPC = pCharacter->getScene()->GetObjManager()->GetObj(idNPC);

	ORESULT oRes = Validate_Event( pNPC );
	if( oRes != OR_OK )
	{
		return oRes;
	}
	/*
	ScriptID_t idScript = pNPC->GetScriptID();
	if ( idScript == (ScriptID_t)(0xFFFFFFFF) )
		return OR_NO_SCRIPT;

	pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDDD( 
		idScript, 
		"OnMissionSubmit",
		(INT)pCharacter->getScene()->SceneID(),
		(INT)pCharacter->GetID(), 
		(INT)pNPC->GetID(), 
		(INT)idMissionScript,
		(INT)uSelectRadioID );
		*/

	BOOL bVerifyEvent = FALSE;
	if(pNPC->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		Obj_Monster* pMonster = (Obj_Monster*) pNPC;
		NPC_QUEST* pQuest = g_NpcQuestTbl.Get(pMonster->GetDataID());
		if(pQuest)
		{
			for(int i = 0;i<pQuest->m_EventCount;i++)
			{
				if(pQuest->m_EventList[i] == idMissionScript)
				{
					bVerifyEvent = TRUE;
					break;
				}
			}
		}
	}
	if(bVerifyEvent)
	{
		pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDD( 
			idMissionScript, 
			"OnSubmit", 
			pCharacter->getScene()->SceneID(),
			pCharacter->GetID(),
			pNPC->GetID(),
			uSelectRadioID);

	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_MissionCheck( ObjID_t idNPC, ScriptID_t idMissionScript, UINT index1, UINT index2, UINT index3, UINT indexPet )
{
	__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ʹ��Humanͣ����
	Stop();

	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	Obj* pNPC = pCharacter->getScene()->GetObjManager()->GetObj(idNPC);

	ORESULT oRes = Validate_Event( pNPC );
	if( oRes != OR_OK )
	{
		return oRes;
	}
	
	ScriptID_t idScript = pNPC->GetScriptID();
	if ( idScript == (ScriptID_t)(0xFFFFFFFF) )
		return OR_NO_SCRIPT;

	pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDDDDDD( 
		idScript, 
		"OnMissionCheck",
		(INT)pCharacter->getScene()->SceneID(),
		(INT)pCharacter->GetID(), 
		(INT)pNPC->GetID(), 
		(INT)idMissionScript, 
		(INT)index1, 
		(INT)index2, 
		(INT)index3, 
		(INT)indexPet ) ;

	return OR_OK;

	__LEAVE_FUNCTION

		return OR_ERROR;
}


ORESULT AI_Human::PushCommand_MissionContinue( ObjID_t idNPC, ScriptID_t idMissionScript )
{
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	// ʹ��Humanͣ����
	Stop();

	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return OR_ERROR;

	if( idNPC!=INVALID_ID )
	{
		Obj* pNPC = pCharacter->getScene()->GetObjManager()->GetObj(idNPC);

		ORESULT oRes = Validate_Event( pNPC );
		if( oRes != OR_OK )
		{
			return oRes;
		}

		/*ScriptID_t idScript = pNPC->GetScriptID();
		if ( idScript == (ScriptID_t)(0xFFFFFFFF) )
			return OR_NO_SCRIPT;

		pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDD( 
			idScript, 
			"OnMissionContinue",
			(INT)pCharacter->getScene()->SceneID(),
			(INT)pCharacter->GetID(), 
			(INT)pNPC->GetID(), 
			(INT)idMissionScript );
			*/
		BOOL bVerifyEvent = FALSE;
		if(pNPC->GetObjType() == Obj::OBJ_TYPE_MONSTER)
		{
			Obj_Monster* pMonster = (Obj_Monster*) pNPC;
			NPC_QUEST* pQuest = g_NpcQuestTbl.Get(pMonster->GetDataID());
			if(pQuest)
			{
				for(int i = 0;i<pQuest->m_EventCount;i++)
				{
					if(pQuest->m_EventList[i] == idMissionScript)
					{
						bVerifyEvent = TRUE;
						break;
					}
				}
			}
		}
		if(bVerifyEvent)
		{
			pCharacter->getScene()->GetLuaInterface()->ExeScript_DDD( 
				idMissionScript, 
				"OnContinue", 
				pCharacter->getScene()->SceneID(),
				pCharacter->GetID(),
				pNPC->GetID());

		}

	}
	else
	{
		/*pCharacter->getScene()->GetLuaInterface()->ExeScript_DDDD( 
			idMissionScript, 
			"OnMissionContinue",
			(INT)pCharacter->getScene()->SceneID(),
			(INT)pCharacter->GetID(), 
			(INT)INVALID_ID, 
			(INT)idMissionScript );*/
		pCharacter->getScene()->GetLuaInterface()->ExeScript_DD( 
			idMissionScript, 
			"OnContinue", 
			pCharacter->getScene()->SceneID(),
			pCharacter->GetID());
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_TeamFollow( )
{ // ����Ӧ�ü��϶Խ�����������������ж�
__ENTER_FUNCTION

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	return StartTeamFollow();

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::PushCommand_StopTeamFollow( )
{
__ENTER_FUNCTION

	Obj_Human* pHuman = (Obj_Human*)GetCharacter();
	if ( pHuman==NULL )
	{
		Assert( FALSE && pHuman == NULL );
		return OR_ERROR;
	}

	if ( !(GetCharacter()->IsActiveObj()) )
		return OR_ERROR;

	if ( pHuman->__GetTeamFollowFlag() )
	{
		return StopTeamFollow();
	}
	else
	{
		return OR_OK;
	}

__LEAVE_FUNCTION

	return OR_ERROR;
}

//#ifdef ___TEAMFOLLOW_LEADERDRIVEN___

BOOL AI_Human::GetFollowPos( WORLD_POS& Pos, const WORLD_POS *pStartPos, const WORLD_POS *pEndPos, FLOAT fDist )
{
__ENTER_FUNCTION

	Assert( pStartPos );
	Assert( pEndPos );
	Assert( fDist );

	Pos.m_fX = (pStartPos->m_fX - pEndPos->m_fX) / fDist + pEndPos->m_fX;
	Pos.m_fZ = (pStartPos->m_fZ - pEndPos->m_fZ) / fDist + pEndPos->m_fZ;

__LEAVE_FUNCTION

	return TRUE;
}

//#endif
// pHuman ���� fDist ��Զ�� ����ѷ�ʽ �ƶ��� pPos λ��
VOID AI_Human::FollowMove( Obj_Human* pHuman, WORLD_POS* pPos, FLOAT fDist )
{
__ENTER_FUNCTION

	if ( fDist>g_Config.m_ConfigInfo.m_nAvailableFollowDist )
	{ // ����ҳ��� pPos λ�ã����ҷ�����Ϣ֪ͨ�ͻ���
		pHuman->__OutOfTeamFollowRange();
	}
	else
	{
		pHuman->__InTeamFollowRange();

		if ( fDist>3.5 )
		{
			if ( pHuman->__GetTeamFollowSpeedUp()==FALSE )
			{
				pHuman->__SetTeamFollowSpeed( (FLOAT)(pHuman->__GetTeamFollowSpeed()*1.5) );
				pHuman->__SetTeamFollowSpeedUp( TRUE );
			}
		}
		else if ( fDist<1.0 )// �������뷶Χ��
		{
			if ( pHuman->__GetTeamFollowSpeedUp()==TRUE )
			{
				pHuman->__SetTeamFollowSpeed( (FLOAT)(pHuman->__GetTeamFollowSpeed()*2.0/3.0) );
				pHuman->__SetTeamFollowSpeedUp( FALSE );
			}
		}
		// else { 1.0 ~ 3.5 ��֮�䲻�����ٶȵ��� }
	}

	if( Obj_Move(pPos) == OR_OK )
	{
		Baby_Go(pPos);
	}

__LEAVE_FUNCTION
}

ORESULT AI_Human::StartTeamFollow( VOID )
{ // �����Լ����ٶ�Ϊ�ӳ����ٶ�
__ENTER_FUNCTION

	Obj_Human* pHuman = (Obj_Human*)GetCharacter();
	if ( pHuman==NULL )
	{
		Assert( FALSE && pHuman == NULL );
		return OR_ERROR;
	}

	Obj_Human* pLeader = pHuman->__GetFollowedMember(0)->m_pHuman;
	if ( pLeader==NULL )
	{
		Assert(FALSE && pLeader == NULL );
		return OR_ERROR;
	}

	pHuman->__SetTeamFollowSpeed( pLeader->GetMoveSpeed() );

	if ( ESTATE_TEAMFOLLOW != GetAIState()->GetStateID() )
	{
		ChangeState(ESTATE_TEAMFOLLOW);
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AI_Human::StopTeamFollow( VOID )
{
__ENTER_FUNCTION
	Stop();
	ChangeState(ESTATE_IDLE);
	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

VOID AI_Human::AI_Logic_Combat( UINT uTime )
{
	__ENTER_FUNCTION
	//��ý�ɫ����
	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return ;
	Obj_Character& rMe= *pCharacter;
	WORLD_POS position;
	position.m_fX=position.m_fZ=0.0f;
	//�����������ʹ�ü���
		
	SkillID_t nCurrentSkill = INVALID_ID;
	SkillID_t nAutoActivedSkill = m_paramAI_UseSkill.m_nAutoShotSkill;
	SkillID_t nQueuedSkill = m_paramAI_UseSkill.m_nQueuedSkill;
	ID_t BagIndex = m_paramAI_UseItem.m_BagIndex;

	if (INVALID_ID != BagIndex)
	{
		// ���ж���û����ɡ�
		if (FALSE == rMe.CanUseSkillNow(INVALID_ID))
		{
			return;
		}

		Obj_Human* pHuman = (Obj_Human*)GetCharacter();
		Item*	pBagItem = HumanItemLogic::GetBagItem(pHuman,BagIndex);
		//���Ŀǰ��ɫ����û���κ���Ʒ�򷵻�
		if(pBagItem->IsEmpty())
		{
			m_paramAI_UseItem.CleanUp();
			return;
		}
		ScriptID_t nItemScript = pBagItem->GetScriptID();
		SkillID_t nItemSkill = pBagItem->GetSkillID();
		if(INVALID_ID==nItemSkill)
		{
			AssertEx(FALSE,"[AI_Human::AI_Logic_Combat]: Data Error. Can't Skill id from Item Date.");
			m_paramAI_UseItem.CleanUp();
			return;
		}
		//�����ǰ���ܺ���һ����������ͬһ����ȴ�飬������ȴʱ��δ�����򷵻�
		if(FALSE==rMe.Skill_IsSkillCooldowned(nItemSkill))
		{
			Obj_Human* pHuman = static_cast<Obj_Human*>(GetCharacter());
			if(NULL!=pHuman)
			{
				pHuman->SendOperateResultMsg(OR_COOL_DOWNING);
			}
			m_paramAI_UseItem.CleanUp();
			return ; //��δ��ȴ
		}
		// ��ǰ״̬�Ƿ�����ʹ�ü��ܣ���������򷵻�
		if(FALSE==pCharacter->Skill_CanUseThisSkillInThisStatus(nItemSkill))
		{
			Obj_Human* pHuman = static_cast<Obj_Human*>(GetCharacter());
			if(NULL!=pHuman)
			{
				pHuman->SendOperateResultMsg(OR_U_CANNT_DO_THIS_RIGHT_NOW);
			}
			m_paramAI_UseItem.CleanUp();
			return ;
		}
		//���û�ж�Ӧ�Ľű�
		if(INVALID_ID == nItemScript)
		{
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rParams.SetActivatedScript(nItemScript);
			rParams.SetActivatedSkill(nItemSkill);
			rParams.SetBagIndexOfDepletedItem(BagIndex);
			rParams.SetItemIndexOfDepletedItem(pBagItem->GetItemTableIndex());
			rParams.SetDepletedItemGuid(pBagItem->GetGUID());
			Obj_UseSkill(
				nItemSkill,
				m_paramAI_UseItem.m_Target,
				m_paramAI_UseItem.m_posTarget.m_fX,
				m_paramAI_UseItem.m_posTarget.m_fZ,
				m_paramAI_UseSkill.m_iHurtDelayTime,
				m_paramAI_UseSkill.m_fQueueTargetDirection,
				INVALID_ID
				);
		}
		else
		{
			Obj_UseItem(
				m_paramAI_UseItem.m_BagIndex,
				m_paramAI_UseItem.m_Target,
				m_paramAI_UseItem.m_posTarget,
				m_paramAI_UseItem.m_TargetPetGUID,
				m_paramAI_UseItem.m_TargetItem
				);
		}
		m_paramAI_UseItem.CleanUp();
	}
	else if(INVALID_SKILL_ID!=nQueuedSkill)
	{
		// ���ж���û����ɡ�
		if (FALSE == rMe.CanUseSkillNow(nQueuedSkill))
		{
			return;
		}

		if(FALSE ==CheckTargetValid(nQueuedSkill, m_paramAI_UseSkill.m_nQueueTargetObjID))
		{
			m_paramAI_UseSkill.CleanUp();
            // Ŀ���Ѿ���Ч��������
			ChangeState(ESTATE_IDLE);
			rMe.SetLockedTarget(INVALID_ID);
			return ;
		}
		if(FALSE==rMe.Skill_IsSkillCooldowned(nQueuedSkill))
		{
			Obj_Human* pHuman = static_cast<Obj_Human*>(GetCharacter());
			if(NULL!=pHuman)
			{
				pHuman->SendOperateResultMsg(OR_COOL_DOWNING);
			}
			m_paramAI_UseSkill.m_nQueuedSkill = INVALID_ID;
			return ; //��δ��ȴ
		}
		// ��ǰ״̬�Ƿ�����ʹ�ü���
		if(FALSE==pCharacter->Skill_CanUseThisSkillInThisStatus(nQueuedSkill))
		{
			Obj_Human* pHuman = static_cast<Obj_Human*>(GetCharacter());
			if(NULL!=pHuman)
			{
				pHuman->SendOperateResultMsg(OR_U_CANNT_DO_THIS_RIGHT_NOW);
			}
			m_paramAI_UseSkill.m_nQueuedSkill = INVALID_ID;
			return ;
		}
		Obj_UseSkill(
			nQueuedSkill,
			m_paramAI_UseSkill.m_nQueueTargetObjID,
			m_paramAI_UseSkill.m_QueueTargetPosition.m_fX,
			m_paramAI_UseSkill.m_QueueTargetPosition.m_fZ,
			m_paramAI_UseSkill.m_iHurtDelayTime,
			m_paramAI_UseSkill.m_fQueueTargetDirection,
			m_paramAI_UseSkill.m_guidQueueTarget
			);
		m_paramAI_UseSkill.m_nQueuedSkill = INVALID_SKILL_ID;
		rMe.SetLockedTarget(m_paramAI_UseSkill.m_nQueueTargetObjID);
	}
	else if(INVALID_SKILL_ID != nAutoActivedSkill)
	{
		// ���ж���û����ɡ�
		if (FALSE == rMe.CanUseSkillNow(nAutoActivedSkill))
		{
			return;
		}

		if(FALSE ==CheckTargetValid(nAutoActivedSkill, m_paramAI_UseSkill.m_nAutoShotTargetObjID))
		{
			m_paramAI_UseSkill.CleanUp();
            // Ŀ���Ѿ���Ч��������
			ChangeState(ESTATE_IDLE);
			rMe.SetLockedTarget(INVALID_ID);
			return ;
		}
		if(FALSE==rMe.Skill_IsSkillCooldowned(nAutoActivedSkill))
		{
			return ; //��δ��ȴ
		}
		// ��ǰ״̬�Ƿ�����ʹ�ü���
		if(FALSE==pCharacter->Skill_CanUseThisSkillInThisStatus(nAutoActivedSkill))
		{
			return ;
		}
		INT nRet = 0;
		nRet = Obj_UseSkill(
			m_paramAI_UseSkill.m_nAutoShotSkill,
			m_paramAI_UseSkill.m_nAutoShotTargetObjID,
			position.m_fX,
			position.m_fZ,
			m_paramAI_UseSkill.m_iHurtDelayTime,
			m_paramAI_UseSkill.m_fQueueTargetDirection,
			INVALID_ID
			);

		Obj_Character* pTarget = (Obj_Character*)(pCharacter->getScene()->GetObjManager()->GetObj(m_paramAI_UseSkill.m_nAutoShotTargetObjID));
		if (pTarget && OR_OK == nRet && pCharacter->IsEnemy(pTarget))
		{
		}
		else
		{
			m_paramAI_UseSkill.m_nAutoShotSkill = INVALID_SKILL_ID;
			m_paramAI_UseSkill.m_nAutoShotTargetObjID = INVALID_ID;
			//rMe.SetLockedTarget(INVALID_ID);
		}

	}
	else
	{
		// ���ж���û����ɡ�
		if (FALSE == rMe.CanUseSkillNow(INVALID_ID))
		{
			return;
		}

		ChangeState(ESTATE_IDLE);
		return ;
	}
	

	__LEAVE_FUNCTION

}

VOID AI_Human::AI_Logic_Dead( UINT uTime )
{
	__ENTER_FUNCTION

	if ( m_paramAI_Dead.m_AutoTimer.CountingTimer( uTime ) )
	{
		Relive( CITY_POS_RELIVE );
	}

	if(!GetCharacter()->IsDie())
	{
		ChangeState(ESTATE_IDLE);
	}

	__LEAVE_FUNCTION

}

/*
�������������״̬; �����������볣��״̬; 
���볬���޶�������ָ����; ������һ��Ŀ���
*/
VOID AI_Human::AI_Logic_TeamFollow( UINT uTime )
{
__ENTER_FUNCTION

	Obj_Human* pHuman = (Obj_Human*)GetCharacter();
	if ( pHuman == NULL )
	{
		Assert( FALSE && pHuman == NULL );
		return ;
	}

	BOOL bStopped = pHuman->IsCharacterLogicStopped();

#ifdef ___TEAMFOLLOW_LEADERDRIVEN___
	if ( bStopped )
	{
		Stop();
	}
#endif

#ifdef ___TEAMFOLLOW_MEMBERFOLLOW___

	const Obj_Human* pGuide; // ǰ���Ǹ���
	pGuide = pHuman->GetTeamInfo()->GetMyGuide();

	if ( pGuide==NULL )
	{ // û�������Լ�ǰ�棬���綼��û�й���������ô��ʲôҲ����
		return ;
	}

	if ( bStopped || pHuman->GetCharacterLogic() == CHARACTER_LOGIC_IDLE )
	{
		const WORLD_POS* pAimPos = pGuide->__GetWorldPos();
		WORLD_POS MyPos;
		FLOAT fDist;

		pHuman->GetPosition( MyPos, pAimPos, pGuide->getDir() );

		fDist = MySqrt( &MyPos, pHuman->__GetWorldPos() );
		if ( fDist > 0.2 )
		{
			FollowMove( pHuman, &MyPos, fDist );
		}
		else if ( pHuman->GetCharacterLogic() != CHARACTER_LOGIC_IDLE )
		{
			Stop();
		}
	}
	else if ( ++m_TeamFollowCheckTick>9 )
	{ // �˶�������Ҳ���ܵ���·���������ٶȣ��ȵ�
		m_TeamFollowCheckTick = 0;

		const WORLD_POS* pAimPos = pGuide->__GetWorldPos();
		WORLD_POS MyPos;
		FLOAT fDist;

		pHuman->GetPosition( MyPos, pAimPos, pGuide->getDir() );

		fDist = MySqrt( &MyPos, pHuman->__GetWorldPos() );
		if ( fDist > 0.2 )
		{
			FollowMove( pHuman, &MyPos, fDist );
		}
	}

#endif

__LEAVE_FUNCTION
}

BOOL AI_Human::CheckTargetValid(SkillID_t nSkillID, ObjID_t TargetID)
{
__ENTER_FUNCTION
	SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
	if (NULL != pSkillTemplate) 
	{
		if (pSkillTemplate->GetSelectType() == SELECT_TYPE_CHARACTER )
		{// ��Ҫѡ���ɫĿ��ĲŽ������е��ж�
			Obj* pObj = GetCharacter()->GetSpecificObjInSameSceneByID(TargetID);
			if(NULL==pObj)
			{
				return FALSE;
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				return FALSE;
			}
			Obj_Character* pCharacter = (Obj_Character*)pObj;
			INT nState = pSkillTemplate->GetTargetMustInSpecialState();
			BOOL bMustAlive = 0==nState || -1==nState;
			BOOL bMustDead = 1==nState || -1==nState;
			if (pCharacter)
			{
				BOOL bAlive = pCharacter->IsAlive();
				if (bAlive && bMustAlive)
				{// ����ֻ�Ի�Ŀ����Ч
					return TRUE;
				}
				else if (!bAlive && bMustDead)
				{// ����ֻ����Ŀ����Ч
					return TRUE;
				}
				else
					return FALSE;
			}
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
__LEAVE_FUNCTION
	return TRUE;
}
VOID AI_Human::ForceInterruptSkill(VOID)
{
__ENTER_FUNCTION
	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return;
	m_paramAI_UseSkill.CleanUp();
	GetGlobalActionDelegator().InterruptCurrentAction(*pCharacter);
	//if (ESTATE_IDLE!= GetAIState()->GetStateID() )
	//{
	//	ChangeState(ESTATE_IDLE);
	//}
__LEAVE_FUNCTION
}

VOID AI_Human::TeamMemeberToMove(VOID)
{
__ENTER_FUNCTION
#ifdef ___TEAMFOLLOW_LEADERDRIVEN___

	// ��Ӹ���Ķӳ�����Աģʽ
	INT nFollowedMembersCount = ((Obj_Human*)pCharacter)->__GetFollowedMembersCount();
	if ( nFollowedMembersCount>1 )
	{ // ��һ�����Լ������Դ��� 1 �ŵ�����
		int nNodeCount = wNumTargetPos;
		WORLD_POS aNodePos[MAX_CHAR_PATH_NODE_NUMBER];
		Obj_Human* pHuman = ((Obj_Human*)pCharacter);

		for( INT i=0; i<wNumTargetPos; ++i )
		{
			aNodePos[i] = *(paTargetPos+i);
		}

		for( INT iMember=1; iMember<nFollowedMembersCount; ++iMember )
		{
			INT nAppendNodeCount = 0;
			WORLD_POS aAppendNodePos[MAX_CHAR_PATH_NODE_NUMBER];
			const _FOLLOWEDMEMBER* pMoveMember;

			pMoveMember = ((Obj_Human*)pCharacter)->__GetFollowedMember( iMember );

			if ( pMoveMember->m_pHuman == NULL )
			{
				Assert( pMoveMember->m_pHuman );
				continue;
			}

			WORLD_POS MoveMemberPos = *(pMoveMember->m_pHuman->getWorldPos());
			WORLD_POS PrevMemberPos = *(pHuman->getWorldPos());

			pMoveMember->m_pHuman->getScene()->GetMap()->GetPathFinder()->FindPath(
				&MoveMemberPos,
				&PrevMemberPos,
				aAppendNodePos, // �����ǰһ������֮���·��
				nAppendNodeCount);	

			for( INT iAppend=0; iAppend<nNodeCount && nAppendNodeCount<=MAX_CHAR_PATH_NODE_NUMBER; ++iAppend )
			{ // ��ǰһ�����ѵ�·���Ӹ����Լ���·����
				aAppendNodePos[nAppendNodeCount++] = aNodePos[iAppend];
			}

			const WORLD_POS* LastPos;
			const WORLD_POS* PosBeforeLast;

			LastPos = &aAppendNodePos[nAppendNodeCount-1];
			PosBeforeLast = &aAppendNodePos[nAppendNodeCount-2];

			FLOAT fDist = MySqrt( PosBeforeLast, LastPos );

			if ( fDist<1.0 )
			{ // ������һ��·��̫�̣���ֱ��ȥ��
				--nAppendNodeCount;
			}
			else if ( fDist<1.5 )
			{ // ������һ��·������̫���������һ������Ͳ�����
				--nAppendNodeCount;
				GetFollowPos( aAppendNodePos[nAppendNodeCount-1], PosBeforeLast, LastPos, fDist);
			}
			else
			{
				GetFollowPos( aAppendNodePos[nAppendNodeCount-1], PosBeforeLast, LastPos, fDist);
			}

			// ��ʱ������ʧ�����
			// pMoveMember->m_pHuman->GetHumanAI()->SetHumanAI( HUMAN_AI_MOVE );
			pMoveMember->m_pHuman->Do_Move( nHandleID, nAppendNodeCount, aAppendNodePos, TRUE );

			for( INT iSwap=0; iSwap<nAppendNodeCount; ++iSwap )
			{
				aNodePos[iSwap] = aAppendNodePos[iSwap];
			}

			pHuman = pMoveMember->m_pHuman;

				if ( nEquipCount > 0 )
				{
					HUMAN_EQUIP nEquipPoint = (HUMAN_EQUIP)(rand()%nEquipCount);
					HumanItemLogic::EraseEquip( pHuman, (HUMAN_EQUIP)(anEquipIndex[nEquipPoint]) );

					Item *pEquip = HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)(anEquipIndex[nEquipPoint]));
					GCDiscardEquipResult msgDiscardEquipResult;
					msgDiscardEquipResult.SetResult( DISCARDEQUIP_SUCCESS );
					msgDiscardEquipResult.SetEquipPoint( (BYTE)(anEquipIndex[nEquipPoint]) );
					msgDiscardEquipResult.SetItemTableIndex( pEquip->GetItemTableIndex() );
					((GamePlayer*)(pHuman->GetPlayer()))->SendPacket(&msgDiscardEquipResult);
					//����ɼ�
					if(pHuman->IsVisualPart(nEquipPoint))
					{	
						GCCharEquipment msgCharEquipment;
						msgCharEquipment.setObjID(pHuman->GetID());
						msgCharEquipment.setID(nEquipPoint,-1);
						pHuman->getScene()->BroadCast(&msgCharEquipment,pHuman,TRUE);
					}
				}


			}
		}
	}

#endif
__LEAVE_FUNCTION
}

BOOL AI_Human::IsEnterCombatState(ObjID_t idSkill, ObjID_t idTarget)
{
__ENTER_FUNCTION
	//��ý�ɫ����
	Obj_Character *pCharacter = GetCharacter();
	Assert( pCharacter != NULL );
	if ( pCharacter == NULL )
		return FALSE;
	//���SkillID�Ƿ�Ϸ�
	if(INVALID_SKILL_ID==idSkill)
	{
		m_paramAI_UseSkill.CleanUp();
		return FALSE;
	}
	//��ɫ�Ƿ���
	if(FALSE==pCharacter->IsAlive())
	{
		return FALSE;
	}
	//��ü���ģ��
	SkillTemplateData_T const* pSkill = g_SkillTemplateDataMgr.GetInstanceByID(idSkill);
	if(NULL == pSkill)
	{
		return FALSE;
	}
	//����о���Ŀ��������֮
	if(INVALID_ID!=idTarget)
	{
		pCharacter->SetLockedTarget(idTarget);
	}
	//�ж��Ƿ�Ϊ�Զ������ͷż���
	if(TRUE == pSkill->IsAutoShotSkill()&&Obj::OBJ_TYPE_HUMAN==pCharacter->GetObjType())
	{// ֻ���Զ������ͷŵļ��ܲ��ܽ��롮ս����״̬
		m_paramAI_UseSkill.m_nAutoShotSkill = idSkill;
		m_paramAI_UseSkill.m_nAutoShotTargetObjID = idTarget;

		return TRUE;
	}

	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}

VOID AI_Human::PushSkillToQueue(ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, INT iHurtDelayTime, FLOAT fDir, GUID_t guidTarget)
{
__ENTER_FUNCTION
	//��ü���ģ��
	SkillTemplateData_T const* pSkill = g_SkillTemplateDataMgr.GetInstanceByID(idSkill);
	if(NULL == pSkill)
	{
		return ;
	}
	//�����ɫ����Ϸ�
	if(NULL!=m_pCharacter)
	{
		//��ȡ��ɫ���ϵ�Ŀ�꼰���Ķ���ʵ��
		TargetingAndDepletingParams_T rParams = m_pCharacter->GetTargetingAndDepletingParams();

		//���������������Ͽɿ���ִ���µĶ���
		if(FALSE==GetGlobalActionDelegator().CanDoNextAction(*m_pCharacter))
		{
			//���˵�����ظ�������
			if(idSkill==rParams.GetActivatedSkill())
			{
				return; 
			}
		}

		//��������ж��Զ�������ж�֮
		if(0!=pSkill->CanInterruptAutoShot())
		{
			m_paramAI_UseSkill.m_nAutoShotSkill = INVALID_SKILL_ID;
			m_paramAI_UseSkill.m_nAutoShotTargetObjID = INVALID_ID;
		}

		m_paramAI_UseSkill.m_nQueuedSkill				= idSkill;
		m_paramAI_UseSkill.m_nQueueTargetObjID			= idTarget;
		m_paramAI_UseSkill.m_fQueueTargetDirection		= fDir;
		m_paramAI_UseSkill.m_QueueTargetPosition.m_fX	= fTargetX;
		m_paramAI_UseSkill.m_QueueTargetPosition.m_fZ	= fTargetZ;
		m_paramAI_UseSkill.m_guidQueueTarget			= guidTarget;
		m_paramAI_UseSkill.m_iHurtDelayTime				= iHurtDelayTime;
	}
__LEAVE_FUNCTION
}


VOID AI_Human::PushItemToQueue(ID_t nBagIndex, ObjID_t nTargetObj, const WORLD_POS& posTarget, PET_GUID_t& guidTargetPet, ID_t nTargetItem)
{
__ENTER_FUNCTION
	// need gaoqi modify...
	if(NULL!=m_pCharacter)
	{
		TargetingAndDepletingParams_T rParams = m_pCharacter->GetTargetingAndDepletingParams();
		if(FALSE==GetGlobalActionDelegator().CanDoNextAction(*m_pCharacter))
		{
			if(nBagIndex==rParams.GetBagIndexOfDepletedItem())
			{
				return;//���˵�����ظ�����Ʒ
			}
		}
		m_paramAI_UseItem.m_BagIndex		= nBagIndex;
		m_paramAI_UseItem.m_TargetItem		= nTargetItem;
		m_paramAI_UseItem.m_Target			= nTargetObj;
		m_paramAI_UseItem.m_posTarget		= posTarget;
		m_paramAI_UseItem.m_TargetPetGUID	= guidTargetPet;
		
	}
__LEAVE_FUNCTION
}

VOID AI_Human::After_Die(ObjID_t idKiller)
{
	LeaveTeam_After_Die();
	RleasePet_After_Die();
	CanRelive_After_Die();
	Stall_After_Die();
	AddEnemy_After_Die( idKiller );
	Penalty_After_Die(idKiller);
}

VOID AI_Human::LeaveTeam_After_Die()
{
__ENTER_FUNCTION
	Obj_Human* pHuman = (Obj_Human*)GetCharacter();
	if ( pHuman != NULL )
	{
		if ( pHuman->__GetTeamFollowFlag() )
		{ // ���������Ӹ���״̬����ֹͣ����״̬��fancy
			pHuman->__StopTeamFollow( TRUE );
		}
	}
__LEAVE_FUNCTION
}

VOID AI_Human::RleasePet_After_Die()
{
__ENTER_FUNCTION
	//////////////////////////////////////////////////////
	//��������ϵĳ���ȫ���ó�ս��״̬
	Obj_Human* pHuman = ((Obj_Human*)GetCharacter());
	if ( pHuman != NULL )
	{
        pHuman->ReCallPet();
	}
__LEAVE_FUNCTION
}

VOID AI_Human::CanRelive_After_Die()
{
__ENTER_FUNCTION
	WORLD_POS pos;
	ORESULT nRet = OR_ERROR;
	//nRet = pCharacter->Enter_Dead( idKiller );
	//Assert( OR_SUCCEEDED( nRet ) && "AI_Human::Enter_Dead" );

	m_paramAI_Dead.m_AutoTimer.BeginTimer( g_Config.m_ConfigInfo.m_OutGhostTime, g_pTimeManager->CurrentTime() );

	// ���������û�б�������ļ���
	//inerrupt skill execution
	m_paramAI_UseSkill.CleanUp();
	// ����������������Ϣ���ͻ���
	BOOL bCanRelive = ((Obj_Human*)GetCharacter())->IsCanRelive();

	GCPlayerDie msgPlayerDie;
	msgPlayerDie.SetCanRelive( bCanRelive );
	msgPlayerDie.SetTime( g_Config.m_ConfigInfo.m_OutGhostTime + (g_pTimeManager->CurrentTime() - m_paramAI_Dead.m_AutoTimer.GetTickOldTime()) );

	((Obj_Human*)GetCharacter())->GetPlayer()->SendPacket(&msgPlayerDie);
__LEAVE_FUNCTION
}

VOID AI_Human::Penalty_After_Die(ObjID_t idKiller)
{
__ENTER_FUNCTION

	// �ж��������ô������
	Obj_Character* pCharacter = GetCharacter();
	Obj *pKiller = pCharacter->getScene()->GetObjManager()->GetObj( idKiller );
	Obj::ObjType eObjType = pKiller->GetObjType();
	if ( eObjType != Obj::OBJ_TYPE_MONSTER )
	{
		// ������ǹ���ɱ���ģ������ͷ�
		return;
	}

	Obj_Human* pHuman = (Obj_Human*)(GetCharacter() );
	if (!pHuman) {
		return ;
	}

	INT nPenaltyID = 0;
	PenaltyType_After_Die(nPenaltyID, idKiller);
	
	const _DIE_PENALTY_INFO *pPenaltyInfo = g_pDiePenaltyManager->GetDiePenalty( nPenaltyID );
	if (!pPenaltyInfo) {
		return ;
	}

	// ��ǰ�������ͷ��͸���û�й�ϵֻ��һ�������ͷ�������װ�����־ã����������ͷ�ֻ�ڱ����������ʱ�����

	// ������С�ڵ���ʮ�������;öȲ�����
	if ( pHuman->GetLevel() > 10 )
	{
		// װ���;ö���ʧ����, ���кܶ��ֳɵĳͷ������������������ͬһλ������
		Penalty_EquipDur_After_Die( pPenaltyInfo, pHuman );
	}


__LEAVE_FUNCTION
}

VOID AI_Human::Stall_After_Die(VOID)
{
	__ENTER_FUNCTION
	//////////////////////////////////////////////////////
	//��������ϵĳ���ȫ���ó�ս��״̬
	Obj_Human* pHuman = ((Obj_Human*)GetCharacter());
	if ( pHuman != NULL )
	{
		if (pHuman->GetStallIsOpen())
		{
			pHuman->m_StallBox.CleanUp();

			GCStallClose MsgClose;
			pHuman->GetPlayer()->SendPacket(&MsgClose);
		}
	}
	__LEAVE_FUNCTION
}


VOID AI_Human::AddEnemy_After_Die(ObjID_t idKiller)
{
__ENTER_FUNCTION
	// �ж��������ô������
	Obj_Character* pCharacter = GetCharacter();
	Obj *pKiller = pCharacter->getScene()->GetObjManager()->GetObj( idKiller );
	Obj::ObjType eObjType = pKiller->GetObjType();

	// ������������ɱ���򲻽�ɱ������ӵ������б���
	if ( eObjType != Obj::OBJ_TYPE_HUMAN )
	{
		return;
	}

	Obj_Human* pHuman = ((Obj_Human*)GetCharacter());
	if ( !pHuman )
	{
		return;
	}

	if ( pHuman->GetID() == idKiller )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "Ai_Human: Name=%s attempt to add self into enemylist.",
			pHuman->GetName() );
		return;
	}
	
	HumanRelation* pMyRelation = pHuman->GetHumanRelation();

	Obj_Human* pEnemy = pHuman->getScene()->GetHumanManager()->GetHuman(idKiller);		
	AssertEx(pEnemy,"Obj_Character��Obj_Humanָ��ת������,AI_Human");
	
	// �鿴��ϵ�����Ƿ��Ѿ��и����
	RELATION_TYPE RelationType;
	const _OWN_RELATION* Rel = pMyRelation->GetRelationInfo( pEnemy->GetName() );
	
	// ���û���ҵ������Ǻ�ɱ����֮ǰû�й��罻��ϵ
	if ( NULL == Rel )
	{
		RelationType = RELATION_TYPE_STRANGER;
	}
	else
	{
		RelationType = pMyRelation->GetRelationType( pEnemy->GetGUID() );
	}

	switch( RelationType )
	{
	case RELATION_TYPE_ENEMY:	// ����Ѿ��ǳ��ˣ��򲻽��в���
		{
			return;
		}
	break;
	case RELATION_TYPE_BLACKNAME:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_MASTER:
	case RELATION_TYPE_PRENTICE:
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_FRIEND:
	case RELATION_TYPE_STRANGER:
		{ // ��ȷ��ϵ
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_ADDTOENEMY;
			pRelation->SetGUID( pHuman->GetGUID() );
			REQUEST_ADD_RELATION& Relation = pRelation->m_AddRelation;
			Relation.CleanUp();
			Relation.SetTargetGUID( pEnemy->GetGUID() );
			Relation.SetTargetName( pEnemy->GetName() );
			Relation.SetRelationType( RELATION_TYPE_ENEMY );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "AI_Human: Name=%s add Name=%s to black list.",
				pHuman->GetName(), pEnemy->GetName() );
		}
	break;
	default:
		Assert(FALSE);
		break;
	}

__LEAVE_FUNCTION
}


VOID AI_Human::PenaltyType_After_Die(INT& nPenaltyID, ObjID_t idKiller)
{
__ENTER_FUNCTION
	// �����ͷ�
	Obj_Character* pCharacter = GetCharacter();
	if (!pCharacter) {
		return ;
	}

	INT nSceneType, nPKValueIndex, nDieType;
	nSceneType = pCharacter->getScene()->GetSceneType();
	if ( pCharacter->GetLevel() < 10 )
	{
		nPKValueIndex = 0;
	}
	else
	{
		INT nPKValue = ((Obj_Human*)pCharacter)->GetPKValue();
		if ( nPKValue < PK_VALUE_RANGE_1 )
		{
			nPKValueIndex = 1;
		}
		else if ( nPKValue < PK_VALUE_RANGE_20 )
		{
			nPKValueIndex = 2;
		}
		else if ( nPKValue < PK_VALUE_RANGE_40 )
		{
			nPKValueIndex = 3;
		}
		else if ( nPKValue < PK_VALUE_RANGE_60 )
		{
			nPKValueIndex = 4;
		}
		else if ( nPKValue < PK_VALUE_RANGE_80 )
		{
			nPKValueIndex = 5;
		}
		else
		{
			nPKValueIndex = 6;
		}
	}

	Obj *pKiller = pCharacter->getScene()->GetObjManager()->GetObj( idKiller );
	Obj::ObjType eObjType = pKiller->GetObjType();
	switch ( eObjType )
	{
	case Obj::OBJ_TYPE_HUMAN:
		{
			// �Ƿ�Ϊ�д��Ŀ�����
			if ( FALSE )
			{
				nDieType = HUMAN_DIE_TYPE_INTERCHANGE;
			}
			else
			{
				nDieType = HUMAN_DIE_TYPE_PLAYER_KILL;
			}
		}
		break;
	case Obj::OBJ_TYPE_PET:
		{
			Obj_Character *pReadyKiller = ((Obj_Pet*)pKiller)->GetOwner();
			if ( pReadyKiller->GetObjType() == Obj::OBJ_TYPE_HUMAN )
			{
				// �Ƿ�Ϊ�д��Ŀ�����
				if ( FALSE )
				{
					nDieType = HUMAN_DIE_TYPE_INTERCHANGE;
				}
				else
				{
					nDieType = HUMAN_DIE_TYPE_PLAYER_KILL;
				}
			}
			else
			{
				nDieType = HUMAN_DIE_TYPE_MONSTER_KILL;
			}
		}
		break;
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_DROP_ITEM:
	case Obj::OBJ_TYPE_PLATFORM:
	case Obj::OBJ_TYPE_SPECIAL:
	default:
		nDieType = HUMAN_DIE_TYPE_MONSTER_KILL;
		break;
	}

	nPenaltyID = (nSceneType*100)+(nPKValueIndex*10)+nDieType;


__LEAVE_FUNCTION
}

VOID AI_Human::Penalty_Exp_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman)
{
	__ENTER_FUNCTION
	if (pPenaltyInfo == NULL || pHuman == NULL)  {
		return ;
	}

	// Exp
	UINT uMinExpModify, uMaxExpModify;
	if ( pPenaltyInfo->m_bPercent_ExpMin )
	{
		uMinExpModify = (UINT)((double)(pHuman->GetExp()) * (double)(pPenaltyInfo->m_nPercent_ExpMin) / 100.0);
	}
	else
	{
		uMinExpModify = (UINT)(pPenaltyInfo->m_nExpMin);
	}
	if ( pPenaltyInfo->m_bPercent_ExpMax )
	{
		uMaxExpModify = (UINT)((double)(pHuman->GetExp()) * (double)(pPenaltyInfo->m_nPercent_ExpMax) / 100.0);
	}
	else
	{
		uMaxExpModify = (UINT)(pPenaltyInfo->m_nExpMax);
	}
	if ( uMaxExpModify >= uMinExpModify )
	{
		UINT uModify;
		if ( uMaxExpModify != uMinExpModify )
		{
			uModify = rand()%(uMaxExpModify - uMinExpModify) + uMinExpModify;
		}
		else
		{
			uModify = uMinExpModify;
		}
		if ( pHuman->GetExp() >= uModify )
		{
			pHuman->SetExp( pHuman->GetExp() - uModify );
		}
		else
		{
			pHuman->SetExp( 0 );
		}

		if(uModify > 0)
		{
			GCChat msgChat;
			CHAR szChat[128];
			sprintf(szChat, "@*;SrvMsg;DIE_LOSE_EXP_MSG;%u", uModify);
			INT Size = (INT)strlen(szChat) ;
			msgChat.SetChatType( CHAT_TYPE_SELF) ;
			msgChat.SetContexSize( Size ) ;
			msgChat.SetContex( szChat ) ;
			msgChat.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
			msgChat.SetSourName( pHuman->GetName()) ;
			msgChat.SetSourID(pHuman->GetID()) ;
			pHuman->GetPlayer()->SendPacket(&msgChat);
		}
	}
	__LEAVE_FUNCTION

}

VOID AI_Human::Penalty_Money_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman)
{
	__ENTER_FUNCTION
	if (pPenaltyInfo == NULL || pHuman == NULL)  {
		return ;
	}

	// Money
	UINT uMinMoneyModify, uMaxMoneyModify;
	if ( pPenaltyInfo->m_bPercent_MoneyMin )
	{
		uMinMoneyModify = (UINT)((double)(pHuman->GetMoney()) * (double)(pPenaltyInfo->m_nPercent_MoneyMin) / 100.0);
	}
	else
	{
		uMinMoneyModify = pPenaltyInfo->m_nMoneyMin;
	}
	if ( pPenaltyInfo->m_bPercent_MoneyMax )
	{
		uMaxMoneyModify = (UINT)((double)(pHuman->GetMoney()) * (double)(pPenaltyInfo->m_nPercent_MoneyMax) / 100.0);
	}
	else
	{
		uMaxMoneyModify = (UINT)(pPenaltyInfo->m_nMoneyMax);
	}
	if ( uMaxMoneyModify >= uMinMoneyModify )
	{
		UINT uModify;
		if ( uMaxMoneyModify != uMinMoneyModify )
		{
			uModify = rand()%(uMaxMoneyModify - uMinMoneyModify) + uMinMoneyModify;
		}
		else
		{
			uModify = uMinMoneyModify;
		}
		if ( pHuman->GetMoney() > uModify )
		{
			pHuman->SetMoney( pHuman->GetMoney() - uModify );

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_DIE_COST;	
			MoneyLogParam.Count		=	uModify;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);
		}
		else
		{
			if(pHuman->GetMoney()>0)
			{
				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_DIE_COST;	
				MoneyLogParam.Count		=	pHuman->GetMoney();
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
			pHuman->SetMoney( 0 );
		}

		if(uModify > 0)
		{
			GCChat msgChat ;
			CHAR szChat[128];
			sprintf(szChat, "@*;SrvMsg;DIE_LOSE_MONEY_MSG;%u", uModify);
			INT Size = (INT)strlen(szChat) ;
			msgChat.SetChatType( CHAT_TYPE_SELF) ;
			msgChat.SetContexSize( Size ) ;
			msgChat.SetContex( szChat ) ;
			msgChat.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
			msgChat.SetSourName( pHuman->GetName()) ;
			msgChat.SetSourID(pHuman->GetID()) ;
			pHuman->GetPlayer()->SendPacket(&msgChat);
		}
	}
	__LEAVE_FUNCTION
}

VOID AI_Human::Penalty_EquipDur_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman)
{
	__ENTER_FUNCTION
	if (pPenaltyInfo == NULL || pHuman == NULL)  {
		return ;
	}

	// װ���;ö���ʧ// ����ԭ�Ȱ��ո��ʵ���־õĴ���
	/*if ( pPenaltyInfo->m_bPercent_EquipDurMin && pPenaltyInfo->m_bPercent_EquipDurMax )
	{
		INT nMinEquipDurPercent, nMaxEquipDurPercent;
		nMinEquipDurPercent = pPenaltyInfo->m_nPercent_EquipDurMin;
		nMaxEquipDurPercent = pPenaltyInfo->m_nPercent_EquipDurMax;

		Item *pEquip;
		INT i;
		for ( i= 0; i < HEQUIP_NUMBER; i++ )
		{
			pEquip = HumanItemLogic::GetEquip( pHuman, (HUMAN_EQUIP)i );
			if ( pEquip != NULL && !pEquip->IsEmpty() )
			{
				INT nEquipDurPercent;
				if ( nMinEquipDurPercent != nMaxEquipDurPercent )
				{
					nEquipDurPercent = rand()%(nMaxEquipDurPercent - nMinEquipDurPercent) + nMinEquipDurPercent;
				}
				else
				{
					nEquipDurPercent = nMinEquipDurPercent;
				}

				INT nReadyEquipDur = pEquip->GetDurPoints() - (nEquipDurPercent * pEquip->GetMaxDurPoint())/100;
				if ( nReadyEquipDur < 0 )
					nReadyEquipDur = 0;

				HumanItemLogic::SetEquipDur( pHuman, (HUMAN_EQUIP)i, nReadyEquipDur );

				_ITEM itemTemp;
				pEquip->SaveValueTo( &itemTemp );
				GCDetailEquipList msgDetailEquipList;
				msgDetailEquipList.setObjID( pHuman->GetID() );
				msgDetailEquipList.SetAskMode( ASK_EQUIP_SET );
				msgDetailEquipList.SetEquipData( (HUMAN_EQUIP)i, &itemTemp );
				((GamePlayer*)(pHuman->GetPlayer()))->SendPacket(&msgDetailEquipList);
			}
		}
	}*/

	Item *pEquip;
	for ( INT i=0; i<HEQUIP_NUMBER; ++i )
	{
		pEquip = HumanItemLogic::GetEquip( pHuman, (HUMAN_EQUIP)i );
		if ( pEquip != NULL && !pEquip->IsEmpty() )
		{
			INT nReadyEquipDur = 0;

			// �۳���װ�����;õ�5%���ǰ�װ����10%
			if ( pEquip->GetItemBind() )
			{
				INT al = pEquip->GetDurPoints();
				nReadyEquipDur = pEquip->GetDurPoints() - (INT)pEquip->GetDurPoints()*0.05;
			}
			else
			{
				nReadyEquipDur = pEquip->GetDurPoints() - (INT)pEquip->GetDurPoints()*0.1;
			}	
			
			if ( nReadyEquipDur < 0 )
				nReadyEquipDur = 0;

			HumanItemLogic::SetEquipDur( pHuman, (HUMAN_EQUIP)i, nReadyEquipDur );

			_ITEM itemTemp;
			pEquip->SaveValueTo( &itemTemp );
			GCDetailEquipList msgDetailEquipList;
			msgDetailEquipList.setObjID( pHuman->GetID() );
			msgDetailEquipList.SetAskMode( ASK_EQUIP_SET );
			msgDetailEquipList.SetEquipData( (HUMAN_EQUIP)i, &itemTemp );
			((GamePlayer*)(pHuman->GetPlayer()))->SendPacket(&msgDetailEquipList);
		}
	}

	__LEAVE_FUNCTION
}

VOID AI_Human::Penalty_ItemDrop_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman)
{
	__ENTER_FUNCTION
	if (pPenaltyInfo == NULL || pHuman == NULL)  {
		return ;
	}

	// ��Ʒ����
	if ( pPenaltyInfo->m_bPercent_ItemDropMin && pPenaltyInfo->m_bPercent_ItemDropMax )
	{
		INT nMinItemDropPercent, nMaxItemDropPercent;
		nMinItemDropPercent = pPenaltyInfo->m_nPercent_ItemDropMin;
		nMaxItemDropPercent = pPenaltyInfo->m_nPercent_ItemDropMax;

		INT nItemDropPercent;
		if ( nMinItemDropPercent != nMaxItemDropPercent )
		{
			nItemDropPercent = rand()%(nMaxItemDropPercent - nMinItemDropPercent) + nMinItemDropPercent;
		}
		else
		{
			nItemDropPercent = nMinItemDropPercent;
		}

		// ����
		if ( rand()%100 < nItemDropPercent )
		{
			INT nSize = pHuman->GetBaseContain()->GetContainerSize() - pHuman->GetBaseContain()->CalcItemSpace();
			//nSize += pHuman->GetMatContain()->GetContainerSize() - pHuman->GetMatContain()->CalcItemSpace();
			if ( nSize > 0 )
			{
				BOOL bDropped = FALSE;
				INT nItemPoint = rand()%nSize;

				INT nMaxBagSize = pHuman->GetBaseContain()->GetContainerSize();
				//pHuman->GetEquipContain()->GetContainerSize();

				Item *pItemTemp;

				INT i;
				INT nItemCount = 0;
				for ( i = 0; i < nMaxBagSize; i++ )
				{
					pItemTemp = HumanItemLogic::GetBagItem( pHuman, i + BASE_CONTAINER_OFFSET );
					if ( pItemTemp != NULL && !pItemTemp->IsEmpty() )
					{
						if ( nItemPoint == nItemCount )
						{
							GCChat msgChat ;
							CHAR szChat[128];
							sprintf(szChat, "@*;SrvMsg;DIE_LOSE_ITEM_MSG;%u", pItemTemp->GetDataType());
							INT Size = (INT)strlen(szChat) ;
							msgChat.SetChatType( CHAT_TYPE_SELF) ;
							msgChat.SetContexSize( Size ) ;
							msgChat.SetContex( szChat ) ;
							msgChat.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
							msgChat.SetSourName( pHuman->GetName()) ;
							msgChat.SetSourID(pHuman->GetID()) ;
							pHuman->GetPlayer()->SendPacket(&msgChat);

							ITEM_LOG_PARAM	ItemLogParam;
							ItemLogParam.OpType	=	ITEM_DIE_LOST;
							BOOL bRet =	HumanItemLogic::EraseBagItem(&ItemLogParam, pHuman, i + BASE_CONTAINER_OFFSET );
							Assert(bRet);
							if(bRet) SaveItemLog(&ItemLogParam);

							GCDiscardItemResult msgDiscardItemResult;
							msgDiscardItemResult.setItemTableIndex( pItemTemp->GetItemTableIndex() );
							msgDiscardItemResult.setResult( DISCARDITEM_SUCCESS );
							msgDiscardItemResult.setBagIndex( i + BASE_CONTAINER_OFFSET );
							((GamePlayer*)(pHuman->GetPlayer()))->SendPacket(&msgDiscardItemResult);

							bDropped = TRUE;
							break;
						}
						nItemCount++;
					}
				}

				//if ( !bDropped )
				//{
				//	//nMaxBagSize = pHuman->GetMatContain()->GetContainerSize();

				//	nItemCount = 0;
				//	for ( i = 0; i < nMaxBagSize; i++ )
				//	{
				//		pItemTemp = HumanItemLogic::GetItem( pHuman, i + MAT_CONTAINER_OFFSET );
				//		if ( pItemTemp != NULL && !pItemTemp->IsEmpty() )
				//		{
				//			if ( nItemPoint == nItemCount )
				//			{
				//				GCChat msgChat ;
				//				CHAR szChat[128];
				//				sprintf(szChat, "@*;SrvMsg;DIE_LOSE_ITEM_MSG;%u", pItemTemp->GetDataType());
				//				INT Size = (INT)strlen(szChat) ;
				//				msgChat.SetChatType( CHAT_TYPE_SELF) ;
				//				msgChat.SetContexSize( Size ) ;
				//				msgChat.SetContex( szChat ) ;
				//				msgChat.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
				//				msgChat.SetSourName( pHuman->GetName()) ;
				//				msgChat.SetSourID(pHuman->GetID()) ;
				//				pHuman->GetPlayer()->SendPacket(&msgChat);

				//				ITEM_LOG_PARAM	ItemLogParam;
				//				ItemLogParam.OpType	=	ITEM_DIE_LOST;
				//				BOOL bRet =	HumanItemLogic::EraseItem(&ItemLogParam, pHuman, i + MAT_CONTAINER_OFFSET );
				//				Assert(bRet);
				//				if(bRet)
				//				{
				//					SaveItemLog(&ItemLogParam);
				//				}

				//				GCDiscardItemResult msgDiscardItemResult;
				//				msgDiscardItemResult.setItemTableIndex( pItemTemp->GetItemTableIndex() );
				//				msgDiscardItemResult.setResult( DISCARDITEM_SUCCESS );
				//				msgDiscardItemResult.setBagIndex( i + MAT_CONTAINER_OFFSET );
				//				((GamePlayer*)(pHuman->GetPlayer()))->SendPacket(&msgDiscardItemResult);
				//				break;
				//			}
				//			nItemCount++;
				//		}
				//	}
				//}
			}
		}
	}
	__LEAVE_FUNCTION
}

VOID AI_Human::Penalty_EquipDrop_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman)
{
__ENTER_FUNCTION
	if (pPenaltyInfo == NULL || pHuman == NULL)  {
		return ;
	}
	// װ������
	if ( pPenaltyInfo->m_bPercent_EquipDropMin && pPenaltyInfo->m_bPercent_EquipDropMax )
	{
		INT nMinEquipDropPercent, nMaxEquipDropPercent;
		nMinEquipDropPercent = pPenaltyInfo->m_nPercent_EquipDropMin;
		nMaxEquipDropPercent = pPenaltyInfo->m_nPercent_EquipDropMax;

		INT nEquipDropPercent;
		if ( nMinEquipDropPercent != nMaxEquipDropPercent )
		{
			nEquipDropPercent = rand()%(nMaxEquipDropPercent - nMinEquipDropPercent) + nMinEquipDropPercent;
		}
		else
		{
			nEquipDropPercent = nMinEquipDropPercent;
		}

		// ����
		if ( rand()%100 < nEquipDropPercent )
		{
			Item *pEquipTemp;
			INT anEquipIndex[HEQUIP_NUMBER];
			INT nEquipDropIndex, nEquipCount;

			nEquipCount = 0;
			for ( nEquipDropIndex = 0; nEquipDropIndex < HEQUIP_NUMBER; nEquipDropIndex++ )
			{
				pEquipTemp = HumanItemLogic::GetEquip( pHuman, (HUMAN_EQUIP)nEquipDropIndex );
				if ( pEquipTemp != NULL && !pEquipTemp->IsEmpty() )
				{
					anEquipIndex[nEquipCount++] = nEquipDropIndex;
				}
			}

			if ( nEquipCount > 0 )
			{
				HUMAN_EQUIP nEquipPoint = (HUMAN_EQUIP)(rand()%nEquipCount);
				HumanItemLogic::EraseEquip( pHuman, (HUMAN_EQUIP)(anEquipIndex[nEquipPoint]) );

				Item *pEquip = HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)(anEquipIndex[nEquipPoint]));

				GCChat msgChat ;
				CHAR szChat[128];
				sprintf(szChat, "@*;SrvMsg;DIE_LOSE_EQUIPMENT_MSG;%u", pEquip->GetDataType());
				INT Size = (INT)strlen(szChat) ;
				msgChat.SetChatType( CHAT_TYPE_SELF) ;
				msgChat.SetContexSize( Size ) ;
				msgChat.SetContex( szChat ) ;
				msgChat.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
				msgChat.SetSourName( pHuman->GetName()) ;
				msgChat.SetSourID(pHuman->GetID()) ;
				pHuman->GetPlayer()->SendPacket(&msgChat);

				GCDiscardEquipResult msgDiscardEquipResult;
				msgDiscardEquipResult.SetResult( DISCARDEQUIP_SUCCESS );
				msgDiscardEquipResult.SetEquipPoint( (BYTE)(anEquipIndex[nEquipPoint]) );
				msgDiscardEquipResult.SetItemTableIndex( pEquip->GetItemTableIndex() );
				((GamePlayer*)(pHuman->GetPlayer()))->SendPacket(&msgDiscardEquipResult);
				//����ɼ�
				if(pHuman->IsVisualPart(nEquipPoint))
				{	
					GCCharEquipment msgCharEquipment;
					msgCharEquipment.setObjID(pHuman->GetID());
					msgCharEquipment.setID(nEquipPoint,-1);
					pHuman->getScene()->BroadCast(&msgCharEquipment,pHuman,TRUE);
				}
			}
		}
	}
	__LEAVE_FUNCTION
}


//BOOL AI_Human::AI_Logic_Move( UINT uTime )
//{
//__ENTER_FUNCTION
//
//	Obj_Character *pCharacter = GetCharacter();
//	Assert( pCharacter != NULL );
//	if ( pCharacter == NULL )
//	{
//		return FALSE;
//	}
//
//	if ( pCharacter->IsLimitMove() )
//	{
//		GCArrive	msgArrive;
//		msgArrive.setObjID( pCharacter->GetID() );
//		msgArrive.setHandleID( pCharacter->GetMoveHandleID() );
//		msgArrive.setWorldPos( *(pCharacter->getWorldPos()) );
//
//		pCharacter->getScene()->BroadCast( &msgArrive, pCharacter, TRUE ) ;
//
//		OnAIStopped( );
//		return TRUE;
//	}
//
//	Obj_Character& rMe = *pCharacter;
//	BOOL bStopped = rMe.IsCharacterLogicStopped();
//	if ( bStopped )
//	{
//		Obj_Human* pHuman = (Obj_Human*)pCharacter ;
//		pHuman->GetDB()->SetDBPosition(*pHuman->getWorldPos());
//		SkillID_t& rAutoActivedSkill = m_paramAI_UseSkill.m_nAutoShotSkill;
//		SkillID_t& rQueuedSkill = m_paramAI_UseSkill.m_nQueuedSkill;
//		if(INVALID_SKILL_ID!=rQueuedSkill||INVALID_SKILL_ID != rAutoActivedSkill)
//		{
//			SetHumanAI(HUMAN_AI_USE_SKILL);				
//		}
//		else
//		{
//			OnAIStopped( );
//		}
//	}
//	return TRUE;
//__LEAVE_FUNCTION
//	return FALSE;
//}
//BOOL AI_Human::AI_Logic( UINT uTime )
//{
//__ENTER_FUNCTION
//
//	Obj_Character *pCharacter = GetCharacter();
//	if ( pCharacter == NULL )
//		return TRUE;
//
//	BOOL bResult;
//	switch( GetHumanAI() )
//	{
//	case HUMAN_AI_IDLE:
//		bResult = Logic_Idle( uTime );
//		break;
//	case HUMAN_AI_MOVE:
//		bResult = Logic_Move( uTime );
//		break;
//	case HUMAN_AI_USE_SKILL:
//		bResult = Logic_UseSkill( uTime );
//		break;
//	case HUMAN_AI_USE_ABILITY:
//		bResult = Logic_UseAbility( uTime );
//		break;
//	case HUMAN_AI_DEAD:
//		bResult = Logic_Dead( uTime );
//		break;
//	case HUMAN_AI_TEAMFOLLOW:
//		bResult = Logic_TeamFollow( uTime );
//		break;
//	default:
//		bResult = FALSE;
//		break;
//	}
//	return bResult;
//__LEAVE_FUNCTION
//	return FALSE ;
//}
//ORESULT AI_Human::Enter_Idle( VOID )
//{
//__ENTER_FUNCTION
//	Obj_Character *pCharacter = GetCharacter();
//	Assert( pCharacter != NULL );
//	if ( pCharacter == NULL )
//		return OR_ERROR;
//
//	if ( !CanInterruptCurrentAI(HUMAN_AI_IDLE) )
//	{
//		return OR_ERROR;
//	}
//
//	ORESULT oResult = pCharacter->Enter_Idle();
//	if ( OR_FAILED( oResult ) )
//		return oResult;
//
//	if ( HUMAN_AI_IDLE != GetHumanAI() )
//	{
//		SetHumanAI( HUMAN_AI_IDLE );
//	}
//
//	return OR_OK;
//
//__LEAVE_FUNCTION
//	return OR_ERROR;
//}
//ORESULT AI_Human::Enter_PreUseAbility( VOID )
//{
//__ENTER_FUNCTION
//
//	ORESULT res=OR_OK;
//	Obj_Human* pHuman=NULL;
//
//	pHuman = (Obj_Human* )GetCharacter();
//	if(NULL==pHuman)
//	{
//		AssertEx(FALSE,"[AI_Human::Enter_PreUseAbility]:Null human pointer found!!");
//	}
//	Obj_Character& rMe = *pHuman;
//	if( HUMAN_AI_USE_ABILITY == GetHumanAI()
//	 && !(rMe.IsCharacterLogicStopped())
//	 )
//	{
//		return OR_OK;
//	}
//
//	if ( !CanInterruptCurrentAI(HUMAN_AI_USE_ABILITY) )
//	{
//		return OR_BUSY;
//	}
//
//	res = g_pAbilityManager->GetAbility(pHuman->GetAbilityOpera()->m_AbilityID)->CanUseAbility(pHuman);
//	if( res != OR_OK )
//	{
//		return res;
//	}
//	if(FALSE==rMe.CanUseSkillNow())
//	{
//		return OR_BUSY; //����ʹ�ü�����
//	}
//
//	// if( ... )
//	//	return OR_BUSY;
//
//	res = GetCharacter()->Enter_UseAbility();
//	if ( res != OR_OK )
//	{
//		pHuman->reset_AbilityOpera();
//		return res;
//	}
//
//	SetHumanAI(HUMAN_AI_USE_ABILITY);
//	return OR_OK;
//
//__LEAVE_FUNCTION
//
//	return OR_ERROR;
//}
//
//ORESULT AI_Human::Enter_Dead( ObjID_t idKiller )
//{
//__ENTER_FUNCTION
//	Obj_Character *pCharacter = GetCharacter();
//	Assert( pCharacter != NULL );
//	if ( pCharacter == NULL )
//		return OR_ERROR;
//
//BOOL AI_Human::CanInterruptCurrentAI( ENUM_HUMAN_AI newAI )
//{
//	switch( GetHumanAI() )
//	{ // ������ǰ AI
//	case HUMAN_AI_TEAMFOLLOW:
//		{
//			switch( newAI )
//			{
//			case HUMAN_AI_DEAD: // ֻ������������
//			case HUMAN_AI_IDLE:
//				return TRUE;
//			case HUMAN_AI_MOVE:
//			case HUMAN_AI_USE_SKILL:
//			case HUMAN_AI_USE_ABILITY:
//			case HUMAN_AI_TEAMFOLLOW:
//			default:
//				return FALSE;
//			}
//		}
//		break;
//	//case :
//	//	break;
//	default:
//		return TRUE;
//	}
//}
//


//ORESULT AI_Human::Enter_Move( INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos )
//{
//__ENTER_FUNCTION
//
//	Obj_Character *pCharacter = GetCharacter();
//	Assert( pCharacter != NULL );
//	if ( pCharacter == NULL )
//		return OR_ERROR;
//
//	if ( !CanInterruptCurrentAI(HUMAN_AI_MOVE) )
//	{
//		return OR_LIMIT_MOVE;
//	}
//
//	// �Ƿ������ƶ�
//	if ( pCharacter->IsLimitMove() )
//	{
//		GCArrive	msgArrive;
//		msgArrive.setObjID( pCharacter->GetID() );
//		msgArrive.setHandleID( pCharacter->GetMoveHandleID() );
//		msgArrive.setWorldPos( *(pCharacter->getWorldPos()) );
//
//		pCharacter->getScene()->BroadCast( &msgArrive, pCharacter, TRUE ) ;
//
//		return OR_LIMIT_MOVE;
//	}
//
//	ORESULT oResult = pCharacter->Enter_Move( nHandleID, wNumTargetPos, paTargetPos );
//	if ( OR_FAILED( oResult ) )
//		return oResult;
//	//inerrupt skill execution	
//	m_paramAI_UseSkill.CleanUp();
//
