

#include "stdafx.h"
#include "WGChat.h"



BOOL WGChat::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	Reset() ;

 	iStream.Read( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;

	iStream.Read( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	if( m_ContexSize>0 && m_ContexSize<=MAX_CHAT_SIZE )
		iStream.Read( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	m_Contex[MAX_CHAT_SIZE-1]=0;
	
	iStream.Read( (CHAR*)(&m_SourNameSize), sizeof(BYTE) ) ;
	if( m_SourNameSize>0 && m_SourNameSize<=MAX_CHARACTER_NAME )
		iStream.Read( (CHAR*)(&m_SourName), sizeof(CHAR)*m_SourNameSize ) ;
	m_SourName[MAX_CHARACTER_NAME-1]=0;

	if( m_ChatType == CHAT_TYPE_SYSTEM ||
		m_ChatType == CHAT_TYPE_WORLD )
	{
	}
	else if(	m_ChatType == CHAT_TYPE_TEAM || 
				m_ChatType == CHAT_TYPE_USER ||
				m_ChatType == CHAT_TYPE_SELF
				)
	{
	 	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	}
	else if(	m_ChatType == CHAT_TYPE_GUILD )
	{
	 	iStream.Read( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
	}
	else if(	m_ChatType == CHAT_TYPE_JIAZU )
	{
	 	iStream.Read( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
	 	iStream.Read( (CHAR*)(&m_JiaZuID), sizeof(JiaZuID_t) ) ;
	}
	else if(	m_ChatType == CHAT_TYPE_COUNTRY )
	{
	 	iStream.Read( (CHAR*)(&m_CountryID), sizeof(CountryID_t) ) ;
	}
	else
	{
		Assert(FALSE) ;
	}
	iStream.Read( (CHAR*)(&m_uWorldChatID), sizeof(UINT) ) ;
	iStream.Read( (CHAR*)(&m_ChatShowPos), sizeof(BYTE) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGChat::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;

	oStream.Write( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	if( m_ContexSize>0 && m_ContexSize<=MAX_CHAT_SIZE )
		oStream.Write( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	
	oStream.Write( (CHAR*)(&m_SourNameSize), sizeof(BYTE) ) ;
	if( m_SourNameSize>0 && m_SourNameSize<=MAX_CHARACTER_NAME )
		oStream.Write( (CHAR*)(&m_SourName), sizeof(CHAR)*m_SourNameSize ) ;

	if( m_ChatType == CHAT_TYPE_SYSTEM ||
		m_ChatType == CHAT_TYPE_WORLD )
	{
	}
	else if(	//m_ChatType == CHAT_TYPE_CHANNEL || 
				m_ChatType == CHAT_TYPE_TEAM || 
				m_ChatType == CHAT_TYPE_USER ||
				m_ChatType == CHAT_TYPE_SELF
				)
	{
	 	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	}
	else if( m_ChatType == CHAT_TYPE_GUILD )
	{
	 	oStream.Write( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
	}
	else if( m_ChatType == CHAT_TYPE_JIAZU )
	{
	 	oStream.Write( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
	 	oStream.Write( (CHAR*)(&m_JiaZuID), sizeof(JiaZuID_t) ) ;
	}
	else if( m_ChatType == CHAT_TYPE_COUNTRY )
	{
	 	oStream.Write( (CHAR*)(&m_CountryID), sizeof(CountryID_t) ) ;
	}
	else
	{
		Assert(FALSE) ;
	}
	oStream.Write( (CHAR*)(&m_uWorldChatID), sizeof(UINT) ) ;
	oStream.Write( (CHAR*)(&m_ChatShowPos), sizeof(BYTE) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGChat::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGChatHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


