// Obj_Character.cpp

#include "stdafx.h"
#include "Scene.h"

#include "TimeManager.h"
#include "AI_Character.h"
#include "Obj_Character.h"
#include "Combat_Core.h"
#include "StdImpact051.h"
#include "Obj_Pet.h"


#define CHECK_SENDMSG_TIME	(5000)
#define ATTR_REFESH_TIME	(500)

#define REFESH_LEVEL_LIMIT	(10)
using namespace Combat_Module::Skill_Module;

using namespace Combat_Module::Impact_Module;

ENUM_REFESH_ATTR_TYPE CalcRefeshAttrType(Obj_Character *pSour, Obj_Character *pTarget )
{
__ENTER_FUNCTION

	BOOL bEnemy = pSour->IsEnemy(pTarget );
	if ( bEnemy )
	{
		BOOL bRefeshLevel = ((pSour->GetLevel() - pTarget->GetLevel()) < REFESH_LEVEL_LIMIT)?(TRUE):(FALSE);
		if ( bRefeshLevel )
		{
			return REFESH_ATTR_TYPE_PUBLIC;
		}
		else
		{
			return REFESH_ATTR_TYPE_PUBLIC_AND_LEVEL;
		}
	}
	else
	{
		return REFESH_ATTR_TYPE_PROTECTED;
	}

__LEAVE_FUNCTION

	return REFESH_ATTR_TYPE_INVALID ;
}

Obj_Character::Obj_Character( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

Obj_Character::~Obj_Character( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pAI_Character );

	CleanUp( ) ;
		
__LEAVE_FUNCTION
}
BOOL Obj_Character::Init( const _OBJ_INIT *pInit )
{
	__ENTER_FUNCTION

	if ( !Obj_Dynamic::Init( pInit ) )
		return FALSE;

	UINT	CreateTime		= g_pTimeManager->CurrentTime();
	m_SendMsgTimer.BeginTimer( CHECK_SENDMSG_TIME, CreateTime ) ;

	m_AttrMsgTimer.BeginTimer( ATTR_REFESH_TIME, CreateTime ) ;
	m_AttrMsgToTeamIntervalCount	= 0;
	m_PublicAttrVersion				= 0;
	m_ProtectedAttrVersion			= 0;
	MarkAllIntAttrDirtyFlags();
	MarkAllIntAttrRefixDirtyFlags();
	MarkAllBoolAttrDirtyFlags();
	InitAIObj( );
	m_nAutoRepeatCooldown=0;

	//m_paramLogic_Move.m_nHandleID = 0;

	m_nView_PrevLevel			= GetLevel();
	m_nView_PrevStealthLevel	= GetStealthLevel();
	m_nView_PrevDetectLevel		= GetDetectLevel();
	m_UpdateViewCharacterTimer.BeginTimer( 1000, g_pTimeManager->CurrentTime());

	ResetReliveInfo();
	return TRUE;

	__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Character::InitAIObj( )
{
	//pAI_Character = new Obj_Character;
	//Assert( pAI_Character );
	//pAI_Character->Init( this );
}

VOID Obj_Character::CleanUp( VOID )
{
	__ENTER_FUNCTION

	m_SendMsgTimer.CleanUp();
	m_bSendMsgDirty	= FALSE;

	m_AttrMsgTimer.CleanUp();
	m_AttrMsgToTeamIntervalCount	= 0;
	m_PublicAttrVersion				= 0;
	m_ProtectedAttrVersion			= 0;

	m_CharacterID = INVALID_ID ;

	m_RecoverTimer.CleanUp( ) ;

	SetCharacterLogic( CHARACTER_LOGIC_INVALID );
	m_nActionTime = 0;
	m_nLockedTargetID = -1;
	m_nElapsedRevocerTick = 0;
	//m_nElapsedRageRevocerTick = 0;
	m_nElapsedPneumaRevocerTick = 0;
	m_eMoveMode = MOVE_MODE_WALK;
	m_pActionLogic = NULL;
	m_ActionParams.Reset();

	m_nView_PrevLevel			= -1;
	m_nView_PrevStealthLevel	= -1;
	m_nView_PrevDetectLevel		= -1;
	m_UpdateViewCharacterTimer.CleanUp();

	m_nMoodState				= INVALID_MOOD_STATE;
	m_pGuard					= NULL;
	ResetReliveInfo();
	Obj_Dynamic::CleanUp( ) ;
	m_pVisitor = NULL;
	for( INT i=0; i<SkillTemplateData_T::MAX_SUB_SKILL; ++i )
	{
		m_aSubSkill[i] = -1;
	}
	memset( m_DelayTimeSkillMissList, -1, sizeof(m_DelayTimeSkillMissList) );
	__LEAVE_FUNCTION
}
VOID Obj_Character::OnEnterScene(VOID)
{
	__ENTER_FUNCTION

	Obj::OnEnterScene();
	//附加效果数据
	Impact_InitList();
	__LEAVE_FUNCTION
}

BOOL Obj_Character::HeartBeat( UINT uTime )
{
	__ENTER_FUNCTION
	if(FALSE==Obj_Dynamic::HeartBeat(uTime))
	{
		return FALSE;
	}
	//更新区域
	UpdateZone();
	HeartBeat_Action(uTime); //elapsing action time
	HeartBeat_AutoRepeatCooldown(uTime); //elapsing Auto-Repeat cooldown
	if(TRUE==IsActiveObj())
	{
		//恢复
		HeartBeat_Recover(uTime);
	}
	//处理角色Logic
	if ( !IsCharacterLogicStopped() )
	{
		BOOL bResult = FALSE;
		//获取角色Logic动作
		switch ( GetCharacterLogic() )
		{
		case CHARACTER_LOGIC_IDLE:
			Obj_Logic_Idle(uTime );
			break;
		case CHARACTER_LOGIC_MOVE:
			bResult = Obj_Logic_Move(uTime );
			break;
		case CHARACTER_LOGIC_USE_SKILL:
			bResult = Obj_Logic_UseSkill(uTime);
			break;
		case CHARACTER_LOGIC_USE_ABILITY:
			bResult = Obj_Logic_UseAbility(uTime );
			break;
		default:
			break;
		}

		if(!bResult)
		{
			StopCharacterLogic(TRUE);
		}
	}
	Impact_HeartBeat();

	HeartBeat_Cooldown(uTime);

	m_pAI_Character->Logic(uTime);

	if(IsActiveObj())
	{
		if ( m_AttrMsgTimer.CountingTimer(uTime) )
		{
			SendMsg_RefeshAttrib( );
			m_AttrMsgToTeamIntervalCount++;

			if ( m_AttrMsgToTeamIntervalCount > 9 )
			{
				m_AttrMsgToTeamIntervalCount = 0;
				SyncTeamMemberInfo();
			}
		}
	}

	//延时技能生效处理
	ImpactDelayTimeHeartBeat();

	//子技能发放
	SubSkillAutoUseHeartBeat();
	//延时的未命中消息
	DelayTimeSkillMissHeartBeat();

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

// 未被击活的Obj所执行的逻辑
BOOL Obj_Character::HeartBeat_OutZone( UINT uTime )
{
	__ENTER_FUNCTION
	return Obj_Dynamic::HeartBeat_OutZone(uTime);
	__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Character::OnRegisterToZone(VOID)
{
__ENTER_FUNCTION

	Obj::OnRegisterToZone();

	SetCharacterLogic( CHARACTER_LOGIC_IDLE );

__LEAVE_FUNCTION
}

VOID Obj_Character::SendMsg_RefeshAttrib( VOID )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID Obj_Character::AskMsg_BaseAttrib( Obj_Human *pTargetHuman )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

///////////////////////////////////////////////////////////
// 复活点相关
///////////////////////////////////////////////////////////
BOOL Obj_Character::IsCanRelive( VOID )const
{
	return m_bCanSkillRelive;
}

const Obj_Character::_RELIVE_INFO *Obj_Character::GetReliveInfo( BOOL bSkillRelive )const
{
	if ( bSkillRelive && m_bCanSkillRelive )
	{
		return &m_SkillReliveInfo;
	}
	return NULL;
}

// 重置复活的信息
VOID Obj_Character::ResetReliveInfo( VOID )
{
	__ENTER_FUNCTION
	m_bCanSkillRelive = FALSE;
	m_SkillReliveInfo.Reset();
	__LEAVE_FUNCTION
}

//获取最近可掉落的点
BOOL Obj_Character::GetNearCanDropPos(WORLD_POS& pos)
{
	__ENTER_FUNCTION
	return FALSE;
	__LEAVE_FUNCTION
}

BOOL Obj_Character::HeartBeat_Action(UINT uTime)
{
__ENTER_FUNCTION
	Time_t m_nDeltaTime = GetLogicTime();
	//动作时间
	if(0<m_nActionTime)
	{
		m_nActionTime -= m_nDeltaTime;
		0>m_nActionTime?m_nActionTime=0:NULL;
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL Obj_Character::HeartBeat_AutoRepeatCooldown(UINT uTime)
{
__ENTER_FUNCTION
	Time_t m_nDeltaTime = GetLogicTime();
	//自动重复技能的冷却时间
	if(0<m_nAutoRepeatCooldown)
	{
		m_nAutoRepeatCooldown -= m_nDeltaTime;
		if( m_nAutoRepeatCooldown < 0 )
			m_nAutoRepeatCooldown = 0;
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Character::SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo )
{
	if ( pReliveInfo == NULL )
	{
		Assert( pReliveInfo != NULL && "Obj_Character::SetReliveInfo" );
		return;
	}

	m_bCanSkillRelive = bSkillRelive;
	if ( bSkillRelive )
	{
		m_SkillReliveInfo = *pReliveInfo;
	}
}
Obj* Obj_Character::GetMyShadowGuard(VOID)
{
	__ENTER_FUNCTION

	SOT_ShadowGuard_T logic;
	OWN_IMPACT* pImp = Impact_GetFirstImpactOfSpecificLogicID(logic.ID);
	if(NULL==pImp)
	{
		return NULL;
	}
	return (Obj*)( (GetSpecificObjInSameSceneByID(logic.GetShadowGuardID(*pImp))) );

	__LEAVE_FUNCTION
	return NULL;
}
Obj* Obj_Character::GetMyPet(INT iIndex)
{
	__ENTER_FUNCTION
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		Obj_Human* pHuman = (Obj_Human*)(this);
		return (Obj*)(pHuman->GetPet(iIndex));
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}
Obj* Obj_Character::GetMyMaster(VOID)
{
	__ENTER_FUNCTION
	if(Obj::OBJ_TYPE_PET==GetObjType())
	{
		Obj_Pet* pPet = (Obj_Pet*)(this);
		return (Obj*)(pPet->GetCreator());
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

VOID Obj_Character::UpdateViewCharacter(VOID)
{
	// 隐身数据改变
	if(m_nView_PrevStealthLevel != GetStealthLevel() && getZoneID() != INVALID_ID)
	{
		HUMANLIST listHuman;
		getScene()->ScanHuman( getZoneID(), MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );

		Packet *pNewObjPacket		= CreateNewObjPacket();
		Packet *pDeleteObjPacket	= CreateDeleteObjPacket();

		BYTE i;
		//BOOL bPrevCanViewMe;
		BOOL bCurrentCanViewMe;
		Obj_Human *pHuman;
		for ( i = 0; i < listHuman.m_Count; i++ )
		{
			pHuman				= listHuman.m_aHuman[i];
			//bPrevCanViewMe		= IsPrevCanViewMe(pHuman);
			bCurrentCanViewMe	= IsCanViewMe(pHuman);
			if(/*bPrevCanViewMe && */!bCurrentCanViewMe)
			{
				pHuman->GetPlayer()->SendPacket(pDeleteObjPacket);
			}
			else/* if(!bPrevCanViewMe && bCurrentCanViewMe)*/
			{
				pHuman->GetPlayer()->SendPacket(pNewObjPacket);
			}
		}
		DestroyNewObjPacket( pNewObjPacket );
		DestroyDeleteObjPacket( pDeleteObjPacket );

		// 放在最后
		m_nView_PrevStealthLevel = GetStealthLevel();
	}
}
BOOL Obj_Character::DepletingUsedItem(VOID)
{
	__ENTER_FUNCTION
	if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	{
		Obj_Human* pHuman = (Obj_Human*)(this);
		return pHuman->Skill_DepleteItem();
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Character::Accept( CharacterVisitor* pVisitor )
{
	__ENTER_FUNCTION
	m_pVisitor = pVisitor;
	__LEAVE_FUNCTION
}

INT Obj_Character::GetHorseDataIndex( INT iIndex )
{
	__ENTER_FUNCTION
	if( iIndex<0 || iIndex>=MAX_HORSE_DATA_INDEX )
	{
		return INVALID_ID;
	}
	return aHorseDataIndex[iIndex];
	__LEAVE_FUNCTION
	return INVALID_ID;
}

VOID Obj_Character::SetHorseDataIndex( INT iIndex, INT iVal )
{
	__ENTER_FUNCTION
	if( iIndex<0 || iIndex>=MAX_HORSE_DATA_INDEX )
	{
		return;
	}
	aHorseDataIndex[iIndex] = iVal;
	__LEAVE_FUNCTION
}

VOID Obj_Character::CleanupHorseBuff()
{
	__ENTER_FUNCTION

	for( INT i=0; i<6; ++i )
	{
		INT iDataIndex = GetHorseDataIndex( i );
		if( iDataIndex == INVALID_ID )
		{
			continue;
		}
		Impact_DispelSpecificImpactByDataIndex( iDataIndex, 1 );
		SetHorseDataIndex( i, INVALID_ID );
	}
	__LEAVE_FUNCTION

}

