
#include "StdAfx.h"
#include "Network\NetManager.h"
#include "Gamepro_ChangeScene.h"
#include "GamePro_Enter.h"
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
#include "Event\GMEventSystem.h"
#include "GameStruct.h"
#include "CLAskCreateChar.h"
#include "CGConnect.h"
#include "Global.h"
#include "dbc/GMDataBase.h"
#include "GIGfxSystem.h"
#include "GIVariable.h"
#include "GIException.h"



CGamePro_ChangeScene::CGamePro_ChangeScene(void)
{
	m_Status = CHANGESCENE_DISCONNECT;
}

CGamePro_ChangeScene::~CGamePro_ChangeScene(void)
{
}


VOID CGamePro_ChangeScene::Init(VOID)
{
}

VOID CGamePro_ChangeScene::Tick(VOID)
{
	CGameProcedure::Tick();

	switch( GetStatus() )
	{
	case CHANGESCENE_DISCONNECT:
		{
			// 连接到 game server(socket) 
			ConnectToGameServer();
			break;
		}
	case CHANGESCENE_CONNECT_SUCCESS:
		{
			//SetStatus(ENTERSCENE_REQUESTING);

			// 发送CGConnect 消息
			SendCGConnectMsg();
			break;
		}
	case CHANGESCENE_SENDING_CGCONNECT:
		{
			break;
		}
	case CHANGESCENE_RECEIVE_CGCONNECT_SUCCESS:
		{
			ChangeToEnterGameProdure();
			break;
		}
	case CHANGESCENE_RECEIVE_CGCONNECT_FAIL:
		{
			break;
		}
	default:
		{
			break;
		}
	}
	
}

VOID CGamePro_ChangeScene::Render(VOID)
{
	if( s_pGfxSystem ) 
		s_pGfxSystem->RenderFrame();
}

VOID CGamePro_ChangeScene::Release(VOID)
{
}

// 设置状态
void CGamePro_ChangeScene::SetStatus(PLAYER_CHANGE_SERVER_STATUS status)
{
	m_Status = status;
}

// 得到状态
CGamePro_ChangeScene::PLAYER_CHANGE_SERVER_STATUS CGamePro_ChangeScene::GetStatus(VOID)
{
	return m_Status; 
}//

// 连接到服务器socket
void CGamePro_ChangeScene::ConnectToGameServer()
{
	std::string strIp = CGameProcedure::s_pVariableSystem->GetAs_String("GameServer_Address");
	INT			iPort = CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_Port");
	CNetManager::GetMe()->ConnectToServer(strIp.c_str(), iPort);
	SetStatus( CHANGESCENE_CONNECTING );
}


// 发送cgconnect
void CGamePro_ChangeScene::SendCGConnectMsg()
{
	CGConnect msg;

	msg.SetServerID( INVALID_ID );
	msg.SetKey( (UINT)s_pVariableSystem->GetAs_Int			("GameServer_Key") );		// 服务器端生成的一个关键字
	msg.SetGUID( (GUID_t)s_pVariableSystem->GetAs_Int		("User_GUID") );			// 服务器端生成的用户唯一的标识
	msg.SetAccount( (CHAR*)(s_pVariableSystem->GetAs_String	("User_NAME").c_str()) );	// 玩家的名称（在选择界面保存）
	msg.SetGender( s_pVariableSystem->GetAs_Int				("User_GENDER") );			// 性别（在选择界面保存）
	msg.SetProfession( s_pVariableSystem->GetAs_Int			("User_MENPAI") );			// 职业（从对话框取得）
	msg.SetCountry( s_pVariableSystem->GetAs_Int			("User_COUNTRY") );			// 国家

	// 如果是直接连接到 GameServer
	if( CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1 )
	{
		msg.SetFaceModel( INVALID_ID );													// 脸部模型（直连时不需要选择， 用默认的。
																						//			 在创建界面可以选择， 保存到Login中）
	}

	CNetManager::GetMe()->SendPacket(&msg);
	SetStatus( CHANGESCENE_SENDING_CGCONNECT );
}

// 切换到enter game 流程
void CGamePro_ChangeScene::ChangeToEnterGameProdure()
{
	// 在标题栏显示游戏服务器ip
	CHAR szTitle[MAX_PATH];
	_snprintf(	szTitle, MAX_PATH, "%s %s (%s %s) (GameServer:%s)",
				GAME_TITLE, VERSION_INFO, __DATE__, __TIME__, CGameProcedure::s_pVariableSystem->GetAs_String("GameServer_Address").c_str() );
	::SetWindowText(g_hMainWnd, szTitle);


	// 切换到进入场景流程
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcEnter);
	CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_READY);
}
