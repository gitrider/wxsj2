


#ifndef __PACKETFACTORYMANAGER_H__
#define __PACKETFACTORYMANAGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


class PacketFactoryManager 
{
public :
	
	PacketFactoryManager( ) ;
	
	~PacketFactoryManager( ) ;

private :
	VOID				AddFactory( PacketFactory* pFactory ) ;
	
public :
	//�ⲿ����ͨ�ýӿ�

	//��ʼ���ӿ�
	BOOL				Init( ) ;
	//������Ϣ���ʹ��ڴ��������Ϣʵ�����ݣ�������߳�ͬʱ���ã�
	Packet*				CreatePacket( PacketID_t packetID ) ;
	//������Ϣ����ȡ�ö�Ӧ��Ϣ�����ߴ磨������߳�ͬʱ���ã�
	UINT				GetPacketMaxSize( PacketID_t packetID ) ;
	//ɾ����Ϣʵ�壨������߳�ͬʱ���ã�
	VOID				RemovePacket( Packet* pPacket ) ;
	
	VOID				Lock( ){ m_Lock.Lock() ; } ;
	VOID				Unlock( ){ m_Lock.Unlock() ; } ;

private :
	
	PacketFactory **	m_Factories ;
	
	USHORT				m_Size ;

	MyLock				m_Lock ;

public :
	UINT*				m_pPacketAllocCount ;

};

extern PacketFactoryManager* g_pPacketFactoryManager ;

#if defined (_FOX_LOGIN)  && defined (_FOX_BILLING)

class BillPacketFactoryManager 
{
public :

	BillPacketFactoryManager( ) ;

	~BillPacketFactoryManager( ) ;

private :
	VOID				AddFactory( BillPacketFactory* pFactory ) ;

public :
	//�ⲿ����ͨ�ýӿ�

	//��ʼ���ӿ�
	BOOL				Init( ) ;
	//������Ϣ���ʹ��ڴ��������Ϣʵ�����ݣ�������߳�ͬʱ���ã�
	BillPacket*			CreatePacket( BPacketID_t packetID ) ;
	//������Ϣ����ȡ�ö�Ӧ��Ϣ�����ߴ磨������߳�ͬʱ���ã�
	UINT				GetPacketMaxSize( BPacketID_t packetID ) ;
	//ɾ����Ϣʵ�壨������߳�ͬʱ���ã�
	VOID				RemovePacket( BillPacket* pPacket ) ;

	VOID				Lock( ){ m_Lock.Lock() ; } ;
	VOID				Unlock( ){ m_Lock.Unlock() ; } ;

private :

	BillPacketFactory **	m_Factories ;

	USHORT				m_Size ;

	MyLock				m_Lock ;

public :
	UINT*				m_pPacketAllocCount ;

};

extern BillPacketFactoryManager* g_pBPacketFactoryManager ;

#endif






#endif
