#ifndef _WGTEAMASKAPPLY_H_
#define _WGTEAMASKAPPLY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamAskApply:	public Packet
	{
	public:
		WGTeamAskApply()
		{
			m_SourNameSize = 0;
			m_DestNameSize = 0;

			memset( (void*)m_SourName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
			memset( (void*)m_DestName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
		}

		virtual		~WGTeamAskApply(){};

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_TEAMASKAPPLY; }
		virtual UINT			GetPacketSize() const { return sizeof(GUID_t)*2+
															sizeof(PlayerID_t)*2+
															sizeof(UCHAR)*2+
															sizeof(CHAR)*(m_SourNameSize+m_DestNameSize)+
															sizeof(UINT)+
															sizeof(SceneID_t)+
															sizeof(INT)+
															sizeof(USHORT); }

	public :

		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetSourPlayerID( PlayerID_t pid ) { m_SourPlayerID = pid; }
		PlayerID_t				GetSourPlayerID( ) { return m_SourPlayerID; }

		VOID					SetSourGUID( GUID_t guid ) { m_SourGUID = guid; }
		GUID_t					GetSourGUID( ) { return m_SourGUID; }

		VOID					SetDestGUID( GUID_t guid ){ m_DestGUID = guid; }
		GUID_t					GetDestGUID( ) { return m_DestGUID; }

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

		VOID					SetFamily( UINT family ) { m_uFamily = family; }
		UINT					GetFamily( ) { return m_uFamily; }

		VOID					SetScene( SceneID_t id ){ m_Scene = id; }
		SceneID_t				GetScene( ){ return m_Scene; }

		VOID					SetLevel(INT lvl) { m_Level = lvl; }
		INT						GetLevel() { return m_Level; }

		VOID					SetSex( USHORT uSex ) { m_uDataID = uSex; }
		INT						GetSex( ) { return m_uDataID; }

	public :
		PlayerID_t				m_PlayerID;		//������
		PlayerID_t				m_SourPlayerID; // ������ PlayerID
		GUID_t					m_SourGUID;		//������
		GUID_t					m_DestGUID;		//��������
		UCHAR					m_SourNameSize;
		UCHAR					m_DestNameSize;
		CHAR					m_SourName[MAX_CHARACTER_NAME];			// 1.����������
		CHAR					m_DestName[MAX_CHARACTER_NAME];			// ������������
		UINT					m_uFamily;								// 2.����
		SceneID_t				m_Scene;								// 3.����
		INT						m_Level;								// 4.�ȼ�
		USHORT					m_uDataID;								// 5.����Ա�
	};

	class WGTeamAskApplyFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new WGTeamAskApply(); }
			PacketID_t	GetPacketID()const			{ return PACKET_WG_TEAMASKAPPLY; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2+
															sizeof(PlayerID_t)*2+
															sizeof(UCHAR)*2+
															sizeof(CHAR)*MAX_CHARACTER_NAME*2+
															sizeof(UINT)+
															sizeof(SceneID_t)+
															sizeof(INT)+
															sizeof(USHORT); }			
	};

	class WGTeamAskApplyHandler
	{
		public:
			static UINT Execute(WGTeamAskApply* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif