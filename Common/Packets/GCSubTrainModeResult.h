

#ifndef __GCSUBTRAINMODERESULT_H__
#define __GCSUBTRAINMODERESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCSubTrainModeResult : public Packet 
	{
	public:
		GCSubTrainModeResult( ){} ;
		virtual ~GCSubTrainModeResult( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SUBTRAINMODERESULT ; }
		virtual UINT			GetPacketSize()const { return 2*sizeof(UINT) ; }

	public :
		VOID		SetMoneyRate( UINT nMoneyRate ) { m_nMoneyRate = nMoneyRate; }
		UINT		GetMoneyRate( ) { return m_nMoneyRate; }

		VOID		SetExpRate( UINT nExpRate ) { m_nExpRate = nExpRate; }
		UINT		GetExpRate( ) { return m_nExpRate; }
	private:
		UINT	m_nMoneyRate;
		UINT	m_nExpRate;
	};


	class GCSubTrainModeResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCSubTrainModeResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SUBTRAINMODERESULT ; }
		UINT		GetPacketMaxSize()const { return 2*sizeof(UINT) ; }
	};


	class GCSubTrainModeResultHandler 
	{
	public:
		static UINT Execute( GCSubTrainModeResult* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif