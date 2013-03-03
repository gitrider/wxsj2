
/**	Obj_PlayerOther.cpp
 *	
 *	���ܣ�	
 *	
 *	�޸ļ�¼��
 *			080221	�޸� SetBodyEquipEffect �� �˺����������Ǽ���װ���ϵ���Ч
 *			080221	���� AddEquipEffect �� ��������������ϵ���Ч
 *			
 */

#include "StdAfx.h"
#include "Obj_PlayerOther.h"
#include "Global.h"
#include "Network\NetManager.h"
#include "CGAttack.h"
#include "CGCharAskEquipment.h"
#include "..\..\Manager\ObjectManager.h"
#include "..\Character\Obj_PlayerMySelf.h"
#include "CGOtherEquip.h"
#include "NetWork\NetManager.h"
#include "Procedure\GameProcedure.h"
#include "GITimeSystem.h"
#include "GISound.h"
#include "..\..\ObjectDef.h"
#include "..\..\Manager\ActionSetManager.h"
#include "DBC\GMDataBase.h"
#include "GIDBC_Struct.h"
#include "GIException.h"
#include "..\..\ObjectCommandDef.h"
#include "GameCommand.h"
#include "..\Item\Obj_Item_Equip.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Event\GMEventSystem.h"
#include "GIProfile.h"
#include "AI\GMAI_Base.h"

#include "GIActionSystem.h"
#include "GIGameInterface.h"
#include "DataPool\GMDP_Struct_Skill.h"
#include "DataPool\GMDP_CharacterData.h"
#include "DataPool\GMDataPool.h"
#include "GameStruct_Skill.h"
#include "Obj_PlayerMySelf.h"
#include "Procedure\GameProcedure.h"
#include "SkillDataMgr.h"



KL_IMPLEMENT_DYNAMIC(CObject_PlayerOther, GETCLASS(CObject_PlayerNPC));


CObject_PlayerOther::CObject_PlayerOther()
{
	m_pCharRace		= NULL;
}

CObject_PlayerOther::~CObject_PlayerOther()
{
}


/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::Initial( VOID *pParam )
{
	CObject_PlayerNPC::Initial( pParam );

	// ����
	SetWeaponType( WEAPON_TYPE_NONE );
}

/*--------------------------------------------------
--------------------------------------------------*/
RC_RESULT CObject_PlayerOther::OnCommand(const SCommand_Object *pCmd )
{
	RC_RESULT rcResult = RC_SKIP;

	switch( pCmd->m_wID )
	{
	// ��������װ��
	case OC_UPDATE_EQUIPMENT:
		{
			UINT	dwModifyFlags	= pCmd->m_adwParam[0];
			UINT	*pData			= (UINT*)(pCmd->m_apParam[1]);
			UINT	i, dwIndex;
			dwIndex = 0;
			for ( i = 0; i < HEQUIP_NUMBER; i++ )
			{
				if ( dwModifyFlags & (1<<i) )
				{
					GetCharacterData()->Set_Equip((HUMAN_EQUIP)i, pData[dwIndex]);
					dwIndex++;
				}
			}
			rcResult = RC_OK;
		}
		break;

	// ���¶�����
	case OC_UPDATE_TEAM_FLAG:
		{
			if(this != CObjectManager::GetMe()->GetMySelf())
			{
				CCharacterData* pData = GetCharacterData();
				pData->Set_HaveTeamFlag( pCmd->m_abParam[0] );		// �Ƿ���ӱ��
				pData->Set_TeamLeaderFlag( pCmd->m_abParam[1] );	// �Ƿ��Ƕӳ�
				pData->Set_TeamFullFlag( pCmd->m_abParam[2] );		// ���ӱ��
			}
			rcResult = RC_OK;
		}
		break;

	// ��������б�
	case OC_TEAM_FOLLOW_MEMBER:
		{
			CCharacterData* pData = GetCharacterData();

			pData->Set_TeamFollowListCount( pCmd->m_adwParam[0] );

			for( INT i=0; i<(INT)(pCmd->m_adwParam[0]); ++i )
			{
				pData->Set_TeamFollowedMember(i, pCmd->m_adwParam[i+1]);
			}

			rcResult = RC_OK;
		}
		break;

	// �����Լ��Ķ��������
	case OC_UPDATE_TEAM_FOLLOW_FLAG:
		{
			CCharacterData* pData = GetCharacterData();

			pData->Set_TeamFollowFlag( pCmd->m_abParam[0] );
			CharacterLogic_GetAI()->OnAIStopped();

			rcResult = RC_OK;
		}
		break;

	default:
		return CObject_PlayerNPC::OnCommand(pCmd);
	}

	return rcResult;
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::OnHideWeapon( INT nAppointedWeaponID, ENUM_WEAPON_LOCATOR_TYPE loc )
{
	// ����ָ������
	if( nAppointedWeaponID != INVALID_ID )
	{
		EquipItem_BodyLocator( nAppointedWeaponID, loc );
	}
	else
	{
		// ж��װ��
		if( WL_RIGHT == loc )
			UnEquipItem( HEQUIP_MAINHAND );
		if( WL_LEFT == loc )
			UnEquipItem( HEQUIP_ASSIHAND);
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::OnShowWeapon(VOID)
{
	if( m_bRightWeapon )
		UpdateEquip(HEQUIP_MAINHAND);
	if( m_bLeftWeapon )
		UpdateEquip(HEQUIP_ASSIHAND);
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::CreateRenderInterface(VOID)
{
	if( m_pCharRace != NULL )
	{
		// ģ�Ͷ���
		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)

		LPCSTR lpszModelFileName = NULL;
		INT nModelID = GetCharModelID();

		// char_model.tab
		const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU(nModelID));
		if( pCharModel != NULL )
		{
			lpszModelFileName = pCharModel->m_pszModelName;

			for(int i = 0; i < TERRAIN_TYPE_NUM; ++i)
				m_SoundId[i] = pCharModel->m_nSoundId[i];
		}

		if( lpszModelFileName != NULL )
		{
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
			m_pRenderInterface->SetData(GetID());

			// ����ActorFile
			m_pRenderInterface->Actor_SetFile( lpszModelFileName );

			// ������
			if( GetConstCharacterData()->Get_ModelID() == -1 )
			{
				UpdateFaceMesh();
//				UpdateHairMesh();
//				UpdateHairColor();

				// ����ˢ��װ��

				UpdateEquip( HEQUIP_MAINHAND );		// ���� ��
				UpdateEquip( HEQUIP_ASSIHAND );		// ���� ��

				UpdateEquip( HEQUIP_HEAD );			// ͷ
				UpdateEquip( HEQUIP_BACK );			// ���ɿ���
				UpdateEquip( HEQUIP_SHOULDER );		// ��
				UpdateEquip( HEQUIP_HAND );			// ��
				UpdateEquip( HEQUIP_FEET );			// ��

				UpdateEquip( HEQUIP_NECK );			// �� ����
				UpdateEquip( HEQUIP_WRIST );		// ��
				UpdateEquip( HEQUIP_WAIST );		// ��
			}

			SendEuipRequest();
		}
	}

	CObject_Character::CreateRenderInterface();
}

/*--------------------------------------------------
// �����������ϸ��Ϣ������
--------------------------------------------------*/
VOID CObject_PlayerOther::SendEuipRequest(VOID)
{
	if(GetServerID() == -1) return;

	CGCharAskEquipment msg;
	msg.setObjID((ObjID_t)GetServerID());

	CNetManager::GetMe()->SendPacket(&msg);
}

/*--------------------------------------------------
// ���ָ��ķ���
--------------------------------------------------*/
BOOL CObject_PlayerOther::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;

	// ����Ʒ
	if(pActiveSkill && pActiveSkill->GetType() == AOT_ITEM)
	{
		// ����Ϲҵ���һ����Ʒ
		return CObject_PlayerNPC::FillMouseCommand_Left(pOutCmd, pActiveSkill);
	}
	else
	{
		ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );
		switch ( eCampType )
		{
		case RELATION_FRIEND:	// ����
			{
				break;
			}

		case RELATION_ENEMY:	// ����
		default:
			{
				if( pActiveSkill != NULL && 
					pActiveSkill->GetType() == AOT_SKILL )
				{
					const SCLIENT_SKILL* pSkillImpl = (const SCLIENT_SKILL*)pActiveSkill->GetImpl();
					if( pSkillImpl != NULL && 
						pSkillImpl->m_pDefine->m_nSelectType == SELECT_TYPE_CHARACTER)
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

	return TRUE;
}


/*--------------------------------------------------
// �Ҽ�ָ��ķ���
--------------------------------------------------*/
BOOL CObject_PlayerOther::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );
	switch ( eCampType )
	{
	case RELATION_FRIEND:
		{
			pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_CONTEXMENU;
			pOutCmd->m_adwParam[0]	= GetServerID();
		}
		break;
	case RELATION_ENEMY:
	default:
		{
			pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_PLAYER_SELECT;
			pOutCmd->m_adwParam[0]	= GetServerID();
		}
		break;
	}

	return TRUE;
}


/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::OnDataChanged_Equip(HUMAN_EQUIP point)
{
	UpdateEquip(point);
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::UpdateFaceMesh(VOID)
{
	if(m_pRenderInterface != NULL)
	{
		INT nFaceMeshID = GetCharacterData()->Get_FaceMesh();
		if( nFaceMeshID == INVALID_ID )
		{
			nFaceMeshID = m_pCharRace->nDefFaceGeo;
		}

		if( nFaceMeshID != INVALID_ID )
		{
			// ��ɫͷ��ģ�����ݿ�
			DBC_DEFINEHANDLE(s_pCharHeadGeo, DBC_CHARACTER_HEAD_GEO);	// char_head.tab
			// ������Ӧ��¼
			const _DBC_CHAR_HEAD_GEO* pFaceGeo = (const _DBC_CHAR_HEAD_GEO* )s_pCharHeadGeo->Search_Index_EQU(nFaceMeshID);
			if( pFaceGeo != NULL )
			{
				m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_FACE_MESH, pFaceGeo->pMeshFile);
			}
			else
			{
				m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_FACE_MESH, "");
			}

			// �����UI���壬��������VisibleFlag
			if(GetFakeObjectFlag())
			{
				m_pRenderInterface->Actor_SetUIVisibleFlag();
			}
		}
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::UpdateHairMesh(VOID)
{
	return ;

	if(m_pRenderInterface != NULL)
	{
		INT nHairMesh = GetCharacterData()->Get_HairMesh();
		if(nHairMesh == INVALID_ID)
		{
			nHairMesh = m_pCharRace->nDefHairGeo;
		}

		if(nHairMesh != INVALID_ID)
		{
			// ��ɫͷ��ģ�����ݿ�
			DBC_DEFINEHANDLE(s_pCharHairMesh, DBC_CHARACTER_HAIR_GEO);
			// ������Ӧ��¼
			const _DBC_CHAR_HAIR_GEO* pHairMesh = (const _DBC_CHAR_HAIR_GEO*)s_pCharHairMesh->Search_Index_EQU(nHairMesh);
			if(pHairMesh != NULL)
			{
				m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_HAIR_MESH, pHairMesh->pMeshFile);
			}
			else
			{
				m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_HAIR_MESH, "");
			}

			// �����UI���壬��������VisibleFlag
			if(GetFakeObjectFlag())
			{
				m_pRenderInterface->Actor_SetUIVisibleFlag();
			}
		}
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::UpdateHairColor(VOID)
{
	// ��Ⱦ������
	if(m_pRenderInterface)
	{
		UINT uHairColor = GetCharacterData()->Get_HairColor();
		m_pRenderInterface->Actor_SetHairColor(uHairColor);
	}
}


/*--------------------------------------------------
--------------------------------------------------*/

VOID CObject_PlayerOther::DelEquipEffect(HUMAN_EQUIP nPart)
{
	if (!m_pRenderInterface)
		return;
	
	// ���Ҹò�λӵ�е���Ч
	EquitEffectMap::iterator it = m_EquitEffectMap.find(nPart);
	if (it != m_EquitEffectMap.end())
	{
		// ɾ���ò�λ������Ч
		std::vector<UINT>::iterator effectID = it->second.begin();
		for (; effectID != it->second.end(); ++effectID)
		{
			m_pRenderInterface->DelEffect(*effectID);
		}
		it->second.clear();
	}

}
/*--------------------------------------------------
--------------------------------------------------*/

VOID CObject_PlayerOther::SetBodyEquipEffect( INT nEquipId, HUMAN_EQUIP nPart )
{
	if (!m_pRenderInterface)
		return;


	// �����ݱ�
	DBC_DEFINEHANDLE( s_pItem_EquipVisual, DBC_ITEM_EQUIP_VISUAL );
	// ������¼
	const _DBC_ITEM_EQUIP_VISUAL* pEquipVisual = \
		(const _DBC_ITEM_EQUIP_VISUAL*)s_pItem_EquipVisual->Search_Index_EQU((UINT)nEquipId);
	
	if( !pEquipVisual )
		return ;


	UINT effectID;
	EquitEffectMap::iterator it;

	// ����װ���ϵ���Ч
	for( int i=0; i<EQUIP_EFFECT_MAX_NUM; ++i )
	{
		// ��Ч���Ʋ�Ϊ��
		if (('\0' != pEquipVisual->szEffect[i].szEffect[0]) &&
			('\0' != pEquipVisual->szEffect[i].szBindPoint[0]) )
		{
			// ����Ч
			effectID = m_pRenderInterface->AddEffect( pEquipVisual->szEffect[i].szEffect, pEquipVisual->szEffect[i].szBindPoint );

			// ������Ч���б�
			it = m_EquitEffectMap.find( nPart );
			if (it == m_EquitEffectMap.end())
			{
				std::vector<UINT> effectList;

				effectList.push_back(effectID);

				m_EquitEffectMap.insert( std::make_pair( nPart, effectList ) );
			}
			else
			{
				it->second.push_back(effectID);
			}
		}
	}

}
VOID CObject_PlayerOther::SetBodyEquipEffect( HUMAN_EQUIP nPart, LPCTSTR szBindPoint, LPCTSTR szEffect )
{
	if (!m_pRenderInterface)
		return;

	if( 0 == strcmp(szBindPoint, "") || 0 == strcmp(szEffect, "") )
		return ;

	UINT effectID = m_pRenderInterface->AddEffect( szEffect, szEffect );
	EquitEffectMap::iterator it = m_EquitEffectMap.find( nPart );

	if (it == m_EquitEffectMap.end())
	{
		std::vector<UINT> effectList;

		effectList.push_back(effectID);

		m_EquitEffectMap.insert( std::make_pair( nPart, effectList ) );
	}
	else
	{
		it->second.push_back(effectID);
	}
}


VOID CObject_PlayerOther::SetUseSkillID( SkillID_t id )
{
	INT nRandAnimID = GetSkillDataMgr()->GetRandAnim( id, 0, TRUE);
	CObject_Character::ChangeAction( nRandAnimID, GetLogicSpeed(), FALSE );
	//// �����߼�״̬
	//CharacterLogic_Set( CHARACTER_LOGIC_SKILL_SEND, LOGIC_ACTION );
}


VOID CObject_PlayerOther::AddEquipEffect( INT nEquipId, HUMAN_EQUIP eEquipType )
{
	if( !m_pRenderInterface )
		return ;


	// �����ݱ�
	DBC_DEFINEHANDLE( s_pItem_EquipVisual, DBC_ITEM_EQUIP_VISUAL );
	// ������¼
	const _DBC_ITEM_EQUIP_VISUAL* pEquipVisual = \
		(const _DBC_ITEM_EQUIP_VISUAL*)s_pItem_EquipVisual->Search_Index_EQU((UINT)nEquipId);
	
	if( !pEquipVisual )
		return ;

	switch( eEquipType )
	{
	// ����
	case HEQUIP_MAINHAND:
		m_pRenderInterface->ClearWeaponEffect( tEntityNode::WEAP_RIGHT );

		// ��������Ĭ�ϵ���Ч
		
		for( int i = 0; i < EQUIP_EFFECT_MAX_NUM; ++i )
		{
			CObject_Character::ChangerWeaponEffect( tEntityNode::WEAP_RIGHT, 
													pEquipVisual->szEffect[i].szEffect, 
													pEquipVisual->szEffect[i].szBindPoint, 
													1 
												  );
	

		}
		
		break;
	
	// ����
	case HEQUIP_ASSIHAND:

		tEntityNode::eWEAPATTR eWeapAttr = tEntityNode::WEAP_LEFT;

		// 
		if( 0 == strcmp( pEquipVisual->szType, "��" ) )
		{
			eWeapAttr = tEntityNode::WEAP_LEFT_SHIELD;
		}


		m_pRenderInterface->ClearWeaponEffect( eWeapAttr );

		// ��������Ĭ�ϵ���Ч
		for( int i = 0; i < EQUIP_EFFECT_MAX_NUM; ++i )
		{
			CObject_Character::ChangerWeaponEffect( eWeapAttr, 
													pEquipVisual->szEffect[i].szEffect, 
													pEquipVisual->szEffect[i].szBindPoint, 
													1 
												);
		

		}
		
		break;
	};

}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::UpdateEquip( HUMAN_EQUIP point )
{
	CCharacterData *pCharacterData = GetCharacterData();
	if( pCharacterData == NULL )
		return ;

	// ���ǽ���ģ��
	if( !GetFakeObjectFlag() )
	{
		// ����״̬
		if( pCharacterData->Get_ModelID() != INVALID_ID )
		{
			return ;
		}
	}

	// ����һ����ʱ��Item����
	CObject_Item_Equip *pTempEquip = NULL;
	INT nID = pCharacterData->Get_Equip( point);
	if( nID != INVALID_ID )
	{
		// ͨ��װ����id������
		pTempEquip = (CObject_Item_Equip *)CObject_Item::NewItem( nID );
	}


	// �����װ������װ���װ����Ʒ
	if( pTempEquip != NULL && ICLASS_EQUIP == pTempEquip->GetItemClass() )
	{
		switch( pTempEquip->GetItemType() )
		{
			// �������ҽ���װ��
			//����
			case HEQUIP_MAINHAND:
			{
				// װ�����������֡����֡����ƣ�
				EquipItem_BodyLocator( pTempEquip->GetVisualID(), WL_RIGHT );	// pTempEquip->GetItemType()
				// �����������ͣ� ������������������Ӧ����
				SetWeaponType( pTempEquip->GetWeaponType(), WL_RIGHT );

				AddEquipEffect( nID, point );
			}
			break;
			// ��������, ��
			case HEQUIP_ASSIHAND:    
			{
				if (pTempEquip->IsDefence())
				{
					EquipItem_BodyLocator( pTempEquip->GetVisualID(), WL_L_ARM );
					SetWeaponType( pTempEquip->GetWeaponType(), WL_L_ARM );
				}
				else
				{
					EquipItem_BodyLocator( pTempEquip->GetVisualID(), WL_LEFT );
					SetWeaponType( pTempEquip->GetWeaponType(), WL_LEFT );
				}

				AddEquipEffect( nID, point );
			}
			break;

			// ñ��, ����, �ֱ�, Ь, װ������Ʒ
			case HEQUIP_HEAD:
			case HEQUIP_BACK:				
			case HEQUIP_HAND:				
			case HEQUIP_FEET:
			case HEQUIP_SHOULDER:
			{
				EquipItem_BodyPart(pTempEquip->GetItemType(), pTempEquip->GetVisualID());

				DelEquipEffect( point );
				SetBodyEquipEffect( nID, point );
			}
			break;

			// ����, ����, ����, ��
			case HEQUIP_WRIST:
			case HEQUIP_WAIST:
			case HEQUIP_RING1:				
			case HEQUIP_RING2:
			case HEQUIP_NECK:
			default:
				break;
		}

	}
	else
	{
		// ��������ò�λ����Ч
		DelEquipEffect( point );
		// ж��װ��
		UnEquipItem( point );
	}

	// �����UI���壬��������VisibleFlag
	if( GetFakeObjectFlag() )
	{
		m_pRenderInterface->Actor_SetUIVisibleFlag();
	}


	// ������ʱ��װ��
	CObject_Item::DestroyItem(pTempEquip);
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::EquipItem_BodyPart( HUMAN_EQUIP nPart, INT nID )
{
	// ��Ⱦ���Ƿ���Ч
	if( !m_pRenderInterface )
		return;

	CCharacterData *pCharacterData = GetCharacterData();
	if( !pCharacterData )
		return ;

	// ��ɫװ�����������ݿ�
	DBC_DEFINEHANDLE(s_pEquipItem, DBC_ITEM_VISUAL_CHAR);	// item_char.tab
	const _DBC_ITEM_VISUAL_CHAR* pEquipVisual = (const _DBC_ITEM_VISUAL_CHAR*)s_pEquipItem->Search_Index_EQU( nID );
	if( !pEquipVisual )
	{
		return;
	}


	// �����Ա��ְҵ��ȷ��ģ��

	// ����(�У�Ů)
	INT nRaceID = GetCharacterData()->Get_RaceID()%2;
	KLAssert( nRaceID >= 0 && nRaceID < CHAR_RACE_NUM);

	// ְҵ
	INT nProfession = GetCharacterData()->Get_MenPai();
	if( -1 == nProfession )
		return ;
	if( nProfession > CLIENT_MENPAI_NUM )
	{
		KLAssert( nProfession >= 0 && nProfession < CLIENT_MENPAI_NUM );
	}

	// ְҵ��ģ������
	INT nProfessionIndex = 0;
	if( -1 != nProfession )
//		nProfessionIndex = nProfession * 4;	// 4��ʾ���Ǹ�ְҵ���������
		nProfessionIndex = nProfession * 2;	// 2��ʾ���Ǹ�ְҵ���������

	
	// ��ɫװ�������ݿ�
	DBC_DEFINEHANDLE(s_pEquipLoc, DBC_EQUIP_LOC);	// equip_loc.tab
	const _DBC_EQUIP_LOC* pEquipLoc = (const _DBC_EQUIP_LOC*)s_pEquipLoc->Search_Index_EQU( pEquipVisual->pVisualEntityIndex[ nRaceID + nProfessionIndex ] );
	if( !pEquipLoc )
	{
		return ;
	}

	switch( nPart )
	{
	// ͷ��
	case HEQUIP_HEAD:			
		{
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CAP_MESH, pEquipLoc->pMeshFile );
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CAP_MAT, pEquipLoc->pMatFile );
		}
		break;
	// ��
	case HEQUIP_SHOULDER:
		{
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_SHOULDER_MESH, pEquipLoc->pMeshFile );
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_SHOULDER_MAT, pEquipLoc->pMatFile );
		}
		break;
	// ����
	case HEQUIP_BACK:
		{
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_MAINBODY_MESH, pEquipLoc->pMeshFile );
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_MAINBODY_MAT, pEquipLoc->pMatFile );
		}
		break;
	// ˫��
	case HEQUIP_HAND:
		{
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_ARM_MESH, pEquipLoc->pMeshFile );
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_ARM_MAT, pEquipLoc->pMatFile );
		}
		break;
	// ˫��
	case HEQUIP_FEET:
		{
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_FOOT_MESH, pEquipLoc->pMeshFile );
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_FOOT_MAT, pEquipLoc->pMatFile );
		}
		break;
	};

	for( INT i = 0; i < EQUIP_LOC_EFFECT_MAX_NUM; ++i )
	{
		if( 0 == strcmp(pEquipLoc->szREffect[i].szEffect, "") || 
			0 == strcmp(pEquipLoc->szREffect[i].szBindPoint, "") )
			continue ;

		SetBodyEquipEffect( nPart, pEquipLoc->szREffect[i].szEffect, pEquipLoc->szREffect[i].szBindPoint );
	}

}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::EquipItem_BodyLocator( INT nID, ENUM_WEAPON_LOCATOR_TYPE loc )
{
	if( !m_pRenderInterface )
		return;


	// ��ɫ�ҽ������ݿ�( item_loc.tab )
	DBC_DEFINEHANDLE(s_pWeaponItem, DBC_ITEM_VISUAL_LOCATOR);
	const _DBC_ITEM_VISUAL_LOCATOR* pEquipVisual = (_DBC_ITEM_VISUAL_LOCATOR*)s_pWeaponItem->Search_Index_EQU( nID );
	if( !pEquipVisual )
		return;


	//================
	//====== �ҽ�����

	// ����
	if( WL_RIGHT == loc )
	if( pEquipVisual->pObjFile_Right && pEquipVisual->pObjFile_Right[0] != '\0')
	{
		// ����Ⱦʵ���ϰ�����
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_RIGHTWEAPON, pEquipVisual->pObjFile_Right );

		// ��ʯ����
		INT uGemType = INVALID_UID;
		// ���ǹ֣�ȡ����ʯ����
		if( CT_MONSTER != GetCharacterType() )
		{
			uGemType = GetCharacterData()->Get_EquipGem( HEQUIP_MAINHAND );
		}

		if( INVALID_UID != uGemType )
		{
			DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);	// jew_info.tab
			// ������¼
			const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
			UINT nColor = 0;
			LPCTSTR szColor;

			if( pGem )
			{
				// ��ɫ
				szColor = pGem->szColor;

				INT eff_index = pGem->nEffectIndex;
				eff_index--;	// ���Ŵ�1��ʼ����Ӧ�д�0��ʼ

				// ����߻�����������-1����ʾ����ʾ��ɫ��
				if( szColor[0] != '-' && szColor[1] != '1' && (eff_index >= 0) ) 
				{
					sscanf(szColor,"%8X",&nColor);

					if( pEquipVisual->pEffectName[eff_index] )//&& pEquipVisual->pEffectName[0] != '\0')
					{
						m_pRenderInterface->SetRightWeaponEffect(pEquipVisual->pEffectName[eff_index], GetWeaponLocatorName(LOCATOR_WEAPON_1), nColor);
					}
				}
				else
				{
					m_pRenderInterface->SetRightWeaponEffect('\0', GetWeaponLocatorName(LOCATOR_WEAPON_1), 0);
				}
			}
			else
			{
				m_pRenderInterface->SetRightWeaponEffect('\0', GetWeaponLocatorName(LOCATOR_WEAPON_1), 0);
			}
		}

		// ������ֳ�������
		m_bRightWeapon = TRUE;
	}
	else
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_RIGHTWEAPON, "" );
		KLThrow( _T( " ��������� û���������� ��" ) );
	}


	// ����
	if( WL_LEFT == loc )
	if( pEquipVisual->pObjFile_Left && pEquipVisual->pObjFile_Left[0] != '\0' )
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_LEFTWEAPON, pEquipVisual->pObjFile_Left );

		// ��ʯ����
		UINT uGemType = INVALID_UID;
		// ���ǹ�
		if( CT_MONSTER != GetCharacterType() )
		{
			uGemType = GetCharacterData()->Get_EquipGem( HEQUIP_ASSIHAND );
		}

		if( INVALID_UID != uGemType )
		{
			DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);	// jew_info.tab
			// ������¼
			const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
			UINT nColor = 0;
			LPCTSTR szColor;

			if( pGem )
			{
				szColor = pGem->szColor;
				INT eff_index = pGem->nEffectIndex;
				eff_index--;	// ���Ŵ�1��ʼ����Ӧ�д�0��ʼ

				// ����߻�����������-1����ʾ����ʾ��ɫ��
				if( szColor[0] != '-' && szColor[1] != '1' && (eff_index >= 0) ) 
				{
					sscanf(szColor,"%8X",&nColor);

					if( pEquipVisual->pEffectName[eff_index] )//&& pEquipVisual->pEffectName[0] != '\0')
					{
						m_pRenderInterface->SetLeftWeaponEffect(pEquipVisual->pEffectName[eff_index], GetWeaponLocatorName(LOCATOR_WEAPON_1), nColor);
					}
				}
				else
				{
					m_pRenderInterface->SetLeftWeaponEffect('\0', GetWeaponLocatorName(LOCATOR_WEAPON_1), 0);
				}
			}
			else
			{
				m_pRenderInterface->SetLeftWeaponEffect('\0', GetWeaponLocatorName(LOCATOR_WEAPON_1), 0);
			}
		}

		// ������ֳ�������
		m_bLeftWeapon = TRUE;
	}
	else
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_LEFTWEAPON, "" );
		KLThrow( _T( " ��������� û���������� ��" ) );
	}


	// ���ֶ�
	if( WL_L_ARM == loc )
	{
		if( pEquipVisual->pObjShieldFile && pEquipVisual->pObjShieldFile[0] != '\0' )
		{
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_SHIELD_RIGHTWEAPON, pEquipVisual->pObjShieldFile );
		}
		else
		{
			m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_SHIELD_RIGHTWEAPON, "" );
		}
	}


	// ͬʱ���������Ķ���
	// �������Ƕ����������� �����ǣ������־������Ӧ�Ķ����� ���������־Ͷ�Ӧ�����֣� ��û�оͲ������ˣ� ����Ҳ�����Ƕ�����
	
	// �����ж���
	if( INVALID_ID != pEquipVisual->nWeaponActor )	
	{
		if( m_bRightWeapon || m_bLeftWeapon )
		{
			UpdateWeaponActionSetFile( pEquipVisual->pActorFile );
		}
	}

}

/*--------------------------------------------------
--------------------------------------------------*/

VOID CObject_PlayerOther::UnEquipItem( HUMAN_EQUIP nPart )
{
	if( !m_pRenderInterface )
	{
		return;
	}

	switch( nPart )
	{
	// ���� == ����
	case HEQUIP_MAINHAND:
		// ��Ϊ����״̬
		SetWeaponType( WEAPON_TYPE_NONE, WL_RIGHT );
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_RIGHTWEAPON, "" );
		// �������״��
		m_bRightWeapon = FALSE;
		break;

	// ���� == ����
	case HEQUIP_ASSIHAND:
		SetWeaponType( WEAPON_TYPE_NONE, WL_LEFT );
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_LEFTWEAPON, "" );
		m_bLeftWeapon  = FALSE;

		SetWeaponType(WEAPON_TYPE_NONE, WL_L_ARM );
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_SHIELD_RIGHTWEAPON, "" );
		m_bLeftWeapon  = FALSE;

		break;
	
	//====== ����ȱʡװ��

	// ͷ��
	case HEQUIP_HEAD:      
		EquipItem_BodyPart( HEQUIP_HEAD, m_pCharRace->nDefHairGeo );
		break;
	// �·�
	case HEQUIP_BACK:
		EquipItem_BodyPart(HEQUIP_BACK, m_pCharRace->nDefBody);
		break;
	// ��
	case HEQUIP_SHOULDER:
		EquipItem_BodyPart(HEQUIP_SHOULDER, m_pCharRace->nDefShoulder);
		break;
	// ��
		case HEQUIP_WRIST:
		break;
	// ��
	case HEQUIP_HAND:
		EquipItem_BodyPart(HEQUIP_HAND, m_pCharRace->nDefArm);	
		break;
	// ��
	case HEQUIP_FEET:
		EquipItem_BodyPart(HEQUIP_FEET, m_pCharRace->nDefFoot);		
		break;

	// ����, ����, ����
	case HEQUIP_WAIST:           
	case HEQUIP_RING1:				
	case HEQUIP_RING2:
	case HEQUIP_NECK:
		//Not Care...
		break;

	default:
		break;
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::OnDataChanged_FaceMesh(VOID)
{
	UpdateFaceMesh();
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::OnDataChanged_HairMesh(VOID)
{
	UpdateHairMesh();
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::OnDataChanged_HairColor(VOID)
{
	UpdateHairColor();
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::OnDataChanged_EquipVer(VOID)
{
	// ����װ������
	SendEuipRequest();
}

/*--------------------------------------------------
--------------------------------------------------*/
INT CObject_PlayerOther::CalcCharModelID( VOID )const
{
	// ��������
	if( GetConstCharacterData()->Get_ModelID() != -1 )
	{
		// ֱ�Ӵ�ģ�Ͷ�����ж�ȡģ����
		return GetConstCharacterData()->Get_ModelID();
	}
	else
	{
		if( m_pCharRace != NULL )
		{
			// ͨ���Ա��ְҵ��ȡ��Ӧ��ģ��
			INT nMenpai = GetConstCharacterData()->Get_MenPai();
			if( INVALID_ID < nMenpai && nMenpai < MATTRIBUTE_WUMENPAI )
				return m_pCharRace->nModelID + nMenpai;	
			else 
				return m_pCharRace->nModelID;
		}
		else
			return INVALID_ID;
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
UINT CObject_PlayerOther::GetIdleInterval(VOID)const
{
	if(m_pCharRace != NULL && m_pCharRace->nIdleInterval >= 0)
	{
		return m_pCharRace->nIdleInterval * 1000;
	}
	else
	{
		return CObject_Character::GetIdleInterval();
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::UpdateCharRace(VOID)
{
	m_pCharRace = NULL;

	CCharacterData *pCharacterData = GetCharacterData();

	if(pCharacterData != NULL && pCharacterData->Get_RaceID() != INVALID_ID)
	{
		DBC_DEFINEHANDLE(s_pCharacterRace, DBC_CHARACTER_RACE);	// char_race.tab
		m_pCharRace = (const _DBC_CHAR_RACE*)(s_pCharacterRace->Search_Index_EQU(pCharacterData->Get_RaceID()));
	}
}

/*--------------------------------------------------
--------------------------------------------------*/
VOID CObject_PlayerOther::OnDataChanged_RaceID( VOID )
{
	CObject_Character::OnDataChanged_RaceID();
	Enable(OSF_RAY_QUERY);

	// ֮ǰ���������
	if (INVALID_ID != GetCharacterData()->Get_MountID())
	{
		UpdateMountModel();
	}
}

/*--------------------------------------------------
// ͷ��ı��¼�����
--------------------------------------------------*/

VOID CObject_PlayerOther::OnDataChanged_FaceImage(VOID)			
{
}

VOID CObject_PlayerOther::OnDataChanged_Profession(VOID)
{
}


/** �ı䶯��
*/
BOOL	CObject_PlayerOther::ChangeAction( INT nSetID, BOOL bLoop, ENUM_ACTION_TYPE ePart/*=ACTION_BODY*/, FLOAT fFuseParam/*=-0.03*/ )
{
	return CObject_Character::ChangeAction(nSetID, GetLogicSpeed(), bLoop, ePart, fFuseParam);
}

INT CObject_PlayerOther::GetBusObjID(VOID)const
{
	if(m_pCharacterData != NULL)
	{
		return m_pCharacterData->Get_BusObjID();
	}
	else
	{
		return INVALID_ID;
	}
}