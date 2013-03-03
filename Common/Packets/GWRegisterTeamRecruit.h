/********************************************************************************
 *	文件名：	GWRegisterTeamRecruit.cpp
 *	创建人：	王勇鸷
 *	创建时间：	2008 年 5 月 14 日	17:57
 *
 *	功能说明：	队长发布征收队员的消息
 *	修改记录：
*********************************************************************************/

#ifndef _GW_REGISTER_TEAM_RECRUIT_H_
#define _GW_REGISTER_TEAM_RECRUIT_H_

#include "Type.h"
#include "Packet.h"
#include "GameStruct_Team.h"

namespace Packets
{
	class GWRegisterTeamRecruit:	public Packet
	{
	public:
		GWRegisterTeamRecruit(){};
		virtual		~GWRegisterTeamRecruit(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_REGISTER_TEAM_RECRUIT ; }
		virtual UINT			GetPacketSize()const 
		{
			UINT size = sizeof(UCHAR) + sizeof(GUID_t);

			if ( m_Type == LEADER_RECRUIT )
			{
				size += m_LeaderRecruit.GetSize();
			}
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

	public:

		//LEADER_RECRUIT = 0,		//队长发布征收队员消息
		//MEMBER_RECRUIT ,			//队员发布征收队员消息
		UCHAR					m_Type;
		GUID_t					m_GUID;	

		MEMBER_RECRUIT_ATTRIB	m_MemberRecruit;
		LEADER_RECRUIT_ATTRIB	m_LeaderRecruit;

	};

	class GWRegisterTeamRecruitFactory: public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWRegisterTeamRecruit() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GW_REGISTER_TEAM_RECRUIT ; }
		UINT		GetPacketMaxSize()const		
		{ 
			UINT size = sizeof(UCHAR) + sizeof(GUID_t) + sizeof(LEADER_RECRUIT_ATTRIB) 
						+ sizeof(MEMBER_RECRUIT_ATTRIB);

			return size;
		}			
	};

	class GWRegisterTeamRecruitHandler
	{
	public:
		static UINT Execute(GWRegisterTeamRecruit* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif// 
