/********************************************************************************
 *	文件名：	WGGuild.h
 *	全路径：	d:\Prj\Common\Packets\WGGuild.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 14 日	11:57
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef __WG_GUILD_H__
#define __WG_GUILD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGGuild : public Packet
	{
	public:
		WGGuild(){}
		virtual					~WGGuild(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_GUILD; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(PlayerID_t) + m_GuildPacket.GetPacketSize();
		}

	public:
		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		_GUILD_WGC_PACKET*		GetGuildPacket() { return &m_GuildPacket; }

	private:
		PlayerID_t				m_PlayerID;	//连接者
		_GUILD_WGC_PACKET		m_GuildPacket;

	};

	class WGGuildFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new WGGuild(); }
		PacketID_t				GetPacketID() const { return PACKET_WG_GUILD; }
		UINT					GetPacketMaxSize() const { return _GUILD_WGC_PACKET::GetPacketMaxSize(); }
	};

	class WGGuildHandler
	{
	public:
		static UINT Execute( WGGuild* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __WG_GUILD_H__
