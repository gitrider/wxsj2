

#include "SocketInputStream.h"
#include "Packet.h"
#include "EncoderManager.h"

using namespace Encode_Decode;

SocketInputStream::SocketInputStream( Socket* sock, UINT BufferLen, UINT MaxBufferLen ) 
{
__ENTER_FUNCTION_FOXNET

	m_pSocket = sock ;
	m_BufferLen = BufferLen ;
	m_MaxBufferLen = MaxBufferLen ;
		
//	Assert( m_pSocket != NULL );
//	Assert( m_BufferLen > 0 );

	m_Head = 0 ;
	m_Tail = 0 ;
	m_nEncoderID = INVALID_ID;
	m_nEncoderParam0 = 0;
	m_nEncoderParam1 = 0;
	m_pKey = NULL;
	m_pEncoder = NULL;
	m_Buffer = new CHAR[ m_BufferLen ];
//	Assert( m_Buffer ) ;

	memset( m_Buffer, 0, m_BufferLen ) ;

__LEAVE_FUNCTION_FOXNET
}

SocketInputStream::~SocketInputStream( ) 
{
__ENTER_FUNCTION_FOXNET
		
	SAFE_DELETE_ARRAY(m_Buffer);
		
__LEAVE_FUNCTION_FOXNET
}

UINT SocketInputStream::Length( )const
{
__ENTER_FUNCTION_FOXNET

	if( m_Head<m_Tail )
		return m_Tail-m_Head;
	
	else if( m_Head>m_Tail ) 
		return m_BufferLen-m_Head+m_Tail ;
	
	return 0 ;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}

#define _NOT_USE_DECODE_
#ifdef _NOT_USE_DECODE_
//����0��ʾû�ж�������
UINT SocketInputStream::Read( CHAR* buf, UINT len ) 
{
__ENTER_FUNCTION_FOXNET

//	Assert( buf != NULL );	
	
	if ( len == 0 )
		return 0 ;
		
	if ( len > Length() )
		return 0 ;
	
	if ( m_Head < m_Tail ) 
	{
		memcpy( buf, &m_Buffer[m_Head], len ) ;
	} 
	else 
	{
		UINT rightLen = m_BufferLen-m_Head ;
		if( len<=rightLen ) 
		{
			memcpy( buf, &m_Buffer[m_Head], len ) ;
		} 
		else 
		{
			memcpy( buf, &m_Buffer[m_Head], rightLen ) ;
			memcpy( &buf[rightLen], m_Buffer, len-rightLen ) ;
		}
	}

	m_Head = (m_Head+len)%m_BufferLen ;
	
	return len ;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}

BOOL SocketInputStream::Peek( CHAR* buf, UINT len ) 
{
__ENTER_FUNCTION_FOXNET
			
//	Assert( buf!=NULL ) ;	

	if( len==0 )
		return FALSE ;
	
	if( len>Length() )
		return FALSE ;

	if( m_Head<m_Tail ) 
	{
		memcpy( buf , &m_Buffer[m_Head] , len );

	} 
	else 
	{
		UINT rightLen = m_BufferLen-m_Head ;
		if( len<=rightLen ) 
		{
			memcpy( &buf[0], &m_Buffer[m_Head], len ) ;
		} 
		else 
		{
			memcpy( &buf[0], &m_Buffer[m_Head], rightLen ) ;
			memcpy( &buf[rightLen], &m_Buffer[0], len-rightLen ) ;
		}
	}
		
	return TRUE ;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}

BOOL SocketInputStream::Skip( UINT len ) 
{
__ENTER_FUNCTION_FOXNET
		
	if( len == 0 )
		return FALSE ;

	if( len>Length( ) )
		return FALSE ;
	
	m_Head = (m_Head+len)%m_BufferLen ;

	return TRUE ;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}
#else
//����0��ʾû�ж�������
UINT SocketInputStream::Read( CHAR* buf, UINT len ) 
{
__ENTER_FUNCTION_FOXNET

	UCHAR* pBuffer = NULL;
//	Assert( buf != NULL );	
	
	if ( len == 0 )
		return 0 ;
		
	if ( len > Length() )
		return 0 ;
	
	pBuffer = new UCHAR[len];
	if(NULL==pBuffer)
	{
		return 0;
	}
	
	if ( m_Head < m_Tail ) 
	{
		memcpy( pBuffer, &m_Buffer[m_Head], len ) ;
	} 
	else 
	{
		UINT rightLen = m_BufferLen-m_Head ;
		if( len<=rightLen ) 
		{
			memcpy( pBuffer, &m_Buffer[m_Head], len ) ;
		} 
		else 
		{
			memcpy( pBuffer, &m_Buffer[m_Head], rightLen ) ;
			memcpy( &pBuffer[rightLen], m_Buffer, len-rightLen ) ;
		}
	}

	m_Head = (m_Head+len)%m_BufferLen ;
	if(NULL!=m_pEncoder && NULL!=m_pKey && 0<m_nKeySize)
	{
		Encoder_T::Parameters_T params;

		params.SetPlainText((UCHAR*)buf);
		params.SetPlainTextSize(len);
		params.SetCripher((UCHAR*)pBuffer);
		params.SetCripherSize(len);
		params.SetKey(m_pKey);
		params.SetKeySize(m_nKeySize);
		params.SetParam0(m_nEncoderParam0);
		params.SetParam1(m_nEncoderParam1);

		BOOL bRet = m_pEncoder->Decode(params);

		m_nEncoderParam0 = params.GetParam0();
		m_nEncoderParam1 = params.GetParam1();

		if(FALSE==bRet)
		{
			len = 0;
		}
	}
	else
	{
		memcpy( buf, pBuffer, len ) ;
	}
	SAFE_DELETE_ARRAY(pBuffer);
	return len ;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}

BOOL SocketInputStream::Peek( CHAR* buf, UINT len ) 
{
__ENTER_FUNCTION_FOXNET
			
	UCHAR* pBuffer = NULL;
//	Assert( buf!=NULL ) ;	
	if( len==0 )
		return FALSE ;
	
	if( len>Length() )
		return FALSE ;

	pBuffer = new UCHAR[len];
	if(NULL==pBuffer)
	{
		return 0;
	}

	if( m_Head<m_Tail ) 
	{
		memcpy( pBuffer , &m_Buffer[m_Head] , len );

	} 
	else 
	{
		UINT rightLen = m_BufferLen-m_Head ;
		if( len<=rightLen ) 
		{
			memcpy( &pBuffer[0], &m_Buffer[m_Head], len ) ;
		} 
		else 
		{
			memcpy( &pBuffer[0], &m_Buffer[m_Head], rightLen ) ;
			memcpy( &pBuffer[rightLen], &m_Buffer[0], len-rightLen ) ;
		}
	}
	BOOL bRet = TRUE;
	if(NULL!=m_pEncoder && NULL!=m_pKey && 0<m_nKeySize)
	{
		Encoder_T::Parameters_T params;

		params.SetPlainText((UCHAR*)buf);
		params.SetPlainTextSize(len);
		params.SetCripher((UCHAR*)pBuffer);
		params.SetCripherSize(len);
		params.SetKey(m_pKey);
		params.SetKeySize(m_nKeySize);
		params.SetParam0(m_nEncoderParam0);
		params.SetParam1(m_nEncoderParam1);

		bRet = m_pEncoder->Decode(params);
	}
	else
	{
		memcpy( buf, pBuffer, len ) ;
	}
	SAFE_DELETE_ARRAY(pBuffer);
	return bRet ;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}

BOOL SocketInputStream::Skip( UINT len ) 
{
__ENTER_FUNCTION_FOXNET
		
	if( len == 0 )
		return FALSE ;

	if( len>Length( ) )
		return FALSE ;
	
	m_Head = (m_Head+len)%m_BufferLen ;

	BOOL bRet = TRUE;	
	if(NULL!=m_pEncoder && NULL!=m_pKey && 0<m_nKeySize)
	{
		Encoder_T::Parameters_T params;
		params.SetPlainText(NULL);
		params.SetPlainTextSize(0);
		params.SetCripher(NULL);
		params.SetCripherSize(0);
		params.SetKey(m_pKey);
		params.SetKeySize(m_nKeySize);
		params.SetParam0(m_nEncoderParam0);
		params.SetParam1(m_nEncoderParam1);

		bRet = m_pEncoder->Skip(params, len);

		m_nEncoderParam0 = params.GetParam0();
		m_nEncoderParam1 = params.GetParam1();
	}

	return bRet;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}
#endif

BOOL SocketInputStream::ReadPacket( Packet* pPacket ) 
{
__ENTER_FUNCTION_FOXNET

	BOOL ret ;
		
	ret = Skip( PACKET_HEADER_SIZE ) ;
	if( !ret )
		return FALSE ;

	return pPacket->Read( *this ) ;
	
__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}

VOID SocketInputStream::Initsize( )
{
	m_Head = 0 ;
	m_Tail = 0 ;

	SAFE_DELETE_ARRAY( m_Buffer ) ;
		
	m_Buffer = new CHAR[DEFAULTSOCKETINPUTBUFFERSIZE] ;
//	Assert(m_Buffer) ;
		
	m_BufferLen = DEFAULTSOCKETINPUTBUFFERSIZE ;
		
	memset( m_Buffer, 0, m_BufferLen ) ;
}
	
UINT SocketInputStream::Fill( ) 
{
__ENTER_FUNCTION_FOXNET

	UINT nFilled = 0 ;
	UINT nReceived = 0 ;
	UINT nFree = 0 ;

	if ( m_Head <= m_Tail ) 
	{
		if ( m_Head == 0 ) 
		{
			//
			// H   T		LEN=10
			// 0123456789
			// abcd......
			//

			nReceived = 0 ;
			nFree = m_BufferLen-m_Tail-1 ;
			if( nFree != 0 )
			{
				nReceived = m_pSocket->receive( &m_Buffer[m_Tail] , nFree );
				if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-1 ;
				if (nReceived==0) return SOCKET_ERROR-2 ;

				m_Tail += nReceived;
				nFilled += nReceived;
			}

			if( nReceived == nFree ) 
			{
				UINT available = m_pSocket->available();
				if ( available > 0 ) 
				{
					if( (m_BufferLen+available+1)>m_MaxBufferLen )
					{
						Initsize( ) ;
						return SOCKET_ERROR-3 ;
					}
					if( !Resize( available+1 ) )
						return 0 ;

					nReceived = m_pSocket->receive( &m_Buffer[m_Tail] , available );
					if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
					if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-4 ;
					if (nReceived==0) return SOCKET_ERROR-5;

					m_Tail += nReceived;
					nFilled += nReceived;
				}
			}
		} 
		else 
		{
			//
			//    H   T		LEN=10
			// 0123456789
			// ...abcd...
			//

			nFree = m_BufferLen-m_Tail ;
			nReceived = m_pSocket->receive( &m_Buffer[m_Tail], nFree );
			if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
			if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-6 ;
			if( nReceived==0 ) return SOCKET_ERROR-7 ;

			m_Tail = (m_Tail+nReceived)%m_BufferLen ;
			nFilled += nReceived ;

			if( nReceived==nFree ) 
			{
//				Assert( m_Tail == 0 );

				nReceived = 0 ;
				nFree = m_Head-1 ;
				if( nFree!=0 )
				{
					nReceived = m_pSocket->receive( &m_Buffer[0] , nFree );
					if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
					if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR -8;
					if( nReceived==0 ) return SOCKET_ERROR-9 ;

					m_Tail += nReceived;
					nFilled += nReceived;
				}

				if( nReceived==nFree ) 
				{
					UINT available = m_pSocket->available();
					if ( available > 0 ) 
					{
						if( (m_BufferLen+available+1)>m_MaxBufferLen )
						{
							Initsize( ) ;
							return SOCKET_ERROR-10 ;
						}
						if( !Resize( available+1 ) )
							return 0 ;

						nReceived = m_pSocket->receive( &m_Buffer[m_Tail] , available );
						if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
						if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-11 ;
						if (nReceived==0) return SOCKET_ERROR-12;

						m_Tail += nReceived;
						nFilled += nReceived;
					}
				}
			}
		}

	} 
	else 
	{	
		//
        //     T  H		LEN=10
        // 0123456789
        // abcd...efg
        //

		nReceived = 0 ;
		nFree = m_Head-m_Tail-1 ;
		if( nFree!=0 )
		{
			nReceived = m_pSocket->receive( &m_Buffer[m_Tail], nFree ) ;
			if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
			if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-13 ;
			if( nReceived==0 ) return SOCKET_ERROR-14 ;

			m_Tail += nReceived ;
			nFilled += nReceived ;
		}
		if( nReceived==nFree ) 
		{
			UINT available = m_pSocket->available( ) ;
			if ( available>0 ) 
			{
				if( (m_BufferLen+available+1)>m_MaxBufferLen )
				{
					Initsize( ) ;
					return SOCKET_ERROR-15 ;
				}
				if( !Resize( available+1 ) )
					return 0 ;

				nReceived = m_pSocket->receive( &m_Buffer[m_Tail], available ) ;
				if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
				if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-16 ;
				if( nReceived==0 ) return SOCKET_ERROR-17 ;

				m_Tail += nReceived ;
				nFilled += nReceived ;
			}
		}
	}

	return nFilled ;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}

BOOL SocketInputStream::Resize( INT size )
{
__ENTER_FUNCTION_FOXNET
		
//	Assert( size != 0 ) ;
	
	size = max(size, (int)(m_BufferLen>>1));
	UINT newBufferLen = m_BufferLen + size;
	UINT len = Length();
	
	if ( size < 0 ) 
	{
		if ( newBufferLen < 0 || newBufferLen < len )
			return FALSE ;		
	} 
	
	CHAR * newBuffer = new CHAR[ newBufferLen ];
//	Assert( newBuffer ) ;
		
	if ( m_Head < m_Tail ) 
	{
		memcpy( newBuffer , &m_Buffer[m_Head] , m_Tail - m_Head );
	} 
	else if ( m_Head > m_Tail ) 
	{
		memcpy( newBuffer , &m_Buffer[m_Head] , m_BufferLen - m_Head );
		memcpy( &newBuffer[ m_BufferLen - m_Head ] , m_Buffer , m_Tail );
	}
		
	delete [] m_Buffer ;
		
	m_Buffer = newBuffer ;
	m_BufferLen = newBufferLen ;
	m_Head = 0 ;
	m_Tail = len ;

	return TRUE ;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}

VOID SocketInputStream::CleanUp( )
{
__ENTER_FUNCTION_FOXNET

	m_Head = 0 ;
	m_Tail = 0 ;
	m_nEncoderID = INVALID_ID;
	m_nEncoderParam0 = 0;
	m_nEncoderParam1 = 0;
	m_pKey = NULL;
	m_pEncoder = NULL;

__LEAVE_FUNCTION_FOXNET
}

VOID SocketInputStream::SetEncoderID(INT nID)
{
__ENTER_FUNCTION_FOXNET
	m_pEncoder = Encoder_GetInstanceByID(nID);
	if(NULL==m_pEncoder)
	{
		throw("Unknown encoder ID found!!");
	}
__LEAVE_FUNCTION_FOXNET	
}

#if 0
UINT SocketInputStream::Write( const CHAR* buf, UINT len ) 
{
__ENTER_FUNCTION_FOXNET

		//					//
        //     T  H			//    H   T			LEN=10
        // 0123456789		// 0123456789
        // abcd...efg		// ...abcd...
        //					//
			
	UINT nFree = ( (m_Head<=m_Tail)?(m_BufferLen-m_Tail+m_Head-1):(m_Head-m_Tail-1) ) ;

	if( len>=nFree )
	{
		if( !Resize( len-nFree+1 ) )
			return 0 ;
	}
		
	if( m_Head<=m_Tail ) 
	{	
		if( m_Head==0 ) 
		{
			nFree = m_BufferLen - m_Tail - 1;
			memcpy( &m_Buffer[m_Tail], buf, len ) ;
		} 
		else 
		{
			nFree = m_BufferLen-m_Tail ;
			if( len<=nFree )
			{
				memcpy( &m_Buffer[m_Tail], buf, len ) ;
			}
			else 
			{
				memcpy( &m_Buffer[m_Tail], buf, nFree ) ;
				memcpy( m_Buffer, &buf[nFree], len-nFree ) ;
			}
		}
	} 
	else 
	{	
		memcpy( &m_Buffer[m_Tail], buf, len ) ;
	}
	
	m_Tail = (m_Tail+len)%m_BufferLen ;
		
	return len;
	
__LEAVE_FUNCTION_FOXNET

	return 0 ;
}
#endif

