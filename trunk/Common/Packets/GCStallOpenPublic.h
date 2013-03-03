// GCStallOpenPublic.h
// 
// ���߿ͻ����յ���������
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLOPENPUBLIC_H__
#define __GCSTALLOPENPUBLIC_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallOpenPublic : public Packet
	{
	public:
		GCStallOpenPublic( )
		{
			m_bOpenPublic = FALSE;
		}
		virtual ~GCStallOpenPublic( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLOPENPUBLIC; }
		virtual UINT			GetPacketSize()const { return	sizeof(BOOL);}

	public:
		BOOL					GetIsOpen(VOID) const {return m_bOpenPublic;};
		VOID					SetIsOpen(BOOL bOpenPublic) {m_bOpenPublic = bOpenPublic;};

	private:
		BOOL					m_bOpenPublic;		// �Ƿ���
	};

	class  GCStallOpenPublicFactory: public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallOpenPublic() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLOPENPUBLIC; };
		UINT		GetPacketMaxSize()const { return sizeof(BOOL);};
	};

	class GCStallOpenPublicHandler 
	{
	public:
		static UINT Execute( GCStallOpenPublic* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
