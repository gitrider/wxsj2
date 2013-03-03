

#ifndef __WGNOTIFYUSER_H__
#define __WGNOTIFYUSER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class WGNotifyUser : public Packet 
	{
	public:
	WGNotifyUser( ){} ;
	virtual ~WGNotifyUser( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_WG_NOTIFYUSER ; }
		virtual UINT			GetPacketSize() const 
		{
			if( m_Status == NUS_CANNOTSAY )
			{
				return	sizeof(GUID_t) +
					sizeof(INT)+
					sizeof(INT); 
			}
			else
			{
				return	sizeof(GUID_t) +
					sizeof(INT) ; 
			}
		}

	public :
	enum NOTIFY_USER_STATUS
	{
			NUS_DISCONNECT = 0 ,		
		NUS_REMOVE ,
			NUS_NEED_SERVER_KICK ,		// 废弃[12/21/2006]
			NUS_WORLD_KICK_REQUEST,
			NUS_CANNOTSAY,//禁言

		NUS_NUMBER ,
	};

	public:
	//使用数据接口
	GUID_t					GetGUID( ){ return m_GUID ; } ;
	VOID					SetGUID( GUID_t guid ){ m_GUID = guid ; } ;

	VOID					SetStatus( INT status ){ m_Status = status ; } ;
	INT						GetStatus( ){ return m_Status ; } ;

		VOID					SetTime( INT nTime ){ m_Time = nTime ; } ;
		INT						GetTime( ){ return m_Time ; } ;

	private:
	//数据
	GUID_t					m_GUID ;		//玩家的GUID
	INT						m_Status ;		//当前连接的状态
		INT						m_Time ;

	};


	class WGNotifyUserFactory : public PacketFactory 
	{
	public:
	Packet*		CreatePacket() { return new WGNotifyUser() ; }
	PacketID_t	GetPacketID() const { return PACKET_WG_NOTIFYUSER ; }
	UINT		GetPacketMaxSize() const { return	sizeof(GUID_t)+
			sizeof(INT)+
			sizeof(INT); }
	};


	class WGNotifyUserHandler 
	{
	public:
	static UINT Execute( WGNotifyUser* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
