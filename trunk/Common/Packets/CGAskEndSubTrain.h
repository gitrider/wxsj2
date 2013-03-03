

#ifndef __CGASKENDSUBTRAIN_H__
#define __CGASKENDSUBTRAIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskEndSubTrain : public Packet 
	{
	public:
		CGAskEndSubTrain( ){} ;
		virtual ~CGAskEndSubTrain( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKENDSUBTRAIN ; }
		virtual UINT			GetPacketSize()const { return 0 ; }

	public :

	};


	class CGAskEndSubTrainFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskEndSubTrain() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKENDSUBTRAIN ; }
		UINT		GetPacketMaxSize()const { return 0 ; }
	};


	class CGAskEndSubTrainHandler 
	{
	public:
		static UINT Execute( CGAskEndSubTrain* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif