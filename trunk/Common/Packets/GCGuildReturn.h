/********************************************************************************
 *	文件名：	GCGuildReturn.h
 *	全路径：	d:\Prj\Common\Packets\GCGuildReturn.h
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 12 日	17:34
 *
 *	功能说明：	帮会操作成功的返回值
 *	修改记录：
*********************************************************************************/

#ifndef __GC_GUILDRETURN_H__
#define __GC_GUILDRETURN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCGuildReturn:		public Packet
	{
	public:
		GCGuildReturn(){}
		virtual					~GCGuildReturn(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_GUILDRETURN; }
		virtual UINT			GetPacketSize() const { return m_GuildReturn.GetPacketSize(); }

	public :
		VOID					SetGuildReturn( _GUILD_RETURN* pGuildReturn )
		{
			memcpy((void*)&m_GuildReturn, pGuildReturn, sizeof(m_GuildReturn));
		}

		_GUILD_RETURN*			GetGuildReturn() { return &m_GuildReturn; }

	private :
		_GUILD_RETURN			m_GuildReturn;

	};

	class GCGuildReturnFactory:	public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GCGuildReturn(); }
		PacketID_t				GetPacketID() const { return PACKET_GC_GUILDRETURN; }
		UINT					GetPacketMaxSize() const { return sizeof(_GUILD_RETURN); }
	};

	class GCGuildReturnHandler
	{
	public:
		static UINT				Execute(GCGuildReturn* pPacket, Player* pPlayer);
	};

}

using namespace Packets;

#endif // __GC_GUILDRETURN_H__
