// CGExchangeReplyI.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __CGEXCHANGEREPLYI_H__
#define __CGEXCHANGEREPLYI_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGExchangeReplyI : public Packet
	{
	public:
		CGExchangeReplyI( )
		{
			m_objID = INVALID_ID;
		}
		virtual ~CGExchangeReplyI( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXCHANGEREPLYI; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t);}

	public:
		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

	private:
		ObjID_t					m_objID;		
	};

	class CGExchangeReplyIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGExchangeReplyI() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EXCHANGEREPLYI; };
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t);};
	};

	class CGExchangeReplyIHandler 
	{
	public:
		static UINT Execute( CGExchangeReplyI* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
