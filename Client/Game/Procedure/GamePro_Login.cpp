
#include "StdAfx.h"
#include "Gamepro_login.h"
#include "GamePro_Enter.h"
#include "GamePro_CharSel.h"
#include "Global.h"
#include "Network\NetManager.h"
#include "World\WorldManager.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "CGConnect.h"
#include "CGEnterScene.h"
#include "GIVariable.h"
#include "GIInputSystem.h"
#include "GIEventDefine.h"
#include "GIUISystem.h"
#include "AxProfile.h"
#include "Event\GMEventSystem.h"
#include <string>
#include "Version.h"

#include "CLConnect.h"					// 登录__服务器同步消息
#include "CLAskLogin.h"					// 登录__发送验证用户名密码
#include "CLAskCharList.h"				// 登录__发送得到角色消息



CGamePro_Login::CGamePro_Login()
{
	//m_Status = LOGIN_DEBUG_SETTING;
	m_Status = LOGIN_SELECT_SERVER;
	m_bReLogin = FALSE;
	m_pSoundSource = NULL;

	// 读取服务器配置信息.
	LoadLoginServerInfo();

	// 构造自动选择服务器查找表.
	BuildAutoSelTable();

	// 用户名
	m_strUserName = "";

	// 密码
	m_strPassword = "";

	for(int i = 0; i < PROVIDE_COUNT; i++ )
	{
		// 当前选择的区域
		m_iAutoAreaIndex[i] = -1;

		// 当前选择的login server 索引
		m_iAutoLoginServerIndex[i] = -1;
	}

	// 前一次找到网络状态空闲的索引.
	m_iPreFindIdle = -1;

	// 前一次找到网络状态正常的索引.
	m_iPreFindNor  = -1;

	// 前一次找到网络状态拥挤的索引.
	m_iPreFindBusy = -1;

	// 前一次找到网络状态爆满的索引.
	m_iPreFindFull = -1;

	m_nPrevServer = -1;
	m_nPrevArea = -1;

}

CGamePro_Login::~CGamePro_Login()
{
	delete []m_pAreaInfo;
}

VOID CGamePro_Login::FirstLogin(VOID)
{
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Mode", (INT)LOGIN_FIRST_LOGIN);
}

VOID CGamePro_Login::LoginForChangeScene(VOID)
{
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Mode", (INT)LOGIN_CHANGE_SCENE);
}

VOID CGamePro_Login::Init(VOID)
{
	// 禁止改变窗口大小。
	LONG lStyle = ::GetWindowLong(g_hMainWnd, GWL_STYLE);
	lStyle = lStyle & ~WS_THICKFRAME ;
	lStyle = lStyle & ~WS_MAXIMIZEBOX ;
	::SetWindowLong(g_hMainWnd, GWL_STYLE, lStyle);//
	
	//Rresh title
	::PostMessage(g_hMainWnd, WM_NCACTIVATE, TRUE, 0);

	CGameProcedure::s_pVariableSystem->SetAs_Vector2("View_Resoution", 1024,768, FALSE);

	//--- for debug

	// 直接连GameServer
	if( CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1 ) 
	{
		return ;
	}

	//616======* 单机
	if( CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1 )	
	{
		//616====== 连机测试
		SelectLoginServer(0, 0);
		return;
	}

	//--- for debug


	//打开登录界面
	if(LOGIN_SELECT_SERVER == m_Status)
	{
		s_pEventSystem->PushEvent(GE_GAMELOGIN_OPEN_SELECT_SERVER);
	}

	// 选择旧的服务器
	SelectOldServer();
}

// 在界面上选择服务器
VOID CGamePro_Login::SelectOldServer()
{
	int iPreCurSelArea		  = CGameProcedure::s_pVariableSystem->GetAs_Int("Login_Area");
    int iPreCurSelLoginServer = CGameProcedure::s_pVariableSystem->GetAs_Int("Login_Server");

	if((iPreCurSelArea < m_iAreaCount)&&(iPreCurSelArea >= 0))
	{
		int iLoginServerCount = GetAreaLoginServerCount(iPreCurSelArea);

		if((iPreCurSelLoginServer < iLoginServerCount)&&(iPreCurSelLoginServer >= 0))
		{
			// 选择前一次选择的服务器.
			s_pEventSystem->PushEvent(GE_GAMELOGIN_SELECT_AREA,			iPreCurSelArea);		// 选择区域
			s_pEventSystem->PushEvent(GE_GAMELOGIN_SELECT_LOGINSERVER,	iPreCurSelLoginServer);	// 选择Login Server
			return;
		}
	}
}

VOID CGamePro_Login::Tick(VOID)
{
	CGameProcedure::Tick();

	switch(m_Status)
	{
	case LOGIN_DEBUG_SETTING:
		{
			if(!CGameProcedure::s_pUISystem)
			{
				SetStatus(CGamePro_Login::LOGIN_DISCONNECT);
			}
			else
			{
				//DO NOTING,WAIT UI...
			}
		}
		break;

	case LOGIN_SELECT_SERVER:// 选择服务器状态
		{
			//--- for debug
			if(CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1)
			{
				//直接切换到Change-Server流程
				CGameProcedure::SetActiveProc( (CGameProcedure*)CGameProcedure::s_pProcChangeScene );
				return;
			}
			//--- for debug
			break;
		}
	case LOGIN_DISCONNECT:
		{
			s_pGfxSystem->PushDebugString("Connect to login server %s:%d...", m_szLoginServerAddr, m_nLoginServerPort);

			//开始登录
			SetStatus(LOGIN_CONNECTING);
			CNetManager::GetMe()->ConnectToServer(m_szLoginServerAddr, m_nLoginServerPort);
		}
		break;

	case LOGIN_CONNECTING:
		break;
	
	// 连接成功
	case LOGIN_CONNECTED_OK:
		{
			// 设置正在验证密码
			// SetStatus(LOGIN_ACCOUNT_REQUESTING);
		}
		break;

	// 连接失败
	case LOGIN_CONNECT_FAILED:
		{
			CNetManager::GetMe()->Close();
			SetStatus(LOGIN_SELECT_SERVER);
		}
		break;

	// 正在验证用户名和密码.
	case LOGIN_ACCOUNT_REQUESTING:
		{
			// 判断是否超时, 超时就提示错误信息.
			break;
		}

	case LOGIN_ACCOUNT_BEGIN_REQUESTING:
		{
			break;
		}

	// 登录信息验证成功
	case LOGIN_ACCOUNT_OK:
		{
			// 保存选择的服务器
			CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
			CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

			if(m_bReLogin)
			{
				//直接进入场景
				CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_READY);
				CGameProcedure::s_pProcEnter->SetEnterType(ENTER_TYPE_FIRST);
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcEnter);
			}
			else
			{
				// 设置登录状态为首次登录（以区分切换场景的登录状态）
				CGameProcedure::s_pProcLogIn->FirstLogin();

				//转入人物选择循环
				CGameProcedure::SetActiveProc((CGameProcedure*)s_pProcCharSel);
			}
		}
		break;
	default:
		break;
	}
}

VOID CGamePro_Login::Render(VOID)
{
	if( s_pGfxSystem ) 
		s_pGfxSystem->RenderFrame();
}

VOID CGamePro_Login::Release(VOID)
{
}

VOID CGamePro_Login::ProcessInput(VOID)
{
	// 分析事件队列
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;
		if(event.nEvent != EVENT_KEY_UP) continue;
		switch(event.kIndex)
		{
		case KC_GRAVE:
			CGameProcedure::s_pEventSystem->PushEvent(GE_TOGLE_CONSOLE);
			event.bProcessed = true;
			break;

		case KC_D:
			{
				if(!(s_pInputSystem->IsKeyDown(KC_LMENU))) 
					break;

				if(s_pUISystem)
					s_pUISystem->Debug_SaveFontTexture();
			}
			break;

		default:
			break;
		}
	}
}



//------------------------------------------------------------------------------------------------------------
//
// 从配置文件得到, login server 的ip地址和端口号
//
//

// 设置端口号.
VOID CGamePro_Login::SetPort(INT iPort)
{
	m_nLoginServerPort = iPort;
}

// 设置ip地址
VOID CGamePro_Login::SetIpAddr(LPCTSTR szServerAddr)
{
	memcpy(m_szLoginServerAddr, szServerAddr, sizeof(m_szLoginServerAddr));
}


// 读取login server信息
//
// 返回值: 
//			0 -- 读取成功
//			1 -- 失败找不到配置文件
//          2 -- 其他错误    
int CGamePro_Login::LoadLoginServerInfo(std::string strLoginServerInfoFile)
{
	FILE* pFile = NULL;
	int iReadAreaCount = 0;

	pFile = ::fopen(strLoginServerInfoFile.c_str(), "rt");
	if(NULL == pFile)
	{
		return LOAD_LOGIN_SERVER_FILE_NOT_FINDED;
	}
	else
	{
		fclose(pFile);
		pFile = NULL;
	}
	DBCFile dbcFile(0);
	if( !dbcFile.OpenFromTXT(strLoginServerInfoFile.c_str()) )
	{
		return LOAD_LOGIN_SERVER_OTHER_ERR;
	}

	INT nCount = dbcFile.GetRecordsNum();

	//扫描区域
	typedef std::map<std::string, std::vector<_DBC_LOGINSERVER_DATA*> > AreaMap;
	AreaMap am;
	for (INT i=0; i<nCount; i++)
	{
		_DBC_LOGINSERVER_DATA* pLine = (_DBC_LOGINSERVER_DATA*)dbcFile.Search_Posistion(i, 0);
		AreaMap::iterator it = am.find(pLine->szAreaName);
		if (it == am.end())
		{
			std::vector<_DBC_LOGINSERVER_DATA*> v;
			v.push_back(pLine);
			am.insert(std::make_pair(pLine->szAreaName, v));
		}
		else
		{
			it->second.push_back(pLine);
		}
	}

	m_pAreaInfo = new AREA_INFO[am.size() + 1];

	//处理服务器
	AreaMap::iterator it = am.begin();
	AreaMap::iterator ed = am.end();

	for (INT i =0; it != ed; ++it, ++i)
	{
		m_pAreaInfo[i].iLoginServerCount = (INT)it->second.size() ;
		strncpy(m_pAreaInfo[i].szAreaName, it->first.c_str(), 128);
		if (m_pAreaInfo[i].iLoginServerCount <= 0)
		{
			continue;
		}
		m_pAreaInfo[i].iProvide =  (NET_PROVIDE)it->second[0]->nProvider;
		for (INT j=0; j<MAX_SERVER_EVERY_AREA && j<m_pAreaInfo[i].iLoginServerCount ; j++)
		{
			m_pAreaInfo[i].LoginInfo[j].iLoginServerStatus =(LOGIN_SERVER_STATUS) it->second[j]->nServerStatus;
			m_pAreaInfo[i].LoginInfo[j].nIsNew = it->second[j]->nIsNewServer;
			m_pAreaInfo[i].LoginInfo[j].nServerAreaID = it->second[j]->nServerAreaID;
			m_pAreaInfo[i].LoginInfo[j].nTuiJian = it->second[j]->nTuiJian;
			strncpy( m_pAreaInfo[i].LoginInfo[j].szTooltips,  it->second[j]->szTip, 128);
			strncpy( m_pAreaInfo[i].LoginInfo[j].szLoginServerName,  it->second[j]->szServerName, 128);
			
			STRING strIP = it->second[j]->szServerIP;
			size_t pos = strIP.find_first_of(':');
			strncpy(m_pAreaInfo[i].LoginInfo[j].szIp, strIP.substr(0,pos).c_str(), 64);
			m_pAreaInfo[i].LoginInfo[j].iPort= atoi(strIP.substr(pos+1).c_str());
		}
	}
	m_iAreaCount = (INT)am.size();
	return LOAD_LOGIN_SERVER_SUCCESS;
}

// 初始化区域信息
void CGamePro_Login::InitLoginAreaInfo()
{
	// 区域信息
	m_pAreaInfo = NULL;

	// 区域个数
	m_iAreaCount = 0;
}


// 读取区域信息.
int CGamePro_Login::ReadAreaInfo(FILE* pFile, int iAreaIndex)
{
	if(NULL == pFile)
	{
		return LOAD_LOGIN_SERVER_FILE_NOT_FINDED;
	}

	if(iAreaIndex >= m_iAreaCount)
	{
		return LOAD_LOGIN_SERVER_OTHER_ERR;
	}

	char bufInfo[512];
	char bufTempInfo1[128];
	char bufTempInfo2[128];
	char bufTempInfo3[128];


	while(!feof(pFile))
	{
		
		// 得到区域的名字.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_OTHER_ERR;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %s", bufTempInfo1, bufTempInfo2, bufTempInfo3);
		if(strcmp("区域名字", bufTempInfo1))
		{
			// 如果没有得到正确的区域, 一直读取错误的区域.
			continue;
		}

		// 保存区域的名字.
		strncpy(m_pAreaInfo[iAreaIndex].szAreaName, bufTempInfo3, sizeof(m_pAreaInfo[iAreaIndex].szAreaName));

		// 读取供应商.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %d", bufTempInfo1, bufTempInfo2, &(m_pAreaInfo[iAreaIndex].iProvide));


		// 读取login server 的个数.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %d", bufTempInfo1, bufTempInfo2, &(m_pAreaInfo[iAreaIndex].iLoginServerCount));

		int iReadRes = 0;
		for(int i = 0; i < m_pAreaInfo[iAreaIndex].iLoginServerCount; i++)	
		{
			iReadRes = ReadLoginInfo(pFile, iAreaIndex, i);
			if(iReadRes)
			{
				return iReadRes;
			}
		}

		// 读完一个区域信息就返回
		return LOAD_LOGIN_SERVER_SUCCESS;

	}
	
	// 没有读取完整区域的名字.
	return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
}


// 读取login server信息.
int CGamePro_Login::ReadLoginInfo(FILE* pFile, int iAreaIndex, int iLoginServerIndex)
{
	
	if(NULL == pFile)
	{
		return LOAD_LOGIN_SERVER_FILE_NOT_FINDED;
	}

	char bufInfo[512];
	char bufTempInfo1[128];
	char bufTempInfo2[128];
	char bufTempInfo3[128];


	while(!feof(pFile))
	{
		//-------------------------------------------------------
		// 得到login server 的名字.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_OTHER_ERR;
		}

		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %s", bufTempInfo1, bufTempInfo2, bufTempInfo3);
		if(strcmp("LoginServer名字", bufTempInfo1))
		{
			// 如果没有得到login server, 一直读取错误的行.
			continue;
		}

		// 保存login server 的名字.
		strncpy(
				m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szLoginServerName, 
				bufTempInfo3, 
				sizeof(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szLoginServerName)
				);

		//--------------------------------------------------------
		// 读取状态
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(
					bufInfo, 
					"%s %s %d", 
					bufTempInfo1, 
					bufTempInfo2, 
					&(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iLoginServerStatus)
					);


		//--------------------------------------------------------
		// 读取ip地址
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(
					bufInfo, 
					"%s %s %s", 
					bufTempInfo1, 
					bufTempInfo2, 
					m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szIp
					);


		//---------------------------------------------------------
		// 读取端口号
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(
					bufInfo, 
					"%s %s %d", 
					bufTempInfo1, 
					bufTempInfo2, 
					&(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iPort)
					);

		// 读完一个login server 返回.
		return LOAD_LOGIN_SERVER_SUCCESS;

	}
	
	// 没有读取完整区域的名字.
	return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;//

}


//-----------------------------------------------------------------------------------------------------------------------------
//
// 逻辑数据操作
//
//

// 得到区域的个数
int CGamePro_Login::GetAreaCount()
{
	return m_iAreaCount;
}

// 得到区域名字
const TCHAR* CGamePro_Login::GetAreaName(int iAreaIndex)
{
	if(iAreaIndex >= m_iAreaCount)
	{
		return _T("");
	}

	return m_pAreaInfo[iAreaIndex].szAreaName;
}

// 得到区域信息
PAREA_INFO CGamePro_Login::GetAreaInfo(int iAreaIndex)
{
	if((iAreaIndex >= m_iAreaCount)&&(iAreaIndex < 0))
	{
		return NULL;
	}

	return (m_pAreaInfo + iAreaIndex);
}

// 通过网络供应商选择一个login server
int CGamePro_Login::AutoSelLoginServer(int iProvide)
{
	PPROVIDE_INFO pProvideInfo = NULL;
	int iAreaIndex        = -1;
	int iLoginServerIndex = -1;

	switch(iProvide)
	{
	case 0:// 电信供应商
		{
			pProvideInfo = &m_Provide0;
			break;
		}
	case 1:// 网通供应商
		{
			pProvideInfo = &m_Provide1;
			break;
		}
	case 2:// 其他供应商
		{
			pProvideInfo = &m_Provide2;
			break;
		}
	default:
		{
			break;
		}
	}

	int iLoginServerCount = (int)pProvideInfo->IdleLoginServerVector.size();
	// 查找空闲的服务器
	if(iLoginServerCount)
	{
		if(m_iPreFindIdle + 1 <  iLoginServerCount)
		{
			m_iPreFindIdle++;
			
		}
		else
		{
			m_iPreFindIdle = 0;
		}

		iAreaIndex        = pProvideInfo->IdleLoginServerVector[m_iPreFindIdle].iAreaIndex;
		iLoginServerIndex = pProvideInfo->IdleLoginServerVector[m_iPreFindIdle].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->NorLoginServerVector.size();
	// 查找正常的服务器
	if(iLoginServerCount)
	{
		if(m_iPreFindNor + 1 <  iLoginServerCount)
		{
			m_iPreFindNor++;
		}
		else
		{
			m_iPreFindNor = 0;
		}

		iAreaIndex        = pProvideInfo->NorLoginServerVector[m_iPreFindNor].iAreaIndex;
		iLoginServerIndex = pProvideInfo->NorLoginServerVector[m_iPreFindNor].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->BusyLoginServerVector.size();
	// 查找繁忙的服务器
	if(iLoginServerCount)
	{
		if(m_iPreFindBusy + 1 <  iLoginServerCount)
		{
			m_iPreFindBusy++;
		}
		else
		{
			m_iPreFindBusy = 0;
		}

		iAreaIndex        = pProvideInfo->BusyLoginServerVector[m_iPreFindBusy].iAreaIndex;
		iLoginServerIndex = pProvideInfo->BusyLoginServerVector[m_iPreFindBusy].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->FullLoginServerVector.size();
	// 查找爆满的服务器
	if(iLoginServerCount)
	{
		if(m_iPreFindFull + 1 <  iLoginServerCount)
		{
			m_iPreFindFull++;
		}
		else
		{
			m_iPreFindFull = 0;
		}

		iAreaIndex        = pProvideInfo->FullLoginServerVector[m_iPreFindFull].iAreaIndex;
		iLoginServerIndex = pProvideInfo->FullLoginServerVector[m_iPreFindFull].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}


	return 0;
}

// 构造自动选择login server需要的数据表
int CGamePro_Login::BuildAutoSelTable()
{
	LOGIN_SERVER_POS LoginServerPos;
	PPROVIDE_INFO			 pProvideInfo     = NULL;
	LOGIN_SERVER_POS_VECTOR* pServerPosVector = NULL;
	int iLoginServerCount = 0;
	for(int i = 0; i < m_iAreaCount; i++)
	{
		switch(m_pAreaInfo[i].iProvide)
		{
		case 0:// 电信供应商
			{
				pProvideInfo = &m_Provide0;
				break;
			}
		case 1:// 网通供应商
			{
				pProvideInfo = &m_Provide1;
				break;
			}
		case 2:// 其他供应商
			{
				pProvideInfo = &m_Provide2;
				break;
			}
		}

		iLoginServerCount = m_pAreaInfo[i].iLoginServerCount;
		for(int j = 0; j < iLoginServerCount; j++)
		{
			LoginServerPos.iAreaIndex        = i;
			LoginServerPos.iLoginServerIndex = j;
		
			switch(m_pAreaInfo[i].LoginInfo[j].iLoginServerStatus)
			{
			case LOGIN_SERVER_FULL:
				{
					pServerPosVector = &(pProvideInfo->FullLoginServerVector);
					break;
				}
			case LOGIN_SERVER_BUSY:
				{
					pServerPosVector = &(pProvideInfo->BusyLoginServerVector);
					break;
				}
			case LOGIN_SERVER_NORMAL:
				{
					pServerPosVector = &(pProvideInfo->NorLoginServerVector);
					break;
				}
			case LOGIN_SERVER_IDLE:
				{
					pServerPosVector = &(pProvideInfo->IdleLoginServerVector);
					break;
				}
			default:
				{

				}

			}// switch(m_pAreaInfo[i].LoginInfo[j].iLoginServerStatus)

			pServerPosVector->push_back(LoginServerPos);

		}// for(int j = 0; j < iLoginServerCount; j++)

	}// for(int i = 0; i < m_iAreaCount; i++)

	return 0;
}



	// 得到区域中login server 的个数
int CGamePro_Login::GetAreaLoginServerCount(int iAreaIndex)
{
	if(iAreaIndex >= m_iAreaCount || iAreaIndex<0)
	{
		return 0;
	}

	return m_pAreaInfo[iAreaIndex].iLoginServerCount;
}

// 得到区域中login server 的个数
PLOGINSERVER_INOF CGamePro_Login::GetAreaLoginServerInfo(int iAreaIndex, int iLoginServerIndex)
{
	if(iAreaIndex >= m_iAreaCount)
	{
		return NULL;
	}

	if(iLoginServerIndex >= m_pAreaInfo[iAreaIndex].iLoginServerCount)
	{
		return NULL;
	}

	return  (m_pAreaInfo[iAreaIndex].LoginInfo + iLoginServerIndex);
}


// 连接到login server
int CGamePro_Login::ConnectToLoginServer(int iAreaIndex, int iLoginServerIndex)
{

	if(iAreaIndex >= m_iAreaCount)
	{
		return 1;
	}

	if(iLoginServerIndex >= m_pAreaInfo[iAreaIndex].iLoginServerCount)
	{
		return 1;
	}

	// 设置ip地址和端口号.
	SetIpAddr(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szIp);
	SetPort(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iPort);

	// 设置当前的状态为非连接状态
	SetStatus(LOGIN_DISCONNECT);

	// 通知界面显示系统提示信息, 正在连接服务器.

	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_login_Info_Connecting_Server");
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET, strTemp.c_str());


	return 0;

}


// 记录选择的服务器login server
int CGamePro_Login::SelectLoginServer(int iAreaIndex, int iLoginServerIndex)
{
	// 记录选择的服务器
	m_iCurSelAreaId			= iAreaIndex;
	m_iCurSelLoginServerId  = iLoginServerIndex;

	OpenCountInputDlg();

	return 0;
}


// 连接到login server
int CGamePro_Login::ConnectToLoginServer()
{

	// 设置当前的状态为非连接状态
	SetStatus(LOGIN_DISCONNECT);

	// 通知界面显示系统提示信息, 正在连接服务器.
	
	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_login_Info_Connecting_Server");
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
	return 0;
}


// 得到游戏版本
int CGamePro_Login::GetGameVersion()
{
	return CURRENT_VERSION;
}






//-------------------------------------------------------------------------------------------------------------------------------
//
// 界面操作
//

// 打开帐号输入界面
int CGamePro_Login::OpenCountInputDlg()
{
	// 打开帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_COUNT_INPUT);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭服务器选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_SERVER);

	const PLOGINSERVER_INOF pLoginServerInfo = GetAreaLoginServerInfo(m_iCurSelAreaId,  m_iCurSelLoginServerId);
	if(pLoginServerInfo)
	{
		// 保存选择的服务器
		CGameProcedure::s_pVariableSystem->SetVariable("Login_Area",	GetAreaName(m_iCurSelAreaId), FALSE);
		CGameProcedure::s_pVariableSystem->SetVariable("Login_Server",	pLoginServerInfo->szLoginServerName, FALSE);
		CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Provider", 0/*m_netProvider*/, FALSE);

		CGameProcedure::s_pVariableSystem->SetVariable("Login_ServerName", 	pLoginServerInfo->szLoginServerName, TRUE);
	}

	// 设置帐号输入界面
	//SetStatus(LOGIN_ACCOUNT_BEGIN_REQUESTING);

	return 0;
}

// 打开服务器选择界面
int CGamePro_Login::OpenSelectServerDlg()
{
	// 打开服务器选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_SERVER);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);

	// 设置服务器选择界面
	SetStatus(LOGIN_SELECT_SERVER);

	// 断开网络连接
	CNetManager::GetMe()->Close();

	SelectOldServer();
	return 0;
}


// 打开人物选择界面
int CGamePro_Login::OpenSelectCharacter()
{
	return 0;
}

// 切换到人物选择界面
int CGamePro_Login::ChangeToCharacterSel()
{
	// 保存选择的服务器
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);

	// 显示人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_CHARACTOR);

	// 设置用户名
	CGameProcedure::s_pProcCharSel->SetUserName(m_strUserName.c_str());

	// 进入到人物选择界面， 清空ui模型。
	CGameProcedure::s_pProcCharSel->m_bClearUIModel = TRUE;


	// 发送得到角色信息消息.
	CLAskCharList msg;
	msg.SetAccount((CHAR*)m_strUserName.c_str());
	CNetManager::GetMe()->SendPacket(&msg);


	//切换到人物选择界面
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);

	return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------
//
// 网络命令消息操作
//

// 发送同步消息
int CGamePro_Login::SendClConnectMsg()
{
	CLConnect msg;

	// 发送网络连接消息
	CNetManager::GetMe()->SendPacket(&msg);

	return 0;
}

// 检查用户名和密码.
int CGamePro_Login::CheckAccount(CHAR* szUserName, CHAR* szPassword)
{
	//616======* 单机
	if( (CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1)
//^		 || (CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1)
	  )
	{
		// 转到人物选择界面
		ChangeToCharacterSel();
		return 0;
	}

	if(NULL == szUserName)
	{
		// 通知界面显示系统提示信息, 用户名错误
			
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Input_Name_err");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		return 0;
	}

	if(NULL == szPassword)
	{
		// 通知界面显示系统提示信息, 用户名错误
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Input_Password_e_err");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		return 0;
	}

	m_strUserName = szUserName;
	m_strPassword = szPassword;
	CGameProcedure::s_pVariableSystem->SetVariable("User_Account", m_strUserName.c_str());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 验证用户名
	//
	int iPos = (int)m_strUserName.find('\'');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Name_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "用户名不能含有 \" \' \" ");
		return 0;
	}

    iPos = (int)m_strUserName.find('"');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Name_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "用户名不能含有 \"  \"  \" ");
		return 0;
	}

	iPos = (int)m_strUserName.find('\\');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Name_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "用户名不能含有 \" \\ \" ");
		return 0;
	}//


	//// 到非法字符串表中查找。
	//int iFindPos = -1;
	//char bufInfo[512];
	//for(int i = 0; i < (int)m_UnUseStrVector.size(); i++)
	//{
	//	iFindPos = (int)m_strUserName.find(m_UnUseStrVector[i]);
	//	if(-1 != iFindPos)
	//	{
	//		memset(bufInfo, 0, sizeof(bufInfo));
	//		sprintf(bufInfo, "用户名包含非法符号%s", m_UnUseStrVector[i]);
	//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, bufInfo);
	//		return 0;
	//	}

	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 验证密码
	//
	iPos = (int)m_strPassword.find('\'');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Password_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "密码不能含有 \" \' \" ");
		return 0;
	}

    iPos = (int)m_strPassword.find('"');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Password_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "密码不能含有 \"  \"  \" ");
		return 0;
	}

	iPos = (int)m_strPassword.find('\\');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Password_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "密码不能含有 \" \\ \" ");
		return 0;
	}//

	//// 到非法字符串表中查找
	//for(int j = 0; j < (int)m_UnUseStrVector.size(); j++)
	//{
	//	iFindPos = (int)m_strPassword.find(m_UnUseStrVector[j]);
	//	if(-1 != iFindPos)
	//	{
	//		memset(bufInfo, 0, sizeof(bufInfo));
	//		sprintf(bufInfo, "密码包含非法符号%s", m_UnUseStrVector[j]);
	//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, bufInfo);
	//		return 0;
	//	}
	//}

	// 与服务器进行socket 连接.
	ConnectToLoginServer(m_iCurSelAreaId, m_iCurSelLoginServerId);
	return 0;
}

// 发送check msg 消息
int CGamePro_Login::SendCheckAccountMsg()
{
	// 发送验证密码消息
	CLAskLogin msg;
	msg.SetAccount((CHAR*)m_strUserName.c_str());
	msg.SetPassWord((CHAR*)m_strPassword.c_str());
	msg.SetVersion(GetGameVersion());
	CNetManager::GetMe()->SendPacket(&msg);//

	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_login_Info_Checking_Password");
	
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "正在验证密码....");
	return 0;
}


// 
// 
//
//------------------------------------------------------------------------------------------------------------
// 发送上次登录的服务器和区域
VOID CGamePro_Login::GetLastLoginServerID()
{
	STRING strTemp;

	m_nPrevServer = -1;
	m_nPrevArea = -1;

	//搜索上次选择的服务器区
	strTemp = s_pVariableSystem->GetAs_String("Login_Area");
	if(!strTemp.empty())
	{
		for(int i=0; i<(int)m_iAreaCount; i++)
		{
			if(m_pAreaInfo[i].szAreaName==strTemp) 
			{
				m_nPrevArea=i;
				break;
			}
		}
	}

	//搜索上次选择的服务器
	strTemp = s_pVariableSystem->GetAs_String("Login_Server");
	if(!strTemp.empty() && m_nPrevArea>=0)
	{
		for(INT i=0; i<MAX_SERVER_EVERY_AREA; i++)
		{
			if(m_pAreaInfo[m_nPrevArea].LoginInfo[i].szLoginServerName==strTemp) 
			{
				m_nPrevServer=i;
				break;
			}
		}
	}
}

VOID CGamePro_Login::GetLastLoginAreaAndServerIndex(INT& area, INT &serverId, STRING&name)
{
	GetLastLoginServerID();
	area = m_nPrevArea;
	serverId = m_nPrevServer;
	name = s_pVariableSystem->GetAs_String("Login_Server");
}