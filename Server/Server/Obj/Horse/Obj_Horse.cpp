/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   10:45
	filename: 	MapServer\Server\Obj\Horse\Obj_Horse.cpp
	file path:	MapServer\Server\Obj\Horse
	file base:	Obj_Horse
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "DB_Struct.h"

#include "Scene.h"
#include "ObjManager.h"

#include "AI_Horse.h"
#include "Skill_Core.h"

#include "GCManipulatePetRet.h"
#include "GCLevelUp.h"

#include "GameTable.h"
#include "TimeManager.h"
#include "HumanPetContainer.h"
#include "ActionDelegator.h"
#include "StdImpact051.h"
#include "Obj_Horse.h"
#include "HorseManager.h"

#include "GCCharBaseAttrib.h"
#include "GCDetailAttrib_Pet.h"
#include "GCNewPet.h"
#include "GCNewPet_Move.h"
#include "GCNewPet_Death.h"

using namespace Packets;
using namespace Action_Module;
using namespace Combat_Module::Impact_Module;

Obj_Horse::Obj_Horse( VOID )
{
__ENTER_FUNCTION

	INT i;

	// Capturer
	m_nCapturerCount	= 0;
	for ( i = 0; i < MAX_CAPTURER_NUM; i++ )
	{
		m_aCapturer[i]	= INVALID_ID;
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

	m_pAI_Character		= new AI_Horse;
	m_LifeTimer.CleanUp();
	Assert( m_pAI_Character ); 

__LEAVE_FUNCTION
}

Obj_Horse::~Obj_Horse( VOID )
{
	__ENTER_FUNCTION



	__LEAVE_FUNCTION
}

VOID Obj_Horse::CleanUp( )
{
__ENTER_FUNCTION

	INT i;

	// 消息/属性刷新
	m_AttrBackUp.CleanUp();

	// Capturer
	m_nCapturerCount	= 0;
	for ( i = 0; i < MAX_CAPTURER_NUM; i++ )
	{
		m_aCapturer[i]	= INVALID_ID;
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
	
	m_ImpactList.CleanUp();
	m_LifeTimer.CleanUp();
	//m_pAI_Character

	Obj_Character::CleanUp();

__LEAVE_FUNCTION
}

BOOL Obj_Horse::Init( const _OBJ_INIT *pInit )
{
__ENTER_FUNCTION

	BOOL bResult = Obj_Character::Init( pInit );
	if ( !bResult )
		return FALSE;

	const _OBJ_HORSE_INIT *pPetInit  = NULL;
	pPetInit = (const _OBJ_HORSE_INIT*)pInit;
	if ( pPetInit == NULL )
	{
		Assert( FALSE &&  "Error Obj_Horse::Init, pPetInit = NULL" );
		return FALSE;
	}

	m_GUID				= pPetInit->m_GUID;
	m_CreatorID			= pPetInit->m_CreatorID;
	m_pCreator			= NULL;
	m_nIndex			= pPetInit->m_nIndex;
	if ( m_CreatorID != INVALID_ID )
	{
		Obj_Character *pCreator	= (Obj_Character*)(getScene()->GetObjManager()->GetObj(m_CreatorID) );
		if (pCreator != NULL && pCreator->GetObjType() == OBJ_TYPE_HUMAN)
			m_pCreator = (Obj_Human*)pCreator;
	}

	if ( m_pCreator == NULL )
		m_PetDB = pPetInit->m_PetDB;

	m_PetDB.m_nDataID	= pPetInit->m_PetDB.m_nDataID;
	m_RespawnTime		= pPetInit->m_RespawnTime;

	m_RespawnPos		= pPetInit->m_Pos;
	m_OwnerID			= pPetInit->m_CreatorID;

	_HORSE_ATTR *pPetAttr = NULL;
	//MONSTER_EXT_ATTR *pMonsterAttr = NULL;
	if ( m_PetDB.m_nDataID != INVALID_ID )
	{
		pPetAttr = g_HorseAttrTbl.GetAttr( m_PetDB.m_nDataID );
	}
	m_LifeTimer.BeginTimer( pPetAttr->m_LifeNoMaster, g_pTimeManager->CurrentTime() );


	getScene()->GetHorseManager()->MakeCapturePetAttrib(&m_PetDB);

	GetAIObj()->ChangeState(ESTATE_IDLE);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID Obj_Horse::InitAIObj( )
{
__ENTER_FUNCTION
	Assert( m_pAI_Character );
	m_pAI_Character->Init( this );
__LEAVE_FUNCTION
}

BOOL Obj_Horse::HeartBeat( UINT uTime/* = 0*/ )
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

BOOL Obj_Horse::HeartBeat_Happiness( UINT uTime/* = 0*/ )
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Obj_Horse::HeartBeat_Life( UINT uTime/* = 0*/ )
{
__ENTER_FUNCTION

	if( m_LifeTimer.CountingTimer( uTime ) )
	{
		SetActiveFlag( FALSE );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Obj_Horse::IsEnemy(Obj_Character* pCharacter )
{
	__ENTER_FUNCTION

	return FALSE;
	__LEAVE_FUNCTION

	return FALSE;
}
BOOL Obj_Horse::IsFriend(Obj_Character* pCharacter )
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
BOOL Obj_Horse::IsPartner(Obj_Character* pCharacter )
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

BOOL Obj_Horse::PushSkillToCache(INT idSkill)
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Horse::GetRateOfBuffTakeEffect(INT idSkill, INT& rateOfBuffTakeEffect)
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

VOID Obj_Horse::LevelUp(VOID)
{
	__ENTER_FUNCTION

	__LEAVE_FUNCTION
}

VOID Obj_Horse::SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo )
{
	__ENTER_FUNCTION
	if ( pReliveInfo == NULL )
	{
		Assert( pReliveInfo != NULL && "Obj_Horse::SetReliveInfo" );
		return;
	}

	Obj_Character::SetReliveInfo(bSkillRelive, pReliveInfo);
	
	__LEAVE_FUNCTION
}

BOOL Obj_Horse::AddCapturer(ObjID_t idObj)
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

BOOL Obj_Horse::DelCapturer(ObjID_t idObj)
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

VOID Obj_Horse::SendCaptureFailedToOthers(ObjID_t idObj)
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

INT Obj_Horse::GetModelID( VOID )
{
	INT iDataID = GetDataID();
	_HORSE_ATTR* pPetAttr = g_HorseAttrTbl.GetAttr(iDataID);
	if (!pPetAttr)
		return INVALID_ID;

	return g_HorseAttrTbl.m_aHorseTbl[iDataID].m_iBehaveID;
}

Packet *Obj_Horse::CreateNewObjPacket( VOID )
{
	__ENTER_FUNCTION

	if ( getScene() != NULL )
	{
		if( !IsAlive() )
		{//尸体
			GCNewPet_Death *pPacket = (GCNewPet_Death*)(getScene()->m_pPacket_NewPet_Death);
			pPacket->setObjID( GetID() );
			pPacket->setWorldPos( *(getWorldPos()) );
			pPacket->setDir( getDir() );
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			return pPacket;
		}
		else if( IsMoving() )
		{
			GCNewPet_Move *pPacket = (GCNewPet_Move*)(getScene()->m_pPacket_NewPet_Move);
			pPacket->setObjID( GetID() ) ;
			pPacket->setHandleID( GetLogicCount() ) ;
			pPacket->setWorldPos( *getWorldPos() ) ;
			pPacket->setTargetPos( *GetFinalTarPos() ) ;
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			return pPacket;
		}
		else
		{
			GCNewPet *pPacket = (GCNewPet*)(getScene()->m_pPacket_NewPet);
			pPacket->setObjID( GetID() ) ;
			pPacket->setWorldPos( *(getWorldPos()) ) ;
			pPacket->setDir( getDir() );
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			return pPacket;
		}
	}
	else
	{
		Assert( getScene() != NULL && "Obj_Pet::CreateNewObjPacket" );
		return NULL;
	}

	__LEAVE_FUNCTION

	return NULL;
}

VOID Obj_Horse::DestroyNewObjPacket( Packet *pPacket )
{
}

FLOAT Obj_Horse::GetMoveSpeed( VOID )
{
	__ENTER_FUNCTION

	_HORSE_ATTR* pPetAttr = g_HorseAttrTbl.GetAttr(GetDataID());
	if (!pPetAttr)
		return 0;

	return (FLOAT)pPetAttr->m_iMoveSpeed;
	__LEAVE_FUNCTION
	return 0;
}

VOID Obj_Horse::AskMsg_BaseAttrib( Obj_Human *pTargetHuman )
{
	__ENTER_FUNCTION

	if ( IsCanViewMe( pTargetHuman ) )
	{
		GCCharBaseAttrib msgAttr;

		BYTE yHPPercent = (GetMaxHP())?((BYTE)((GetHP()*100 + GetMaxHP() - 1)/GetMaxHP())):(0);
		BYTE yMPPercent = (GetMaxMP())?((BYTE)((GetMP()*100 + GetMaxMP() - 1)/GetMaxMP())):(0);

		msgAttr.setObjID( GetID() );
		msgAttr.setDataID( (WORD)GetDataID() );
		msgAttr.setLevel(GetLevel());
		msgAttr.setName( GetName() );
		msgAttr.setHPPercent( yHPPercent );
		msgAttr.setMPPercent( yMPPercent );
		msgAttr.setRage( GetRage() );
		msgAttr.setStealthLevel( GetStealthLevel() );
		if ( GetCampData() != NULL )
			msgAttr.setCampData( GetCampData() );
		msgAttr.setOwnerID( GetOwnerID() );
		msgAttr.setModelID( GetModelID() );
		
		msgAttr.setMountID(GetMountID());
		msgAttr.setMoveSpeed(GetMoveSpeed());
		msgAttr.setAttackSpeed((FLOAT)GetAttackSpeed());

		pTargetHuman->GetPlayer()->SendPacket( &msgAttr );

	}

	__LEAVE_FUNCTION
}

VOID Obj_Horse::AskMsg_DetailAttrib( Obj_Human *pTargetHuman )
{
	__ENTER_FUNCTION

	// 
	if ( pTargetHuman == GetOwner() )
	{
		GCDetailAttrib_Pet msgDetail;

		msgDetail.SetGUID( GetPetGUID() );

		msgDetail.SetObjID( GetID() );
		msgDetail.SetDataID( GetDataID() );
		msgDetail.SetName( GetName() );
		msgDetail.SetHP( GetHP() );
		msgDetail.SetHPMax(GetMaxHP());
		msgDetail.SetAIType( GetAIType() );
		msgDetail.SetLevel( GetLevel() );
		msgDetail.SetDataID(GetDataID());
		msgDetail.SetModelID( GetModelID() );
		msgDetail.SetMountID(GetMountID());
		msgDetail.SetLife( 0 );
		msgDetail.SetGeneration( GetGeneration() );

		msgDetail.SetAtt_Near(Attr_VerifyGeneralAttack(GetAttackNear()));
		msgDetail.SetAtt_Far(Attr_VerifyGeneralAttack(GetAttackFar()));

		msgDetail.SetAtt_MagicNear(Attr_VerifyGeneralAttack(GetAttackMagicNear()));
		msgDetail.SetAtt_MagicFar(Attr_VerifyGeneralAttack(GetAttackMagicFar()));

		msgDetail.SetDef_Near(Attr_VerifyDefence(GetDefenceNear()));
		msgDetail.SetDef_Far(Attr_VerifyDefence(GetDefenceFar()));

		msgDetail.SetDef_MagicNear(Attr_VerifyDefence(GetDefenceMagicNear()));
		msgDetail.SetDef_MagicFar(Attr_VerifyDefence(GetDefenceMagicFar()));

		msgDetail.SetHit(GetHit());
		msgDetail.SetMiss(GetMiss());
		msgDetail.SetCritical(GetCritical());
		msgDetail.SetExp(GetExp());
		msgDetail.SetStrPerception(GetStrPerception());
		msgDetail.SetConPerception(GetSmartnessPerception());
		msgDetail.SetDexPerception(GetMindPerception());
		msgDetail.SetIntPerception(GetConstitutionPerception());
		msgDetail.SetStr(GetStr());
		msgDetail.SetCon(GetCon());
		msgDetail.SetDex(GetDex());
		msgDetail.SetInt(GetInt());
		msgDetail.SetRemainPoint(GetRemainPoint());
		pTargetHuman->GetPlayer()->SendPacket(&msgDetail);

	}
	__LEAVE_FUNCTION
}

INT Obj_Horse::GetMountID( VOID )
{
	INT iDataID = GetDataID();
	_HORSE_ATTR* pPetAttr = g_HorseAttrTbl.GetAttr(iDataID);
	if (!pPetAttr)
		return INVALID_ID;

	return g_HorseAttrTbl.m_aHorseTbl[iDataID].m_iMountID;
}

INT Obj_Horse::GetCaptureCount(VOID)const
{
	return m_nCapturerCount;
}