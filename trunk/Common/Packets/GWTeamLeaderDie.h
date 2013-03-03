
/********************************************************************************
*	�ļ�����	CGTopList.h

*	�����ˣ�	�����
*	����ʱ�䣺	2008 �� 6 �� 23 ��	
*
*	����˵����	�ӳ�������֪ͨGL����Ҫ���Ķӳ���
*	�޸ļ�¼��
*********************************************************************************/
#ifndef _GW_TEAM_LEADER_DIE_H_
#define _GW_TEAM_LEADER_DIE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamLeaderDie:	public Packet
	{
		public:
			GWTeamLeaderDie(){};
			virtual		~GWTeamLeaderDie(){};

			//���ýӿ�
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAM_LEADER_DIE ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :
		//���ݽӿ�
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

	public :
		//����
		GUID_t			m_SourGUID ;		//�Լ���GUID

	};

	class GWTeamLeaderDieFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamLeaderDie() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAM_LEADER_DIE ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t); }			
	};

	class GWTeamLeaderDieHandler
	{
		public:
			static UINT Execute(GWTeamLeaderDie* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif