#include "StdAfx.h"
#include "NetManager.h"
#include <process.h>
#include "..\Procedure\GameProcedure.h"
#include "..\Procedure\GamePro_Login.h"
#include "..\Procedure\Gamepro_ChangeScene.h"
#include "..\Procedure\GamePro_Enter.h"
#include "CGHeartBeat.h"
#include "..\Global.h"
#include "GIException.h"
#include "GITimeSystem.h"
#include "..\Script\GMScriptSystem.h"
#include "LuaPlus.h"
#include "..\Event\GMEventSystem.h"
#include "GIUtil.h"
#include "GIException.h"
#include "..\World\WorldManager.h"


#define MAX_CONNECT_TIME	(10*100000)		// �����ʱ��10s

KL_IMPLEMENT_DYNAMIC(CNetManager, GETCLASS(tNode));
CNetManager* CNetManager::s_pMe = NULL;
extern char Error[_ESIZE] ;


CNetManager::CNetManager()
	: m_SocketInputStream(&m_Socket), 
	  m_SocketOutputStream(&m_Socket)
{
	s_pMe = this;
	m_hConnectThread = NULL;
}

CNetManager::~CNetManager()
{
}


VOID CNetManager::Initial(VOID*)
{
	// ��ʼ��WinSock
	WSADATA wsaData;
	if(0!=WSAStartup( MAKEWORD( 2, 2 ), &wsaData) || (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) )
	{
		WSACleanup();
		KLThrow(_T("Could not find a usable WinSock DLL!"));
		return;
	}

	m_hConnectThread = NULL;
	m_PacketFactoryManager.Init();
}


// Tick ��Ϸ��¼����.
VOID CNetManager::TickGameLoginProcedure()
{
	switch(CGameProcedure::s_pProcLogIn->GetStatus())
	{
	case CGamePro_Login::LOGIN_DEBUG_SETTING:
		{
			break;
		}

	case CGamePro_Login::LOGIN_SELECT_SERVER:
		{
			break;
		}
	
		// ��δ��¼,׼��״̬
	case CGamePro_Login::LOGIN_DISCONNECT:
		{
			break;
		}

		// SOCKET������...
	case CGamePro_Login::LOGIN_CONNECTING:
		{
			WaitConnecting();
			break;
		}

		// ����ʧ��
	case CGamePro_Login::LOGIN_CONNECT_FAILED:
		{
			break;
		}

	case CGamePro_Login::LOGIN_ACCOUNT_BEGIN_REQUESTING:
		{
			break;
		}
	// ���ӳɹ��������״̬,��ʼ����������Ϣ��
	case CGamePro_Login::LOGIN_CONNECTED_OK:
		{
			WaitPacket();
			//CGameProcedure::s_pProcLogIn->OpenCountInputDlg();
			//CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_REQUESTING);
			break;
		}
	case CGamePro_Login::LOGIN_ACCOUNT_REQUESTING:
		{
			WaitPacket();
			break;
		}
	default:
		{
			WaitPacket();
			break;
		}
	}
}



// Tick ����ѡ������
VOID	CNetManager::TickGameSelCharcterProcedure(VOID)
{
	//����������Ϣ��
	WaitPacket();
}

// Tick ���ﴴ������
VOID	CNetManager::TickGameCreateCharcterProcedure(VOID)
{
	//����������Ϣ��
	WaitPacket(); 
}

// Tick ����Ϸ����
VOID	CNetManager::TickGameMainProcedure(VOID)
{
	//����������Ϣ��
	WaitPacket();

	//��ʱ����������Ϣ
	SendHeartBeat();
}

// Tick ���ӵ�����������
VOID	CNetManager::TickChangeGameServerProcedure(VOID)
{
	switch(CGameProcedure::s_pProcChangeScene->GetStatus())
		{
		case CGamePro_ChangeScene::CHANGESCENE_DISCONNECT:
			{
			
				break;
			}
			//SOCKET������...
		case CGamePro_ChangeScene::CHANGESCENE_CONNECTING:
			{
				WaitConnecting();
				break;
			}

			//����ʧ��
		case CGamePro_ChangeScene::CHANGESCENE_CONNECT_SUCCESS:
			{
				break;
			}

		//���ӳɹ��������״̬,��ʼ����������Ϣ��
		case CGamePro_ChangeScene::CHANGESCENE_SENDING_CGCONNECT:
			{
				WaitPacket();
				break;
			}
		default:
			{
				break;
			}
		}
}


// Tick ��������
VOID	CNetManager::TickGameEnterProcedure(VOID)
{
	switch(CGameProcedure::s_pProcEnter->GetStatus())
		{
		case CGamePro_Enter::ENTERSCENE_READY:
			{
			
				break;
			}
			//SOCKET������...
		case CGamePro_Enter::ENTERSCENE_REQUESTING:
			{
				WaitPacket();
				break;
			}

			//����ʧ��
		case CGamePro_Enter::ENTERSCENE_OK:
			{
				break;
			}

		//���ӳɹ��������״̬,��ʼ����������Ϣ��
		case CGamePro_Enter::ENTERSCENE_FAILED:
			{
				
				break;
			}
		default:
			{
				break;
			}
		}
}


VOID CNetManager::Tick(VOID)
{
	//��ǰ�����ǵ�¼����
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcLogIn)
	{
		// Tick ��Ϸ��¼����.
		TickGameLoginProcedure();
		//SendHeartBeat();
		
	}
	//��������Ϸѭ����
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		// Tick ����Ϸ����
		TickGameMainProcedure();

	}
	// ��������ѡ������
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcCharSel)
	{
		// Tick ����ѡ��
		TickGameSelCharcterProcedure();
		//SendHeartBeat();
	}
	// �������ﴴ������
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcCharCreate)
	{
		// Tick ���ﴴ������
		TickGameCreateCharcterProcedure();
		//SendHeartBeat();
	}
	// tick game server ��������
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcChangeScene)
	{
		
		TickChangeGameServerProcedure();
		//SendHeartBeat();
	}
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcEnter)
	{
		
		// Tick ��������
		TickGameEnterProcedure();
		//SendHeartBeat();
	}

}

VOID CNetManager::WaitConnecting(VOID)
{

	//����¼�߳��Ƿ����
	int nExitCode = 0;
	
	if(::GetExitCodeThread(m_hConnectThread, (DWORD*)&nExitCode))
	{

	}
	else
	{
		//DWORD dwerr = ::GetLastError();
		//int kk = 0;//
	}

	//��¼�߳�δ����
	if( STILL_ACTIVE == nExitCode)
	{
		//����Ƿ�ʱ
		UINT dwTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
		UINT dwUsed =  CGameProcedure::s_pTimeSystem->CalSubTime(m_timeConnectBegin, dwTimeNow);
		//��ʱ
		if(dwUsed >= MAX_CONNECT_TIME)
		{
			//ǿ�ƽ�����¼�߳�
			TerminateThread(m_hConnectThread, 0);
			nExitCode = -3;
		}
		//�����ȴ�
		else 
		{
			return;
		}
	}

	//��¼�߳��Ѿ����� �رվ��
	if(CloseHandle(m_hConnectThread))
	{
		m_hConnectThread = NULL;
	}
	else
	{
		//DWORD dwerr = ::GetLastError();
		//int kk = 0;//

	}
	
	//��¼�����з�������
	if(nExitCode < 0)
	{
		//LPCTSTR szErrorDesc;
		switch(nExitCode)
		{
		case -1:
			{
				//szErrorDesc = _T("����SOCKET��������");	
				SetNetStatus(CONNECT_FAILED_CREATE_SOCKET_ERROR);

				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("netManager_Info_Create_Socket_err");
				CGameProcedure::s_pEventSystem->PushEvent(GE_NET_CLOSE, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent(GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent(GE_GAMELOGIN_SHOW_SYSTEM_INFO, "������������ʧ��!");
				break;
			}
		case -2:
			{
				//szErrorDesc = _T("Ŀ�ķ��������ܹر�");	
				SetNetStatus(CONNECT_FAILED_CONNECT_ERROR);

				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("netManager_Info_Server_Not_Work");
				CGameProcedure::s_pEventSystem->PushEvent(GE_NET_CLOSE, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent(GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent(GE_GAMELOGIN_SHOW_SYSTEM_INFO, "Ŀ�ķ��������ܹر�!");
				break;
			}
		case -3:
			{
				//szErrorDesc = _T("��ʱ");	
				SetNetStatus(CONNECT_FAILED_TIME_OUT);
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("netManager_Info_Server_Not_Work");
				CGameProcedure::s_pEventSystem->PushEvent(GE_NET_CLOSE, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent(GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent(GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���ӳ�ʱ!");
				break;
			}
		default:
			{
				//szErrorDesc = _T("δ֪����");	
				SetNetStatus(CONNECT_FAILED_CONNECT_ERROR);
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("netManager_Info_UNKnow_err");
				CGameProcedure::s_pEventSystem->PushEvent(GE_NET_CLOSE, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent(GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent(GE_GAMELOGIN_SHOW_SYSTEM_INFO, "δ֪����!");
				break;
			}

		}
	
		//TCHAR szTemp[MAX_PATH];
		//_sntprintf(szTemp, MAX_PATH, _T("�޷����ӵ�������,����ԭ��:%s\n�����˳�!"), szErrorDesc);
		//::MessageBox(NULL, szTemp, _T("=ERROR="), MB_OK|MB_ICONSTOP);
		////�˳�����
		//::PostQuitMessage(0);//

		this->Close();
		return;
	}

	//���ӳɹ�������Ϊ������ģʽ������Linger����
	if(!m_Socket.setNonBlocking() || !m_Socket.setLinger(0))
	{
		SetNetStatus(CONNECT_FAILED_CONNECT_ERROR);
		KLThrow(_T("(CNetManager::Tick)SetSocket Error"));
		return;
	}

	//֪ͨ��¼����,SOCKET���ӳɹ�
	
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcLogIn)
	{
		CGameProcedure::s_pProcLogIn->SendClConnectMsg();
		SetNetStatus(CONNECT_SUCESS);//
	}
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcChangeScene)
	{
		SetNetStatus(CONNECT_SUCESS);//
	}

	return;
}

VOID CNetManager::WaitPacket(VOID)
{
	if(!m_Socket.isValid())
	{
		return;
	}

	//��������������
	if(!Select() || !ProcessExcept() || !ProcessInput() || !ProcessOutput())
	{
		CEventSystem::GetMe()->PushEvent(GE_NET_CLOSE);
	}

	//Packet����
	ProcessCommands( ) ;
}

VOID CNetManager::Release(VOID)
{
	//���е�¼�߳�������,ǿ����ֹ
	if(m_hConnectThread) 
	{
		TerminateThread(m_hConnectThread, 0);
		CloseHandle(m_hConnectThread); m_hConnectThread = NULL;
	}

	m_Socket.close();

	WSACleanup();
}

VOID CNetManager::ConnectToServer(LPCTSTR szServerAddr, INT nServerPort)
{
	//�Ѿ��е�¼�߳���������
	if(m_hConnectThread) return;

	m_strServerAddr = szServerAddr;
	m_nServerPort	= nServerPort;

	//������¼�߳�
	UINT nThreadID;
	m_hConnectThread = (HANDLE)::_beginthreadex(NULL, 0, _ConnectThread, this, CREATE_SUSPENDED|THREAD_QUERY_INFORMATION, &nThreadID );
	if (m_hConnectThread == NULL)
	{
		KLThrow(_T("(CNetManager::ConnectToServer)Can't create connect thread!"));
	}

	//��ʼ���е�¼�߳�
	m_timeConnectBegin = CGameProcedure::s_pTimeSystem->GetTimeNow();
	ResumeThread(m_hConnectThread);
}

VOID CNetManager::SendPacket(Packet* pPacket)
{
	//����Ǵ��ڵȴ��л�����������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain &&
		CWorldManager::GetMe()->GetStation() == CWorldManager::WS_ASK_CHANGE_SCANE)
	{
		return;
	}

	if(m_Socket.isValid())
	{
		UINT nSizeBefore = m_SocketOutputStream.Length();
		m_SocketOutputStream.WritePacket( pPacket ) ;
		UINT nSizeAfter = m_SocketOutputStream.Length();

		if(pPacket->GetPacketSize() != nSizeAfter-nSizeBefore-6)
		{
			KLThrow("Packet size error[id=%d, stream=%d, size=%d]",
				pPacket->GetPacketID(), nSizeAfter-nSizeBefore-6, pPacket->GetPacketSize());
		}
	}
}

UINT CNetManager::_ConnectThread(LPVOID pParam)
{
	KLAssert(pParam);
	CNetManager* pNetManager = (CNetManager*)	pParam;

	return pNetManager->ConnectThread();
}

//�����̷߳���ֵ
// 0  : ��δ����
// 1  : �ɹ����ӵ�������
// -1 : ����SOCKET��������
// -2 : �޷����ӵ�Ŀ�ķ�����
// -3 : ��ʱ����
INT CNetManager::ConnectThread(VOID)
{
	//�ر�SOCKET
	m_Socket.close();
	//�����µ�SOCKET
	if(!m_Socket.create()) 
	{
		return -1;
	}

	//���ӵ�������
	if(!m_Socket.connect( m_strServerAddr.c_str(), m_nServerPort ))
	{
		m_Socket.close();
		return -2 ;
	}
	//�ɹ�����
	return 1;
}

BOOL CNetManager::Select(VOID)
{
	FD_ZERO( &m_ReadFD ) ;
	FD_ZERO( &m_WriteFD ) ;
	FD_ZERO( &m_ExceptFD ) ;

	FD_SET( m_Socket.getSOCKET(), &m_ReadFD ) ;
	FD_SET( m_Socket.getSOCKET(), &m_WriteFD ) ;
	FD_SET( m_Socket.getSOCKET(), &m_ExceptFD ) ;
	
	timeval	Timeout ;
	Timeout.tv_sec = 0 ;
	Timeout.tv_usec = 0 ;

	if(SOCKET_ERROR == SocketAPI::select_ex(0 , 
										&m_ReadFD , 
										&m_WriteFD , 
										&m_ExceptFD , 
										&Timeout ) )
	{
		return FALSE;
	}
	return true;
}

BOOL CNetManager::ProcessInput(VOID)
{
	if( FD_ISSET( m_Socket.getSOCKET(), &m_ReadFD ) )
	{
		uint ret = m_SocketInputStream.Fill( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			//KLThrow("m_SocketInputStream.Fill ret %d %s", (INT)ret, Error);
			m_Socket.close() ;
			return FALSE;
		}
	}
	return true;
}

BOOL CNetManager::ProcessOutput(VOID)
{
	//����Ǵ��ڵȴ��л�����������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain &&
		CWorldManager::GetMe()->GetStation() == CWorldManager::WS_ASK_CHANGE_SCANE)
	{
		return true;
	}

	if( FD_ISSET( m_Socket.getSOCKET(), &m_WriteFD ) )
	{
		uint ret = m_SocketOutputStream.Flush( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			//KLThrow("m_SocketInputStream.Fill ret %d %s", (INT)ret, Error);
			m_Socket.close() ;
			return FALSE;
		}
	}
	return true;
}

//������Ϣ����
VOID CNetManager::ProcessInputStream(SocketInputStream& inputStream)
{
	BOOL ret ;

	char header[PACKET_HEADER_SIZE];
	PacketID_t packetID;
	uint packetuint, packetSize, packetIndex;
	Packet* pPacket = NULL ;

	{

		for( ;; )
		{
			if( !m_SocketInputStream.Peek(&header[0], PACKET_HEADER_SIZE) )
			{//���ݲ��������Ϣͷ
				break ;
			}

			memcpy( &packetID, &header[0], sizeof(PacketID_t) ) ;	
			memcpy( &packetuint, &header[sizeof(PacketID_t)], sizeof(uint) );
			packetSize = GET_PACKET_LEN(packetuint) ;
			packetIndex = GET_PACKET_INDEX(packetuint) ;

			if( packetID >= (PacketID_t)PACKET_MAX )
			{//��Ч����Ϣ����
				KLThrow("Invalid Packet[%d]!", packetID);
				return ;
			}

			{
				if( m_SocketInputStream.Length()<PACKET_HEADER_SIZE+packetSize )
				{
					//��Ϣû�н���ȫ
					break;
				}

				if( packetSize>m_PacketFactoryManager.GetPacketMaxSize(packetID) )
				{
					//��Ϣ�Ĵ�С�����쳣���յ�����Ϣ��Ԥ������Ϣ�����ֵ��Ҫ��
					KLThrow("Packet size error![ID=%d](%d>%d)", 
						packetID, packetSize, m_PacketFactoryManager.GetPacketMaxSize(packetID));
//					m_SocketInputStream.Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return ;
				}

				Packet* pPacket = m_PacketFactoryManager.CreatePacket( packetID ) ;
				Assert( pPacket ) ;
				if( pPacket==NULL )
				{//���ܷ��䵽�㹻���ڴ�
					KLThrow("Create Packet error[%d]!", packetID);
//					m_SocketInputStream.Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return ;
				}

				//������Ϣ���к�
				pPacket->SetPacketIndex( packetIndex ) ;

				ret = m_SocketInputStream.ReadPacket( pPacket ) ;
				if( ret==FALSE )
				{
					//��ȡ��Ϣ���ݴ���
					KLThrow("Read Packet error!");
					continue ;
				}

				//��Ϣ��ִ��
				ret = executePacket_Gen_Exception(pPacket);

				if(ret != PACKET_EXE_NOTREMOVE)
				{
					m_PacketFactoryManager.RemovePacket( pPacket ) ;
				}
			}
		}
	}
}

UINT CNetManager::executePacket_Gen_Exception(Packet* pPacket)
{
	CHAR szTitle[MAX_PATH];
	_snprintf(szTitle, MAX_PATH, "Packet: %d", pPacket->GetPacketID());

	__try 
	{
		return executePacket_CPP_Exception(pPacket);
	}
	__except (tProcessInnerException(GetExceptionInformation(), g_hMainWnd, szTitle), EXCEPTION_EXECUTE_HANDLER) {}

	return PACKET_EXE_CONTINUE;
}

UINT CNetManager::executePacket_CPP_Exception(Packet* pPacket)
{
	try
	{
		return pPacket->Execute((Player*)this);
	}
	catch(const std::exception& e)
	{
		static STRING strCPPException;
		strCPPException = e.what();
		
		//ת��Ϊ�ṹ���쳣
		LPVOID pException = &strCPPException;
		::RaiseException(
			0XE000C0DE, 
			EXCEPTION_NONCONTINUABLE,				// ���ɼ��������ش���
			1,										// 1������
			(CONST ULONG_PTR *)&pException);		// ָ��̬�쳣�ڴ�
	}
	catch(...)
	{
		throw;
	}

	return PACKET_EXE_CONTINUE;
}

VOID CNetManager::ProcessCommands(VOID)
{
	ProcessInputStream(m_SocketInputStream);
}

BOOL CNetManager::ProcessExcept( )
{
	if( FD_ISSET( m_Socket.getSOCKET(), &m_ExceptFD ) )
	{
		m_Socket.close() ;
		return FALSE;
	}
	return true;
}


VOID CNetManager::SendHeartBeat(VOID)
{
	static UINT s_dwLastSendTime = 0;
	const UINT HEART_BEAT_RATE = 60*1000;		// 1 Minutes

	UINT dwTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	if(dwTimeNow - s_dwLastSendTime >= HEART_BEAT_RATE)
	{
		CGHeartBeat msg;
		this->SendPacket(&msg);

		s_dwLastSendTime = dwTimeNow;
	}
}

VOID CNetManager::Close(VOID)
{
	m_Socket.close();
}

// ��������״̬
VOID CNetManager::SetNetStatus(NETMANAGER_STATUS netStatus)
{
	
	//��ǰ�����ǵ�¼����
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcLogIn)
	{
		// Tick ��Ϸ��¼����.
		SetGameLoginStatus(netStatus);
		
	}
	//��������Ϸѭ����
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		// Tick ����Ϸ����
		SetGameMainStatus(netStatus);
	}
	
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcChangeScene)
	{
		CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_CONNECT_SUCCESS);
	}
}

// Tick ��Ϸ��¼����.
VOID	CNetManager::SetGameLoginStatus(NETMANAGER_STATUS netStatus)
{
	switch(netStatus)
	{
	case CONNECT_SUCESS:
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_CONNECTED_OK);//
			break;
		}
	case CONNECT_FAILED_CREATE_SOCKET_ERROR:
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_CONNECT_FAILED);
			break;
		}
	case CONNECT_FAILED_CONNECT_ERROR:
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_CONNECT_FAILED);
			break;
		}
	case CONNECT_FAILED_TIME_OUT:
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_CONNECT_FAILED);
			break;
		}
	default:
		{
			break;
		}
	}
}

// Tick ����Ϸ����
VOID	CNetManager::SetGameMainStatus(NETMANAGER_STATUS netStatus)
{
	switch(netStatus)
	{
	case CONNECT_SUCESS:
		{
			break;
		}
	case CONNECT_FAILED_CREATE_SOCKET_ERROR:
		{
			break;
		}
	case CONNECT_FAILED_CONNECT_ERROR:
		{
			break;
		}
	case CONNECT_FAILED_TIME_OUT:
		{
			break;
		}
	default:
		{
			break;
		}
	}
}
