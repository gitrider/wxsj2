#ifndef _BLRET_REGPASSPORT_H_
#define _BLRET_REGPASSPORT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class BLRetRegPassPort : public BillPacket
	{
	public:
		BLRetRegPassPort()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			mAccLength = mAccResult = 0;
		};
		virtual ~BLRetRegPassPort(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETREGPASSPORT;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+mAccLength+sizeof(BYTE);
		}

		public:
			BYTE					GetAccLength()
			{
				return mAccLength;
			}
			const CHAR*				GetAccount()
			{
				return mAccount;
			}
			BYTE					GetAccResult()
			{
				return mAccResult;
			}
		private:
			BYTE					mAccLength;					//用户名长度
			CHAR					mAccount[MAX_BILLACCOUNT+1];//用户名串
			BYTE					mAccResult;					//认证结果
	};


	class BLRetRegPassPortFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetRegPassPort() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETREGPASSPORT; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE);
		}
	};


	class BLRetRegPassPortHandler 
	{
	public:
		static UINT Execute( BLRetRegPassPort* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif