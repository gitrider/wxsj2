
//
//�ļ����ƣ�	SocketBillingInputStream.h
//�����ߣ�		Jack
//����������	�Ʒ�ϵͳ��Ϣ���ݵĽ��ջ��棬�ṩ���ݵĽ��պ͸�ʽ����ȡ����
//				
//�޸������	2006-5-24 Ver 1.0.0 ��ɻ�������
//				
//
//

#ifndef __SOCKETBILLINGINPUTSTREAM_H__
#define __SOCKETBILLINGINPUTSTREAM_H__

#include "BaseType.h"
#include "Socket.h"
#include "SocketInputStream.h"

class BillPacket;

class SocketBillingInputStream : public SocketInputStream
{
public :
	SocketBillingInputStream( Socket* sock, 
					   UINT BufferSize = DEFAULTSOCKETINPUTBUFFERSIZE,
					   UINT MaxBufferSize = DISCONNECTSOCKETINPUTSIZE ) ;


public :
	BOOL						ReadPacket( BillPacket* pPacket ) ;

};




#endif

