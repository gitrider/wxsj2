
#ifndef __GCReturnOnlineUser_H__
#define __GCReturnOnlineUser_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Query.h"

namespace Packets
{

class GCReturnOnlineUser : public Packet
{
public:
	GCReturnOnlineUser( ){}
	virtual ~GCReturnOnlineUser( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_RETURNONLINEUSER; }
	virtual UINT			GetPacketSize()const { 
		return sizeof(PlayerID_t) + m_ReturnOnlineUsers.GetSize()
			+ sizeof(m_bContinue) + sizeof(INT);
	}

public:
	PlayerID_t			GetPlayerID(VOID) {return m_PlayerID;}
	VOID				SetPlayerID(PlayerID_t id) {m_PlayerID = id;}
	//ʹ�����ݽӿ�
	const OnlineUser_SameCity_PerPage& GetResults() const {return m_ReturnOnlineUsers;}
	VOID				SetResults(const OnlineUser_SameCity_PerPage& value){
		m_ReturnOnlineUsers = value;
	}

	VOID SetContinue(BOOL value) { m_bContinue = value; }
	BOOL GetContinue() const { return m_bContinue; }

	VOID SetReturnPostion(INT value) {m_iReturnPostion = value;}
	INT GetReturnPostion() const {return m_iReturnPostion;}

private:
	//����
	PlayerID_t			m_PlayerID;
	BOOL				m_bContinue;   // �Ƿ�����Ϣ
	OnlineUser_SameCity_PerPage m_ReturnOnlineUsers;
	INT					m_iReturnPostion; //��ѯ����Users�е����Index
};


class GCReturnOnlineUserFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCReturnOnlineUser() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_RETURNONLINEUSER; }
	UINT		GetPacketMaxSize()const {
		return sizeof(PlayerID_t) + sizeof(OnlineUser_SameCity_PerPage)
			+ sizeof(BOOL) + sizeof(INT);
	}

};

class GCReturnOnlineUserHandler 
{
public:
	static UINT Execute( GCReturnOnlineUser* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif