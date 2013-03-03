
#ifndef __GWSearchOnlineUser_H__
#define __GWSearchOnlineUser_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GWSearchOnlineUser : public Packet
{
public:
	GWSearchOnlineUser( ){}
	virtual ~GWSearchOnlineUser( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GW_SEARCHONLINEUSER; }
	virtual UINT			GetPacketSize()const { return sizeof(PlayerID_t) + sizeof(INT) + sizeof(GUID_t) + sizeof(INT); }
	
public:
	//ʹ�����ݽӿ�
	INT					GetPostCode(VOID) { return m_PostCode; }
	VOID				SetPostCode(INT value) { m_PostCode = value; }
	PlayerID_t			GetPlayerID(VOID) {return m_PlayerID;}
	VOID				SetPlayerID(PlayerID_t id) {m_PlayerID = id;}
	GUID_t				GetGUID(){return m_GUID;}
	VOID				SetGUID(GUID_t guid){m_GUID = guid;}
    
	INT					GetMaxPostion(){return m_iMaxPostion;}
	VOID				SetMaxPostion(INT value){m_iMaxPostion = value;}
private:
	//����
	INT					m_PostCode;	//��������ʱ��
	PlayerID_t			m_PlayerID;
	GUID_t				m_GUID;
	INT					m_iMaxPostion; //�ϴβ�ѯʱ��Users�е����Index
};


class GWSearchOnlineUserFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWSearchOnlineUser() ; }
	PacketID_t	GetPacketID()const { return PACKET_GW_SEARCHONLINEUSER; }
	UINT		GetPacketMaxSize()const { return sizeof(PlayerID_t) + sizeof(INT) + sizeof(GUID_t) + sizeof(INT); }
};

class GWSearchOnlineUserHandler 
{
public:
	static UINT Execute( GWSearchOnlineUser* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif