

#ifndef __GCCONNECT_H__
#define __GCCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{


class GCConnect : public Packet 
{
public:
	GCConnect( ){} ;
	virtual ~GCConnect( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_CONNECT ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ID_t)+
															sizeof(SceneID_t)+
															sizeof(WORLD_POS) ; }
	
public:
	//ʹ�����ݽӿ�

	ID_t					GetServerID( ){ return m_ServerID ; } ;
	VOID					SetServerID( ID_t id ){ m_ServerID = id ; } ;

	SceneID_t				GetSceneID( ){ return m_SceneID ; } ;
	VOID					SetSceneID( SceneID_t id ){ m_SceneID = id ; } ;

	WORLD_POS*				GetWorldPos( ){ return &m_Position ; } ;
	VOID					SetWorldPos( const	WORLD_POS* pos ){ m_Position = *pos ; } ;

private:
	//����
	ID_t					m_ServerID ;

//�������������
	SceneID_t				m_SceneID ;
	WORLD_POS				m_Position ;
//��������

};


class GCConnectFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCConnect() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_CONNECT ; }
	UINT		GetPacketMaxSize()const { return	sizeof(ID_t)+
													sizeof(SceneID_t)+
													sizeof(WORLD_POS) ; }
};


class GCConnectHandler 
{
public:
	static UINT Execute( GCConnect* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;


#endif
