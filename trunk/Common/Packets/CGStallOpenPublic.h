// CGStallOpenPublic.h 
// 
// ���̯λ����
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLOPENPUBLIC_H__
#define __CGSTALLOPENPUBLIC_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallOpenPublic : public Packet
	{
	public:
		CGStallOpenPublic( ){};		
		virtual ~CGStallOpenPublic( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLOPENPUBLIC; }
		virtual UINT			GetPacketSize()const { return	sizeof(BOOL);}

	public:
		BOOL					GetIsOpen(VOID) const {return m_bOpenPublic;};
		VOID					SetIsOpen(BOOL bOpenPublic) {m_bOpenPublic = bOpenPublic;};

	private:
		BOOL					m_bOpenPublic;		// �Ƿ��� true ���У�false ͣ��

	};

	class CGStallOpenPublicFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallOpenPublic() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLOPENPUBLIC; };
		UINT		GetPacketMaxSize()const { return sizeof(BOOL);}
	};

	class CGStallOpenPublicHandler 
	{
	public:
		static UINT Execute( CGStallOpenPublic* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
