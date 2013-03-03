

#ifndef __GCSUBTRAINRESULT_H__
#define __GCSUBTRAINRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCSubTrainResult : public Packet 
	{
	public:
		GCSubTrainResult( ){} ;
		virtual ~GCSubTrainResult( ){} ;

		//
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SUBTRAINRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE) ; }

	public :
		VOID		SetResult( BYTE nResult ) { m_nResult = nResult; }
		BYTE		GetResult( ) { return m_nResult; }

	private:
		BYTE m_nResult;
	};


	class GCSubTrainResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCSubTrainResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SUBTRAINRESULT ; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) ; }
	};


	class GCSubTrainResultHandler 
	{
	public:
		static UINT Execute( GCSubTrainResult* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif