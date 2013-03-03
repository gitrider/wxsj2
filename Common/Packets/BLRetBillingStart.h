#ifndef _BLRET_BILLINGSTART_H_
#define _BLRET_BILLINGSTART_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "FoxWin32.h"


namespace Packets
{
	


	class BLRetBillingStart : public BillPacket 
	{
	public:
		BLRetBillingStart( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
		};
		virtual ~BLRetBillingStart( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGSTART ; }
		virtual UINT			GetPacketSize() const 
		{
			INT	VarLength = 0;
			if(mResult==0x01) VarLength = sizeof(UINT)*3;

			return sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(BYTE)
				+sizeof(BYTE)+VarLength;
		}

	public:
		//使用数据接口
		BYTE				GetAccLength()
		{
			return mAccLength;
		}
		VOID		SetAccount(const CHAR* pSrc	)
		{
			Assert(pSrc);
			strncpy(mAccount, pSrc, sizeof(mAccount));
			mAccLength = min(strlen(mAccount), MAX_BILLACCOUNT);
			mAccount[mAccLength] = 0;
			
		}
		const CHAR*			GetAccount()
		{
			return mAccount;
		}
		VOID				SetResult(BYTE res) { mResult = res; }
		BYTE				GetResult()
		{
			return mResult;
		}
		VOID				SetCode(BYTE code) { mCode = code; }
		BYTE				GetCode()
		{
			return mCode;
		}
		VOID				SetFeeType(BYTE val) { mFeeType = val; }
		BYTE				GetFeeType()
		{
			return mFeeType;
		}
		VOID				SetLeftTime(UINT val) { mLeftTime = val; }
		UINT				GetLeftTime()
		{
			return mLeftTime;
		}
		VOID				SetStorePoint(UINT val) { mStorePoint = val; }
		UINT				GetStorePoint()
		{
			return mStorePoint;
		}
		VOID				SetUserPoint(UINT val) { mUserPoint = val; }
		UINT				GetUserPoint()
		{
			return mUserPoint;
		}
	private:
		//数据
		BYTE				mAccLength;//用户名长度
		CHAR				mAccount[MAX_BILLACCOUNT+1];	//用户名串
		BYTE				mResult;//Billing Start 结果
		
		//mResult = 0x01
		BYTE				mFeeType;		//计费类型
		UINT				mLeftTime;		//剩余时间
		UINT				mStorePoint;	//商店点数
		UINT				mUserPoint;		//玩家剩余点数
		//mResult = 0x00
		BYTE				mCode;			//代码
	};

	class BLRetBillingStartFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetBillingStart() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGSTART; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE)
				   +sizeof(BYTE)+sizeof(UINT)*3;
		}
	};


	class BLRetBillingStartHandler 
	{
	public:
		static UINT Execute( BLRetBillingStart* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif