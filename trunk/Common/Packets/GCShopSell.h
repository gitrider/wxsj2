// GCShopSell.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPSELL_H__
#define __GCSHOPSELL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopSell: public Packet
	{
	public:
		enum
		{
			SELL_OK = 0,		// �����ɹ�
			SELL_FAIL,		// ����ʧ��
		};
	public:
		GCShopSell( )
		{
			m_IsSellOk = SELL_OK;
		};
		virtual ~GCShopSell( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPSELL ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetSellOk(VOID) const {return m_IsSellOk;};
		VOID					SetSellOk(BYTE SellOk) {m_IsSellOk = SellOk;};

	private:
		BYTE					m_IsSellOk;		//�ɹ����
	};

	class GCShopSellFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopSell() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPSELL; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class GCShopSellHandler 
	{
	public:
		static UINT Execute( GCShopSell* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
