
#include "stdafx.h"
#include "GCCharUseSkill.h"

namespace Packets
{
	BOOL GCCharUseSkill::Read( SocketInputStream& iStream ) 
	{
		__ENTER_FUNCTION
		
		iStream.Read((CHAR *)(&m_bIsMySelf), sizeof(m_bIsMySelf));	
		iStream.Read((CHAR *)(&m_nSenderID), sizeof(m_nSenderID));
		iStream.Read((CHAR *)(&m_nSkillID), sizeof(m_nSkillID));

		return TRUE ;
		__LEAVE_FUNCTION
			return FALSE ;
	}

	BOOL GCCharUseSkill::Write( SocketOutputStream& oStream )const
	{
		__ENTER_FUNCTION
	
		oStream.Write((CHAR const*)(&m_bIsMySelf), sizeof(m_bIsMySelf));	
		oStream.Write((CHAR const*)(&m_nSenderID), sizeof(m_nSenderID));
		oStream.Write((CHAR const*)(&m_nSkillID), sizeof(m_nSkillID));		

		return TRUE ;
		__LEAVE_FUNCTION
			return FALSE ;
	}

	UINT GCCharUseSkill::Execute( Player* pPlayer )
	{
		__ENTER_FUNCTION
			return GCCharUseSkillHandler::Execute( this, pPlayer ) ;
		__LEAVE_FUNCTION
			return FALSE ;
	}

}
