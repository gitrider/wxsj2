#include "stdafx.h"
#include "Player.h"
#include "Socket.h"
#include "PacketFactoryManager.h"
#include "TimeManager.h"
#include "Log.h"

using namespace Packets ;

Player::Player( )
{
__ENTER_FUNCTION

	m_PID = INVALID_ID ;
	m_UID = INVALID_ID ;
	m_PlayerManagerID = INVALID_ID ;


	m_pSocket = new Socket ;
	Assert( m_pSocket ) ;

	m_pSocketInputStream = new SocketInputStream( m_pSocket ) ;
	Assert( m_pSocketInputStream ) ;

	m_pSocketOutputStream = new SocketOutputStream( m_pSocket ) ;
	Assert( m_pSocketOutputStream ) ;

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

void Player::CleanUp( )
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

void Player::Disconnect( )
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
		if( (int)ret <= SOCKET_ERROR )
		{
			g_pLog->SaveLog(BILLING_LOGFILE, "T=[%.4f] m_pSocketInputStream->Fill ret:%d %s", 
				(FLOAT)(g_pTimeManager->RunTime())/1000.0, (int)ret, MySocketError() ) ;
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

BOOL Player::ProcessCommand( bool Option )
{
__ENTER_FUNCTION

	BOOL ret ;

	char header[PACKET_HEADER_SIZE];
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
					else if( uret==PACKET_EXE_NOTREMOVE_ERROR )
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
//		else if( size < MAX_SEND_SIZE )
//		{//�����е�����С��һ������ʱ������ÿ�ζ���������
//			if( m_CurrentTime < m_LastSendTime+MAX_SEND_TIME )
//			{//�ж���һ�η�������������ʱ���Ƿ񳬹�һ��ʱ�䣬����������򲻷�������
//				return TRUE ;
//			}
//		}
//		m_LastSendTime = m_CurrentTime ;

		uint ret = m_pSocketOutputStream->Flush( ) ;
		if( (int)ret <= SOCKET_ERROR )
		{
			g_pLog->SaveLog(BILLING_LOGFILE, "[%d] m_pSocketOutputStream->Flush ret:%d %s", 
				g_pTimeManager->Time2DWORD(), (int)ret, MySocketError() ) ;
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
void MyTraceOut( Socket* pSocket, const Packet* pPacket )
{
	HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO oldInfo;
	GetConsoleScreenBufferInfo(hStdOut, &oldInfo);
	SetConsoleTextAttribute(hStdOut, FOREGROUND_RED|FOREGROUND_INTENSITY);


	//--- time
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	printf("[%d] %02d:%02d %02d.%03d   ", (int)(pSocket->getSOCKET()),
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

		g_pLog->SaveLog(BILLING_LOGFILE, "SendPacket! ID=%d", pPacket->GetPacketID() ) ;

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

BOOL Player::HeartBeat( DWORD dwTime )
{
__ENTER_FUNCTION



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

void Player::ResetKick( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}


