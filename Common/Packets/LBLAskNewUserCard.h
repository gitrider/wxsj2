#ifndef _LBL_ASKNEWUSERCARD_H_
#define _LBL_ASKNEWUSERCARD_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBLAskNewUserCard : public BillPacket
	{
	public:
		enum
		{
			LB_NEWUSERCARD,
			BL_NEWUSERCARD,
		};

		LBLAskNewUserCard()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mCharName,0,MAX_CHARACTER_NAME+1);
			memset(mNewuserCard,0,MAX_NEWUSER_CARD_SIZE+1);
			memset(mPrize,0,sizeof(_PRIZE_DATA)*MAX_PRIZE_NUMBER);

			mCardLength = mAccLength = mIPLength = mCharNameLength = 0;
			mPrizeResult = 0;
			mType = BL_NEWUSERCARD;
		};
		virtual ~LBLAskNewUserCard(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LBL_NEWUSERCARD;}
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(BYTE)+sizeof(CHAR)*mAccLength;
			if(mType == LB_NEWUSERCARD)
			{
				uSize += sizeof(BYTE)+sizeof(CHAR)*mIPLength
					+sizeof(BYTE)+sizeof(CHAR)*mCharNameLength
					+sizeof(BYTE)+sizeof(CHAR)*mCardLength;
			}
			else if(mType == BL_NEWUSERCARD)
			{
				uSize += sizeof(BYTE);
				if(/*BNCR_SUCCESS*/0x00 == mPrizeResult ||
					/*BNCR_ALREADY_GET*/0x02 == mPrizeResult)
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

			const CHAR*				GetNewUserCard()
			{
				return mNewuserCard;
			}
			VOID					SetNewUserCard(const CHAR* pCard)
			{
				strncpy(mNewuserCard,pCard,MAX_NEWUSER_CARD_SIZE);
				mNewuserCard[MAX_NEWUSER_CARD_SIZE] = '\0';

				mCardLength = 0;
				for(INT i =0;i<MAX_CHARACTER_NAME;i++)
				{
					if(mNewuserCard[i]) mCardLength++;
				}
			}

			VOID					SetType(BYTE ty){mType = ty;}
			BYTE					GetType(){return mType;}

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
			BYTE					mType;									//消息包类型
			
			BYTE					mCardLength;							//卡号长度
			CHAR					mNewuserCard[MAX_NEWUSER_CARD_SIZE+1];	//新手卡卡号
			BYTE					mAccLength;								//帐户名长度
			CHAR					mAccount[MAX_BILLACCOUNT+1];			//帐户名串
			BYTE					mIPLength;								//用户IP长度
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];				//用户IP地址
			BYTE					mCharNameLength;						//角色名长度
			CHAR					mCharName[MAX_CHARACTER_NAME+1];		//角色名串

			BYTE					mPrizeResult;							//抽奖结果
			BYTE					mPrizeNum;								//奖品数量
			_PRIZE_DATA				mPrize[MAX_PRIZE_NUMBER];				//奖品信息
	};


	class LBLAskNewUserCardFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBLAskNewUserCard() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LBL_NEWUSERCARD; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_CHARACTER_NAME
				+sizeof(BYTE)+sizeof(CHAR)*MAX_NEWUSER_CARD_SIZE+sizeof(BYTE)
				+sizeof(BYTE)+_PRIZE_DATA::getSize()*MAX_PRIZE_NUMBER;
		}
	};


	class LBLAskNewUserCardHandler 
	{
	public:
		static UINT Execute( LBLAskNewUserCard* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif