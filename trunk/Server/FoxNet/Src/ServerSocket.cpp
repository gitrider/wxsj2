

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "ServerSocket.h"


ServerSocket::ServerSocket ( UINT port , UINT backlog ) 
{
	__ENTER_FUNCTION_FOXNET

	BOOL ret = FALSE ;

#ifdef __WINDOWS__
	//if( !CheckIt() )
	//	throw 1 ;
#else
#endif

		
	// create socket implementation object
	m_Socket = new Socket;
//	Assert( m_Socket ) ;
	if( m_Socket==NULL )
		throw 1 ;
	
	// create socket
	ret = m_Socket->create( ) ;
	if( ret==FALSE )
		throw 1 ;
//	Assert( ret ) ;
	
	// reuse address before Bind()
	// 바인드 하기 전에 주소 리유즈를 시스템에 알려야 한다.
	ret = m_Socket->setReuseAddr( ) ;
	if( ret==FALSE )
		throw 1 ;
//	Assert( ret ) ;
	
	// bind address to socket
	// 이미 port가 m_Impl에 저장되어 있으므로, 파라미터없는 Bind()를 호출해도 된다.
	ret = m_Socket->bind( port ) ;
	if( ret==FALSE )
		throw 1 ;
//	Assert( ret ) ;

//	m_Impl->setSendBufferSize( 9000 );
//	m_Impl->setReceiveBufferSize( 9000 );
	
	// set listening queue size
	ret = m_Socket->listen( backlog ) ;
	if( ret==FALSE )
		throw 1 ;
//	Assert( ret ) ;

	__LEAVE_FUNCTION_FOXNET

}

ServerSocket::~ServerSocket () 
{
	__ENTER_FUNCTION_FOXNET
		
	if ( m_Socket != NULL ) 
	{
		m_Socket->close();
		delete m_Socket;
		m_Socket = NULL;
	}

	__LEAVE_FUNCTION_FOXNET
}
	
VOID ServerSocket::close ()
{
	__ENTER_FUNCTION_FOXNET
		
	if( m_Socket )
	{
		m_Socket->close( ) ;
	}

	__LEAVE_FUNCTION_FOXNET
}

BOOL ServerSocket::accept ( Socket* socket ) 
{
	__ENTER_FUNCTION_FOXNET
	
	UINT addrlen = sizeof(SOCKADDR_IN) ;
	socket->close( ) ;

	socket->m_SocketID = m_Socket->accept( (struct sockaddr *)(&(socket->m_SockAddr)), &addrlen ) ;
	if( socket->m_SocketID == INVALID_SOCKET )
		return FALSE ;

	socket->m_Port = ntohs( socket->m_SockAddr.sin_port ) ;
	strncpy( socket->m_Host, inet_ntoa(socket->m_SockAddr.sin_addr), IP_SIZE-1 ) ;

	return TRUE;

	__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}










