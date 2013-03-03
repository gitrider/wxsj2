
/**	Obj_Character.cpp
 *	
 *	���ܣ�	
 *
 *	�޸ļ�¼��	
 *			080327	�޸� CObject_Character::SetElement_Title �й��ڳƺ����͵Ĳ���
 *			
 *			
 */

#include "StdAfx.h"
#include "AxTrace.h"
#include <math.h>
#include "GIUtil.h"
#include "GISound.h"
#include "GIException.h"
#include "GITimeSystem.h"
#include "Obj_PlayerMySelf.h"
#include "Obj_PlayerOther.h"
#include "SkillDataMgr.h"
#include "..\..\Manager\ObjectManager.h"
#include "..\..\ObjectCommandDef.h"
#include "..\TripperObj\GMTripperObj_ItemBox.h"
#include "..\TripperObj\GMTripperObj_Resource.h"
#include "GameCommand.h"
#include "..\..\ObjectDef.h"
#include "..\..\Manager\ActionSetManager.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "..\..\Common\DirectlyImpactMgr.h"
#include "..\Surface\Obj_Effect.h"
#include "..\Surface\Obj_Bullet.h"
#include "..\Surface\Obj_SkillObj.h"
#include "Global.h"
#include "Procedure\GameProcedure.h"
#include "Event\GMEventSystem.h"
#include "DBC\GMDataBase.h"
#include "Action\GMActionSystem.h"
#include "GIDBC_Struct.h"
#include "Obj_Character.h"
#include "AI\GMAI_Base.h"
#include "AI\GMAI_MySelf.h"
#include "DataPool\GMDP_Struct_Pet.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GIUISystem.h"
#include "DataPool\GMDataPool.h"
#include "GIProfile.h"
#include "AI\GMAICommandDef.h"
#include "NetWork\NetManager.h"
#include "CGCharIdle.h"
#include "CGCharSit.h"
#include "Sound\GMSoundSystem.h"
#include "AxProfile.h"
#include "..\..\GMObjectLoadQueue.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "World\WorldManager.h"
#include "..\..\Manager\ObjectManager.h"
#include "Sound\GMSoundSource.h"
#include "..\..\ObjectCommand.h"
#include "GIGameInterface.h"
#include "Procedure\GamePro_Main.h"
#include "..\ProjTex\Obj_ProjTex.h"
#include "GIDebuger.h"
#include "..\ProjTex\Obj_ProjTex.h"
#include "..\..\Manager\BulletEntityManager.h"
#include "../Bus/Obj_Bus.h"



KL_IMPLEMENT_DYNAMIC(CObject_Character, GETCLASS(CObject_Dynamic));

NPC_AUTOPATH		CObject_Character::m_ObjAutoPath;



CObject_Character::CObject_Character()
{
	m_pTheAI					= NULL;
	m_pInfoBoard				= NULL;
	m_pCharacterData			= NULL;

	m_uTime_LogicEventListTick	= 0;

	m_bJumping					= FALSE;
	
	m_pBaseLogicCommand			= NULL;
	m_pActionLogicCommand		= NULL;

	m_bCharacterBaseLogicStopped	= TRUE;
	m_bCharacterActionLogicStopped	= TRUE;

	m_nCharacterBaseLogic		= CHARACTER_LOGIC_INVALID;
	m_nCharacterActionLogic		= CHARACTER_LOGIC_INVALID;

	m_eChangeAnimLogicType		= LOGIC_BASE;

	m_paramLogic_MagicSend.m_nSaveMagicID			= -1;
	m_paramLogic_MagicSend.m_bBeAttackEffectShowed	= FALSE;
	m_paramLogic_MagicSend.m_nActionIndex			= -1;

	m_bBaseAnimationEnd			= FALSE;
	m_bActionAnimationEnd		= FALSE;

	m_uFightStateTime			= 0;
	m_fLogic_Speed				= 1.f;

	m_pBloodStain				= NULL;

	m_pMountRenderInterface		= NULL;
	m_pCharActionSetFile		= NULL;
	m_pMountActionSetFile		= NULL;
	m_pWeaponActionSetFile		= NULL;
	m_nCharModelID				= INVALID_ID;	
	m_bHideWeapon				= FALSE;


	m_bRightWeapon				= FALSE;
	m_bLeftWeapon				= FALSE;

	m_pWlakSound = 0;
	m_pLifeAbilitySound = 0;

	m_bIsChatMoodPlaying		= FALSE;
	m_bNeedStandUp				= FALSE;
	m_bStanding					= FALSE;

	m_bDropBox_HaveData			= FALSE;
	m_nDropBox_ItemBoxID		= INVALID_ID;
	m_DropBox_OwnerGUID			= INVALID_ID;
	m_posDropBox_CreatePos		= WORLD_POS(-1.f, -1.f);



	m_bDropItem_HaveData		= FALSE;
	m_nDropItem_ItemDropID		= INVALID_ID;
	//m_DropItem_OwnerObjId     = INVALID_ID;
	for (int i = 0 ;i < MAX_TEAM_MEMBER ;++i)
	{
		m_DropItem_OwnerObjId[i] = INVALID_ID;
	}

	m_posDropItem_CreatePos		= WORLD_POS(-1.f, -1.f);

	m_ProtectTime = -1;
	m_bIsCanPick = FALSE;

	for(int i = 0; i < 6; ++i)
	{
		m_SoundId[i] = -1;
	}

	mPlayState					= true;
	mTerrainType				= -1;

	m_bBeatFly					= FALSE;
	m_fBeatFlyDist				= 0;
	m_bTail						= FALSE;

	m_nMoveAnimId				= BASE_ACTION_INVALID;
	m_nIdleAnimId				= BASE_ACTION_INVALID;
	m_bMoveState				= FALSE;

	m_nBaseAnimId				= BASE_ACTION_INVALID;
	m_nActionAnimId				= BASE_ACTION_INVALID;

	m_UseSkillTag				= FALSE;

	m_fDefaultFuse = 0.03f;

	m_bHaveHitAnimation = FALSE;

	// 20100415 AddCodeBegin
	//m_ObjAutoPath.m_strAutoNPCName = "";
	//m_ObjAutoPath.m_isAutoPath = FALSE;
	//m_ObjAutoPath.m_nSceneID = -1;
	// 20100415 AddCodeEnd
}

CObject_Character::~CObject_Character()
{
	KLAssert(m_pCharacterData == NULL);
	KLAssert(m_pRenderInterface == NULL);
	KLAssert(m_pInfoBoard == NULL);
}


VOID CObject_Character::Initial( VOID* pParam )
{
	CObject_Phy::Initial(pParam);

	m_bBaseAnimationEnd			= FALSE;
	m_bActionAnimationEnd		= FALSE;

	m_uFightStateTime	= 0;
	m_uDisplayInfoTimer	= 0;

	SetBaseLogicCommand(NULL);
	SetActionLogicCommand(NULL);
	m_fLogic_Speed				= 1.f;

	SetMapPosition( fVector2( GetPosition().x, GetPosition().z ) );
	
	// �����߼���Ϣ
	m_pCharacterData = CDataPool::GetMe()->CharacterData_Create(this);


	// ���ǽ������
	if( !GetFakeObjectFlag() &&  CGameProcedure::s_pUISystem )
	{
		// ������Ϣ��
		m_pInfoBoard = CGameProcedure::s_pUISystem->CreateCreatureBoard();
		m_pInfoBoard->SetElement_ObjId( this->GetServerID() );
		m_pInfoBoard->SetElement_CountryFlag(-1);
		m_pInfoBoard->SetElement_LeaderFlag(FALSE, 0);	// �ӳ����
		m_pInfoBoard->SetElement_Name("");
		m_pInfoBoard->SetElement_SaleSign(FALSE);
		SetElement_Title("",_TITLE::NO_TITLE);
		
		m_pInfoBoard->Show(FALSE);
	}

	m_uTime_LogicEventListTick	= CGameProcedure::s_pTimeSystem->GetTimeNow();

	Enable(OSF_VISIABLE);
	Disalbe(OSF_OUT_VISUAL_FIELD);

	m_bDropBox_HaveData			= FALSE;
	m_nDropBox_ItemBoxID		= INVALID_ID;
	m_DropBox_OwnerGUID			= INVALID_ID;
	m_posDropBox_CreatePos		= WORLD_POS( -1.f, -1.f );
	m_ProtectTime = -1;
	m_bIsCanPick = FALSE;

	m_fDefaultFuse = 0.03f;

	m_bDropItem_HaveData		= FALSE;
	m_nDropItem_ItemDropID		= INVALID_ID;
//	m_DropItem_OwnerObjId		= INVALID_ID;
	for (int i = 0 ;i < MAX_TEAM_MEMBER ;++i)
	{
		m_DropItem_OwnerObjId[i] = INVALID_ID;
	}
	m_posDropItem_CreatePos		= WORLD_POS(-1.f, -1.f);


	m_bHideWeapon = FALSE;
	UpdateCharRace();
	UpdateCharModel();
	UpdateMountModel();

	SObjectInit *pCharacterInit = (SObjectInit*)pParam;
	if( pCharacterInit != NULL )
	{
		// ״̬��Ϣ
		RemoveAllImpact();
		Start_Idle();
	}
}

VOID CObject_Character::Release(VOID)
{
	if( GetFakeObjectFlag() == FALSE )
	{
		CBulletEntityManager::GetMe()->OnObjectDestroy(GetServerID());
	}

	DeleteObjectCommand(m_pBaseLogicCommand);
	m_pBaseLogicCommand = NULL;
	DeleteObjectCommand(m_pActionLogicCommand);
	m_pActionLogicCommand = NULL;

	CleanupLogicCommandList(LOGIC_BASE);
	CleanupLogicCommandList(LOGIC_ACTION);

	if( !m_mapImpactEffect.empty() )
	{
		RemoveAllImpact();		
	}

	// ��������е���
	if (!m_mapMyMount.empty())
	{
		MapMountData::iterator iter = m_mapMyMount.begin();
		for (; iter!=m_mapMyMount.end(); ++iter)
		{
			MountData& mountdata = (*iter).second;
			ReleaseMountRenderInterface(mountdata.pMountRenderInterface);
		}
		m_mapMyMount.clear();
	}
	m_pMountRenderInterface		= NULL;
	// ɾ���߼���Ϣ
	CDataPool::GetMe()->CharacterData_Destroy(this);
	m_pCharacterData = NULL;

	// �Ӽ��ض��������
	CObjectManager::GetMe()->GetLoadQueue()->ClearTask(GetID());

	ReleaseRenderInterface();


	//// �ͷ�MouseTarget
	//if( m_pBloodStain )
	//{
	//	m_pBloodStain->Destroy();
	//	m_pBloodStain = NULL;
	//}

	if( m_pInfoBoard && CGameProcedure::s_pUISystem )
	{
		m_pInfoBoard->Destroy();
		m_pInfoBoard = NULL;
	}

	m_uTime_LogicEventListTick	= 0;

	// �ͷ�������Դ
	CGameProcedure::s_pSoundSystem->Source_Destroy(m_pWlakSound);
	m_pWlakSound = 0;
	CGameProcedure::s_pSoundSystem->Source_Destroy(m_pLifeAbilitySound);
	m_pLifeAbilitySound = 0;

	Disalbe(OSF_RAY_QUERY);

	m_pCharActionSetFile		= NULL;
	m_pMountActionSetFile		= NULL;
	m_pWeaponActionSetFile		= NULL;
	m_nCharModelID				= INVALID_ID;
	m_uFightStateTime		= 0;

	m_bDropBox_HaveData			= FALSE;
	m_nDropBox_ItemBoxID		= INVALID_ID;
	m_DropBox_OwnerGUID			= INVALID_ID;
	m_posDropBox_CreatePos		= WORLD_POS(-1.f, -1.f);
	m_ProtectTime = -1;
	m_bIsCanPick = FALSE;

	m_bDropItem_HaveData		= FALSE;
	m_nDropItem_ItemDropID		= INVALID_ID;
    //m_DropItem_OwnerObjId		= INVALID_ID;

	for (int i = 0 ;i < MAX_TEAM_MEMBER ;++i)
	{
		m_DropItem_OwnerObjId[i] = INVALID_ID;
	}
	m_posDropItem_CreatePos		= WORLD_POS(-1.f, -1.f);

	CObject_Dynamic::Release();
}

VOID CObject_Character::CreateRenderInterface(VOID)
{
	if(!m_pRenderInterface) 
		return;

	// ����Ⱦ��ˢ��λ��
	SetPosition(GetPosition());
	Enable(OSF_VISIABLE);

	// �����Ƿ���FakeObject��һ������
	if(GetFakeObjectFlag())
	{
		//�ɼ����Լ���,Ĭ�ϲ��ɼ�
		m_pRenderInterface->SetVisible(FALSE);
		// ��ֹ���ѡ��
		m_pRenderInterface->SetRayQuery(FALSE); 
		// ���������VisibleFlag
		m_pRenderInterface->Actor_SetUIVisibleFlag();
	}
	else
	{
		m_pRenderInterface->Attach_ProjTexture( tEntityNode::SHADOW_BLOB, TRUE, GetShadowRange(), 2.0f );


		// ����ѡ�����ȼ�������Խ�����ȼ���Խ�ߣ�

		// �Լ�
		if(CT_PLAYERMYSELF == GetCharacterType()) 
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLAYERMYSLEF);	// 6
		// �������
		else if(CT_PLAYEROTHER == GetCharacterType()) 
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLAYEROTHER);		// 5
		// ����
		else if(NPC_TYPE_PET == ((CObject_PlayerNPC*)this)->GetNpcType())
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PET);				// 7
		// ������Npc���֣�
		else
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_CREATURE);		// 3
	}

	// ���ù��������ص�
	m_pRenderInterface->SetAnimationEndEvent( (FUNC_ONANIMATIONENDEX)(CObject_Character::_OnAnimationEnd), (DWORD)GetID(), 0 );
	m_pRenderInterface->SetAnimationCanBreakEvent( (FUNC_ONSKILLBREAKTIME)(CObject_Character::_OnAnimationCanBreak), (DWORD)GetID() );
	m_pRenderInterface->SetAnimationHitEvent( (FUNC_ONSKILLHITTIME)(CObject_Character::_OnAnimationHit), (DWORD)GetID() );
	m_pRenderInterface->SetAnimationShakeEvent( (FUNC_ONANIMATIONEND)(CObject_Character::_OnAnimationShakeEvent), (DWORD)GetID() );
}

VOID CObject_Character::ReleaseRenderInterface( VOID )
{
	if( m_pRenderInterface != NULL )
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;
	}
}

tEntityNode* CObject_Character::CreateMountRenderInterface(INT nMountID)
{
	if( nMountID != INVALID_ID )
	{
		// �������
		DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
		const _DBC_CHARACTER_MOUNT *pMount = (const _DBC_CHARACTER_MOUNT*)( s_pCharMountDBC->Search_Index_EQU(nMountID) );
		if (NULL == pMount)
			return NULL;

		// ��ȡģ�����ñ�
		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
		const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU( pMount->m_nModelID ));
		if (NULL == pCharModel)
			return NULL;

		// ��������ģ��
		if( strlen(pCharModel->m_pszModelName) > 0 )
		{
			// ����
			for (INT i=0; i<MAX_SOUND_NUMBER; ++i)
			{
				m_SoundId[i] = pCharModel->m_nSoundId[i];
			}

			tEntityNode* pMountRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
			pMountRenderInterface->SetData(GetID());

			//���ö����ļ�
			pMountRenderInterface->Actor_SetFile( pCharModel->m_pszModelName );

			pMountRenderInterface->RayQuery_SetLevel( tEntityNode::RL_PLAYERMYSLEF );

			// �����ϰ�����Ч
			//INT nIndex = CDataPool::GetMe()->Pet_GetPetByGuid( GetCharacterData()->Get_CurrentPetGUID() );
			//SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(nIndex);
			CreateMountEffect(pMountRenderInterface, nMountID/*, pPetData*/);
			
			return pMountRenderInterface;
		}
	}
	return NULL;
}


VOID CObject_Character::CreateMountEffect(tEntityNode* pMountInterface, INT nMountID, const SDATA_PET* pPetData)
{
	if (NULL == pMountInterface)
		return;	

	// �������
	DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
		const _DBC_CHARACTER_MOUNT *pMount = (const _DBC_CHARACTER_MOUNT*)( s_pCharMountDBC->Search_Index_EQU(nMountID) );

	if (NULL == pMount)
		return;

	// �����ǰ������Ч
	pMountInterface->DelAllEffect();

	// ����Ĭ����Ч
	for (INT i=0; i<MOUNT_EFFECT_MAX_NUM; ++i)
	{
		if (strlen(pMount->m_EffectInfo[i].pszEffectLocator) > 0 && strlen(pMount->m_EffectInfo[i].pszEffectName) > 0)
		{
			pMountInterface->AddEffect(pMount->m_EffectInfo[i].pszEffectName, pMount->m_EffectInfo[i].pszEffectLocator);
		}
	}

	// ����������Ч
	if (pPetData)
	{
		// ............
	}
}


VOID CObject_Character::ReleaseMountRenderInterface(tEntityNode* mountInterface)
{
	if(GetFakeObjectFlag())
	{
		CGameProcedure::s_pGfxSystem->FakeObj_SetAttach(this->m_pRenderInterface, 0);
	}
	if (mountInterface)
	{
		mountInterface->Detach_Object( m_pRenderInterface );
		mountInterface->DelAllEffect();
		mountInterface->Destroy();
		mountInterface = NULL;
	}
}

VOID CObject_Character::CreateNewMount(INT nMountID)
{
	// ����� char_mount.tab
	DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
	const _DBC_CHARACTER_MOUNT *pCharMount = (const _DBC_CHARACTER_MOUNT*)( s_pCharMountDBC->Search_Index_EQU(nMountID) );

	if (pCharMount)
	{
		MountData mountData;

		mountData.nMountID				= nMountID;
		mountData.nMountModelID			= pCharMount->m_nModelID;
		mountData.fMountAddHeight		= pCharMount->m_fAddHeight;
		mountData.pMountRenderInterface	= CreateMountRenderInterface(nMountID);

		m_mapMyMount.insert(std::make_pair(nMountID, mountData));
	}
}

VOID CObject_Character::SetMountVisible(tEntityNode* mountInterface, BOOL bShow)
{
	if (mountInterface)
	{
		// �ɼ���
		mountInterface->SetVisible(bShow);
		// ����ܷ�ѡ��
		mountInterface->SetRayQuery(bShow);
	}
}

VOID CObject_Character::Mounting(tEntityNode* mountInterface)
{
	if (mountInterface)
	{
		// ������
		Dismount();

		mountInterface->Attach_Object( m_pRenderInterface, GetMountLocatorName(LOCATOR_MOUNT_BACK) );
		SetMountVisible(mountInterface, TRUE);
		m_pMountRenderInterface = mountInterface;
	}
}
VOID CObject_Character::Dismount()
{
	if (m_pMountRenderInterface)
	{
		// ���ɼ�
		SetMountVisible(m_pMountRenderInterface, FALSE);
		// ȡ���
		m_pMountRenderInterface->Detach_Object( m_pRenderInterface );

		m_pMountRenderInterface = NULL;
	}

	/*MapMountData::iterator iter = m_mapMyMount.begin();
	for (; iter!=m_mapMyMount.end(); ++iter)
	{
		tEntityNode* pMountInterface = (*iter)->second.pMountRenderInterface;

		SetMountVisible(pMountInterface, FALSE);
		pMountInterface->Detach_Object( m_pRenderInterface );
		
	}
	m_pMountRenderInterface = NULL;*/	
}


VOID CObject_Character::OnDataChanged_MountEquip( HORSE_EQUIP point )
{
	UpdateMountEquip( point );
}

VOID CObject_Character::UpdateMountEquip( HORSE_EQUIP point )
{
	//// ���ȱ�֤�����ʵ���Ѿ�������
	//if( NULL == m_pMountRenderInterface )
	//	return ;	

	//CCharacterData *pCharacterData = GetCharacterData();
	//if( NULL == pCharacterData )
	//	return ;

	//INT id = pCharacterData->Get_MountEquip( point );


	//switch( point )
	//{
	//// ����
	//case HEQUIP_HORSE_FACE:
	//	break;
	//// ����
	//case HEQUIP_HORSE_BODY:
	//	break;
	//// ��Ȧ
	//case HEQUIP_HORSE_NECK:
	//	break;
	//// ��
	//case HEQUIP_HORSE_BACK:
	//	break;
	//// ����
	//case HEQUIP_HORSE_HALTER:
	//	break;
	//// ����
	//case HEQUIP_HORSE_BOOT:
	//	break;
	//};

/*
	// ������װ�����ݿ���ȡ����
	DBC_DEFINEHANDLE(s_pEquipItem, DBC_ITEM_VISUAL_CHAR);
	Const _DBC_ITEM_VISUAL_CHAR* pEquipVisual = (const _DBC_ITEM_VISUAL_CHAR*)s_pEquipItem->Search_Index_EQU(nID);
	if(!pEquipVisual)
		return;

	// ����ָ���Ĳ���
	switch( nPart )
	{
	case :
		{
		m_pMountRenderInterface ->Actor_SetObjectProperty(szIDSTRING_CAP_MESH, pEquipVisual->pVisualEntity[nRaceID*2+0]);
		m_pMountRenderInterface ->Actor_SetObjectProperty(szIDSTRING_CAP_MAT, pEquipVisual->pVisualEntity[nRaceID*2+1]);	// ���ֵͨ���ж����壨�С�Ů���������
											// ����������һ�� ��� �����Ϳ��Խ�����Ĳ���Ҳ�ŵ����ﲿ����
											// �����и����⣺����Ĳ���Ҫ���������ͳһƥ������

		// ���õ� CObjEntityActor::Actor_SetObjectProperty�У� ��󴫸�Engine �е�changeAttribute
		// LogicModel::changeAttribute ����mAttribMap������obj��ע���������

//		�߻�Ҫ���������obj�ļ�����д��
//		<Attributes>
//			<Attribute name="LeftWeaponObj" value="" hints="SlotCreate"/>
//			<Attribute name="RightWeaponObj" value="" hints="SlotCreate"/>
//			<Attribute name="LeftShieldObj" value="" hints="SlotCreate"/>
//		</Attributes>
//		֮��Ĳ���
//		<EntityList> �� <MaterialList> ����ҲҪ��
		}
		break;
	};	// switch
*/
}
VOID CObject_Character::UnMountEquip( HORSE_EQUIP point )
{
	// �ο� VOID CObject_PlayerOther::UnEquipItem(HUMAN_EQUIP nPart) ��ʵ��

	if( NULL == m_pMountRenderInterface )
		return;


	switch( point )
	{
	// ����
	case HEQUIP_HORSE_FACE:
		break;
	// ����
	case HEQUIP_HORSE_BODY:
		break;
	// ��Ȧ
	case HEQUIP_HORSE_NECK:
		break;
	// ��
	case HEQUIP_HORSE_BACK:
		break;
	// ����
	case HEQUIP_HORSE_HALTER:
		break;
	// ����
	case HEQUIP_HORSE_BOOT:
		break;
	};


	// ����Ӧ�Ĳ�����Ϊ��
/*
	switch( nPart )
	{
	case HEQUIP_WEAPON:
		m_pMountRenderInterface->Actor_SetObjectProperty(szIDSTRING_CURRENT_LEFTWEAPON, "");
		m_pMountRenderInterface->Actor_SetObjectProperty(szIDSTRING_CURRENT_RIGHTWEAPON, "");
		break;

	case HEQUIP_CAP:
		m_pMountRenderInterface->Actor_SetObjectProperty(szIDSTRING_CAP_MESH, "");
		m_pMountRenderInterface->Actor_SetObjectProperty(szIDSTRING_CAP_MAT, "");
		break;
	};	// switch
*/
}

VOID CObject_Character::MouseHoverState( BOOL bState ) const
{
	if( NULL != m_pMountRenderInterface )
	{
		m_pMountRenderInterface->Actor_SetMouseHover( bState );
	}
}

VOID CObject_Character::StopCurrentAction()
{
	if( NULL == m_pRenderInterface )
		return;

	m_pRenderInterface->StopCurrentAction();
}

BOOL CObject_Character::ChangeAction(INT nSetID, FLOAT fSpeed, BOOL bLoop, ENUM_ACTION_TYPE ePart, FLOAT fFuseParam )
{
	BOOL bRet = FALSE;
	
	// �����ʵ�������ھ�û��Ҫȥ������������
	if( NULL == m_pRenderInterface )
		return FALSE;

	if( LOGIC_BASE == m_eChangeAnimLogicType )
	{
		SetBaseAnimationEnd(FALSE);
		m_nBaseAnimId = nSetID;
	}
	else
	{
		SetActionAnimationEnd(FALSE);
		m_nActionAnimId = nSetID;
	}

	//�����Ĭ�ϵĻ�,��ôȡĬ��ֵ
	if ( fFuseParam < 0)
	{
		fFuseParam = GetDefaultFUseParam();
	}


	BOOL bMustDoCharAction, bMustDoMountAction;
	(m_pRenderInterface		 != NULL) ? (bMustDoCharAction  = TRUE) : (bMustDoCharAction  = FALSE);
	(m_pMountRenderInterface != NULL) ? (bMustDoMountAction = TRUE) : (bMustDoMountAction = FALSE);

	BOOL bHideWeapon = FALSE;
	INT nAppointedWeaponID = INVALID_ID;
	

	// ��ȡ����Ķ���
	LPCSTR lpszCharActionName = NULL;
	lpszCharActionName = GetCharActionNameByActionSetID( nSetID, GetWeaponType( WL_RIGHT ), &bHideWeapon, &nAppointedWeaponID );
	// ���ﷵ��˵�� ûȡ��ֵ �� ��Ӧ�ı���û����ֵ
	if( (lpszCharActionName == NULL) || (0 == strcmp(lpszCharActionName, "")) )
		return FALSE;	

/*
	if( CT_PLAYERMYSELF != GetCharacterType() )
	{
		CHAR szDamage[32];
		memset(szDamage, 0, sizeof(szDamage));

		fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
		fVector3 fvCurPos = GetPosition();

		_sntprintf(szDamage, 32, "%s", lpszCharActionName);
		CGameProcedure::s_pUISystem->AddNewBeHitBoard(FALSE, szDamage, fvPos.x, fvPos.y, 10, 0);

//		ADDTALKMSG( "�����ı� ������������" );
//		ADD_TEAM_MSG( "�����ı� ������������" );

//		STRING strTemp = NOCOLORMSGFUNC("GMDP_Struct_Skill_Info_Invalidate_Target");
//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, lpszCharActionName);	// strTemp.c_str()

	}
*/


	LPCSTR lpszMountActionName = NULL;

	// ����Ķ���
	if( m_pMountRenderInterface != NULL )
	{
		// ֻ�ж��ƶ���ֹͣ����״̬
		if( TRUE == m_bMoveState )
		{
			nSetID = m_nMoveAnimId;	// �ƶ�
		}
		else
		{	
			nSetID = m_nIdleAnimId;	// ����
		}


		lpszMountActionName = GetMountActionNameByActionSetID( nSetID, GetWeaponType( WL_RIGHT ) );
		// ����Ķ��������Ϸ�
		if( (lpszMountActionName == NULL) ||  (0 == strcmp(lpszMountActionName, "")) )
		{
			// �ж�һ���������������Ķ����Ƿ�Ϸ��� ������ʾ
			if( lpszCharActionName )
				KLThrow( _T( "����������޶�����" ) );
		}
	}


	//====== ���������� ���ݶ������еĲ�����ȷ��״̬
	if( bHideWeapon && !m_bHideWeapon )
	{
		m_bHideWeapon = bHideWeapon;
		// ��������
		// ͨ��ָ��������id��ʵ�֣�-1Ϊ���أ� ��-1����Ϊָ��������
		OnHideWeapon( nAppointedWeaponID, WL_LEFT );
	}
	else if( !bHideWeapon && m_bHideWeapon )
	{
		m_bHideWeapon = bHideWeapon;
		OnShowWeapon();
	}

	
	//====== ͨ�����ܽӿ���������

	// ����Ķ���, �����������ͻ�ȡ�߻�����õĶ�Ӧ�ַ���
	LPCSTR strWeaponName = GetWeaponName( GetWeaponType( WL_RIGHT ) );
	if (m_pRenderInterface->Actor_EnterSkill(FALSE, lpszCharActionName, bLoop, fFuseParam, (INT)ePart, 0, (INT)m_eChangeAnimLogicType))
		bRet = TRUE;

	// ����Ķ���
	if( m_pMountRenderInterface != NULL )
	{
		// ����ֱ���ж�״̬���ã� ֻ��ֹͣ���ƶ�����״̬
		m_pMountRenderInterface->Actor_EnterSkill(FALSE, lpszMountActionName, bLoop, fFuseParam, (INT)ePart, 1);
	}

	// �����Ķ���
	if( !m_bHideWeapon && (NULL != m_pWeaponActionSetFile) )	// && �������Ƿ��ж����� �ӱ���ȡ		(NULL != m_pRenderInterface)
	{
		// ��������
		tEntityNode::eWEAPATTR wat = tEntityNode::WEAP_NONE;

		// ������������ֻ������������������ �����ֻ��������
		if( m_bRightWeapon )
			wat = tEntityNode::WEAP_RIGHT;
		else if( m_bLeftWeapon )
			wat = tEntityNode::WEAP_LEFT;

		// ��ȡ����������
		LPCSTR lpszWeaponActionName = NULL;
		lpszWeaponActionName = GetWeaponActionNameByActionSetID( nSetID, GetWeaponType( WL_RIGHT ) );

		// �ı���������
		if( (lpszWeaponActionName != NULL) && (0 != strcmp(lpszWeaponActionName, "")) )
			m_pRenderInterface->ChangeWeaponAction( wat, lpszWeaponActionName, bLoop, fFuseParam );
	}

	// ����������Ч
	// ���֡����֡�����
	if( !m_bHideWeapon && ( m_bRightWeapon || m_bLeftWeapon ) )
	{
		// ����
//		if( m_bRightWeapon )
//			ChangerWeaponEffect( tEntityNode::WEAP_RIGHT, "wq_danduan_mo01", "��1", 1 );
		// ����
//		if( m_bRightWeapon )
//			ChangerWeaponEffect( tEntityNode::WEAP_LEFT, "wq_danduan_mo01", "��1", 1 );
		// ����
//		if( m_bRightWeapon )
//			ChangerWeaponEffect( tEntityNode::WEAP_SHIELD );
	}


	//====== �ı��ٶ�
	ChangeActionSpeed( fSpeed );

	return bRet;
}


VOID CObject_Character::ChangeActionSpeed(FLOAT fSpeed)
{
	// ����
	if( m_pRenderInterface )
	{
		m_pRenderInterface->Actor_ChangeActionRate( fSpeed );
	}

	// ����
	if( m_pMountRenderInterface )
	{
		m_pMountRenderInterface->Actor_ChangeActionRate( fSpeed );
	}

	// ����
	if( m_pWeaponActionSetFile )
	{
		tEntityNode::eWEAPATTR wat = tEntityNode::WEAP_NONE;

		if( m_bRightWeapon )
		{
			wat = tEntityNode::WEAP_RIGHT;
		}
		else if( m_bLeftWeapon )
		{
			wat = tEntityNode::WEAP_LEFT;
		}

		m_pRenderInterface->ChangeWeaponActionSpeed( wat, fSpeed );
	}
}

VOID CObject_Character::UpdateTransparent(VOID)
{
	if(GetCharacterData()->Get_StealthLevel() > 0)
	{
		SetTransparent(0.6f);
	}
	else
	{
		SetTransparent(0.f);
	}
}

VOID CObject_Character::SetTransparent(FLOAT fTransparency, FLOAT fTime/* = 1.f*/)
{
	if( GetFakeObjectFlag() )
		return;

	if( m_pRenderInterface )
		m_pRenderInterface->Actor_SetTransparent(fTransparency, fTime);

	if( m_pMountRenderInterface )
		m_pMountRenderInterface->Actor_SetTransparent(fTransparency, fTime);
}


VOID CObject_Character::OnDataChanged_RaceID( VOID )
{
	UpdateCharRace();
	UpdateCharModel();

	// ����
	if( GetCharacterType() == CT_MONSTER )
	{
		SetTransparent( 1.0f, 0.f );
		SetTransparent( 0.0f, 1.f );
	}
}

VOID CObject_Character::OnDataChanged_ModelID(VOID)
{
	UpdateCharModel();
}

VOID CObject_Character::OnDataChanged_MountID( VOID )
{
	UpdateMountModel();
}

// ����ӵ�е�������� [7/14/2010 �¾���]
VOID CObject_Character::OnDataChanged_Mount( VOID )
{
	UpdateMountList();
}

VOID CObject_Character::OnDataChanged_Name(VOID)
{
//	DBC_DEFINEHANDLE( s_pSystemColorDBC, DBC_SYSTEM_COLOR );

	if( m_pInfoBoard ) 
	{
		// ���ö������Ƽ�������ɫ
		m_pInfoBoard->SetElement_Name( GetNameWithColor().c_str() );
		Tick_UpdateInfoBoard();
	}

	// �����Npc, ��������������
	if( GetCharacterType() == CT_MONSTER && ( (CObject_PlayerNPC*)this )->CannotBeAttack() )
	{
		( (CObject_PlayerNPC*)this )->SetNpcMissionData();
	}
}

VOID CObject_Character::OnDataChanged_Profession(VOID)
{
}


STRING CObject_Character::GetNameWithColor(void)
{
	if(strlen(GetCharacterData()->Get_Name()) == 0)
	{
		return "";
	}

	DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);	// sys_color.tab
	const _DBC_SYSTEM_COLOR* pNameColor = 0;

	// ����Լ�
	if(CObjectManager::GetMe()->GetMySelf() == this)
	{
		pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_PLAYER_MYSELF);
	}
	// �������
	else if(GetCharacterType() == CT_PLAYEROTHER)
	{
		pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_PLAYER_OTHER);
	}
	//npc
	else if(GetCharacterType() == CT_MONSTER)
	{
		//�Ƿ񲻿ɹ���
		bool bCantBeAttack = ((CObject_PlayerNPC*)this)->GetCreatureAtt() && ((((CObject_PlayerNPC*)this)->GetCreatureAtt()->nCannotBeAttack) == 1);

		// ��Ӫ
		ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType(CObjectManager::GetMe()->GetMySelf(), this );

		// ��ƽ
		if(bCantBeAttack || eCampType == RELATION_FRIEND)
		{
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_NPC_PEACE);
		}
		else
		{
			// ��������
			if(GetCharacterData()->Get_AIType() == NPC_AI_TYPE_SCANNPC)
			{
				// 20100513 ModifyCodeBegin

				//pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_NPC_ATTACK_ACTIVE);
				
				// �͹����м�����������ɫ����
				INT playerLevel = m_pCharacterData->Get_Level();
				INT selfLevel	= CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
				INT nLevelDiff  = playerLevel - selfLevel; 

				// ���ݵȼ���࣬ ��ʾ��������ɫ
				if( nLevelDiff >= 20 ) // ���
				{
					pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_DIFF_1);
				}
				else if ( 20 > nLevelDiff && nLevelDiff >= 5 )  // ��  5 - 20
				{
					pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_DIFF_2);
				}
				else if ( -5 < nLevelDiff && nLevelDiff < 5 )   // ��   -5 - 5
				{
					pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_DIFF_3);
				}
				else if ( -20 < nLevelDiff && nLevelDiff <= -5 ) // �� -20 - -5
				{
					pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_DIFF_4);
				}
				else if ( nLevelDiff <= -20 ) // �� -20 - -~
				{
					pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_DIFF_5);
				}
			}
			// ����������
			else if(GetCharacterData()->Get_AIType() == NPC_AI_TYPE_NOTSCANNPC)
			{
				pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NAME_NPC_ATTACK_PASS);
			}
			
			// 20100513 ModifyCodeEnd
		}

		//// ��ѯ�ǲ����Լ��ĳ���Լ��ĳ����е�����һ����ɫ����
		//SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID());

		//if(pPet)
		//{
		//	if(this->GetServerID() == pPet->m_idServer)
		//	{
		//		pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NANE_SELF_PET);
		//	}
		//}
	}

	STRING strColorName;

	// 20100516 ModifyCodeBegin
	if(pNameColor)
	{
		// ������ɫ
		if(strlen(pNameColor->strFontColor) == 6)
		{
			strColorName += "#c";
			strColorName += pNameColor->strFontColor;
		}
		else
			strColorName = "#W"; // ������ɫ����RGBֵ��ֵ��ɫ

		// ������ɫ
		if(strlen(pNameColor->strExtenColor) == 6)
		{
			strColorName += "#e";
			strColorName += pNameColor->strExtenColor;
		}
	}
	else
		strColorName = "#W"; // û��ȡ������ɫ��ֵ��ɫ

	// 20100513 ModifyCodeEnd

	// ��������
	strColorName += GetCharacterData()->Get_Name();
	
	return strColorName;
}

VOID CObject_Character::OnDataChanged_MoveSpeed( VOID )
{
	FLOAT fSpeedRate = GetCharacterData()->Get_SpeedRate();

	if( CHARACTER_LOGIC_MOVE == CharacterLogic_Get(LOGIC_BASE) )
	{
		//ChangeActionSpeed( fSpeedRate );
	}
}

VOID CObject_Character::OnDataChanged_CurTitles( INT nType )
{
	if( !m_pInfoBoard ) 
		return;

	LPCTSTR szTitle = m_pCharacterData->Get_CurTitle( nType );
	if( NULL == szTitle || strlen(szTitle) == 0 )
	{
		SetElement_Title( "",_TITLE::NO_TITLE );
		return;
	}

//	INT nTitleType = m_pCharacterData->Get_CurTitleType();
	SetElement_Title( szTitle, nType );

	// ���¿��ܿ��ŵĺ��ѽ���
	CEventSystem::GetMe()->PushEvent(GE_UPDATE_FRIEND);
}

// ���óƺ�
VOID CObject_Character::SetElement_Title( LPCTSTR szTitle, INT nType )
{
//	KLAssert(szTitle);

	if( strlen(szTitle) == 0 )
	{
		m_pInfoBoard->SetElement_Title("", nType );
		return;
	}


	// ��ɫ��
	DBC_DEFINEHANDLE( s_pSystemColorDBC, DBC_SYSTEM_COLOR );	// sys_color.tab


	const _DBC_SYSTEM_COLOR* pTitleColor = 0;

	if(nType == _TITLE::BANGPAI_TITLE)			// ���ɳƺ�
	{
		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_BANGPAI);
	}
//	else if(nType == _TITLE::NICK_TITLE)		// ����Լ�����ĳƺ�
//	{
//		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_NORMAL);
//	}
//	else if(nType == _TITLE::MOOD_TITLE)		// �������
//	{
//		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_MOOD);
//	}
//	else if(nType == _TITLE::SHANGDIAN_TITLE)	// ����̵�ƺ�
//	{
//		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_PLAYER_SHOP);
//	}
	else
	{
		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_NORMAL);
	}

	STRING strColorTitle;

	if( pTitleColor )
	{
		// ������ɫ
		if(strlen(pTitleColor->strFontColor) == 6)
		{
			strColorTitle += "#c";
			strColorTitle += pTitleColor->strFontColor;
		}

		// ������ɫ
		if(strlen(pTitleColor->strExtenColor) == 6)
		{
			strColorTitle += "#e";
			strColorTitle += pTitleColor->strExtenColor;
		}
	}

	strColorTitle += szTitle;

	m_pInfoBoard->SetElement_Title( strColorTitle.c_str(), nType );

}

// ����̯״̬�����ı�
VOID CObject_Character::OnDataChanged_IsInStall(VOID)
{
	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
		return;

	BOOL bIsInStall = pCharacterData->Get_IsInStall();

	if(m_pInfoBoard) 
	{
		m_pInfoBoard->SetElement_SaleSign(bIsInStall);
	}

	// ����ǰ�̯״̬����ֻ��ʾ���֡����ұ�־�Ͱ�̯��  20100527 BLL
	if ( bIsInStall == TRUE )
	{
		if ( this == CObjectManager::GetMe()->GetMySelf() )
		{
			m_pInfoBoard->ShowIsInStall( TRUE );
		}
		else
			m_pInfoBoard->ShowIsInStall( FALSE );
	}

	if(bIsInStall)
	{
		Start_Stall(TRUE);
	}
}
// �����˵�̯λ���ı�ʱ
VOID CObject_Character::OnDataChanged_StallName(VOID)
{
	if(CObjectManager::GetMe()->GetMySelf() == this )
	{
		// �Լ�
		CDataPool::GetMe()->MyStallBox_SetStallName(GetCharacterData()->Get_StallName());
		//CEventSystem::GetMe()->PushEvent(GE_IMPACT_SELF_UPDATE);
	}
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallBuy"))
	{
		if(GetServerID() == CDataPool::GetMe()->OtStallBox_GetObjID())
		{
			// ����
			CDataPool::GetMe()->OtStallBox_SetStallName(GetCharacterData()->Get_StallName());
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_BUY);
		}
	}
	if(m_pInfoBoard) 
	{
		m_pInfoBoard->SetElement_SaleText(GetCharacterData()->Get_StallName());
	}
}

BOOL CObject_Character::InsertImpact( UINT nSN, INT idImpact, INT nTimeCount, ObjID_t nCreatorID, INT nNumOflayer, BOOL bShowEnableEffect )
{
	if( idImpact == INVALID_ID )
	{
		//Assert( idImpact != INVALID_ID && "CObject_Character::InsertImpact" );
		return FALSE;
	}

	// 20100423 ModifyCodeBegin ��Ϊÿ�ε�nSN����һ������nSN�Ǽ����ͷŵ�Ψһ�ؼ��֣����Բ���ͨ������жϣ�Ҫͨ��impactID����Ч��ID�ж�
	// itFind = m_mapImpactEffect.find( nSN );
	CImpactEffectMap::iterator itFind, itEnd, itCur;
	SImpactEffect *pImpactEffect = NULL;

	itEnd   = m_mapImpactEffect.end();
	itCur	= m_mapImpactEffect.begin();
	itFind  = itEnd; // ��ʼ��itFindΪδ�ҵ�״̬��������Ĵ�������ҵ������¸�ֵΪ�ҵ�����iterator

	while ( itCur != itEnd )
	{
		if ( itCur->second->GetImpactID() == idImpact )
		{
			itFind =  itCur;
			break;
		}

		itCur++;
	}
	// 20100423 ModifyCodeEnd

	// û�и�Ч����������Ч��
	if( itFind == itEnd )
	{
		_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( idImpact );
		if( NULL != pBuffImpact )
		{
			pImpactEffect = new SImpactEffect;
			m_mapImpactEffect[nSN] = pImpactEffect;

			pImpactEffect->SetLayerNum( nNumOflayer );
			pImpactEffect->SetImpactID( idImpact );
			pImpactEffect->SetCreatorID( nCreatorID );
			pImpactEffect->SetLineEffect( pBuffImpact->m_bLineEffect );
			pImpactEffect->SetRotateEffect( pBuffImpact->m_bRotateEffect );
			pImpactEffect->SetCreatorLocator( pBuffImpact->m_pszCreatorLocator);

			UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();

			pImpactEffect->SetShowEffect( bShowEnableEffect );
			pImpactEffect->SetBeginTime( uTimeNow );
			pImpactEffect->SetArrayIndex( nSN );

			// ���ó���buff�ĵ���ʱ���� 20100531 BLL
			pImpactEffect->SetContinuance( nTimeCount );
			pImpactEffect->SetTimeNow( uTimeNow );
			
			// ����Ч��
			if( bShowEnableEffect )
			{
				// Ч������ʱ����Ч��RenderInterface�Զ���ɾ����
				if( strlen( pBuffImpact->m_lpszEffect_Active ) > 0 && m_pRenderInterface )
				{
					m_pRenderInterface->AddEffect( pBuffImpact->m_lpszEffect_Active, pBuffImpact->m_lpszBind_Active );
				}

				//����
				if( strlen( pBuffImpact->m_lpszSound_Active ) > 0 )
				{
					fVector3 fvPos = GetPosition();
					// �˽ӿ����ṩ�����õ���Ч�ӿڣ����Դ������������gfx����
					fVector3 fvGame;
					
					if( CGameProcedure::s_pGfxSystem->Axis_Trans( tGfxSystem::AX_GAME, fvPos, tGfxSystem::AX_GFX, fvGame ) )
					{
						CSoundSystemFMod::_PlaySoundFunc( pBuffImpact->m_lpszSound_Active, &fvGame.x, false );
					}
				}
			}

			// ������Ч����Ҫͨ����Ϣ�������
			if (strlen( pBuffImpact->m_lpszEffect_Continuous ) > 0 && m_pRenderInterface)
			{
				// ����Ч��
				if (pImpactEffect->IsLineEffect())
				{
					UINT uEffect = m_pRenderInterface->AddEffect( pBuffImpact->m_lpszEffect_Continuous, pBuffImpact->m_lpszBind_Continuous );
					pImpactEffect->SetEffect( uEffect );

					UpdateEffectTargetPos(pImpactEffect);
				}
				// ��תЧ��
				else if (pImpactEffect->IsRotateEffect())
				{
					for (INT i=0; i<nNumOflayer; ++i)
					{
						CObject_Bullet *pBullet = CBulletEntityManager::GetMe()->CreateBulletEffect(NULL);

						pBullet->SetSenderID( this->GetServerID() );
						pBullet->SetTargetID( this->GetServerID() );
						pBullet->SetContrailType( BULLET_CONTRAIL_TYPE_ROTATE );
						pBullet->SetEffect( pBuffImpact->m_lpszEffect_Continuous, FALSE );
						pBullet->SetHitEffectLocator ( pBuffImpact->m_lpszBind_Continuous );
						pBullet->SetAngle( 360.0f / nNumOflayer * i );

						pImpactEffect->GetBulletIndexList().push_back( pBullet->GetID() );
					}
				}
				// һ��Ч��
				else
				{
					UINT uEffect = m_pRenderInterface->AddEffect( pBuffImpact->m_lpszEffect_Continuous, pBuffImpact->m_lpszBind_Continuous );
					pImpactEffect->SetEffect( uEffect );
				}				
			}
		}		
	}
	// ��Ч�����У�����
	else
	{
		pImpactEffect = itFind->second;

		pImpactEffect->SetLayerNum( nNumOflayer );

		UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();

		// ���ó���buff�ĵ���ʱ����		20100531 BLL
		pImpactEffect->SetContinuance( nTimeCount );
		pImpactEffect->SetTimeNow( uTimeNow );

			// ��תЧ��,��Ҫ������Ч��
		if( pImpactEffect->IsRotateEffect() )
		{
			// ����
			_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( idImpact );
			if (NULL != pBuffImpact)
			{
				// ��������
				if (nNumOflayer > (INT)(pImpactEffect->GetBulletIndexList().size()))
				{
					INT nNum = nNumOflayer - (INT)(pImpactEffect->GetBulletIndexList().size());

					for (INT i=0; i<nNum; ++i)
					{
						CObject_Bullet *pBullet = CBulletEntityManager::GetMe()->CreateBulletEffect(NULL);

						pBullet->SetSenderID( this->GetServerID() );
						pBullet->SetTargetID( this->GetServerID() );
						pBullet->SetContrailType( BULLET_CONTRAIL_TYPE_ROTATE );
						pBullet->SetEffect( pBuffImpact->m_lpszEffect_Continuous, FALSE );
						pBullet->SetHitEffectLocator ( pBuffImpact->m_lpszBind_Continuous );						

						pImpactEffect->GetBulletIndexList().push_back( pBullet->GetID() );
					}					
				}
				// ��������
				else if (nNumOflayer < (INT)(pImpactEffect->GetBulletIndexList().size()))
				{
					INT nNum = (INT)(pImpactEffect->GetBulletIndexList().size()) - nNumOflayer;
					
					for (INT i=0; i<nNum; ++i)
					{						
						if (pImpactEffect->GetBulletIndexList().size() > 0)
						{
							std::vector<INT>::reverse_iterator iter = pImpactEffect->GetBulletIndexList().rbegin();

							CBulletEntityManager::GetMe()->DestroyBulletByID(*iter);							
							
							pImpactEffect->GetBulletIndexList().pop_back();
						}
					}
				}

				//���³�ʼ���Ƕ�
				INT k = 0;
				std::vector<INT>::iterator iter = pImpactEffect->GetBulletIndexList().begin();
				for (; iter!=pImpactEffect->GetBulletIndexList().end(); ++iter)
				{
					CObject_Bullet *pBullet = CBulletEntityManager::GetMe()->GetBulletByID(*iter);
					if (pBullet)
					{
						FLOAT fAngle = 360.0f / pImpactEffect->GetBulletIndexList().size() * k;
						
						pBullet->SetAngle( fAngle );
						pBullet->CumulateTimeClear();
					}					
					++k;
				}
			}			
		}		
	}
	

	// ���³���Frame
	//UpdatePetFrame();

	return TRUE;
}

VOID CObject_Character::RemoveImpact( UINT nSN )
{	
	CImpactEffectMap::iterator itFind, itEnd, itCur;
	SImpactEffect *pImpactEffect = NULL;
	
	itFind = m_mapImpactEffect.find( nSN );
	itEnd  = m_mapImpactEffect.end();
	
	if( itFind != itEnd )
	{
		pImpactEffect = itFind->second;		

		UINT uEffect = pImpactEffect->GetEffect();
		if( uEffect != 0 && m_pRenderInterface)
		{
			m_pRenderInterface->DelEffect( uEffect );
		}

		//������а󶨵��ӵ�		
		std::vector<INT>::iterator iter = pImpactEffect->GetBulletIndexList().begin();
		for (; iter!=pImpactEffect->GetBulletIndexList().end(); ++iter)
		{
			tObject* obj = CObjectManager::GetMe()->FindObject(*iter);
			if(obj && g_theKernel.IsKindOf(obj->GetClass(), GETCLASS(CObject_Bullet)))
			{
				CObject_Bullet *pBullet = (CObject_Bullet*)obj;
				pBullet->Destroy();
			}				
		}
		pImpactEffect->GetBulletIndexList().clear();
		

		m_mapImpactEffect.erase( itFind );
		delete pImpactEffect;
		pImpactEffect = NULL;

	}

	// ���³���Frame
	//UpdatePetFrame();
}

VOID CObject_Character::RemoveAllImpact( VOID )
{
	CImpactEffectMap::iterator itCur, itEnd;
	SImpactEffect *pImpactEffect = NULL;

	itCur = m_mapImpactEffect.begin();
	itEnd = m_mapImpactEffect.end();
	while ( itCur != itEnd )
	{
		pImpactEffect = itCur->second;
		itCur++;

		UINT uEffect = pImpactEffect->GetEffect();
		if ( uEffect != 0 && m_pRenderInterface)
		{
			m_pRenderInterface->DelEffect( uEffect );
		}
		//������а󶨵��ӵ�		
		std::vector<INT>::iterator iter = pImpactEffect->GetBulletIndexList().begin();
		for (; iter!=pImpactEffect->GetBulletIndexList().end(); ++iter)
		{
			tObject* obj = CObjectManager::GetMe()->FindObject(*iter);
			if(obj && g_theKernel.IsKindOf(obj->GetClass(), GETCLASS(CObject_Bullet)))
			{
				CObject_Bullet *pBullet = (CObject_Bullet*)obj;
				pBullet->Destroy();
			}				
		}
		pImpactEffect->GetBulletIndexList().clear();

		
		delete pImpactEffect;
		pImpactEffect = NULL;
	}
	m_mapImpactEffect.erase( m_mapImpactEffect.begin(), itEnd );

	//���³���Frame
	//UpdatePetFrame();
}


VOID CObject_Character::UpdatePetFrame()
{
	/*if(!CObjectManager::GetMe()) return;
	if(!CObjectManager::GetMe()->GetMySelf()) return;
	if(!CObjectManager::GetMe()->GetMySelf()->GetCharacterData()) return;

	if(!CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull())
	{
		PET_GUID_t pg = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();
		if(CDataPool::GetMe()->Pet_GetPet(pg))
		{
			if((CDataPool::GetMe()->Pet_GetPet(pg))->m_idServer == GetServerID())
			{
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_PAGE);
			}
		}
	}*/
}

VOID CObject_Character::Tick_UpdateEffect( VOID )
{
	// ����ʵʱ��Ч
	if (!m_mapImpactEffect.empty())
	{
		CImpactEffectMap::iterator itCur, itEnd;
		SImpactEffect *pImpactEffect;

		itCur = m_mapImpactEffect.begin();
		itEnd = m_mapImpactEffect.end();

		while( itCur != itEnd )
		{			
			pImpactEffect = itCur->second;

			itCur++;

			// ������Ч
			if( pImpactEffect->IsLineEffect() )
			{
				UpdateEffectTargetPos(pImpactEffect);
			}
		}
	}

	// �ж��ӳٶ���
	if (!m_mapImpactDelay.empty())
	{
		CImpactEffectMap::iterator itCur, itEnd;
		SImpactEffect *pImpactEffect;

		itCur = m_mapImpactDelay.begin();
		itEnd = m_mapImpactDelay.end();

		UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();

		while( itCur != itEnd )
		{
			const UINT uSN	= itCur->first;
			pImpactEffect = itCur->second;

			itCur++;

			if( ( pImpactEffect->GetDelayTime() > 0 ) && 
				( uTimeNow - pImpactEffect->GetBeginTime() >= pImpactEffect->GetDelayTime() ) )
			{
				INT nArrayIndex		= pImpactEffect->GetArrayIndex();
				INT nLayerNum		= pImpactEffect->GetLayerNum();
				INT nImpactId		= pImpactEffect->GetImpactID();
				ObjID_t nCreatorID	= pImpactEffect->GetCreatorID();
				BOOL bShowEffect	= pImpactEffect->GetShowEffect();
				UINT nDelayTime		= pImpactEffect->GetDelayTime();

				// ���ó���buff�ĵ���ʱ����		20100531 BLL
				INT	nTimeCount		= pImpactEffect->GetContinuance();

				InsertImpact( nArrayIndex, nImpactId, nTimeCount, nCreatorID, nLayerNum, bShowEffect );

				// 20100428 AddCodeBegin
				// ���ͽ�ɫ�ͷ�buff��Ҫˢ����Ϣ����targetframe��Ӧ��������buffͼ��
				CGameProcedure::s_pEventSystem->PushEvent( GE_MAINTARGET_BUFF_REFRESH );
				// 20100428 AddCodeEnd

				// �Ӷ�����ɾ��
				m_mapImpactDelay.erase(uSN);
				SAFE_DELETE(pImpactEffect);
			}
		}
	}	
}

VOID CObject_Character::UpdateEffectTargetPos(const SImpactEffect *pImpactEffect)
{
	if( pImpactEffect == NULL )
		return;

	if( pImpactEffect->IsLineEffect() )
	{
		UINT uEffect = pImpactEffect->GetEffect();
		if( uEffect != 0 && m_pRenderInterface != NULL && pImpactEffect->GetCreatorID() != INVALID_ID )
		{
			CObject_Character *pCreator = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(pImpactEffect->GetCreatorID()));
			if( pCreator != NULL && g_theKernel.IsKindOf(pCreator->GetClass(), GETCLASS(CObject_Character)) )
			{
				fVector3 fvPos;
				if( pCreator->GetRenderInterface() != NULL && strlen( pImpactEffect->GetCreatorLocator() ) > 0 )
				{
					pCreator->GetRenderInterface()->Actor_GetLocator( pImpactEffect->GetCreatorLocator(), fvPos );
				}
				else
				{
					fvPos = pCreator->GetPosition();
				}
				m_pRenderInterface->Actor_SetEffectExtraTransformInfos(uEffect, fvPos);
			}
		}
	}
}

VOID CObject_Character::Tick(VOID)
{
	// 
	if( GetCharacterType() == CT_PLAYEROTHER )
	{
		if( CGameProcedure::s_pProcMain->m_bShowOtherObj )
		{
			// �Ѿ�����
			if( IsDisable(OSF_VISIABLE) )
			{
				Enable( OSF_VISIABLE );

				// ��ʾ
				if( m_pRenderInterface )
					m_pRenderInterface->SetVisible( TRUE );
			}
		}
		else
		{
//			Disalbe( OSF_VISIABLE );

			// �Ѿ���ʾ
			if( IsEnable(OSF_VISIABLE) )
			{
				Disalbe( OSF_VISIABLE );

				// ����
				if( m_pRenderInterface )
					m_pRenderInterface->SetVisible( FALSE );
			}
		}
	}


	// ����Tick
	CObject_Phy::Tick();

	// �߼�����
	ProcessLogicCommand();

	// AI�߼�Tick
	if( m_pTheAI )
		m_pTheAI->Tick();

	UINT uElapseTime = (UINT)(CGameProcedure::s_pTimeSystem->GetDeltaTime());
	uElapseTime = (UINT)((FLOAT)uElapseTime * GetLogicSpeed());

	// �����˺���Ϣ��ʱ��
	UpdataDisplayInfoTimer( uElapseTime );

	// �����߼�
	if( !CharacterLogic_IsStopped(LOGIC_BASE) )
	{
		BOOL bResult = FALSE;
		
		switch( CharacterLogic_Get(LOGIC_BASE) )
		{
		case CHARACTER_LOGIC_IDLE:
			bResult = Tick_Idle(uElapseTime);
			break;
		case CHARACTER_LOGIC_MOVE:
			bResult = Tick_Move(uElapseTime);
			break;
		case CHARACTER_LOGIC_ABILITY_ACTION:
			bResult = Tick_Ability(uElapseTime);
			break;
		case CHARACTER_LOGIC_DEAD:
			bResult = Tick_Dead(uElapseTime);
			break;
		case CHARACTER_LOGIC_STALL:
			bResult = Tick_Stall(uElapseTime);
			break;
		default:
			bResult = FALSE;
			break;
		};

		if(!bResult)
			Start_Idle();
	}
	
	// ��Ϊ�߼�
	if( !CharacterLogic_IsStopped(LOGIC_ACTION) )
	{
		switch( CharacterLogic_Get(LOGIC_ACTION) )
		{
		case CHARACTER_LOGIC_ACTION:
			Tick_Action(uElapseTime);
			break;
		case CHARACTER_LOGIC_SKILL_GATHER:
			Tick_MagicCharge(uElapseTime);
			break;
		case CHARACTER_LOGIC_SKILL_LEAD:
			Tick_MagicChannel(uElapseTime);
			break;
		case CHARACTER_LOGIC_SKILL_SEND:
			Tick_MagicSend(uElapseTime);
			break;
		//test2
		case CHARACTER_LOGIC_SKILL_PREP_SEND:
			Tick_MagicPrepSend( uElapseTime );
			break ;
		};
	}


	// ����
	if( this == CObjectManager::GetMe()->GetMySelf() )
	{
		//====== �������λ��ͬ�������

		// ������ڸ���״̬
		if(m_bTail)
		{
/*
			FLOAT fSpeed = 3;
			fVector3 vPos = GetPosition();
			fVector3 vFootPos = GetFootPosition();

			FLOAT fCurTickDist = fSpeed * CGameProcedure::s_pTimeSystem->GetDeltaTime() / 1000.0f;
			FLOAT fCurTickDistSq = fCurTickDist * fCurTickDist;
			FLOAT fDistSq = KLU_GetDistSq( vPos, vFootPos );

			// �ӽ���
			if( fDistSq <= fCurTickDistSq )
			{
				SetFootPosition( vPos );
				m_bTail = FALSE;	// ������
			}
			else
			{
				fVector3 vDir = vPos - vFootPos;
				vDir.normalise();

				fVector3 vLength;
				vLength.x = vDir.x * fCurTickDist;
				vLength.y = vDir.y * fCurTickDist;
				vLength.z = vDir.z * fCurTickDist;

				vFootPos = vLength + GetFootPosition();

				SetFootPosition( vFootPos );
			}
*/
		}
		else
		{
			// �����λ��
			CGameProcedure::s_pGfxSystem->Camera_SetLookAt( GetFootPosition() );
		}


		//====== �������λ��ͬ��������Ч
		CGameProcedure::s_pGfxSystem->Scene_SetEnviromentInfo( GetPosition() );
		
		//====== ͬ��ָʾ�⻷
//		if( CGameProcedure::s_pProcMain->IsAutoRun() )
//		{
//			CGameProcedure::s_pProcMain->GetMouseTarget()->UpdateAsCursor();
//		}


		// ���³ƺŵ�ʱ��
		if( m_pCharacterData )
		{
			_TITLE_ title;

			INT nTitleNum = m_pCharacterData->Get_TitleNum();
			for( INT i = 0; i < nTitleNum; ++i )
			{
				title = m_pCharacterData->Get_Title( i );
				// ��Ϊ0˵������ʱ�޵�
				if( title.nTime != 0 )
				{
					// ����
					title.nTime -= uElapseTime;
				}
			}
		}
	}


	// ͷ����Ϣ��Tick
	Tick_UpdateInfoBoard();

	// ��Чˢ��
	Tick_UpdateEffect();

	// ÿ�������buff��ʣ��ʱ��		20100531 BLL
	TickBuffElapseCount();	

	// �����˺���Ϣ
	Update_LogicEvent();

	// ս��״̬
	if( m_uFightStateTime > 0 )
	{
		UINT uET = (UINT)( CGameProcedure::s_pTimeSystem->GetDeltaTime() );
		if( uET > m_uFightStateTime )
		{
			m_uFightStateTime = 0;
		}
		else
		{
			m_uFightStateTime -= uET;
		}
	}

}

VOID CObject_Character::Tick_UpdateInfoBoard(VOID)
{
	// ͷ����Ϣ���Ӧ��UI�ӿ�
	if( !m_pInfoBoard )
	{
		return;
	}

	// �����У����ټ���
	if( !IsEnable(OSF_VISIABLE) || !GetRenderInterface() ) 
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}
 
	// ����Ƿ�رյ������˵���Ϣ
	if( !( CGameProcedure::s_pProcMain->m_bShowOtherObjBoard ) && ( GetCharacterType() == CT_PLAYEROTHER ) )	
	{
		m_pInfoBoard->Show( FALSE );
		return;
	}


	// ��������
	if( strlen( GetCharacterData()->Get_Name() ) == 0 )
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}

	// �ǹֲ�������
	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_DEAD && GetCharacterType() == CT_MONSTER )
	{
		m_pInfoBoard->Show(FALSE);
		return ;
	}


	// ����λ��
	fVector2 fvPos = m_pInfoBoard->GetPosition();
	fVector3 fvCurPos = GetPosition();		// �����λ��
	if( m_pMountRenderInterface != NULL )	// ��������ﻹҪ�������ĸ߶�
	{
		fvCurPos.y += m_fMountAddHeight;
	}


	// �Ƿ�ɼ�
	BOOL bVisible;

	if(	GetCharacterType() == CT_MONSTER && (((CObject_PlayerNPC*)this)->GetBoardHeight() > 0 ) )
		bVisible = GetRenderInterface()->Actor_GetInfoBoardPos( fvPos, &fvCurPos, ((CObject_PlayerNPC*)this)->GetBoardHeight() );
	else
		bVisible = GetRenderInterface()->Actor_GetInfoBoardPos( fvPos, &fvCurPos );

 	if( !bVisible )
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}


	// ���������ǵľ���
	fVector3 fMyselfPos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
	FLOAT fDistance = KLU_GetDistSq( CObjectManager::GetMe()->GetMySelf()->GetPosition(), GetPosition() );

	m_fvInfoBoardPos = fvPos;

	m_pInfoBoard->SetInfoDistance( fDistance );


	// �����Ƿ���ʾ����״̬
	
	// �������
	if( GetCharacterType() == CT_PLAYEROTHER )
	{
		// �ж��Ƿ�pk��ϵ
		// ��Ӫ
		ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );

		if( eCampType == RELATION_ENEMY )
		{
			//SetBoardHpLevelState( TRUE );

			// �ȼ�����ʾ��ʽ
			INT playerLevel = m_pCharacterData->Get_Level();
			INT selfLevel	= CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();

			// �жϵȼ���࣬ ��ʾ���ֻ�ͼ��
			if( playerLevel - selfLevel >= 20 )
			{
				m_pInfoBoard->SetElement_Level( -1, 1 );
			}
		}
		
		// �����������ң�����û�а�̯ʱ��ʼ����ʾ�ȼ�    20100527 BLL
		if ( GetCharacterData()->Get_StallOpenPublic() == TRUE )
		{
			SetBoardHpLevelState( FALSE );
		}
		else
			SetBoardHpLevelState( TRUE );
	}
	// npc
	else if( GetCharacterType() == CT_MONSTER )
	{
		// �Ƿ񲻿ɹ���
		bool bCantBeAttack = ( ( CObject_PlayerNPC*)this)->GetCreatureAtt() && 
							   (( ((CObject_PlayerNPC*)this)->GetCreatureAtt()->nCannotBeAttack) == 1 );
		// ��Ӫ
		ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );

		// ��ƽ
		if( bCantBeAttack || eCampType == RELATION_FRIEND )
		{
			// ����ʾѪ���͵ȼ�
			SetBoardHpLevelState( FALSE );

			
			// ������ʾ������ʾ
			MISSION_STATE ms = ( (CObject_PlayerNPC*)this )->GetNpcMissionState();

			switch( ms )
			{
			// �пɽ�����
			case MS_MISS:
				{
					m_pInfoBoard->SetElement_PKFlag( 1, 0, TRUE );
					break;
				}
			// �ѽ����� δ���
			case MS_HAVE_MISS_NO_DONE:
				{
					m_pInfoBoard->SetElement_PKFlag( 1, 1, TRUE );
					break;
				}
			// �ѽ����� �����
			case MS_HAVE_MISS_DONE:
				{
					m_pInfoBoard->SetElement_PKFlag( 1, 2, TRUE );
					break;
				}

			default:
				m_pInfoBoard->SetElement_PKFlag( 1, 3, FALSE ); 
				// �ޱ�ǣ� ��ͨ״̬
				;

			};

		}
		else
		{
			SetBoardHpLevelState( TRUE );

			// 20100510 AddCodeBegin
			// �ж�NPC��ʾ�ȼ�
			// �ȼ�����ʾ��ʽ
			INT playerLevel = m_pCharacterData->Get_Level();
			INT selfLevel	= CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();

			// �жϵȼ���࣬ ��ʾ���ֻ�ͼ��
			if( playerLevel - selfLevel >= 20 )
			{
				m_pInfoBoard->SetElement_Level( -1, 1 );
			}

			// 20100510 AddCodeEnd
		}

		// ����ʾ�ƺŴ���
//		m_pInfoBoard->ShowCharTitle( FALSE, 1 );
	}

	// �Ƿ���ʾ��Ϣ
	if(	GetCharacterType() == CT_MONSTER && (((CObject_PlayerNPC*)this)->IsDisplayBoard() == FALSE) )
		m_pInfoBoard->Show(FALSE);
	else
	{
		m_pInfoBoard->Show(TRUE);	
	}

	m_pInfoBoard->SetPosition(m_fvInfoBoardPos.x, m_fvInfoBoardPos.y);
}

VOID CObject_Character::SetBoardTitleState( BOOL bShow, INT nType )
{
	m_pInfoBoard->ShowCharTitle( bShow, nType );
}
VOID CObject_Character::SetBoardHpLevelState( BOOL bShow )
{
	m_pInfoBoard->ShowCharAttr( bShow );
}


VOID CObject_Character::UpdateCharModel(VOID)
{
	INT nCharModelID = CalcCharModelID();
	if( nCharModelID != m_nCharModelID )
	{
		m_nCharModelID = nCharModelID;

		UpdateCharActionSetFile();

		ReleaseRenderInterface();

		if( m_nCharModelID != INVALID_ID )
		{
			CreateRenderInterface();
		}

		SetPosition(GetPosition());
		SetFaceDir( GetFaceDir() );
		Enable( IsEnable(OSF_VISIABLE) );
		Disalbe( IsEnable(OSF_OUT_VISUAL_FIELD) );

		UpdateTransparent();
		UpdateBuffEffect();

		// �������������
		FreshAnimation();
		if(CObjectManager::GetMe()->GetMainTarget() == this && GetRenderInterface() != NULL)
		{
			GetRenderInterface()->Attach_ProjTexture(tEntityNode::SELECT_RING, TRUE, GetProjtexRange() );
		}
	}
}

VOID CObject_Character::UpdateMountModel(VOID)
{
	if (NULL == GetCharacterData())
		return;

	INT nMountID = GetCharacterData()->Get_MountID();

	// ����� char_mount.tab
	DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
	const _DBC_CHARACTER_MOUNT *pCharMount = (const _DBC_CHARACTER_MOUNT*)( s_pCharMountDBC->Search_Index_EQU(nMountID) );

	// ����ID��Ч������������
	if( pCharMount == NULL )
	{
		// ����
		Dismount();

		// ����Ķ���
		UpdateCharActionSetFile();
	}
	// ��Ч������
	else
	{
		MountData* pMount = GetMyMount( GetCharacterData()->Get_MountID() );

		// �������ʵ���Ѿ�ӵ��,����
		if (pMount)
		{
			// �ǵ�ǰ����Ͳ��û���
			if (m_pMountRenderInterface != pMount->pMountRenderInterface)
			{
				// ����Ķ���
				UpdateCharActionSetFile();

				// ����Ķ���
				UpdateMountActionSetFile();

				// ���õ�ǰ����
				Mounting(pMount->pMountRenderInterface);

				// ͷ������������
				m_fMountAddHeight = pMount->fMountAddHeight;
			}
		}
		// û�У�������ﲢ����
		else
		{
			// ����
			CreateNewMount(nMountID);

			// ����Ķ���
			UpdateCharActionSetFile();

			// ����Ķ���
			UpdateMountActionSetFile();		

			// ���õ�ǰ����
			MountData* pNewMount = GetMyMount(nMountID);
			if (pNewMount)
			{
				Mounting(pNewMount->pMountRenderInterface);

				m_fMountAddHeight = pNewMount->fMountAddHeight;
			}
		}
	}

	SetPosition( GetPosition() );
	SetFaceDir( GetFaceDir() );
	Enable( IsEnable(OSF_VISIABLE) );
	Disalbe( IsEnable(OSF_OUT_VISUAL_FIELD) );

	UpdateTransparent();

	// �������������
	if( CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_DEAD && CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_MOVE )
	{
		Start_Idle();
	}
	else
	{
		FreshAnimation();
	}
}

// ����ӵ�е���  [7/14/2010 �¾���]
VOID CObject_Character::UpdateMountList(VOID)
{
	if (NULL == GetCharacterData())
		return;

	INT nMountID = GetCharacterData()->Get_MountID();

	// ����� char_mount.tab
	DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
	const _DBC_CHARACTER_MOUNT *pCharMount = (const _DBC_CHARACTER_MOUNT*)( s_pCharMountDBC->Search_Index_EQU(nMountID) );

}

VOID CObject_Character::UpdateCharActionSetFile(VOID)
{
	m_pCharActionSetFile	= NULL;

	if( GetCharacterData() != NULL )
	{
		INT nCharModelID	= GetCharModelID();
		INT nMountID		= GetCharacterData()->Get_MountID();

		if(nCharModelID != INVALID_ID)
		{
			DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
			const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU(nCharModelID));
			// ������Ч
			if (pCharModel != NULL)
			{
				//�������ID�Ƿ���Ч
				DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
				const _DBC_CHARACTER_MOUNT *pMount = (const _DBC_CHARACTER_MOUNT*)( s_pCharMountDBC->Search_Index_EQU(nMountID) );

				m_fDefaultFuse = pCharModel->m_fFuseTime;

				const CHAR *pszActionSetFileName = NULL;

				// ������Ч���������϶�����
				if(pMount)
				{
					INT nCharActionIndex = pMount->m_nCharActionIndex; // �������ʱ�Ķ�����,��Ӧchar_model.tab��mount0~19

					if( nCharActionIndex >= 0 && nCharActionIndex < MAX_MOUNT_NUMBER )
					{
						if( strlen(pCharModel->m_apszActionSetName_Mount[nCharActionIndex]) > 0 )
						{
							pszActionSetFileName = pCharModel->m_apszActionSetName_Mount[nCharActionIndex];
						}
						else
						{
							pszActionSetFileName = pCharModel->m_pszActionSetName_None;

							KLThrow( _T( " Char_Model ����û����������ﶯ���ļ� ��" ) );
						}						
					}
					else
					{
						pszActionSetFileName = pCharModel->m_pszActionSetName_None;
					}
				}
				// ��Ч���������¶�����
				else
				{
					if( strlen(pCharModel->m_pszActionSetName_None ) > 0)
					{
						pszActionSetFileName = pCharModel->m_pszActionSetName_None;
					}
					else
					{
						KLThrow( _T( " Char_Model ����û������Ķ����ļ� ��" ) );
					}
				}

				// ���صĶ���
				if (NULL != pszActionSetFileName)
				{
					m_pCharActionSetFile = GetActionSetMgr()->GetActionSetFile(pszActionSetFileName);
				}				
			}
		}
	}
}

VOID CObject_Character::UpdateMountActionSetFile(VOID)
{
	m_pMountActionSetFile	= NULL;

	if( GetCharacterData() != NULL )
	{
		//�������ID�Ƿ���Ч
		DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
		const _DBC_CHARACTER_MOUNT *pMount = (const _DBC_CHARACTER_MOUNT*)( s_pCharMountDBC->Search_Index_EQU(GetCharacterData()->Get_MountID()) );
	
		if (pMount)
		{
			DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
			const _DBC_CHARACTER_MODEL *pMountModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU(pMount->m_nModelID));
			if( pMountModel != NULL && strlen(pMountModel->m_pszActionSetName_None) > 0 )
			{
				m_pMountActionSetFile = GetActionSetMgr()->GetActionSetFile(pMountModel->m_pszActionSetName_None);
			}
			else
			{
				KLThrow( _T( " �� Char_Model ����û�ҵ� %d ��Ӧ������ģ�� �� ����û�����ļ� ��" ), pMount->m_nModelID );
			}
		}
	}
}

VOID CObject_Character::UpdateWeaponActionSetFile( LPCTSTR szWeaponAnimName )
{
	m_pWeaponActionSetFile = NULL;

	if( NULL != szWeaponAnimName )
	{
		m_pWeaponActionSetFile = GetActionSetMgr()->GetActionSetFile( szWeaponAnimName );
	}
}


LPCSTR CObject_Character::GetCharActionNameByActionSetID(INT nActionSetID, INT nWeaponType, BOOL *pbHideWeapon, INT *pnAppointedWeaponID)
{
	INT nCalcWeaponType = nWeaponType;

	// ͨ��������������ȡ��������
	switch( GetCharacterType() )
	{
	case CT_MONSTER:
		// �����ܷ񱻹������ж���Npc ���ǹ�
		// �������ͱ���Ϊ �� �� Npc
		if( CannotBeAttack() )
		{
			// ���ܱ���������NPC
			nCalcWeaponType = WEAPON_TYPE_NPC;
		}
		break;

	case CT_PLAYEROTHER:
	case CT_PLAYERMYSELF:
	default:
		break;
	};

	if( (m_pCharActionSetFile != NULL)	&&				// �����ļ�ʵ����Ч
		(nActionSetID != -1)			&&				// ָ���Ķ���id��Ч
		(nCalcWeaponType >= 0)			&&				// ����������Ч
		(nCalcWeaponType < WEAPON_TYPE_NUMBERS) )	// ������
	{
		// ȡ�������еĶ����У�����ʲô�������ܡ����� ����������
		const _DBC_CHARACTER_ACTION_SET *pActionSet = (const _DBC_CHARACTER_ACTION_SET*)(m_pCharActionSetFile->Search_Index_EQU(nActionSetID));
		if(pActionSet != NULL)
		{
			// ��������
			if( pbHideWeapon != NULL )
			{
				*pbHideWeapon = pActionSet->bHideWeapon;
			}

			// ָ������
			if( pnAppointedWeaponID != NULL )
			{
				*pnAppointedWeaponID = pActionSet->nAppointedWeaponID;
			}

			// ȡ��Ӧ���У���ʲô�����Ķ����� Ҳ����Npc �� �ֵĶ�����
			return pActionSet->pWeapon_Set[ nCalcWeaponType ];	
		}
	}

	return NULL;
}

LPCSTR CObject_Character::GetMountActionNameByActionSetID(INT nActionSetID, INT nWeaponType)
{
	INT nCalcWeaponType = nWeaponType;
	switch( GetCharacterType() )
	{
	case CT_MONSTER:
		if( CannotBeAttack() )
		{
			nCalcWeaponType = WEAPON_TYPE_NPC;
		}
		break;

	case CT_PLAYEROTHER:
	case CT_PLAYERMYSELF:
	default:
		break;
	}

	if( (m_pMountActionSetFile != NULL) && 
		(nActionSetID != -1)			&& 
		(nCalcWeaponType >= 0)			&& 
		(nCalcWeaponType < WEAPON_TYPE_NUMBERS) )
	{
		const _DBC_CHARACTER_ACTION_SET *pActionSet = (const _DBC_CHARACTER_ACTION_SET*)(m_pMountActionSetFile->Search_Index_EQU(nActionSetID));
		if( pActionSet != NULL )
		{
			return pActionSet->pWeapon_Set[ nCalcWeaponType ];
		}
	}

	return NULL;
}


LPCSTR CObject_Character::GetWeaponActionNameByActionSetID( INT nActionSetID, INT nWeaponType )
{
	if( (m_pWeaponActionSetFile != NULL)	&&				// ���������ļ�
		(nActionSetID != -1)				&&				// ����id
		(nWeaponType >= 1)					&&				// �������ͺϷ��ԣ������ǿ��֣� ���ܳ���������������
		(nWeaponType < WEAPON_TYPE_NUMBERS) )
	{
		// ͬ���ﶯ�����ʽ��ͬ
		const _DBC_CHARACTER_ACTION_SET* pActionSet = (const _DBC_CHARACTER_ACTION_SET*)( m_pWeaponActionSetFile->Search_Index_EQU( nActionSetID ) );
		if( pActionSet != NULL )
		{
			return pActionSet->pWeapon_Set[ nWeaponType ];
		}
	}

	return NULL;
}


bool CObject_Character::_OnAnimationEnd(LPCTSTR szAnimationName, UINT dwParam, INT nPart, INT nLogic)
{
	//ȡ�ø�Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		//������Ӧ�Ķ�����
		return (((CObject_Character*)pObj)->OnAnimationEnd( szAnimationName, (ENUM_ACTION_TYPE)nPart, (LOGIC_TYPE)nLogic ) ) ? (true):(false);
	else
		//�ö����Ѿ�����?
		return false;
}

bool CObject_Character::_OnAnimationCanBreak(LPCTSTR szAnimationName, UINT dwParam)
{
	//ȡ�ø�Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		//������Ӧ�Ķ�����
		return (((CObject_Character*)pObj)->OnAnimationCanBreak(szAnimationName))?(true):(false);
	else
		//�ö����Ѿ�����?
		return false;
}

bool CObject_Character::_OnAnimationHit(LPCTSTR szAnimationName, UINT dwParam)
{
	//ȡ�ø�Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		//������Ӧ�Ķ�����
		return (((CObject_Character*)pObj)->OnAnimationHit(szAnimationName))?(true):(false);
	else
		//�ö����Ѿ�����?
		return false;
}

bool CObject_Character::_OnAnimationShakeEvent(LPCTSTR szAnimationName, UINT dwParam)
{
	//ȡ�ø�Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		//������Ӧ�Ķ�����
		return (((CObject_Character*)pObj)->OnAnimationShakeEvent(szAnimationName))?(true):(false);
	else
		//�ö����Ѿ�����?
		return false;
}

//֪ͨһ�������¼�����
VOID CObject_Character::NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam)
{
	switch(eventid)
	{
	case PE_COLLISION_WITH_GROUND:
		{
			if(GetbJumping())
			{
				// �����
				SetbJumping(FALSE);
				FreshAnimation();
				PhyEnable(FALSE);
				UnRegisterPhyEvent(PE_COLLISION_WITH_GROUND);
			}
		}
		break;
	default:
		break;
	}
	return;
}


BOOL CObject_Character::OnAnimationShakeEvent(LPCTSTR szAnimationName)
{
	CGameProcedure::s_pGfxSystem->Camera_VibrateOneTime();
	return TRUE;
}

STRING CObject_Character::GetDebugDesc(VOID)
{
	STRING strHistory;
	for(INT i=0; i<(INT)m_qDebugString.size(); i++)
	{
		strHistory += m_qDebugString[i];
		strHistory += "\n";
	}

	CHAR* szLogic = NULL;
	{
		switch(CharacterLogic_Get(LOGIC_BASE))
		{
		case CHARACTER_LOGIC_IDLE:				szLogic = "IDLE";			break;
		case CHARACTER_LOGIC_ACTION:			szLogic = "ACTION";			break;
		case CHARACTER_LOGIC_MOVE:				szLogic = "MOVE";			break;
		case CHARACTER_LOGIC_SKILL_GATHER:		szLogic = "SKILL_GATHER";	break;
		case CHARACTER_LOGIC_SKILL_LEAD:		szLogic = "SKILL_LEAD";		break;
		case CHARACTER_LOGIC_SKILL_SEND:		szLogic = "SKILL_SEND";		break;
		case CHARACTER_LOGIC_DEAD:				szLogic = "DEAD";			break;
		case CHARACTER_LOGIC_ABILITY_ACTION:	szLogic = "ABILITY_ACTION";	break;
		case CHARACTER_LOGIC_STALL:				szLogic = "STALL";			break;
		default:								szLogic = "UNKNOWN";		break;
		}
	}


	TCHAR szTemp[1024];
	_sntprintf(szTemp, 1024, 
		_T("ID:(%d) Pos:(%.1f,%.1f)\n")
		_T("Logic:(%s)\n")
		_T("HP:%.1f%%  MP:%.1f%%\n")
		_T("Board(%.2f, %.2f)")
		_T("%s"), 

		GetServerID(),
		this->GetPosition().x, this->GetPosition().z,
		szLogic,
		GetCharacterData()->Get_HPPercent()*100.0f, 
		GetCharacterData()->Get_MPPercent()*100.0f, 
		m_fvInfoBoardPos.x, m_fvInfoBoardPos.y,
		strHistory.c_str());

	return STRING(szTemp);
}

const _CAMP_DATA *CObject_Character::GetCampData(VOID) const
{
	if ( GetConstCharacterData() != NULL )
		return GetConstCharacterData()->Get_CampData();
	else
		return NULL;
}

ObjID_t CObject_Character::GetOwnerID(VOID) const
{
	if ( GetConstCharacterData() != NULL )
		return GetConstCharacterData()->Get_OwnerID();
	else
		return INVALID_ID;
}


VOID CObject_Character::OnDataChanged_CountryFlag(VOID)
{
	if( m_pInfoBoard )
		m_pInfoBoard->SetElement_CountryFlag( GetCharacterData()->Get_Country() );
}

VOID CObject_Character::OnDataChanged_TeamLeaderFlag(VOID)
{
	if( !m_pInfoBoard )
		return ;

	// 20100514 ModifyCodeBegin
	// ���ֶ�Ա���ӳ����
	if( GetCharacterData()->Get_HaveTeamFlag() == TRUE )
	{
		if ( GetCharacterData()->Get_TeamLeaderFlag() == TRUE )
		{
			m_pInfoBoard->SetElement_LeaderFlag( GetCharacterData()->Get_TeamLeaderFlag(), 1 );
		}
		else
			m_pInfoBoard->SetElement_LeaderFlag( TRUE, 0 ); 
	}
	else
		m_pInfoBoard->SetElement_LeaderFlag( FALSE, -1 );
	// 20100514 ModifyCodeEnd

}
VOID CObject_Character::OnDataChanged_TeamFlag(VOID)
{
	if( !m_pInfoBoard )
		return ;
//�����ö����ǵ�ʱ����õ�����ʾ�ӳ���ǣ���������ע�͵�
//	m_pInfoBoard->SetElement_LeaderFlag( GetCharacterData()->Get_HaveTeamFlag(), 1 );
}


VOID CObject_Character::OnDataChanged_Die(VOID)
{
	if(GetCharacterData()->IsDie())
	{
		if(m_listLogicEvent.empty())
		{
			Start_Dead(TRUE);
		}
	}
}

INT CObject_Character::GetActivatedSkillID()
{
	if (CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_GATHER)
	{
		return m_paramLogic_MagicCharge.m_nSkillID;
	}
	if (CharacterLogic_Get(LOGIC_ACTION) == CHARACTER_LOGIC_SKILL_LEAD)
	{
		return m_paramLogic_MagicChannel.m_nSkillID;
	}
	return -1;
}

VOID CObject_Character::OnDataChanged_StealthLevel(VOID)
{
	UpdateTransparent();
}

VOID CObject_Character::OnDataChanged_FightState(VOID)
{
}

VOID CObject_Character::OnDataChanged_Sit(VOID)
{
	if( CharacterLogic_Get(LOGIC_BASE) == CHARACTER_LOGIC_IDLE && !CharacterLogic_IsStopped(LOGIC_BASE) )
	{
		if(!m_bJumping)
		{
			if(GetCharacterData()->IsSit())
			{
				ClearAllChatMoodAction();
				ChangeAction(BASE_ACTION_N_SIT_DOWN, GetLogicSpeed(), FALSE);
			}
			else
			{
				ChangeAction(BASE_ACTION_N_SIT_STAND, GetLogicSpeed(), FALSE);
			}
		}
	}
}

VOID CObject_Character::OnDataChanged_Level(VOID)
{
	INT nLevel = GetCharacterData()->Get_Level();
	m_pInfoBoard->SetElement_Level( nLevel, 0 );
}

VOID CObject_Character::OnDataChanged_HP(VOID)
{
//	INT hp = GetCharacterData()->Get_HP();
//	INT maxHp = GetCharacterData()->Get_MaxHP();
//
//	if( hp != 0 && maxHp != 0 )
//		m_pInfoBoard->SetElement_HPProgress( hp, maxHp );

	FLOAT hpPercent = GetCharacterData()->Get_HPPercent();
	m_pInfoBoard->SetElement_HPProgress( hpPercent );
}

VOID CObject_Character::OnDataChanged_Penuma(VOID)
{

}

VOID	CObject_Character::OnDataChanged_BusObjID(VOID)
{
	UpdateCharActionSetFile();

	if(GetBusObjID() == INVALID_ID)
	{
		// �������������
		if ( CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_DEAD
			&& CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_MOVE
			//&& CharacterLogic_Get() != CHARACTER_LOGIC_FLY
			&& CharacterLogic_Get(LOGIC_BASE) != CHARACTER_LOGIC_SKILL_LEAD )
		{
			CharacterLogic_Stop(TRUE, LOGIC_BASE);
			Start_Idle();
		}
		else
		{
			FreshAnimation();
		}

		SetPosition(GetPosition());

		if(m_pRenderInterface)
		{
			//m_pRenderInterface->Actor_RecaculateBoardPos(GetBoardHeight());
		}
	}
	else
	{
		//���¼���ͷ����Ϣ�����λ��
		CObject_Bus *pBus = (CObject_Bus*)(CObjectManager::GetMe()->FindServerObject(GetBusObjID()));
		if(pBus && m_pRenderInterface)
		{
			//m_pRenderInterface->Actor_RecaculateBoardPos(GetBoardHeight(), 0.f); //pBus->GetBusData()->m_fModelHeight);
		}
	}
}

INT CObject_Character::GetCharModelID( VOID )const
{
	return m_nCharModelID;
}

CObject_Character::MountData* CObject_Character::GetMyMount(INT nMountID)
{
	MapMountData::iterator iter = m_mapMyMount.find(nMountID);
	if (iter!=m_mapMyMount.end())
	{
		return &(iter->second);
	}
	return NULL;
}

INT CObject_Character::CalcCharModelID( VOID )const
{
	return INVALID_ID;
}

VOID CObject_Character::SetFightState( BOOL bSet )
{
	if( bSet )
	{
		m_uFightStateTime = FIGHT_STATE_DURATION_TIME;
	}
	else
	{
		m_uFightStateTime = 0;
	}
}

VOID CObject_Character::SetPosition(const fVector3& fvPosition) 
{
	if(m_pMountRenderInterface)
	{
		m_fvPosition = fvPosition;
		m_pMountRenderInterface->SetPosition( GetPosition() );
	}
	else
	{
		Tick_UpdateInfoBoard();
		CObject_Dynamic::SetPosition( fvPosition );
	}
}

VOID			CObject_Character::SetScale(FLOAT fScale)
{
	if (m_pRenderInterface)
	{
		m_pRenderInterface->SetScaleFactor(fScale, fScale, fScale);
	}
	if (m_pMountRenderInterface)
	{
		m_pMountRenderInterface->SetScaleFactor(fScale, fScale ,fScale);
	}
}

VOID CObject_Character::SetRotation(const fVector3& fvRotation) 
{ 
	if(m_pMountRenderInterface)
	{
		m_fvRotation = fvRotation;
		m_pMountRenderInterface->SetOrientation( GetRotation() );
	}
	else
	{
		CObject_Dynamic::SetRotation( fvRotation );
	}
}

VOID CObject_Character::Enable(UINT dwFlag)
{
	CObject_Dynamic::Enable( dwFlag );

	switch(dwFlag)
	{
	case OSF_VISIABLE:
		if(m_pMountRenderInterface)m_pMountRenderInterface->SetVisible(true);
		break;

	case OSF_RAY_QUERY:
		if(m_pMountRenderInterface)m_pMountRenderInterface->SetRayQuery(true);
		break;

	default:
		break;
	}
}

VOID CObject_Character::Disalbe(UINT dwFlag)
{ 
	CObject_Dynamic::Disalbe( dwFlag );

	switch(dwFlag)
	{
	case OSF_VISIABLE:
		if(m_pMountRenderInterface)m_pMountRenderInterface->SetVisible(FALSE);
		break;

	case OSF_RAY_QUERY:
		if(m_pMountRenderInterface)m_pMountRenderInterface->SetRayQuery(FALSE);
		break;
	default:
		break;
	}
}

void CObject_Character::Say( const STRING& str )
{
	m_pInfoBoard->SetElement_PaoPaoText( str.c_str() );
}

UINT CObject_Character::CalcIdleRandInterval(VOID)
{
	UINT uInterval = GetIdleInterval();
	if(uInterval == UINT_MAX)
	{
		uInterval = UINT_MAX;
	}
	else
	{
		uInterval = (UINT)((FLOAT)(GetIdleInterval()) * ((FLOAT)(64 - (rand() % 16)))/64.f);
	}
	return uInterval;
}

BOOL CObject_Character::IsDie(VOID)const
{
	if(m_pCharacterData != NULL)
	{
		return m_pCharacterData->IsDie();
	}
	else
	{
		return FALSE;
	}
}

BOOL CObject_Character::AddDropBoxEvent(ObjID_t idItemBox, GUID_t idOwner, const WORLD_POS *pCreatePos)
{
	if(pCreatePos == NULL)
	{
		return FALSE;
	}

	m_bDropBox_HaveData		= TRUE;
	
	m_nDropBox_ItemBoxID	= idItemBox;
	m_DropBox_OwnerGUID		= idOwner;
	m_posDropBox_CreatePos	= *pCreatePos;

	return TRUE;
}

BOOL CObject_Character::AddDropItemEvent(ObjID_t idItemDrop, GUID_t* idOwner, const WORLD_POS *pCreatePos,UINT time ,BOOL isCanPick)
{
	if(pCreatePos == NULL)
	{
		return FALSE;
	}
	m_bDropItem_HaveData    = TRUE;
	m_nDropItem_ItemDropID = idItemDrop;
	//m_DropItem_OwnerObjId = idOwner;
	if (!idOwner)
	{
		return FALSE;
	}

	for (int i = 0 ;i < MAX_TEAM_MEMBER ;++i)
	{
		m_DropItem_OwnerObjId[i] = idOwner[i];
	}
	m_posDropItem_CreatePos = *pCreatePos;

	m_ProtectTime = time;
	m_bIsCanPick = isCanPick;

	return TRUE;
}

LPCSTR CObject_Character::GetWeaponName( ENUM_WEAPON_TYPE wType )
{
	// �����������ͷ�����Ӧ���ַ�
	// �߻����Ƶ���������

/*
	switch( wType )
	{
	case WEAPON_TYPE_NONE:
		return "ͽ��";
		break;
	case WEAPON_TYPE_FALCHION:
		return "��";
		break;
	case WEAPON_TYPE_SWORD:
		return "��";
		break;
	case WEAPON_TYPE_BOW:
		return "����";
		break;
	case WEAPON_TYPE_SPEAR:
		return "��ǹ";
		break;
	case WEAPON_TYPE_GAVEL:
		return "�";
		break;
	case WEAPON_TYPE_STAFF:
		return "��";
		break;
	case WEAPON_TYPE_HOLD:
		return "˫��";
		break;
	case WEAPON_TYPE_DOUBLE:
		return "˫��";
		break;
	case WEAPON_TYPE_FLAG:
		return "����";
		break;
	case WEAPON_TYPE_LONG:
		return "����";
		break;
	case WEAPON_TYPE_NPC:
		return "NPC";
		break;
	};
*/

	return "";
}

VOID CObject_Character::ChangerWeaponEffect( tEntityNode::eWEAPATTR ePart, LPCTSTR szEffectName, LPCTSTR nLocatorName, UINT color )
{
	if( NULL == m_pRenderInterface )
		return ;

	//test ���Ա����пո�
	if( (0 == strcmp(nLocatorName, " ")) || (0 == strcmp(szEffectName, " ")) )
	{
		KLThrow( _T( " װ�����еĹҽ���Ч��󶨵����˿ո� ��" ) );
	}


	if( (NULL == szEffectName) || (NULL == nLocatorName) )
		return ;
	if( (0 == strcmp(szEffectName, "")) || (0 == strcmp(nLocatorName, "")) )
		return ;
	if( (0 == strcmp(szEffectName, "-1")) || (0 == strcmp(nLocatorName, "-1")) )
		return ;


	switch( ePart )
	{
	case tEntityNode::WEAP_RIGHT:
		m_pRenderInterface->SetRightWeaponEffect( szEffectName, nLocatorName, color);
		break;
	case tEntityNode::WEAP_LEFT:
		m_pRenderInterface->SetLeftWeaponEffect( szEffectName,nLocatorName, color);
		break;
	case tEntityNode::WEAP_LEFT_SHIELD:
		break ;
	}
}

GUID_t	CObject_Character::GetGUID(VOID) 
{
	if (GetCharacterData())
	{
		return GetCharacterData()->Get_GUID();
	}
	return INVALID_GUID;
}

BOOL CObject_Character::CreateBloodStain( VOID )
{
	//if( m_pBloodStain )
	//{
	//	m_pBloodStain->Destroy();
	//	m_pBloodStain = NULL;
	//}

	//fVector3 pos = GetPosition();

	//// ����
	//m_pBloodStain = (CObject_ProjTex_BloodStain*)CObjectManager::GetMe()->NewObject( "CObject_ProjTex_BloodStain" );
	//m_pBloodStain->Initial( NULL );
	//m_pBloodStain->SetParameter( 1.0f, fVector2(pos.x, pos.z) );

	return TRUE;
}

// 20100415 AddCodeBegin
VOID CObject_Character::SetIsAutoNPCPath( INT nSceneID, STRING strNPCName, BOOL isAutoPath )	 // ����NPC�Զ�Ѱ·���ܵ��������
{
	
	if ( strNPCName.empty() && isAutoPath)
		return;

	m_ObjAutoPath.m_isAutoPath     = isAutoPath;
	m_ObjAutoPath.m_strAutoNPCName = strNPCName;
	m_ObjAutoPath.m_nSceneID       = nSceneID;
	
}
// 20100415 AddCodeEnd  


// 20100416 AddCodeBegin
VOID CObject_Character::QuestOpenNPCDlg()	 // �����NPC�Ի�����
{
	if( !IsMySelf(GetServerID()))
	{
		return;
	}
	// �Զ�Ѱ·�������������������б����󣬴�NPC�Ի���
	if ( m_ObjAutoPath.m_isAutoPath == TRUE  && CGameProcedure::s_pWorldManager->GetActiveSceneID() == m_ObjAutoPath.m_nSceneID)
	{
		CObject_Character* pCharObj  = NULL;
		pCharObj  = CObjectManager::GetMe()->FindCharacterByName( m_ObjAutoPath.m_strAutoNPCName );

		if (  pCharObj != NULL )
		{
			int i = pCharObj->GetServerID();
			CGameProcedure::s_pGameInterface->Player_Speak( i );

			m_ObjAutoPath.m_isAutoPath = FALSE;
			m_ObjAutoPath.m_strAutoNPCName = "";
			m_ObjAutoPath.m_nSceneID = -1;
		}		
	}
}
// 20100416 AddCodeEnd  


// ��̯�󣬻ָ���̯ǰ��ͷ����Ϣ��״̬	20100527 BLL
VOID CObject_Character::RestoreInfoBoard( VOID )
{
	OnDataChanged_CountryFlag();

	OnDataChanged_Name();

	OnDataChanged_Level();

	OnDataChanged_TeamLeaderFlag();

	SetBoardTitleState( TRUE, GetCharacterData()->Get_CurTitleType() );

	OnDataChanged_HP();
}

VOID CObject_Character::PlayAnimation(INT nSetID, FLOAT fStartTime, FLOAT fWeight)
{
	LPCSTR lpszCharActionName = NULL;
	if(m_pRenderInterface != NULL)
	{
		lpszCharActionName = GetCharActionNameByActionSetID(nSetID, GetWeaponType( WL_RIGHT ));
		if(lpszCharActionName == NULL)
			return;

		m_pRenderInterface->Actor_PlayAnimation(lpszCharActionName, fStartTime, fWeight);
	}
}

VOID CObject_Character::OnBulletHit()
{
	DisplayHitAnimation();
}