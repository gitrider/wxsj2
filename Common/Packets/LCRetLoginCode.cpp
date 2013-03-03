#include "stdafx.h" 
#include "LCRetLoginCode.h"

BOOL LCRetLoginCode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_RetType,sizeof(BYTE));
	    if(RETLOGINCODE_IMAGE==m_RetType)
		{
			iStream.Read((CHAR*)m_imageBuf,sizeof(UCHAR)*IMAGE_BUF_LENGTH_1);
		}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LCRetLoginCode::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_RetType,sizeof(BYTE));
		if(RETLOGINCODE_IMAGE==m_RetType)
		{
			oStream.Write((CHAR*)m_imageBuf,sizeof(UCHAR)*IMAGE_BUF_LENGTH_1);
		}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LCRetLoginCode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LCRetLoginCodeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
