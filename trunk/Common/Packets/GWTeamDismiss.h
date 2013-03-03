#ifndef _GWTEAMDISMISS_H_
#define _GWTEAMDISMISS_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamDismiss:	public Packet
	{
		public:
			GWTeamDismiss(){};
			virtual		~GWTeamDismiss(){};

			//���ýӿ�
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMDISMISS ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :
		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

	private :
		GUID_t				m_GUID ;	//��ɢ�������

	};

	class GWTeamDismissFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamDismiss() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAMDISMISS ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t); }			
	};

	class GWTeamDismissHandler
	{
		public:
			static UINT Execute(GWTeamDismiss* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif