#ifndef _BLRET_PRIZE_H_
#define _BLRET_PRIZE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class BLRetPrize : public BillPacket
	{
	public:
		BLRetPrize()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mPrize,0,sizeof(_PRIZE_DATA)*MAX_PRIZE_NUMBER);
			memset(mPrizeSerial,0,MAX_PRIZE_SERIAL_LENGTH+1);
			mAccLength = mPrizeResult = mPrizeNum = 0;
		};
		virtual ~BLRetPrize(){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_PRIZE;}
		virtual UINT			GetPacketSize() const 
		{
			UINT	nPacketSize = sizeof(BYTE)+mAccLength+
								  sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH+sizeof(BYTE);
			if(/*BPR_SUCCESS*/0x00 == mPrizeResult ||
			   /*BPR_ALREADY_GET*/0x02 == mPrizeResult)
			{
				nPacketSize += sizeof(BYTE);
				nPacketSize += _PRIZE_DATA::getSize()*mPrizeNum;
			}
			return nPacketSize;
		}

		public:
			BYTE					GetAccLength()
			{
				return mAccLength;
			}
			const CHAR*				GetAccount()
			{
				return mAccount;
			}
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
			const CHAR*				GetPrizeSerial()
			{
				return mPrizeSerial;
			}
			VOID					SetPrizeSerial(const CHAR* pSerial)
			{
				strncpy(mPrizeSerial,pSerial,MAX_PRIZE_SERIAL_LENGTH);
				mPrizeSerial[MAX_PRIZE_SERIAL_LENGTH] = '\0';
			}
		private:
			BYTE					mAccLength;						//�û�������
			CHAR					mAccount[MAX_BILLACCOUNT+1];	//�û�����

			BYTE					mPrizeResult;					//�齱���

			BYTE					mPrizeNum;						//��Ʒ����
			_PRIZE_DATA				mPrize[MAX_PRIZE_NUMBER];		//��Ʒ��Ϣ

			CHAR					mPrizeSerial[MAX_PRIZE_SERIAL_LENGTH+1];	//�콱���кţ�����
	};


	class BLRetPrizeFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetPrize() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_PRIZE; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE)+
				sizeof(BYTE)+_PRIZE_DATA::getSize()*MAX_PRIZE_NUMBER+
				   sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH;
		}
	};


	class BLRetPrizeHandler 
	{
	public:
		static UINT Execute( BLRetPrize* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif