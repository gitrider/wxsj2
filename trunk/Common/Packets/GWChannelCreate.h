

#ifndef __GWCHANNELCREATE_H__
#define __GWCHANNELCREATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWChannelCreate : public Packet 
	{
	public:
		GWChannelCreate( ){} ;
		virtual ~GWChannelCreate( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_CHANNELCREATE ; }
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


	class GWChannelCreateFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWChannelCreate() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_CHANNELCREATE ; }
		UINT		GetPacketMaxSize()const { return sizeof(GUID_t) ; }
	};


	class GWChannelCreateHandler 
	{
	public:
		static UINT Execute( GWChannelCreate* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
