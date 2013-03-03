#ifndef _LBL_NEWPRIZE_H_
#define _LBL_NEWPRIZE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBLNewPrize : public BillPacket
	{
	public:
		enum
		{
			LB_NEW_PRIZE,
			BL_NEW_PRIZE,
		};

		LBLNewPrize()
		{
			mType = BL_NEW_PRIZE;

			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mCharName,0,MAX_CHARACTER_NAME+1);
			memset(mPrizeSerial,0,MAX_PRIZE_SERIAL_LENGTH+1);

			mAccLength = mIPLength = mCharNameLength = 0;

			memset(mPrize,0,sizeof(_PRIZE_DATA)*MAX_PRIZE_NUMBER);
			mPrizeNum = mPrizeResult = 0;
		};
		virtual ~LBLNewPrize(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LBL_NEWPRIZE;}
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH;
			if(LB_NEW_PRIZE == mType)
			{
				uSize += sizeof(BYTE)+sizeof(CHAR)*mIPLength
						 +sizeof(BYTE)+sizeof(CHAR)*mCharNameLength;
			}
			else if(BL_NEW_PRIZE == mType)
			{
				uSize += sizeof(BYTE);
				if(/*BNPR_SUCCESS*/0x00 == mPrizeResult ||
					/*BNPR_ALREADY_GET*/0x02 == mPrizeResult)
				{
					uSize += sizeof(BYTE);
					uSize += _PRIZE_DATA::getSize()*mPrizeNum;
				}
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

			BYTE					GetPrizeResult()
			{
				return mPrizeResult;
			}
			BYTE					GetPrizeNum()
			{
				return mPrizeNum;
			}
			_PRIZE_DATA*			GetPrize(INT idx)
			{
				if(idx >= MAX_PRIZE_NUMBER || idx < 0) return NULL;
				return &(mPrize[idx]);
			}

		private:
			BYTE					mType;

			BYTE					mAccLength;								//帐户名长度
			CHAR					mAccount[MAX_BILLACCOUNT+1];			//帐户名串

			BYTE					mIPLength;								//用户IP长度
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];				//用户IP地址

			BYTE					mCharNameLength;						//角色名长度
			CHAR					mCharName[MAX_CHARACTER_NAME+1];		//角色名串

			CHAR					mPrizeSerial[MAX_PRIZE_SERIAL_LENGTH+1];	//领奖序列号，定长
			
			BYTE					mPrizeResult;					//抽奖结果

			BYTE					mPrizeNum;						//奖品数量
			_PRIZE_DATA				mPrize[MAX_PRIZE_NUMBER];		//奖品信息
	};


	class LBLNewPrizeFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBLNewPrize() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LBL_NEWPRIZE; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_CHARACTER_NAME
				+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH
				+sizeof(BYTE)+sizeof(BYTE)
				+_PRIZE_DATA::getSize()*MAX_PRIZE_NUMBER;
		}
	};


	class LBLNewPrizeHandler 
	{
	public:
		static UINT Execute( LBLNewPrize* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif