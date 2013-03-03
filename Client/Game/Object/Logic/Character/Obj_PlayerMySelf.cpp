
#include "StdAfx.h"
#include "..\..\ObjectCommandDef.h"
#include "NetWork\NetManager.h"
#include "GameCommand.h"
#include "..\..\Manager\ObjectManager.h"
#include "Procedure\GameProcedure.h"
#include "Obj_PlayerMySelf.h"
#include "AI\GMAI_MySelf.h"
#include "event\GMEventSystem.h"
#include "FakeObject\GMFakeObjSystem.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GIUtil.h"
#include "GIException.h"
#include "GISound.h"
#include "GIActionSystem.h"
#include "GIGameInterface.h"
#include "GIVariable.h"
#include "DataPool\GMDP_Struct_Skill.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GameStruct_Skill.h"
#include "Obj_PlayerMySelf.h"
#include "Procedure\GameProcedure.h"
#include "Interface\GMInterface_Script_Talk.h"

#include "GIVariable.h"
#include "CGEnterScene.h"
#include "..\..\..\datapool\GMDP_CharacterData.h"
#include "..\..\..\action\GMActionSystem.h"
#include "GIActionSystem.h"
#include "..\..\..\Procedure\GamePro_Enter.h"


KL_IMPLEMENT_DYNAMIC(CObject_PlayerMySelf, GETCLASS(CObject_PlayerOther));


CObject_PlayerMySelf::CObject_PlayerMySelf( VOID )
{
	// 初始化AI
	m_pTheAI = new CAI_MySelf(this);

	m_pAvatar = NULL;
	m_pAvatar2 = NULL;
	m_pAvatarArm = NULL;
}

CObject_PlayerMySelf::~CObject_PlayerMySelf( VOID )
{
	SAFE_DELETE(m_pTheAI);	
}



VOID CObject_PlayerMySelf::Initial(VOID* pParam)
{
	CObject_PlayerOther::Initial(pParam);

	//创建Avatar
	m_pAvatar  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerOther", "Player", "Camera_Main"));

	m_pAvatar2  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerOther", "Player2", "Camera_Main"));

	m_pAvatarArm  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerOther", "PlayerArm", "Camera_Main"));

	m_pAvatarPet = (CObject_PlayerNPC*)( CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerNPC", "PlayerPet" , "Camera_Main" ) );
	
	m_pAvatarZuoJi = (CObject_PlayerNPC*)( CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerNPC", "PlayerZuoji" , "Camera_Main" ) );

	//616======* 单机人物创建
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1)
	{
		GetCharacterData()->Set_ModelID(0);
		GetCharacterData()->Set_RaceID(1);
		GetCharacterData()->Set_HairColor(0);
		GetCharacterData()->Set_MoveSpeed(3.5f);
	}

	//设置初始方向
	SetFaceDir((FLOAT)KLU_PI);
}

VOID CObject_PlayerMySelf::Release(VOID)
{
	CFakeObjSystem::GetMe()->DestroyObject("PlayerZuoji");
	CFakeObjSystem::GetMe()->DestroyObject("PlayerPet");
	CFakeObjSystem::GetMe()->DestroyObject("PlayerArm");
	CFakeObjSystem::GetMe()->DestroyObject("Player2");
	CFakeObjSystem::GetMe()->DestroyObject("Player");
	CFakeObjSystem::GetMe()->DestroyObject("Player_Head");
	m_pAvatar = NULL;
	m_pAvatar2 = NULL;
	m_pAvatarArm = NULL;
AxTrace(0,0, "CObject_PlayerMySelf::Release");
	CObject_PlayerOther::Release();
}


BOOL CObject_PlayerMySelf::IsInVisualFiled( CObject_Character* pObj )
{
	KLAssert(pObj);

	const iVector2& vThat = pObj->GetMapPosition();
	const iVector2& vThis = GetMapPosition();

	if(	abs(vThat.x - vThis.x) > 1 ||
		abs(vThat.y - vThis.y) > 1 )
		return FALSE;
	else
		return true;
}

// 左键指令的分析
BOOL CObject_PlayerMySelf::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActionItem )
{
	if(pActionItem && pActionItem->GetType() == AOT_ITEM)
	{
		//鼠标上挂的是一个物品
		return CObject_PlayerNPC::FillMouseCommand_Left(pOutCmd, pActionItem);
	}
	else
	{
		pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	}
	return TRUE;
}

// 右键指令的分析
BOOL CObject_PlayerMySelf::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_CONTEXMENU;
	pOutCmd->m_adwParam[0]	= GetServerID();
	return TRUE;
}

VOID CObject_PlayerMySelf::OnDataChanged_RaceID(VOID)
{
	// 创建Avatar外形
	m_pAvatar->GetCharacterData()->Set_RaceID( GetCharacterData()->Get_RaceID() );
	m_pAvatar->SetFaceDir(0.0f);

	m_pAvatar2->GetCharacterData()->Set_RaceID( GetCharacterData()->Get_RaceID() );
	m_pAvatar2->SetFaceDir(0.0f);

	m_pAvatarArm->GetCharacterData()->Set_RaceID( GetCharacterData()->Get_RaceID() );
	m_pAvatarArm->SetFaceDir(0.0f);

	CObject_PlayerOther::OnDataChanged_RaceID();

	// 之前缓冲的坐骑
	if (INVALID_ID != GetCharacterData()->Get_MountID())
	{
		UpdateMountModel();
	}
	if(CGameProcedure::s_pProcEnter->GetEnterType() != ENTER_TYPE_FIRST )
	{
		//如果玩家不是第一次登录，只是在场景间跳转，则只需要从客户端获得装备列表的数据后Set_Equip即可

		CObject* m_pMySelf = (CObject*)CObjectManager::GetMe()->GetMySelf();
		CCharacterData* pCharacterData = ((CObject_Character*)m_pMySelf)->GetCharacterData();

		for(INT i=0; i<HEQUIP_NUMBER; i++)
		{
			tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(i, "equip");
			if(pActionItem != NULL)
			{
				tObject_Item* pItem = (tObject_Item*)pActionItem->GetImpl();
				INT nID = pItem->GetIdTable();

				pCharacterData->Set_Equip((HUMAN_EQUIP)i,nID);
			}
		}
	}
}

VOID CObject_PlayerMySelf::OnDataChanged_HairMesh(VOID)
{
	m_pAvatar->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());

	m_pAvatar2->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());

	m_pAvatarArm->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());

	CObject_PlayerOther::OnDataChanged_HairMesh();
}

VOID CObject_PlayerMySelf::OnDataChanged_HairColor(VOID)
{
	m_pAvatar->GetCharacterData()->Set_HairColor(GetCharacterData()->Get_HairColor());

	m_pAvatar2->GetCharacterData()->Set_HairColor(GetCharacterData()->Get_HairColor());

	m_pAvatarArm->GetCharacterData()->Set_HairColor(GetCharacterData()->Get_HairColor());

	CObject_PlayerOther::OnDataChanged_HairColor();
}

VOID CObject_PlayerMySelf::OnDataChanged_FaceMesh(VOID)
{
	m_pAvatar->GetCharacterData()->Set_FaceMesh(GetCharacterData()->Get_FaceMesh());
	m_pAvatar2->GetCharacterData()->Set_FaceMesh(GetCharacterData()->Get_FaceMesh());
	m_pAvatarArm->GetCharacterData()->Set_FaceMesh(GetCharacterData()->Get_FaceMesh());

	CObject_PlayerOther::OnDataChanged_FaceMesh();
}

VOID CObject_PlayerMySelf::OnDataChanged_Equip(HUMAN_EQUIP point)
{
	m_pAvatar->GetCharacterData()->Set_EquipGem( point,GetCharacterData()->Get_EquipGem(point) );
	m_pAvatar->GetCharacterData()->Set_Equip( point, GetCharacterData()->Get_Equip(point) );

	m_pAvatar2->GetCharacterData()->Set_EquipGem( point,GetCharacterData()->Get_EquipGem(point) );
	m_pAvatar2->GetCharacterData()->Set_Equip( point, GetCharacterData()->Get_Equip(point) );

	m_pAvatarArm->GetCharacterData()->Set_EquipGem( point,GetCharacterData()->Get_EquipGem(point) );
	m_pAvatarArm->GetCharacterData()->Set_Equip( point, GetCharacterData()->Get_Equip(point) );

	CObject_PlayerOther::OnDataChanged_Equip(point);
}

VOID CObject_PlayerMySelf::OnDataChanged_Level(VOID)
{
	SCRIPT_SANDBOX::Talk::s_Talk.SetTalkRule();

	INT nLevel = GetCharacterData()->Get_Level();
	m_pInfoBoard->SetElement_Level( nLevel, 0 );
}

VOID CObject_PlayerMySelf::OnDataChanged_HP(VOID)
{
	INT hp = GetCharacterData()->Get_HP();
	INT maxHp = GetCharacterData()->Get_MaxHP();

	if( hp != 0 && maxHp != 0 )
		m_pInfoBoard->SetElement_HPProgress( hp, maxHp );
}

VOID CObject_PlayerMySelf::OnDataChanged_CurTitles( INT nType )
{
	if( !m_pInfoBoard ) 
		return;

	LPCTSTR szTitle = m_pCharacterData->Get_CurTitle( nType );
//	if( NULL == szTitle || strlen(szTitle) == 0 )
//	{
//		SetElement_Title( "",_TITLE::NO_TITLE );
//		return;
//	}

	SetElement_Title( szTitle, nType );

}

VOID CObject_PlayerMySelf::OnDataChanged_Profession(VOID)
{
	m_pAvatar->GetCharacterData()->Set_MenPai( GetCharacterData()->Get_MenPai() );
	m_pAvatar2->GetCharacterData()->Set_MenPai( GetCharacterData()->Get_MenPai() );
	m_pAvatarArm->GetCharacterData()->Set_MenPai( GetCharacterData()->Get_MenPai() );
}


VOID CObject_PlayerMySelf::SetPosition(const fVector3& fvPosition)
{
	CObject_PlayerOther::SetPosition( fvPosition );
	CGameProcedure::m_bNeedFreshMinimap = true;
}

VOID CObject_PlayerMySelf::OnDataChanged_FaceImage(VOID)
{
	m_pAvatar->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());
	m_pAvatar2->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());
	m_pAvatarArm->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());
	CObject_PlayerOther::OnDataChanged_HairMesh();
}

VOID CObject_PlayerMySelf::OnDataChanged_Die(VOID)
{
	CObject_PlayerOther::OnDataChanged_Die();
	if(GetCharacterData()->IsDie())
	{
		//进入死亡效果
		CGameProcedure::s_pGfxSystem->Scene_SetPostFilter_Death(true);
	}
	else
	{
		CGameProcedure::s_pGfxSystem->Scene_SetPostFilter_Death(FALSE);
	}
}

VOID CObject_PlayerMySelf::Tick_UpdateInfoBoard(VOID)
{
	// 隐藏中，不再计算
	if( !IsEnable( OSF_VISIABLE ) || !GetRenderInterface() ) 
		return;

	if( !m_pInfoBoard )
		return;


/*
	if (CharacterLogic_Get() == CHARACTER_LOGIC_DEAD && g_theKernel.IsKindOf(GetClass(), GETCLASS(CObject_PlayerNPC)))
	{
		m_pInfoBoard->Show(FALSE);
		return ;
	}
*/


	// 计算位置
	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
	fVector3 fvCurPos = GetPosition();
	if( m_pMountRenderInterface != NULL )
	{
		fvCurPos.y += m_fMountAddHeight;
	}

	BOOL bVisible;
	bVisible = GetRenderInterface()->Actor_GetInfoBoardPos( fvPos, &fvCurPos );
	if( !bVisible )
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}


	// 设置离主角的距离
	fVector3 fMyselfPos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
	FLOAT fDistance = KLU_GetDistSq( CObjectManager::GetMe()->GetMySelf()->GetPosition(), GetPosition() );

	m_fvInfoBoardPos = fvPos;
	float Pos_X = m_pInfoBoard->GetPosition().x;

	if( (fvPos.x - Pos_X < 10) && (fvPos.x - Pos_X > -10) )
		// 为了不晃动
        m_pInfoBoard->SetPosition(Pos_X, fvPos.y);
	else
		m_pInfoBoard->SetPosition(fvPos.x,fvPos.y);


	//AxTrace(0, 1, "Pos_X=%d", Pos_X);
	//AxTrace(0, 1, "ivPos.x=%d", ivPos.x);


	m_pInfoBoard->SetInfoDistance( fDistance );

	// m_pInfoBoard->ShowCharAttr( TRUE );
	// 摆摊状态下，不显示等级	20100527 BLL
	if ( GetCharacterData()->Get_IsInStall() == TRUE )
	{
		// 玩家自己只要一摆摊则显示摆摊牌，其他玩家的摆摊牌在开市时才显示	20100706 BLL
		OnDataChanged_IsInStall();	
		SetBoardHpLevelState( FALSE );
	}
	else
		SetBoardHpLevelState( TRUE );

	m_pInfoBoard->Show(TRUE);
}

VOID CObject_PlayerMySelf::SetBoardTitleState( BOOL bShow, INT nType )
{
	// 设置是否显示称号
	m_pInfoBoard->ShowCharTitle( bShow, nType );
}

VOID CObject_PlayerMySelf::SetBoardHpLevelState( BOOL bShow )
{
	m_pInfoBoard->ShowCharAttr( bShow );
}

VOID CObject_PlayerMySelf::SetSouXiaUseSkill_id( SkillID_t id )
{
	if( !m_pAvatar2 ) return;

	m_pAvatar2->SetUseSkillID( id );
}

VOID CObject_PlayerMySelf::SetSouXiaPrewArm(INT ArmID, INT EquipPoint )
{
	//m_pAvatarArm->EquipItem_BodyLocator( id, loc );
	m_pAvatarArm->GetCharacterData()->Set_Equip( (HUMAN_EQUIP)EquipPoint, ArmID );
}

VOID CObject_PlayerMySelf::ClearSouXiaPrewArm()
{
	for( INT i=0; i<HEQUIP_NUMBER; ++i )
	{
		//m_pAvatarArm->UnEquipItem( (HUMAN_EQUIP)i );
		m_pAvatarArm->GetCharacterData()->Set_Equip( (HUMAN_EQUIP)i, GetCharacterData()->Get_Equip((HUMAN_EQUIP)i) );
	}
}

VOID CObject_PlayerMySelf::SetSouXiaPrewPet( INT modelId )
{
	m_pAvatarPet->GetCharacterData()->Set_RaceID( modelId );
}

VOID CObject_PlayerMySelf::SetSouXiaPrewZuoJi( INT modelId )
{
	m_pAvatarZuoJi->GetCharacterData()->Set_RaceID( modelId );
}


VOID CObject_PlayerMySelf::ResetBaseLogicCount(VOID)
{
	CObject_PlayerOther::ResetBaseLogicCount();
	m_nSimulantLogicCount	= -1;
}
VOID CObject_PlayerMySelf::ResetActionLogicCount(VOID)
{
	CObject_PlayerOther::ResetActionLogicCount();
	m_nSimulantLogicCount	= -1;
}