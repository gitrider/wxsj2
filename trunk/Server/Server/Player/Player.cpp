
#include "stdafx.h"
#include "Player.h"
#include "Socket.h"
#include "PacketFactoryManager.h"
#include "TimeManager.h"
#include "Log.h"
#include "GCCharIdle.h"

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
		SaveCodeLog( ) ;
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
			Log::SaveLog( SERVER_ERRORFILE, "[%d] m_pSocketInputStream->Fill ret:%d %s", 
				g_pTimeManager->Time2DWORD(), (INT)ret, MySocketError() ) ;
			return FALSE ;
		}
	} 
	_MY_CATCH
	{
		SaveCodeLog( ) ;
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
		{//执行部分选项操作
		}

//每帧可以执行的消息数量上限
#define EXE_COUNT_PER_TICK 12
		for( INT i=0;i<EXE_COUNT_PER_TICK; i++ )
		{
			if( !m_pSocketInputStream->Peek(&header[0], PACKET_HEADER_SIZE) )
			{//数据不能填充消息头
				break ;
			}

			memcpy( &packetID, &header[0], sizeof(PacketID_t) ) ;	
			memcpy( &packetuint, &header[sizeof(PacketID_t)], sizeof(uint) );
			packetSize = GET_PACKET_LEN(packetuint) ;
			packetIndex = GET_PACKET_INDEX(packetuint) ;

			if( packetID >= (PacketID_t)PACKET_MAX )
			{//无效的消息类型
				return FALSE ;
			}

			_MY_TRY
			{

				if( m_pSocketInputStream->Length()<PACKET_HEADER_SIZE+packetSize )
				{//消息没有接收全
					break;
				}

				if( packetSize>g_pPacketFactoryManager->GetPacketMaxSize(packetID) )
				{
					//消息的大小出现异常，收到的消息比预定义消息的最大值还要大
					//抛弃当前消息，并且写日志
					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					CHAR ExpMsg[256] = {0};
					sprintf(ExpMsg,"消息大小异常,PacketID = %d",packetID);
					AssertEx(FALSE,ExpMsg);
					return FALSE ;
				}

				Packet* pPacket = g_pPacketFactoryManager->CreatePacket( packetID ) ;
				if( pPacket==NULL )
				{//不能分配到足够的内存
//					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return FALSE ;
				}

				//设置消息序列号
				pPacket->SetPacketIndex( packetIndex ) ;

				ret = m_pSocketInputStream->ReadPacket( pPacket ) ;
				if( ret==FALSE )
				{//读取消息内容错误
					g_pPacketFactoryManager->RemovePacket( pPacket ) ;
					return FALSE ;
				}

				BOOL bNeedRemove = TRUE ;
				BOOL bException = FALSE ;

				_MY_TRY
				{
					//修正m_KickTime信息，m_KickTime信息中的值为判断是否需要踢掉
					//客户端的依据
					ResetKick( ) ;

					uint uret ;
					_MY_TRY
					{
						uret = pPacket->Execute( this ) ;
					}
					_MY_CATCH
					{
						SaveCodeLog( ) ;
						uret = PACKET_EXE_ERROR ;
					}
					if( uret==PACKET_EXE_ERROR )
					{//出现异常错误，断开此玩家连接
						if( pPacket ) 
							g_pPacketFactoryManager->RemovePacket( pPacket ) ;
						return FALSE ;
					}
					else if( uret==PACKET_EXE_BREAK )
					{//当前消息的解析执行将停止
					 //直到下个循环时才继续对缓存中的数据进行消息格式
					 //化和执行。
					 //当需要将客户端的执行从一个场景转移到另外一个场景时：
					 //需要在发送转移消息后将执行在本线程中停止。
						if( pPacket ) 
							g_pPacketFactoryManager->RemovePacket( pPacket ) ;
						break ;
					}
					else if( uret==PACKET_EXE_CONTINUE )
					{//继续解析剩下的消息
					}
					else if( uret==PACKET_EXE_NOTREMOVE )
					{//继续解析剩下的消息，并且不回收当前消息
						bNeedRemove = FALSE ;
					}
					else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
					{
						return FALSE ;
					}
					else
					{//未知的返回值
					}
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;
					bException = TRUE ;
				}

				if( pPacket && bNeedRemove ) 
					g_pPacketFactoryManager->RemovePacket( pPacket ) ;

				if( bException )
					return FALSE ;
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
				return FALSE ;
			}
		}
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
		return FALSE ;
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
			Log::SaveLog( SERVER_ERRORFILE, "[%d] m_pSocketOutputStream->Flush ret:%d %s", 
				g_pTimeManager->Time2DWORD(), (INT)ret, MySocketError() ) ;
			return FALSE ;
		}
	} 
	_MY_CATCH
	{
		SaveCodeLog( ) ;
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

		UINT nSizeBefore = m_pSocketOutputStream->Length();
		BOOL ret = m_pSocketOutputStream->WritePacket( pPacket ) ;
		Assert( ret ) ;
		UINT nSizeAfter = m_pSocketOutputStream->Length();

		if(pPacket->GetPacketSize() != nSizeAfter-nSizeBefore-6)
		{
			g_pLog->FastSaveLog( LOG_FILE_6, "!!!!!!!PacketSizeError! ID=%d(Write%d,Should%d)", 
				pPacket->GetPacketID(), 
				nSizeAfter-nSizeBefore-6, pPacket->GetPacketSize());
		}

		if (pPacket->GetPacketID() == PACKET_GC_CHARIDLE )
		{
			g_pLog->FastSaveLog( LOG_FILE_6, "SendPacket! SOCKET=%d, ID=%d, X=%f, Z=%f", 
				m_pSocket->getSOCKET(), pPacket->GetPacketID(), 
				((GCCharIdle*)pPacket)->getWorldPos().m_fX,
				((GCCharIdle*)pPacket)->getWorldPos().m_fZ ) ;
		}
		else
		{
			//影响效率不写Log
			//g_pLog->FastSaveLog( LOG_FILE_6, "SendPacket! SOCKET=%d, ID=%d", 
			//m_pSocket->getSOCKET(), pPacket->GetPacketID() ) ;
		}

	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Player::HeartBeat( UINT uTime, INT nFlag )
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


