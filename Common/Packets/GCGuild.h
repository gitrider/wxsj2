/********************************************************************************
 *	文件名：	GCGuild.h
 *	全路径：	d:\Prj\Common\Packets\GCGuild.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 14 日	11:59
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef __GC_GUILD_H__
#define __GC_GUILD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCGuild : public Packet
	{
	public:
		GCGuild(){}
		virtual					~GCGuild(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_GUILD; }

		virtual UINT			GetPacketSize() const
		{
			return m_GuildPacket.GetPacketSize();
		}

	public:
		VOID					SetGuildPacket(const _GUILD_WGC_PACKET* pGuildPacket)
		{
			memcpy((void*)&m_GuildPacket, pGuildPacket, sizeof(_GUILD_WGC_PACKET));
		}
		_GUILD_WGC_PACKET*		GetGuildPacket() { return &m_GuildPacket; }

	private:
		_GUILD_WGC_PACKET		m_GuildPacket;

	};

	class GCGuildFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GCGuild(); }
		PacketID_t				GetPacketID() const { return PACKET_GC_GUILD; }
		UINT					GetPacketMaxSize() const { return _GUILD_WGC_PACKET::GetPacketMaxSize(); }
	};

	class GCGuildHandler
	{
	public:
		static UINT Execute( GCGuild* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __GC_GUILD_H__
