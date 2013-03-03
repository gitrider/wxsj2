

#ifndef __PACKET_H__
#define __PACKET_H__

#include "BaseType.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "SocketBillingInputStream.h"
#include "SocketBillingOutputStream.h"

class Socket;
class Player;

#define GET_PACKET_INDEX(a) ((a)>>24)
#define SET_PACKET_INDEX(a,index) ((a)=(((a)&0xffffff)+((index)<<24)))
#define GET_PACKET_LEN(a) ((a)&0xffffff)
#define SET_PACKET_LEN(a,len) ((a)=((a)&0xff000000)+(len))
//��Ϣͷ�а�����PacketID_t-2�ֽڣ�UINT-4�ֽ��и�λһ���ֽ�Ϊ��Ϣ���кţ�����
//�����ֽ�Ϊ��Ϣ����
//ͨ��GET_PACKET_INDEX��GET_PACKET_LEN�꣬����ȡ��UINT�����������Ϣ���кźͳ���
//ͨ��SET_PACKET_INDEX��SET_PACKET_LEN�꣬��������UINT�����������Ϣ���кźͳ���
#define PACKET_HEADER_SIZE (sizeof(PacketID_t)+sizeof(UINT))




//Packet::Execute(...) �ķ���ֵ
enum PACKET_EXE
{
	PACKET_EXE_ERROR = 0 ,
	PACKET_EXE_BREAK ,
	PACKET_EXE_CONTINUE ,
	PACKET_EXE_NOTREMOVE ,
	PACKET_EXE_NOTREMOVE_ERROR ,
};

class Packet
{
public :
	BYTE			m_Index ;
	BYTE			m_Status ;

public :
	Packet( ) ;
	virtual ~Packet( ) ;

	virtual VOID	CleanUp( ){} ;

	virtual BOOL	Read( SocketInputStream& iStream ) = 0 ;
	
	virtual BOOL	Write( SocketOutputStream& oStream ) const = 0;
	
	//����ֵΪ��PACKET_EXE �е����ݣ�
	//PACKET_EXE_ERROR ��ʾ�������ش��󣬵�ǰ������Ҫ��ǿ�ƶϿ�
	//PACKET_EXE_BREAK ��ʾ���غ�ʣ�µ���Ϣ�����ڵ�ǰ����ѭ���ﴦ��
	//PACKET_EXE_CONTINUE ��ʾ�����ڵ�ǰѭ����ִ��ʣ�µ���Ϣ
	//PACKET_EXE_NOTREMOVE ��ʾ�����ڵ�ǰѭ����ִ��ʣ�µ���Ϣ,���ǲ����յ�ǰ��Ϣ
	virtual UINT		Execute( Player* pPlayer ) = 0 ;
	
	virtual	PacketID_t	GetPacketID( ) const = 0 ;
	
	virtual	UINT		GetPacketSize( ) const = 0 ;

	virtual BOOL		CheckPacket( ){ return TRUE ; }

	BYTE				GetPacketIndex( ) const { return m_Index ; } ;
	VOID				SetPacketIndex( BYTE Index ){ m_Index = Index ; } ;

	BYTE				GetPacketStatus( ) const { return m_Status ; } ;
	VOID				SetPacketStatus( BYTE Status ){ m_Status = Status ; } ;
};
//Login ��Billing ֮�����Ϣ��ʽ
#define BILLING_PACKET_HEADER_SIZE (sizeof(USHORT)+sizeof(USHORT)+sizeof(BYTE))
#define BILLING_PACKET_TAIL_SIZE (sizeof(USHORT))

class BillPacket
{
public :
	USHORT			m_Index ;
	BYTE			m_Status ;

public :
	BillPacket( ) {};
	virtual ~BillPacket( ){} ;

	virtual VOID		CleanUp( ){} ;

	virtual BOOL		Read( SocketBillingInputStream& iStream ) = 0 ;

	virtual BOOL		Write( SocketBillingOutputStream& oStream ) const = 0;

	//����ֵΪ��PACKET_EXE �е����ݣ�
	//PACKET_EXE_ERROR ��ʾ�������ش��󣬵�ǰ������Ҫ��ǿ�ƶϿ�
	//PACKET_EXE_BREAK ��ʾ���غ�ʣ�µ���Ϣ�����ڵ�ǰ����ѭ���ﴦ��
	//PACKET_EXE_CONTINUE ��ʾ�����ڵ�ǰѭ����ִ��ʣ�µ���Ϣ
	//PACKET_EXE_NOTREMOVE ��ʾ�����ڵ�ǰѭ����ִ��ʣ�µ���Ϣ,���ǲ����յ�ǰ��Ϣ
	virtual UINT		Execute( Player* pPlayer ) = 0 ;

	virtual	BPacketID_t	GetPacketID( ) const = 0 ;

	virtual	UINT		GetPacketSize( ) const = 0 ;

	virtual BOOL		CheckPacket( ){ return TRUE ; }

	USHORT				GetPacketIndex( ) const { return m_Index ; } ;
	VOID				SetPacketIndex( USHORT Index ){ m_Index = Index ; } ;

	BYTE				GetPacketStatus( ) const { return m_Status ; } ;
	VOID				SetPacketStatus( BYTE Status ){ m_Status = Status ; } ;
};






#endif
