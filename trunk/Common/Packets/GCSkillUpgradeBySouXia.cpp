/********************************************************************
	created:	2010/05/10
	filename: 	Common\Packets\GCSkillUpgradeBySouXia.cpp
	file path:	Common\Packets
	file base:	GCSkillUpgradeBySouXia
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "GCSkillUpgradeBySouXia.h"

GCAddSouXiaSkill::GCAddSouXiaSkill()
	:m_nStudySkillType(0),m_nSkillID(0),m_nCurSouXiaPos(-1),m_nCurUseTime(0)
{

}

BOOL GCAddSouXiaSkill::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nCurSouXiaPos), sizeof(SHORT));
	iStream.Read( (CHAR*)(&m_nStudySkillType), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_nSkillID),sizeof(SkillID_t));
	iStream.Read((CHAR*)(&m_nCurUseTime),sizeof(SHORT));
	iStream.Read((CHAR*)(&m_nResult),sizeof(BYTE));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCAddSouXiaSkill::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_nCurSouXiaPos), sizeof(SHORT));
	oStream.Write((CHAR*)(&m_nStudySkillType),sizeof(BYTE));
	oStream.Write((CHAR*)(&m_nSkillID),sizeof(SkillID_t));
	oStream.Write((CHAR*)(&m_nCurUseTime),sizeof(SHORT));
	oStream.Write((CHAR*)(&m_nResult),sizeof(BYTE));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCAddSouXiaSkill::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCAddSouXiaSkillHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}

SkillID_t Packets::GCAddSouXiaSkill::GetSkillID() const
{
	__ENTER_FUNCTION
	return m_nSkillID;
	__LEAVE_FUNCTION
	return -1;
}

VOID Packets::GCAddSouXiaSkill::SetSkillID( SkillID_t val )
{
	__ENTER_FUNCTION
	m_nSkillID = val;
	__LEAVE_FUNCTION
}


