
/**	Obj_PlayerNPC.cpp
 *	
 *	功能：	
 *	
 *	修改记录：
 *			080221	将怪的武器特效的绑定点由id改成字符串， 由策划在表中直接输入名字
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

			// 设置ActorFile
			m_pRenderInterface->Actor_SetFile( lpszModelFileName );
		}
	}

	CObject_Character::CreateRenderInterface();

	// 挂武器
	OnDataChanged_Equip( HEQUIP_MAINHAND );
//	OnDataChanged_Equip( HEQUIP_ASSIHAND );
	// 加坐骑
	CObject_Character::OnDataChanged_MountID();

	// 加特效
	SetNpcEffect();

}

BOOL CObject_PlayerNPC::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;

	if( CanbeSelect() )
	{
		// 是物品
		if(pActiveSkill && pActiveSkill->GetType() == AOT_ITEM)
		{
			// 鼠标上挂的是一个物品

			// 空物品
			if(!pActiveSkill || pActiveSkill->GetType() != AOT_ITEM) 
				return TRUE;

			CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActiveSkill)->GetItemImpl());
			if(!pItem) 
				return  TRUE;

			// 必须是能够使用的物品
			if(pItem->GetItemClass() != ICLASS_COMITEM) 
				return  TRUE;
			
			// 是否能够使用
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
		// 不是物品
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
			case RELATION_FRIEND:	// 朋友
				if ( CannotBeAttack() )
				{
					pOutCmd->m_typeMouse = SCommand_Mouse::MCT_SPEAK;
					pOutCmd->m_adwParam[0] = GetServerID();
				}
				break;

			case RELATION_ENEMY:	// 敌人
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
	// 经过变身
	if( GetConstCharacterData()->Get_ModelID() != -1 )
	{
		// 直接从模型定义表中读取模型名
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
		// 打开生物定义表 mon_attr_ex.tab
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
	// 如果是任务NPC，设置其自身的方向
	if(GetCharacterType() == CT_MONSTER)
	{
		// 不是自己 也不是其他玩家 只能是NPC
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
				// 玩家的位置
				CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
				fvThis.x = pMySelf->GetPosition().x;
				fvThis.y = pMySelf->GetPosition().z;
				// fvTargent2为NPC的位置
				fvTarget2.x = GetPosition().x;
				fvTarget2.y = GetPosition().z;
				// 调用NPC对象的SetFaceDir,使其转向玩家
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

	// 处理NPC转向问题
	ProcessFaceToPlayer();

	return TRUE;
}


VOID CObject_PlayerNPC::OnDataChanged_Equip(HUMAN_EQUIP point)
{
	UpdateEquip(point);
}

VOID CObject_PlayerNPC::UpdateEquip( HUMAN_EQUIP point )
{
	// 只添加武器
	if( HEQUIP_MAINHAND != point )
		return ;

	// 获取人物数据
	CCharacterData *pCharacterData = GetCharacterData();
	if( pCharacterData == NULL )
		return ;


	// 这种方法的武器id需要从 mon_attr_ex.tab 中获取， 或从人物属性中获取

	// 取出武器id
	INT nID = pCharacterData->Get_MonstWeapon( );

	EquipItem_BodyLocator( nID, WL_BOTH );	// 这个参数不起作用， 只是编译
}


VOID CObject_PlayerNPC::EquipItem_BodyLocator( INT nID, ENUM_WEAPON_LOCATOR_TYPE loc )
{
	// 如果是怪的话可能会出现这种情况
	if( -1 == nID )
		return ;

	if( !m_pRenderInterface )
		return;

	// 怪
	if( CT_MONSTER != GetCharacterType() )
		return ;

	// 角色挂接类数据库( item_mon_loc.tab )
	DBC_DEFINEHANDLE(s_pWeaponItem, DBC_ITEM_MONSTER_VISUAL_LOCATOR);
	const _DBC_ITEM_MONSTER_VISUAL_LOCATOR* pEquipVisual = (_DBC_ITEM_MONSTER_VISUAL_LOCATOR*)s_pWeaponItem->Search_Index_EQU( nID );
	if( !pEquipVisual )
		return;

	// 设置武器类型
	if( (ENUM_WEAPON_TYPE)(pEquipVisual->pWeaponType) >= WEAPON_TYPE_NUMBERS )
		return ;

	SetWeaponType( (ENUM_WEAPON_TYPE)(pEquipVisual->pWeaponType) );


	//================
	//====== 挂接武器

	// 右手
	if( pEquipVisual->pObjFile_Right && pEquipVisual->pObjFile_Right[0] != '\0')
	{
		// 在渲染实体上绑武器
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_RIGHTWEAPON, pEquipVisual->pObjFile_Right );

		// 加载武器默认的特效
		for( int i = 0; i < MONSTER_EFFECT_MAX_NUM; ++i )
		{
			CObject_Character::ChangerWeaponEffect( tEntityNode::WEAP_RIGHT, 
													pEquipVisual->szREffect[i].szEffect, 
													pEquipVisual->szREffect[i].szBindPoint, 
													1 
												  );
		}

		// 标记右手持有武器
		m_bRightWeapon = TRUE;
	}
	else
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_RIGHTWEAPON, "" );
	}


	// 左手
	if( pEquipVisual->pObjFile_Left && pEquipVisual->pObjFile_Left[0] != '\0' )
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_LEFTWEAPON, pEquipVisual->pObjFile_Left );

		// 加载武器默认的特效
		for( int i = 0; i < MONSTER_EFFECT_MAX_NUM; ++i )
		{
			CObject_Character::ChangerWeaponEffect( tEntityNode::WEAP_LEFT, 
													pEquipVisual->szLEffect[i].szEffect, 
													pEquipVisual->szLEffect[i].szBindPoint, 
													1 
												  );
		}

		// 标记左手持有武器
		m_bLeftWeapon = TRUE;
	}
	else
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_CURRENT_LEFTWEAPON, "" );
	}


	// 左手盾
	if( pEquipVisual->pObjShieldFile && pEquipVisual->pObjShieldFile[0] != '\0' )
	{
		m_pRenderInterface->Actor_SetObjectProperty( szIDSTRING_SHIELD_RIGHTWEAPON, pEquipVisual->pObjShieldFile );

		// 加载武器默认的特效
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


	// 同时加载武器的动作
	// （不考虑多武器动作， 规则是：有右手就是其对应的动作， 否则有左手就对应到左手， 都没有就不考虑了， 盾牌也不考虑动作）
	if( INVALID_ID != pEquipVisual->nWeaponActor )	// 武器有动作
	{
		if( m_bRightWeapon || m_bLeftWeapon )
		{
			UpdateWeaponActionSetFile( pEquipVisual->pActorFile );
		}
	}

}

VOID CObject_PlayerNPC::UnEquipItem( HUMAN_EQUIP nPart )
{
	// 怪物的武器不用卸
	return ;
}


VOID CObject_PlayerNPC::SetNpcEffect()
{
	if( !m_pRenderInterface )
		return ;

	switch (GetNpcType())
	{
		// 骑宠特效
	case NPC_TYPE_MOUNT:
		{
			
		}		
		break;

		// 其他
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

	// 清掉单个
//	m_pRenderInterface->DelEffect( 0 );

	// 清除全部
	m_pRenderInterface->DelAllEffect();
}



VOID CObject_PlayerNPC::SetNpcMissionData()
{
	// 设置任务数据
	std::vector< _DBC_QUEST_LOG* >::iterator iter = QuestLogDataMgr::GetMe()->s_FilteredMissions.begin();

	for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++ )
	{
		INT    nRet1  = -1;
		INT    nRet2  = -1;

		// 20100511 ModifyCodeBegin  根据questlog.tab 新增交任务NPC列后修改
		nRet1 = strcmp( (*iter)->szNPCName, GetCharacterData()->Get_Name() );
		nRet2 = strcmp( (*iter)->szNPCCommit, GetCharacterData()->Get_Name() );

		if( nRet1 == 0 || nRet2 == 0 ) // 如果该NPC是接任务或交任务NPC
		{
			if ( strcmp( (*iter)->szNPCName, (*iter)->szNPCCommit ) == 0 ) // 如果该NPC既是交任务NPC,又是接任务NPC
			{
				AddNpcMission( (*iter)->nMissionID, 3 );
			}
			else
			{
				if ( nRet1 == 0 ) // 如果是接任务NPC
				{
					AddNpcMission( (*iter)->nMissionID, 1 );
				}
				else if ( nRet2 == 0 ) // 如果是交任务NPC
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
	// 无更改， 不做检测
 	if( !m_bMissStateChange )
	{
//		return m_eMissionState;
	}

	MISSION_STATE msState = MS_INVALID;
	
	const CDetailAttrib_Player* pAttrib = CUIDataPool::GetMe()->GetMySelfDetailAttrib();	

	// 20100510 ModifyCodeBegin
	// npc任务列表
	if( !m_vMissionData.empty() )
	{
		BOOL bRet; // NPC身上是否有可接任务

		bRet = CUIDataPool::GetMe()->IsNpcMissionCanAccept( GetCharacterData()->Get_Name() );  

		// 判断玩家身上的任务数量
 		INT nMissionNum = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission_Num();
		
		// 玩家身上没有任务
		if( nMissionNum == 0 )
		{		
			// 如果该NPC身上有可接任务
			if ( bRet == TRUE )
			{
				// 显示该npc为“可接任务状态”
				msState = MS_MISS;
				return msState;
			}			
		}
	
		// npc任务列表
		std::vector< NPC_MISSION_DATA >::iterator iter = m_vMissionData.begin();
		for( ; iter != m_vMissionData.end(); iter++ )
		{
			if ( iter->nMissionNPCState == 1 ) // 如果是接任务NPC继续
			{
				continue;
			}

			// 判断玩家身上是否有这个任务
			INT nIndex = pAttrib->GetMissionIndexByID( (*iter).nMissionId );

			// 有
			if( nIndex != UINT_MAX )
			{	
				INT nRet = pAttrib->IsMissionCanCommit( (*iter).nMissionId );
				// 是否已经完成
				if(  nRet == 1 )
				{				
					// 显示该npc为“有可交任务状态”
					msState = MS_HAVE_MISS_DONE;
					return msState;				
				}
				// 已接未完成
				else if( nRet == 0 )
				{				
					msState = MS_HAVE_MISS_NO_DONE;		
				}

			}//if( nIndex != UINT_MAX )

		}// for( ; iter != m_vMissionData.end(); iter++ )

		// 如果有已接未完成任务或者msState仍然为MS_INVALID,即该NPC只可能有接任务的职能
		if ( msState == MS_HAVE_MISS_NO_DONE || msState == MS_INVALID )
		{
			// 如果有其他可接任务
			if ( bRet == TRUE )
			{
				msState = MS_MISS;
			}
		}

	}//if( !m_vMissionData.empty() )
	// 20100510 ModifyCodeEnd
	
	// 更新状态
	m_eMissionState = msState;	

	// 更新标记
	m_bMissStateChange = FALSE;

	return msState;
}

