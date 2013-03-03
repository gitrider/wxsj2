#ifndef _LB_BILLINGSTART_H_
#define _LB_BILLINGSTART_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	


	class LBBillingStart : public BillPacket 
	{
	public:
		LBBillingStart( ){};
		virtual ~LBBillingStart( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_BILLINGSTART ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength+
				   sizeof(BYTE)+sizeof(CHAR)*mCharLength+sizeof(UINT)+
				   sizeof(USHORT)+sizeof(UINT);
		}

	public:
		//使用数据接口

		BYTE				GetAccLength()
		{
			return mAccLength;
		}
		const CHAR*		GetAccount(VOID) { return mAccount; }
		VOID					SetAccount(const CHAR* pAcc)
		{
			Assert(pAcc);
			strncpy(mAccount,pAcc,MAX_BILLACCOUNT);
			mAccLength = strlen(pAcc);
		}
		BYTE				GetCharLength()
		{
			return mCharLength;
		}
		VOID				SetCharName(const CHAR* pChar)
		{
			strncpy(mCharName,pChar,MAX_BILLACCOUNT);
			mCharLength = 0;
			for(INT i =0;i<MAX_BILLACCOUNT;i++)
			{
				if(mCharName[i]) mCharLength++;
			}
		}
		VOID				SetCharGUID(UINT guid)
		{
			mCharGUID = guid;
		}
		UINT				GetCharGUID()
		{
			return mCharGUID;
		}
		USHORT				GetCharLevel()
		{
			return mCharLevel;
		}
		VOID				SetCharLevel(USHORT charLevel)
		{
			mCharLevel = charLevel;
		}
		VOID				SetStartTime(UINT uStartTime)
		{
			mStartTime = uStartTime;
		}
		UINT				GetStartTime()
		{
			return mStartTime;
		}
		

	private:
		//数据
		BYTE				mAccLength;//用户名长度
		CHAR				mAccount[MAX_BILLACCOUNT];	//用户名串
		BYTE				mCharLength;	//角色名长度
		CHAR				mCharName[MAX_BILLACCOUNT];	//角色名
		UINT				mCharGUID;	//角色GUID
		USHORT				mCharLevel;	//用户角色级别
		UINT				mStartTime;	//开始时间戳
	};

	class LBBillingStartFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBBillingStart() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_BILLINGSTART; }
		UINT			GetPacketMaxSize() const
		{ 
			return 2*(sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT)+sizeof(UINT)+sizeof(USHORT)+sizeof(UINT);
		}
	};


	class LBBillingStartHandler 
	{
	public:
		static UINT Execute( LBBillingStart* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif