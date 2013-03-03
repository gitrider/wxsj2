// GCExchangeError.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGEERROR_H__
#define __GCEXCHANGEERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "ExchangeMsgDefine.h"

namespace Packets
{
	class GCExchangeError : public Packet
	{
	public:
		GCExchangeError( )
		{
			m_ID = 0;
		}
		virtual ~GCExchangeError( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGEERROR; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetID(VOID) const {return m_ID;};
		VOID					SetID(BYTE EID) {m_ID = EID;};

	private:
		BYTE					m_ID;		//�ɹ����
	};

	class GCExchangeErrorFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeError() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGEERROR; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class GCExchangeErrorHandler 
	{
	public:
		static UINT Execute( GCExchangeError* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
