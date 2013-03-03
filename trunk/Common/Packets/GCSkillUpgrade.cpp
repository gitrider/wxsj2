/********************************************************************
	created:	2008/01/15
	created:	15:1:2008   14:05
	filename: 	Common\Packets\GCSkillUpgrade.cpp
	file path:	Common\Packets
	file base:	GCSkillUpgrade
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "GCSkillUpgrade.h"

GCSkillUpgrade::GCSkillUpgrade()
	:m_iSkillID(-1),
	 m_iLevel(-1),
	 m_bSuccess(FALSE)
{

}

BOOL GCSkillUpgrade::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_iSkillID), sizeof(INT));
	iStream.Read((CHAR*)(&m_iLevel),sizeof(INT));
	iStream.Read((CHAR*)(&m_bSuccess),sizeof(BOOL));
	
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCSkillUpgrade::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_iSkillID), sizeof(INT));
	oStream.Write((CHAR*)(&m_iLevel),sizeof(INT));
	oStream.Write((CHAR*)(&m_bSuccess),sizeof(BOOL));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCSkillUpgrade::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCSkillUpgradeHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}

INT Packets::GCSkillUpgrade::GetSkillID() const
{
	__ENTER_FUNCTION
	return m_iSkillID;
	__LEAVE_FUNCTION
	return -1;
}

VOID Packets::GCSkillUpgrade::SetSkillID( INT val )
{
	__ENTER_FUNCTION
	m_iSkillID = val;
	__LEAVE_FUNCTION
}

INT Packets::GCSkillUpgrade::GetLevel() const
{
	__ENTER_FUNCTION
	return m_iLevel;
	__LEAVE_FUNCTION
	return -1;
}

VOID Packets::GCSkillUpgrade::SetLevel( INT val )
{
	__ENTER_FUNCTION
	m_iLevel = val;
	__LEAVE_FUNCTION
}

BOOL Packets::GCSkillUpgrade::IsSuccessful()
{
	return m_bSuccess;
}

VOID Packets::GCSkillUpgrade::SetSuccessful( BOOL val )
{
	m_bSuccess = val;
}