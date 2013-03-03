


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
	//外部调用通用接口

	//初始化接口
	BOOL				Init( ) ;
	//根据消息类型从内存里分配消息实体数据（允许多线程同时调用）
	Packet*				CreatePacket( PacketID_t packetID ) ;
	//根据消息类型取得对应消息的最大尺寸（允许多线程同时调用）
	UINT				GetPacketMaxSize( PacketID_t packetID ) ;
	//删除消息实体（允许多线程同时调用）
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
	//外部调用通用接口

	//初始化接口
	BOOL				Init( ) ;
	//根据消息类型从内存里分配消息实体数据（允许多线程同时调用）
	BillPacket*			CreatePacket( BPacketID_t packetID ) ;
	//根据消息类型取得对应消息的最大尺寸（允许多线程同时调用）
	UINT				GetPacketMaxSize( BPacketID_t packetID ) ;
	//删除消息实体（允许多线程同时调用）
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
