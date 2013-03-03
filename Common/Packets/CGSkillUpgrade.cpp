/********************************************************************
	created:	2008/01/14
	created:	14:1:2008   11:56
	filename: 	Common\Packets\CGSkillUpgrade.cpp
	file path:	Common\Packets
	file base:	CGSkillUpgrade
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "CGSkillUpgrade.h"


Packets::CGSkillUpgrade::CGSkillUpgrade()
	:m_iSkillID(-1),
	 m_iLevel(-1)
{

}

BOOL Packets::CGSkillUpgrade::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_iSkillID), sizeof(INT) ) ;
	iStream.Read( (CHAR*)(&m_iLevel), sizeof(INT) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Packets::CGSkillUpgrade::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_iSkillID), sizeof(INT) ) ;
	oStream.Write( (CHAR*)(&m_iLevel), sizeof(INT) ) ;
	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

UINT Packets::CGSkillUpgrade::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CGSkillUpgradeHandler::Execute( this, pPlayer );

	__LEAVE_FUNCTION
	return FALSE ;
}

INT Packets::CGSkillUpgrade::GetSkillID() const
{
	__ENTER_FUNCTION

	return m_iSkillID;
	__LEAVE_FUNCTION
	return 0;
}

VOID Packets::CGSkillUpgrade::SetSkillID( INT val )
{
	__ENTER_FUNCTION
	m_iSkillID = val;
	__LEAVE_FUNCTION

}

INT Packets::CGSkillUpgrade::GetLevel() const
{
	__ENTER_FUNCTION

	return m_iLevel;
	__LEAVE_FUNCTION

}

VOID Packets::CGSkillUpgrade::SetLevel( INT val )
{
	__ENTER_FUNCTION

	m_iLevel = val;
	__LEAVE_FUNCTION

}