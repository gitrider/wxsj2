/********************************************************************************
 *	�ļ�����	GWAskTeamRecruitInfo.h
 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 5 �� 14 ��	17:57
 *
 *	����˵����	����õ����������Ϣ
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _GW_ASK_TEAM_RECRUIT_INFO_H_
#define _GW_ASK_TEAM_RECRUIT_INFO_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Team.h"

namespace Packets
{
	class GWAskTeamRecruitInfo:	public Packet
	{
	public:
		GWAskTeamRecruitInfo(){};
		virtual		~GWAskTeamRecruitInfo(){};

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_ASK_TEAM_RECRUIT ; }
		virtual UINT			GetPacketSize() const
		{
			UINT size = sizeof(UCHAR) + sizeof(GUID_t);
			return size;
		}

		VOID				SetType(UCHAR Type) { m_Type=Type; }
		UCHAR				GetType() { return m_Type; }
		
		VOID				SetGUID(GUID_t id) { m_GUID=id; }
		GUID_t				GetGUID() { return m_GUID; }

	public :

		//LEADER_RECRUIT = 0,		//�ӳ�����
		//MEMBER_RECRUIT ,			//��Ա����
		UCHAR					m_Type;
		GUID_t					m_GUID;	

	};

	class GWAskTeamRecruitInfoFactory: public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWAskTeamRecruitInfo() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GW_ASK_TEAM_RECRUIT ; }
		UINT		GetPacketMaxSize()	const	
		{ 
			UINT size = sizeof(UCHAR) + sizeof(GUID_t);

			return size;
		}					
	};

	class GWAskTeamRecruitInfoHandler
	{
	public:
		static UINT Execute(GWAskTeamRecruitInfo* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif// 
