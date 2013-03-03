// Obj_Pet.cpp

#include "stdafx.h"
#include "DB_Struct.h"

#include "Scene.h"
#include "ObjManager.h"

#include "AI_Pet.h"
#include "Skill_Core.h"

#include "GCManipulatePetRet.h"
#include "GCLevelUp.h"

#include "Obj_Pet.h"
#include "Obj_Pet.inl"
#include "PetManager.h"
#include "GameTable.h"
#include "TimeManager.h"
#include "HumanPetContainer.h"
#include "ActionDelegator.h"
#include "StdImpact051.h"

#include "Impact_Core.h"

using namespace Packets;
using namespace Action_Module;
using namespace Combat_Module::Impact_Module;

_PET_ATTR_BACKUP::_PET_ATTR_BACKUP( VOID )
{
	m_bNameModified			= FALSE;
	m_bTitleModified		= FALSE;

	m_MoveSpeed				= 0.f;
	m_nOwnerID				= INVALID_ID;
	m_nAIType				= -1;
	m_Level					= -1;

	m_nDataID				= INVALID_ID;
	m_nModelID				= INVALID_ID;
	m_nMountID				= INVALID_ID;

	//m_GUID.Reset();
	m_byGeneration			= 0;
	m_byHappiness			= 0;

	m_iStrength				= 0;	
	m_iSmartness			= 0;	
	m_iMind					= 0;		
	m_iConstitution			= 0;
	

	m_nAtt_Near             	= -1;
	m_nAtt_Far              	= -1;
	
	m_nAtt_MagicNear			= -1;
	m_nAtt_MagicFar				= -1;

	m_nDef_Near             	= -1;
	m_nDef_Far              	= -1;

	m_nDef_MagicNear			= -1;
	m_nDef_MagicFar				= -1;

	m_nHit						= -1;
	m_nMiss						= -1;

	m_nExp						= -1;

	m_iStrengthPerception		= -1;
	m_iSmartnessPerception		= -1;
	m_iMindPerception			= -1;
	m_iConstitutionPerception	= -1;
	m_GenGu						= -1;
	m_nRemainPoint				= -1;
	m_iSavvy					= 0;
	m_iPullulationRate			= 0;
}

VOID _PET_ATTR_BACKUP::CleanUp( VOID )
{
	m_bNameModified				= FALSE;
	m_bTitleModified			= FALSE;
	m_MoveSpeed					= 0.f;
	m_CampData.CleanUp();
	m_nOwnerID					= INVALID_ID;
	m_SpouseGUID.Reset();
	m_nAIType					= -1;
	m_Level						= -1;

	m_nDataID					= INVALID_ID;
	m_nModelID					= INVALID_ID;
	m_nMountID					= INVALID_ID;

	m_GUID.Reset();
	m_byGeneration				= 0;
	m_byHappiness				= 0;

	m_nAtt_Near					= -1;
	m_nAtt_Far					= -1;

	m_nAtt_MagicNear			= -1;
	m_nAtt_MagicFar				= -1;

	m_nDef_Near					= -1;
	m_nDef_Far					= -1;

	m_nDef_MagicNear			= -1;
	m_nDef_MagicFar				= -1;

	m_nHit						= -1;
	m_nMiss						= -1;

	m_nExp						= -1;

	m_iStrengthPerception		= -1;
	m_iSmartnessPerception		= -1;
	m_iMindPerception			= -1;
	m_iConstitutionPerception	= -1;
	m_GenGu						= -1;
	m_iSavvy					= 0;
	m_iPullulationRate			= 0;

	m_iStrength					= 0;	
	m_iSmartness				= 0;	
	m_iMind						= 0;		
	m_iConstitution				= 0;

	m_nRemainPoint				= -1;
}

Obj_Pet::Obj_Pet( VOID )
{
__ENTER_FUNCTION

	INT i;

	// 消息/属性刷新
	//m_AttrBackUp.CleanUp();

	// 快乐度变化规则
	//m_HappinessTimer.CleanUp();

	// 寿命变化规则
	m_LifeTimer.CleanUp();

	// 效果
	//m_ImpactList.CleanUp();

	// Capturer
	m_nCapturerCount	= 0;
	for ( i = 0; i < MAX_CAPTURER_NUM; i++ )
	{
		m_aCapturer[i]	= INVALID_ID;
	}
	// aCacheOfSkill
	for (i = 0; i < HUMAN_PET_MAX_COUNT; ++i)
	{
		m_aCacheOfSkill[i] = INVALID_ID;
	}

	// AI
	m_OwnerID			= INVALID_ID;
	m_TargetID			= INVALID_ID;
	m_RespawnPos		= WORLD_POS( -1.f, -1.f );
	m_RespawnTime		= 0;

	// 一级属性
	m_GUID				= INVALID_GUID;
	m_PetDB.CleanUp();
	m_CreatorID			= INVALID_ID;
	m_pCreator			= NULL;
	m_nIndex			= -1;

	m_pAI_Character		= new AI_Pet;
	Assert( m_pAI_Character ); 

__LEAVE_FUNCTION
}

Obj_Pet::~Obj_Pet( VOID )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

VOID Obj_Pet::CleanUp( )
{
__ENTER_FUNCTION

	INT i;

	// 消息/属性刷新
	m_AttrBackUp.CleanUp();

	// 快乐度变化规则
	m_HappinessTimer.CleanUp();

	// 寿命变化规则
	m_LifeTimer.CleanUp();

	// 效果
	m_ImpactList.CleanUp();

	// Capturer
	m_nCapturerCount	= 0;
	for ( i = 0; i < MAX_CAPTURER_NUM; i++ )
	{
		m_aCapturer[i]	= INVALID_ID;
	}

	// aCacheOfSkill
	for (i = 0; i < HUMAN_PET_MAX_COUNT; ++i)
	{
		m_aCacheOfSkill[i] = INVALID_ID;
	}
	// AI
	m_OwnerID			= INVALID_ID;
	m_TargetID			= INVALID_ID;
	m_RespawnPos		= WORLD_POS( -1.f, -1.f );
	m_RespawnTime		= 0;

	// 一级属性
	m_GUID				= INVALID_GUID;
	m_CreatorID			= INVALID_ID;
	m_pCreator			= NULL;
	m_PetDB.CleanUp();
	m_nIndex			= -1;

	//m_pAI_Character

	Obj_Character::CleanUp();

__LEAVE_FUNCTION
}

BOOL Obj_Pet::Init( const _OBJ_INIT *pInit )
{
__ENTER_FUNCTION

	BOOL bResult = Obj_Character::Init( pInit );
	if ( !bResult )
		return FALSE;

	const _OBJ_PET_INIT *pPetInit  = NULL;
	pPetInit = (const _OBJ_PET_INIT*)pInit;
	if ( pPetInit == NULL )
	{
		Assert( FALSE &&  "Error Obj_Pet::Init, pPetInit = NULL" );
		return FALSE;
	}

	m_GUID				= pPetInit->m_GUID;
	m_CreatorID			= pPetInit->m_CreatorID;
	m_pCreator			= NULL;
	m_nIndex			= pPetInit->m_nIndex;
	m_iLevel			= pPetInit->m_PetDB.m_nLevel;
	if ( m_CreatorID != INVALID_ID )
	{
		Obj_Character *pCreator	= (Obj_Character*)(getScene()->GetObjManager()->GetObj(m_CreatorID) );
		if (pCreator != NULL && pCreator->GetObjType() == OBJ_TYPE_HUMAN)
			m_pCreator = (Obj_Human*)pCreator;
	}

	if ( m_pCreator == NULL )
	{
		m_PetDB = pPetInit->m_PetDB;
		return FALSE;
	}

	m_PetDB.m_nDataID	= pPetInit->m_PetDB.m_nDataID;
	m_RespawnTime		= pPetInit->m_RespawnTime;

	m_RespawnPos		= pPetInit->m_Pos;
	m_OwnerID			= pPetInit->m_CreatorID;

	m_HappinessTimer.BeginTimer( g_Config.m_ConfigInfo.m_PetHappinessInterval, g_pTimeManager->CurrentTime() );
	SetDataID( m_pCreator->GetMonsterPetDataID() );

	MONSTER_EXT_ATTR *pPetAttr = g_MonsterAttrExTbl.GetExtAttr( GetDataID() );
	if(pPetAttr)
	{
		m_iLevel = pPetAttr->m_Level;
	}
	else
	{
		m_iLevel = 0;
	}

	PetManager* pPetManager = getScene()->GetPetManager();
	if(pPetManager)
	{
		m_oInit = pPetManager->GetInitData();
	}
		
	//初始化技能相关内容
	InitLife();


	SetHP( GetMaxHP() );

	// 使AI进入IDLE状态
	GetAIObj()->ChangeState(ESTATE_IDLE);

    InitBackupAttr();
	//附加效果数据
	Impact_InitList();

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

VOID Obj_Pet::InitAIObj( )
{
__ENTER_FUNCTION
	Assert( m_pAI_Character );
	m_pAI_Character->Init( this );
__LEAVE_FUNCTION
}

BOOL Obj_Pet::HeartBeat( UINT uTime/* = 0*/ )
{
__ENTER_FUNCTION

	BOOL bResult = Obj_Character::HeartBeat( uTime );
	if ( !bResult )
		return FALSE;

	HeartBeat_Happiness( uTime );
	HeartBeat_Life(uTime);
	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Obj_Pet::HeartBeat_Happiness( UINT uTime/* = 0*/ )
{
__ENTER_FUNCTION

	if ( m_HappinessTimer.CountingTimer( uTime ) )
	{
		if ( GetHappiness() > 0 )
		{
			SetHappiness( GetHappiness() - 1 );
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Obj_Pet::HeartBeat_Life( UINT uTime/* = 0*/ )
{
	__ENTER_FUNCTION

	if( GetHP()<=0 )
	{
		RecallPet();
	}

	if( -1 == m_oInit.iLifeTime )
	{
		return TRUE;
	}

	if ( m_LifeTimer.CountingTimer( uTime ) )
	{
		RecallPet();
	}

	return TRUE;

	__LEAVE_FUNCTION
	SetActiveFlag(FALSE);
	return FALSE;
}

BOOL Obj_Pet::IsEnemy(Obj_Character* pCharacter )
{
	__ENTER_FUNCTION

	Obj_Character *pOwner = GetOwner();
	if(GetID()==pCharacter->GetID())
	{
		return FALSE;
	}
	if(pOwner != NULL)
	{
		if(pOwner->GetID()==pCharacter->GetID())
		{
			return FALSE;
		}
		return pOwner->IsEnemy(pCharacter);
	}
	else
	{
		SOT_ShadowGuard_T logic;
		OWN_IMPACT* pImp = Impact_GetFirstImpactOfSpecificLogicID(logic.ID);
		if(NULL!=pImp)
		{
			Obj* pObj = (Obj*)( (GetSpecificObjInSameSceneByID(logic.GetSummonerID(*pImp))) );
			if(NULL!=pObj)
			{
				if(TRUE == IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rSummoner = *static_cast<Obj_Character*>(pObj);
					return rSummoner.IsEnemy(pCharacter);
				}
			}
		}
		Assert( pCharacter );
		BOOL bIsEnemy = IsEnemyCamp(*pCharacter);
		return bIsEnemy;
	}
	__LEAVE_FUNCTION

	return FALSE;
}
BOOL Obj_Pet::IsFriend(Obj_Character* pCharacter )
{
__ENTER_FUNCTION
	
	Obj_Character *pOwner = GetOwner();
	if(pOwner != NULL)
	{
		return pOwner->IsFriend(pCharacter);
	}
	else
	{
		Assert( pCharacter );
		return FALSE==IsEnemy(pCharacter);
	}
__LEAVE_FUNCTION

	return FALSE;
}
BOOL Obj_Pet::IsPartner(Obj_Character* pCharacter )
{
__ENTER_FUNCTION
	if(NULL==pCharacter)
	{
		return FALSE;
	}
	if(GetID()==pCharacter->GetID())
	{
		return TRUE;
	}
	Obj_Character *pOwner = GetOwner();
	if(pOwner != NULL)
	{
		return pOwner->IsPartner(pCharacter);
	}
	else
	{
		return FALSE;
	}
	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

Obj_Character *Obj_Pet::GetOwner( VOID )
{
	Obj_Character *pCharacter = NULL;
	ObjID_t idOwner = GetOwnerID();
	if ( idOwner != INVALID_ID && getScene() != NULL )
	{
		pCharacter = (Obj_Character*)(getScene()->GetObjManager()->GetObj( idOwner ));
	}

	return pCharacter;
}

VOID Obj_Pet::ToAttack( ObjID_t TargetID )
{
__ENTER_FUNCTION

	if (INVALID_ID != TargetID && m_TargetID == TargetID) {
		return ;
	}

	m_TargetID = TargetID;
	
	AI_Pet* pAIPet = (AI_Pet*)GetAIObj();
	if (pAIPet)
	{
		pAIPet->BabyToAttack();
	}

__LEAVE_FUNCTION
}

BOOL Obj_Pet::PushSkillToCache(INT idSkill)
{
__ENTER_FUNCTION
	INT i;
	for ( i = 0; i < HUMAN_PET_MAX_COUNT; ++i)
	{
		if (INVALID_ID == m_aCacheOfSkill[i])
		{
			m_aCacheOfSkill[i] = idSkill;
			break;
		}
	}

	if (HUMAN_PET_MAX_COUNT == i)
	{
		return FALSE;
	}
	return TRUE;

__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Pet::GetRateOfBuffTakeEffect(INT idSkill, INT& rateOfBuffTakeEffect)
{
__ENTER_FUNCTION
	const SkillTemplateData_T* pSkillData_T = g_SkillTemplateDataMgr.GetInstanceByID(idSkill);
	if (!pSkillData_T)
	{
		Assert(NULL && "Obj_Pet::GetRateOfBuffTakeEffect...pSkillData_T=NULL...");
		return ;
	}
	ID_t TypeOfSkill = pSkillData_T->GetTypeOfPetSkill();
	Obj_Human* pCreator = GetCreator();
	if (pCreator) {
		ItemContainer* pItemContainer = pCreator->GetPetContain();
		Assert(pItemContainer);
		Item* pItem = pItemContainer->GetItem(m_nIndex);
		Assert(pItem);
		
		INT nAIType = pItem->GetAIType();

		if (nAIType < 0 || nAIType > PET_AITYPE)
			return ;
		if (TypeOfSkill < 0 || TypeOfSkill > PET_SKILL_TYPE)
			return ;
		FLOAT fLaunchRate = g_PetAIStrategyTble[TypeOfSkill][nAIType];
		rateOfBuffTakeEffect = (INT)(rateOfBuffTakeEffect * fLaunchRate);
	}

__LEAVE_FUNCTION
}

VOID Obj_Pet::LevelUp(VOID)
{
	__ENTER_FUNCTION
	if (INVALID_ID == m_OwnerID)	
	{
		return ;
	}

	Obj* pOwner = getScene()->GetObjManager()->GetObj(m_CreatorID);
	if (!pOwner || pOwner->GetObjType() != Obj::OBJ_TYPE_HUMAN)
	{
		return ;
	}

	ItemContainer* pPetContainer = ((Obj_Human*)pOwner)->GetPetContain();
	Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	Assert(pPetItem);

	INT nLevel = GetLevel() + 1;
	if (nLevel == PET_LEVEL_NUM)
	{
		return ;
	}
	SetLevel(nLevel);

	// 设置宠物相关属性

	// 所有一级属性加1
	//人物的基础点只保存手工分点，宠物没有查看对应表格。所以将下面代码注释。确认表格后，应该删除下面的代码 
	//10/03/26 chenjun begin
	//SetBaseStr(GetBaseStr() + 1);
	////SetBaseSpr(GetBaseSpr() + 1);
	//SetBaseCon(GetBaseCon() + 1);
	//SetBaseInt(GetBaseInt() + 1);
	//SetBaseDex(GetBaseDex() + 1);
	//10/03/26 end

	SetRemainPoint(pPetItem->GetRemainPoint()+5);

	// 一定要先设置MaxHP:)
	SetHP(GetMaxHP());
	SetHappiness( 100 );

	// 调用宠物的领悟技能函数
	Skill_Apperceive();

	GCLevelUp	msgLevelUp;
	msgLevelUp.setObjId(GetID());
	msgLevelUp.setLevel(GetLevel());

	getScene()->BroadCast(&msgLevelUp, this, TRUE);

	__LEAVE_FUNCTION
}

VOID Obj_Pet::SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo )
{
	__ENTER_FUNCTION
	if ( pReliveInfo == NULL )
	{
		Assert( pReliveInfo != NULL && "Obj_Pet::SetReliveInfo" );
		return;
	}

	Obj_Character::SetReliveInfo(bSkillRelive, pReliveInfo);
	
	__LEAVE_FUNCTION
}

BOOL Obj_Pet::AddCapturer(ObjID_t idObj)
{
	__ENTER_FUNCTION
	INT i;
	for (i = 0; i < MAX_CAPTURER_NUM; ++i)
	{
		if (m_aCapturer[i] == INVALID_ID)
		{
			m_aCapturer[i] = idObj;
			++m_nCapturerCount;
			return TRUE;
		}
	}
	
	if (i == MAX_CAPTURER_NUM)
		return FALSE;

	return TRUE;
	__LEAVE_FUNCTION
	return TRUE;
}

BOOL Obj_Pet::DelCapturer(ObjID_t idObj)
{
	__ENTER_FUNCTION
	INT i;
	for (i = 0; i < MAX_CAPTURER_NUM; ++i)
	{
		if (m_aCapturer[i] == idObj)
		{
			m_aCapturer[i] = INVALID_ID;
			--m_nCapturerCount;
			if (m_nCapturerCount < 0)
				m_nCapturerCount = 0;
			return TRUE;
		}
	}
	
	if (i == MAX_CAPTURER_NUM)
		return FALSE;

	return TRUE;
	__LEAVE_FUNCTION
	return TRUE;

}

VOID Obj_Pet::SendCaptureFailedToOthers(ObjID_t idObj)
{
	__ENTER_FUNCTION
	GCManipulatePetRet msg;
	for (INT i = 0; i < MAX_CAPTURER_NUM; ++i)
	{
		if (INVALID_ID != m_aCapturer[i] && m_aCapturer[i] != idObj)
		{
			Scene* pScene = getScene();
			if (!pScene)
			{
				m_nCapturerCount = 0;
				return ;
			}
			ObjManager* pObjMgr = pScene->GetObjManager();
			if (!pObjMgr)
				return ;
			Obj* pObj = pObjMgr->GetObj(m_aCapturer[i]);
			if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN)
			{
				Obj_Human* pHuman = (Obj_Human*)pObj;
				if (pHuman)
				{
					GetGlobalActionDelegator().InterruptCurrentAction(*pHuman);
					msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CAPTUREFALID);

					AI_Character* pAIHuman = pHuman->GetAIObj();
					if (pAIHuman)
					{
						pAIHuman->Stop();
					}

					pHuman->GetPlayer()->SendPacket(&msg);
				}
			}
		}
	}
	__LEAVE_FUNCTION
}

BOOL Obj_Pet::InitSkill()
{
	__ENTER_FUNCTION

	Scene* pScene = getScene();
	if( NULL == pScene )
	{
		return FALSE;
	}

	PetManager* pPetManager = pScene->GetPetManager();
	if( NULL == pPetManager )
	{
		return FALSE;
	}
	
	for (INT i=0; i<MONSTER_PET_MAX_SKILL; ++i)
	{
		PushSkillToCache(m_oInit.aSkillID[i]);
	}
	if( INVALID_ID != m_oInit.iImpactToSelf )
	{
		g_ImpactCore.SendImpactToUnit( *this, m_oInit.iImpactToSelf, GetID() );
	}

	if( INVALID_ID != m_oInit.iImpactToOnwer )
	{
		g_ImpactCore.SendImpactToUnit( *m_pCreator, m_oInit.iImpactToOnwer, GetID() );
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Pet::InitLife()
{
	__ENTER_FUNCTION

	if( -1 != m_oInit.iLifeTime )
	{
		m_LifeTimer.BeginTimer( m_oInit.iLifeTime, g_pTimeManager->CurrentTime() );
	}
	__LEAVE_FUNCTION
}

VOID Obj_Pet::RecallPet()
{
	__ENTER_FUNCTION

	Obj_Human* pOwner = (Obj_Human*)GetOwner();
	if( NULL != pOwner )
	{
		pOwner->ReCallPet( m_nIndex	);
	}
	__LEAVE_FUNCTION
}

INT Obj_Pet::GetIndexInOnwer()
{
	return m_nIndex;
}

VOID Obj_Pet::InitMonsterPetDB( _Monster_Pet_DB_Struct& oMonsterPet )
{
	__ENTER_FUNCTION
	//用总生命周期，减去已经历的时间
	oMonsterPet.iLifeTime = GetLifeTime();

	GetNowImpactList(oMonsterPet);
	oMonsterPet.m_iDataIndex	= GetDataID();
	oMonsterPet.m_iHP			= GetHP();
	memcpy( oMonsterPet.m_aSkillList, m_aCacheOfSkill, sizeof( oMonsterPet.m_aSkillList ) );
	__LEAVE_FUNCTION
}

INT Obj_Pet::GetLifeTime()
{
	__ENTER_FUNCTION

	INT iLife = INVALID_ID;
	if( m_oInit.iLifeTime > 0 )
	{
		iLife = m_oInit.iLifeTime - (g_pTimeManager->CurrentTime() - m_LifeTimer.GetTickOldTime());
		if( iLife < 0 )
		{
			iLife = 0;
		}
	}
	return iLife;
	__LEAVE_FUNCTION
	return 0;
}

VOID Obj_Pet::GetNowImpactList( _Monster_Pet_DB_Struct &oMonsterPet )
{
	for( INT i=0; i<m_ImpactList.m_Count; ++i )
	{
		if( i >= MONSTER_PET_MAX_IMPACT )
		{
			break;
		}
		oMonsterPet.m_aImpactList[i] = m_ImpactList.m_aImpacts[i];
	}
}