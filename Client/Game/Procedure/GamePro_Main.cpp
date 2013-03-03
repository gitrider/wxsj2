
/**	GamePro_Main.cpp
 *
 *	功能：	游戏主流程
 *
 *	修改记录：
 *			080324	修改刷新小地图
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
#include "CGAskDetailMountList.h"  //  [7/8/2010 陈军龙]
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
	// 已经初始化了
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
	// 重置LogicCount
//	m_pMySelf->ResetLogicCount();
	m_pMySelf->ResetBaseLogicCount();
	m_pMySelf->ResetActionLogicCount();

	//------------------------------------------------------------------
	// 清空加载队列
	CObjectManager::GetMe()->GetLoadQueue()->ClearAllTask();

	//------------------------------------------------------------------
	// 发送环境请求
	CGEnvRequest msgEnvRequest;
	CNetManager::GetMe()->SendPacket(&msgEnvRequest);

	//------------------------------------------------------------------
	// 发送自身数据请求

	// 自身基本信息
	CGCharAskBaseAttrib msgMyBaseAttrib;
	msgMyBaseAttrib.setTargetID(m_nID);
	CNetManager::GetMe()->SendPacket(&msgMyBaseAttrib);

	// 基本属性
	CGAskDetailAttrib	msgMyAttrib;
	msgMyAttrib.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMyAttrib);

	////自身装备(用于创建渲染层外形)
	//CGCharAskEquipment msgAskMyEquip;
	//msgAskMyEquip.setObjID(m_nID);
	//CNetManager::GetMe()->SendPacket(&msgAskMyEquip);

	// 生活技能
	CGAskDetailAbilityInfo msgAbility;
	msgAbility.setTargetID(m_nID );
	CNetManager::GetMe()->SendPacket(&msgAbility);

	// 心法
	CGAskDetailXinFaList	msgMyXinFa;
	msgMyXinFa.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMyXinFa);

	// 技能
	CGAskDetailSkillList	msgMySkill;
	msgMySkill.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMySkill);

	// 组队数据
	CUIDataPool::GetMe()->ClearTeamInfo();
	CGAskTeamInfo msgTeamInfo;
	msgTeamInfo.SetObjID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgTeamInfo);

	//如果是城市向服务器请求城市数据
	CGCityAskAttr	MsgCityBuilding;
	CNetManager::GetMe()->SendPacket( &MsgCityBuilding );

	// 所有称号
	CGCharAllTitles	msgAllTitles;
	msgAllTitles.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgAllTitles);

	// 是第一次登陆
	if( CGameProcedure::s_pProcEnter->GetEnterType() == ENTER_TYPE_FIRST )
	{
		// 二级密码是否已经设置
		CGMinorPasswd MsgMinorPasswd;
		MsgMinorPasswd.GetMinorPassword()->CleanUp();
		MsgMinorPasswd.GetMinorPassword()->m_Type = MREQT_PASSWDSETUP;
		CNetManager::GetMe()->SendPacket(&MsgMinorPasswd);

		// 二级密码是否处在强制解除阶段
		CGMinorPasswd MsgDelMinorPasswd;
		MsgDelMinorPasswd.GetMinorPassword()->CleanUp();
		MsgDelMinorPasswd.GetMinorPassword()->m_Type = MREQT_DELETEPASSWDTIME;
		CNetManager::GetMe()->SendPacket(&MsgDelMinorPasswd);

		// 详细装备(用于在数据池中存储物品详细属性)
		CGAskDetailEquipList msgAskMyDetailEquip;
		msgAskMyDetailEquip.setObjID( m_nID );
		msgAskMyDetailEquip.SetAskMode(ASK_EQUIP_ALL);
		CNetManager::GetMe()->SendPacket(&msgAskMyDetailEquip);

		// 背包
		CGAskMyBagList	msgMyBag;
		msgMyBag.SetAskMode(ASK_ALL);
		CNetManager::GetMe()->SendPacket(&msgMyBag);

		// 坐骑  [7/8/2010 陈军龙]
		CGAskDetailMountList msgAskMyDetailMount;
		msgAskMyDetailMount.setObjID( m_nID );
		CNetManager::GetMe()->SendPacket(&msgAskMyDetailMount);

		// MissionList
		CGAskMissionList	msgMissionList;
		msgMissionList.setTargetID( m_nID );
		CNetManager::GetMe()->SendPacket(&msgMissionList);

		// 向服务器请求快捷栏数据
		CGAskSetting msgSetting;
		CNetManager::GetMe()->SendPacket( &msgSetting );

		// 向服务器请求关系人列表
		CGRelation MsgRelation;
		MsgRelation.GetRelation()->m_Type = REQ_RELATIONLIST;
		CNetManager::GetMe()->SendPacket( &MsgRelation );

		// 向服务器请求邮件
		CGAskMail MsgMail;
		MsgMail.SetAskType( ASK_TYPE_LOGIN );
		CNetManager::GetMe()->SendPacket( &MsgMail );

		// 向服务器请求银行中的金钱数量
		CGBankMoney	MsgBankMoney;
		MsgBankMoney.SetIsSave(CGBankMoney::UPDATE_MONEY);
		CNetManager::GetMe()->SendPacket( &MsgBankMoney );
	}

	s_pGfxSystem->Camera_SetZoom(fTransDistance);

	// Talk
	SCRIPT_SANDBOX::Talk::s_Talk.Initial();
	// PetInviteFriend
	SCRIPT_SANDBOX::PetInviteFriend::s_PetInviteFriend.InitPetList();


	// 重新设置显示状态
	if(CGameProcedure::s_pProcEnter->GetEnterType() == ENTER_TYPE_FIRST)
	{
		static BOOL bFirst = TRUE;
		if(bFirst)
		{
			const char* szNeedReset[] = 
			{
				"View_FullScreen",		//全屏模式
				"View_Resoution",		//分辨率
//				"View_FullScreenLight",	//全屏泛光效果
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

	// 允许变窗口大小。
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

		//行走速度
		_snprintf(szTemp, MAX_PATH, "%.2f", m_pMySelf->GetCharacterData()->Get_MoveSpeed());
		CGameProcedure::s_pGfxSystem->SetDebugString("Speed", szTemp);

		//显示位置
		fVector3 fvPosGfx;
		CGameProcedure::s_pGfxSystem->Axis_Trans(tGfxSystem::AX_GAME, m_pMySelf->GetPosition(), tGfxSystem::AX_GFX, fvPosGfx);

		_snprintf(szTemp, MAX_PATH, "%.1f, %.1f", m_pMySelf->GetPosition().x, m_pMySelf->GetPosition().z);
		CGameProcedure::s_pGfxSystem->SetDebugString("Position", szTemp);

		static UINT nTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
		static UINT	nTalkTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();


		if( gbTestMinimap )
		{
			// 如果立马需要刷新小地图
			if( CGameProcedure::m_bNeedFreshMinimap ) 
			{
				if( CGameProcedure::s_pTimeSystem->GetTimeNow() - nTimer > 100 ) // 每秒刷10次
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
			// 除了主角以外的别的东西变了
			else if( CGameProcedure::m_bWaitNeedFreshMinimap ) 
			{
				if( CGameProcedure::s_pTimeSystem->GetTimeNow() - nTimer > 300 ) // 每秒刷3次
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

		// 摄影机复位过程中
		if( m_nResetCamera != 0 ) 
		{
			s_pGfxSystem->Camera_AddDirection( CGameProcedure::s_pTimeSystem->GetDeltaTime() / 1000 * m_nResetCamera ); 
			if( m_nResetCamera < 0 )
			{
				if( s_pGfxSystem->Camera_GetDirection() < __PI ) // 如果已经抄过界限了
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



//处理键盘
VOID CGamePro_Main::ProcessInput_Keybord(VOID)
{
	static INT CanDieUseKey[] ={KC_ESCAPE, KC_SYSRQ};

	CObject_PlayerMySelf *pMySelf	= CObjectManager::GetMe()->GetMySelf();

	//-----------------------------------------------------------
	// 如果主角渲染层对象尚未建立，暂不接受任何键盘消息
	if(pMySelf == NULL || pMySelf->GetRenderInterface() == NULL) 
		return;

	// 分析事件队列
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) 
			continue;

		// 人物死亡的时候能处理的快捷键才放行
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

			// 不可使用的键
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

// 处理键盘单键输入
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
			// 主工具条快捷键
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

				// 处理跳跃
				CAI_Base *pMySelfAI;
				pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

				//查询如果玩家处于摆摊状态，不能跳
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

			//取消特殊鼠标指令
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
			// 聊天栏GM命令滚动
			if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
			{
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowUp(1);
				}
			}
			break;
		case KC_PERIOD:
			// 聊天栏GM命令滚动
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
				// 截屏
				// 注意：这个消息循环里处理的是两帧之间的事件，在配置低的机器上会引起问题。
				// 要在windows的消息处理函数CInputSystem::MessageProc里及时的做相应的截屏操作。
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
			// 主工具条快捷键
		case KC_F1:
		case KC_F2:
		case KC_F3:
		case KC_F4:
		case KC_F5:
		case KC_F6:
		case KC_F7:
		case KC_F8:
		case KC_F9:
		case KC_F10:	// 快捷键分F1-F6和1-6这两种，所以+6以区分 Nick 2008.1.12
			CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_F1 + 6, FALSE);
			break;
			// 技能快捷键 Nick 2008.1.12

		case KC_F11:
			{
				// 隐藏所有玩家（不包括本人）的头顶文字
				m_bShowOtherObjBoard= !m_bShowOtherObjBoard;
			}
			break;
		case KC_F12:
			{
				// 隐藏所有玩家（不包括本人）的模型和头顶文字
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

// 处理键盘组合键输入
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
			case	KC_UP:		//输入状态下，Ctrl+↑
				if ( s_pInputSystem->IsCtrlDown() )
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Ctrl_ArrowUp();			
				break;

			case KC_DOWN:		//输入状态下，Ctrl+↓
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
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "ctrl", "left", 1 ); // 1表示按下
			break;

		case KC_RCONTROL:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "ctrl", "right", 1 ); // 1表示按下
			break;

		case KC_LSHIFT:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift", "left", 1 ); // 1表示按下
			break;

		case KC_RSHIFT:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift", "right", 1 ); //  1表示按下
			break;
		case KC_A:
			//--- for debug
			//Ctrl+Alt+A 锁定最近怪物
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
					// 处理跳跃
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
			// 调试键(ALT+N)
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
				//temp 测试称号系统
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
				//temp 财产保护安全时限界面
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
				//temp 财产保护界面

				// 判断是否设置过了
				if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup( 1 ) )
				{
					// 设置过了， 直接显示设置窗口
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
			// 聊天栏GM命令滚动
			if(s_pInputSystem->IsCtrlDown() && s_pInputSystem->IsAltDown())
			{
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowUp(1);
				}
			}
			break;
		case KC_PERIOD:
			// 聊天栏GM命令滚动
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
				// 截屏
				// 注意：这个消息循环里处理的是两帧之间的事件，在配置低的机器上会引起问题。
				// 要在windows的消息处理函数CInputSystem::MessageProc里及时的做相应的截屏操作。
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
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "ctrl", "left", 0 ); // 0表示弹起
			break;

		case KC_RCONTROL:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "ctrl", "right", 0 ); // 0表示弹起
			break;

		case KC_LSHIFT:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift", "left", 0 ); //  0表示弹起
			break;

		case KC_RSHIFT:
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift", "right", 0 ); //  0表示弹起
			break;

			// 主工具条快捷键
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


//处理鼠标输入
VOID CGamePro_Main::ProcessInput_Mouse(VOID)
{
	static const INT	DRAG_MOVE_TIME_INTERVAL	= 500;

	//-----------------------------------------------------------
	//鼠标状态
	const UINT dwMouseFlag = s_pInputSystem->MouseGetFlag();
	const UINT dwMouseFlagOld = s_pInputSystem->MouseGetFlagOld();
	POINT pt = s_pInputSystem->MouseGetPos();
	POINT ptOld = s_pInputSystem->MouseGetPosOld();

	RECT rectClient;
	::GetClientRect(g_hMainWnd, &rectClient);
	//-----------------------------------------------------------
	//如果主窗口不激活或者鼠标不再客户区,返回
	if(!CGameProcedure::IsWindowActive() || !(((CInputSystem*)s_pInputSystem)->MouseIsInClient()))
	{
		return;
	}

	//-----------------------------------------------------------
	//取得鼠标信息
	INT nMousePosXDiff	= pt.x - ptOld.x;
	INT nMousePosYDiff	= pt.y - ptOld.y;
	BOOL bLBtnDown		= ( ( dwMouseFlag & MOUSE_LBDOWN) != FALSE );
	BOOL bRBtnDown		= ( ( dwMouseFlag & MOUSE_RBDOWN) != FALSE );
	BOOL bLBtnClick		= s_pInputSystem->IsEventExist(EVENT_MOUSE_LBCLICK);
	BOOL bRBtnClick		= s_pInputSystem->IsEventExist(EVENT_MOUSE_RBCLICK);

	//-----------------------------------------------------------
	//鼠标碰撞上的物体
	fVector3 fvMouseHitPlan;
	CObject* pSelectObj = ( CObject* )CObjectManager::GetMe()->GetMouseOverObject( pt.x, pt.y, fvMouseHitPlan );
	//鼠标是否在ui上空
	BOOL bInUIHover = s_pUISystem && s_pUISystem->IsMouseHover() && CInputSystem::GetMe()->GetCapture()!=ICS_GAME;

	//-----------------------------------------------------------
	// 如果左右键都没有按下,计算鼠标命令
	HCURSOR LastCursor = NULL;

	if(!bLBtnClick && !bRBtnClick)
	{
		CCursorMng::GetMe()->MouseCommand_Set(bInUIHover, pSelectObj, fvMouseHitPlan, CActionSystem::GetMe()->GetDefaultAction());
		// 设置光标类型
		const SCommand_Mouse& cmd = CCursorMng::GetMe()->MouseCommand_GetLeft();

		// 在UI上面
		if( bInUIHover )
		{
			switch(cmd.m_typeMouse)
			{
			// 修理
			case SCommand_Mouse::MCT_REPAIR:			
				CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);
				break;

			// 鉴定
			case SCommand_Mouse::MCT_UI_USE_IDENTIFY:	
				CCursorMng::GetMe()->SetCursor( CURSOR_IDENTIFY);
				break;

			default:
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;
			}
		}
		// 光标类型
		else
		{
			LastCursor = CCursorMng::GetMe()->GetCursor();
			switch(cmd.m_typeMouse)
			{
			// 移动
			case SCommand_Mouse::MCT_PLAYER_MOVETO:		
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;

			// 选择目标
			case SCommand_Mouse::MCT_PLAYER_SELECT:		
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;

			// 目标攻击
			case SCommand_Mouse::MCT_SKILL_OBJ:			
				CCursorMng::GetMe()->SetCursor( CURSOR_ATTACK );
				break;

			// 区域攻击
			case SCommand_Mouse::MCT_SKILL_AREA:		
				CCursorMng::GetMe()->SetCursor( CURSOR_RING );		// CURSOR_ATTACK
				break;

			// 方向型技能
			case SCommand_Mouse::MCT_SKILL_DIR:			
				CCursorMng::GetMe()->SetCursor( CURSOR_DIRECTION );	// CURSOR_ATTACK
				break;

			// 放在能够操作的TripperObj上(矿物，箱子...)
			case SCommand_Mouse::MCT_HIT_TRIPPEROBJ:	// TripperObj
				{
					CTripperObject* pTripperObj = (CTripperObject*)CObjectManager::GetMe()->FindServerObject(cmd.m_adwParam[0]);
					if(pTripperObj)
						CCursorMng::GetMe()->SetCursor( pTripperObj->Tripper_GetCursor() );
					else
						CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				}
				break;

			// 谈话
			case SCommand_Mouse::MCT_SPEAK:				
				CCursorMng::GetMe()->SetCursor( CURSOR_SPEAK);
				break;

			// 物品
			case SCommand_Mouse::MCT_USE_ITEM:			
				CCursorMng::GetMe()->SetCursor(CURSOR_HOVER);
				break;

			// 修理
			case SCommand_Mouse::MCT_REPAIR:			
				CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);
				break;

			// 鉴定
			case SCommand_Mouse::MCT_UI_USE_IDENTIFY:	
				CCursorMng::GetMe()->SetCursor( CURSOR_IDENTIFY);
				break;

			// 添加好友
			case SCommand_Mouse::MCT_ADD_FRIEND:
				CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);	//temp
				break;

			// 交易
			case SCommand_Mouse::MCT_EXCHANGE:
				break;
			
			default:
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;
			}


			//====== 
			// 显示 或 隐藏 技能范围指示环

			if( (SCommand_Mouse::MCT_SKILL_AREA == cmd.m_typeMouse) || ((SCommand_Mouse::MCT_USE_ITEM == cmd.m_typeMouse) && (cmd.m_adwParam[4] == TRUE)) )
			{
				// 不存在指示环实例就创建
				if(!m_pSkillArea)
				{
					m_pSkillArea = (CObject_ProjTex_AuraDure*)CObjectManager::GetMe()->NewObject("CObject_ProjTex_AuraDure");
					m_pSkillArea->Initial(NULL);
				}

				// 获取表中的参数， 为了获得范围尺寸
				tActionItem* pActiveSkill = CActionSystem::GetMe()->GetDefaultAction();
				const SCLIENT_SKILL* pSkillImpl = (const SCLIENT_SKILL*)pActiveSkill->GetImpl();

				//如果有指定特效,那么使用之
				if( pSkillImpl->m_pDefine&& strlen( pSkillImpl->m_pDefine->m_lpszAttachProjectTextureEffect) > 0 )
				{
					// 显示
					m_pSkillArea->SetShowEnable(TRUE, pSkillImpl->m_pDefine->m_lpszAttachProjectTextureEffect);
				}
				else
				{
					// 显示
					m_pSkillArea->SetShowEnable(TRUE);
				}
				m_pSkillArea->SetPosition(fvMouseHitPlan);

				if(pSkillImpl)
				{
					// 设置尺寸
					m_pSkillArea->SetRingRange( pSkillImpl->GetDemageRange() );
				}
			}
			else if(m_pSkillArea && m_pSkillArea->GetShowEnable())
			{
				// 隐藏
				m_pSkillArea->SetShowEnable(FALSE);
			}

		}

		//if(SCommand_Mouse::MCT_REPAIR == cmd.m_typeMouse)	//"修理"单独处理
		//	CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);

	}


	// 分析事件队列
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;

		switch(event.nEvent)
		{
		// 右键点击
		case EVENT_MOUSE_RBDOWN:
			{
				m_bCameraDraging = TRUE;
				m_bIsLastCameraDraging = FALSE;
				event.bProcessed = true;
				// 隐藏鼠标
				s_pInputSystem->SetCapture(ICS_GAME);
				CCursorMng::GetMe()->ShowCursor(FALSE);
				::SetCapture(g_hMainWnd);

				// 跳跃
//				CObjectManager::GetMe()->GetMySelf()->Jump();

		}
			break;

		// 右键拖动
		case EVENT_MOUSE_RDRAG_MOVE:
			{
				// 调节摄像机方向
				if(m_bCameraDraging)
				{
					FLOAT fSub = max( FLOAT( nMousePosXDiff /10.0f ), 1.0f) / 100;

					BOOL iCamera_Rotation = (BOOL)s_pVariableSystem->GetAs_Int("Camera_Rotation");
					if(iCamera_Rotation)
					{
						s_pGfxSystem->Camera_AddDirection( FLOAT( -nMousePosXDiff /300.0f ));	//616======	锁定旋转
					}
					
					event.bProcessed = true;

					// 鼠标归到原来位置
					s_pInputSystem->MouseSetPos(ptOld.x, ptOld.y);
					CCursorMng::GetMe()->ShowCursor(FALSE);

					m_bIsLastCameraDraging = TRUE;
				}
				////这个时候不应该如果右键放开了,不应该在执行任何指令了,因为这次操作是因为右键拖动发起的
				//CCursorMng::GetMe()->MouseCommand_GetRight().Cleanup();
			}
			break;

		// 左键长时间按下
		case EVENT_MOUSE_LBDOWNREPEAT:
			{
				// 开始自动行走
				m_bAutoRun = TRUE;
				event.bProcessed = true;
			}
			break; 

		// 左键按下
		case EVENT_MOUSE_LBDOWN:
			{
				// 取消自动行走
				m_bAutoRun = FALSE; 
				event.bProcessed = true;

				// 设置按下的图标
//				CCursorMng::GetMe()->SetCursor( CURSOR_MINE );
			}
			break;

		// 左键点击
		case EVENT_MOUSE_LBUP:
			{
				// 激活鼠标命令
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

				// 设置抬起的图标
//				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
			}
			break;

		// 右键抬起
		case EVENT_MOUSE_RBUP:
			{
				// 显示鼠标
				CCursorMng::GetMe()->ShowCursor(true);
				//event.bProcessed = true;
				s_pInputSystem->SetCapture(ICS_NONE);
				::ReleaseCapture();
			}
			break;

		// 右键点击
		case EVENT_MOUSE_RBCLICK:
			{
				// 激活鼠标命令
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
		// 滚轮向上
		case EVENT_MOUSE_WHEEL_UP:
			{
				s_pGfxSystem->Camera_SetZoom( TRUE, 2 );
				event.bProcessed = true;
			}
			break;

		// 滚轮向下
		case EVENT_MOUSE_WHEEL_DOWN:
			{
				s_pGfxSystem->Camera_SetZoom( FALSE, 2 );
				event.bProcessed = true;
			}
			break;
		}
	}

	//-----------------------------------------------------------
	// 如果右键已经松开
	if(!bRBtnDown)
	{
		m_bCameraDraging = FALSE;
		if(m_bAutoRun) // 自动行中... 
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
		// 创建
		m_pMouseTarget = (CObject_ProjTex_MouseTarget*)CObjectManager::GetMe()->NewObject("CObject_ProjTex_MouseTarget");
		m_pMouseTarget->Initial(NULL);
	}

	// 判断目标是否不可走
	m_pMouseTarget->SetReachAble( !CPath::IsPointInUnreachRegion( fvPos ) );
	// 设置位置
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

	// 释放MouseTarget
	if(m_pMouseTarget)
	{
		m_pMouseTarget->Destroy();
		m_pMouseTarget = NULL;
	}

	// 释放SkillArea
	if(m_pSkillArea)
	{
		m_pSkillArea->Destroy();
		m_pSkillArea = NULL;
	}


	//test 性能报告
//	AxProfile::AxProfile_Report();


	// 保存摄像机距离
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

