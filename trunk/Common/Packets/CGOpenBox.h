#ifndef _CGOPENBOX_H_
#define _CGOPENBOX_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGOpenBox : public Packet
	{
	public:
		CGOpenBox( void ) { };
		virtual ~CGOpenBox( void ) { };

	public:
		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;//�麯����const���ƿ���������
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_OPENBOX; }
		virtual UINT			GetPacketSize() const { return sizeof(UINT); }

	public:
		UINT					GetBoxID(void) { return m_BoxID; };
		VOID					SetBoxID(UINT BoxID) { m_BoxID = BoxID; };
	private:
		UINT		m_BoxID;			//���ӵ�id
		
	};

	class CGOpenBoxFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGOpenBox() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_OPENBOX; }
		UINT		GetPacketMaxSize() const { return sizeof(UINT); }
	};

	class CGOpenBoxHandler 
	{
	public:
		static UINT Execute( CGOpenBox* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif

