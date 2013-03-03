// CGShopBuy.h
// 
// 客户端通知服务器买什么东西
// 
//////////////////////////////////////////////////////

#ifndef __CGSHOPBUY_H__
#define __CGSHOPBUY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGShopBuy : public Packet
	{
	public:
		CGShopBuy( )
		{
			m_nndex		= 0;
			m_UniqueID	= 0;
            m_uCount    = 0;
		};
		virtual ~CGShopBuy( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SHOPBUY ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+sizeof(UINT)*3;}

	public:
		BYTE					GetIndex(VOID) const {return m_nndex;};
		VOID					SetIndex(BYTE index) {m_nndex = index;};

		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }

        VOID					SetCount(UINT count) { m_uCount = count; }
        UINT					GetCount(void) { return m_uCount; }

        VOID					SetVersion(UINT uVersion) { m_uVersion = uVersion; }
        UINT					GetVersion(void) { return m_uVersion; }
	private:
		BYTE					m_nndex;		//位置
		UINT					m_UniqueID;		
        UINT                    m_uCount;
        UINT                    m_uVersion;     //版本号，校验用
	};

	class CGShopBuyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGShopBuy() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SHOPBUY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(UINT)*3;};
	};

	class CGShopBuyHandler 
	{
	public:
		static UINT Execute( CGShopBuy* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
