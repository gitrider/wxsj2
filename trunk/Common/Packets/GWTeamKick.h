#ifndef _GWTEAMKICK_H_
#define _GWTEAMKICK_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamKick:	public Packet
	{
		public:
			GWTeamKick(){};
			virtual		~GWTeamKick(){};

			//���ýӿ�
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMKICK ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t)*2 ; }


	public :
		//���ݽӿ�
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestGUID( GUID_t guid ){	m_DestGUID = guid ; }
		GUID_t			GetDestGUID( ){ return m_DestGUID ; }


	private :
		//����
		GUID_t			m_SourGUID ;		//������
		GUID_t			m_DestGUID ;		//������


	};

	class GWTeamKickFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamKick() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAMKICK ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2; }			
	};

	class GWTeamKickHandler
	{
		public:
			static UINT Execute(GWTeamKick* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif