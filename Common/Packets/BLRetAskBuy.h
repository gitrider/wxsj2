#ifndef _BLRET_ASKBUY_H_
#define _BLRET_ASKBUY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class BLRetAskBuy : public BillPacket
	{
	public:
		BLRetAskBuy()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mBuySerial,0,MAX_PRIZE_SERIAL_LENGTH+1);
			mAccLength = mBuyResult = 0;
			mGoodsNum = 0;
			mLeftPoint = 0;
		};
		virtual ~BLRetAskBuy(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_ASKBUY;}
		virtual UINT			GetPacketSize() const 
		{
			UINT	nPacketSize = sizeof(BYTE)+mAccLength+
								  sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH+sizeof(BYTE);
			if(/*BABR_SUCCESS*/0x00 == mBuyResult ||
			   /*BABR_ALREADY_GET*/0x01 == mBuyResult)
			{
				nPacketSize += sizeof(INT);
				nPacketSize += sizeof(USHORT);
				nPacketSize += _RETBUY_DATA::getSize()*mGoodsNum;
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
			BYTE					GetBuyResult()
			{
				return mBuyResult;
			}
			USHORT					GetGoodsNum()
			{
				return mGoodsNum;
			}
			_RETBUY_DATA*			GetGoods(INT idx)
			{
				if(idx >= MAX_PRIZE_NUMBER || idx < 0) return NULL;
				return &(mGoods[idx]);
			}
			const CHAR*				GetBuySerial()
			{
				return mBuySerial;
			}
			VOID					SetBuySerial(const CHAR* pSerial)
			{
				strncpy(mBuySerial,pSerial,MAX_PRIZE_SERIAL_LENGTH);
				mBuySerial[MAX_PRIZE_SERIAL_LENGTH] = '\0';
			}
			INT						GetLeftPoint(){return mLeftPoint;}
		private:
			BYTE					mAccLength;						//用户名长度
			CHAR					mAccount[MAX_BILLACCOUNT+1];	//用户名串

			BYTE					mBuyResult;						//购买结果

			INT						mLeftPoint;						//剩余点数					
			USHORT					mGoodsNum;						//商品数量
			_RETBUY_DATA			mGoods[MAX_PRIZE_NUMBER];		//商品信息

			CHAR					mBuySerial[MAX_PRIZE_SERIAL_LENGTH+1];	//购买序列号，定长
	};


	class BLRetAskBuyFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetAskBuy() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_ASKBUY; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE)+sizeof(INT)+
				   sizeof(USHORT)+_RETBUY_DATA::getSize()*MAX_PRIZE_NUMBER+
				   sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH;
		}
	};


	class BLRetAskBuyHandler 
	{
	public:
		static UINT Execute( BLRetAskBuy* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif