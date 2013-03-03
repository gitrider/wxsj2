#ifndef _LB_ACCCHECK_H_
#define _LB_ACCCHECK_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

	class LBAccCheck : public BillPacket 
	{
	public:
		LBAccCheck( )
		{
			memset(mAccount,0,(MAX_BILLACCOUNT+1));
			memset(mPwMd5,0,MAX_MD5_SIZE+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
			{
				memset(mAllMiBaoKey[i],0,MIBAOUNIT_NAME_LENGTH+1);
				memset(mAllMiBaoValue[i],0,MIBAOUNIT_VALUE_LENGTH+1);
			}
			memset(mMacAddr,0,MAX_MD5_SIZE+1);
		} ;
		virtual ~LBAccCheck( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_ACCCHECK;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength
				+sizeof(BYTE)+sizeof(CHAR)*mPwLength+sizeof(BYTE)
				+sizeof(CHAR)*mIPLength+sizeof(USHORT)
				+sizeof(CHAR)*MIBAOUNIT_NUMBER*MIBAOUNIT_NAME_LENGTH
				+sizeof(CHAR)*MIBAOUNIT_NUMBER*MIBAOUNIT_VALUE_LENGTH
				+sizeof(CHAR)*MAX_MD5_SIZE;
		}

	public:
		//使用数据接口
		BYTE					GetAccLength()
		{
			return mAccLength;
		}
		
		const CHAR*				GetAccount()
		{
			return mAccount;
		}
		VOID					SetAccount(const CHAR* pAcc)
		{
			strncpy(mAccount,pAcc,MAX_BILLACCOUNT);
			mAccount[MAX_BILLACCOUNT] = '\0';

			mAccLength = 0;
			for(INT i =0;i<MAX_BILLACCOUNT;i++)
			{
				if(mAccount[i]) mAccLength++;
			}
		}

		BYTE					GetPwLength()
		{
			return mPwLength;
		}
		
		VOID					SetPassWord(const CHAR* pPw)
		{
			strncpy(mPwMd5,pPw,MAX_MD5_SIZE);
			mPwLength = 0;
			for(INT i =0;i<MAX_MD5_SIZE;i++)
			{
				if(mPwMd5[i]) mPwLength++;
			}
		}
		VOID					SetMacAddr(const CHAR* pMacMD5)
		{
			strncpy(mMacAddr,pMacMD5,MAX_MD5_SIZE);
			mMacAddr[MAX_MD5_SIZE] = 0;
		}
		BYTE					GetIPLength()
		{
			return	mIPLength;
		}
		
		VOID					SetIPAddr(const CHAR* pIP)
		{
			strncpy(mIP,pIP,MAX_BILLING_IP_SIZE);
			mIPLength = 0;
			for(INT i =0;i<MAX_BILLING_IP_SIZE;i++)
			{
				if(mIP[i]) mIPLength++;
			}
		}

		VOID					SetUserLevel(USHORT uLevel)
		{
			mUserLevel = uLevel;
		}

		VOID					SetMiBaoKey(INT idx, const CHAR* pKey)
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return;
			strncpy(mAllMiBaoKey[idx],pKey,MIBAOUNIT_NAME_LENGTH);
		}

		VOID					SetMiBaoValue(INT idx, const CHAR* pValue)
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return;
			strncpy(mAllMiBaoValue[idx],pValue,MIBAOUNIT_VALUE_LENGTH);
		}
	private:
	
		BYTE					mAccLength;					//用户名长度
		CHAR					mAccount[MAX_BILLACCOUNT+1];	//用户名串
		BYTE					mPwLength;					//密码长度
		CHAR					mPwMd5[MAX_MD5_SIZE+1];		//密码md5
		BYTE					mIPLength;					//用户IP长度
		CHAR					mIP[MAX_BILLING_IP_SIZE+1];	//用户IP地址
		USHORT					mUserLevel;					//用户级别

		CHAR					mAllMiBaoKey[MIBAOUNIT_NUMBER][MIBAOUNIT_NAME_LENGTH+1];	//密保名
		CHAR					mAllMiBaoValue[MIBAOUNIT_NUMBER][MIBAOUNIT_VALUE_LENGTH+1];	//密保值
		CHAR					mMacAddr[MAX_MD5_SIZE+1];	//用户电脑的MAC地址MD5
	};

	class LBAccCheckFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBAccCheck() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_ACCCHECK; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				   +sizeof(BYTE)+sizeof(CHAR)*MAX_MD5_SIZE+sizeof(BYTE)
				   +sizeof(CHAR)*MAX_BILLING_IP_SIZE+sizeof(USHORT)
				   +sizeof(CHAR)*MIBAOUNIT_NUMBER*MIBAOUNIT_NAME_LENGTH
				   +sizeof(CHAR)*MIBAOUNIT_NUMBER*MIBAOUNIT_VALUE_LENGTH
				   +sizeof(CHAR)*MAX_MD5_SIZE;
		}
	};


	class LBAccCheckHandler 
	{
	public:
		static UINT Execute( LBAccCheck* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif