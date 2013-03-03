#ifndef _LB_PRIZE_H_
#define _LB_PRIZE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBPrize : public BillPacket
	{
	public:
		LBPrize()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mCharName,0,MAX_CHARACTER_NAME+1);
			memset(mPrizeSerial,0,MAX_PRIZE_SERIAL_LENGTH+1);

			mAccLength = mIPLength = mCharNameLength = 0;
			mPrizeType = 0;
			mLevel = 1;
		};
		virtual ~LBPrize(){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_PRIZE;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength
				+sizeof(BYTE)+sizeof(CHAR)*mIPLength
				+sizeof(BYTE)+sizeof(CHAR)*mCharNameLength
				+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH
				+sizeof(USHORT)+sizeof(USHORT);
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

			VOID					SetPrizeType(USHORT type)
			{
				mPrizeType = type;
			}
		private:
			BYTE					mAccLength;								//�ʻ�������
			CHAR					mAccount[MAX_BILLACCOUNT+1];			//�ʻ�����
			USHORT					mLevel;									//�ʻ�����

			BYTE					mIPLength;								//�û�IP����
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];				//�û�IP��ַ

			BYTE					mCharNameLength;						//��ɫ������
			CHAR					mCharName[MAX_CHARACTER_NAME+1];		//��ɫ����

			CHAR					mPrizeSerial[MAX_PRIZE_SERIAL_LENGTH+1];	//�콱���кţ�����
			USHORT					mPrizeType;									//�콱����
	};


	class LBPrizeFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBPrize() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_PRIZE; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_CHARACTER_NAME
				+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH
				+sizeof(USHORT)+sizeof(USHORT);
		}
	};


	class LBPrizeHandler 
	{
	public:
		static UINT Execute( LBPrize* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif