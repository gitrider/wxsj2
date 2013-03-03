#ifndef _WGTEAMRESULT_H_
#define _WGTEAMRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamResult:	public Packet
	{
		public:
			WGTeamResult()
			{
				m_SceneID = INVALID_ID; // �����ʼ����Server �յ�������ô�ֵ������ SceneResID
				m_NameSize = 0;
				memset( (void*)m_Name, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
			}
			virtual		~WGTeamResult(){};

			//���ýӿ�
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_WG_TEAMRESULT ; }
			virtual UINT			GetPacketSize()const { return	sizeof(PlayerID_t)+
																	sizeof(BYTE)+
																	sizeof(GUID_t)+
																	sizeof(TeamID_t)+
																	sizeof(GUID_t)+
																	sizeof(SceneID_t)+
																	sizeof(UCHAR)+
																	sizeof(CHAR) * m_NameSize+
																	sizeof(INT)+
																	sizeof(USHORT) +
																	sizeof(UINT) +
																	sizeof(INT); }

	public :
		VOID				SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t			GetPlayerID( ){ return m_PlayerID ; } ;

		VOID				SetReturn( BYTE Return ){ m_Return = Return ; }
		BYTE				GetReturn( ){ return m_Return ; }

		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

		VOID				SetTeamID( TeamID_t tid ){	m_TeamID = tid ; }
		TeamID_t			GetTeamID( ){ return m_TeamID ; }

		VOID				SetGUIDEx( GUID_t guid ){	m_GUIDEx = guid ; }
		GUID_t				GetGUIDEx( ){ return m_GUIDEx ; }

		VOID				SetSceneID( SceneID_t SceneID ){ m_SceneID = SceneID ; }
		SceneID_t			GetSceneID( ){ return m_SceneID ; }

		VOID				SetName( const CHAR* pName )
		{
			strncpy( m_Name, pName, MAX_CHARACTER_NAME-1 );
			m_NameSize = (UCHAR)strlen(m_Name);
		}
		const CHAR*			GetName( ){ return m_Name ; }

		VOID				SetIcon( INT icon ) { m_nPortrait = icon; }
		INT					GetIcon( ) { return m_nPortrait; }

		VOID				SetDataID(USHORT dataid) { m_uDataID = dataid; }
		USHORT				GetDataID() { return m_uDataID; }

		VOID				SetFamily(UINT uFamily) { m_uFamily = uFamily; }
		UINT				GetFamily() { return m_uFamily; }

		VOID				SetLevel( INT Level ) { m_Level = Level; }
		INT					GetLevel( ) { return m_Level; }

	public :
		PlayerID_t			m_PlayerID ;	//������
		BYTE				m_Return ;
		GUID_t				m_GUID ;
		TeamID_t			m_TeamID ;
		GUID_t				m_GUIDEx ;
		SceneID_t			m_SceneID ;
		UCHAR				m_NameSize;
		CHAR				m_Name[MAX_CHARACTER_NAME];			// ��Ա�����֣��ڶ�Ա��ӵ�ʱ����
		INT					m_nPortrait;						// ͷ��
		USHORT				m_uDataID;							// ��Ա���Ա�
		UINT				m_uFamily;							// 2.����
		INT					m_Level;							// �ȼ�

	};

	class WGTeamResultFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new WGTeamResult() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_WG_TEAMRESULT ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(PlayerID_t)+
																sizeof(BYTE)+
																sizeof(GUID_t)+
																sizeof(TeamID_t)+
																sizeof(GUID_t)+
																sizeof(SceneID_t)+
																sizeof(UCHAR)+
																sizeof(CHAR) * MAX_CHARACTER_NAME +
																sizeof(INT)+
																sizeof(USHORT) +
																sizeof(UINT) +
																sizeof(INT); }			
	};

	class WGTeamResultHandler
	{
		public:
			static UINT Execute(WGTeamResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif