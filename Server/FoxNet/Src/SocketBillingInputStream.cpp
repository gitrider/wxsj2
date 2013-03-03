
#include "SocketBillingInputStream.h"
#include "Packet.h"


SocketBillingInputStream::SocketBillingInputStream( Socket* sock, UINT BufferLen, UINT MaxBufferLen ) 
: SocketInputStream( sock, BufferLen, MaxBufferLen )
{
__ENTER_FUNCTION_FOXNET


__LEAVE_FUNCTION_FOXNET
}

BOOL SocketBillingInputStream::ReadPacket( BillPacket* pPacket )
{
__ENTER_FUNCTION_FOXNET

	BOOL ret ;
		
	ret = Skip( BILLING_PACKET_HEADER_SIZE+sizeof(USHORT) );
	if( !ret )
		return FALSE ;

	ret = pPacket->Read( *this ) ;
	if(!ret)
		return FALSE;

	ret = Skip(BILLING_PACKET_TAIL_SIZE);

	return TRUE ;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}

