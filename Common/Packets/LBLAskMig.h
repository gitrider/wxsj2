#ifndef _LBL_ASKMIG_H_
#define _LBL_ASKMIG_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBLAskMig : public BillPacket
	{
	public:
		enum
		{
			LB_ASKMIG,
			BL_ASKMIG,
		};

		LBLAskMig()
		{
			mType = BL_ASKMIG;

			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mCharName,0,MAX_CHARACTER_NAME+1);
			memset(mPrizeSerial,0,MAX_PRIZE_SERIAL_LENGTH+1);

			mAccLength = mIPLength = mCharNameLength = 0;
			mCharLevel = 0;
			mCharGUID = INVALID_ID;
			mMigResult = 0;
		};
		virtual ~LBLAskMig(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LBL_ASKMIG;}
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH;
			if(LB_ASKMIG == mType)
			{
				uSize += sizeof(BYTE)+sizeof(CHAR)*mIPLength+sizeof(USHORT)+sizeof(UINT)
						 +sizeof(BYTE)+sizeof(CHAR)*mCharNameLength;
			}
			else if(BL_ASKMIG == mType)
			{
				uSize += sizeof(BYTE);
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

			BYTE					GetCharNameLength()
			{
				return mCharNameLength;
			}

			const CHAR*				GetCharName()
			{
				return mCharName;
			}
			VOID					SetCharName(const CHAR* pCharName)
			{
				strncpy(mCharName,pCharName,MAX_CHARACTER_NAME);
				mCharName[MAX_CHARACTER_NAME] = '\0';

				mCharNameLength = 0;
				for(INT i =0;i<MAX_CHARACTER_NAME;i++)
				{
					if(mCharName[i]) mCharNameLength++;
				}
			}

			const CHAR*				GetPrizeSerial()
			{
				return mPrizeSerial;
			}
			VOID					SetPrizeSerial(const CHAR* pSerial)
			{
				strncpy(mPrizeSerial,pSerial,MAX_PRIZE_SERIAL_LENGTH);
				mPrizeSerial[MAX_PRIZE_SERIAL_LENGTH] = '\0';
			}
			
			BYTE					GetType(){return mType;}
			VOID					SetType(BYTE nType){mType = nType;}

			USHORT					GetCharLevel(){return mCharLevel;}
			VOID					SetCharLevel(USHORT lv){mCharLevel = lv;}

			UINT					GetCharGUID(){return mCharGUID;}
			VOID					SetCharGUID(UINT guid){mCharGUID = guid;}

			BYTE					GetMigResult(){return mMigResult;}
			VOID					SetMigResult(BYTE rt){mMigResult = rt;}
		private:
			BYTE					mType;

			BYTE					mAccLength;								//帐户名长度
			CHAR					mAccount[MAX_BILLACCOUNT+1];			//帐户名串

			BYTE					mIPLength;								//用户IP长度
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];				//用户IP地址

			BYTE					mCharNameLength;						//角色名长度
			CHAR					mCharName[MAX_CHARACTER_NAME+1];		//角色名串

			CHAR					mPrizeSerial[MAX_PRIZE_SERIAL_LENGTH+1];	//序列号，定长
			
			USHORT					mCharLevel;								//角色等级
			UINT					mCharGUID;								//角色GUID

			BYTE					mMigResult;								//移民结果
	};


	class LBLAskMigFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBLAskMig() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LBL_ASKMIG; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_CHARACTER_NAME
				+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH
				+sizeof(USHORT)+sizeof(UINT)+sizeof(BYTE);
		}
	};


	class LBLAskMigHandler 
	{
	public:
		static UINT Execute( LBLAskMig* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif