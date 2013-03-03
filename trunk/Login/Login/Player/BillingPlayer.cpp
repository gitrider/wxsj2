#include "stdafx.h" 
#include "Log.h"
#include "BillingPlayer.h"
#include "PacketFactoryManager.h"
#include "SocketBillingInputStream.h"
#include "SocketBillingOutputStream.h"
#include "TurnPlayerQueue.h"
#include "LBKeepLive.h"
#include "LBAccCheck.h"

using namespace Packets;
#define NORMAL_BILLING_PACKET_HEAD	0x55AA

BillingPlayer::BillingPlayer( BOOL bIsServer ) 
{
	__ENTER_FUNCTION

		m_Status = 0 ;


	m_PID = INVALID_ID ;
	m_UID = INVALID_ID ;
	m_PlayerManagerID = INVALID_ID ;
	m_pSocket = new Socket ;
	Assert( m_pSocket ) ;

	m_pSocketInputStream = new SocketBillingInputStream( m_pSocket ) ;
	Assert( m_pSocketInputStream ) ;

	m_pSocketOutputStream = new SocketBillingOutputStream( m_pSocket ) ;
	Assert( m_pSocketOutputStream ) ;

	m_IsEmpty		= TRUE ;
	m_IsDisconnect	= FALSE ;
	m_PacketIndex	 = 0 ;
	m_Status = PS_BILLING_EMPTY;
	m_LastKeepLiveTime = 0;
	m_KeepLiveSendNum = 0;

	__LEAVE_FUNCTION
}

BillingPlayer::~BillingPlayer( )
{
	__ENTER_FUNCTION

		__LEAVE_FUNCTION
}

BOOL BillingPlayer::ProcessCommand( BOOL Option )
{
	__ENTER_FUNCTION

		BOOL ret ;

	CHAR header[BILLING_PACKET_HEADER_SIZE+sizeof(SHORT)];

	BPacketID_t PacketID;
	USHORT	PacketHeadFlag,PacketSize,PacketIndex;

	BillPacket* pPacket = NULL ;

	if( IsDisconnect( ) ) //连接状态检查
	{
		//g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand Is Disconnect!!!");
		return TRUE ;
	}

	//g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand Begin!!! Length=%u",m_pSocketInputStream->Length());
	_MY_TRY
	{
		if( Option ) 
		{//执行部分选项操作
		}

		for( ;; )
		{
			if( !m_pSocketInputStream->Peek(&header[0], BILLING_PACKET_HEADER_SIZE+sizeof(SHORT)) )
			{//数据不能填充消息头
				break ;
			}

			memcpy(&PacketHeadFlag,&header[0],sizeof(USHORT));
			if(NORMAL_BILLING_PACKET_HEAD != PacketHeadFlag)
			{//消息包包头错误，一般是上一个Billing发回来的数据没有被全部接收走
				//需要检查上一个消息包接收是不是有问题，一般发生修改某个协议的包长时出现，或连接的是旧版本Billing
				g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand Invalid PacketHeadFlag!!!...Wrong Billing or Bad Packet!!!");
				Assert( FALSE ) ;
				return FALSE ;
			}
			memcpy(&PacketSize,&header[sizeof(USHORT)],sizeof(PacketSize));
			PacketSize = ntohs(PacketSize);
			memcpy(&PacketID,&header[sizeof(USHORT)+sizeof(USHORT)],sizeof(BPacketID_t));
			memcpy(&PacketIndex,&header[sizeof(USHORT)+sizeof(USHORT)+sizeof(BPacketID_t)],sizeof(USHORT));
			PacketIndex = ntohs(PacketIndex);

			if( PacketID >= (BPacketID_t)BPACKET_MAX )
			{//无效的消息类型
				Assert( FALSE ) ;
				return FALSE ;
			}

			_MY_TRY
			{

				if( m_pSocketInputStream->Length()<BILLING_PACKET_HEADER_SIZE+BILLING_PACKET_TAIL_SIZE+PacketSize-1)
				{//消息没有接收全
					//g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand Not Complete Packet!!!...PacketID=0x%X PacketSize=%u",
					//	PacketID,PacketSize);
					break;
				}

				if( (PacketSize-sizeof(PacketIndex)-sizeof(PacketID))>g_pBPacketFactoryManager->GetPacketMaxSize(PacketID) )
				{//消息的大小出现异常，收到的消息比预定义消息的最大值还要大
					g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand Invalid Max Packet!!!...PacketID=0x%X PacketSize=%u MaxSize=%u",
						PacketID,PacketSize,g_pBPacketFactoryManager->GetPacketMaxSize(PacketID));
					Assert( FALSE ) ;
					//m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return FALSE ;
				}

				//g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand PacketID=0x%X Begin",PacketID);

				BillPacket* pPacket = g_pBPacketFactoryManager->CreatePacket( PacketID ) ;
				if( pPacket==NULL )
				{//不能分配到足够的内存
					g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand Not Enough Memory!!! PacketID=0x%X",PacketID);
					Assert( FALSE ) ;
					//					m_pSocketInputStream->Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return FALSE ;
				}

				//设置消息序列号
				pPacket->SetPacketIndex( PacketIndex ) ;

				ret = ((SocketBillingInputStream*)m_pSocketInputStream)->ReadPacket( pPacket ) ;
				if( ret==FALSE )
				{//读取消息内容错误
					g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand ReadPacket Error!!! PacketID=0x%X",PacketID);
					Assert( FALSE ) ;
					g_pBPacketFactoryManager->RemovePacket( pPacket ) ;
					return FALSE ;
				}

				BOOL bNeedRemove = TRUE ;

				_MY_TRY
				{
					//修正m_KickTime信息，m_KickTime信息中的值为判断是否需要踢掉
					//客户端的依据
					ResetKick( ) ;

					uint uret = pPacket->Execute( this ) ;
					if( uret==PACKET_EXE_ERROR )
					{//出现异常错误，断开此玩家连接
						g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand PACKET_EXE_ERROR!!! PacketID=0x%X",PacketID);
						if( pPacket ) 
							g_pBPacketFactoryManager->RemovePacket( pPacket ) ;
						return FALSE ;
					}
					else if( uret==PACKET_EXE_BREAK )
					{//当前消息的解析执行将停止
						//直到下个循环时才继续对缓存中的数据进行消息格式
						//化和执行。
						//当需要将客户端的执行从一个场景转移到另外一个场景时：
						//需要在发送转移消息后将执行在本线程中停止。
						//g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand PACKET_EXE_BREAK!!! PacketID=0x%X",PacketID);
						if( pPacket ) 
							g_pBPacketFactoryManager->RemovePacket( pPacket ) ;
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
						g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand PACKET_EXE_NOTREMOVE_ERROR!!! PacketID=0x%X",PacketID);
						return FALSE ;
					}
					else
					{//未知的返回值
						g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand Unknown uret=%u!!! PacketID=0x%X",uret,PacketID);
						Assert(FALSE) ;
					}

					//g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand PacketID=0x%X End",PacketID);
				}
				_MY_CATCH
				{
				}
				if( pPacket && bNeedRemove ) 
					g_pBPacketFactoryManager->RemovePacket( pPacket ) ;
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

	//g_pLog->FastSaveLog(LOG_FILE_0, "BillingPlayer::ProcessCommand End!!!");
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BillingPlayer::ProcessInput( )
{
	__ENTER_FUNCTION

		return Player::ProcessInput( ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BillingPlayer::ProcessOutput( )
{
	__ENTER_FUNCTION

		return Player::ProcessOutput( ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BillingPlayer::SendPacket( BillPacket* pPacket )
{
	__ENTER_FUNCTION

		if( IsDisconnect( ) )
			return TRUE ;

	if( m_pSocketOutputStream!=NULL )
	{
		//pPacket->SetPacketIndex( 0 ) ;
		BOOL ret = ((SocketBillingOutputStream*)m_pSocketOutputStream)->WritePacket( pPacket ) ;
		Assert( ret ) ;
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BillingPlayer::IsValid( )
{
	__ENTER_FUNCTION

		if( !Player::IsValid( ) )
			return FALSE ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

VOID BillingPlayer::CleanUp( )
{
	__ENTER_FUNCTION

		m_Status = PS_BILLING_EMPTY ;

	m_LastKeepLiveTime = 0;
	m_KeepLiveSendNum = 0;

	Player::CleanUp( ) ;

	__LEAVE_FUNCTION
}

#define KEEPLIVETIME 30000				//心跳包间隔
#define KEEPLIVE_SEND_MAX_NUM	4		//未收到心跳包的次数

BOOL BillingPlayer::HeartBeat( UINT uTime )
{
	__ENTER_FUNCTION

		switch(GetPlayerStatus())
	{
		case PS_BILLING_CONNECT:
			{
				//重发次数过多,断开Billing的连接
				if(m_KeepLiveSendNum > KEEPLIVE_SEND_MAX_NUM)	return FALSE;
				//定时发送心跳包
				if(((uTime-m_LastKeepLiveTime)>KEEPLIVETIME)||((INT)(m_LastKeepLiveTime-uTime)>KEEPLIVETIME))
				{
					LBKeepLive 	Msg;
					Msg.SetZoneID(g_Config.m_WorldInfo.m_ZoneID);
					Msg.SetWorldID(g_Config.m_WorldInfo.m_WorldID);
					Msg.SetPlayerCount(g_WorldPlayerCounter.m_WorldPlayerCount);
					SendPacket(&Msg);
					m_LastKeepLiveTime = uTime;
					m_KeepLiveSendNum++;
				}
			}
			break;
		default:
			break;
	}
	return Player::HeartBeat(uTime ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}