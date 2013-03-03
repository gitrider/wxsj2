

#ifndef __GCENDSUBTRAINRESULT_H__
#define __GCENDSUBTRAINRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCEndSubTrainResult : public Packet 
	{
	public:
		GCEndSubTrainResult( ){} ;
		virtual ~GCEndSubTrainResult( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ENDSUBTRAINRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(UINT)+sizeof(UINT); }

	public :
		//使用数据接口
		UINT					getCountMoney(VOID) { return m_nCountMoney; }
		VOID					setMoney(UINT nMoney) { m_nCountMoney = nMoney; }

		//使用数据接口
		UINT					getCountExp(VOID) { return m_nCountExp; }
		VOID					setExp(UINT nExp) 
		{ 
			m_nCountExp = nExp;
		}
	private:
		UINT	m_nCountMoney;
		UINT	m_nCountExp;
	};


	class GCEndSubTrainResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCEndSubTrainResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_ENDSUBTRAINRESULT ; }
		UINT		GetPacketMaxSize()const { return sizeof(UINT)+sizeof(UINT); }
	};


	class GCEndSubTrainResultHandler 
	{
	public:
		static UINT Execute( GCEndSubTrainResult* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif