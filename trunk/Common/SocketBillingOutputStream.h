
//
//�ļ����ƣ�	SocketBillingOutputStream.h
//�����ߣ�		Jack
//����������	�Ʒ�ϵͳ��Ϣ���ݵ�������棬�ṩ���ݵĸ�ʽ��д��ͷ��͹���
//				
//�޸������	2006-5-24 Ver 1.0.0 ��ɻ�������
//				
//
//

#ifndef __SOCKETBILLINGOUTPUTSTREAM_H__
#define __SOCKETBILLINGOUTPUTSTREAM_H__

#include "BaseType.h"
#include "Socket.h"
#include "SocketOutputStream.h"

class BillPacket;

class SocketBillingOutputStream : public SocketOutputStream
{
public :
	SocketBillingOutputStream( Socket* sock, 
						UINT BufferSize = DEFAULTSOCKETOUTPUTBUFFERSIZE,
						UINT MaxBufferSize = DISCONNECTSOCKETOUTPUTSIZE ) ;


public :
	BOOL						WritePacket( const BillPacket* pPacket ) ;


};




#endif

