#include "stdafx.h"
#include "GCSouXiaSkillUpdate.h"

using namespace Packets;

BOOL GCSouXiaSkillUpdate::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_nCurSouXiaPos), sizeof(SHORT));
	iStream.Read((CHAR*)(&m_nSkillType), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_nSkillId), sizeof(SkillID_t));
	iStream.Read((CHAR*)(&m_nLeftTime), sizeof(SHORT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCSouXiaSkillUpdate::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_nCurSouXiaPos), sizeof(SHORT));
	oStream.Write((CHAR*)(&m_nSkillType), sizeof(BYTE));
	oStream.Write((CHAR*)(&m_nSkillId), sizeof(SkillID_t));
	oStream.Write((CHAR*)(&m_nLeftTime), sizeof(SHORT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCSouXiaSkillUpdate::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCSouXiaSkillUpdateHandler::Execute( this, pPlayer );  //¿Í»§¶ËÐÞ¸Ä
	__LEAVE_FUNCTION

		return FALSE ;
}

