#include "stdafx.h" 
#include "Player.h"
#include "Socket.h"
#include "PacketFactoryManager.h"
#include "TimeManager.h"
#include "Log.h"
#include "Config.h"
#include "ServerManager.h"

using namespace Packets ;

Player::Player( BOOL bIsServer )
{
__ENTER_FUNCTION

	m_PID = INVALID_ID ;
	m_UID = INVALID_ID ;
	m_PlayerManagerID = INVALID_ID ;


	m_pSocket = new Socket ;
	Assert( m_pSocket ) ;

	if( bIsServer )
	{
		m_pSocketInputStream = new SocketInputStream( m_pSocket,DEFAULTSOCKETINPUTBUFFERSIZE,64*1024*1024 ) ;
		Assert( m_pSocketInputStream ) ;

		m_pSocketOutputStream = new SocketOutputStream( m_pSocket,DEFAULTSOCKETOUTPUTBUFFERSIZE,64*1024*1024 ) ;
		Assert( m_pSocketOutputStream ) ;
	}
	else
	{
		m_pSocketInputStream = new SocketInputStream( m_pSocket ) ;
		Assert( m_pSocketInputStream ) ;

		m_pSocketOutputStream = new SocketOutputStream( m_pSocket ) ;
		Assert( m_pSocketOutputStream ) ;
	}

	m_IsEmpty		= TRUE ;
	m_IsDisconnect	= FALSE ;

	m_PacketIndex	 = 0 ;

__LEAVE_FUNCTION
}

Player::~Player( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pSocketInputStream ) ;
	SAFE_DELETE( m_pSocketOutputStream ) ;

	SAFE_DELETE( m_pSocket ) ;

__LEAVE_FUNCTION
}

VOID Player::CleanUp( )
{
__ENTER_FUNCTION

	m_pSocket->close() ;
	m_pSocketInputStream->CleanUp() ;
	m_pSocketOutputStream->CleanUp() ;
	SetPlayerManagerID( INVALID_ID ) ;
	SetUserID( INVALID_ID ) ;
	m_PacketIndex = 0 ;
	SetDisconnect(FALSE) ;


__LEAVE_FUNCTION
}

VOID Player::Disconnect( )
{
__ENTER_FUNCTION

	_MY_TRY
	{
		m_pSocket->close() ;
	}
	_MY_CATCH
	{
	}

__LEAVE_FUNCTION
}

BOOL Player::IsValid( )
{
__ENTER_FUNCTION

	if( m_pSocket==NULL ) return FALSE ;

	if( !m_pSocket->isValid() ) return FALSE ;


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Player::ProcessInput( )
{
__ENTER_FUNCTION

	if( IsDisconnect() )
		return TRUE ;

	_MY_TRY 
	{
		uint ret = m_pSocketInputStream->Fill( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "[%d] m_pSocketInputStream->Fill ret:%d %s", 
				g_pTimeManager->Time2DWORD(), (INT)ret, MySocketError() ) ;
			return FALSE ;
		}
	} 
	_MY_CATCH
	{
		return FALSE ;
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Player::ProcessCommand( BOOL Option )
{
__ENTER_FUNCTION

	BOOL ret ;

	CHAR header[PACKET_HEADER_SIZE];
	PacketID_t packetID;
	uint packetuint, packetSize, packetIndex;
	Packet* pPacket = NULL ;

	if( IsDisconnect( ) )
		return TRUE ;

	_MY_TRY
	{
		if( Option ) 
		{//ִ�в���ѡ�����
		}

		for( ;; )
		{
			if( !m_pSocketInputStream->Peek(&header[0], PACKET_HEADER_SIZE) )
			{//���ݲ��������Ϣͷ
				break ;
			}

			memcpy( &packetID, &header[0], sizeof(PacketID_t) ) ;	
			memcpy( &packetuint, &header[sizeof(PacketID_t)], sizeof(uint) );
			packetSize = GET_PACKET_LEN(packetuint) ;
			packetIndex = GET_PACKET_INDEX(packetuint) ;

			if( packetID >= (PacketID_t)PACKET_MAX )
			{//��Ч����Ϣ����
				Assert( FALSE ) ;
				return FALSE ;
			}

			_MY_TRY
			{

				if( m_pSocketInputStream->Length()<PACKET_HEADER_SIZE+packetSize )
				{//��Ϣû�н���ȫ
					break;
				}

				if( packetSize>g_pPacketFactoryManager->GetPacketMaxSize(packetID) )
				{//��Ϣ�Ĵ�С�����쳣���յ�����Ϣ��Ԥ������Ϣ�����ֵ��Ҫ��
					Assert( FALSE ) ;
//					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return FALSE ;
				}

				Packet* pPacket = g_pPacketFactoryManager->CreatePacket( packetID ) ;
				if( pPacket==NULL )
				{//���ܷ��䵽�㹻���ڴ�
					Assert( FALSE ) ;
//					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return FALSE ;
				}

				//������Ϣ���к�
				pPacket->SetPacketIndex( packetIndex ) ;

				ret = m_pSocketInputStream->ReadPacket( pPacket ) ;
				if( ret==FALSE )
				{//��ȡ��Ϣ���ݴ���
					Assert( FALSE ) ;
					g_pPacketFactoryManager->RemovePacket( pPacket ) ;
					return FALSE ;
				}

				BOOL bNeedRemove = TRUE ;

				_MY_TRY
				{
					//����m_KickTime��Ϣ��m_KickTime��Ϣ�е�ֵΪ�ж��Ƿ���Ҫ�ߵ�
					//�ͻ��˵�����
					ResetKick( ) ;

					uint uret = pPacket->Execute( this ) ;
					if( uret==PACKET_EXE_ERROR )
					{//�����쳣���󣬶Ͽ����������
						if( pPacket ) 
							g_pPacketFactoryManager->RemovePacket( pPacket ) ;
						return FALSE ;
					}
					else if( uret==PACKET_EXE_BREAK )
					{//��ǰ��Ϣ�Ľ���ִ�н�ֹͣ
					 //ֱ���¸�ѭ��ʱ�ż����Ի����е����ݽ�����Ϣ��ʽ
					 //����ִ�С�
					 //����Ҫ���ͻ��˵�ִ�д�һ������ת�Ƶ�����һ������ʱ��
					 //��Ҫ�ڷ���ת����Ϣ��ִ���ڱ��߳���ֹͣ��
						if( pPacket ) 
							g_pPacketFactoryManager->RemovePacket( pPacket ) ;
						break ;
					}
					else if( uret==PACKET_EXE_CONTINUE )
					{//��������ʣ�µ���Ϣ
					}
					else if( uret==PACKET_EXE_NOTREMOVE )
					{//��������ʣ�µ���Ϣ�����Ҳ����յ�ǰ��Ϣ
						bNeedRemove = FALSE ;
					}
					else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
					{
						return FALSE ;
					}
					else
					{//δ֪�ķ���ֵ
						Assert(FALSE) ;
					}
				}
				_MY_CATCH
				{
				}

				if( pPacket && bNeedRemove ) 
					g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			}
			_MY_CATCH
			{
				return FALSE;
			}
		}
	}
	_MY_CATCH
	{
	}



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Player::ProcessOutput( )
{
__ENTER_FUNCTION

	if( IsDisconnect( ) )
		return TRUE ;

	_MY_TRY
	{
		uint size = m_pSocketOutputStream->Length() ;
		if( size==0 )
		{
			return TRUE ;
		}

		uint ret = m_pSocketOutputStream->Flush( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "[%d] m_pSocketOutputStream->Flush ret:%d %s", 
				g_pTimeManager->Time2DWORD(), (INT)ret, MySocketError() ) ;
			return FALSE ;
		}
	} 
	_MY_CATCH
	{
		return FALSE ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}


//#define _DEBUG_JINCHAO
#ifdef _DEBUG_JINCHAO
#include "Packet.h"
//==========================================
// Add by jinchao begin
VOID MyTraceOut( Socket* pSocket, const Packet* pPacket )
{
	HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO oldInfo;
	GetConsoleScreenBufferInfo(hStdOut, &oldInfo);
	SetConsoleTextAttribute(hStdOut, FOREGROUND_RED|FOREGROUND_INTENSITY);


	//--- time
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	printf("[%d] %02d:%02d %02d.%03d   ", (INT)(pSocket->getSOCKET()),
		timeNow.wHour, timeNow.wMinute, timeNow.wSecond, timeNow.wMilliseconds);

	//----------------------
	switch(pPacket->GetPacketID())
	{
	case PACKET_GC_CONNECT:
		printf("PACKET_GC_CONNECT\n");
		break;
		
	case PACKET_GC_ENTERSCENE:
		printf("PACKET_GC_ENTERSCENE\n");
		break;
		
	case PACKET_GC_LEAVESCENE:
		printf("PACKET_GC_LEAVESCENE\n");
		break;
		
	case PACKET_GC_NEWPLAYER:
		printf("PACKET_GC_NEWPLAYER\n");
		break;
		
	case PACKET_GC_NEWMONSTER:
		printf("PACKET_GC_NEWMONSTER\n");
		break;
		
	case PACKET_GC_NEWITEM:
		printf("PACKET_GC_NEWITEM\n");
		break;
		
	case PACKET_GC_PLAYERREALMOVE:
		printf("PACKET_GC_PLAYERREALMOVE\n");
		break;
		
	case PACKET_GC_MONSTERREALMOVE:
		printf("PACKET_GC_MONSTERREALMOVE\n");
		break;
		
	case PACKET_GC_MYEQUIPMENT:
		printf("PACKET_GC_MYEQUIPMENT\n");
		break;
		
	case PACKET_GC_OTHEREQUIPMENT:
		printf("PACKET_GC_OTHEREQUIPMENT\n");
		break;
		
	case PACKET_GC_TEST:
		printf("PACKET_GC_TEST\n");
		break;
		
	case PACKET_GC_MOVE:
		printf("PACKET_GC_MOVE\n");
		break;
		
	case PACKET_GC_ARRIVE:
		printf("PACKET_GC_ARRIVE\n");
		break;
		
	case PACKET_GC_ATTACK:
		printf("PACKET_GC_ATTACK\n");
		break;
		
	case PACKET_GC_OTHERATTACK:
		printf("PACKET_GC_OTHERATTACK\n");
		break;
		
	case PACKET_GC_ERRORATTACK:
		printf("PACKET_GC_ERRORATTACK\n");
		break;
		
	case PACKET_GC_SKILL:
		printf("PACKET_GC_SKILL\n");
		break;
		
	case PACKET_GC_OTHERSKILL:
		printf("PACKET_GC_OTHERSKILL\n");
		break;
		
	case PACKET_GC_ERRORSKILL:
		printf("PACKET_GC_ERRORSKILL\n");
		break;

	default:
		printf("[%d]UNKNOWN!!!", pPacket->GetPacketID());
		break;
	}
	//----------------------



	SetConsoleTextAttribute(hStdOut, oldInfo.wAttributes);
}
// Add by jinchao end
//==========================================
#endif

BOOL Player::SendPacket( Packet* pPacket )
{
__ENTER_FUNCTION

	if( IsDisconnect( ) )
		return TRUE ;

	if( m_pSocketOutputStream!=NULL )
	{
		pPacket->SetPacketIndex( m_PacketIndex++ ) ;

		BOOL ret = m_pSocketOutputStream->WritePacket( pPacket ) ;
		Assert( ret ) ;
		//Log::SaveLog( "./Log/LoginDebug.log", "SendPacket! SOCKET=%d, ID=%d", 
		//	m_pSocket->getSOCKET(), pPacket->GetPacketID() ) ;
		

#ifdef _DEBUG_JINCHAO
//=========================================
// Add by jinchao begin
	MyTraceOut(m_pSocket, pPacket);
// Add by jinchao end
//=========================================
#endif	
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Player::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID Player::ResetKick( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}
INT Player::WhereThisPlayerFrom(VOID) const
{
	if(NULL!=m_pSocket)
	{
		//CHAR szLog[1024];
		
		//tsnprintf(szLog, sizeof(szLog), 
		//	"[Player::WhereThisPlayerFrom]: PlayerIP: %s, ProxyCncIP: %s, ProxyCtcIP: %s, ProxyEduIP: %s!!", 
		//	m_pSocket->m_Host,
		//	g_Config.m_InternalIpofProxy.m_szProxyForCncUser[0],
		//	g_Config.m_InternalIpofProxy.m_szProxyForCtcUser[0],
		//	g_Config.m_InternalIpofProxy.m_szProxyForEduUser[0]
		//	);
		//szLog[sizeof(szLog)-1]='\0';
		//DiskLog( URGENCY_LOG, szLog);
		//return g_Config.m_InternalIpofProxy.WhereThisIpFrom(m_pSocket->m_Host);
	}
	return 0;//INVALID_ISP;
}
CHAR* ProxyTool_GetCorrespondingIP(Player const* pPlayer, ID_t ServerID)
{
	__ENTER_FUNCTION
	_SERVER_DATA* pData = g_pServerManager->FindServerInfo( ServerID );
	if(NULL==pData)
	{
		CHAR szLog[1024];
		tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingIP] Error: Can't found the specific server(%d).", ServerID);
		szLog[sizeof(szLog)-1] = '\0';
		AssertEx(pData, szLog);
	}
	INT nIsp = pPlayer->WhereThisPlayerFrom();

	//CHAR szLog[1024];
	//tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingIP]: Player is From %s!!", g_szIspID[nIsp]);
	//szLog[sizeof(szLog)-1]='\0';
	//DiskLog( URGENCY_LOG, szLog);

	if(INVALID_ISP==nIsp)
	{
		//CHAR szLog[1024];
		//tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingIP]: INVALID_ISP, return original ip!!");
		//szLog[sizeof(szLog)-1]='\0';
		//DiskLog( URGENCY_LOG, szLog);
		return pData->m_IP0;
	}
	else
	{
		if(TRUE==RangeCheckForIndex_Assert(nIsp, 0, NUM_OF_ISP-1, "ProxyTool_GetCorrespondingIP"))
		{
			PROXY_DATA& rProxy = pData->m_aProxy[nIsp];
			if(TRUE==rProxy.m_bEnabled)
			{
				//CHAR szLog[1024];
				//tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingIP]: return proxy ip: %s!!", rProxy.m_szIP);
				//szLog[sizeof(szLog)-1]='\0';
				//DiskLog( URGENCY_LOG, szLog);
				return rProxy.m_szIP;
			}
			//CHAR szLog[1024];
			//tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingIP]: Proxy is not enabled, return original ip!!");
			//szLog[sizeof(szLog)-1]='\0';
			//DiskLog( URGENCY_LOG, szLog);
			return pData->m_IP0;
		}
		else
		{
			//CHAR szLog[1024];
			//tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingIP]: Invalid Isp ID, return original ip!!");
			//szLog[sizeof(szLog)-1]='\0';
			//DiskLog( URGENCY_LOG, szLog);
			return pData->m_IP0;
		}
	}
	__LEAVE_FUNCTION
	return NULL;
}
UINT  ProxyTool_GetCorrespondingPort(Player const* pPlayer, ID_t ServerID)
{
	__ENTER_FUNCTION
	_SERVER_DATA* pData = g_pServerManager->FindServerInfo( ServerID );
	if(NULL==pData)
	{
		CHAR szLog[1024];
		tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetCorrespondingPort] Error: Can't found the specific server(%d).", ServerID);
		szLog[sizeof(szLog)-1] = '\0';
		AssertEx(pData, szLog);
	}
	INT nIsp = pPlayer->WhereThisPlayerFrom();
	if(INVALID_ISP==nIsp)
	{
		return pData->m_Port0;
	}
	else
	{
		if(TRUE==RangeCheckForIndex_Assert(nIsp, 0, NUM_OF_ISP-1, "ProxyTool_GetCorrespondingIP"))
		{
			PROXY_DATA& rProxy = pData->m_aProxy[nIsp];
			if(TRUE==rProxy.m_bEnabled)
			{
				return rProxy.m_nPort;
			}
			return pData->m_Port0;
		}
		else
		{
			return pData->m_Port0;
		}
	}
	__LEAVE_FUNCTION
	return NULL;
}
