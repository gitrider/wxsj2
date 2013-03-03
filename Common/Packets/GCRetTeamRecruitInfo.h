#ifndef _GC_RET_TEAM_RECRUIT_INFO_H_
#define _GC_RET_TEAM_RECRUIT_INFO_H_


#include "Type.h"
#include "Packet.h"
#include "GameStruct_Team.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCRetTeamRecruitInfo:public Packet
	{
	public:
		GCRetTeamRecruitInfo(){ m_nMemberCount = 0; };
		virtual ~GCRetTeamRecruitInfo(){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_RET_TEAM_RECRUIT; }
		virtual UINT			GetPacketSize() const
		{
			UINT				uSize;

			uSize				= sizeof(UCHAR) ;

			for( UCHAR i=0; i<m_nMemberCount; ++i)
			{
				uSize			+= m_MemberInfo[i].GetSize();
			}

			return uSize;
		}
		
	public:
		//使用数据接口
		VOID					AddMember( RETURN_RECRUIT_INFO& entry )
		{
			Assert( m_nMemberCount<ONCE_RECRUIT_LIST_LEN );
			memcpy(&m_MemberInfo[m_nMemberCount++],&entry,sizeof(RETURN_RECRUIT_INFO));
		}
		const RETURN_RECRUIT_INFO*	GetMember( UCHAR i )
		{
			Assert( i<m_nMemberCount );
			return &m_MemberInfo[i];
		}

		UCHAR					GetMemberCount( ) { return m_nMemberCount; }

	private:
		//数据
		UCHAR					m_nMemberCount;
		RETURN_RECRUIT_INFO		m_MemberInfo[ONCE_RECRUIT_LIST_LEN];

	};

	class GCRetTeamRecruitInfoFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCRetTeamRecruitInfo() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_RET_TEAM_RECRUIT ; }
		UINT		GetPacketMaxSize()const		
		{
			return sizeof(UCHAR) + sizeof(RETURN_RECRUIT_INFO) * ONCE_RECRUIT_LIST_LEN;
		}		
	};

	class GCRetTeamRecruitInfoHandler
	{
	public:
		static UINT Execute(GCRetTeamRecruitInfo* pPacket,Player* pPlayer) ;
	};

};

using namespace Packets;


#endif