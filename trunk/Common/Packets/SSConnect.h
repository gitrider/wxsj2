

#ifndef __SSCONNECT_H__
#define __SSCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class SSConnect : public Packet 
	{
	public:
		SSConnect( ):m_ServerID(INVALID_ID),m_ZoneID(INVALID_ID),m_WorldID(INVALID_ID){} ;
	virtual ~SSConnect( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_SS_CONNECT ; }
		virtual UINT			GetPacketSize() const { return sizeof(ID_t)+sizeof(ID_t)+sizeof(ID_t) ; }

	public:
	//ʹ�����ݽӿ�
	ID_t			GetServerID( ){ return m_ServerID ; } ;
	VOID			SetServerID( ID_t id ){ m_ServerID = id ; } ;

		ID_t			GetZoneID(){return m_ZoneID;}
		VOID			SetZoneID(ID_t id){m_ZoneID=id;}

		ID_t			GetWorldID(){return m_WorldID;}
		VOID			SetWorldID(ID_t id){m_WorldID=id;}
	private:
	//����
	ID_t			m_ServerID ;	//������Ϣ�ķ������˳����ServerID
		ID_t			m_ZoneID;		//������Ϣ�ķ������˳����ZoneID
		ID_t			m_WorldID;		//������Ϣ�ķ������˳����WorldID
	};


	class SSConnectFactory : public PacketFactory 
	{
	public:
	Packet*		CreatePacket() { return new SSConnect() ; }
	PacketID_t	GetPacketID() const { return PACKET_SS_CONNECT ; }
		UINT		GetPacketMaxSize() const { return sizeof(ID_t)+sizeof(ID_t)+sizeof(ID_t) ; }
	};


	class SSConnectHandler 
	{
	public:
	static UINT Execute( SSConnect* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
