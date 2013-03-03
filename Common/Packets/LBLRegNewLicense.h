#ifndef _LBL_REGNEWLICENSE_H_
#define _LBL_REGNEWLICENSE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBLRegNewLicense : public BillPacket
	{
	public:
		enum
		{
			LB_REGLICENSE,
			BL_REGLICENSE,
		};

		LBLRegNewLicense()
		{
			mType = BL_REGLICENSE;
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			mAccLength = 0;
			mRegLicense = 'Y';
			mIPLength = 0;
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
		};
		virtual ~LBLRegNewLicense(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LBL_REGLICENSE;}
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE)+sizeof(CHAR)*mAccLength;
			if(LB_REGLICENSE == mType)
			{
				uSize += sizeof(UCHAR)+sizeof(BYTE)+sizeof(CHAR)*mIPLength;
			}
			else if(BL_REGLICENSE == mType)
			{
				uSize += sizeof(UCHAR);
			}

			return uSize;
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

			BYTE					GetType(){return mType;}
			VOID					SetType(BYTE nType){mType = nType;}

			BYTE					GetServerResult(){return mRegLicense;}
			VOID					SetClientResult(BYTE rt){mRegLicense =(rt>0)?'Y':'N';}

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
		private:
			BYTE					mType;

			BYTE					mAccLength;								//帐户名长度
			CHAR					mAccount[MAX_BILLACCOUNT+1];			//帐户名串
			BYTE					mIPLength;					//用户IP长度
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];	//用户IP地址

			UCHAR					mRegLicense;							//新协议结果
	};


	class LBLRegNewLicenseFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBLRegNewLicense() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LBL_REGLICENSE; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(UCHAR)+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE;
		}
	};


	class LBLRegNewLicenseHandler 
	{
	public:
		static UINT Execute( LBLRegNewLicense* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif