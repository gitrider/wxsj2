#ifndef _WGTEAMLEADERASKINVITE_H_
#define _WGTEAMLEADERASKINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamLeaderAskInvite:	public Packet
	{
	public:
		WGTeamLeaderAskInvite()
		{
			m_SourNameSize = 0;
			m_DestNameSize = 0;

			memset( (void*)m_SourName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
			memset( (void*)m_DestName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
		}
		virtual		~WGTeamLeaderAskInvite(){};

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_TEAMLEADERASKINVITE; }
		virtual UINT			GetPacketSize()const { return	sizeof(PlayerID_t)+
																sizeof(GUID_t)*2+
																sizeof(UCHAR)*2+
																sizeof(CHAR)*(m_SourNameSize+m_DestNameSize); }

	public :
		VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ){ return m_PlayerID; }

		VOID					SetSourGUID( GUID_t guid ){	m_SourGUID = guid; }
		GUID_t					GetSourGUID( ){ return m_SourGUID; }

		VOID					SetDestGUID( GUID_t guid ){	m_DestGUID = guid; }
		GUID_t					GetDestGUID( ){ return m_DestGUID; }

		VOID					SetSourName( const CHAR* pName )
		{
			strncpy( m_SourName, pName, MAX_CHARACTER_NAME-1 );
			m_SourNameSize = (UCHAR)strlen(m_SourName);
		}
		const CHAR*				GetSourName( ){ return m_SourName; }

		VOID					SetDestName( const CHAR* pName )
		{
			strncpy( m_DestName, pName, MAX_CHARACTER_NAME-1 );
			m_DestNameSize = (UCHAR)strlen(m_DestName);
		}
		const CHAR*				GetDestName( ){ return m_DestName; }

	public :
		PlayerID_t				m_PlayerID;	//������
		GUID_t					m_SourGUID;		//������
		GUID_t					m_DestGUID;		//��������
		CHAR					m_SourName[MAX_CHARACTER_NAME];			//����������
		CHAR					m_DestName[MAX_CHARACTER_NAME];			//������������
		UCHAR					m_SourNameSize;
		UCHAR					m_DestNameSize;
	};

	class WGTeamLeaderAskInviteFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new WGTeamLeaderAskInvite(); }
			PacketID_t	GetPacketID()const			{ return PACKET_WG_TEAMLEADERASKINVITE; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(PlayerID_t)+
																sizeof(GUID_t)*2+
																sizeof(UCHAR)*2+
																MAX_CHARACTER_NAME*2; }			
	};

	class WGTeamLeaderAskInviteHandler
	{
		public:
			static UINT Execute(WGTeamLeaderAskInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif