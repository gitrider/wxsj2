
/**	GamePro_Main.cpp
 *
 *	���ܣ�	��Ϸ������
 *
 *	�޸ļ�¼��
 *			080324	�޸�ˢ��С��ͼ
 *			
 *			
 */

#include "StdAfx.h"
#include "Gamepro_Main.h"
#include "Global.h"
#include "World\WorldManager.h"
#include "Network\NetManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Manager\ObjectManager.h"
#include "CGEnvRequest.h"
#include "CGCharAskEquipment.h"
#include "CGAskDetailEquipList.h"
#include "CGAskDetailMountList.h"  //  [7/8/2010 �¾���]
#include "CGCharAskBaseAttrib.h"
#include "CGAskDetailAttrib.h"
#include "CGAskDetailXinFaList.h"
#include "CGAskDetailSkillList.h"
#include "CGAskMissionList.h"
#include "CGAskTeamInfo.h"
#include "CGAskSetting.h"
#include "CGCharAllTitles.h"
#include "CGRelation.h"
#include "CGAskMail.h"
#include "CGBankMoney.h"
#include "CGCityAskAttr.h"
#include "Input\GMInputSystem.h"
#include "GISound.h"
#include "GIUtil.h"
#include "GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "Interface\GMGameInterface.h"
#include "Object\Logic\TripperObj\GMTripperObj_ItemBox.h"
#include "CGAskMyBagList.h"
#include "CGAskChangeScene.h"
#include "GCMissionList.h"
#include "CGMinorPasswd.h"
#include "GIDebuger.h"
#include "Cursor\CursorMng.h"
#include "DBC\GMDataBase.h"
#include "Object\Logic\ProjTex\Obj_ProjTex.h"
#include "CGAskDetailAbilityInfo.h"
#include "Action\GMActionSystem.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GIVariable.h"
#include "GamePro_Login.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "GIEventDefine.h"
#include "Sound\GMSoundSystem.h"
#include "GIUISystem.h"
#include "Object\Logic\Character\AI\GMAI_MySelf.h"
#include "Object\Logic\Character\AI\GMAICommandDef.h"
#include "FakeObject\GMFakeObjSystem.h"
#include "GITimesystem.h"
#include "AxProfile.h"
#include "Object\GMObjectLoadQueue.h"
#include "Object\Logic\Surface\Obj_Special.h"
#include "Object\Logic\Surface\Obj_Bullet.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "Interface\GMInterface_Script_PetInviteFriend.h"
#include "Object\Logic\Obj_Building.h"
#include "Helper\GMHelperSystem.h"
#include "Event\GMEventSystem.h"
#include "GamePro_Enter.h"
#include "CGEnterScene.h"

#include "DirectlyImpactMgr.h"
#include "Object/Logic/Item/Obj_Item_Equip.h"

#include "EffectSystem\FairyBulletFlowSystem.h"
#include "EffectSystem\FairyBulletFlowSystemManager.h"

#include "CGGuildApply.h"
#include "CGGuildJoin.h"
#include "CGGuild.h"

#include "CGManipulatePet.h"

//#include "..\client\cegui\cegui\include\CEGUI.h"
//#include "..\client\cegui\cegui\include\CEGUIIme.h"


#include "CGCountryInfoRequest.h"
#include "Network\NetManager.h"



bool gbTestMinimap = true;
int CGamePro_Main::m_nResetCamera = 0;

static FLOAT fTransDistance = 1.0f;
//Fairy::BulletFlowSystemManager *pBulletFlowSystemManager = NULL;

CGamePro_Main::CGamePro_Main()
{
	m_bCameraDraging = FALSE;
	m_pSkillArea = NULL;
	m_pActiveSkill = NULL;
	m_pMouseTarget = NULL;
	m_bAutoRun = FALSE;

	m_bInit = FALSE;

	m_bulletFlow = NULL;

	m_bShowOtherObj	= TRUE;
	m_bShowOtherObjBoard = TRUE;

	m_bIsLastCameraDraging = FALSE;

	//pBulletFlowSystemManager = new Fairy::BulletFlowSystemManager();
}

CGamePro_Main::~CGamePro_Main()
{
	//delete pBulletFlowSystemManager; pBulletFlowSystemManager = NULL;
	// PetInviteFriend
	SCRIPT_SANDBOX::PetInviteFriend::s_PetInviteFriend.DestoryPetList();
}


VOID CGamePro_Main::Init(VOID)
{
	// �Ѿ���ʼ����
	if( m_bInit )
		return ;

	m_pMySelf = CObjectManager::GetMe()->GetMySelf();
	INT m_nID = m_pMySelf->GetServerID();
	m_pActiveSkill = NULL;
	m_pSkillArea = NULL;
	m_pMouseTarget = NULL;
	m_bAutoRun = FALSE;
	m_bIsLastCameraDraging = FALSE;

	//------------------------------------------------------------------
	// ����LogicCount
//	m_pMySelf->ResetLogicCount();
	m_pMySelf->ResetBaseLogicCount();
	m_pMySelf->ResetActionLogicCount();

	//------------------------------------------------------------------
	// ��ռ��ض���
	CObjectManager::GetMe()->GetLoadQueue()->ClearAllTask();

	//------------------------------------------------------------------
	// ���ͻ�������
	CGEnvRequest msgEnvRequest;
	CNetManager::GetMe()->SendPacket(&msgEnvRequest);

	//------------------------------------------------------------------
	// ����������������

	// ���������Ϣ
	CGCharAskBaseAttrib msgMyBaseAttrib;
	msgMyBaseAttrib.setTargetID(m_nID);
	CNetManager::GetMe()->SendPacket(&msgMyBaseAttrib);

	// ��������
	CGAskDetailAttrib	msgMyAttrib;
	msgMyAttrib.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMyAttrib);

	////����װ��(���ڴ�����Ⱦ������)
	//CGCharAskEquipment msgAskMyEquip;
	//msgAskMyEquip.setObjID(m_nID);
	//CNetManager::GetMe()->SendPacket(&msgAskMyEquip);

	// �����
	CGAskDetailAbilityInfo msgAbility;
	msgAbility.setTargetID(m_nID );
	CNetManager::GetMe()->SendPacket(&msgAbility);

	// �ķ�
	CGAskDetailXinFaList	msgMyXinFa;
	msgMyXinFa.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMyXinFa);

	// ����
	CGAskDetailSkillList	msgMySkill;
	msgMySkill.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMySkill);

	// �������
	CUIDataPool::GetMe()->ClearTeamInfo();
	CGAskTeamInfo msgTeamInfo;
	msgTeamInfo.SetObjID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgTeamInfo);

	//����ǳ���������������������
	CGCityAskAttr	MsgCityBuilding;
	CNetManager::GetMe()->SendPacket( &MsgCityBuilding );

	// ���гƺ�
	CGCharAllTitles	msgAllTitles;
	msgAllTitles.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgAllTitles);

	// �ǵ�һ�ε�½
	if( CGameProcedure::s_pProcEnter->GetEnterType() == ENTER_TYPE_FIRST )
	{
		// ���������Ƿ��Ѿ�����
		CGMinorPasswd MsgMinorPasswd;
		MsgMinorPasswd.GetMinorPassword()->CleanUp();
		MsgMinorPasswd.GetMinorPassword()->m_Type = MREQT_PASSWDSETUP;
		CNetManager::GetMe()->SendPacket(&MsgMinorPasswd);

		// ���������Ƿ���ǿ�ƽ���׶�
		CGMinorPasswd MsgDelMinorPasswd;
		MsgDelMinorPasswd.GetMinorPassword()->CleanUp();
		MsgDelMinorPasswd.GetMinorPassword()->m_Type = MREQT_DELETEPASSWDTIME;
		CNetManager::GetMe()->SendPacket(&MsgDelMinorPasswd);

		// ��ϸװ��(���������ݳ��д洢��Ʒ��ϸ����)
		CGAskDetailEquipList msgAskMyDetailEquip;
		msgAskMyDetailEquip.setObjID( m_nID );
		msgAskMyDetailEquip.SetAskMode(ASK_EQUIP_ALL);
		CNetManager::GetMe()->SendPacket(&msgAskMyDetailEquip);

		// ����
		CGAskMyBagList	msgMyBag;
		msgMyBag.SetAskMode(ASK_ALL);
		CNetManager::GetMe()->SendPacket(&msgMyBag);

		// ����  [7/8/2010 �¾���]
		CGAskDetailMountList msgAskMyDetailMount;
		msgAskMyDetailMount.setObjID( m_nID );
		CNetManager::GetMe()->SendPacket(&msgAskMyDetailMount);

		// MissionList
		CGAskMissionList	msgMissionList;
		msgMissionList.setTargetID( m_nID );
		CNetManager::GetMe()->SendPacket(&msgMissionList);

		// �������������������
		CGAskSetting msgSetting;
		CNetManager::GetMe()->SendPacket( &msgSetting );

		// ������������ϵ���б�
		CGRelation MsgRelation;
		MsgRelation.GetRelation()->m_Type = REQ_RELATIONLIST;
		CNetManager::GetMe()->SendPacket( &MsgRelation );

		// ������������ʼ�
		CGAskMail MsgMail;
		MsgMail.SetAskType( ASK_TYPE_LOGIN );
		CNetManager::GetMe()->SendPacket( &MsgMail );

		// ����������������еĽ�Ǯ����
		CGBankMoney	MsgBankMoney;
		MsgBankMoney.SetIsSave(CGBankMoney::UPDATE_MONEY);
		CNetManager::GetMe()->SendPacket( &MsgBankMoney );
	}

	s_pGfxSystem->Camera_SetZoom(fTransDistance);

	// Talk
	SCRIPT_SANDBOX::Talk::s_Talk.Initial();
	// PetInviteFriend
	SCRIPT_SANDBOX::PetInviteFriend::s_PetInviteFriend.InitPetList();


	// ����������ʾ״̬
	if(CGameProcedure::s_pProcEnter->GetEnterType() == ENTER_TYPE_FIRST)
	{
		static BOOL bFirst = TRUE;
		if(bFirst)
		{
			const char* szNeedReset[] = 
			{
				"View_FullScreen",		//ȫ��ģʽ
				"View_Resoution",		//�ֱ���
//				"View_FullScreenLight",	//ȫ������Ч��
			};
			for(int i=0; i<sizeof(szNeedReset)/sizeof(const char*); i++)
			{
				CEventSystem::GetMe()->PushEvent(
											GE_VARIABLE_CHANGED, 
											szNeedReset[i], 
											CGameProcedure::s_pVariableSystem->GetAs_String(szNeedReset[i]).c_str()
											);
			}
		}

		bFirst = FALSE;
	}

	// ����䴰�ڴ�С��
	LONG lStyle = ::GetWindowLong(g_hMainWnd, GWL_STYLE);
	lStyle = lStyle | WS_THICKFRAME ;
	lStyle = lStyle | WS_MAXIMIZEBOX ;
	::SetWindowLong(g_hMainWnd, GWL_STYLE, lStyle);//

	// Rresh title
	::PostMessage(g_hMainWnd, WM_NCACTIVATE, TRUE, 0);

	ShowWindow(g_hMainWnd, SW_SHOW);

	m_bInit = TRUE;
}

VOID CGamePro_Main::Tick(VOID)
{
	CGameProcedure::Tick();
	//AxProfile::AxProfile_PushNode("main");
	if( m_pMySelf != NULL )
	{

//---- for debug
		CHAR szTemp[MAX_PATH] = {0};

		//�����ٶ�
		_snprintf(szTemp, MAX_PATH, "%.2f", m_pMySelf->GetCharacterData()->Get_MoveSpeed());
		CGameProcedure::s_pGfxSystem->SetDebugString("Speed", szTemp);

		//��ʾλ��
		fVector3 fvPosGfx;
		CGameProcedure::s_pGfxSystem->Axis_Trans(tGfxSystem::AX_GAME, m_pMySelf->GetPosition(), tGfxSystem::AX_GFX, fvPosGfx);

		_snprintf(szTemp, MAX_PATH, "%.1f, %.1f", m_pMySelf->GetPosition().x, m_pMySelf->GetPosition().z);
		CGameProcedure::s_pGfxSystem->SetDebugString("Position", szTemp);

		static UINT nTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
		static UINT	nTalkTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();


		if( gbTestMinimap )
		{
			// ���������Ҫˢ��С��ͼ
			if( CGameProcedure::m_bNeedFreshMinimap ) 
			{
				if( CGameProcedure::s_pTimeSystem->GetTimeNow() - nTimer > 100 ) // ÿ��ˢ10��
				{
					CWorldManager::GetMe()->setMinimapSerachRange( 3 );
					CWorldManager::GetMe()->MinimapNeedFresh();
					CWorldManager::GetMe()->UpdateMinimapData();
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_MAP);	
					nTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
					CGameProcedure::m_bWaitNeedFreshMinimap = false;
					CGameProcedure::m_bNeedFreshMinimap		= false;
				}
			}
			// ������������ı�Ķ�������
			else if( CGameProcedure::m_bWaitNeedFreshMinimap ) 
			{
				if( CGameProcedure::s_pTimeSystem->GetTimeNow() - nTimer > 300 ) // ÿ��ˢ3��
				{
					CWorldManager::GetMe()->setMinimapSerachRange( 3 );
					CWorldManager::GetMe()->MinimapNeedFresh();
					CWorldManager::GetMe()->UpdateMinimapData();
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_MAP);	
					nTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
					CGameProcedure::m_bWaitNeedFreshMinimap = false;
				}
			}
		}


		if(CGameProcedure::s_pTimeSystem->GetTimeNow() - nTalkTimer > 1000)
		{
			CEventSystem::GetMe()->PushEvent(GE_TIME_UPDATE);
			nTalkTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
		}

		// ��Ӱ����λ������
		if( m_nResetCamera != 0 ) 
		{
			s_pGfxSystem->Camera_AddDirection( CGameProcedure::s_pTimeSystem->GetDeltaTime() / 1000 * m_nResetCamera ); 
			if( m_nResetCamera < 0 )
			{
				if( s_pGfxSystem->Camera_GetDirection() < __PI ) // ����Ѿ�����������
				{
					s_pGfxSystem->Camera_Direction( __PI );
					m_nResetCamera = 0;
				}
			}
			else 
			{
				if( s_pGfxSystem->Camera_GetDirection() > __PI ) 
				{
					s_pGfxSystem->Camera_Direction( __PI );
					m_nResetCamera = 0;
				}
			}
		}
	}

	//AxProfile::AxProfile_PopNode("main");
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Daytips"))
	{
		time_t  clientTime;
		time(&clientTime);

		time_t serverTime = CGameProcedure::s_pTimeSystem->GetServerTime();
		if( serverTime != 0 )
			clientTime = serverTime;

		tm* etm = localtime(&clientTime);

		// clear sting stream
		std::ostringstream strStream;
		strStream.str("");

		// write date
		strStream	<< std::setw(4) << (1900 + etm->tm_year) << '-'					// year
			<< std::setfill('0') << std::setw(2) << 1 + etm->tm_mon << '-'	// month
			<< std::setfill('0') << std::setw(2) << etm->tm_mday << ' ';	// day

		// write time
		strStream	<< std::setfill('0') << std::setw(2) << etm->tm_hour << ':'		// hour
			<< std::setfill('0') << std::setw(2) << etm->tm_min << ':'		// minute
			<< std::setfill('0') << std::setw(2) << etm->tm_sec << ' ';		// second

		STRING str( strStream.str() );

		CEventSystem::GetMe()->PushEvent( GE_UPDATE_DATE_TIME, str.c_str() );	
	}
}

VOID CGamePro_Main::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_Main::ProcessInput(VOID)
{
	ProcessInput_Keybord();
	ProcessInput_Mouse();
}



//�������
VOID CGamePro_Main::ProcessInput_Keybord(VOID)
{
	static INT CanDieUseKey[] ={KC_ESCAPE, KC_SYSRQ};

	CObject_PlayerMySelf *pMySelf	= CObjectManager::GetMe()->GetMySelf();

	//-----------------------------------------------------------
	// ���������Ⱦ�������δ�������ݲ������κμ�����Ϣ
	if(pMySelf == NULL || pMySelf->GetRenderInterface() == NULL) 
		return;

	// �����¼�����
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) 
			continue;

		// ����������ʱ���ܴ���Ŀ�ݼ��ŷ���
		if(pMySelf->IsDie() && (event.nEvent==EVENT_KEY_DOWN || event.nEvent == EVENT_KEY_UP))
		{
			BOOL bCanDieUse = FALSE;
			for(INT i = 0; i < sizeof(CanDieUseKey)/sizeof(INT); ++i)
			{
				if(CanDieUseKey[i] == event.kIndex) 
				{
					bCanDieUse = TRUE;
					break;
				}
			}

			// ����ʹ�õļ�
			if(!bCanDieUse)
			{
				event.bProcessed = TRUE;
				continue;
			}
		}

		Handle_AccelerateKey( event );

		Handle_Combination_Key( event );

	}

}

// ������̵�������
VOID CGamePro_Main::Handle_AccelerateKey(InputEvent& event)
{
	if( CGameProcedure::s_pUISystem->HasInputFocus() )
	{

		if( event.nEvent == EVENT_KEY_DOWN )
		{
			switch( event.kIndex )
			{
			//case KC_LEFT:
			//	CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "kc_left");
			//	break;

			//case KC_RIGHT:
			//	CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "kc_right");
			//	break;
			case KC_PGUP:
				SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_PageUp();
				break;
			case KC_PGDOWN:
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_PageDown();
				break;
			case KC_UP:
				if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
				{
					s_pGfxSystem->Camera_SetZoom( true, 2 );
				}
				else
				{
					if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
					{
						SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowUp(0);
					}
				}
				break;
			case KC_DOWN:
				if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
				{
					s_pGfxSystem->Camera_SetZoom( FALSE, 2 );
				}
				else
				{
					if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
					{
						SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowDown(0);
					}
				}
				break;
			}
		}

		return;
	}


	if(event.nEvent == EVENT_KEY_DOWN)
	{
		switch(event.kIndex)
		{
			// ����������ݼ�
		//case KC_F1:
		//case KC_F2:
		//case KC_F3:
		//case KC_F4:
		//case KC_F5:
		//case KC_F6:
		//case KC_F7:			
		//	CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_F1 + 6, true);
		//	break;
		case KC_7:
		case KC_8:
		case KC_9:
			CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex - KC_1, FALSE);
			break;
		case KC_0:
			CActionSystem::GetMe()->MainMenuBar_DoAction(9, FALSE);
			break;
		//case KC_MINUS:
		//	CActionSystem::GetMe()->MainMenuBar_DoAction(10, FALSE);
		//	break;
		//case KC_EQUALS:
		//	CActionSystem::GetMe()->MainMenuBar_DoAction(11, FALSE);
		//	break;

		case KC_A:
			break;
		case KC_B:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "b");
			break;
		case KC_C:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "c");
			break;
		case KC_D:
			break;
		case KC_E:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "e");
			break;
		case KC_F:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "f");
			break;
		case KC_G:	
			break;
		case KC_H:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "h");
			break;
		case KC_I:
			break;
		case KC_J:
			break;
		case KC_K:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "k");
			break;
		case KC_L:
			break;
		case KC_M:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "m");
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_CLOSESCENEMAP);
			break;
		case KC_N:
			break;
		case KC_O:
			break;
		case KC_P:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "p");
			break;
		case KC_Q:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "q");
			break;
		case KC_R:
			break;
		case KC_S:
			break;
		case KC_T:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "t");
			break;
		case KC_U:
			break;
		case KC_V:
			break;
		case KC_W:
			break;
		case KC_X:
			break;
		case KC_Y:
			break;
		case KC_Z:
			break;

		case KC_1:
			if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_1");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_1");
			}
			else
			{
				CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_1, TRUE);
			}
			break;

		case KC_2:
			if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_2");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_2");
			}
			else
			{
				CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_1, TRUE);
			}
			break;

		case KC_3:
			if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_3");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_3");
			}
			else
			{
				CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_1, TRUE);
			}
			break;

		case KC_4:
			if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_4");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_4");
			}
			else
			{
				CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_1, TRUE);
			}
			break;
		case KC_5:
			if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_5");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_5");
			}
			else
			{
				CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_1, TRUE);
			}
			break;
		case KC_6:
			if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_6");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_6");
			}
			else
			{
				CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_1, TRUE);
			}
			break;
		case KC_PGUP:
			//SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_PageUp();
			break;
		case KC_PGDOWN:
			{
				//SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_PageDown();

				// ������Ծ
				CAI_Base *pMySelfAI;
				pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

				//��ѯ�����Ҵ��ڰ�̯״̬��������
				if(FALSE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
				{
					((CAI_MySelf*)pMySelfAI)->PushCommand_Jump();
				}
			}
			break;
		case KC_TAB:
			{
				if( s_pInputSystem->IsKeyDown( KC_LMENU ) ) 
					break;
				std::vector< STRING > vParam;
				vParam.push_back( CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->szSceneMap );
				vParam.push_back( "2" );
				vParam.push_back( "tab" );
				CEventSystem::GetMe()->PushEvent(GE_TOGLE_SCENEMAP, vParam );
				CEventSystem::GetMe()->PushEvent(GE_TOGLE_CLOSELARGEMAP);
				CGameProcedure::m_bNeedFreshMinimap = true;
				CGameProcedure::m_bWaitNeedFreshMinimap = true;

			}
			break;
		case KC_ESCAPE:
			s_pUISystem->OnPushEcsape();

			//ȡ���������ָ��
			CGameInterface::GetMe()->Skill_CancelAction();
			break;

		//case KC_UP:
		//	if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
		//	{
		//		s_pGfxSystem->Camera_SetZoom( true, 2 );
		//	}
		//	else
		//	{
		//		if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
		//		{
		//			SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowUp(0);
		//		}
		//	}
		//	break;
		//case KC_DOWN:
		//	if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
		//	{
		//		s_pGfxSystem->Camera_SetZoom( FALSE, 2 );
		//	}
		//	else
		//	{
		//		if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
		//		{
		//			SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowDown(0);
		//		}
		//	}
		//	break;
		case KC_RETURN:
			if( !CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
			{
				CEventSystem::GetMe()->PushEvent( GE_SEND_MAIL );
			}
			break;
		case KC_DELETE:
			if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_delete");
			}
			break;
		case KC_COMMA:
			// ������GM�������
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowUp(1);
				}
			}
			break;
		case KC_PERIOD:
			// ������GM�������
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowDown(1);
				}
			}
			break;
		case KC_ADD:
			break;
		case KC_SUBTRACT:
			break;
		case KC_SYSRQ:
			{
				// ����
				// ע�⣺�����Ϣѭ���ﴦ�������֮֡����¼��������õ͵Ļ����ϻ��������⡣
				// Ҫ��windows����Ϣ������CInputSystem::MessageProc�Ｐʱ������Ӧ�Ľ���������
			}
			break;
		default:
			break;
		}
	}
	else if(event.nEvent == EVENT_KEY_UP)
	{
		switch(event.kIndex)
		{
			// ����������ݼ�
		case KC_F1:
		case KC_F2:
		case KC_F3:
		case KC_F4:
		case KC_F5:
		case KC_F6:
		case KC_F7:
		case KC_F8:
		case KC_F9:
		case KC_F10:	// ��ݼ���F1-F6��1-6�����֣�����+6������ Nick 2008.1.12
			CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_F1 + 6, FALSE);
			break;
			// ���ܿ�ݼ� Nick 2008.1.12

		case KC_F11:
			{
				// ����������ң����������ˣ���ͷ������
				m_bShowOtherObjBoard= !m_bShowOtherObjBoard;
			}
			break;
		case KC_F12:
			{
				// ����������ң����������ˣ���ģ�ͺ�ͷ������
				m_bShowOtherObj = !m_bShowOtherObj;
			}
			break;

		case KC_1:
		case KC_2:
		case KC_3:
		case KC_4:
		case KC_5:
		case KC_6:
			break;
		default:
			break;
		}
	}
}

// ���������ϼ�����
VOID CGamePro_Main::Handle_Combination_Key(InputEvent& event)
{
	BOOL isChatActive = FALSE;
	if( CGameProcedure::s_pUISystem->HasInputFocus() )
	{
		if( !CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox") )
		{
			return;
		}
		else
		{
			isChatActive = TRUE;
		}

		if( event.nEvent == EVENT_KEY_DOWN )
		{
			switch( event.kIndex )
			{
			case	KC_UP:		//����״̬�£�Ctrl+��
				if ( s_pInputSystem->IsCtrlDown() )
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Ctrl_ArrowUp();			
				break;

			case KC_DOWN:		//����״̬�£�Ctrl+��
				if( s_pInputSystem->IsCtrlDown() )
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Ctrl_ArrowDown();
				break;

			case KC_V:
				if (s_pInputSystem->IsCtrlDown())
				{
					CEventSystem::GetMe()->PushEvent( GE_ACCELERATE_KEYSEND, "ctrl_v" );
				}
				break;

			case KC_C:
				if (s_pInputSystem->IsCtrlDown())
				{
					//SCRIPT_SANDBOX::Talk::s_Talk.ChangeClipboardTxt();
				}
			}
		}
	}

	if(event.nEvent == EVENT_KEY_DOWN)
	{
		switch(event.kIndex)
		{
		case KC_LCONTROL:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "ctrl", "left", 1 ); // 1��ʾ����
			break;

		case KC_RCONTROL:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "ctrl", "right", 1 ); // 1��ʾ����
			break;

		case KC_LSHIFT:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift", "left", 1 ); // 1��ʾ����
			break;

		case KC_RSHIFT:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift", "right", 1 ); //  1��ʾ����
			break;
		case KC_A:
			//--- for debug
			//Ctrl+Alt+A �����������
			if(s_pInputSystem->IsCtrlDown() && s_pInputSystem->IsAltDown())
			{
				CObjectManager::GetMe()->Debug_LockNearestTarget();
			}
			//--- for debug
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_a");
			}
			break;
		case KC_B:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_b");
			}
			break;
		case KC_C:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_c");
			}
			break;
		case KC_D:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_d");
			}
			break;
		case KC_E:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_e");
			}
			break;
		case KC_F:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_f");
			}
			break;
		case KC_G:	
			{
				if(CGameProcedure::s_pInputSystem->IsKeyDown(KC_LMENU))
				{
					CGameProcedure::s_pGfxSystem->Debug_ToggleShowDebugPanel();
					event.bProcessed = true;
				}
			}
			break;
		case KC_H:
			{
				if(s_pInputSystem->IsAltDown())
				{
					CHelperSystem* pHelpSystem = (CHelperSystem*)CGameProcedure::s_pHelperSystem;
					if(pHelpSystem)
					{
						pHelpSystem->GotoAnchorHelp(STRING(""));
					}
				}
			}
			break;
		case KC_I:
			if(CGameProcedure::s_pInputSystem->IsKeyDown(KC_LMENU))
			{
				gbTestMinimap = !gbTestMinimap;
			}
			break;
		case KC_J:
			{
				if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
				{
					// ������Ծ
					CAI_Base *pMySelfAI;
					pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

					((CAI_MySelf*)pMySelfAI)->PushCommand_Jump();
				}
			}
			break;
		case KC_K:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_k");
			}
			break;
		case KC_L:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_l");
			}
			break;
		case KC_M:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_m");
			}
			break;
			// ���Լ�(ALT+N)
		case KC_N:
			{
				if(CGameProcedure::s_pInputSystem->IsKeyDown(KC_LMENU))
				{
					event.bProcessed = true;
				}
				if(s_pInputSystem->IsAltDown() && 
					CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain/*&& g_bDebugMode*/)
				{
					s_pGfxSystem->Scene_ShowGridSwitch(CScene::SIZE_OF_ZONE);
				}
			}
			break;
		case KC_O:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_o");
			}
			break;
		case KC_P:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_p");
			}
			break;
		case KC_Q:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_q");
			}
			break;
		case KC_R:
			if(CGameProcedure::s_pInputSystem->IsKeyDown(KC_LMENU))
			{
				CGameProcedure::s_pGfxSystem->Debug_ToggleShowBoundingBox();
			}
			break;
		case KC_S:
			if(s_pInputSystem->IsAltDown())
			{
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					//SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Alt_S();
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Alt_S();
				}
				else
				{
					CEventSystem::GetMe()->PushEvent( GE_SEND_MAIL );
				}
			}

			break;
		case KC_T:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_t");
			}
			break;
		case KC_U:
			break;
		case KC_V:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_v");
			}

			break;
		case KC_W:
			break;
		case KC_X:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_x");
			}
			break;
		case KC_Y:
			break;
		case KC_Z:
			if (s_pInputSystem->IsShiftDown() && s_pInputSystem->IsCtrlDown())
			{
				//Toggle UI Show
				s_pUISystem->UIRenderToggleShow();
			}
			else if(s_pInputSystem->IsAltDown() && !s_pInputSystem->IsCtrlDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_z");
			}
			
			break;

		case KC_1:
			if(s_pInputSystem->IsAltDown())
			{
				//temp ���Գƺ�ϵͳ
				CEventSystem::GetMe()->PushEvent( GE_SHOW_TITLE );	// GE_UPDATE_TITLE


				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_1");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_1");
			}
			break;

		case KC_2:
			if(s_pInputSystem->IsAltDown())
			{
				//temp
				CDataPool::GetMe()->SetMouseCmd();


				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_2");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_2");
			}
			break;

		case KC_3:
			if(s_pInputSystem->IsAltDown())
			{
				//temp
				CDataPool::GetMe()->CancelMouseCmd();


				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_3");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_3");
			}
			break;

		case KC_4:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_4");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_4");
			}
			break;
		case KC_5:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_5");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_5");
			}
			break;
		case KC_6:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_6");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_6");
			}
			break;
		case KC_7:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_7");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_7");
			}
			break;
		case KC_8:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_8");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_8");
			}
			break;
		case KC_9:
			if(s_pInputSystem->IsAltDown())
			{
				//temp �Ʋ�������ȫʱ�޽���
				CEventSystem::GetMe()->PushEvent( GE_RICH_SAFE_TIME );


				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_9");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_9");
			}
			break;
		case KC_0:
			if(s_pInputSystem->IsAltDown())
			{
				//temp �Ʋ���������

				// �ж��Ƿ����ù���
				if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup( 1 ) )
				{
					// ���ù��ˣ� ֱ����ʾ���ô���
					CEventSystem::GetMe()->PushEvent( GE_RICH_SETUP );
				}


				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_0");
			}
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_0");
			}
			break;

		case KC_UP:
			if(s_pInputSystem->IsAltDown())
			{
				s_pGfxSystem->Camera_SetZoom( true, 2 );
			}
			break;
		case KC_DOWN:
			if(s_pInputSystem->IsAltDown())
			{
				s_pGfxSystem->Camera_SetZoom( FALSE, 2 );
			}
			break;

		case KC_DELETE:
			if(s_pInputSystem->IsAltDown())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_delete");
			}
			break;
		case KC_MINUS:
			if(s_pInputSystem->IsKeyDown(KC_LMENU) || s_pInputSystem->IsKeyDown(KC_RMENU))
			{
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Alt_FaceMotion();
				}
			}
			break;
		case KC_COMMA:
			// ������GM�������
			if(s_pInputSystem->IsCtrlDown() && s_pInputSystem->IsAltDown())
			{
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowUp(1);
				}
			}
			break;
		case KC_PERIOD:
			// ������GM�������
			if(s_pInputSystem->IsCtrlDown() && s_pInputSystem->IsAltDown())
			{
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowDown(1);
				}
			}
			break;
		case KC_ADD:
			break;
		case KC_SUBTRACT:
			break;
		case KC_SYSRQ:
			{
				// ����
				// ע�⣺�����Ϣѭ���ﴦ�������֮֡����¼��������õ͵Ļ����ϻ��������⡣
				// Ҫ��windows����Ϣ������CInputSystem::MessageProc�Ｐʱ������Ӧ�Ľ���������
			}
			break;
		default:
			break;
		}
	}
	else if(event.nEvent == EVENT_KEY_UP)
	{
		switch(event.kIndex)
		{
		case KC_LCONTROL:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "ctrl", "left", 0 ); // 0��ʾ����
			break;

		case KC_RCONTROL:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "ctrl", "right", 0 ); // 0��ʾ����
			break;

		case KC_LSHIFT:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift", "left", 0 ); //  0��ʾ����
			break;

		case KC_RSHIFT:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift", "right", 0 ); //  0��ʾ����
			break;

			// ����������ݼ�
		case KC_F1:
		case KC_F2:
		case KC_F3:
		case KC_F4:
		case KC_F5:
		case KC_F6:
		case KC_F7:
		case KC_F8:
		case KC_F9:
		case KC_F10:
			break;
		case KC_1:
		case KC_2:
		case KC_3:
		case KC_4:
		case KC_5:
		case KC_6:
			if( !s_pInputSystem->IsKeyDown(KC_LSHIFT) || !s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				if( isChatActive == FALSE )
				{
					CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_1, FALSE);
				}
			}
			break;
		default:
			break;
		}
	}
}


//�����������
VOID CGamePro_Main::ProcessInput_Mouse(VOID)
{
	static const INT	DRAG_MOVE_TIME_INTERVAL	= 500;

	//-----------------------------------------------------------
	//���״̬
	const UINT dwMouseFlag = s_pInputSystem->MouseGetFlag();
	const UINT dwMouseFlagOld = s_pInputSystem->MouseGetFlagOld();
	POINT pt = s_pInputSystem->MouseGetPos();
	POINT ptOld = s_pInputSystem->MouseGetPosOld();

	RECT rectClient;
	::GetClientRect(g_hMainWnd, &rectClient);
	//-----------------------------------------------------------
	//��������ڲ����������겻�ٿͻ���,����
	if(!CGameProcedure::IsWindowActive() || !(((CInputSystem*)s_pInputSystem)->MouseIsInClient()))
	{
		return;
	}

	//-----------------------------------------------------------
	//ȡ�������Ϣ
	INT nMousePosXDiff	= pt.x - ptOld.x;
	INT nMousePosYDiff	= pt.y - ptOld.y;
	BOOL bLBtnDown		= ( ( dwMouseFlag & MOUSE_LBDOWN) != FALSE );
	BOOL bRBtnDown		= ( ( dwMouseFlag & MOUSE_RBDOWN) != FALSE );
	BOOL bLBtnClick		= s_pInputSystem->IsEventExist(EVENT_MOUSE_LBCLICK);
	BOOL bRBtnClick		= s_pInputSystem->IsEventExist(EVENT_MOUSE_RBCLICK);

	//-----------------------------------------------------------
	//�����ײ�ϵ�����
	fVector3 fvMouseHitPlan;
	CObject* pSelectObj = ( CObject* )CObjectManager::GetMe()->GetMouseOverObject( pt.x, pt.y, fvMouseHitPlan );
	//����Ƿ���ui�Ͽ�
	BOOL bInUIHover = s_pUISystem && s_pUISystem->IsMouseHover() && CInputSystem::GetMe()->GetCapture()!=ICS_GAME;

	//-----------------------------------------------------------
	// ������Ҽ���û�а���,�����������
	HCURSOR LastCursor = NULL;

	if(!bLBtnClick && !bRBtnClick)
	{
		CCursorMng::GetMe()->MouseCommand_Set(bInUIHover, pSelectObj, fvMouseHitPlan, CActionSystem::GetMe()->GetDefaultAction());
		// ���ù������
		const SCommand_Mouse& cmd = CCursorMng::GetMe()->MouseCommand_GetLeft();

		// ��UI����
		if( bInUIHover )
		{
			switch(cmd.m_typeMouse)
			{
			// ����
			case SCommand_Mouse::MCT_REPAIR:			
				CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);
				break;

			// ����
			case SCommand_Mouse::MCT_UI_USE_IDENTIFY:	
				CCursorMng::GetMe()->SetCursor( CURSOR_IDENTIFY);
				break;

			default:
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;
			}
		}
		// �������
		else
		{
			LastCursor = CCursorMng::GetMe()->GetCursor();
			switch(cmd.m_typeMouse)
			{
			// �ƶ�
			case SCommand_Mouse::MCT_PLAYER_MOVETO:		
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;

			// ѡ��Ŀ��
			case SCommand_Mouse::MCT_PLAYER_SELECT:		
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;

			// Ŀ�깥��
			case SCommand_Mouse::MCT_SKILL_OBJ:			
				CCursorMng::GetMe()->SetCursor( CURSOR_ATTACK );
				break;

			// ���򹥻�
			case SCommand_Mouse::MCT_SKILL_AREA:		
				CCursorMng::GetMe()->SetCursor( CURSOR_RING );		// CURSOR_ATTACK
				break;

			// �����ͼ���
			case SCommand_Mouse::MCT_SKILL_DIR:			
				CCursorMng::GetMe()->SetCursor( CURSOR_DIRECTION );	// CURSOR_ATTACK
				break;

			// �����ܹ�������TripperObj��(�������...)
			case SCommand_Mouse::MCT_HIT_TRIPPEROBJ:	// TripperObj
				{
					CTripperObject* pTripperObj = (CTripperObject*)CObjectManager::GetMe()->FindServerObject(cmd.m_adwParam[0]);
					if(pTripperObj)
						CCursorMng::GetMe()->SetCursor( pTripperObj->Tripper_GetCursor() );
					else
						CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				}
				break;

			// ̸��
			case SCommand_Mouse::MCT_SPEAK:				
				CCursorMng::GetMe()->SetCursor( CURSOR_SPEAK);
				break;

			// ��Ʒ
			case SCommand_Mouse::MCT_USE_ITEM:			
				CCursorMng::GetMe()->SetCursor(CURSOR_HOVER);
				break;

			// ����
			case SCommand_Mouse::MCT_REPAIR:			
				CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);
				break;

			// ����
			case SCommand_Mouse::MCT_UI_USE_IDENTIFY:	
				CCursorMng::GetMe()->SetCursor( CURSOR_IDENTIFY);
				break;

			// ��Ӻ���
			case SCommand_Mouse::MCT_ADD_FRIEND:
				CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);	//temp
				break;

			// ����
			case SCommand_Mouse::MCT_EXCHANGE:
				break;
			
			default:
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;
			}


			//====== 
			// ��ʾ �� ���� ���ܷ�Χָʾ��

			if( (SCommand_Mouse::MCT_SKILL_AREA == cmd.m_typeMouse) || ((SCommand_Mouse::MCT_USE_ITEM == cmd.m_typeMouse) && (cmd.m_adwParam[4] == TRUE)) )
			{
				// ������ָʾ��ʵ���ʹ���
				if(!m_pSkillArea)
				{
					m_pSkillArea = (CObject_ProjTex_AuraDure*)CObjectManager::GetMe()->NewObject("CObject_ProjTex_AuraDure");
					m_pSkillArea->Initial(NULL);
				}

				// ��ȡ���еĲ����� Ϊ�˻�÷�Χ�ߴ�
				tActionItem* pActiveSkill = CActionSystem::GetMe()->GetDefaultAction();
				const SCLIENT_SKILL* pSkillImpl = (const SCLIENT_SKILL*)pActiveSkill->GetImpl();

				//�����ָ����Ч,��ôʹ��֮
				if( pSkillImpl->m_pDefine&& strlen( pSkillImpl->m_pDefine->m_lpszAttachProjectTextureEffect) > 0 )
				{
					// ��ʾ
					m_pSkillArea->SetShowEnable(TRUE, pSkillImpl->m_pDefine->m_lpszAttachProjectTextureEffect);
				}
				else
				{
					// ��ʾ
					m_pSkillArea->SetShowEnable(TRUE);
				}
				m_pSkillArea->SetPosition(fvMouseHitPlan);

				if(pSkillImpl)
				{
					// ���óߴ�
					m_pSkillArea->SetRingRange( pSkillImpl->GetDemageRange() );
				}
			}
			else if(m_pSkillArea && m_pSkillArea->GetShowEnable())
			{
				// ����
				m_pSkillArea->SetShowEnable(FALSE);
			}

		}

		//if(SCommand_Mouse::MCT_REPAIR == cmd.m_typeMouse)	//"����"��������
		//	CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);

	}


	// �����¼�����
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;

		switch(event.nEvent)
		{
		// �Ҽ����
		case EVENT_MOUSE_RBDOWN:
			{
				m_bCameraDraging = TRUE;
				m_bIsLastCameraDraging = FALSE;
				event.bProcessed = true;
				// �������
				s_pInputSystem->SetCapture(ICS_GAME);
				CCursorMng::GetMe()->ShowCursor(FALSE);
				::SetCapture(g_hMainWnd);

				// ��Ծ
//				CObjectManager::GetMe()->GetMySelf()->Jump();

		}
			break;

		// �Ҽ��϶�
		case EVENT_MOUSE_RDRAG_MOVE:
			{
				// �������������
				if(m_bCameraDraging)
				{
					FLOAT fSub = max( FLOAT( nMousePosXDiff /10.0f ), 1.0f) / 100;

					BOOL iCamera_Rotation = (BOOL)s_pVariableSystem->GetAs_Int("Camera_Rotation");
					if(iCamera_Rotation)
					{
						s_pGfxSystem->Camera_AddDirection( FLOAT( -nMousePosXDiff /300.0f ));	//616======	������ת
					}
					
					event.bProcessed = true;

					// ���鵽ԭ��λ��
					s_pInputSystem->MouseSetPos(ptOld.x, ptOld.y);
					CCursorMng::GetMe()->ShowCursor(FALSE);

					m_bIsLastCameraDraging = TRUE;
				}
				////���ʱ��Ӧ������Ҽ��ſ���,��Ӧ����ִ���κ�ָ����,��Ϊ��β�������Ϊ�Ҽ��϶������
				//CCursorMng::GetMe()->MouseCommand_GetRight().Cleanup();
			}
			break;

		// �����ʱ�䰴��
		case EVENT_MOUSE_LBDOWNREPEAT:
			{
				// ��ʼ�Զ�����
				m_bAutoRun = TRUE;
				event.bProcessed = true;
			}
			break; 

		// �������
		case EVENT_MOUSE_LBDOWN:
			{
				// ȡ���Զ�����
				m_bAutoRun = FALSE; 
				event.bProcessed = true;

				// ���ð��µ�ͼ��
//				CCursorMng::GetMe()->SetCursor( CURSOR_MINE );
			}
			break;

		// ������
		case EVENT_MOUSE_LBUP:
			{
				// �����������
				if(s_pUISystem && s_pUISystem->IsDragFlag())
				{
					s_pUISystem->ChangeDragFlag(false);
				} 
				else
				{

					//616======*
					if(CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1)
					{
						CGameProcedure::s_pGameInterface->Player_MoveTo( fVector2(fvMouseHitPlan.x, fvMouseHitPlan.z) );
					}


					CCursorMng::GetMe()->MouseCommand_Active( CCursorMng::GetMe()->MouseCommand_GetLeft() );
				}
				//CGameProcedure::s_pGfxSystem->PushDebugString("Active Left");

				event.bProcessed = true;

				// ����̧���ͼ��
//				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
			}
			break;

		// �Ҽ�̧��
		case EVENT_MOUSE_RBUP:
			{
				// ��ʾ���
				CCursorMng::GetMe()->ShowCursor(true);
				//event.bProcessed = true;
				s_pInputSystem->SetCapture(ICS_NONE);
				::ReleaseCapture();
			}
			break;

		// �Ҽ����
		case EVENT_MOUSE_RBCLICK:
			{
				// �����������
				if( pSelectObj )
				{
					m_bAutoRun = FALSE; 
				}
				if( !m_bIsLastCameraDraging )
				{
					CCursorMng::GetMe()->MouseCommand_Active( CCursorMng::GetMe()->MouseCommand_GetRight() );
				}
				event.bProcessed = true;
			}
			break;
		// ��������
		case EVENT_MOUSE_WHEEL_UP:
			{
				s_pGfxSystem->Camera_SetZoom( TRUE, 2 );
				event.bProcessed = true;
			}
			break;

		// ��������
		case EVENT_MOUSE_WHEEL_DOWN:
			{
				s_pGfxSystem->Camera_SetZoom( FALSE, 2 );
				event.bProcessed = true;
			}
			break;
		}
	}

	//-----------------------------------------------------------
	// ����Ҽ��Ѿ��ɿ�
	if(!bRBtnDown)
	{
		m_bCameraDraging = FALSE;
		if(m_bAutoRun) // �Զ�����... 
		{
			CCursorMng::GetMe()->SetCursor( CURSOR_AUTORUN );
			CGameProcedure::s_pGameInterface->Player_MoveTo( fVector2(fvMouseHitPlan.x,  fvMouseHitPlan.z) );
//			CCursorMng::GetMe()->SetCursor( CURSOR_AUTORUN );
		}
		else if( LastCursor != NULL && CCursorMng::GetMe()->GetCursor() != LastCursor )
		{
			CCursorMng::GetMe()->OnSetCursor();
		}
	}
}

VOID CGamePro_Main::SetMouseTargetProjTex(const fVector2& fvPos)
{

	if( !m_pMouseTarget )
	{
		// ����
		m_pMouseTarget = (CObject_ProjTex_MouseTarget*)CObjectManager::GetMe()->NewObject("CObject_ProjTex_MouseTarget");
		m_pMouseTarget->Initial(NULL);
	}

	// �ж�Ŀ���Ƿ񲻿���
	m_pMouseTarget->SetReachAble( !CPath::IsPointInUnreachRegion( fvPos ) );
	// ����λ��
	m_pMouseTarget->SetMapPosition(fvPos);

}


VOID CGamePro_Main::CloseRequest(VOID)
{
	CActionSystem::GetMe()->SaveAction();
}

VOID CGamePro_Main::Release(VOID)
{
	m_bCameraDraging = FALSE;
	m_bAutoRun = FALSE;

	// �ͷ�MouseTarget
	if(m_pMouseTarget)
	{
		m_pMouseTarget->Destroy();
		m_pMouseTarget = NULL;
	}

	// �ͷ�SkillArea
	if(m_pSkillArea)
	{
		m_pSkillArea->Destroy();
		m_pSkillArea = NULL;
	}


	//test ���ܱ���
//	AxProfile::AxProfile_Report();


	// �������������
	fTransDistance	=	s_pGfxSystem->Camera_GetZoom();

	m_bInit = FALSE;
}

LRESULT CGamePro_Main::MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CGameProcedure::MainWndProc(hWnd, message, wParam, lParam);
}

VOID CGamePro_Main::OnSetCursor(VOID)
{
}

