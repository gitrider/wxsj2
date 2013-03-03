#include "stdafx.h" 
#include "BLRetPrize.h"



BOOL BLRetPrize::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&mAccLength,sizeof(BYTE));
		if(mAccLength>MAX_BILLACCOUNT) mAccLength = MAX_BILLACCOUNT;
		iStream.Read(mAccount,mAccLength*sizeof(CHAR));
		iStream.Read((CHAR*)(mPrizeSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
		iStream.Read((CHAR*)&mPrizeResult,sizeof(BYTE));
		
		if(/*BPR_SUCCESS*/0x00 == mPrizeResult ||
		   /*BPR_ALREADY_GET*/0x02 == mPrizeResult)
		{
			iStream.Read((CHAR*)&mPrizeNum,sizeof(BYTE));
			if(mPrizeNum>MAX_PRIZE_NUMBER) mPrizeNum = MAX_PRIZE_NUMBER;
			
			for(INT i = 0; i < mPrizeNum; ++i)
			{
				iStream.Read(mPrize[i].m_PrizeString,MAX_PRIZE_STRING*sizeof(CHAR));
				iStream.Read((CHAR*)&(mPrize[i].m_PrizeNum),sizeof(BYTE));
			}
		}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetPrize::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetPrize::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetPrizeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}