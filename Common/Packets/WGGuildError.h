/********************************************************************************
 *	文件名：	WGGuildError.h
 *	全路径：	d:\Prj\Common\Packets\WGGuildError.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 12 日	9:10
 *
 *	功能说明：	帮会相关的错误信息
 *	修改记录：
*********************************************************************************/

#ifndef __WG_GUILDERROR_H__
#define __WG_GUILDERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGGuildError:			public Packet
	{
	public:
		WGGuildError(){}
		virtual					~WGGuildError(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_GUILDERROR; }
		virtual UINT			GetPacketSize() const { return sizeof(m_PlayerID) + sizeof(m_Error); }

	public :
		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetErrorCode( CHAR Error ){ m_Error = Error; }
		CHAR					GetErrorCode( ){ return m_Error; }

	private :
		PlayerID_t				m_PlayerID;	//连接者
		CHAR					m_Error;

	};

	class WGGuildErrorFactory:	public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new WGGuildError(); }
		PacketID_t				GetPacketID() const { return PACKET_WG_GUILDERROR; }
		UINT					GetPacketMaxSize() const { return sizeof(PlayerID_t) + sizeof(CHAR); }
	};

	class WGGuildErrorHandler
	{
	public:
		static UINT				Execute(WGGuildError* pPacket, Player* pPlayer);
	};

}

using namespace Packets;

#endif // __WG_GUILDERROR_H__
