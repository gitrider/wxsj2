#ifndef _GWTEAMAPPOINT_H_
#define _GWTEAMAPPOINT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamAppoint:	public Packet
	{
		public:
			GWTeamAppoint(){};
			virtual		~GWTeamAppoint(){};

			//���ýӿ�
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMAPPOINT ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t)*2 ; }

	public :
		//���ݽӿ�
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestGUID( GUID_t guid ){	m_DestGUID = guid ; }
		GUID_t			GetDestGUID( ){ return m_DestGUID ; }


	public :
		//����
		GUID_t			m_SourGUID ;		//ԭ�ӳ�
		GUID_t			m_DestGUID ;		//�¶ӳ�

	};

	class GWTeamAppointFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamAppoint() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAMAPPOINT ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2; }			
	};

	class GWTeamAppointHandler
	{
		public:
			static UINT Execute(GWTeamAppoint* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif