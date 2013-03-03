

#include "SocketOutputStream.h"
#include "Packet.h"
#include "EncoderManager.h"

using namespace Encode_Decode;

SocketOutputStream::SocketOutputStream( Socket* sock, UINT BufferLen, UINT MaxBufferLen ) 
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

SocketOutputStream::~SocketOutputStream( ) 
{
__ENTER_FUNCTION_FOXNET
		
	SAFE_DELETE_ARRAY(m_Buffer);
		
__LEAVE_FUNCTION_FOXNET
}

UINT SocketOutputStream::Length( )const
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

#define _NOT_USE_ENCODE_
#ifdef _NOT_USE_ENCODE_
UINT SocketOutputStream::Write( const CHAR* buf, UINT len )
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
#else
UINT SocketOutputStream::Write( const CHAR* buf, UINT len ) 
{
__ENTER_FUNCTION_FOXNET

		//					//
        //     T  H			//    H   T			LEN=10
        // 0123456789		// 0123456789
        // abcd...efg		// ...abcd...
        //					//
	UCHAR* pBuffer = NULL;
			
	UINT nFree = ( (m_Head<=m_Tail)?(m_BufferLen-m_Tail+m_Head-1):(m_Head-m_Tail-1) ) ;
	if( len>=nFree )
	{
		if( !Resize( len-nFree+1 ) )
			return 0 ;
	}
	
	pBuffer = new UCHAR[len];
	if(NULL==pBuffer)
	{
		return 0;
	}
	if(NULL!=m_pEncoder && NULL!=m_pKey && 0<m_nKeySize)
	{
		Encoder_T::Parameters_T params;
		params.SetPlainText((UCHAR*)(buf));
		params.SetPlainTextSize(len);
		params.SetCripher(pBuffer);
		params.SetCripherSize(len);
		params.SetKey(m_pKey);
		params.SetKeySize(m_nKeySize);
		params.SetParam0(m_nEncoderParam0);
		params.SetParam1(m_nEncoderParam1);
		BOOL bRet = m_pEncoder->Encode(params);
		m_nEncoderParam0 = params.GetParam0();
		m_nEncoderParam1 = params.GetParam1();
		
		if(FALSE==bRet)
		{
			SAFE_DELETE_ARRAY(pBuffer);
			return 0;
		}
	}
	else
	{
		memcpy(pBuffer, buf, len);
	}
	if( m_Head<=m_Tail ) 
	{	
		if( m_Head==0 ) 
		{
			nFree = m_BufferLen - m_Tail - 1;
			memcpy( &m_Buffer[m_Tail], pBuffer, len ) ;
		} 
		else 
		{
			nFree = m_BufferLen-m_Tail ;
			if( len<=nFree )
			{
				memcpy( &m_Buffer[m_Tail], pBuffer, len ) ;
			}
			else 
			{
				memcpy( &m_Buffer[m_Tail], pBuffer, nFree ) ;
				memcpy( m_Buffer, &pBuffer[nFree], len-nFree ) ;
			}
		}
	} 
	else 
	{	
		memcpy( &m_Buffer[m_Tail], pBuffer, len ) ;
	}
	
	m_Tail = (m_Tail+len)%m_BufferLen ;

	SAFE_DELETE_ARRAY(pBuffer);
	return len;
	
__LEAVE_FUNCTION_FOXNET

	return 0 ;
}
#endif

BOOL SocketOutputStream::WritePacket( const Packet* pPacket )
{
__ENTER_FUNCTION_FOXNET
		
	PacketID_t packetID = pPacket->GetPacketID() ;
	UINT w = Write( (CHAR*)&packetID , sizeof(PacketID_t) ) ;
//	Assert( w!=0 ) ;

	UINT packetUINT ;
	
	UINT packetSize = pPacket->GetPacketSize( ) ;
	UINT packetIndex = pPacket->GetPacketIndex( ) ;

	SET_PACKET_INDEX(packetUINT, packetIndex) ;
	SET_PACKET_LEN(packetUINT, packetSize) ;

	w = Write( (CHAR*)&packetUINT, sizeof(UINT) ) ;
//	Assert( w!=0 ) ;
	
	return pPacket->Write( *this ) ;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}

VOID SocketOutputStream::Initsize( )
{
	m_Head = 0 ;
	m_Tail = 0 ;

	SAFE_DELETE_ARRAY( m_Buffer ) ;
		
	m_Buffer = new CHAR[DEFAULTSOCKETOUTPUTBUFFERSIZE] ;
//	Assert(m_Buffer) ;
		
	m_BufferLen = DEFAULTSOCKETOUTPUTBUFFERSIZE ;
		
	memset( m_Buffer, 0, m_BufferLen ) ;
}

UINT SocketOutputStream::Flush( ) 
{
__ENTER_FUNCTION_FOXNET

//	Assert( m_pSocket != NULL );

	UINT nFlushed = 0;
	UINT nSent = 0;
	UINT nLeft;

	if( m_BufferLen>m_MaxBufferLen )
	{//��������ͻ��˵Ļ���̫�����������û��棬�����˿ͻ��˶Ͽ�����
		Initsize( ) ;
		
		return SOCKET_ERROR-1 ;
	}

#if defined(__WINDOWS__)
	UINT flag = MSG_DONTROUTE ;
#elif defined(__LINUX__)
	UINT flag = MSG_NOSIGNAL ;
#endif
	
	_MY_TRY 
	{
		if ( m_Head < m_Tail ) 
		{
			nLeft = m_Tail - m_Head;
	
			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , flag );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-2 ;
				if (nSent==0) return 0;
				
				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}

//			Assert( nLeft == 0 );
	
		} 
		else if( m_Head > m_Tail ) 
		{
			nLeft = m_BufferLen - m_Head;
	
			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , flag );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-3 ;
				if (nSent==0) return 0;
				
				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
			
//			Assert( m_Head == m_BufferLen );
			
			m_Head = 0;
			
			nLeft = m_Tail;
	
			while ( nLeft > 0 ) 
			{
				nSent = m_pSocket->send( &m_Buffer[m_Head] , nLeft , flag );
				if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return SOCKET_ERROR-4 ;
				if (nSent==0) return 0;
				
				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
	
//			Assert( nLeft == 0 );
		}
	
		if ( m_Head != m_Tail ) 
		{
//			Assert( m_Head == m_Tail );
		}
		
	}
	_MY_CATCH
	{
		if( nSent>0 )
		{
			m_Head += nSent;
		}
	} 

	m_Head = m_Tail = 0 ;

	return nFlushed;

__LEAVE_FUNCTION_FOXNET

	return 0 ;
}

BOOL SocketOutputStream::Resize( INT size )
{
__ENTER_FUNCTION_FOXNET
		
//	Assert( size != 0 );

	INT orgSize = size;

	size = max( size, (int)(m_BufferLen>>1) ) ;
	UINT newBufferLen = m_BufferLen+size ;
	UINT len = Length( ) ;
	
	if( size<0 ) 
	{
		if( newBufferLen<0 || newBufferLen<len )
			return FALSE ;
	} 
	
	CHAR * newBuffer = new CHAR[ newBufferLen ] ;
//	Assert( newBuffer ) ;
	if( newBuffer==NULL )
		return FALSE ;
		
	if( m_Head<m_Tail ) 
	{
		memcpy( newBuffer, &m_Buffer[m_Head], m_Tail-m_Head ) ;
	} 
	else if( m_Head>m_Tail ) 
	{
		memcpy( newBuffer, &m_Buffer[m_Head] , m_BufferLen-m_Head );
		memcpy( &newBuffer[m_BufferLen-m_Head], m_Buffer, m_Tail );
	}
		
	delete [] m_Buffer;
		
	m_Buffer = newBuffer;
	m_BufferLen = newBufferLen;
	m_Head = 0;
	m_Tail = len;	

	return TRUE ;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}

VOID SocketOutputStream::CleanUp( )
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


VOID SocketOutputStream::SetEncoderID(INT nID)
{
__ENTER_FUNCTION_FOXNET
	m_pEncoder = Encoder_GetInstanceByID(nID);
	if(NULL==m_pEncoder)
	{
		throw("Unknown encoder ID found!!");
	}
__LEAVE_FUNCTION_FOXNET	
}




