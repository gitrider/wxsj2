/********************************************************************
	created:	2007/11/27
	created:	27:11:2007   10:37
	filename: 	d:\project\projectNew\Common\Packets\GCUseBallImpact.cpp
	file path:	d:\project\projectNew\Common\Packets
	file base:	GCUseBallImpact
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include  "GCUseBallImpact.h"


BOOL	GCUseBallImpact::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_nSkillID, sizeof(SkillID_t) );
	iStream.Read( (CHAR*)&m_uBallNum, sizeof(UINT) );
	iStream.Read( (CHAR*)&m_nCharacterID, sizeof(UINT) );


	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCUseBallImpact::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR*)&m_nSkillID, sizeof(SkillID_t) );
	oStream.Write( (CHAR*)&m_uBallNum, sizeof(UINT) );
	oStream.Write( (CHAR*)&m_nCharacterID, sizeof(UINT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCUseBallImpact::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCUseBallImpactHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

