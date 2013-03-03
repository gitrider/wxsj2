

#ifndef __CGCHANNELCREATE_H__
#define __CGCHANNELCREATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGChannelCreate : public Packet 
	{
	public:
		CGChannelCreate( ){} ;
		virtual ~CGChannelCreate( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHANNELCREATE ; }
		virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :

	public:
		//ʹ�����ݽӿ�
		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

	private:
		//����
		GUID_t				m_GUID ;

	};


	class CGChannelCreateFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGChannelCreate() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHANNELCREATE ; }
		UINT		GetPacketMaxSize()const { return sizeof(GUID_t) ; }
	};


	class CGChannelCreateHandler 
	{
	public:
		static UINT Execute( CGChannelCreate* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
