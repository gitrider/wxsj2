#ifndef _LB_BILLINGEND_H_
#define _LB_BILLINGEND_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class LBBillingEnd : public BillPacket 
	{
	public:
		LBBillingEnd( ){};
		virtual ~LBBillingEnd( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_BILLINGEND; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(USHORT)+sizeof(BYTE)
				   +sizeof(UINT)+sizeof(UINT);
		}

	public:
		//使用数据接口

		BYTE				GetAccLength()
		{
			return mAccLength;
		}

		VOID					SetAccount(const CHAR* pAcc)
		{
			strncpy(mAccount,pAcc,MAX_BILLACCOUNT);
			mAccLength = 0;
			for(INT i =0;i<MAX_BILLACCOUNT;i++)
			{
				if(mAccount[i]) mAccLength++;
			}
		}
		USHORT				GetCharLevel()
		{
			return mCharLevel;
		}
		VOID				SetCharLevel(USHORT charLevel)
		{
			mCharLevel = charLevel;
		}
		BYTE				GetEndType()
		{
			return mEndType;
		}
		VOID				SetEndType(BYTE bType)
		{
			mEndType = bType;
		}

		VOID				SetStartTime(UINT uStartTime)
		{
			mStartTime = uStartTime;
		}
		UINT				GetStartTime()
		{
			return mStartTime;
		}

		VOID				SetEndTime(UINT uEndTime)
		{
			mEndTime = uEndTime;
		}
		UINT				GetEndTime()
		{
			return mEndTime;
		}


	private:
		//数据
		BYTE				mAccLength;					//用户名长度
		CHAR				mAccount[MAX_BILLACCOUNT];	//用户名串
		USHORT				mCharLevel;					//用户角色级别
		BYTE				mEndType;					//计费结束类型
		UINT				mStartTime;					//开始时间
		UINT				mEndTime;					//结束时间
	};

	class LBBillingEndFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBBillingEnd() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_BILLINGEND; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(USHORT)+sizeof(BYTE)
				   +sizeof(UINT)+sizeof(UINT);
		}
	};


	class LBBillingEndHandler 
	{
	public:
		static UINT Execute( LBBillingEnd* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif