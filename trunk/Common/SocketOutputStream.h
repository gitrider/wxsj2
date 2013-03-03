
//
//文件名称：	SocketOutputStream.h
//开发者：		Jack
//功能描述：	消息数据的输出缓存，提供数据的格式化写入和发送功能
//				
//修改情况：	2005-03-22 Ver 1.0.0 完成基本功能
//				
//
//

#ifndef __SOCKETOUTPUTSTREAM_H__
#define __SOCKETOUTPUTSTREAM_H__

#include "BaseType.h"
#include "Socket.h"
#include "Encoder.h"

using namespace Encode_Decode;

//初始化的发送缓存长度
#define DEFAULTSOCKETOUTPUTBUFFERSIZE 8192
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETOUTPUTSIZE 100*1024


class Packet ;
class SocketOutputStream 
{
public :
	SocketOutputStream( Socket* sock, 
						UINT BufferSize = DEFAULTSOCKETOUTPUTBUFFERSIZE,
						UINT MaxBufferSize = DISCONNECTSOCKETOUTPUTSIZE ) ;
	virtual ~SocketOutputStream( ) ;


public :
	UINT	Write( const CHAR* buf, UINT len ) ;
	BOOL	WritePacket( const Packet* pPacket ) ;

	UINT	Flush( ) ;

	VOID	Initsize( ) ;
	BOOL	Resize( INT size ) ;

	INT		Capacity ()const { return m_BufferLen ; }
 
    UINT	Length( )const ;
    UINT	Size( )const { return Length( ) ; }

	CHAR*	GetBuffer( )const { return m_Buffer ; }
 
    BOOL	IsEmpty ()const { return m_Head==m_Tail ; }

	VOID	CleanUp( ) ;

	VOID	SetEncoderID(INT nID);
	INT 	GetEncoderID(VOID) const{return m_nEncoderID;};

	VOID	SetKey(UCHAR const* pKey) {m_pKey = pKey;};
	UCHAR const* GetKey(VOID) const{return m_pKey;};

	VOID	SetKeySize(INT nSize) {m_nKeySize = nSize;};
	INT 	GetKeySize(VOID) const{return m_nKeySize;};

	VOID	SetEncoderParam0(INT nParam) {m_nEncoderParam0 = nParam;};
	INT		GetEncoderParam0(VOID) const{return m_nEncoderParam0;};

	VOID	SetEncoderParam1(INT nParam) {m_nEncoderParam1 = nParam;};
	INT		GetEncoderParam1(VOID) const{return m_nEncoderParam1;};
protected :
	
	Socket*	m_pSocket ;
	
	CHAR*	m_Buffer ;
	
	UINT	m_BufferLen ;
	UINT	m_MaxBufferLen ;
	
	UINT	m_Head ;
	UINT	m_Tail ;

	INT 	m_nEncoderID;
	Encoder_T const* m_pEncoder;
	UCHAR const* m_pKey;
	INT 	m_nKeySize;
	INT		m_nEncoderParam0;
	INT		m_nEncoderParam1;
};



#endif
