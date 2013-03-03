/********************************************************************************
 *	�ļ�����	CGRegisterTeamRecruit.cpp
 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 5 �� 14 ��	17:57
 *
 *	����˵����	�ӳ��������ն�Ա����Ϣ
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _CG_REGISTER_TEAM_RECRUIT_H_
#define _CG_REGISTER_TEAM_RECRUIT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Team.h"

namespace Packets
{
	class CGRegisterTeamRecruit:	public Packet
	{
	public:
		CGRegisterTeamRecruit(){};
		virtual		~CGRegisterTeamRecruit(){};

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_REGISTER_TEAM_RECRUIT ; }
		virtual UINT			GetPacketSize() const
		{
			UINT size = sizeof(UCHAR) + sizeof(GUID_t);
			if ( m_Type == LEADER_RECRUIT )
				size += m_LeaderRecruit.GetSize();
			else if (m_Type == MEMBER_RECRUIT )
			{
				size += m_MemberRecruit.GetSize();
			}
			else
			{
				Assert(FALSE);
			}

			return size;
		}

		VOID				SetType(UCHAR Type) { m_Type=Type; }
		UCHAR				GetType() { return m_Type; }
		
		VOID				SetGUID(GUID_t id) { m_GUID=id; }
		GUID_t				GetGUID() { return m_GUID; }

		VOID				SetLeaderRecruit(LEADER_RECRUIT_ATTRIB& Data)
		{
			memcpy(&m_LeaderRecruit,&Data,sizeof(LEADER_RECRUIT_ATTRIB));
		}

		VOID				SetMemberRecruit(MEMBER_RECRUIT_ATTRIB& Data)
		{
			memcpy(&m_MemberRecruit,&Data,sizeof(MEMBER_RECRUIT_ATTRIB));
		}

		LEADER_RECRUIT_ATTRIB*	GetLeaderRecruit() { return &m_LeaderRecruit;}
		MEMBER_RECRUIT_ATTRIB*	GetMemberRecruit() { return &m_MemberRecruit;}

	public :

		//LEADER_RECRUIT = 0,		//�ӳ��������ն�Ա��Ϣ
		//MEMBER_RECRUIT ,			//��Ա�������ն�Ա��Ϣ
		UCHAR					m_Type;
		GUID_t					m_GUID;	

		MEMBER_RECRUIT_ATTRIB	m_MemberRecruit;
		LEADER_RECRUIT_ATTRIB	m_LeaderRecruit;

	};

	class CGRegisterTeamRecruitFactory: public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGRegisterTeamRecruit() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_REGISTER_TEAM_RECRUIT ; }
		UINT		GetPacketMaxSize()	const	
		{ 
			UINT size = sizeof(UCHAR) + sizeof(GUID_t) + sizeof(LEADER_RECRUIT_ATTRIB)
						+ sizeof(MEMBER_RECRUIT_ATTRIB);
			return size;
		}					
	};

	class CGRegisterTeamRecruitHandler
	{
	public:
		static UINT Execute(CGRegisterTeamRecruit* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif// 
