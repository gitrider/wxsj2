

#include "stdafx.h"
#include "GCChat.h"



BOOL GCChat::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

 	iStream.Read( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;

	iStream.Read( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	if( m_ContexSize>0 && m_ContexSize<=MAX_CHAT_SIZE )
		iStream.Read( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	

	if(CHAT_TYPE_NEAR == m_ChatType){
		iStream.Read( (CHAR*)(&m_SourID), sizeof(ObjID_t) ) ;
	}
	else if(CHAT_TYPE_DMA_FORCE == m_ChatType ||
		CHAT_TYPE_DMA_REQUEST == m_ChatType ||
		CHAT_TYPE_DMA_AGREE== m_ChatType ||
		CHAT_TYPE_DMA_REFUSE== m_ChatType ||
		CHAT_TYPE_DMA_CANCEL == m_ChatType)
	{
		iStream.Read( (CHAR*)(&m_SourID), sizeof(ObjID_t) );
		iStream.Read( (CHAR*)(&m_DestID), sizeof(ObjID_t) );

		return TRUE;
	}

	iStream.Read( (CHAR*)(&m_SourNameSize), sizeof(BYTE) ) ;
	if( m_SourNameSize>0 && m_SourNameSize<=MAX_CHARACTER_NAME )
		iStream.Read( (CHAR*)(&m_SourName), sizeof(CHAR)*m_SourNameSize ) ;

	iStream.Read( (CHAR*)(&m_uWorldChatID), sizeof(UINT) ) ;
	iStream.Read( (CHAR*)(&m_ChatShowPos), sizeof(BYTE) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCChat::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;

	oStream.Write( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	if( m_ContexSize>0 && m_ContexSize<=MAX_CHAT_SIZE )
		oStream.Write( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	
	if(CHAT_TYPE_NEAR == m_ChatType){
		oStream.Write( (CHAR*)(&m_SourID), sizeof(ObjID_t) ) ;
	}
	else if(CHAT_TYPE_DMA_FORCE == m_ChatType ||
		 CHAT_TYPE_DMA_REQUEST == m_ChatType ||
		 CHAT_TYPE_DMA_AGREE== m_ChatType ||
		 CHAT_TYPE_DMA_REFUSE== m_ChatType ||
		 CHAT_TYPE_DMA_CANCEL == m_ChatType)
	{
		oStream.Write( (CHAR*)(&m_SourID), sizeof(ObjID_t) );
		oStream.Write( (CHAR*)(&m_DestID), sizeof(ObjID_t) );

		return TRUE;
	}

	oStream.Write( (CHAR*)(&m_SourNameSize), sizeof(BYTE) ) ;
	if( m_SourNameSize>0 && m_SourNameSize<=MAX_CHARACTER_NAME )
		oStream.Write( (CHAR*)(&m_SourName), sizeof(CHAR)*m_SourNameSize ) ;

	oStream.Write( (CHAR*)(&m_uWorldChatID), sizeof(UINT) ) ;
	oStream.Write( (CHAR*)(&m_ChatShowPos), sizeof(BYTE) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCChat::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCChatHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


