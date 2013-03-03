/********************************************************************************
 *	�ļ�����	WGRetTeamRecruitInfo.h
 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 5 �� 14 ��	15:34
 *
 *	����˵����	���������������б�ʱ�����ص���Ϣ
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _WG_RET_TEAM_RECRUIT_INFO_H_
#define _WG_RET_TEAM_RECRUIT_INFO_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Team.h"

namespace Packets
{
	class WGRetTeamRecruitInfo:	public Packet
	{
	public:
		WGRetTeamRecruitInfo() { m_nMemberCount = 0; }
		virtual ~WGRetTeamRecruitInfo() {}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_RET_TEAM_RECRUIT; }
		virtual UINT			GetPacketSize() const
		{
			UINT				uSize;

			uSize				= sizeof(UCHAR) + sizeof(PlayerID_t);

			for( UCHAR i=0; i<m_nMemberCount; ++i)
			{
				uSize			+= m_MemberInfo[i].GetSize();
			}

			return uSize;
		}

	public :

		VOID					AddMember( RETURN_RECRUIT_INFO& entry )
		{
			Assert( m_nMemberCount<ONCE_RECRUIT_LIST_LEN );
			memcpy(&m_MemberInfo[m_nMemberCount++],&entry,sizeof(RETURN_RECRUIT_INFO));
		}
		RETURN_RECRUIT_INFO*	GetMember( UCHAR i )
		{
			Assert( i<m_nMemberCount );
			return &m_MemberInfo[i];
		}

		UCHAR					GetMemberCount( ) { return m_nMemberCount; }

		VOID				SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t			GetPlayerID( ){ return m_PlayerID ; } ;

	public :
		PlayerID_t				m_PlayerID ;	//������
		UCHAR					m_nMemberCount;
		RETURN_RECRUIT_INFO		m_MemberInfo[ONCE_RECRUIT_LIST_LEN];
	};

	class WGRetTeamRecruitInfoFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new WGRetTeamRecruitInfo(); }
		PacketID_t	GetPacketID()const { return PACKET_WG_RET_TEAM_RECRUIT; }
		UINT		GetPacketMaxSize()const 
		{
			return sizeof(UCHAR) + sizeof(PlayerID_t) + 
				   sizeof(RETURN_RECRUIT_INFO) * ONCE_RECRUIT_LIST_LEN; }
	};

	class WGRetTeamRecruitInfoHandler
	{
	public:
		static UINT Execute(WGRetTeamRecruitInfo* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _WG_TEAM_LIST_H_
