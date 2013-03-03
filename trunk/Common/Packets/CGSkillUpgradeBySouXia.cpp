/********************************************************************
	created:	2008/01/14
	created:	14:1:2008   11:56
	filename: 	Common\Packets\CGSkillUpgradeBySouXia.cpp
	file path:	Common\Packets
	file base:	CGSkillUpgrade
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "CGSkillUpgradeBySouXia.h"


Packets::CGSkillUpgradeBySouXia::CGSkillUpgradeBySouXia()
	:m_iStudySkillType(-1),m_iBagIndex(-1)
{

}

BOOL Packets::CGSkillUpgradeBySouXia::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_iStudySkillType), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_iBagIndex), sizeof(BYTE) ) ;
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Packets::CGSkillUpgradeBySouXia::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_iStudySkillType), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_iBagIndex), sizeof(BYTE) ) ;
	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

UINT Packets::CGSkillUpgradeBySouXia::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CGSkillUpgradeBySouXiaHandler::Execute( this, pPlayer );

	__LEAVE_FUNCTION
	return FALSE ;
}

