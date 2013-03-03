//
//�ļ����ƣ�	SocketInputStream.h
//�����ߣ�		Jack
//����������	��Ϣ���ݵĽ��ջ��棬�ṩ���ݵĽ��պ͸�ʽ����ȡ����
//				
//�޸������	2005-03-22 Ver 1.0.0 ��ɻ�������
//				
//
//


#ifndef __SOCKETINPUTSTREAM_H__
#define __SOCKETINPUTSTREAM_H__

#include "BaseType.h"
#include "Socket.h"
#include "Encoder.h"

using namespace Encode_Decode;

//��ʼ���Ľ��ջ��泤��
#define DEFAULTSOCKETINPUTBUFFERSIZE 64*1024
//����������Ļ��泤�ȣ������������ֵ����Ͽ�����
#define DISCONNECTSOCKETINPUTSIZE 96*1024

class Packet ;
class SocketInputStream
{
public :
	SocketInputStream( Socket* sock, 
					   UINT BufferSize = DEFAULTSOCKETINPUTBUFFERSIZE,
					   UINT MaxBufferSize = DISCONNECTSOCKETINPUTSIZE ) ;
	virtual ~SocketInputStream( ) ;


public :
#if 0
	UINT	Write( const CHAR* buf, UINT len ) ;
#endif
	UINT	Read( CHAR* buf, UINT len ) ;
	BOOL	ReadPacket( Packet* p ) ;

	BOOL	Peek( CHAR* buf, UINT len ) ;
	
	BOOL	Skip( UINT len ) ;
	
	UINT	Fill( ) ;
	UINT	Fill_RAW( ) ;

	VOID	Initsize( ) ;
	BOOL	Resize( INT size ) ;
	
	UINT	Capacity( )const { return m_BufferLen; }
	
	UINT	Length( )const ;
	UINT	Size( )const { return Length(); }

	BOOL	IsEmpty( )const { return m_Head==m_Tail; }

	VOID	CleanUp( ) ;

	VOID	SetEncoderID(INT nID);
	INT 	GetEncoderID(VOID) const{return m_nEncoderID;};

	VOID	SetKey(UCHAR const* pKey) {m_pKey = pKey;};
	UCHAR const* GetKey(VOID) const{return m_pKey;};

	VOID	SetKeySize(INT nSize) {m_nKeySize = nSize;};
	INT		GetKeySize(VOID) const{return m_nKeySize;};

	VOID	SetEncoderParam0(INT nParam) {m_nEncoderParam0 = nParam;};
	INT		GetEncoderParam0(VOID) const{return m_nEncoderParam0;};

	VOID	SetEncoderParam1(INT nParam) {m_nEncoderParam1 = nParam;};
	INT		GetEncoderParam1(VOID) const{return m_nEncoderParam1;};
private :
	Socket*	m_pSocket ;
	
	CHAR*	m_Buffer ;
	
	UINT	m_BufferLen ;
	UINT	m_MaxBufferLen ;
	
	UINT	m_Head ;
	UINT	m_Tail ;

	INT		m_nEncoderID;
	Encoder_T const* m_pEncoder;
	UCHAR const* m_pKey;
	INT 	m_nKeySize;
	INT		m_nEncoderParam0;
	INT		m_nEncoderParam1;
};




#endif
