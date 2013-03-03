
/**	GamePro_CharCreate.cpp
 *	
 *	功能：	在登陆流程创建新的角色
 *
 *	修改记录：
 *			080220	在确定创建角色后， 增加将”职业“属性发到 LoginServer
 *	
 */

#include "StdAfx.h"
#include "Network\NetManager.h"
#include "GamePro_CharCreate.h"
#include "GamePro_CharSel.h"

#include "Input\GMInputSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "object\ObjectDef.h"
#include "object\Logic\character\Obj_PlayerOther.h"
#include "FakeObject\GMFakeObjSystem.h"
#include "object\Logic\Character\Obj_Character.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GIGfxSystem.h"
#include "Event\GMEventSystem.h"
#include "GameStruct.h"
#include "CLAskCreateChar.h"
#include "GIVariable.h"
#include "GIException.h"
#include "..\DBC\GMDataBase.h"



CGamePro_CharCreate::CGamePro_CharCreate()
{
	m_pAvatar[0] = NULL;
	m_pAvatar[1] = NULL;

	m_Status = CHAR_CREATE_CREATEING;

	m_iFaceColor1  = 0;
	m_iFaceModle1  = -1;
	m_iHairColor1  = 0;
	m_iHairModle1  = 0;
    m_iCountry1    = 0;
	m_iProfession1 = 0;
	m_iIsLoadFace = 0;

	m_iIsLoadFaceModel = 0;

	m_iIsLoadHairModel = 0;

	m_eGender		= GENDER_FEMALE;
	m_nUIViewProfession	= 0;

	//
	m_nDefHeadGeo	= -1;
	m_nDefShoulder	= -1;
	m_nDefBody		= -1;
	m_nDefArm		= -1;
	m_nDefFoot		= -1;

}

CGamePro_CharCreate::~CGamePro_CharCreate()
{
	if(m_pAvatar[0])
	{
		CFakeObjSystem::GetMe()->DestroyObject("CreateRole_Woman");
	}

	if(m_pAvatar[1])
	{
		CFakeObjSystem::GetMe()->DestroyObject("CreateRole_Man");
	}
}


VOID CGamePro_CharCreate::Init(VOID)
{
	//s_pGfxSystem->Scene_Load( -1, "yangliucun.scene" );
	s_pGfxSystem->Camera_SetEyePos   (fVector3(1000, 325, 1000));
	s_pGfxSystem->Camera_SetLookAtPos(fVector3(1000, 320, 0));
	
	if(0 == m_iIsLoadFace)
	{
		int res = m_FaceMng.GetCharacterFaceIfo();
		if(0 == res)
		{
			m_iIsLoadFace = 1;
		}
	}

	if(0 == m_iIsLoadFaceModel)
	{
		int res = m_FaceModelmng.GetCharacterModel();
		if(0 == res)
		{
			m_iIsLoadFaceModel = 1;
		}
	}

	if(0 == m_iIsLoadHairModel)
	{
		int res = m_HairModelMng.GetCharacterModel();
		if(0 == res)
		{
			m_iIsLoadHairModel = 1;
		}
	}

	// 创建模型信息.
	CreateModel();
}

VOID CGamePro_CharCreate::Tick(VOID)
{
	CGameProcedure::Tick();
	//CGameProcedure::ProcessActiveInput();
}

VOID CGamePro_CharCreate::Render(VOID)
{
	if(s_pGfxSystem) 
		s_pGfxSystem->RenderFrame();
}

VOID CGamePro_CharCreate::Release(VOID)
{
}

VOID CGamePro_CharCreate::MouseInput(VOID)
{
}

// 切换到选择人物界面
void CGamePro_CharCreate::ChangeToSelectRole()
{
	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 打开人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_CHARACTOR);

	// 关闭人物创建界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR);

	// 不清空显示模型。
	CGameProcedure::s_pProcCharSel->m_bClearUIModel = FALSE;
	//CGameProcedure::s_pProcCharSel->m_bClearUIModel = TRUE;

	//切换到人物选择界面
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);
}


VOID CGamePro_CharCreate::ProcessInput(VOID)
{
	
	//分析事件队列
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;
		if(event.nEvent != EVENT_KEY_UP) continue;
		switch(event.kIndex)
		{
		case KC_GRAVE:
			{
			}
			break;
		case KC_1:
			{	
				//-------------------------------------------------------
				//按键 '1', 回到人物选择流程.
				//CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);
			}
			break;
		case KC_2:
			{
				
			}
		
		default:
			break;
		}
	}

}

// 创建界面模型
int CGamePro_CharCreate::CreateModel()
{
	if( m_pAvatar[0] )
	{
		return TRUE;
	}

	fVector3 CharacterPos[ 2 ];
	fVector3 SrcPos[ 2 ];
	SrcPos[ 0 ] = fVector3(1000, 320, 1000);
	SrcPos[ 1 ] = fVector3( 1000, 320, 1000);

	s_pGfxSystem->Axis_Trans( s_pGfxSystem->AX_GFX, SrcPos[ 0 ],s_pGfxSystem->AX_GAME, CharacterPos[ 0 ] );
	s_pGfxSystem->Axis_Trans( s_pGfxSystem->AX_GFX, SrcPos[ 1 ],s_pGfxSystem->AX_GAME, CharacterPos[ 1 ] );

	// 创建Avatar
	m_pAvatar[0] = (CObject_PlayerOther*)( CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), _T("CreateRole_Woman"), "Camera_Main") );
	m_pAvatar[1] = (CObject_PlayerOther*)( CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), _T("CreateRole_Man"),   "Camera_Main") );

	// 女模型
	m_pAvatar[0]->SetFaceDir(0);
	m_pAvatar[0]->GetCharacterData()->Set_MenPai( CLIENT_GAOMO_MENPAI );
	m_pAvatar[0]->GetCharacterData()->Set_RaceID( 2 );
	//m_pAvatar[ 0 ]->SetPosition( CharacterPos[ 0 ] );
	//m_pAvatar[ 0 ]->SetScale(100);
	// 设置缺省头发颜色
//	m_pAvatar[0]->GetCharacterData()->Set_HairColor(0);|
	
	// 男模型 
	m_pAvatar[1]->SetFaceDir(0);
	m_pAvatar[1]->GetCharacterData()->Set_MenPai( CLIENT_GAOMO_MENPAI );
	m_pAvatar[1]->GetCharacterData()->Set_RaceID( 3 );
	//m_pAvatar[ 1 ]->SetPosition( CharacterPos[ 1 ] );
	//m_pAvatar[ 1 ]->SetScale(100);
	// 设置缺省头发颜色
//	m_pAvatar[1]->GetCharacterData()->Set_HairColor(0);

	return TRUE;
}


// 创建角色
int	CGamePro_CharCreate::CreateRole()
{
	const char* szInvalid = " #\\/`~!@~$%^&*()-_+=|{}[];:'\"<>,.?";
	const char* szHZBlank = "　"; //中文空格

	CLAskCreateChar msg;
	
	std::string strName = m_szCreateRoleName;
	if(strName.empty())
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_change_scene_Info_Name_Not_Null");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	
		return 1;
	}

	// 查找是否有非法ascii字符
	STRING::size_type nInvalidPos = strName.find_first_of(szInvalid);
	STRING strInvalidChar;
	if(nInvalidPos == STRING::npos)
	{
		// 中文空格
		nInvalidPos = strName.find(szHZBlank);
		strInvalidChar = szHZBlank;
	}
	else
	{
		// 非法字符
		strInvalidChar = strName.substr(nInvalidPos, 1);
	}

	if(nInvalidPos != STRING::npos)
	{
		STRING strTemp = NOCOLORMSGFUNC("INVALID_CHARACTER_IN_NAME", strInvalidChar.c_str());
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	
		return 0;
	}

	if(CGameProcedure::s_pUISystem)
	{
		if(!CGameProcedure::s_pUISystem->CheckStringFilter(strName, FT_ROLE)||(!CGameProcedure::s_pUISystem->CheckStringFullCompare(strName)))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}
	}

	msg.SetAccount((CHAR*)CGameProcedure::s_pVariableSystem->GetAs_String("User_Account").c_str());
	msg.SetName		( m_szCreateRoleName );	//设置角色名称
	msg.SetFaceColor( m_iFaceColor1 );		//设置面部颜色
	msg.SetFaceModel( m_iFaceModle1 );		//设置面部模型
	msg.SetHairColor( m_iHairColor1 );		//设置头发颜色
	msg.SetHairModel( m_iHairModle1 );		//设置头发模型
	msg.SetSex		( m_iSex );				//设置性别
	msg.SetHeadID	( m_iFaceId );			//设置头像id
	msg.SetProfession( m_iProfession1 );	//设置职业
	msg.SetCountry	(m_iCountry1);			//设置国家

	CNetManager::GetMe()->SendPacket(&msg);

	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("Login_Create_Role_Doing");
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());	

	return 0;
}

// 切换到角色选择界面
void CGamePro_CharCreate::ChangeToRoleSel()
{
	// 显示人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_CHARACTOR);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭人物创建界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR);

	// 不清空显示模型。
	CGameProcedure::s_pProcCharSel->m_bClearUIModel = FALSE;
	
	//切换到人物选择界面
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);
}

// 设置头像id
void CGamePro_CharCreate::SetFace(int iFaceId)
{
	m_iFaceId = iFaceId;
}

// 通过性别和索引设置头像id
void CGamePro_CharCreate::SetFaceByRaceAndIndex(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			SetFace(m_FaceMng.GetWomanFaceId(iIndex));
			break;
		}
	case 1:
		{
			SetFace(m_FaceMng.GetManFaceId(iIndex));
			break;
		}
	}
}

// 设置脸形id
void CGamePro_CharCreate::SetFaceModel(int iFaceModelId)
{
	m_iFaceModle1 = iFaceModelId;
}

// 通过性别和索引设置脸id
void CGamePro_CharCreate::SetFaceModelByRaceAndIndex(int iRace, int iIndex)
{
	int iRaceIndex = iRace;
	switch(iRace)
	{
	case 0:
		{
			SetFaceModel( m_FaceModelmng.GetWomanFaceModelId(iIndex) );//
			break;
		}
	case 1:
		{
			SetFaceModel( m_FaceModelmng.GetManFaceModelId(iIndex) );
			break;
		}
	}
	
	m_pAvatar[iRaceIndex]->GetCharacterData()->Set_FaceMesh( m_iFaceModle1 );
//	CObjectManager::GetMe()->GetActorAvatar(iRaceIndex)->GetCharacterData()->Set_FaceMesh(m_iFaceModle1);
}

// 通过性别和索引设置脸id
void CGamePro_CharCreate::PlayAnimation(int iGender, int iActionIndex)
{
	m_pAvatar[iGender]->ChangeAction(iActionIndex, FALSE);
}


// 设置头发id
void CGamePro_CharCreate::SetHairModel(int iHairModelId)
{
	m_iHairModle1 = iHairModelId;
}

// 通过性别和索引设置头发id
void CGamePro_CharCreate::SetHairModelByRaceAndIndex(int iRace, int iIndex)
{
	int iRaceIndex = iRace;

	switch(iRace)
	{
	case 0:
		{
			SetHairModel(m_HairModelMng.GetWomanHairModelId(iIndex));
			break;
		}
	case 1:
		{
			SetHairModel(m_HairModelMng.GetManHairModelId(iIndex));
			break;
		}
	}

	m_pAvatar[iRaceIndex]->GetCharacterData()->Set_HairMesh(m_iHairModle1);
//	CObjectManager::GetMe()->GetActorAvatar(iRaceIndex)->GetCharacterData()->Set_HairMesh(m_iHairModle1);
}



INT CGamePro_CharCreate::GetGender( VOID )
{
	return (INT)m_eGender;
}

VOID CGamePro_CharCreate::SetGender( INT nGender )
{
	m_eGender = (ENUM_GENDER)nGender;

	m_pAvatar[nGender]->GetCharacterData()->Set_MenPai( CLIENT_GAOMO_MENPAI );


	DBC_DEFINEHANDLE( s_pCharacterRace, DBC_CHARACTER_RACE );		// char_race.tab
	const _DBC_CHAR_RACE* pCharRace = ( const _DBC_CHAR_RACE* )( s_pCharacterRace->Search_Index_EQU( nGender +2) );
	if( !pCharRace )
		return ;


	m_nDefHeadGeo	= pCharRace->nDefHairGeo;
	m_nDefShoulder	= pCharRace->nDefShoulder;
	m_nDefBody		= pCharRace->nDefBody;
	m_nDefArm		= pCharRace->nDefArm;
	m_nDefFoot		= pCharRace->nDefFoot;

	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_HEAD, m_nDefHeadGeo );		// 头		item_char.tab
	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_SHOULDER, m_nDefShoulder );	// 肩
	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_BACK, m_nDefBody );			// 躯干
	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_HAND, m_nDefArm );			// 手
	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_FEET, m_nDefFoot );			// 脚
}


INT CGamePro_CharCreate::GetProfession( VOID )
{
	return m_nUIViewProfession;
}

VOID CGamePro_CharCreate::SetProfession( INT nProfession )
{
	m_nUIViewProfession = nProfession;

	m_pAvatar[m_eGender]->GetCharacterData()->Set_MenPai( m_nUIViewProfession );


	DBC_DEFINEHANDLE( s_pCharacterRace, DBC_CHARACTER_RACE );		// char_race.tab
	const _DBC_CHAR_RACE* pCharRace = ( const _DBC_CHAR_RACE* )( s_pCharacterRace->Search_Index_EQU( (INT)m_eGender ) );
	if( !pCharRace )
		return ;


	m_nDefHeadGeo	= pCharRace->nDefHairGeo;
	m_nDefShoulder	= pCharRace->nDefShoulder;
	m_nDefBody		= pCharRace->nDefBody;
	m_nDefArm		= pCharRace->nDefArm;
	m_nDefFoot		= pCharRace->nDefFoot;

	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_HEAD, m_nDefHeadGeo );		// 头		item_char.tab
	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_SHOULDER, m_nDefShoulder );	// 肩
	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_BACK, m_nDefBody );			// 躯干
	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_HAND, m_nDefArm );			// 手
	m_pAvatar[(INT)m_eGender]->EquipItem_BodyPart( HEQUIP_FEET, m_nDefFoot );			// 脚

}
