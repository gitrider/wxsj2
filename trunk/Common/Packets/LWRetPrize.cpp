#include "stdafx.h" 
#include "LWRetPrize.h"


BOOL LWRetPrize::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mPrizeUserID,sizeof(ID_t));
		iStream.Read((CHAR*)&mAccName,sizeof(CHAR)*MAX_ACCOUNT);
		iStream.Read((CHAR*)&mPrizeResult,sizeof(BYTE));
		iStream.Read((CHAR*)&mRetType,sizeof(BYTE));
        if( (ASK_PRIZE == mRetType && BPR_SUCCESS == mPrizeResult) ||
            (ASK_NEWUSER_CARD == mRetType && BNCR_SUCCESS == mPrizeResult) ||
			(ASK_NEWPRIZE == mRetType && BNPR_SUCCESS == mPrizeResult) ||
			(ASK_NEWCHECKPRIZE == mRetType && BNCPR_SUCCESS == mPrizeResult))
		{
			iStream.Read((CHAR*)&mPrizeNum,sizeof(USHORT));
			Assert(mPrizeNum > 0 && mPrizeNum <=MAX_PRIZE_NUMBER);
			iStream.Read((CHAR*)mPrize,sizeof(_BUY_DATA)*mPrizeNum);
		}
		else if(ASK_BUY == mRetType && BABR_SUCCESS == mPrizeResult)
		{
			iStream.Read((CHAR*)&mLeftPoint,sizeof(UINT));
			iStream.Read((CHAR*)&mPrizeNum,sizeof(USHORT));
			Assert(mPrizeNum > 0 && mPrizeNum <=MAX_PRIZE_NUMBER);
			iStream.Read((CHAR*)mPrize,sizeof(_BUY_DATA)*mPrizeNum);
		}
        else if(ASK_POINT == mRetType && BAPR_SUCCESS == mPrizeResult)
		{
			iStream.Read((CHAR*)&mLeftPoint,sizeof(UINT));
		}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LWRetPrize::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&mPrizeUserID,sizeof(ID_t));
		oStream.Write((CHAR*)&mAccName,sizeof(CHAR)*MAX_ACCOUNT);
		oStream.Write((CHAR*)&mPrizeResult,sizeof(BYTE));
		oStream.Write((CHAR*)&mRetType,sizeof(BYTE));
		if( (ASK_PRIZE == mRetType && BPR_SUCCESS == mPrizeResult) ||
			(ASK_NEWUSER_CARD == mRetType && BNCR_SUCCESS == mPrizeResult) ||
			(ASK_NEWPRIZE == mRetType && BNPR_SUCCESS == mPrizeResult) ||
			(ASK_NEWCHECKPRIZE == mRetType && BNCPR_SUCCESS == mPrizeResult))
		{
			oStream.Write((CHAR*)&mPrizeNum,sizeof(USHORT));
			Assert(mPrizeNum > 0 && mPrizeNum <=MAX_PRIZE_NUMBER);
			oStream.Write((CHAR*)mPrize,sizeof(_BUY_DATA)*mPrizeNum);
		}
		else if(ASK_BUY == mRetType && BABR_SUCCESS == mPrizeResult)
		{
			oStream.Write((CHAR*)&mLeftPoint,sizeof(UINT));
			oStream.Write((CHAR*)&mPrizeNum,sizeof(USHORT));
			Assert(mPrizeNum > 0 && mPrizeNum <=MAX_PRIZE_NUMBER);
			oStream.Write((CHAR*)mPrize,sizeof(_BUY_DATA)*mPrizeNum);
		}
		else if(ASK_POINT == mRetType && BAPR_SUCCESS == mPrizeResult)
		{
			oStream.Write((CHAR*)&mLeftPoint,sizeof(UINT));
		}

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LWRetPrize::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	LWRetPrizeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

