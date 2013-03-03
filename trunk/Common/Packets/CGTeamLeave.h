#ifndef _CG_TEAMLEAVE_H_
#define _CG_TEAMLEAVE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTeamLeave:	public Packet
	{
		public:
			CGTeamLeave(){};
			virtual		~CGTeamLeave(){};

			//���ýӿ�
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEAMLEAVE ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :
		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

	public :
		GUID_t				m_GUID ;	//�뿪�������

	};

	class CGTeamLeaveFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGTeamLeave() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_TEAMLEAVE ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t); }			
	};

	class CGTeamLeaveHandler
	{
		public:
			static UINT Execute(CGTeamLeave* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif