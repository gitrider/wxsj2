
#include "SocketBillingOutputStream.h"
#include "Packet.h"


SocketBillingOutputStream::SocketBillingOutputStream( Socket* sock, UINT BufferSize, UINT MaxBufferSize )
: SocketOutputStream( sock, BufferSize, MaxBufferSize )
{
__ENTER_FUNCTION_FOXNET


__LEAVE_FUNCTION_FOXNET
}

BOOL SocketBillingOutputStream::WritePacket( const BillPacket* pPacket )
{
__ENTER_FUNCTION_FOXNET


	
/*
		struct  LB
				{
						BYTE	h1;
						CHAR	h2;
						USHORT	size;
						BYTE	ID;
						USHORT  Index;
						USHORT	Group;
						BYTE	length;
						CHAR	mIP[11];
						CHAR	t1;
						BYTE	t2;
		
						LB()
						{
							memset(this,0,sizeof(*this));
						}
				};
			
				LB  lb;
			
				lb.h1		=  0xAA;
				lb.h2		=  0x55;
				lb.ID		=  0xA0;
				lb.Index	=   htons(0);
				lb.Group	=	htons(0);
				lb.length	=  11;
				strncpy(lb.mIP,"10.1.164.45",11);
				lb.t1		=  0x55;
				lb.t2		=  0xAA;
				lb.size		=  htons(17);
			
				UINT w = Write((CHAR*)&lb.h1,sizeof(BYTE));
				w = Write((CHAR*)&lb.h2,sizeof(CHAR));
				w = Write((CHAR*)&lb.size,sizeof(USHORT));
				w = Write((CHAR*)&lb.ID,sizeof(BYTE));
				w = Write((CHAR*)&lb.Index,sizeof(USHORT));
				w = Write((CHAR*)&lb.Group,sizeof(USHORT));
				w = Write((CHAR*)&lb.length,sizeof(BYTE));
				w = Write((CHAR*)lb.mIP,sizeof(BYTE)*11);
				w = Write((CHAR*)&lb.t1,sizeof(BYTE));
				w = Write((CHAR*)&lb.t2,sizeof(BYTE));*/

	
			    USHORT nHead = 0x55AA ;
				UINT w = Write( (CHAR*)&nHead, sizeof(USHORT) ) ;
				
				USHORT packetSize = (USHORT)(pPacket->GetPacketSize( ));
				packetSize += 3 ;
				packetSize = htons(packetSize);
				w = Write( (CHAR*)&packetSize, sizeof(USHORT) ) ;
				
					
				PacketID_t packetID = pPacket->GetPacketID() ;
				BYTE bPacketID = (BYTE)packetID ;
				w = Write( (CHAR*)&bPacketID , sizeof(BYTE) ) ;
				USHORT PacketIndex = htons(pPacket->GetPacketIndex());	
				w = Write((CHAR*)&PacketIndex,sizeof(USHORT));
				pPacket->Write( *this ) ;
				USHORT nTail = 0xAA55 ;
				w = Write( (CHAR*)&nTail, sizeof(USHORT) ) ;
				
		
	

	return TRUE ;

__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}

