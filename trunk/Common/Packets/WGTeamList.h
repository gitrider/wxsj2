/********************************************************************************
 *	�ļ�����	WGTeamList.h
 *	ȫ·����	d:\Prj\Common\Packets\WGTeamList.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 1 �� 15 ��	15:34
 *
 *	����˵����	���Ͷ�Ա�б�����������л������������Լ������������
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _WG_TEAM_LIST_H_
#define _WG_TEAM_LIST_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamList:	public Packet
	{
	public:
		WGTeamList() { m_nMemberCount = 0; }
		virtual ~WGTeamList() {}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_TEAM_LIST; }
		virtual UINT			GetPacketSize() const
		{
			UINT				uSize;

			uSize				= sizeof(PlayerID_t) + sizeof(TeamID_t) + sizeof(UCHAR);

			for( UCHAR i=0; i<m_nMemberCount; ++i)
			{
				uSize			+= m_Members[i].GetSize();
			}

			return uSize;
		}

	public :
		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetTeamID( TeamID_t tid ) { m_TeamID = tid; }
		TeamID_t				GetTeamID( ) { return m_TeamID; }

		VOID					AddTeamMember( const TEAM_LIST_ENTRY& entry )
		{
			Assert( m_nMemberCount<MAX_TEAM_MEMBER );
			m_Members[m_nMemberCount++] = entry;
		}
		const TEAM_LIST_ENTRY&	GetTeamMember( UCHAR i )
		{
			Assert( i<m_nMemberCount );
			return m_Members[i];
		}

		UCHAR					GetMemberCount( ) { return m_nMemberCount; }

	public :
		PlayerID_t				m_PlayerID;				// ������
		TeamID_t				m_TeamID;				// �����
		UCHAR					m_nMemberCount;			// �м�����Ա
		TEAM_LIST_ENTRY			m_Members[MAX_TEAM_MEMBER];
	};

	class WGTeamListFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new WGTeamList(); }
		PacketID_t	GetPacketID()const { return PACKET_WG_TEAM_LIST; }
		UINT		GetPacketMaxSize()const { return sizeof(PlayerID_t)
													+ sizeof(TeamID_t)
													+ sizeof(UCHAR)
													+ sizeof(TEAM_LIST_ENTRY) * MAX_TEAM_MEMBER; }
	};

	class WGTeamListHandler
	{
	public:
		static UINT Execute(WGTeamList* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _WG_TEAM_LIST_H_
