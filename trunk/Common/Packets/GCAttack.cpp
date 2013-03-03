

#include "stdafx.h"
#include "GCAttack.h"



BOOL GCAttack::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_uExp), sizeof(UINT) ) ;
	
	iStream.Read( (CHAR*)(&m_byListNum), sizeof(BYTE) ) ;
	if(m_byListNum > MAX_DAM_LIST_NUM) m_byListNum = MAX_DAM_LIST_NUM;

	iStream.Read( (CHAR*)(m_listDam), sizeof(_DAMAGE_INFO)*m_byListNum ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCAttack::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_uExp), sizeof(UINT) ) ;

	oStream.Write( (CHAR*)(&m_byListNum), sizeof(BYTE) ) ;

	oStream.Write( (CHAR*)(m_listDam), sizeof(_DAMAGE_INFO)*(m_byListNum>MAX_DAM_LIST_NUM ? MAX_DAM_LIST_NUM : m_byListNum ) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCAttack::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCAttackHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


