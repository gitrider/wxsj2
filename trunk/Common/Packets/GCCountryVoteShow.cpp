#include "stdafx.h"
#include "GCCountryVoteShow.h"

BOOL GCCountryVoteShow::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read( (CHAR*)(&m_nCurVotes), sizeof(m_nCurVotes) );
	iStream.Read( (CHAR*)(&m_fPercentVote), sizeof(m_fPercentVote) );
    iStream.Read( (CHAR*)(&m_nLeftTime), sizeof(m_nLeftTime) );
   
	 return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GCCountryVoteShow::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

    oStream.Write( (CHAR*)(&m_nCurVotes), sizeof(m_nCurVotes) );
    oStream.Write( (CHAR*)(&m_fPercentVote), sizeof(m_fPercentVote) );
	oStream.Write( (CHAR*)(&m_nLeftTime), sizeof(m_nLeftTime) );
   
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GCCountryVoteShow::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GCCountryVoteShowHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
