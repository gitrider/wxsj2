
//
//文件名称：	SocketBillingInputStream.h
//开发者：		Jack
//功能描述：	计费系统消息数据的接收缓存，提供数据的接收和格式化读取功能
//				
//修改情况：	2006-5-24 Ver 1.0.0 完成基本功能
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

