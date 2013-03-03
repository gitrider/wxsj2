

#include "stdafx.h"
#include "CGSkill.h"



BOOL CGSkill::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_SkillID), sizeof(SkillID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGSkill::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_SkillID), sizeof(SkillID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGSkill::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGSkillHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


