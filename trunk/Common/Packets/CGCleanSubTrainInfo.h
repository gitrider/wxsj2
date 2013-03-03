

#ifndef __CGCLEANSUBTRAININFO_H__
#define __CGCLEANSUBTRAININFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCleanSubTrainInfo : public Packet 
	{
	public:
		CGCleanSubTrainInfo( ){} ;
		virtual ~CGCleanSubTrainInfo( ){} ;

		//
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CLEANSUBTRAININFO ; }
		virtual UINT			GetPacketSize()const { return 0 ; }

	public :
	};


	class CGCleanSubTrainInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCleanSubTrainInfo() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CLEANSUBTRAININFO ; }
		UINT		GetPacketMaxSize()const { return 0 ; }
	};


	class CGCleanSubTrainInfoHandler 
	{
	public:
		static UINT Execute( CGCleanSubTrainInfo* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif