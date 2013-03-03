

#include "stdafx.h"
#include "CGChat.h"



BOOL CGChat::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	m_ChatType = 0 ;
	m_ContexSize = 0 ;
	memset( m_Contex, 0, MAX_CHAT_SIZE ) ;
	m_TargetSize = 0 ;
	memset( m_TargetName, 0, MAX_CHARACTER_NAME ) ;
	m_TeamID = INVALID_ID ;
	//m_ChannelID = INVALID_ID ;
	m_GuildID = INVALID_ID ;
	m_JiaZuID = INVALID_ID;
	m_CountryID = INVALID_ID;
	//m_MenpaiID = MATTRIBUTE_WUMENPAI ;


	iStream.Read( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	Assert( m_ContexSize<=MAX_CHAT_SIZE ) ;
	iStream.Read( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	m_Contex[MAX_CHAT_SIZE-1]=0;

	switch( m_ChatType )
	{
	case CHAT_TYPE_NEAR:
	case CHAT_TYPE_SYSTEM:
	case CHAT_TYPE_WORLD:
		break ;
	case CHAT_TYPE_TEAM:
		iStream.Read( (CHAR*)(&m_TeamID), sizeof(TeamID_t) ) ;
		break ;
	case CHAT_TYPE_USER:
		iStream.Read( (CHAR*)(&m_TargetSize), sizeof(BYTE) ) ;
		Assert( m_TargetSize<=MAX_CHARACTER_NAME ) ;
		iStream.Read( (CHAR*)(&m_TargetName), m_TargetSize*sizeof(CHAR) ) ;
		m_TargetName[MAX_CHARACTER_NAME-1]=0;
		break ;
	case CHAT_TYPE_GUILD:
		iStream.Read( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
		break ;
	case CHAT_TYPE_JIAZU:
		iStream.Read( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
		iStream.Read( (CHAR*)(&m_JiaZuID), sizeof(JiaZuID_t) ) ;
		break ;
	case CHAT_TYPE_COUNTRY:
		iStream.Read( (CHAR*)(&m_CountryID), sizeof(CountryID_t) ) ;
		break;
	case CHAT_TYPE_DMA_AGREE:
	case CHAT_TYPE_DMA_REQUEST:
	case CHAT_TYPE_DMA_CANCEL:
	case CHAT_TYPE_DMA_FORCE:
	case CHAT_TYPE_DMA_REFUSE:
		iStream.Read( (CHAR*)(&m_DestID), sizeof(m_DestID) ) ;
		break;
	default :
		Assert(FALSE) ;
		break ;
	}


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGChat::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	Assert( m_ContexSize<=MAX_CHAT_SIZE ) ;
	oStream.Write( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	
	switch( m_ChatType )
	{
	case CHAT_TYPE_NEAR:
	case CHAT_TYPE_SYSTEM:
	case CHAT_TYPE_WORLD:
		break ;
	case CHAT_TYPE_TEAM:
		oStream.Write( (CHAR*)(&m_TeamID), sizeof(TeamID_t) ) ;
		break ;
	case CHAT_TYPE_USER:
		oStream.Write( (CHAR*)(&m_TargetSize), sizeof(BYTE) ) ;
		Assert( m_TargetSize<=MAX_CHARACTER_NAME ) ;
		oStream.Write( (CHAR*)(&m_TargetName), m_TargetSize*sizeof(CHAR) ) ;
		break ;
	case CHAT_TYPE_GUILD:
		oStream.Write( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
		break ;
	case CHAT_TYPE_JIAZU:
		oStream.Write( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
		oStream.Write( (CHAR*)(&m_JiaZuID), sizeof(JiaZuID_t) ) ;
		break ;
	case CHAT_TYPE_COUNTRY:
		oStream.Write( (CHAR*)(&m_CountryID), sizeof(CountryID_t) ) ;
		break;
	case CHAT_TYPE_DMA_AGREE:
	case CHAT_TYPE_DMA_REQUEST:
	case CHAT_TYPE_DMA_CANCEL:
	case CHAT_TYPE_DMA_FORCE:
	case CHAT_TYPE_DMA_REFUSE:
		oStream.Write( (CHAR*)(&m_DestID), sizeof(m_DestID) ) ;
		break;
	default :
		Assert(FALSE) ;
		break ;
	}


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGChat::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGChatHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


