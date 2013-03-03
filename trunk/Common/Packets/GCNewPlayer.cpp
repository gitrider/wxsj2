
#include "stdafx.h"
#include "GCNewPlayer.h"

BOOL GCNewPlayer::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_fDir), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_wEquipVer), sizeof(WORD));
	iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewPlayer::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_wEquipVer), sizeof(WORD));
	oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewPlayer::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewPlayerHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

