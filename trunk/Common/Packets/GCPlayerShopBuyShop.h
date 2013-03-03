// GCPlayerShopBuyShop.h
// 
// ֪ͨ���������̵��Ѿ����̳�
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPBUYSHOP_H__
#define __GCPLAYERSHOPBUYSHOP_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPlayerShopBuyShop : public Packet
	{
	public:
		GCPlayerShopBuyShop( ){};
		virtual ~GCPlayerShopBuyShop( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPBUYSHOP; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID);}
	public:
		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}
	private:
		_PLAYERSHOP_GUID		m_ShopID;			//�̵�ID
	};

	class GCPlayerShopBuyShopFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopBuyShop() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPBUYSHOP; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID);};
	};

	class GCPlayerShopBuyShopHandler 
	{
	public:
		static UINT Execute( GCPlayerShopBuyShop* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
