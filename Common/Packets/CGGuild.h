/********************************************************************************
 *	文件名：	CGGuild.h
 *	全路径：	d:\Prj\Common\Packets\CGGuild.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 11 日	13:07
 *
 *	功能说明：	客户端往服务器端发送的帮会相关的操作请求
 *	修改记录：
*********************************************************************************/

#ifndef __CG_GUILD_H__
#define __CG_GUILD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGGuild : public Packet
	{
	public:
		CGGuild(){}
		virtual					~CGGuild(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_GUILD; }

		virtual UINT			GetPacketSize() const
		{
			return m_GuildPacket.GetPacketSize();
		}

	public:
		_GUILD_CGW_PACKET*		GetGuildPacket() { return &m_GuildPacket; }

	private:
		_GUILD_CGW_PACKET		m_GuildPacket;

	};

	class CGGuildFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGGuild(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_GUILD; }
		UINT					GetPacketMaxSize() const { return _GUILD_CGW_PACKET::GetPacketMaxSize(); }
	};

	class CGGuildHandler
	{
	public:
		static UINT Execute( CGGuild* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __CG_GUILD_H__
