// Obj_Monster.cpp

#include "stdafx.h"
#include "Scene.h"
#include "Log.h"
#include "Config.h"
#include "GCCharBaseAttrib.h"
#include "GCErrorAttack.h"
#include "GCNewMonster.h"
#include "GCNewMonster_Move.h"
#include "GCNewMonster_Death.h"
#include "GCChat.h"

#include "Scene.h"
#include "TimeManager.h"
#include "GameTable.h"
//��Ʒ���
#include "Obj_ItemBox.h"
#include "ItemTypes.h"
#include "ItemRuler.h"

#include "Obj_Pet.h"
#include "Obj_Monster.h"
#include "ItemOperator.h"
#include "ItemContainer.h"

#include "AI_Monster.h"
#include "ShopManager.h"
#include "PetPlacard.h"
#include "StdImpact033.h"
#include "StdImpact051.h"
#include "Patrol.h"
#include "GCNotifyGoodBad.h"
#include "CharacterVisitor.h"
#include "PetManager.h"
#include "HorseManager.h"
#include "DropFromMonster.h"
#include "GCRelation.h"
#include "GWRelation.h"
#include "ServerManager.h"


using namespace Packets;
using namespace Combat_Module::Impact_Module;

Obj_Monster::Obj_Monster( VOID )
{
__ENTER_FUNCTION

	// NPC�̵�
	m_pShopManager			= NULL;
	// ���﹫���
	m_pPetPlacardSystem		= NULL;
	m_nOwnerElapsedTick     = 0;
	ClearSelf();
	m_pAI_Character = new AI_Monster;
	Assert( m_pAI_Character ) ;

__LEAVE_FUNCTION
}

Obj_Monster::~Obj_Monster( VOID )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pAI_Character );
	SAFE_DELETE( m_pShopManager );
	if(m_pPetPlacardSystem != NULL)
	{
		m_pPetPlacardSystem->CleanUp();
		SAFE_DELETE(m_pPetPlacardSystem);
	}

__LEAVE_FUNCTION
}

VOID Obj_Monster::ClearSelf()
{
	__ENTER_FUNCTION

	// ��Ϣ/����ˢ��
	m_AttrBackUp.CleanUp();

	// CoolDown
	m_nCooldown				= 0;

	// Ч��
	m_ImpactList.CleanUp();

	// ����
	m_RespawnTime			= -1;
	m_RespawnTimer.CleanUp();
	m_RespawnDir			= -1.f;
	m_RespawnPos			= WORLD_POS(-1.f, -1.f);

	// �˺��б�
	m_Own_TeamID			= INVALID_ID;
	m_OccupantGUID			= INVALID_GUID;
	m_OccupantLevel			= 0;
	m_DamageMemList.CleanUp();
	m_OwnerList.CleanUp();
	m_nOwnerElapsedTick     = 0;
	// AI
	m_AIType				= -1;
	m_AIScript				= -1;
	m_LeaderID				= -1;
	m_PositionRange			= 0;

	// NPC�̵�
	SAFE_DELETE( m_pShopManager );

	// ���﹫���
	ReleasePetPlacardSystem();

	// ����
	m_nPatrolID				= INVALID_ID;
	m_DropSearchRange		= 0.f;
	m_DropTeamCount			= 0;
	m_CreateFlag			= FALSE;

	//m_DropRuler;

	//m_sKillObj;
	m_nKillCount			= 0;

	// һ������
	m_GUID					= INVALID_GUID;
	m_uDataID				= INVALID_ID;
	memset(m_szName, 0, sizeof(m_szName));
	memset(m_szTitle, 0, sizeof(m_szTitle));
	m_HP					= 0;
	m_MP					= 0;
	m_Level					= 0;
	m_BaseExp				= 0;
	m_idScript				= 0;
	//m_CampData;
	m_MinDamagePercent		= 0;
	m_uGroupID				= INVALID_ID;
	m_uTeamID				= INVALID_ID;
	//ÿ��������ı���Ҫ���������
	m_iAttackNear			= 0;
	m_iAttackFar			= 0;

	m_iAttackMagicNear		= 0;
	m_iAttackMagicFar		= 0;
	
	m_iDefenceNear			= 0;
	m_iDefenceFar			= 0;

	m_iDefenceMagicNear		= 0;
	m_iDefenceMagicFar		= 0;

	m_iHorseID				= -1;
	m_iWeaponID				= 0;
	m_iMaxHP				= 0;
	m_iMaxMP				= 0;
	m_iHPRestore			= 0;
	m_iMPRestore			= 0;
	m_iHitRate				= 0;
	m_iMissRate				= 0;
	m_iCriticalRate			= 0;

	m_iAttackGold			= 0;
	m_iAttackWood			= 0;

	m_iAttackWater			= 0;
	m_iAttackFire			= 0;
	m_iAttackSoil			= 0;
	
	m_iDefenceGold			= 0;
	m_iDefenceWood			= 0;

	m_iDefenceWater			= 0;
	m_iDefenceFire			= 0;
	m_iDefenceSoil			= 0;
	
	__LEAVE_FUNCTION
}

VOID Obj_Monster::CleanUp( VOID )
{
	__ENTER_FUNCTION
	ClearSelf();
	Obj_Character::CleanUp();
	__LEAVE_FUNCTION
}

BOOL Obj_Monster::Init( const _OBJ_INIT *pInit )
{
__ENTER_FUNCTION

	if ( !Obj_Character::Init( pInit ) )
		return FALSE;

	_OBJ_MONSTER_INIT *pMonsterInit = (_OBJ_MONSTER_INIT*)pInit;
	MONSTER_EXT_ATTR *pAttr = g_MonsterAttrExTbl.GetExtAttr( pMonsterInit->m_uDataID );
	if ( pAttr == NULL )
		return FALSE;
	m_GUID				= pMonsterInit->m_GUID;
	//��������
	m_uDataID			= pMonsterInit->m_uDataID;
	//���������������Monster.ini��û�д���ֵ���ڹ��������
	if ( pMonsterInit->m_szName[0] == '\0' )
		strncpy( m_szName, pAttr->m_Name, sizeof( m_szName ) - 1 );
	else
		strncpy( m_szName, pMonsterInit->m_szName, sizeof( m_szName ) - 1 );

	strncpy( m_szTitle, pMonsterInit->m_szTitle, sizeof( m_szTitle ) - 1 );
	//���Monster.ini��û�д���ֵ���ڹ��������
	//����ʱ����
	m_RespawnTime		= pMonsterInit->m_RespawnTime;	//�������ɼ��ʱ��
	if( m_RespawnTime<=0 && !m_CreateFlag )
	{
		m_RespawnTime = pAttr->m_RespawnTime;
	}
	//�ȼ�
	m_Level	= pAttr->m_Level;
	//���﷽��
	SetRespawnDir( pMonsterInit->m_Dir );	
	//����λ��
	SetRespawnPos( &pMonsterInit->m_Pos );		
	//��ʼ��������ʱ��
	m_RespawnTimer.BeginTimer( m_RespawnTime, 0 );		
	m_nPatrolID			= pMonsterInit->m_nPatrolID;
	m_uGroupID			= pMonsterInit->m_uGroupID;
	m_uTeamID			= pMonsterInit->m_uTeamID;
	m_AIType			= pMonsterInit->m_BaseAI;
	m_AIScript          = pMonsterInit->m_ExtAIScript;
	m_LeaderID			= pMonsterInit->m_LeaderID;
	m_idScript			= pMonsterInit->m_idScript;

	m_PositionRange		= g_Config.m_ConfigInfo.m_PositionRange;
	m_BaseExp			= (INT)(pAttr->m_BaseExp * g_Config.m_ConfigInfo.m_ExpParam);
	m_MinDamagePercent	= pAttr->m_MinDamagePercent;
	m_CampData.m_nCampID= pAttr->m_Camp;
	m_DropSearchRange	= pAttr->m_DropSearchRange;
	m_DropTeamCount		= pAttr->m_SearchTeamCount;
	m_AttackAnimTime	= pAttr->m_AttackAnimTime;
	m_AttackCooldownTime = pAttr->m_AttackCooldownTime;
	m_nCooldown			= 0;
	m_nOwnerElapsedTick = 0;
	GetMonsterAI()->GetScriptFile(m_AIScript);
		
	//����Ч������
	Impact_InitList();

	Respawn( m_pVisitor );

	InitBackupAttr();

	//��ʼ���̵�
	BOOL hasShop = FALSE ;
	INT i ;
	for( i=0; i<MAX_SHOP_INIT; i++)
	{
		if( pMonsterInit->m_nShop[i] >= 0 )
		{
			hasShop = TRUE ;
			break ;
		}
	}
	if(hasShop)
	{
		this->InitDynamicShop();
		for(i = 0; i<MAX_SHOP_INIT; i++)
		{
			if(pMonsterInit->m_nShop[i] >= 0)
				this->BindDynamicShop(pMonsterInit->m_nShop[i]);
		}

	}
	//pMonster->BindDynamicShop(1);

	return TRUE;

__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Monster::InitAIObj( )
{
__ENTER_FUNCTION

	Assert( m_pAI_Character );
	m_pAI_Character->Init( this );

__LEAVE_FUNCTION
}


BOOL Obj_Monster::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	static INT nSceneID = 21;
	static INT nObjID = 46;
	if(getScene()->SceneID() == nSceneID && GetID() == nObjID)
	{
		INT i;
		i = 0;
		i++;
	}

	BOOL bResult = Obj_Character::HeartBeat(uTime );
	if ( !bResult )
		return bResult;

	//���̵�����̵��߼�
    if(m_pShopManager)	m_pShopManager->Tick(uTime );

    if(m_pPetPlacardSystem != NULL)
	{
		m_pPetPlacardSystem->HeartBeat( uTime );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Obj_Monster::HeartBeat_OutZone( UINT uTime )
{
__ENTER_FUNCTION

	BOOL bResult = Obj_Character::HeartBeat_OutZone(uTime );
	if ( !bResult )
		return bResult;

	if( m_RespawnTime<0 )
		return FALSE ;

	if( m_RespawnTimer.CountingTimer(uTime) )
	{//ʬ���Ѿ���ʧ������ʱ�䵽
		m_RespawnTimer.CleanUp();
		Respawn( m_pVisitor );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

Packet *Obj_Monster::CreateNewObjPacket( VOID )
{
__ENTER_FUNCTION

	if ( getScene() != NULL )
	{
		if( !IsAlive() )
		{//ʬ��
			GCNewMonster_Death *pPacket = (GCNewMonster_Death*)(getScene()->m_pPacket_NewMonster_Death);
			pPacket->setObjID( GetID() );
			pPacket->setWorldPos( *(getWorldPos()) );
			pPacket->setDir( getDir() );
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			return pPacket;
		}
		else if( IsMoving() )
		{
			GCNewMonster_Move *pPacket = (GCNewMonster_Move*)(getScene()->m_pPacket_NewMonster_Move);
			pPacket->setObjID( GetID() ) ;
			pPacket->setHandleID( GetLogicCount() ) ;
			pPacket->setWorldPos( *getWorldPos() ) ;
			pPacket->setTargetPos( *GetFinalTarPos() ) ;
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			pPacket->setHorseID( GetHorseID() );
			pPacket->setWeaponID( GetWeaponID() );

			return pPacket;
		}
		else
		{
			GCNewMonster *pPacket = (GCNewMonster*)(getScene()->m_pPacket_NewMonster);
			pPacket->setObjID( GetID() ) ;
			pPacket->setWorldPos( *(getWorldPos()) ) ;
			pPacket->setDir( getDir() );
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			pPacket->setHorseID( GetHorseID() );
			pPacket->setWeaponID( GetWeaponID() );
			return pPacket;
		}
	}
	else
	{
		Assert( getScene() != NULL && "Obj_Monster::CreateNewObjPacket" );
		return NULL;
	}

__LEAVE_FUNCTION

	return NULL;
}

VOID Obj_Monster::DestroyNewObjPacket( Packet *pPacket )
{
}

VOID Obj_Monster::OnDie_Before( ObjID_t idKiller )
{
	//�������������������˭ӵ�е�����Ʒ��
	m_DropRuler = MonsterDropRuler::GetMonsterDropRuler(this);
	switch(m_DropRuler) 
	{
	case BDR_UNKNOW:
	/*case BDR_COMMON:*/
	case BDR_BOSS:
		{
			MonsterDropRuler::CaculateBossOwnerList(this);
		}
		break;
	case BDR_COMMON:
		{	
			MonsterDropRuler::CaculateCommOwnerList(this);
		}
		break;
		//{
		//	MonsterDropRuler::CaculateCommOwnerList(this);
		//}
		//break;
	default:
		//Assert(FALSE);
		break;
	}	
}

VOID Obj_Monster::OnDie( ObjID_t idKiller )
{
__ENTER_FUNCTION

	OnDie_Before( idKiller ) ;
	
	Obj_Character::OnDie( idKiller );

	OnDie_After( idKiller ) ;

__LEAVE_FUNCTION
}

VOID Obj_Monster::OnDie_After( ObjID_t idKiller )
{
__ENTER_FUNCTION

	// ȷ���ж��ٸ����ѿ��Է��䵽����
	//������Ͳ��ܹ�ͨ���˺�������ȷ����˭��þ����ˣ�ͨ����þ�����˵Ķ����Ա��ȷ��˭��þ���(amend by yyz)
	MONSTER_OWNER_LIST OwnerList = GetOwnerList();
	INT nOwnerCount = OwnerList.OwnerCount;
	
	Obj_Human *apValidMember[MAX_TEAM_MEMBER];
	FLOAT	fSearchLength	=	GetDropSearchRange();


	Obj_Human* pHuman = NULL;
	INT nValidMemberCount = 0;						// ��Ч��Χ�ڷ��ϼ����������������
	INT nValidNewbieMemberCount = 0;
	//for( INT i=0; i<nOwnerCount; i++ )
	//{
	//	if ((OwnerList.OwnerDropList[i].m_Guid != INVALID_ID)&&(OwnerList.OwnerDropList[i].m_ObjID!=INVALID_ID))
	//	{
	//		pHuman = getScene()->GetHumanManager()->GetHuman(OwnerList.OwnerDropList[i].m_ObjID);
	//		if (pHuman)
	//		{
	//			if (pHuman->GetGUID() == OwnerList.OwnerDropList[i].m_Guid)
	//			{
	//				if (pHuman->IsInValidRadius( this, fSearchLength ))
	//				{
	//					apValidMember[nValidMemberCount++] = pHuman;
	//				}
	//			}
	//		}
	//	}
	//}
	if ((OwnerList.OwnerDropList[0].m_Guid != INVALID_ID)&&(OwnerList.OwnerDropList[0].m_ObjID!=INVALID_ID))
	{
		pHuman = getScene()->GetHumanManager()->GetHuman(OwnerList.OwnerDropList[0].m_ObjID);
		if (pHuman)
		{
			if (pHuman->__GetTeamID()!=INVALID_ID)
			{
				INT	nTeamPersonsCount =	pHuman->GetTeamInfo()->GetTeamMemberCount( );
				for (INT i=0; i<nTeamPersonsCount; ++i)
				{
					const TEAMMEMBER*	pTeamMember = pHuman->GetTeamInfo()->GetTeamMember(i);
					Assert(pTeamMember != NULL);
					//������ͨ�������ó���������ö��ѵ�OBJ�������п��ܶ�����������������ò���OBJ������������������򲻸�������
					Obj_Human *PTeamHuman = getScene()->GetHumanManager()->GetHuman(pTeamMember->m_ObjID);
					if(NULL != PTeamHuman)
					{
						if (PTeamHuman->IsInValidRadius( this, fSearchLength) && !(pHuman->IsDie()))
						{
							apValidMember[nValidMemberCount++] = PTeamHuman;
						}
					}
				}
			}
			else
			{
				apValidMember[nValidMemberCount++] = pHuman;
			}
		}
	}

	MonsterExpCaculateRuler CaculateExp;

	// �����Ҷ����Եõ�����
	if ( nValidMemberCount > 1 )
	{
		INT nExp = (m_BaseExp + (m_BaseExp*(nValidMemberCount-1))/10) / nValidMemberCount;
		UINT auExp[MAX_TEAM_MEMBER];
		INT i;

		for ( i = 0; i < nValidMemberCount; i++ )
		{ 
			Assert(apValidMember[i]);
			
			// ����ÿ����Ӧ�õľ���ֵ
			if (apValidMember[i]->GetSubTrainSetting().m_bSubTrainOpen)
			{
				auExp[i] = 1;
			}
			else
				auExp[i] = CaculateExp.CaculateBaseExp(GetLevel(),apValidMember[i]->GetLevel(),nExp, 1);
			
			// ���Ӿ���ֵ
			if(auExp[i]>0)
			{
				apValidMember[i]->SetAlterExp(auExp[i]);
				//������ת��Ϊŭ��
				apValidMember[i]->RestoreRageFromEXP(auExp[i]);
			}
			apValidMember[i]->OnKillObject( GetID() );

			// �ڴ˼�����ҵĳ������Ӿ����Section
			
			PET_GUID_t guidPet = apValidMember[i]->GetGUIDOfCallUpHorse();
			if (!guidPet.IsNull())
			{
				// �ڴ˼�����ҵĳ������Ӿ����Section
				INT iIndex = apValidMember[i]->GetHorseIndexByGUID( guidPet );
				_PET_DB* pPetDB = apValidMember[i]->GetHorseDB( iIndex );

				INT iAddExp = 0;
				if (apValidMember[i]->GetSubTrainSetting().m_bSubTrainOpen)
				{
					iAddExp = 1;
				}
				else
					iAddExp = CaculateExp.CaculateBaseExp(GetLevel(), pPetDB->m_nLevel, nExp, 1);

				if (iAddExp > 0)
					//pPet->IncrementExp(iAddExp);
					apValidMember[i]->UpdateHouseExp( iAddExp );
			}
		}

		// ����������к��ѣ������������Ӷ�������ѵĺ��Ѷ� ���˫�������ò�һ�����������ٵ����������ӵĺ��Ѷ�
		INT nPoint = 0;	// Ҫ���ӵĺ��Ѷȵ�ֵ

		// ��ͻ��˷���
		GCRelation CMsg;
		CMsg.GetRelation()->CleanUp();
		CMsg.GetRelation()->m_Type = RET_FRIENDADDPOINT;
		RETURN_NOTIFY_FRIEND* pCSend = &(CMsg.GetRelation()->m_NotifyFriend);

		for ( INT i=0; i<nValidMemberCount-1; i++ )
		{
			Assert(apValidMember[i]);
			GUID_t MyGUID = apValidMember[i]->GetGUID();
			HumanRelation* pRelation = apValidMember[i]->GetHumanRelation();
			Assert( pRelation );

			for( INT j=i+1; j<nValidMemberCount; j++ )
			{
				Assert(apValidMember[j]) ;
				GUID_t FriendGUID = apValidMember[j]->GetGUID();
				HumanRelation* pFriendRelation = apValidMember[j]->GetHumanRelation();
				Assert( pFriendRelation );

				// ����Ǻ���
				if(pRelation->IsFriend(FriendGUID) && pFriendRelation->IsFriend(MyGUID))
				{ 
					// ���ӵĺ��ѶȰ��յõ������ٵ��Ǹ��˵ľ�������
					if (auExp[i] > auExp[j])
					{
						//nPoint = auExp[j]/10;		// ����߻���û�и����������ֵ�͹�ʽ����ʱ�趨��õĺ��Ѷ��Ǿ����1/10
						nPoint = auExp[j]*10;
					}
					else
					{
						//nPoint = auExp[i]/10;
						nPoint = auExp[i]*10;
					}

					if ( nPoint>0 )
					{
						// �����޸�server�˵ĺ����б��еĺ��Ѷ�
						pRelation->IncFriendPoint(FriendGUID, nPoint);
						pFriendRelation->IncFriendPoint(MyGUID, nPoint);

						// ʵʱˢ��
						// ��word�����޸ĺ��Ѷ���Ϣ, ��word��Ҫ�õ�ʵʱ��point��Ϣ�����Ҳ��Ҫ��word���͸�����Ϣ
						{
							GWRelation* WMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
							WMsg->GetRelation()->CleanUp();
							WMsg->GetRelation()->m_Type = REQ_FRIENDADDPOINT;
							WMsg->GetRelation()->SetGUID( apValidMember[j]->GetGUID() );
							REQUEST_RELATION_INFO* pWSend = &(WMsg->GetRelation()->m_RequestInfo);
							pWSend->CleanUp();
							pWSend->SetTargetGUID( nPoint );
							pWSend->SetTargetName( apValidMember[i]->GetName() );
							g_pServerManager->SendPacket( WMsg, INVALID_ID );

							GWRelation* WTMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
							WTMsg->GetRelation()->CleanUp();
							WTMsg->GetRelation()->m_Type = REQ_FRIENDADDPOINT;
							WTMsg->GetRelation()->SetGUID( apValidMember[i]->GetGUID() );
							REQUEST_RELATION_INFO* pWTSend = &(WTMsg->GetRelation()->m_RequestInfo);
							pWTSend->CleanUp();
							pWTSend->SetTargetGUID( nPoint );
							pWTSend->SetTargetName( apValidMember[j]->GetName() );
							g_pServerManager->SendPacket( WTMsg, INVALID_ID );
						}

						// ��click�����޸ĺ��Ѷ���Ϣ,�������˫�������ͺ��Ѷ����ӵ���Ϣ
						pCSend->CleanUp();
						pCSend->SetGUID( nPoint );	// ��ʱ�ô�GUID��λ�ô����ӵĺ��Ѷ�
						pCSend->SetName( apValidMember[j]->GetName() );
						apValidMember[i]->GetPlayer()->SendPacket( &CMsg );

						pCSend->CleanUp();
						pCSend->SetGUID( nPoint );
						pCSend->SetName( apValidMember[i]->GetName() );
						apValidMember[j]->GetPlayer()->SendPacket( &CMsg );
					}
				}
			}
		}

	}
	// ������ҵõ�����
	else if( nValidMemberCount==1 )
	{
		uint auExp = 0;
		if (apValidMember[0]->GetSubTrainSetting().m_bSubTrainOpen)
		{
			auExp = 1;
		}
		else
			auExp = CaculateExp.CaculateBaseExp(GetLevel(),apValidMember[0]->GetLevel(),m_BaseExp, 0);

		if(auExp>0)
		{
			apValidMember[0]->SetAlterExp(auExp);
			//������ת��Ϊŭ��
			apValidMember[0]->RestoreRageFromEXP(auExp);
		}
		apValidMember[0]->OnKillObject( GetID() ) ;

		// �ڴ˼�����ҵĳ������Ӿ����Section
		PET_GUID_t guidPet = apValidMember[0]->GetGUIDOfCallUpHorse();
		if (!guidPet.IsNull())
		{
			INT iIndex = apValidMember[0]->GetHorseIndexByGUID( guidPet );
			_PET_DB* pPetDB = apValidMember[0]->GetHorseDB( iIndex );
			UINT iAddExp = 0;
			if (apValidMember[0]->GetSubTrainSetting().m_bSubTrainOpen)
			{
				iAddExp = 1;
			}
			else
				iAddExp = CaculateExp.CaculateBaseExp(GetLevel(), pPetDB->m_nLevel, m_BaseExp, 0);

			if (iAddExp > 0)
				//pPet->IncrementExp(iAddExp);
				apValidMember[0]->UpdateHouseExp( iAddExp );
		}


	}	

	//���ݵ�����򣬼����������䴦��
	switch(m_DropRuler) 
	{
	case BDR_COMMON:
		{	
			if(!MonsterDropRuler::CaculateCommDropRuler(this, m_DropRuler))
			{

			}
			
		}
		break;
	case BDR_BOSS:
	//case BDR_UNKNOW:
		{
			if(!MonsterDropRuler::CaculateBossDropRuler(this,m_DropRuler))
			{
			}
		}
		break;
		//break;
		//break;
		//{
		//	MonsterDropRuler::CaculateCommDropRuler(this);
		//}
	default:
		Assert(FALSE);
		break;
	}

	//�����
	DropHorseOnDie_After();

	return;

__LEAVE_FUNCTION
}

VOID Obj_Monster::Enter_Respawn( VOID )
{
__ENTER_FUNCTION
	//m_DamageMemList.CleanUp();
	m_Own_TeamID = INVALID_ID ;
	m_OccupantGUID = INVALID_ID ;
	m_OccupantLevel = 0;
	GetOwnerList().CleanUp();
	GetKillerRec().CleanUp();

	if( m_RespawnTime>=0 )
	{
		m_RespawnTimer.BeginTimer( (UINT)(m_RespawnTime*g_Config.m_ConfigInfo.m_fRespawnParam), NowTime() );
	}
	else
	{
		m_RespawnTimer.BeginTimer( m_RespawnTime, NowTime() );
	}

	SetActiveFlag( FALSE );

__LEAVE_FUNCTION
}

BOOL Obj_Monster::Respawn( CharacterVisitor* pVisitor )
{
__ENTER_FUNCTION

	
	Assert( !IsActiveObj() );//�����Ĺ������Ҫ����
	Assert( pVisitor != NULL );	
	if( pVisitor == NULL )
	{
		GetRespawnPos();
	}
	FLOAT fDir;
	WORLD_POS Pos;
	Pos = *GetRespawnPos();
	fDir = GetRespawnDir();
	getScene()->GetMap()->VerifyPos( &Pos );
	getScene()->GetMap()->IsCanGo(Pos) ? Pos : Pos = *GetRespawnPos();
	setWorldPos( &Pos );
	m_pVisitor->VisitInitMonster( this );
	//ˢ�¹�����Ҹ�client������Ϣ
	UpdateZone();

	//��������
	SetHP( GetMaxHP() );
	SetMP( GetMaxMP() );	// add by gh 2010/03/25 Ϊ�˳�ʼ������MP
	m_nKillCount = 0;
	memset(m_sKillObj,-1,sizeof(m_sKillObj));
	

	GetMonsterAI()->CleanUp();

	AI_Monster* pAI = (AI_Monster*)GetAIObj();
	if (pAI) 
	{
		if ( isPatrolMonster( ) ) 
		{
			pAI->StartPatrol( ) ;
		}
		else if (INVALID_ID != m_LeaderID)
		{
			pAI->StartTeamFollow();
		}
		else
		{
			pAI->ChangeState(ESTATE_IDLE);
		}
	}
    
	SetActiveFlag(TRUE);
	//Relive();
	
	//Log::SaveLog( SERVER_LOGFILE, "Obj_Monster Spawn OK! Type=%d ObjID:%d", 
	//	GetDataID(), GetID() );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

INT	Obj_Monster::CannotFight()
{
	__ENTER_FUNCTION
	MONSTER_EXT_ATTR *pAttr = g_MonsterAttrExTbl.GetExtAttr( GetDataID() );
	
	Assert(pAttr);
	
	return pAttr->m_CannotFight;	

	__LEAVE_FUNCTION

	return 0;
}

BOOL Obj_Monster::IsEnemy(Obj_Character* pCharacter )
{
__ENTER_FUNCTION
	OWN_IMPACT* pImp = NULL;
	Assert( pCharacter );
	if(GetID()==pCharacter->GetID())
	{
		return FALSE;
	}
	{ //����ר��
		SOT_ShadowGuard_T logic;
		pImp = Impact_GetFirstImpactOfSpecificLogicID(logic.ID);
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
	} while(0);
	{ //�߻�ר��
		SOT_XingXiu011_T logic;
		pImp = Impact_GetFirstImpactOfSpecificLogicID(logic.ID);
		if(NULL!=pImp)
		{
			Obj* pObj = Impact_GetCaster(*this, *pImp);
			if(NULL!=pObj)
			{
				if(TRUE == IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rFascinator = *static_cast<Obj_Character*>(pObj);
					return rFascinator.IsEnemy(pCharacter);
				}
			}
		}
	} while(0);
	BOOL bIsEnemy = IsEnemyCamp(*pCharacter);
	return bIsEnemy;

__LEAVE_FUNCTION

	return FALSE;
}
BOOL Obj_Monster::IsFriend(Obj_Character* pCharacter )
{
__ENTER_FUNCTION
	
	Assert( pCharacter );
	return FALSE==IsEnemy(pCharacter);

__LEAVE_FUNCTION

	return FALSE;
}
BOOL Obj_Monster::IsPartner(Obj_Character* pCharacter )
{
__ENTER_FUNCTION
	
	Assert( pCharacter );
	if( pCharacter->GetObjType() == Obj::OBJ_TYPE_MONSTER)
		return TRUE;

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID Obj_Monster::SetName( const CHAR *pszName )
{
	strncpy( m_szName, pszName, sizeof( m_szName ) - 1 );
	m_AttrBackUp.m_bNameModified = TRUE;
}

VOID Obj_Monster::SetTitle( const CHAR *pszTitle )
{
	strncpy( m_szTitle, pszTitle, sizeof( m_szTitle ) - 1 );
	m_AttrBackUp.m_bTitleModified = TRUE;
}
INT	Obj_Monster::GetHP( VOID )
{
	if(m_HP>GetMaxHP())
	{
		m_HP=GetMaxHP();
	}
	return m_HP;
}

VOID Obj_Monster::SetHP( INT nHP )
{
	INT nMaxHP = GetMaxHP();
	if( nHP > nMaxHP )
	{
		nHP = nMaxHP;
	}
	m_HP = nHP;
}
// modify by gh 2010/03/25
VOID Obj_Monster::SetMP( INT nMP )
{
	INT nMaxMP = GetMaxMP();
	if( nMP > nMaxMP )
	{
		nMP = nMaxMP;
	}
	m_MP = nMP;
}
// add by gh 2010/03/25 Ϊ�˽������MPΪ0
INT Obj_Monster::GetMP( VOID )
{
	if(m_MP>GetMaxMP())
	{
		m_MP=GetMaxMP();
	}
	return m_MP;
}
// end modify 

VOID Obj_Monster::SetRage(INT nRage )
{

}

VOID Obj_Monster::SetLevel( INT nLevel )
{
	if(m_Level != nLevel)
	{
		m_Level = nLevel;
		OnLevelChanged();
	}
}

VOID Obj_Monster::InitDynamicShop()
{
__ENTER_FUNCTION

	SAFE_DELETE(m_pShopManager);
	m_pShopManager = new DynamicShopManager(this);
	Assert(m_pShopManager);

	m_pShopManager->Init();

__LEAVE_FUNCTION
}

INT	Obj_Monster::BindDynamicShop(INT shopindex)
{
__ENTER_FUNCTION

	return m_pShopManager->AddDynamicShop(g_pStaticShopManager->GetShopByID(shopindex));

__LEAVE_FUNCTION

	return 0 ;
}

BOOL Obj_Monster::CreatePetPlacardSystem(VOID)
{
	if(m_pPetPlacardSystem == NULL)
	{
		m_pPetPlacardSystem = new PetPlacardSystem;
		BOOL bResult = m_pPetPlacardSystem->Init();
		if(!bResult)
			return FALSE;
	}
	return TRUE;
}

VOID Obj_Monster::ReleasePetPlacardSystem(VOID)
{
	if(m_pPetPlacardSystem != NULL)
	{
		m_pPetPlacardSystem->CleanUp();
		SAFE_DELETE(m_pPetPlacardSystem);
	}
}

DAMAGE_MEM_LIST&	Obj_Monster::GetKillerRec()
{
	return m_DamageMemList;
}

VOID Obj_Monster::UpdateDamageList(INT nDamage, Obj_Character* pAttacker)
{
	__ENTER_FUNCTION
	if(nDamage >= 1 && pAttacker )
	{
		Obj_Human *pHuman = NULL;
		BOOL      BIsChangeTeamInfo = FALSE;	//�Ƿ�����˺����������ݵ���Ϣ

		Obj::ObjType nType = pAttacker->GetObjType();
		switch(nType) 
		{
		case Obj::OBJ_TYPE_HUMAN:
			pHuman = (Obj_Human*)(pAttacker);
			break;
		case Obj::OBJ_TYPE_PET:
			{
				Obj_Character* pOwner = ((Obj_Pet*)(pAttacker))->GetOwner();

				if (pOwner->GetObjType() == Obj::OBJ_TYPE_HUMAN )
				{
					pHuman = (Obj_Human*)pOwner;
				}
			}
			break;
		default:
			break;
		}

		if ( pHuman != NULL )
		{
			ObjID_t	nHumanID = pHuman->GetID();
			if ( GetOccupantGUID() == INVALID_ID )
			{
				if( pHuman->__GetTeamID()!=INVALID_ID )
				{
					BIsChangeTeamInfo = TRUE;
				}
				else
				{
					m_DamageMemList.AddMember(pHuman->GetGUID(), pHuman->GetID(), pHuman->__GetTeamID(),nDamage);
				}
				SetOccupantGUID( pHuman->GetGUID() ) ;
				SetOccupantLevel(pHuman->GetLevel());
			}
			else
			{
				m_nOwnerElapsedTick	  += GetLogicTime();
				INT nElapsedTick = m_nOwnerElapsedTick - g_Config.m_ConfigInfo.m_ValidAttackTime;

				if(nElapsedTick > 0)
				{
					if( pHuman->__GetTeamID()!=INVALID_ID )
					{
						BIsChangeTeamInfo = TRUE;
					}
					else
					{
						m_DamageMemList.CleanUp();
						m_DamageMemList.AddMember(pHuman->GetGUID(), pHuman->GetID(), pHuman->__GetTeamID(),nDamage);
					}
					SetOccupantGUID( pHuman->GetGUID() ) ;
					SetOccupantLevel(pHuman->GetLevel());
				}
				else if(GetOccupantGUID() == pHuman->GetGUID())
				{
					m_nOwnerElapsedTick = 0;
					if (pHuman->__GetTeamID() != GetOccupantTeamID())
					{
						//���鷢������������������飬�����飩������ID�����˱仯����ô����Ʒ����Ʒ��¼�����ID����,��ʹ�ж��飬�����е�������Ҳ�ò���
						m_DamageMemList.CleanUp();
						m_DamageMemList.AddMember(pHuman->GetGUID(), pHuman->GetID(), pHuman->__GetTeamID(),nDamage);
					}
					else
					{
						//�������IDû�б仯������¶������Ϣ���п��ܶ��ѻ�ӳ���Ϣ�б����
						if( pHuman->__GetTeamID()!=INVALID_ID )
						{
							BIsChangeTeamInfo = TRUE;
						}
					}
				}
				else if ( pHuman->__GetTeamID() != INVALID_ID )
				{
					if ( GetOccupantTeamID() == pHuman->GetTeamInfo()->GetTeamID() )
					{
						m_nOwnerElapsedTick = 0;
						Obj_Human *pAttackerHuman = getScene()->GetHumanManager()->GetHuman(m_DamageMemList.m_DamageRec[0].m_KillerObjID);//���ԭӵ����
						if (pAttackerHuman != NULL)
						{
							//���ռ���߸����˶���
							if (pAttackerHuman->__GetTeamID() != GetOccupantTeamID())
							{
								m_DamageMemList.CleanUp();
								m_DamageMemList.AddMember(pAttackerHuman->GetGUID(), pAttackerHuman->GetID(), pAttackerHuman->__GetTeamID(),nDamage);
							}
							else 
							{
								pHuman = pAttackerHuman;	//���û�������飬����ռ�����Լ������ķ�ʽ����
								BIsChangeTeamInfo = TRUE;
							}
						}
					}
				}
			}
			
			if (BIsChangeTeamInfo)
			{
				if( pHuman->__GetTeamID()!=INVALID_ID )
				{
					m_DamageMemList.CleanUp();
					TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
					Assert(pTeamInfo);
					SetOccupantTeamID( pTeamInfo->GetTeamID() ) ;
					SetDistribMode(pTeamInfo->GetDistribMode());
					SetLastOwnerPos(pTeamInfo->GetLastOwnerPos());
					SetOwnTeamLeader(pTeamInfo->Leader()->m_GUID);

					INT	nTeamPersonsCount =	pTeamInfo->GetTeamMemberCount( );//�����г�Ա���� human
					//�����޸Ŀ���ʹ����ĵ�һ��Ԫ�ش�����Ǹ�ռ���ߣ�����������ӵ�����£��ڷֱ�ʰȡģʽ�¿��Կ��ٶ�λ�Ǹ�ռ����
					m_DamageMemList.AddMember(pHuman->GetGUID(), pHuman->GetID(), pHuman->__GetTeamID(),nDamage);
					for (INT i=0; i<nTeamPersonsCount; ++i)
					{
						const TEAMMEMBER*	pTeamMember = pTeamInfo->GetTeamMember(i);
						Assert(pTeamMember != NULL);
						if (pHuman->GetGUID() != pTeamMember->m_GUID)
						{
							m_DamageMemList.AddMember(pTeamMember->m_GUID, pTeamMember->m_ObjID, pTeamInfo->GetTeamID(),nDamage);
						}	
					}
				}
				BIsChangeTeamInfo = FALSE;
			}
		}
	}
	__LEAVE_FUNCTION
}

MONSTER_OWNER_LIST&	Obj_Monster::GetOwnerList() 	
{
	return m_OwnerList;
}

FLOAT		Obj_Monster::GetDropSearchRange() const
{
	return m_DropSearchRange;
}
INT			Obj_Monster::GetDropTeamCount() const
{
	return  m_DropTeamCount;	
}

INT			Obj_Monster::GetMinDamagePercent() const
{
	return m_MinDamagePercent;
}

VOID		Obj_Monster::SetPatrolId(INT PatrolID)
{
__ENTER_FUNCTION
	PatrolPathMgr* pPatrolPathMgr = getScene()->GetPatrolPathMgr();
	if (!pPatrolPathMgr)
	{
		return;
	}
	if (TRUE == pPatrolPathMgr->FindPatrolID(PatrolID))
	{
		m_nPatrolID = PatrolID;
	}
__LEAVE_FUNCTION
}
//VOID Obj_Monster::StartPatrol( VOID )
//{
//	GetMonsterAI()->StartPatrol();
//}
//
//VOID Obj_Monster::StopPatrol( VOID )
//{
//	GetMonsterAI()->StopPatrol();
//}



INT	 Obj_Monster::GetMonsterAIType(VOID)
{
	INT nAIType = GetAIType();
	INT nRet = GCCharBaseAttrib::CHAR_AI_TYPE_INVALID;
	if (g_MonsterAITbl.Get(nAIType, AIPARAM_SCANTIME) > 0)
	{// ��������NPC
		nRet = GCCharBaseAttrib::CHAR_AI_TYPE_SCANNPC;
	}
	else if (TRUE == IsUnbreakable())
	{// �����Թ���NPC
		nRet = GCCharBaseAttrib::CHAR_AI_TYPE_CANNOTATTACK;
	}
	else if (g_MonsterAITbl.Get(nAIType, AIPARAM_SCANTIME) <= 0)
	{// ����������NPC
		nRet = GCCharBaseAttrib::CHAR_AI_TYPE_NOTSCANNPC;
	}
	else
	{
	}
	
	return nRet;
}

//VOID	Obj_Monster::Betray(const Obj_Character* pCharacter)
//{
//#define SCANMONSTER (0)
//	__ENTER_FUNCTION
//		
//	if (!pCharacter) {
//		return ;
//	}
//
//	// ���ó�����ɨ���
//	SetAIType(SCANMONSTER);
//	// ���������Ӫ���ó�pCharacter����Ӫ
//	SetBaseCampID(pCharacter->GetBaseCampID() );
//
//	__LEAVE_FUNCTION
//}

VOID	Obj_Monster::ProcessPaoPao(VOID)
{
__ENTER_FUNCTION
	
	AI_Monster* pAIMonster = (AI_Monster*)GetAIObj();
	if (pAIMonster)
	{
		pAIMonster->ProcessPaoPao();
	}

__LEAVE_FUNCTION
}

VOID	Obj_Monster::PaoPao(INT idPaoPaoBase, INT idPaoPaoStep)
{
	__ENTER_FUNCTION
	if (idPaoPaoBase < 0 || idPaoPaoStep < 0) {
		return ;
	}
	Scene* pScene = getScene();
	if (!pScene) {
		return ;
	}

	if (idPaoPaoStep == 0) {
		return ;
	}
	INT idPaoPao = rand() % idPaoPaoStep + idPaoPaoBase;
	// ͨ������ͨ������Χ��human�㲥
	// ���͵ĸ�ʽ����
	/* @*;npcpaopao;idObj;idPaoPao */
	CHAR szContex[64];
	memset(szContex, 0, 64);
	sprintf(szContex, "@*;npcpaopao;%d;%d", GetID(), idPaoPao);

	GCChat msg;
	msg.SetChatType( CHAT_TYPE_SELF );
	msg.SetContex( szContex );
	msg.SetContexSize( (BYTE)strlen(szContex) );

	pScene->BroadCast(&msg, getZoneID() );

	__LEAVE_FUNCTION

}

VOID Obj_Monster::DropHorseOnDie_After()
{
	__ENTER_FUNCTION
	if( GetHorseID() != INVALID_ID )
	{
		ObjID_t iPetID;
		MONSTER_EXT_ATTR *pAttr = g_MonsterAttrExTbl.GetExtAttr( GetDataID() );
		if ( pAttr == NULL )
			return;
		if( pAttr->m_iHorseDropCount == 0 )
			return;
		INT iHorseDropID = getScene()->GetRand100() % pAttr->m_iHorseDropCount;
		if( iHorseDropID >= pAttr->m_iHorseDropCount )
		{
			iHorseDropID = pAttr->m_iHorseDropCount - 1;
		}
		//�����������1��ʼ���ڴ˼�һ
		INT iHorseDropCount = DropFromMonster::DropHorse( pAttr->m_aHorseDropID[iHorseDropID], this );
		for( INT i=0; i<iHorseDropCount; ++i )
		{
			INT iHorseID = DropFromMonster::GetDropID( i );
			if( iHorseID == INVALID_ID )
			{
				continue;
			}
			getScene()->GetHorseManager()->CreateHorse( iHorseID, (WORLD_POS*)getWorldPos(), iPetID );
			//getScene()->GetHorseManager()->CreateHorse( GetHorseID(), (WORLD_POS*)getWorldPos(), iPetID );
		}
	}
	__LEAVE_FUNCTION
}

BOOL	Obj_Monster::GetNearCanDropPos(WORLD_POS& dropPos)
{
	__ENTER_FUNCTION

	SceneDropPosManager* pDropPosMag = getScene()->GetSceneDropPosManager();
	Assert(pDropPosMag);

	WORLD_POS	Pos	=	*getWorldPos() ;
	UINT pos_x = (UINT)(Pos.m_fX/g_Config.m_ConfigInfo.m_DropPosDistance);
	UINT pos_z = (UINT)(Pos.m_fZ/g_Config.m_ConfigInfo.m_DropPosDistance);

	Pos.m_fX = pos_x*g_Config.m_ConfigInfo.m_DropPosDistance;
	Pos.m_fZ = pos_z*g_Config.m_ConfigInfo.m_DropPosDistance;

	INT nRandPos = 0;
	m_DropRuler = MonsterDropRuler::GetMonsterDropRuler(this);
	switch(m_DropRuler) 
	{
	case BDR_BLUE:
	case BDR_GOLD:
		{
			//nRandPos = RandGen::GetRand(0, 20);
			nRandPos = 0;

			//DROP_POS_TB* pDropPos = g_ItemTable.GetDropPosTB(nRandPos);
			//Assert(pDropPos);
			//dropPos.m_fX =  Pos.m_fX + pDropPos->m_DropX;
			//dropPos.m_fZ =  Pos.m_fZ + pDropPos->m_DropZ;
			DROP_POS_TB* pDropPos;

			for (INT i=0; i<g_Config.m_ConfigInfo.m_DropSearchRange; ++i)
			{
				pDropPos = g_ItemTable.GetDropPosTB(nRandPos);
				Assert(pDropPos);
				dropPos.m_fX =  Pos.m_fX + (pDropPos->m_DropX/2*g_Config.m_ConfigInfo.m_DropPosDistance);
				dropPos.m_fZ =  Pos.m_fZ + (pDropPos->m_DropZ/2*g_Config.m_ConfigInfo.m_DropPosDistance);

				if (pDropPosMag->CanDrop(dropPos.m_fX, dropPos.m_fZ))
				{
					return TRUE;
				}
				if (nRandPos>=g_Config.m_ConfigInfo.m_DropSearchRange)
				{
					nRandPos = 0;
				}
				++nRandPos;
			}
		}
	case BDR_BOSS:
		{
			//nRandPos = RandGen::GetRand(0, 90);
			nRandPos = 0;

			//DROP_POS_TB* pDropPos = g_ItemTable.GetDropPosTB(nRandPos);
			//Assert(pDropPos);
			//dropPos.m_fX =  Pos.m_fX + pDropPos->m_DropX;
			//dropPos.m_fZ =  Pos.m_fZ + pDropPos->m_DropZ;
			DROP_POS_TB* pDropPos;
			//INT count = 0;

			for (INT i=0; i<g_Config.m_ConfigInfo.m_DropSearchRange; ++i)
			{
				pDropPos = g_ItemTable.GetDropPosTB(nRandPos);
				Assert(pDropPos);
				dropPos.m_fX =  Pos.m_fX + (pDropPos->m_DropX/2*g_Config.m_ConfigInfo.m_DropPosDistance);
				dropPos.m_fZ =  Pos.m_fZ + (pDropPos->m_DropZ/2*g_Config.m_ConfigInfo.m_DropPosDistance);

				if (pDropPosMag->CanDrop(dropPos.m_fX, dropPos.m_fZ))
				{
					return TRUE;
				}
				++nRandPos;
				if (nRandPos>=g_Config.m_ConfigInfo.m_DropSearchRange)
				{
					nRandPos = 0;
				}
			}
		}
		break;
	case BDR_COMMON:
		{	
			//nRandPos = RandGen::GetRand(0, 3);
			nRandPos = 0;

			//DROP_POS_TB* pDropPos = g_ItemTable.GetDropPosTB(nRandPos);
			//Assert(pDropPos);
			//dropPos.m_fX =  Pos.m_fX + pDropPos->m_DropX;
			//dropPos.m_fZ =  Pos.m_fZ + pDropPos->m_DropZ;
			DROP_POS_TB* pDropPos;

			for (INT i=0; i<g_Config.m_ConfigInfo.m_DropSearchRange; ++i)
			{
				pDropPos = g_ItemTable.GetDropPosTB(nRandPos);
				Assert(pDropPos);
				dropPos.m_fX =  Pos.m_fX + (pDropPos->m_DropX/2*g_Config.m_ConfigInfo.m_DropPosDistance);
				dropPos.m_fZ =  Pos.m_fZ + (pDropPos->m_DropZ/2*g_Config.m_ConfigInfo.m_DropPosDistance);

				if (pDropPosMag->CanDrop(dropPos.m_fX, dropPos.m_fZ))
				{
					return TRUE;
				}
				++nRandPos;
				if (nRandPos>=g_Config.m_ConfigInfo.m_DropSearchRange)
				{
					nRandPos = 0;
				}
			}
		}
		break;
	default:
		return FALSE;
		break;
	}	
	return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}
