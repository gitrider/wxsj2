
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

#include "CLConnect.h"					// ��¼__������ͬ����Ϣ
#include "CLAskLogin.h"					// ��¼__������֤�û�������
#include "CLAskCharList.h"				// ��¼__���͵õ���ɫ��Ϣ



CGamePro_Login::CGamePro_Login()
{
	//m_Status = LOGIN_DEBUG_SETTING;
	m_Status = LOGIN_SELECT_SERVER;
	m_bReLogin = FALSE;
	m_pSoundSource = NULL;

	// ��ȡ������������Ϣ.
	LoadLoginServerInfo();

	// �����Զ�ѡ����������ұ�.
	BuildAutoSelTable();

	// �û���
	m_strUserName = "";

	// ����
	m_strPassword = "";

	for(int i = 0; i < PROVIDE_COUNT; i++ )
	{
		// ��ǰѡ�������
		m_iAutoAreaIndex[i] = -1;

		// ��ǰѡ���login server ����
		m_iAutoLoginServerIndex[i] = -1;
	}

	// ǰһ���ҵ�����״̬���е�����.
	m_iPreFindIdle = -1;

	// ǰһ���ҵ�����״̬����������.
	m_iPreFindNor  = -1;

	// ǰһ���ҵ�����״̬ӵ��������.
	m_iPreFindBusy = -1;

	// ǰһ���ҵ�����״̬����������.
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
	// ��ֹ�ı䴰�ڴ�С��
	LONG lStyle = ::GetWindowLong(g_hMainWnd, GWL_STYLE);
	lStyle = lStyle & ~WS_THICKFRAME ;
	lStyle = lStyle & ~WS_MAXIMIZEBOX ;
	::SetWindowLong(g_hMainWnd, GWL_STYLE, lStyle);//
	
	//Rresh title
	::PostMessage(g_hMainWnd, WM_NCACTIVATE, TRUE, 0);

	CGameProcedure::s_pVariableSystem->SetAs_Vector2("View_Resoution", 1024,768, FALSE);

	//--- for debug

	// ֱ����GameServer
	if( CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1 ) 
	{
		return ;
	}

	//616======* ����
	if( CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1 )	
	{
		//616====== ��������
		SelectLoginServer(0, 0);
		return;
	}

	//--- for debug


	//�򿪵�¼����
	if(LOGIN_SELECT_SERVER == m_Status)
	{
		s_pEventSystem->PushEvent(GE_GAMELOGIN_OPEN_SELECT_SERVER);
	}

	// ѡ��ɵķ�����
	SelectOldServer();
}

// �ڽ�����ѡ�������
VOID CGamePro_Login::SelectOldServer()
{
	int iPreCurSelArea		  = CGameProcedure::s_pVariableSystem->GetAs_Int("Login_Area");
    int iPreCurSelLoginServer = CGameProcedure::s_pVariableSystem->GetAs_Int("Login_Server");

	if((iPreCurSelArea < m_iAreaCount)&&(iPreCurSelArea >= 0))
	{
		int iLoginServerCount = GetAreaLoginServerCount(iPreCurSelArea);

		if((iPreCurSelLoginServer < iLoginServerCount)&&(iPreCurSelLoginServer >= 0))
		{
			// ѡ��ǰһ��ѡ��ķ�����.
			s_pEventSystem->PushEvent(GE_GAMELOGIN_SELECT_AREA,			iPreCurSelArea);		// ѡ������
			s_pEventSystem->PushEvent(GE_GAMELOGIN_SELECT_LOGINSERVER,	iPreCurSelLoginServer);	// ѡ��Login Server
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

	case LOGIN_SELECT_SERVER:// ѡ�������״̬
		{
			//--- for debug
			if(CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1)
			{
				//ֱ���л���Change-Server����
				CGameProcedure::SetActiveProc( (CGameProcedure*)CGameProcedure::s_pProcChangeScene );
				return;
			}
			//--- for debug
			break;
		}
	case LOGIN_DISCONNECT:
		{
			s_pGfxSystem->PushDebugString("Connect to login server %s:%d...", m_szLoginServerAddr, m_nLoginServerPort);

			//��ʼ��¼
			SetStatus(LOGIN_CONNECTING);
			CNetManager::GetMe()->ConnectToServer(m_szLoginServerAddr, m_nLoginServerPort);
		}
		break;

	case LOGIN_CONNECTING:
		break;
	
	// ���ӳɹ�
	case LOGIN_CONNECTED_OK:
		{
			// ����������֤����
			// SetStatus(LOGIN_ACCOUNT_REQUESTING);
		}
		break;

	// ����ʧ��
	case LOGIN_CONNECT_FAILED:
		{
			CNetManager::GetMe()->Close();
			SetStatus(LOGIN_SELECT_SERVER);
		}
		break;

	// ������֤�û���������.
	case LOGIN_ACCOUNT_REQUESTING:
		{
			// �ж��Ƿ�ʱ, ��ʱ����ʾ������Ϣ.
			break;
		}

	case LOGIN_ACCOUNT_BEGIN_REQUESTING:
		{
			break;
		}

	// ��¼��Ϣ��֤�ɹ�
	case LOGIN_ACCOUNT_OK:
		{
			// ����ѡ��ķ�����
			CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
			CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

			if(m_bReLogin)
			{
				//ֱ�ӽ��볡��
				CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_READY);
				CGameProcedure::s_pProcEnter->SetEnterType(ENTER_TYPE_FIRST);
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcEnter);
			}
			else
			{
				// ���õ�¼״̬Ϊ�״ε�¼���������л������ĵ�¼״̬��
				CGameProcedure::s_pProcLogIn->FirstLogin();

				//ת������ѡ��ѭ��
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
	// �����¼�����
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
// �������ļ��õ�, login server ��ip��ַ�Ͷ˿ں�
//
//

// ���ö˿ں�.
VOID CGamePro_Login::SetPort(INT iPort)
{
	m_nLoginServerPort = iPort;
}

// ����ip��ַ
VOID CGamePro_Login::SetIpAddr(LPCTSTR szServerAddr)
{
	memcpy(m_szLoginServerAddr, szServerAddr, sizeof(m_szLoginServerAddr));
}


// ��ȡlogin server��Ϣ
//
// ����ֵ: 
//			0 -- ��ȡ�ɹ�
//			1 -- ʧ���Ҳ��������ļ�
//          2 -- ��������    
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

	//ɨ������
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

	//���������
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

// ��ʼ��������Ϣ
void CGamePro_Login::InitLoginAreaInfo()
{
	// ������Ϣ
	m_pAreaInfo = NULL;

	// �������
	m_iAreaCount = 0;
}


// ��ȡ������Ϣ.
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
		
		// �õ����������.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_OTHER_ERR;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %s", bufTempInfo1, bufTempInfo2, bufTempInfo3);
		if(strcmp("��������", bufTempInfo1))
		{
			// ���û�еõ���ȷ������, һֱ��ȡ���������.
			continue;
		}

		// �������������.
		strncpy(m_pAreaInfo[iAreaIndex].szAreaName, bufTempInfo3, sizeof(m_pAreaInfo[iAreaIndex].szAreaName));

		// ��ȡ��Ӧ��.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %d", bufTempInfo1, bufTempInfo2, &(m_pAreaInfo[iAreaIndex].iProvide));


		// ��ȡlogin server �ĸ���.
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

		// ����һ��������Ϣ�ͷ���
		return LOAD_LOGIN_SERVER_SUCCESS;

	}
	
	// û�ж�ȡ�������������.
	return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
}


// ��ȡlogin server��Ϣ.
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
		// �õ�login server ������.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_OTHER_ERR;
		}

		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %s", bufTempInfo1, bufTempInfo2, bufTempInfo3);
		if(strcmp("LoginServer����", bufTempInfo1))
		{
			// ���û�еõ�login server, һֱ��ȡ�������.
			continue;
		}

		// ����login server ������.
		strncpy(
				m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szLoginServerName, 
				bufTempInfo3, 
				sizeof(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szLoginServerName)
				);

		//--------------------------------------------------------
		// ��ȡ״̬
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
		// ��ȡip��ַ
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
		// ��ȡ�˿ں�
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

		// ����һ��login server ����.
		return LOAD_LOGIN_SERVER_SUCCESS;

	}
	
	// û�ж�ȡ�������������.
	return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;//

}


//-----------------------------------------------------------------------------------------------------------------------------
//
// �߼����ݲ���
//
//

// �õ�����ĸ���
int CGamePro_Login::GetAreaCount()
{
	return m_iAreaCount;
}

// �õ���������
const TCHAR* CGamePro_Login::GetAreaName(int iAreaIndex)
{
	if(iAreaIndex >= m_iAreaCount)
	{
		return _T("");
	}

	return m_pAreaInfo[iAreaIndex].szAreaName;
}

// �õ�������Ϣ
PAREA_INFO CGamePro_Login::GetAreaInfo(int iAreaIndex)
{
	if((iAreaIndex >= m_iAreaCount)&&(iAreaIndex < 0))
	{
		return NULL;
	}

	return (m_pAreaInfo + iAreaIndex);
}

// ͨ�����繩Ӧ��ѡ��һ��login server
int CGamePro_Login::AutoSelLoginServer(int iProvide)
{
	PPROVIDE_INFO pProvideInfo = NULL;
	int iAreaIndex        = -1;
	int iLoginServerIndex = -1;

	switch(iProvide)
	{
	case 0:// ���Ź�Ӧ��
		{
			pProvideInfo = &m_Provide0;
			break;
		}
	case 1:// ��ͨ��Ӧ��
		{
			pProvideInfo = &m_Provide1;
			break;
		}
	case 2:// ������Ӧ��
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
	// ���ҿ��еķ�����
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
	// ���������ķ�����
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
	// ���ҷ�æ�ķ�����
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
	// ���ұ����ķ�����
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

// �����Զ�ѡ��login server��Ҫ�����ݱ�
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
		case 0:// ���Ź�Ӧ��
			{
				pProvideInfo = &m_Provide0;
				break;
			}
		case 1:// ��ͨ��Ӧ��
			{
				pProvideInfo = &m_Provide1;
				break;
			}
		case 2:// ������Ӧ��
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



	// �õ�������login server �ĸ���
int CGamePro_Login::GetAreaLoginServerCount(int iAreaIndex)
{
	if(iAreaIndex >= m_iAreaCount || iAreaIndex<0)
	{
		return 0;
	}

	return m_pAreaInfo[iAreaIndex].iLoginServerCount;
}

// �õ�������login server �ĸ���
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


// ���ӵ�login server
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

	// ����ip��ַ�Ͷ˿ں�.
	SetIpAddr(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szIp);
	SetPort(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iPort);

	// ���õ�ǰ��״̬Ϊ������״̬
	SetStatus(LOGIN_DISCONNECT);

	// ֪ͨ������ʾϵͳ��ʾ��Ϣ, �������ӷ�����.

	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_login_Info_Connecting_Server");
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET, strTemp.c_str());


	return 0;

}


// ��¼ѡ��ķ�����login server
int CGamePro_Login::SelectLoginServer(int iAreaIndex, int iLoginServerIndex)
{
	// ��¼ѡ��ķ�����
	m_iCurSelAreaId			= iAreaIndex;
	m_iCurSelLoginServerId  = iLoginServerIndex;

	OpenCountInputDlg();

	return 0;
}


// ���ӵ�login server
int CGamePro_Login::ConnectToLoginServer()
{

	// ���õ�ǰ��״̬Ϊ������״̬
	SetStatus(LOGIN_DISCONNECT);

	// ֪ͨ������ʾϵͳ��ʾ��Ϣ, �������ӷ�����.
	
	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_login_Info_Connecting_Server");
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
	return 0;
}


// �õ���Ϸ�汾
int CGamePro_Login::GetGameVersion()
{
	return CURRENT_VERSION;
}






//-------------------------------------------------------------------------------------------------------------------------------
//
// �������
//

// ���ʺ��������
int CGamePro_Login::OpenCountInputDlg()
{
	// ���ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_COUNT_INPUT);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �رշ�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_SERVER);

	const PLOGINSERVER_INOF pLoginServerInfo = GetAreaLoginServerInfo(m_iCurSelAreaId,  m_iCurSelLoginServerId);
	if(pLoginServerInfo)
	{
		// ����ѡ��ķ�����
		CGameProcedure::s_pVariableSystem->SetVariable("Login_Area",	GetAreaName(m_iCurSelAreaId), FALSE);
		CGameProcedure::s_pVariableSystem->SetVariable("Login_Server",	pLoginServerInfo->szLoginServerName, FALSE);
		CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Provider", 0/*m_netProvider*/, FALSE);

		CGameProcedure::s_pVariableSystem->SetVariable("Login_ServerName", 	pLoginServerInfo->szLoginServerName, TRUE);
	}

	// �����ʺ��������
	//SetStatus(LOGIN_ACCOUNT_BEGIN_REQUESTING);

	return 0;
}

// �򿪷�����ѡ�����
int CGamePro_Login::OpenSelectServerDlg()
{
	// �򿪷�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_SERVER);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �ر��ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);

	// ���÷�����ѡ�����
	SetStatus(LOGIN_SELECT_SERVER);

	// �Ͽ���������
	CNetManager::GetMe()->Close();

	SelectOldServer();
	return 0;
}


// ������ѡ�����
int CGamePro_Login::OpenSelectCharacter()
{
	return 0;
}

// �л�������ѡ�����
int CGamePro_Login::ChangeToCharacterSel()
{
	// ����ѡ��ķ�����
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �ر��ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);

	// ��ʾ����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_CHARACTOR);

	// �����û���
	CGameProcedure::s_pProcCharSel->SetUserName(m_strUserName.c_str());

	// ���뵽����ѡ����棬 ���uiģ�͡�
	CGameProcedure::s_pProcCharSel->m_bClearUIModel = TRUE;


	// ���͵õ���ɫ��Ϣ��Ϣ.
	CLAskCharList msg;
	msg.SetAccount((CHAR*)m_strUserName.c_str());
	CNetManager::GetMe()->SendPacket(&msg);


	//�л�������ѡ�����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);

	return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------
//
// ����������Ϣ����
//

// ����ͬ����Ϣ
int CGamePro_Login::SendClConnectMsg()
{
	CLConnect msg;

	// ��������������Ϣ
	CNetManager::GetMe()->SendPacket(&msg);

	return 0;
}

// ����û���������.
int CGamePro_Login::CheckAccount(CHAR* szUserName, CHAR* szPassword)
{
	//616======* ����
	if( (CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1)
//^		 || (CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1)
	  )
	{
		// ת������ѡ�����
		ChangeToCharacterSel();
		return 0;
	}

	if(NULL == szUserName)
	{
		// ֪ͨ������ʾϵͳ��ʾ��Ϣ, �û�������
			
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Input_Name_err");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		return 0;
	}

	if(NULL == szPassword)
	{
		// ֪ͨ������ʾϵͳ��ʾ��Ϣ, �û�������
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
	// ��֤�û���
	//
	int iPos = (int)m_strUserName.find('\'');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Name_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�û������ܺ��� \" \' \" ");
		return 0;
	}

    iPos = (int)m_strUserName.find('"');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Name_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�û������ܺ��� \"  \"  \" ");
		return 0;
	}

	iPos = (int)m_strUserName.find('\\');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Name_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�û������ܺ��� \" \\ \" ");
		return 0;
	}//


	//// ���Ƿ��ַ������в��ҡ�
	//int iFindPos = -1;
	//char bufInfo[512];
	//for(int i = 0; i < (int)m_UnUseStrVector.size(); i++)
	//{
	//	iFindPos = (int)m_strUserName.find(m_UnUseStrVector[i]);
	//	if(-1 != iFindPos)
	//	{
	//		memset(bufInfo, 0, sizeof(bufInfo));
	//		sprintf(bufInfo, "�û��������Ƿ�����%s", m_UnUseStrVector[i]);
	//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, bufInfo);
	//		return 0;
	//	}

	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��֤����
	//
	iPos = (int)m_strPassword.find('\'');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Password_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���벻�ܺ��� \" \' \" ");
		return 0;
	}

    iPos = (int)m_strPassword.find('"');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Password_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���벻�ܺ��� \"  \"  \" ");
		return 0;
	}

	iPos = (int)m_strPassword.find('\\');
	if(-1 != iPos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("produce_login_Info_Password_Include_Gang");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���벻�ܺ��� \" \\ \" ");
		return 0;
	}//

	//// ���Ƿ��ַ������в���
	//for(int j = 0; j < (int)m_UnUseStrVector.size(); j++)
	//{
	//	iFindPos = (int)m_strPassword.find(m_UnUseStrVector[j]);
	//	if(-1 != iFindPos)
	//	{
	//		memset(bufInfo, 0, sizeof(bufInfo));
	//		sprintf(bufInfo, "��������Ƿ�����%s", m_UnUseStrVector[j]);
	//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, bufInfo);
	//		return 0;
	//	}
	//}

	// �����������socket ����.
	ConnectToLoginServer(m_iCurSelAreaId, m_iCurSelLoginServerId);
	return 0;
}

// ����check msg ��Ϣ
int CGamePro_Login::SendCheckAccountMsg()
{
	// ������֤������Ϣ
	CLAskLogin msg;
	msg.SetAccount((CHAR*)m_strUserName.c_str());
	msg.SetPassWord((CHAR*)m_strPassword.c_str());
	msg.SetVersion(GetGameVersion());
	CNetManager::GetMe()->SendPacket(&msg);//

	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_login_Info_Checking_Password");
	
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
	//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "������֤����....");
	return 0;
}


// 
// 
//
//------------------------------------------------------------------------------------------------------------
// �����ϴε�¼�ķ�����������
VOID CGamePro_Login::GetLastLoginServerID()
{
	STRING strTemp;

	m_nPrevServer = -1;
	m_nPrevArea = -1;

	//�����ϴ�ѡ��ķ�������
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

	//�����ϴ�ѡ��ķ�����
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