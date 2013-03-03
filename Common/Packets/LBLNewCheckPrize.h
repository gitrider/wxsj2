#ifndef _LBL_NEWCHECKPRIZE_H_
#define _LBL_NEWCHECKPRIZE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBLNewCheckPrize : public BillPacket
	{
	public:
		enum
		{
			LB_CHECK_PRIZE,
			BL_CHECK_PRIZE,
		};

		LBLNewCheckPrize()
		{
			mType = BL_CHECK_PRIZE;

			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mCharName,0,MAX_CHARACTER_NAME+1);
			
			mAccLength = mIPLength = mCharNameLength = 0;

			memset(mPrize,0,sizeof(_PRIZE_DATA)*MAX_PRIZE_NUMBER);
			mPrizeNum = mPrizeResult = 0;
		};
		virtual ~LBLNewCheckPrize(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LBL_CHECKPRIZE;}
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE)+sizeof(CHAR)*mAccLength;
			if(LB_CHECK_PRIZE == mType)
			{
				uSize += sizeof(BYTE)+sizeof(CHAR)*mIPLength
						 +sizeof(BYTE)+sizeof(CHAR)*mCharNameLength;
			}
			else if(BL_CHECK_PRIZE == mType)
			{
				uSize += sizeof(BYTE);
				if(/*BNPR_SUCCESS*/0x00 == mPrizeResult)
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

			BYTE					mPrizeResult;					//抽奖结果

			BYTE					mPrizeNum;						//奖品数量
			_PRIZE_DATA				mPrize[MAX_PRIZE_NUMBER];		//奖品信息
	};


	class LBLNewCheckPrizeFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBLNewCheckPrize() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LBL_CHECKPRIZE; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_CHARACTER_NAME
				+sizeof(BYTE)+sizeof(BYTE)
				+_PRIZE_DATA::getSize()*MAX_PRIZE_NUMBER;
		}
	};


	class LBLNewCheckPrizeHandler 
	{
	public:
		static UINT Execute( LBLNewCheckPrize* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif