

#ifndef __GCGETSUBTRAINEXPRESULT_H__
#define __GCGETSUBTRAINEXPRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCGetSubTrainExpResult : public Packet 
	{
	public:
		GCGetSubTrainExpResult( ){} ;
		virtual ~GCGetSubTrainExpResult( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_GETSUBTRAINEXPRESULT ; }
		virtual UINT			GetPacketSize()const { return 0; }

	public :
	};


	class GCGetSubTrainExpResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCGetSubTrainExpResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_GETSUBTRAINEXPRESULT ; }
		UINT		GetPacketMaxSize()const { return 0; }
	};


	class GCGetSubTrainExpResultHandler 
	{
	public:
		static UINT Execute( GCGetSubTrainExpResult* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif