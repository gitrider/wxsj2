#include "stdafx.h" 
#include "LBAskBuy.h"



BOOL LBAskBuy::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
	iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
	iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
	iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
	iStream.Read( (CHAR*)(&mCharNameLength),sizeof(BYTE));
	iStream.Read( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
	iStream.Read( (CHAR*)(mPrizeSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
	//USHORT uGoodsTypeNum = htons(mGoodsTypeNum);
	iStream.Read( (CHAR*)(&mGoodsTypeNum),sizeof(USHORT));
	//UINT uGoodsType = htonl(mGoodsType);
	iStream.Read( (CHAR*)&mGoodsType,sizeof(UINT));
	//USHORT uGoodsNum = htons(mGoodsNumber);
	iStream.Read( (CHAR*)(&mGoodsNumber),sizeof(USHORT));	
	//UINT uCostPoint = htonl(mCostPoint);
	iStream.Read( (CHAR*)(&mCostPoint),sizeof(UINT));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBAskBuy::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		oStream.Write( (CHAR*)(mPrizeSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
		USHORT uGoodsTypeNum = htons(mGoodsTypeNum);
		oStream.Write( (CHAR*)(&uGoodsTypeNum),sizeof(USHORT));
		UINT uGoodsType = htonl(mGoodsType);
		oStream.Write( (CHAR*)&uGoodsType,sizeof(UINT));
		USHORT uGoodsNum = htons(mGoodsNumber);
		oStream.Write( (CHAR*)(&uGoodsNum),sizeof(USHORT));	
		UINT uCostPoint = htonl(mCostPoint);
		oStream.Write( (CHAR*)(&uCostPoint),sizeof(UINT));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBAskBuy::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBAskBuyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}