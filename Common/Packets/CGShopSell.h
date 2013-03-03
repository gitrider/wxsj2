// CGShopSell.h
// 
// 客户端通知服务器要卖什么东西
// 
//////////////////////////////////////////////////////

#ifndef __CGSHOPSELL_H__
#define __CGSHOPSELL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGShopSell : public Packet
	{
	public:
		CGShopSell( )
		{
			m_nBagIndex =	0;
			m_UniqueID	=	0;
            m_uCount    =   0;
		};
		virtual ~CGShopSell( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SHOPSELL ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+sizeof(UINT)*2;}

    public:
		BYTE					GetBagIndex(VOID) const {return m_nBagIndex;};
		VOID					SetBagIndex(BYTE nNumber) {m_nBagIndex = nNumber;};

		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }

        VOID					SetCount(UINT count) { m_uCount = count; }
        UINT					GetCount(void) { return m_uCount; }

	private:
		BYTE					m_nBagIndex;	//格子索引
		UINT					m_UniqueID;
        UINT                    m_uCount;       //卖出数量
	};

	class CGShopSellFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGShopSell() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SHOPSELL; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(UINT)*2;};
	};

	class CGShopSellHandler 
	{
	public:
		static UINT Execute( CGShopSell* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
