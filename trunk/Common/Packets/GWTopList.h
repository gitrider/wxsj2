/********************************************************************************
 *	文件名：	GWTopList.h
 *	创建人：	王勇鸷
 *	创建时间：	2008 年 4 月 10 日
 *
 *	功能说明：	服务器端往 World 发送的排行榜相关的操作请求
 *	修改记录：
*********************************************************************************/

#ifndef __GW_TOPLIST_H__
#define __GW_TOPLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTopList : public Packet
	{
	public:
		GWTopList(){}
		virtual					~GWTopList(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_TOP_LIST; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_GUID) + sizeof(m_Type);
		}

	public:
		VOID					SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t					GetGUID() { return m_GUID; }

		VOID					SetType(UCHAR type) { m_Type = type; }
		UCHAR					GetType() { return m_Type; }

	private:
		GUID_t					m_GUID;
		UCHAR					m_Type;

	};

	class GWTopListFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GWTopList(); }
		PacketID_t				GetPacketID() const { return PACKET_GW_TOP_LIST; }
		UINT					GetPacketMaxSize() const { return sizeof(GUID_t) + sizeof(UCHAR); }
	};

	class GWTopListHandler
	{
	public:
		static UINT Execute( GWTopList* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __GW_GUILD_H__
