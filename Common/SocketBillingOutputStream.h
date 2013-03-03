
//
//文件名称：	SocketBillingOutputStream.h
//开发者：		Jack
//功能描述：	计费系统消息数据的输出缓存，提供数据的格式化写入和发送功能
//				
//修改情况：	2006-5-24 Ver 1.0.0 完成基本功能
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

