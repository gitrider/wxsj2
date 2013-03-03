/********************************************************************************
 *	�ļ�����	WGTeamFollowList.h
 *	ȫ·����	d:\Prj\Common\Packets\WGTeamFollowList.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	23:41
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#ifndef _WG_TEAMFOLLOWLIST_H_
#define _WG_TEAMFOLLOWLIST_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamFollowList:	public Packet
	{
	public:
		WGTeamFollowList(){ m_Count = 0; }
		virtual		~WGTeamFollowList(){};

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_TEAMFOLLOW_LIST; }
		virtual UINT			GetPacketSize() const { return sizeof(PlayerID_t) +
																sizeof(UCHAR) +
																m_Count * sizeof(GUID_t); }

	public :
		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

   		VOID					AddFollowMember( GUID_t guid )
		{
			Assert( m_Count<=MAX_TEAM_MEMBER );
			m_GUIDs[m_Count++] = guid;
		}
		GUID_t					GetFollowMember( UCHAR i )
		{
			Assert( i<m_Count );
			return m_GUIDs[i];
		}

		UCHAR					GetMemberCount( ) { return m_Count; }
	public :
		PlayerID_t				m_PlayerID ;	//������
		UCHAR					m_Count;
		GUID_t					m_GUIDs[MAX_TEAM_MEMBER];
	};

	class WGTeamFollowListFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new WGTeamFollowList(); }
		PacketID_t	GetPacketID() const			{ return PACKET_WG_TEAMFOLLOW_LIST; }
		UINT		GetPacketMaxSize() const	{ return sizeof(PlayerID_t) +
															sizeof(UCHAR) +
															MAX_TEAM_MEMBER * sizeof(GUID_t); }
	};

	class WGTeamFollowListHandler
	{
	public:
		static UINT Execute(WGTeamFollowList* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _WG_TEAMFOLLOWLIST_H_
