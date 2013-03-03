
#include "stdafx.h"
#include "SSScenePlayerCount.h"



BOOL SSScenePlayerCount::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_nChangeCount,sizeof(SHORT));
	if( m_nChangeCount>0 )
	{
		if( m_nChangeCount<=MAX_SCENE )
		{
		iStream.Read((CHAR*)&m_aChangeSceneID,sizeof(SceneID_t)*m_nChangeCount);
		iStream.Read((CHAR*)&m_aChangeScenePlayerCount,sizeof(SHORT)*m_nChangeCount);
	}
	iStream.Read((CHAR*)&m_nTotalPlayerCount,sizeof(SHORT));
	}
	else
	{
		iStream.Read((CHAR*)&m_ServerID,sizeof(ID_t));
		iStream.Read((CHAR*)&m_PlayerPoolSize,sizeof(INT));
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SSScenePlayerCount::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_nChangeCount,sizeof(SHORT));
	if( m_nChangeCount>0 )
	{
		if( m_nChangeCount<=MAX_SCENE )
		{
		oStream.Write((CHAR*)&m_aChangeSceneID,sizeof(SceneID_t)*m_nChangeCount);
		oStream.Write((CHAR*)&m_aChangeScenePlayerCount,sizeof(SHORT)*m_nChangeCount);
	}
	oStream.Write((CHAR*)&m_nTotalPlayerCount,sizeof(SHORT));
	}
	else
	{
		oStream.Write((CHAR*)&m_ServerID,sizeof(ID_t));
		oStream.Write((CHAR*)&m_PlayerPoolSize,sizeof(INT));
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT SSScenePlayerCount::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return SSScenePlayerCountHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}


