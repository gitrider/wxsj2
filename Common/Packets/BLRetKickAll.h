#ifndef _BL_RETKICKALL_H_
#define _BL_RETKICKALL_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class BLRetKickALL : public BillPacket 
	{
	public:
		BLRetKickALL( )
		{
			mResult = 0;
		} ;
		virtual ~BLRetKickALL( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETKICKALL ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(mResult);
		}

	public:
		//使用数据接口
		BYTE					GetResult()
		{
			return mResult;
		}
	private:
		//数据
		BYTE					mResult;
		
	};

	class BLRetKickALLFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetKickALL() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETKICKALL; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE);
		}
	};


	class BLRetKickALLHandler 
	{
	public:
		static UINT Execute( BLRetKickALL* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif