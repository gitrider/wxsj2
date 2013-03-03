// GCCharSkill_Lead.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCCharSkill_Lead.h"

BOOL GCCharSkill_Lead::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	iStream.Read( (CHAR*)(&m_SkillDataID), sizeof(m_SkillDataID));
	iStream.Read( (CHAR*)(&m_posUser), sizeof(m_posUser));
	iStream.Read( (CHAR*)(&m_TargetID), sizeof(m_TargetID));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	iStream.Read( (CHAR*)(&m_fDir), sizeof(m_fDir));
	iStream.Read( (CHAR*)(&m_nTotalTime), sizeof(m_nTotalTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharSkill_Lead::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	oStream.Write( (CHAR*)(&m_SkillDataID), sizeof(m_SkillDataID));
	oStream.Write( (CHAR*)(&m_posUser), sizeof(m_posUser));
	oStream.Write( (CHAR*)(&m_TargetID), sizeof(m_TargetID));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(m_fDir));
	oStream.Write( (CHAR*)(&m_nTotalTime), sizeof(m_nTotalTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharSkill_Lead::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharSkill_LeadHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

