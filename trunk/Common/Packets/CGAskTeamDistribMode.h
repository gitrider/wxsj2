#ifndef _CG_ASKTEAMDISTRIBMODE_H_
#define _CG_ASKTEAMDISTRIBMODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class CGAskTeamDistribMode:public Packet
	{

	public:
		CGAskTeamDistribMode(){};
		virtual				~CGAskTeamDistribMode(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_ASKTEAMDISTRIBMODE; }

		virtual UINT			GetPacketSize() const 
		{
			return 0;
		}

	private:
	};


	class CGAskTeamDistribModeFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGAskTeamDistribMode(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_ASKTEAMDISTRIBMODE; }
		UINT		GetPacketMaxSize() const { return 0; }
	};


	class CGAskTeamDistribModeHandler
	{
	public:
		static UINT Execute( CGAskTeamDistribMode* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif