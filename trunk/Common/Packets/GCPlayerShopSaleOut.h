// GCPlayerShopSaleOut.h
// 
// �ظ��ͻ����̵��Ѿ����̳�
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPSALEOUT_H__
#define __GCPLAYERSHOPSALEOUT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"

namespace Packets
{
	class GCPlayerShopSaleOut : public Packet
	{
	public:
		GCPlayerShopSaleOut( )
		{
			m_bSaleOut	= FALSE;
			m_Serial	=	0;
		}
		virtual ~GCPlayerShopSaleOut( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPSALEOUT; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
																sizeof(BYTE);}

	public:
		VOID					SetSaleOut( BYTE bSaleOut ){ m_bSaleOut = bSaleOut; } ;
		BYTE					GetSaleOut( ){ return m_bSaleOut; } ;

		VOID					SetSerial( BYTE nSerial ){ m_Serial = nSerial; } ;
		BYTE					GetSerial( ){ return m_Serial; } ;

	private:
		BYTE					m_bSaleOut;
		BYTE					m_Serial;

	};

	class GCPlayerShopSaleOutFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopSaleOut() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPSALEOUT; };
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE)+
														sizeof(BYTE);}
	};

	class GCPlayerShopSaleOutHandler 
	{
	public:
		static UINT Execute( GCPlayerShopSaleOut* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
