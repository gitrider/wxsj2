// GCExchangeEstablishI.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGEESTABLISHI_H__
#define __GCEXCHANGEESTABLISHI_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCExchangeEstablishI : public Packet
	{
	public:
		GCExchangeEstablishI( )
		{
			m_objID = INVALID_ID;
		}
		virtual ~GCExchangeEstablishI( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGEESTABLISHI; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t);}

	public:
		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

	private:
		ObjID_t					m_objID;		//�ɹ����
	};

	class GCExchangeEstablishIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeEstablishI() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGEESTABLISHI; };
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t);};
	};

	class GCExchangeEstablishIHandler 
	{
	public:
		static UINT Execute( GCExchangeEstablishI* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
