
/**	Obj_PlayerNPC.cpp
 *	
 *	���ܣ�	
 *	
 *	�޸ļ�¼��
 *			080221	���ֵ�������Ч�İ󶨵���id�ĳ��ַ����� �ɲ߻��ڱ���ֱ����������
 *	
 */

#include "StdAfx.h"
#include "GITimeSystem.h"
#include "GIUtil.h"
#include "Obj_PlayerNPC.h"
#include "Global.h"
#include "World\WorldManager.h"
#include "Procedure\GameProcedure.h"
#include "Obj_PlayerMySelf.h"
#include "..\..\Manager\ObjectManager.h"
#include "..\..\ObjectDef.h"
#include "DBC\GMDataBase.h"
#include "GIDBC_Struct.h"
#include "GIException.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GITimeSystem.h"
#include "AxProfile.h"
#include "GIActionSystem.h"
#include "GIGameInterface.h"
#include "DataPool\GMDP_Struct_Skill.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GameStruct_Skill.h"
#include "Obj_PlayerMySelf.h"
#include "Procedure\GameProcedure.h"
#include "..\Item\Obj_Item.h"
#include "..\Item\Obj_Item_Medicine.h"
#include "Action\GMActionSystem_Item.h"

#include "DataPool\GMUIDataPool.h"



KL_IMPLEMENT_DYNAMIC(CObject_PlayerNPC, GETCLASS(CObject_Character));



CObject_PlayerNPC::CObject_PlayerNPC()
{
	m_pCreatureInfo			= NULL;
	m_GUIDServer			= 0xFFFFFFFF;
	m_bNeedFaceToPlayer		= FALSE;
	m_eNpcType				= NPC_TYPE_INVALID;
	m_uStartFaceToPlayerTime= 0;
	m_bFaceToPlayer			= FALSE;
	m_fvPreRotation			= fVector3(0.0f, 0.0f, 0.0f);
	m_eMissionState			= MS_INVALID;
	m_bMissStateChange		= TRUE;
}

CObject_PlayerNPC::~CObject_PlayerNPC()
{
	m_vMissionData.clear();
}


VOID CObject_PlayerNPC::Initial( VOID *pParam )
{
	CObject_Character::Initial( pParam );

	SetWeaponType( WEAPON_TYPE_NONE );

	Disalbe(OSF_RAY_QUERY);
	m_uStartFaceToPlayerTime	= 0;
	m_bFaceToPlayer				= FALSE;
}

ENUM_NPC_TYPE CObject_PlayerNPC::GetNpcType(VOID)const
{
	return m_eNpcType;
}

VOID CObject_PlayerNPC::SetNpcType(ENUM_NPC_TYPE eType)
{
	m_eNpcType = eType;

	if( m_pRenderInterface )
	{
		if( NPC_TYPE_PET == m_eNpcType )
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PET);
		else
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_CREATURE);
	}
}

VOID CObject_PlayerNPC::CreateRenderInterface(VOID)
{
	if( m_pCreatureInfo != NULL || NPC_TYPE_MOUNT == GetNpcType())
	{
		LPCSTR lpszModelFileName = NULL;
		INT nCharModelID = GetCharModelID();

		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
		const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU( nCharModelID ));
		
		if(pCharModel != NULL)
		{
			lpszModelFileName = pCharModel->m_pszModelName;
		}

		if(lpszModelFileName != NULL)
		{
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
			m_pRenderInterface->SetData(GetID());

			// ����ActorFile
			m_pRenderInterface->Actor_SetFile( lpszModelFileName );
		}
	}

	CObject_Character::CreateRenderInterface();

	// ������
	OnDataChanged_Equip( HEQUIP_MAINHAND );
//	OnDataChanged_Equip( HEQUIP_ASSIHAND );
	// ������
	CObject_Character::OnDataChanged_MountID();

	// ����Ч
	SetNpcEffect();

}

BOOL CObject_PlayerNPC::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;

	if( CanbeSelect() )
	{
		// ����Ʒ
		if(pActiveSkill && pActiveSkill->GetType() == AOT_ITEM)
		{
			// ����Ϲҵ���һ����Ʒ

			// ����Ʒ
			if(!pActiveSkill || pActiveSkill->GetType() != AOT_ITEM) 
				return TRUE;

			CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActiveSkill)->GetItemImpl());
			if(!pItem) 
				return  TRUE;

			// �������ܹ�ʹ�õ���Ʒ
			if(pItem->GetItemClass() != ICLASS_COMITEM) 
				return  TRUE;
			
			// �Ƿ��ܹ�ʹ��
			int objID;
			PET_GUID_t petID;
			bool bCanuseDir = ((CObject_Item_Medicine*)pItem)->IsValidTarget(this, 
				fVector2(-1, -1), objID, petID);

			if(bCanuseDir)
			{
				pOutCmd->m_typeMouse = SCommand_Mouse::MCT_USE_ITEM;
				pOutCmd->m_apParam[0] = (VOID*)pActiveSkill;
				pOutCmd->m_adwParam[1] = GetServerID();
				pOutCmd->m_adwParam[4] = FALSE;
			}
		}
		// ������Ʒ
		else
		{
			ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );

			if(CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CHARACTER_LOGIC_DEAD)
			{
				pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
				return TRUE;
			}

			switch( eCampType )
			{
			case RELATION_FRIEND:	// ����
				if ( CannotBeAttack() )
				{
					pOutCmd->m_typeMouse = SCommand_Mouse::MCT_SPEAK;
					pOutCmd->m_adwParam[0] = GetServerID();
				}
				break;

			case RELATION_ENEMY:	// ����
			default:
				{
					if( pActiveSkill != NULL
						&& pActiveSkill->GetType() == AOT_SKILL
						&& !CannotBeAttack() )
					{
						const SCLIENT_SKILL* pSkillImpl = (const SCLIENT_SKILL*)pActiveSkill->GetImpl();
						if(pSkillImpl != NULL && pSkillImpl->m_pDefine->m_nSelectType == SELECT_TYPE_CHARACTER)
						{
							pOutCmd->m_typeMouse = SCommand_Mouse::MCT_SKILL_OBJ;
							pOutCmd->m_apParam[0] = (VOID*)pActiveSkill;
							pOutCmd->m_adwParam[1] = GetServerID();
						}
					}
				}
				break;

			}
		}
	}

	return TRUE;
}

BOOL CObject_PlayerNPC::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	if ( CanbeSelect() )
	{
		ENUM_NPC_TYPE ot = GetNpcType();
		switch(ot)
		{
		case NPC_TYPE_PET:
			pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_CONTEXMENU;
			pOutCmd->m_adwParam[0]	= GetServerID();
			break;
		default:
			pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_PLAYER_SELECT;
			pOutCmd->m_adwParam[0]	= GetServerID();
			break;
		}
	}
	return TRUE;
}

INT CObject_PlayerNPC::CannotBeAttack( VOID )const
{
	if(m_pCreatureInfo)
		return m_pCreatureInfo->nCannotBeAttack; 
	else 
		return CObject_Character::CannotBeAttack();
}

VOID CObject_PlayerNPC::FaceToPlayer(VOID)
{
	m_bFaceToPlayer = TRUE;
	m_uStartFaceToPlayerTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
	m_fvPreRotation  = GetRotation();
}

VOID CObject_PlayerNPC::StopFaceToPlayer(VOID)
{
	m_bFaceToPlayer = FALSE;
	SetFaceDir(GetPreRotation().y);
}

BOOL CObject_PlayerNPC::IsCanFaceToPlayer(VOID) 
{
	if(m_pCreatureInfo != NULL)
	{
		return (m_pCreatureInfo->nIsCanSwerve != 0)?(TRUE):(FALSE);
	}
	else
	{
		return FALSE;
	}
}

BOOL CObject_PlayerNPC::IsDisplayBoard(VOID) 
{
	if( m_pCreatureInfo != NULL )
	{
		return (m_pCreatureInfo->nIsDisplayerName != 0) ? (TRUE) :(FALSE);
	}
	else
	{
		return FALSE;
	}
}


FLOAT CObject_PlayerNPC::GetBoardHeight(VOID) 
{
	if(m_pCreatureInfo != NULL)
	{
		return m_pCreatureInfo->fBoardHeight;
	}
	else
	{
		return 0;
	}
}

FLOAT CObject_PlayerNPC::GetProjtexRange(VOID) 
{
	if(GetCharacterType() == CT_MONSTER)
	{
		return m_pCreatureInfo ? m_pCreatureInfo->fProjTexRange : 0.0f;
	}
	else return 200.0f;
}

FLOAT CObject_PlayerNPC::GetShadowRange(VOID) 
{
	if(GetCharacterType() == CT_MONSTER)
	{
		return m_pCreatureInfo ? m_pCreatureInfo->fShadowRange : 0.0f;
	}
	else 
		return 100.0f;
}

LPCSTR CObject_PlayerNPC::GetPortrait(VOID) 
{
	if(m_pCreatureInfo != NULL)
	{
		return m_pCreatureInfo->szIconName;
	}
	else
	{
		return NULL;
	}
}

INT CObject_PlayerNPC::CalcCharModelID( VOID )const
{
	// ��������
	if( GetConstCharacterData()->Get_ModelID() != -1 )
	{
		// ֱ�Ӵ�ģ�Ͷ�����ж�ȡģ����
		return GetConstCharacterData()->Get_ModelID();
	}
	else
	{
		if( m_pCreatureInfo != NULL )
			return m_pCreatureInfo->nModelID;
		else
			return INVALID_ID;
	}
}

UINT CObject_PlayerNPC::GetIdleInterval(VOID)const
{
	if(m_pCreatureInfo != NULL && m_pCreatureInfo->nIdleInterval >= 0)
	{
		return m_pCreatureInfo->nIdleInterval * 1000;
	}
	else
	{
		return CObject_Character::GetIdleInterval();
	}
}

VOID CObject_PlayerNPC::UpdateCharRace(VOID)
{
	CCharacterData *pCharacterData = GetCharacterData();
	if( pCharacterData != NULL && pCharacterData->Get_RaceID() != INVALID_ID )
	{
		// �����ﶨ��� mon_attr_ex.tab
		DBC_DEFINEHANDLE(s_pCreatureDBC, DBC_CREATURE_ATT)
		m_pCreatureInfo = (const _DBC_CREATURE_ATT*)(s_pCreatureDBC->Search_Index_EQU(pCharacterData->Get_RaceID()));
		m_bHaveHitAnimation = m_pCreatureInfo->nIsPlayAnimation;
	}
}

VOID CObject_PlayerNPC::OnDataChanged_RaceID( VOID )
{
	CObject_Character::OnDataChanged_RaceID();

	if( m_pCreatureInfo != NULL && m_pCreatureInfo->nCanHandle )
		Enable(OSF_RAY_QUERY);
	else
		Disalbe(OSF_RAY_QUERY);
}

VOID CObject_PlayerNPC::ProcessFaceToPlayer(VOID)
{
	// ���������NPC������������ķ���
	if(GetCharacterType() == CT_MONSTER)
	{
		// �����Լ� Ҳ����������� ֻ����NPC
		BOOL bRet = GetNeedFaceToPlayer();
		if (TRUE == bRet)
		{
			FaceToPlayer();
			SetNeedFaceToPlayer(FALSE);
		}

		if(m_bFaceToPlayer)
		{
			UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
			if(m_uStartFaceToPlayerTime + 10000 < uTimeNow)
			{
				StopFaceToPlayer();
			}
			else
			{
				FLOAT fDir;
				fVector2 fvThis;
				fVector2 fvTarget2;
				// ��ҵ�λ��
				CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
				fvThis.x = pMySelf->GetPosition().x;
				fvThis.y = pMySelf->GetPosition().z;
				// fvTargent2ΪNPC��λ��
				fvTarget2.x = GetPosition().x;
				fvTarget2.y = GetPosition().z;
				// ����NPC�����SetFaceDir,ʹ��ת�����
				fDir = KLU_GetYAngle(fvTarget2, fvThis);
				SetFaceDir( fDir );
			}
		}
	}
}

VOID CObject_PlayerNPC::CharacterLogic_OnStopped(BOOL bFinished, LOGIC_TYPE nLogicTag)
{
	if(CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CHARACTER_LOGIC_IDLE)
	{
		if(m_bFaceToPlayer)
		{
			StopFaceToPlayer();
		}
	}

	CObject_Character::CharacterLogic_OnStopped(bFinished, nLogicTag);
}

BOOL CObject_PlayerNPC::Tick_Idle(UINT uElapseTime)
{
	BOOL bResult = CObject_Character::Tick_Idle(uElapseTime);
	if( !bResult )
		return bResult;

	// ����NPCת������
	ProcessFaceToPlayer();

	return TRUE;
}


VOID CObject_PlayerNPC::OnDataChanged_Equip(HUMAN_EQUIP point)
{
	UpdateEquip(point);
}

VOID CObject_PlayerNPC::UpdateEquip( HUMAN_EQUIP point )
{
	// ֻ�������
	if( HEQUIP_MAINHAND != point )
		return ;

	// ��ȡ��������
	CCharacterData *pCharacterData = GetCharacterData();
	if( pCharacterData == NULL )
		return ;


	// ���ַ���������id��Ҫ�� mon_attr_ex.tab �л�ȡ�� ������������л�ȡ

	// ȡ������id
	INT nID = pCharacterData->Get_MonstWeapon( );

	EquipItem_BodyLocator( nID, WL_BOTH );	// ��������������ã� ֻ�Ǳ���
}


VOID CObject_PlayerNPC::EquipItem_BodyLocator( INT nID, ENUM_WEAPON_LOCATOR_TYPE loc )
{
	// ����ǹֵĻ����ܻ�����������
	if( -1 == nID )
		return ;

	if( !m_pRenderInterface )
		return;

	// ��
	if( CT_MONSTER != GetCharacterType() )
		return ;

	// ��ɫ�ҽ������ݿ�( item_mon_loc.tab )
	DBC_DEFINEHANDLE(s_pWeaponItem, DBC_ITEM_MONSTER_VISUAL_LOCATOR);
	const _DBC_ITEM_MONSTER_VISUAL_LOCATOR* pEquipVisual = (_DBC_ITEM_MONSTER_VISUAL_LOCATOR*)s_pWeaponItem->Search_Index_EQU( nID );
	if( !pEquipVisual )
		return;

	// ������������
	if( (ENUM_WEAPON_TYPE)(pEquipVisual->pWeaponType) >= WEAPON_TYPE_NUMBERS )
		return ;

	SetWeaponType( (ENUM_WEAPON_TYPE)(pEquipVisual->pWeaponType) );


	//================
	//====== �ҽ�����

	// ����
	if( pEquipVisual->pObjFile_Right && pEquipVisual->pObjFile_Right[0] != '\0')
	{
		// ����Ⱦʵ���ϰ�����
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_RIGHTWEAPON, pEquipVisual->pObjFile_Right );

		// ��������Ĭ�ϵ���Ч
		for( int i = 0; i < MONSTER_EFFECT_MAX_NUM; ++i )
		{
			CObject_Character::ChangerWeaponEffect( tEntityNode::WEAP_RIGHT, 
													pEquipVisual->szREffect[i].szEffect, 
													pEquipVisual->szREffect[i].szBindPoint, 
													1 
												  );
		}

		// ������ֳ�������
		m_bRightWeapon = TRUE;
	}
	else
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_RIGHTWEAPON, "" );
	}


	// ����
	if( pEquipVisual->pObjFile_Left && pEquipVisual->pObjFile_Left[0] != '\0' )
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_LEFTWEAPON, pEquipVisual->pObjFile_Left );

		// ��������Ĭ�ϵ���Ч
		for( int i = 0; i < MONSTER_EFFECT_MAX_NUM; ++i )
		{
			CObject_Character::ChangerWeaponEffect( tEntityNode::WEAP_LEFT, 
													pEquipVisual->szLEffect[i].szEffect, 
													pEquipVisual->szLEffect[i].szBindPoint, 
													1 
												  );
		}

		// ������ֳ�������
		m_bLeftWeapon = TRUE;
	}
	else
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_LEFTWEAPON, "" );
	}


	// ���ֶ�
	if( pEquipVisual->pObjShieldFile && pEquipVisual->pObjShieldFile[0] != '\0' )
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_SHIELD_RIGHTWEAPON, pEquipVisual->pObjShieldFile );

		// ��������Ĭ�ϵ���Ч
		for( int i = 0; i < MONSTER_EFFECT_MAX_NUM; ++i )
		{
			CObject_Character::ChangerWeaponEffect( tEntityNode::WEAP_LEFT_SHIELD, 
													pEquipVisual->szLSEffect[i].szEffect, 
													pEquipVisual->szLSEffect[i].szBindPoint, 
													1 
												  );
		}
	}
	else
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_SHIELD_RIGHTWEAPON, "" );
	}


	// ͬʱ���������Ķ���
	// �������Ƕ����������� �����ǣ������־������Ӧ�Ķ����� ���������־Ͷ�Ӧ�����֣� ��û�оͲ������ˣ� ����Ҳ�����Ƕ�����
	if( INVALID_ID != pEquipVisual->nWeaponActor )	// �����ж���
	{
		if( m_bRightWeapon || m_bLeftWeapon )
		{
			UpdateWeaponActionSetFile( pEquipVisual->pActorFile );
		}
	}

}

VOID CObject_PlayerNPC::UnEquipItem( HUMAN_EQUIP nPart )
{
	// �������������ж
	return ;
}


VOID CObject_PlayerNPC::SetNpcEffect()
{
	if( !m_pRenderInterface )
		return ;

	switch (GetNpcType())
	{
		// �����Ч
	case NPC_TYPE_MOUNT:
		{
			
		}		
		break;

		// ����
	default:
		{
			if (NULL == m_pCreatureInfo)
				break;

			/*
			for( int i = 0; i < MONSTER_EFFECT_MAX_NUM; ++i )
			{
				MONSTER_EFFECT me = m_pCreatureInfo->sMonsterEffect[i];
				

				if( (NULL == me.szEffect) || (0 == strcmp(me.szEffect, "")) ||
					(NULL == me.szBind) || (0 == strcmp(me.szBind, "")) )
					continue ;

				if( (NULL == me.szEffect) || (0 == strcmp(me.szEffect, "-1")) ||
					(NULL == me.szBind) || (0 == strcmp(me.szBind, "-1")) )
					continue ;


				m_pRenderInterface->AddEffect( me.szEffect, me.szBind );
			}
			*/
		}
		break;
	}
}
VOID CObject_PlayerNPC::SetNpcMountEffect(INT nMountID, SDATA_PET* pPetData)
{
	if (GetNpcType() == NPC_TYPE_MOUNT)
	{
		if (m_pRenderInterface)
		{
			CreateMountEffect(m_pRenderInterface, nMountID, pPetData);
		}
	}
}


VOID CObject_PlayerNPC::ReleaseNpcEffect()
{
	if( !m_pRenderInterface )
		return ;

	// �������
//	m_pRenderInterface->DelEffect( 0 );

	// ���ȫ��
	m_pRenderInterface->DelAllEffect();
}



VOID CObject_PlayerNPC::SetNpcMissionData()
{
	// ������������
	std::vector< _DBC_QUEST_LOG* >::iterator iter = QuestLogDataMgr::GetMe()->s_FilteredMissions.begin();

	for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++ )
	{
		INT    nRet1  = -1;
		INT    nRet2  = -1;

		// 20100511 ModifyCodeBegin  ����questlog.tab ����������NPC�к��޸�
		nRet1 = strcmp( (*iter)->szNPCName, GetCharacterData()->Get_Name() );
		nRet2 = strcmp( (*iter)->szNPCCommit, GetCharacterData()->Get_Name() );

		if( nRet1 == 0 || nRet2 == 0 ) // �����NPC�ǽ����������NPC
		{
			if ( strcmp( (*iter)->szNPCName, (*iter)->szNPCCommit ) == 0 ) // �����NPC���ǽ�����NPC,���ǽ�����NPC
			{
				AddNpcMission( (*iter)->nMissionID, 3 );
			}
			else
			{
				if ( nRet1 == 0 ) // ����ǽ�����NPC
				{
					AddNpcMission( (*iter)->nMissionID, 1 );
				}
				else if ( nRet2 == 0 ) // ����ǽ�����NPC
				{
					AddNpcMission( (*iter)->nMissionID, 2 );

				}//if ( nRet1 == 0 )

			}//if ( strcmp( (*iter)->szNPCName, (*iter)->szNPCCommit ) == 0 )

		}//if( nRet1 == 0 || nRet2 == 0 )

		// 20100511 ModifyCodeEnd

	}//for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++ )
}

// 20100511 ModifyCodeBegin
VOID CObject_PlayerNPC::AddNpcMission( INT nMissionId, INT nNPCState ) 
{
	NPC_MISSION_DATA data;

	data.nMissionId			= nMissionId;
	data.nMissionNPCState	= nNPCState;

	m_vMissionData.push_back( data );
}
// 20100511 ModifyCodeEnd

VOID CObject_PlayerNPC::DelNpcMission( INT nMissionId )
{
}

VOID CObject_PlayerNPC::SetNpcMissionState( INT nMissionId )
{
	m_bMissStateChange = TRUE;
}


MISSION_STATE CObject_PlayerNPC::GetNpcMissionState()
{
	// �޸��ģ� �������
 	if( !m_bMissStateChange )
	{
//		return m_eMissionState;
	}

	MISSION_STATE msState = MS_INVALID;
	
	const CDetailAttrib_Player* pAttrib = CUIDataPool::GetMe()->GetMySelfDetailAttrib();	

	// 20100510 ModifyCodeBegin
	// npc�����б�
	if( !m_vMissionData.empty() )
	{
		BOOL bRet; // NPC�����Ƿ��пɽ�����

		bRet = CUIDataPool::GetMe()->IsNpcMissionCanAccept( GetCharacterData()->Get_Name() );  

		// �ж�������ϵ���������
 		INT nMissionNum = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission_Num();
		
		// �������û������
		if( nMissionNum == 0 )
		{		
			// �����NPC�����пɽ�����
			if ( bRet == TRUE )
			{
				// ��ʾ��npcΪ���ɽ�����״̬��
				msState = MS_MISS;
				return msState;
			}			
		}
	
		// npc�����б�
		std::vector< NPC_MISSION_DATA >::iterator iter = m_vMissionData.begin();
		for( ; iter != m_vMissionData.end(); iter++ )
		{
			if ( iter->nMissionNPCState == 1 ) // ����ǽ�����NPC����
			{
				continue;
			}

			// �ж���������Ƿ����������
			INT nIndex = pAttrib->GetMissionIndexByID( (*iter).nMissionId );

			// ��
			if( nIndex != UINT_MAX )
			{	
				INT nRet = pAttrib->IsMissionCanCommit( (*iter).nMissionId );
				// �Ƿ��Ѿ����
				if(  nRet == 1 )
				{				
					// ��ʾ��npcΪ���пɽ�����״̬��
					msState = MS_HAVE_MISS_DONE;
					return msState;				
				}
				// �ѽ�δ���
				else if( nRet == 0 )
				{				
					msState = MS_HAVE_MISS_NO_DONE;		
				}

			}//if( nIndex != UINT_MAX )

		}// for( ; iter != m_vMissionData.end(); iter++ )

		// ������ѽ�δ����������msState��ȻΪMS_INVALID,����NPCֻ�����н������ְ��
		if ( msState == MS_HAVE_MISS_NO_DONE || msState == MS_INVALID )
		{
			// ����������ɽ�����
			if ( bRet == TRUE )
			{
				msState = MS_MISS;
			}
		}

	}//if( !m_vMissionData.empty() )
	// 20100510 ModifyCodeEnd
	
	// ����״̬
	m_eMissionState = msState;	

	// ���±��
	m_bMissStateChange = FALSE;

	return msState;
}

