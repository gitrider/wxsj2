/********************************************************************************
 *	文件名：	CGTopList.h

 *	创建人：	王勇鸷
 *	创建时间：	2008 年 4 月 10 日	
 *
 *	功能说明：	客户端往服务器端发送的请求排行榜消息
 *	修改记录：
*********************************************************************************/

#ifndef __CG_TOPLIST_H__
#define __CG_TOPLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTopList : public Packet
	{
	public:
		CGTopList(){}
		virtual					~CGTopList(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_TOP_LIST; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_Type);
		}

		VOID					SetType(UCHAR type) { m_Type = type; }
		UCHAR					GetType() { return m_Type; }

	private:
		//请求的排行榜类型
		UCHAR					m_Type;					

	};

	class CGTopListFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGTopList(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_TOP_LIST; }
		UINT					GetPacketMaxSize() const { return sizeof(UCHAR); }
	};

	class CGTopListHandler
	{
	public:
		static UINT Execute( CGTopList* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __CG_GUILD_H__
