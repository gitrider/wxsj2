#ifndef _LB_ASKBUY_H_
#define _LB_ASKBUY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBAskBuy : public BillPacket
	{
	public:
		LBAskBuy()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mCharName,0,MAX_CHARACTER_NAME+1);
			memset(mPrizeSerial,0,MAX_PRIZE_SERIAL_LENGTH+1);

			mAccLength = mIPLength = mCharNameLength = 0;
			mGoodsTypeNum = 1;
			mCostPoint = 0;
			mGoodsType = 0;
			mGoodsNumber = 0;
		};
		virtual ~LBAskBuy(){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_ASKBUY;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength
				+sizeof(BYTE)+sizeof(CHAR)*mIPLength
				+sizeof(BYTE)+sizeof(CHAR)*mCharNameLength
				+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH
				+sizeof(USHORT)+sizeof(UINT)
				+sizeof(USHORT)+sizeof(UINT);
		}

		public:
			//ʹ�����ݽӿ�
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

			UINT					GetCostPoint(){return mCostPoint;}
			VOID					SetCostPoint(UINT pt){mCostPoint = pt;}

			VOID					SetGoodsType(UINT nGoodsType){mGoodsType=nGoodsType;}
			VOID					SetGoodsNumber(USHORT nGoodsNumber){mGoodsNumber=nGoodsNumber;}
		private:
			BYTE					mAccLength;								//�ʻ�������
			CHAR					mAccount[MAX_BILLACCOUNT+1];			//�ʻ�����

			BYTE					mIPLength;								//�û�IP����
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];				//�û�IP��ַ

			BYTE					mCharNameLength;						//��ɫ������
			CHAR					mCharName[MAX_CHARACTER_NAME+1];		//��ɫ����

			CHAR					mPrizeSerial[MAX_PRIZE_SERIAL_LENGTH+1];	//�콱���кţ�����

			USHORT					mGoodsTypeNum;							//��Ʒ��������(��Զ��1)
			UINT					mGoodsType;								//��Ʒ����
			USHORT					mGoodsNumber;							//��Ʒ����
			UINT					mCostPoint;								//���ĵ���
	};


	class LBAskBuyFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBAskBuy() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_ASKBUY; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_CHARACTER_NAME
				+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH+sizeof(USHORT)
				+sizeof(UINT)+sizeof(USHORT)+sizeof(UINT);
		}
	};


	class LBAskBuyHandler 
	{
	public:
		static UINT Execute( LBAskBuy* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif