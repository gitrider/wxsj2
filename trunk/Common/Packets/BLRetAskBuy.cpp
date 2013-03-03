#include "stdafx.h" 
#include "BLRetAskBuy.h"



BOOL BLRetAskBuy::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&mAccLength,sizeof(BYTE));
		if(mAccLength>MAX_BILLACCOUNT) mAccLength = MAX_BILLACCOUNT;
		iStream.Read(mAccount,mAccLength*sizeof(CHAR));
		iStream.Read((CHAR*)(mBuySerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
		iStream.Read((CHAR*)&mBuyResult,sizeof(BYTE));
		
		if(/*BABR_SUCCESS*/0x00 == mBuyResult ||
		   /*BABR_ALREADY_GET*/0x01 == mBuyResult)
		{
			iStream.Read((CHAR*)&mLeftPoint,sizeof(INT));
			mLeftPoint = ntohl(mLeftPoint);
			iStream.Read((CHAR*)&mGoodsNum,sizeof(USHORT));
			mGoodsNum = ntohs(mGoodsNum);
			if(mGoodsNum>MAX_PRIZE_NUMBER) mGoodsNum = MAX_PRIZE_NUMBER;
			
			for(INT i = 0; i < mGoodsNum; ++i)
			{
				iStream.Read((CHAR*)&(mGoods[i].m_BuyInt),sizeof(INT));
				mGoods[i].m_BuyInt = ntohl(mGoods[i].m_BuyInt);
				iStream.Read((CHAR*)&(mGoods[i].m_BuyNumber),sizeof(USHORT));
				mGoods[i].m_BuyNumber = ntohs(mGoods[i].m_BuyNumber);
			}
		}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetAskBuy::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetAskBuy::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetAskBuyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}