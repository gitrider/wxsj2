// CGCharCancelSkill.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGCharCancelSkill.h"
namespace Packets
{
	BOOL CGCharCancelSkill::Read( SocketInputStream& iStream ) 
	{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	BOOL CGCharCancelSkill::Write( SocketOutputStream& oStream )const
	{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	UINT CGCharCancelSkill::Execute( Player* pPlayer )
	{
	__ENTER_FUNCTION

		return CGCharCancelSkillHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
	}
}
