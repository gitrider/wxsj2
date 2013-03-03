#ifndef _BLRET_BILLINGEND_H_
#define _BLRET_BILLINGEND_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	


	class BLRetBillingEnd : public BillPacket 
	{
	public:
		BLRetBillingEnd( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
		};
		virtual ~BLRetBillingEnd( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGEND ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(BYTE) ;
		}

	public:
		//使用数据接口
		BYTE				GetAccLength()
		{
			return mAccLength;
		}
		const CHAR*			GetAccount()
		{
			return mAccount;
		}
		BYTE				GetResult()
		{
			return mResult;
		}
	private:
		//数据
		BYTE				mAccLength;//用户名长度
		CHAR				mAccount[MAX_BILLACCOUNT+1];	//用户名串
		BYTE				mResult;//Billing End 结果
	};

	class BLRetBillingEndFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetBillingEnd() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGEND; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE) ;
		}
	};


	class BLRetBillingEndHandler 
	{
	public:
		static UINT Execute( BLRetBillingEnd* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif